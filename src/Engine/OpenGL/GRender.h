//! Класс GRender - рендер рисования, использующий OpenGL версионно зависимые функции рисования
/*! \file GRender.h


*                                                    (с) absolutist.com (abeyn) 2013-12-19
********************************************************************************************************/
#ifndef _GRENDEROGL_ABS_H
#define _GRENDEROGL_ABS_H

#include "IGRender.h"
#include <windows.h>
#include <gl/gl.h>
#include "ext\glext.h"

using namespace std;


//-----------------------------------------------------------------------------------------
//! Рендер рисования OpenGL 2.0 и выше
//
class GRender: public IGRender
{
public:
   GRender::GRender();

   void		init( int w, int h );										      //!< инициализировать контекст рисования
   void		setClearColor( unsigned int c );							      //!< установить цвет очистки
   void		clear();													            //!< очистить окно
   void		drawTri( float x1, float y1, 
                     float x2, float y2, 
                     float x3, float y3);								      //!< нарисовать залитый триугольник

   void		drawFrame(float x, float y, float w, float h);							      //!< нарисовать прямоугольник (левый верхний угол - x,y; ширина,высота - w,h
   void		drawLine(float x1, float y1, float z1, float x2, float y2, float z2);	//!< нарисовать линию (между вершинами - x1,y1,z1 и x2,y2,z2)
   int      createTexture(   TexturePixType format,					
                                 unsigned char* data, 
                                 int texWidth, int texHeight, 
                                 int fid,
                                 GThreadSafeErrors &err,
                                 bool upload=true);	               //!< создание текстуры из буфера
   void		destroyTexture( unsigned int glId , bool unload = true);	//!< удаление текстуры
   void		setClipRect( int aX, int aY, int aWidth, int aHeight);	//!< установить прямоулольник отсечения
   void		clearClipRect();											         //!< убрать режим отсечения
   void     applyBlending();				                              //!< применить текущие настройки блендинга. ВНИМАНИЕ - старые НЕ сохраняются автоматически
   void     applyCullMode();	   			                           //!< применить текущие настройки отсечения примитивов
   void		applyTransform();			                                 //!< установить матрицу трансформации. Операция дорогая так как идет пересчет view матрицы!!! 
   void		applyDepthTest();	   			                           //!< применить текущие настройки Z-буфера
   void		clearDepthBuffer();	   			                        //!< очистить Z-буфер

   void		startFrame();												         //!< начать рисование нового кадра
   void		endFrame();													         //!< завершить рисование текущего кадра
   void		draw();														         //!< нарисовать все

protected:
	class PlatformMatrix {
	public:
		PlatformMatrix();
		float m[4][4];
		void SetIdentity();
	};
	void              convertToPlatformMatrix(IGMatrix& matrix, PlatformMatrix& platformMatrix);
   void		drawBatchedTris();											      //!< рисуем пакет триугольников
   PlatformMatrix    _curTransformMatrix;
   std::vector<IGRect>		_clipRect;															   //!< стек сохраненных прямоугольников отсечения
   float				   _clearColorR, _clearColorG, _clearColorB;

   static float _angle3DVert;											                  //!< угол вертикального переворота в 3D
   static float _globalScale;											                  //!< глобальный скейл; нужен для пост-обработки вершин
   static float _farSideScale;											               //!< скейл дальней стороны трапеции при имитации 3D-переворота

private:

   GLint                   _glMajorVer;                              //!< основная версия OpenGL
   GLint                   _glMinorVer;                              //!< вторичная версия OpenGL
};

inline int checkForGLErrors( GThreadSafeErrors &err )
{
	//return 0;
   int errors = 0 ;

   while ( true )
   {
      GLenum x = glGetError();
      if ( x == GL_NO_ERROR )
         return errors;

      std::string aError;      

      switch( x )
      {
      case GL_INVALID_ENUM:      aError = "GL_INVALID_ENUM"; break;
      case GL_INVALID_VALUE:     aError = "GL_INVALID_VALUE"; break;
      case GL_INVALID_OPERATION: aError = "GL_INVALID_OPERATION"; break;
      case GL_STACK_OVERFLOW:    aError = "GL_STACK_OVERFLOW"; break;
      case GL_STACK_UNDERFLOW:   aError = "GL_STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:     aError = "GL_OUT_OF_MEMORY"; break;
      default:
         {
            char str[20];
            sprintf( str, "GL_ERROR = %d", (int)x );
            aError = str;
         }
      }

      err.push(std::string("checkForGLErrors> ") + aError );
      errors++;
   }

   return errors;
}

#endif