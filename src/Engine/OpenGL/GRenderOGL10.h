//! Класс GRenderOGL11 - рендер рисования, использующий OpenGL версионно зависимые функции рисования
/*! \file GRenderOGL11.h


*                                                    (с) absolutist.com (abeyn) 2013-12-19
********************************************************************************************************/
#ifndef _GRENDEROGL10_ABS_H
#define _GRENDEROGL10_ABS_H

#include "sys/GRender.h"
#include <windows.h>
#include <gl/gl.h>

//-----------------------------------------------------------------------------------------
//! Рендер рисования OpenGL 1.0
//
class GRenderOGL10: public GRender
{
public:
   void		init( int w, int h );										      //!< инициализировать контекст рисования
   void		setClearColor( unsigned int c );							      //!< установить цвет очистки
   void		clear();													            //!< очистить окно
   void		drawTri( float x1, float y1, 
                     float x2, float y2, 
                     float x3, float y3);								      //!< нарисовать залитый триугольник

   void		drawFrame(float x, float y, float w, float h);		      //!< нарисовать прямоугольник (левый верхний угол - x,y; ширина,высота - w,h
   void		drawLine(float x1, float y1, float x2, float y2);	      //!< нарисовать линию (между вершинами - x1,y1 и x2,y2
   void		drawCircle(float x, float y, float r);					      //!< нарисовать окружность
   unsigned int createTexture(   int format,						         // GL_RGBA или GL_RGB
                                 unsigned char* data, 
                                 int texWidth, int texHeight, 
                                 GThreadSafeErrors &err,
                                 bool upload=true);	               //!< создание текстуры из буфера
   void		destroyTexture( unsigned int glId , bool unload = true);	//!< удаление текстуры
   void     setBlending(AlphaMode blending);                         //!< установить режим смешивания альфы
   void		loadIdentityMatrix();										      //!< установка начальных настроек рисования
   void		save();														         //!< сохранение настроек рендера (текущие значения заталкиваются в стек)
   void		restore();													         //!< востановление настроек рендера (текущими значениями становятся сохраненные в стеке )
   void		translate( float x, float y, float z = 0.0f );				//!< перемещение камеры
   void		rotate( float ang );										         //!< поворот камеры
   void		rotate( float angX, float angY, float angZ );				//!< поворот камеры 3D
   void		scale( float x, float y );									      //!< установить маштабирование
   void		setClipRect( int aX, int aY, int aWidth, int aHeight);	//!< установить прямоулольник отсечения
   void		clearClipRect();											         //!< убрать режим отсечения
   void     captureScreenPixels(unsigned char** buf, int &bufLen);   //!< получить пиксели экрана  
   void     freeScreenPixels(unsigned char** buf);                   //!< очистить буффер пикселей экрана  

protected:
   void		drawBatchedImages();											      //!< рисуем пакет изображений
   void		drawBatchedRects();											      //!< рисуем пакет прямоугольников
   void		convertVertexDataToModelSpace();							      //!< конвертируем координаты вершин в координатную систему модели
private:
   bool     visible();                                               //!< видим ли будет объект который собираемся рисовать

};

inline int checkForGLErrors( GThreadSafeErrors &err )
{
   int errors = 0 ;

   while ( true )
   {
      GLenum x = glGetError();
      if ( x == GL_NO_ERROR )
         return errors;

      OString aError;      

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

      err.push( OString("checkForGLErrors> ") + aError );
      errors++;
   }

   return errors;
}

#endif