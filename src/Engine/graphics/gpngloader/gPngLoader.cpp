
#include "gPngLoader.h"
#include "Utils.h"
#include <stdlib.h>
#include <malloc.h>


static void png_image_read(png_structp png_ptr, png_bytep data, png_size_t size)
{
	png_image_raw_data* self = (png_image_raw_data*)png_ptr->io_ptr;
	if (self->size < size)
		png_error(png_ptr, "Read error");
	else
	{
		size_t count = fread(data, 1, size, self->file);
		self->size -= size;
		if (count != size)
			png_error(png_ptr, "Read error");
	}
}

bool LoadPngImage(const char* aFileName, unsigned char** aImage, unsigned long* aWidth, unsigned long* aHeight)
{
	*aImage = 0;
	*aWidth = 0;
	*aHeight = 0;

	png_image_raw_data	raw;

	raw.file = fopen(aFileName, "rb");
	if (!raw.file)
		return false;

	fseek(raw.file, 0L, SEEK_END);
	raw.size = ftell(raw.file);
	fseek(raw.file, 0L, SEEK_SET);


	png_structp	pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (pngPtr == NULL) {
		fclose(raw.file);
		return false;
	}

	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (infoPtr == NULL)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(raw.file);
		return false;
	}

	png_infop endInfo = png_create_info_struct(pngPtr);
	if (endInfo == NULL)
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)NULL);
		fclose(raw.file);
		return false;
	}


	png_set_read_fn(pngPtr, &raw, png_image_read);
	png_read_info(pngPtr, infoPtr);

	int	width = png_get_image_width(pngPtr, infoPtr);
	int	height = png_get_image_height(pngPtr, infoPtr);
	int	bpp = png_get_bit_depth(pngPtr, infoPtr);
	int	colorType = png_get_color_type(pngPtr, infoPtr);
	int	rowBytes = png_get_rowbytes(pngPtr, infoPtr);
	int	numComponents = 0;

	switch (colorType)
	{
	case PNG_COLOR_TYPE_RGB:
		numComponents = 3;
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:
		numComponents = 4;
		break;

	case PNG_COLOR_TYPE_GRAY:
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		numComponents = 1;
		break;

	case PNG_COLOR_TYPE_PALETTE:
		numComponents = 3;
		break;

	default:
		break;
	}

	if (numComponents == 0)
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)NULL);
		fclose(raw.file);
		return false;
	}

	*aHeight = height;
	*aWidth = width;
	*aImage = (unsigned char*)malloc(width * height * 4 * sizeof(unsigned char));


	png_bytep*	row_ptr = new png_bytep[height];
	for (int i = 0; i < height; i++)
		row_ptr[i] = (png_bytep)malloc(rowBytes);

	unsigned int* lineBuf = new unsigned int[width];

	png_read_image(pngPtr, row_ptr);

	for (int i = 0; i < height; i++)
	{
		png_bytep	ptr = row_ptr[i];

		if (colorType == PNG_COLOR_TYPE_RGB)
		{
			for (int x = 0; x < width; x++, ptr += 3)
				lineBuf[x] = rgbaToInt(ptr[2], ptr[1], ptr[0]);
		}
		else
			if (colorType == PNG_COLOR_TYPE_RGB_ALPHA)
			{
				for (int x = 0; x < width; x++, ptr += 4)
					lineBuf[x] = rgbaToInt(ptr[2], ptr[1], ptr[0], ptr[3]);
			}
			else
				if (colorType == PNG_COLOR_TYPE_PALETTE)
				{
					png_color*	pngPal = NULL;
					png_color	pal[256];
					int			numColors;
					int			numEntries = (1 << bpp) - 1;

					if (!png_get_PLTE(pngPtr, infoPtr, (png_colorp *)&pngPal, (int*)&numColors))
					{
						numColors = numEntries + 1;
						pngPal = pal;

						for (int c = 0; c < numColors; c++)
							pngPal[c].red =
							pngPal[c].green =
							pngPal[c].blue = (png_byte)((c * 255) / numEntries);
					}
					else
					{
						for (int c = 0; c < numColors; c++)
						{
							pngPal[c].red = (png_byte)((pngPal[c].red * 255) / numEntries);
							pngPal[c].green = (png_byte)((pngPal[c].green * 255) / numEntries);
							pngPal[c].blue = (png_byte)((pngPal[c].blue * 255) / numEntries);
						}
					}

					for (int w = 0; w < width; w++)
					{
						int	ind = ptr[w];
						lineBuf[w] = rgbaToInt(pngPal[ind].blue, pngPal[ind].green, pngPal[ind].red);
					}
				}
				else
				{
					if (colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
					{
						for (int x = 0; x < width; x++, ptr++)
							lineBuf[x] = rgbaToInt(255, 255, 255, ptr[0]);
					}
					else
					{
						for (int x = 0; x < width; x++, ptr++)
							lineBuf[x] = ptr[0];
					}
				}

		int row_size = 4 * sizeof(unsigned char) * width;
		unsigned char* line_ptr = *aImage + i * row_size;
		memmove(line_ptr, lineBuf, row_size);

		free(row_ptr[i]);
	}

	delete lineBuf;
	delete row_ptr;

	png_read_end(pngPtr, endInfo);
	png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);

	if (raw.file)
		fclose(raw.file);

	return true;
}
