
#ifndef _GRENDEROGL_ABS_H
#define _GRENDEROGL_ABS_H
/*
#include "IGRender.h"
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

class GRender: public IGRender
{
public:
   GRender::GRender();

   void		init( int w, int h );										      
   void		setClearColor( unsigned int c );							      
   void		clear();													      
   void		drawTri( float x1, float y1, 
                     float x2, float y2, 
                     float x3, float y3);								    
						     
   void		drawLine(float x1, float y1, float z1, float x2, float y2, float z2);	

   uint32 loadTexture(const unsigned char* bits, uint32 textureWidth, uint32 textureHegih);
   void   unloadTexture(uint32 textureId);		                        

   void		startFrame();											
   void		endFrame();												
   void		draw();													

private:
   void	 drawBatchedTris();				
   float mClearColorR;
   float mClearColorG;
   float mClearColorB;
};

inline int checkForGLErrors( GThreadSafeErrors &err )
{
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
}*/

#endif