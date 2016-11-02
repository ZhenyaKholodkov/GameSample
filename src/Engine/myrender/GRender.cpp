#include "GRender.h"

#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C

GRender::PlatformMatrix::PlatformMatrix()
{
	SetIdentity();
}

void GRender::PlatformMatrix::SetIdentity()
{
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
}


GRender::GRender():
	_width(0), 
	_height(0),
	_clearColorR(0),
	_clearColorG(0),
	_clearColorB(0)
{
	_glMajorVer = 0;
	_glMinorVer = 0;

}

//--------------------------------------------------------------------------------------
//! инициализировать контекст рисования
//
void GRender::init(int w, int h)
{

	glGetIntegerv(GL_MAJOR_VERSION, &_glMajorVer);
	glGetIntegerv(GL_MINOR_VERSION, &_glMinorVer);


	glViewport(0, 0, _width, _height);

	glClearColor(0, 0, 0, 0);                                      // цвет фона белый
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//loadIdentityMatrix();

	const GLfloat matrix[4][4] = {
		{ 1.0f,   0.0f, 0.0f, 0.0f },
		{ 0.0f,   -1.0f, 0.0f, 0.0f },
		{ 0.0f,   0.0f, 1.0f, 0.0f },
		{ 0.0f,   0.0f, 0.0f, 1.0f } };

	glLoadMatrixf((GLfloat*)matrix);

	glOrtho(0, (float)_width, 0, (float)_height, 2000, -50000);

	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);

	//glEnable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	//glEnableClientState(GL_VERTEX_ARRAY);

	/*if (checkForGLErrors(err)>0)
	{
		AbsTrace(L_INFO, GRender, "init GL error %s", err[0].c_str());
	}*/

}

void GRender::setClearColor(unsigned int c)
{
	_clearColorR = ((c >> 16) & 0xFF) / 255.0f;
	_clearColorG = ((c >> 8) & 0xFF) / 255.0f;
	_clearColorB = ((c) & 0xFF) / 255.0f;

	glClearColor(_clearColorR, _clearColorG, _clearColorB, 1);
}

//----------------------------------------------------------------------------------------
//! очистить окно
//
void GRender::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------------------------------------
//! рисуем пакет изображений
//
void GRender::drawBatchedTris()
{
	if (_batchVertIndex == 0)
		return;

	uint32 text0ID = 0;

	if (_currentTextureID >= 0)
	{
		HWTexture* hwTexture = (HWTexture*)_textures[_currentTextureID];

		if (!hwTexture)
		{
			return;
		}

		text0ID = hwTexture->text0PlatformID();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, text0ID);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	int vertexCount = _batchVertIndex;
	int vertexFirstIndex = _cacheVertIndex - vertexCount;

	save();

	if (_curSpace == SPACE_MODEL)
	{
		convertToPlatformMatrix(_batchMatrix, _curTransformMatrix);
	}
	else
	{
		_curTransformMatrix.SetIdentity();	// т.к. вершины уже в модельных координатах
	}

	applyTransform();

	if (_currentShader)
	{
		_currentShader->start();
	}

	glDisable(GL_LIGHTING);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, &_uvData[vertexFirstIndex]);

	glDisableClientState(GL_NORMAL_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &_vertexData[vertexFirstIndex]);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, &_colourData[vertexFirstIndex]);

	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_SHORT, &_indexesData[vertexFirstIndex]);

	if (_currentTextureID >= 0)
	{
		glDisable(GL_TEXTURE_2D);
	}

	if (_currentShader)
	{
		glUseProgram(0);
	}

	restore();

	_currentTextureID = -1;
	_batchVertIndex = 0;
	_drawImageBatchesPerFrameCount++;
}


//----------------------------------------------------------------------------------------
//! нарисовать залитый триугольник
//
void GRender::drawTri(float x1, float y1, float x2, float y2, float x3, float y3)
{
	drawBatchedTris();

	updateRenderState();

	convertToPlatformMatrix(_matrix, _curTransformMatrix);

	applyTransform();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(_fillR, _fillG, _fillB, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x1, y1);
	glEnd();
	glPopAttrib();
	glEnable(GL_BLEND);
}


//----------------------------------------------------------------------------------------
//! Нарисовать линию (между вершинами - x1,y1,z1 и x2,y2,z2
//
void GRender::drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
	drawBatchedTris();

	updateRenderState();

	convertToPlatformMatrix(_matrix, _curTransformMatrix);

	applyTransform();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(_lineR* _globalR, _lineG * _globalG, _lineB * _globalB, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();
	glPopAttrib();
	glEnable(GL_BLEND);
}

//----------------------------------------------------------------------------------------
//! Создание текстуры из буфера
//
int GRender::createTexture(TexturePixType format, unsigned char* data,
	int texWidth, int texHeight,
	int fid, bool upload/*=true*/)
{
	//checkForGLErrors(err);
	//err.clear();

	bool  compressedTexture = false;
	int   imgSize = 0;
	int   glFormt = GL_RGBA;
	int   aBitPerPixel = 32;

	switch (format)
	{
	//case TPT_BGRA_8888:     glFormt = GL_BGRA;                           aBitPerPixel = 32;  compressedTexture = false;   break;
	case TPT_ABGR_8888:     glFormt = GL_RGBA;                           aBitPerPixel = 32;  compressedTexture = false;   break;
	case TPT_BGR_888:       glFormt = GL_RGB;                            aBitPerPixel = 24;  compressedTexture = false;   break;
	//case TPT_COMPRESS_BC3:  glFormt = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;  aBitPerPixel = 8;   compressedTexture = true;    break;
	}

	imgSize = texWidth*texHeight*aBitPerPixel / 8;

	if (format == GL_RGB && (texWidth % 4)>0)	// текстуры с форматом RGB и шириной некратной 4-м загружаются некорректно
	{
		return -1;
	}

	unsigned int glTextureID;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &glTextureID);

	if (checkForGLErrors(err) > 0)
	{
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, glTextureID);

	/*
	if (_glMajorVer < 2)
	{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else
	{
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	if (checkForGLErrors(err) > 0)
		return -1;

#ifdef SAVE_VRAM_TEXTURES 
	static int textNum = 0;
	char textName[1024];
	sprintf(textName, "rtext_%d_f%d_w%d_h%d.dat", glTextureID, glFormt, texWidth, texHeight);
	s3eFile* f = s3eFileOpen(textName, "wb");
	s3eFileWrite(data, imgSize, 1, f);
	s3eFileClose(f);
#endif

	if (compressedTexture)
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, glFormt, texWidth, texHeight, 0, imgSize, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, glFormt, texWidth, texHeight, 0, glFormt, GL_UNSIGNED_BYTE, data);

	if (checkForGLErrors(err) > 0)
	{
		AbsTrace(L_INFO, GRender, "createTexture error: format %d, (w,h)=(%d,%d);\n", format, texWidth, texHeight);
		return -1;
	}

	HWTexture* hwTexture = new HWTexture(glTextureID, fid, aBitPerPixel);

	unsigned int textureID = addHWTexture(hwTexture);

	static int vramCnt = 0;

	vramCnt += texWidth*texHeight * 4;

	_textureCount++;

	if (_maxTextureCount<_textureCount)
		_maxTextureCount = _textureCount;

	return textureID;
}

//----------------------------------------------------------------------------------------
//! Удаление текстуры
//
void GRender::destroyTexture(unsigned int textureID, bool unload/* = true*/)
{
	if ((textureID >= 0) && ((int)textureID < _textures.size()))
	{
		if (_textures[textureID])
		{
			_textureCount--;
			if (unload)
			{
				delete _textures[textureID];
			}
		}
		_textures[textureID] = 0;
	}
}


void GRender::clearDepthBuffer()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------------------------------------
//! установить прямоулольник отсечения
//
void GRender::setClipRect(int aX, int aY, int aWidth, int aHeight)
{
	drawBatchedTris();

	if (_clipRect.size()>0)
	{
		glDisable(GL_SCISSOR_TEST);
		glPopAttrib();
	}

	_clipRect.push_back(_currentClipRect);

	aX = (aX<0) ? 0 : aX;
	aY = (aY<0) ? 0 : aY;

	aX = (aX>_width) ? _width : aX;
	aY = (aY>_height) ? _height : aY;

	aWidth = (aX + aWidth) >_width ? _width - aX : aWidth;
	aHeight = (aY + aHeight)>_height ? _height - aY : aHeight;

	// вложенное отсечение должно быть внутри текущего
	if (aX<_currentClipRect.x)
	{
		aWidth -= _currentClipRect.x - aX;
		aX = _currentClipRect.x;
	}
	if (aX + aWidth>_currentClipRect.x + _currentClipRect.w)
	{
		aWidth -= aX + aWidth - _currentClipRect.x - _currentClipRect.w;
	}

	if (aY<_currentClipRect.y)
	{
		aHeight -= _currentClipRect.y - aY;
		aY = _currentClipRect.y;
	}
	if (aY + aHeight>_currentClipRect.y + _currentClipRect.h)
	{
		aHeight -= aY + aHeight - _currentClipRect.y - _currentClipRect.h;
	}

	aWidth = (aWidth<0) ? 0 : aWidth;
	aHeight = (aHeight<0) ? 0 : aHeight;

	_currentClipRect.set(aX, aY, aWidth, aHeight);


	glPushAttrib(GL_SCISSOR_BIT);
	glEnable(GL_SCISSOR_TEST);
	glScissor((GLint)aX, (GLint)(_height - aY - aHeight), (GLsizei)aWidth, (GLsizei)aHeight);
}
//----------------------------------------------------------------------------------------
//! убрать режим отсечения
//
void GRender::clearClipRect()
{
	drawBatchedTris();

	if (_clipRect.size()>0)
	{
		glDisable(GL_SCISSOR_TEST);
		glPopAttrib();

		_currentClipRect = _clipRect[_clipRect.size() - 1];
		_clipRect.pop_back();

		if (_clipRect.size() != 0)
		{
			glPushAttrib(GL_SCISSOR_BIT);
			glEnable(GL_SCISSOR_TEST);
			glScissor((GLint)_currentClipRect.x, (GLint)(_height - _currentClipRect.y - _currentClipRect.h), (GLsizei)_currentClipRect.w, (GLsizei)_currentClipRect.h);
		}
	}
}

void GRender::applyTransform()
{
	glLoadMatrixf((GLfloat*)_curTransformMatrix.m);
}