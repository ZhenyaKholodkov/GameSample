#include "GRender.h"
#include <gl\GL.h>
#include <gl\GLU.h>
#include "ext/wglext.h"
#include "ext/olOpenGL.h"

#ifndef USE_OGL_10

//!< обвертка текстуры загружаемой мармеладом в видеопамять
class HWTexture: public IHWTexture
{
public:
      HWTexture(UInt32 text0PlatformIDV, int fid, UInt8 bitsPerPixel, IHWTextureType hwTextureType = IHWTEXTURE_TYPE_ABGR_STAGE0);
      ~HWTexture(); 
      UInt32         text0PlatformID() {return _text0PlatformID;}

private:
   UInt32         _text0PlatformID; //!< id основной текстуры
};

HWTexture::HWTexture(UInt32 text0PlatformIDV, int fid, UInt8 bitsPerPixel, IHWTextureType hwTextureType/* = IHWTEXTURE_TYPE_ABGR_STAGE0*/): 
IHWTexture(fid,bitsPerPixel,hwTextureType),
_text0PlatformID(text0PlatformIDV)
{

}

HWTexture::~HWTexture()
{
    glDeleteTextures(1, &_text0PlatformID );
}


IGRender* IGRender::Create()
{
   _instance = new GRender();
   return _instance;
}


#endif


GRender::GRender() :
_clearColorR(1.0f),
_clearColorG(1.0f),
_clearColorB(1.0f)
{
   _glMajorVer = 0;
   _glMinorVer = 0;
}

//--------------------------------------------------------------------------------------
//! инициализировать контекст рисования
//
void GRender::init( int w, int h )
{
   Stage::firstStageZPos = 0;                                  //!< позиция первой сцены
   Stage::stage3DZDepth  = -400;                               //!< глубина сцены

   IGRender::init(w,h);

   GThreadSafeErrors err;
   
   glGetIntegerv(GL_MAJOR_VERSION, &_glMajorVer);
   glGetIntegerv(GL_MINOR_VERSION, &_glMinorVer);

  // AbsTrace(L_INFO,GRender,"init (%dx%d) OpenGL version %d.%d",w,h,_glMajorVer,_glMinorVer);

   glViewport( 0, 0, _width, _height );

   glClearColor( 100, 100, 100, 1 );                                      // цвет фона белый
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   loadIdentityMatrix();

   const GLfloat matrix[4][4] = {
      {1.0f,   0.0f, 0.0f, 0.0f},
      {0.0f,   -1.0f, 0.0f, 0.0f},
      {0.0f,   0.0f, 1.0f, 0.0f},
      {0.0f,   0.0f, 0.0f, 1.0f}};
  
   glLoadMatrixf((GLfloat*)matrix);

   glOrtho(0, (float)_width, 0, (float)_height, 2000, -50000);

   glMatrixMode(GL_MODELVIEW);
   glShadeModel(GL_SMOOTH);

   //glEnable(GL_TEXTURE_2D);

   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);

   //glEnableClientState(GL_VERTEX_ARRAY);

   setDepthTest(false);    // For 3D true, for other false
   applyDepthTest();

   //GLint status = 0; glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

   if (checkForGLErrors( err )>0)
   {
    //  AbsTrace(L_INFO,GRender,"init GL error %s", err[0].c_str());
   }

}

void GRender::setClearColor( unsigned int c )
{
   _clearColorR = ((c>>16)&0xFF)/255.0f;
   _clearColorG = ((c>> 8)&0xFF)/255.0f;
   _clearColorB = ((  c  )&0xFF)/255.0f;

   //glClearColor(100, 100, 100, 0);
   glClearColor( _clearColorR, _clearColorG, _clearColorB, 1 ); 
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
   if (_batchVertIndex==0)
      return;

   UInt32 text0ID = 0;

   if (_currentTextureID>=0)
   {
      HWTexture* hwTexture = (HWTexture*)_textures[_currentTextureID];

      if (!hwTexture)
      {
       //  AbsTrace(L_INFO,GRender,"drawBatchedImages() texture %d doesn't exists", _currentTextureID);
         return;
      }      

      text0ID = hwTexture->text0PlatformID();

      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, text0ID );
   }
   else
   {
      glDisable(GL_TEXTURE_2D);
   }

   int vertexCount      = _batchVertIndex;
   int vertexFirstIndex = _cacheVertIndex-vertexCount;

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

   glDisable(GL_LIGHTING);

   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glTexCoordPointer(2, GL_FLOAT, 0, &_uvData[vertexFirstIndex]);

   glDisableClientState(GL_NORMAL_ARRAY);

   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, &_vertexData[vertexFirstIndex]);

   glEnableClientState(GL_COLOR_ARRAY);
   glColorPointer(4,  GL_UNSIGNED_BYTE, 0, &_colourData[vertexFirstIndex]);

   glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_SHORT, &_indexesData[vertexFirstIndex]);
   
   if (_currentTextureID>=0)
   {
      glDisable(GL_TEXTURE_2D);
   }
   
   restore();

   _currentTextureID = -1;
   _batchVertIndex = 0;
}


//----------------------------------------------------------------------------------------
//! нарисовать залитый триугольник
//
void GRender::drawTri(float x1, float y1,float x2, float y2,float x3, float y3)
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
//! нарисовать прямоугольник (левый верхний угол - x,y; ширина,высота - w,h
//
void GRender::drawFrame(float x, float y, float w, float h)					
{
   drawBatchedTris();

   updateRenderState();

   convertToPlatformMatrix(_matrix, _curTransformMatrix);

   applyTransform();

   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);

   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glColor4f(_lineR, _lineG, _lineB, 1.0f);
   glBegin(GL_LINE_LOOP);
   glVertex2f(x, y);
   glVertex2f(x+w, y);
   glVertex2f(x+w, y+h);
   glVertex2f(x,   y+h);
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


uint32 GRender::LoadTexture(const unsigned char* bits, uint32 textureWidth, uint32 textureHegih)
{
	int   glFormt = GL_RGBA;

	GThreadSafeErrors err;
	unsigned int glTextureID;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &glTextureID);
	if (checkForGLErrors(err) > 0)
	{
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, glTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	if (checkForGLErrors(err) > 0)
		return -1;

	glTexImage2D(GL_TEXTURE_2D, 0, glFormt, textureWidth, textureHegih, 0, glFormt, GL_UNSIGNED_BYTE, bits);

	if (checkForGLErrors(err) > 0)
		return -1;

	return glTextureID;
}
void  GRender::UnloadTexture(uint32 textureId)
{
	glDeleteTextures(1, &textureId);
}
//----------------------------------------------------------------------------------------
//! Создание текстуры из буфера
//
int GRender::createTexture(TexturePixType format,  unsigned char* data, 
                                       int texWidth, int texHeight, 
                                       int fid,
                                       GThreadSafeErrors &err, bool upload/*=true*/)
{
   checkForGLErrors(err);
   err.clear();

   bool  compressedTexture = false;
   int   imgSize = 0;
   int   glFormt = GL_RGBA;
   int   aBitPerPixel   = 32;

   switch(format)
   {
      case TPT_BGRA_8888:     glFormt = GL_BGRA;                           aBitPerPixel = 32;  compressedTexture = false;   break;
      case TPT_ABGR_8888:     glFormt = GL_RGBA;                           aBitPerPixel = 32;  compressedTexture = false;   break;
      case TPT_BGR_888:       glFormt = GL_RGB;                            aBitPerPixel = 24;  compressedTexture = false;   break;
      case TPT_COMPRESS_BC3:  glFormt = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;  aBitPerPixel = 8;   compressedTexture = true;    break;
   }

   imgSize = texWidth*texHeight*aBitPerPixel/8;

   if (format==GL_RGB && (texWidth%4)>0)	// текстуры с форматом RGB и шириной некратной 4-м загружаются некорректно
   {
     // AbsTrace(L_INFO,GRender,"createTexture: ERROR: format==GL_RGB && (texWidth%%4)>0\n");
      return -1;
   }
   
   unsigned int glTextureID;

   glEnable(GL_TEXTURE_2D);
   glGenTextures(1, &glTextureID);

   if( checkForGLErrors( err ) > 0 )
   {
     // AbsTrace(L_INFO,GRender,"createTexture error glGenTextures: %s;\n",err[0].c_str());
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

   if( checkForGLErrors( err ) > 0 )
      return -1;

#ifdef SAVE_VRAM_TEXTURES 
   static int textNum = 0;
   char textName[1024];
   sprintf(textName,"rtext_%d_f%d_w%d_h%d.dat",glTextureID,glFormt,texWidth,texHeight);
   s3eFile* f = s3eFileOpen(textName,"wb");
   s3eFileWrite(data,imgSize,1,f);
   s3eFileClose(f);
#endif
   
   
 //  if (compressedTexture)
     // glCompressedTexImage2D(GL_TEXTURE_2D, 0, glFormt, texWidth, texHeight, 0, imgSize, data );
 //  else
      glTexImage2D(GL_TEXTURE_2D, 0, glFormt, texWidth, texHeight, 0, glFormt, GL_UNSIGNED_BYTE, data );

   if( checkForGLErrors( err ) > 0 )
   {
    //  AbsTrace(L_INFO,GRender,"createTexture error: format %d, (w,h)=(%d,%d);\n",format,texWidth,texHeight);
      return -1;
   }
   
   HWTexture* hwTexture = new HWTexture(glTextureID,fid,aBitPerPixel);

   unsigned int textureID = addHWTexture(hwTexture);

   static int vramCnt = 0;

   vramCnt+=texWidth*texHeight*4;

   return textureID;
}

//----------------------------------------------------------------------------------------
//! Удаление текстуры
//
void GRender::destroyTexture( unsigned int textureID , bool unload/* = true*/)
{   
   if( (textureID>=0) && ((int)textureID < _textures.size()))
   {	   
      if( _textures[textureID] )
      {
         if (unload)
         {
            delete _textures[textureID];
         }
      }       		  
      _textures[textureID] = 0;
   }
}


//! Установить текущие настройки блендинга. ВНИМАНИЕ - старые НЕ сохраняются автоматически
void GRender::applyBlending()
{
   drawBatchedTris();

   _drawBlending = _curBlending;

   switch (_drawBlending)
   {
      case ALPHA_NONE:	   glDisable(GL_BLEND);														               break;
      case ALPHA_HALF:	   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	glEnable(GL_BLEND);		break;
      case ALPHA_ADD:		glBlendFunc(GL_SRC_ALPHA, GL_LINES);				   glEnable(GL_BLEND);		break;
      case ALPHA_SUB:		glBlendFunc(GL_POINTS, GL_ONE_MINUS_SRC_COLOR);		glEnable(GL_BLEND);		break;
      case ALPHA_BLEND:	   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	glEnable(GL_BLEND);		break;
   }
}	

//--------------------------------------------------------------------------------------
//! Применить текущие настройки отсечения примитивов
//
void GRender::applyCullMode()
{
   drawBatchedTris();

   switch (_curCullMode)
   {
   case CULL_NONE:	glDisable(GL_CULL_FACE);		                  break;
   case CULL_BACK:	glCullFace(GL_BACK); glEnable(GL_CULL_FACE);		break;
   case CULL_FRONT:	glCullFace(GL_FRONT); glEnable(GL_CULL_FACE);	break;
   }
}

//----------------------------------------------------------------------------------------
//! применить текущие настройки Z-буфера
//
void GRender::applyDepthTest()
{
   drawBatchedTris();

   _drawDepthTest = _curDepthTest;

   if (_curDepthTest)
   {
      glDepthFunc(GL_LEQUAL);
      glEnable(GL_DEPTH_TEST);
   }
   else
   {
      glDisable(GL_DEPTH_TEST);
   }
}

void GRender::clearDepthBuffer()
{
   glClear(GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------------------------------------
//! установить прямоулольник отсечения
//
void GRender::setClipRect( int aX, int aY, int aWidth, int aHeight)
{
   drawBatchedTris();

   if (_clipRect.size()>0)
   {
      glDisable(GL_SCISSOR_TEST);
      glPopAttrib();
   }

   _clipRect.push_back(_currentClipRect);

   aX = (aX<0)?0:aX;
   aY = (aY<0)?0:aY;

   aX = (aX>_width)?_width:aX;
   aY = (aY>_height)?_height:aY;

   aWidth  = (aX+aWidth) >_width? _width -aX:aWidth;
   aHeight = (aY+aHeight)>_height?_height-aY:aHeight;

   // вложенное отсечение должно быть внутри текущего
   if (aX<_currentClipRect.x)
   {
      aWidth -= _currentClipRect.x - aX;
      aX = _currentClipRect.x;
   } 
   if (aX + aWidth>_currentClipRect.x+_currentClipRect.w)
   {
      aWidth -= aX + aWidth - _currentClipRect.x - _currentClipRect.w;
   }

   if (aY<_currentClipRect.y)
   {
      aHeight -= _currentClipRect.y - aY;
      aY = _currentClipRect.y;
   }
   if (aY + aHeight>_currentClipRect.y+_currentClipRect.h)
   {
      aHeight -= aY + aHeight - _currentClipRect.y - _currentClipRect.h;
   }

   aWidth  = (aWidth<0)?0:aWidth;
   aHeight = (aHeight<0)?0:aHeight;

   _currentClipRect.set(aX,aY,aWidth,aHeight);


   glPushAttrib(GL_SCISSOR_BIT);
   glEnable(GL_SCISSOR_TEST);
   glScissor((GLint)aX, (GLint)( _height - aY - aHeight ), (GLsizei)aWidth, (GLsizei)aHeight);
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

      _currentClipRect = _clipRect[ _clipRect.size() - 1 ];
      _clipRect.pop_back();	

      if (_clipRect.size()!=0)
      {
         glPushAttrib(GL_SCISSOR_BIT);
         glEnable(GL_SCISSOR_TEST);
         glScissor((GLint)_currentClipRect.x, (GLint)( _height - _currentClipRect.y - _currentClipRect.h ), (GLsizei)_currentClipRect.w, (GLsizei)_currentClipRect.h);
      }
   }
}

void GRender::applyTransform()
{
   glLoadMatrixf((GLfloat*)_curTransformMatrix.m);
}


//----------------------------------------------------------------------------------------
//! начать рисование нового кадра
//
void GRender::startFrame()
{
	IGRender::startFrame();
	//AbsTrace(L_INFO,GRender,"startFrame()\n");   
	clear();
}

//----------------------------------------------------------------------------------------
//! завершить рисование текущего кадра
//
void GRender::endFrame()
{
	draw();

	_isDraw = false;

	if (!_skipFrame)
	{
		// Application::GetInstance()->Draw3D();   // Обновить экран после 3D вывода - вызвать в конце OnTimer или DrawScene
	}

	_skipFrame = false;
}

//----------------------------------------------------------------------------------------
//! нарисовать все
//
void GRender::draw()
{
	/*if (!_isDraw)
	return;*/

	drawBatchedTris();

	_cacheVertIndex = 0;
}

void GRender::convertToPlatformMatrix(IGMatrix& matrix, PlatformMatrix& platformMatrix)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			platformMatrix.m[i][j] = matrix.m[i][j];

	platformMatrix.m[3][3] = 1.0f;

	platformMatrix.m[3][0] = matrix.t.x;
	platformMatrix.m[3][1] = matrix.t.y;
	platformMatrix.m[3][2] = matrix.t.z;
	platformMatrix.m[0][3] = platformMatrix.m[1][3] = platformMatrix.m[2][3] = 0.0f;
}

GRender::PlatformMatrix::PlatformMatrix()
{
	SetIdentity();
}

void GRender::PlatformMatrix::SetIdentity()
{
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.0f;
}
