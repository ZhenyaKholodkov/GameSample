#ifndef _GRENDER_H
#define _GRENDER_H

#include <windows.h>
#include <gl/gl.h>
#include "Types.h"

class GRender
{
public:

	class PlatformMatrix {
	public:
		PlatformMatrix();
		float m[4][4];
		void SetIdentity();
	};

	enum TexturePixType
	{
		TPT_BGRA_8888,
		TPT_ABGR_8888,
		TPT_BGR_888,
		TPT_A_8,
		TPT_COMPRESS_BC3,
		TPT_COMPRESS_PVRTC,
		TPT_COMPRESS_ETC1
	};

	GRender::GRender();

	void		init(int w, int h);										      //!< инициализировать контекст рисования
	void		setClearColor(unsigned int c);							      //!< установить цвет очистки
	void		clear();													            //!< очистить окн

	void		drawTri(float x1, float y1,
		float x2, float y2,
		float x3, float y3);								      //!< нарисовать залитый триугольник

	void		drawLine(float x1, float y1, float z1, float x2, float y2, float z2);	//!< нарисовать линию (между вершинами - x1,y1,z1 и x2,y2,z2)

	int      createTexture(TexturePixType format,
		unsigned char* data,
		int texWidth, int texHeight,
		int fid,
		bool upload = true);	               //!< создание текстуры из буфера
	void		destroyTexture(unsigned int glId, bool unload = true);	//!< удаление текстуры
	void		setClipRect(int aX, int aY, int aWidth, int aHeight);	//!< установить прямоулольник отсечения
	void		clearClipRect();											         //!< убрать режим отсечения
    void		clearDepthBuffer();	   			                        //!< очистить Z-буфер  
	void  applyTransform();
protected:
	void		drawBatchedTris();											      //!< рисуем пакет триугольников

private:
	uint32				      _cacheVertIndex;								//!< указатель на используемый блок кеша вершин
	uint32				      _batchVertIndex;								//!< порядковый номер вершины в партии отрисовки  
	int                       _vertexes;                             //!< число вершин в кеше
	int			            _currentTextureID;							//!< текущая основная текстура, которую рисуем

	IGMatrix 	            _matrix;											//!< матрица трансформации текущей модели

	GLint                   _glMajorVer;                              //!< основная версия OpenGL
	GLint                   _glMinorVer;                              //!< вторичная версия OpenGL

	int                     _width;
	int                     _height;

	float   _clearColorR ;
	float	_clearColorG ;
	float	_clearColorB ;


};

#endif