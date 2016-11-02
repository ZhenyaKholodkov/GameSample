#include "GRenderOGL10.h"
#include "GUtils.h"

#ifdef USE_OGL_10

IGRender* IGRender::Create()
{
   _instance = new GRenderOGL10();
   return _instance;
}

//----------------------------------------------------------------------------------------
//! Создать экземпляр эффекта
//
GRender::IGRenderEffectAmbientWithLight* GRender::IGRenderEffectAmbientWithLight::Create()
{
return new GRenderOGL10::GRenderEffectAmbientWithLightPlatform();
}

#endif

//--------------------------------------------------------------------------------------
//! инициализировать контекст рисования
//
void GRenderOGL10::init( int w, int h )
{
   _width = w;
   _height = h;

   glViewport( 0, 0, _width, _height );

   glClearColor( 0, 0, 0, 0 );                                      // цвет фона белый
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   loadIdentityMatrix();

   const GLfloat matrix[4][4] = {
      {1.0f,   0.0f, 0.0f, 0.0f},
      {0.0f,  -1.0f, 0.0f, 0.0f},
      {0.0f,   0.0f, 1.0f, 0.0f},
      {0.0f,   0.0f, 0.0f, 1.0f}};
  
   glLoadMatrixf((GLfloat*)matrix);

   glOrtho(0, (float)_width, 0, (float)_height, -1000, 1000);

   glMatrixMode(GL_MODELVIEW);
   glShadeModel(GL_SMOOTH);

   //glEnable(GL_TEXTURE_2D);

   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);

   //glEnableClientState(GL_VERTEX_ARRAY);

   _currentClipRect.set(0,0,_width,_height);   
}

void GRenderOGL10::setClearColor( unsigned int c )
{
   _clearColorR = ((c>>16)&0xFF)/255.0f;
   _clearColorG = ((c>> 8)&0xFF)/255.0f;
   _clearColorB = ((  c  )&0xFF)/255.0f;

   glClearColor( _clearColorR, _clearColorG, _clearColorB, 1 ); 
}

//----------------------------------------------------------------------------------------
//! очистить окно
//
void GRenderOGL10::clear()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------------------------------------
//! рисуем пакет изображений
//
void GRenderOGL10::drawBatchedImages()
{
   if (_batchImgIndex==0 || (_currentTexture==-1))
      return;

   int vertexCount = _batchImgIndex*4;
   int vertexFirstIndex = _cacheVertIndex-vertexCount;

   save();

   loadIdentityMatrix();	// т.к. вершины уже в модельных координатах

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, _currentTexture );

   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);

   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glTexCoordPointer(2, GL_FLOAT, 0, &_uvData[vertexFirstIndex]);

   glDisableClientState(GL_NORMAL_ARRAY);

   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, &_vertexData[vertexFirstIndex]);

   glEnableClientState(GL_COLOR_ARRAY);
   glColorPointer(4,  GL_FLOAT, 0, &_colourData[vertexFirstIndex]);

   glDrawElements(GL_QUADS, vertexCount, GL_UNSIGNED_SHORT, &_indexesData[vertexFirstIndex]);

   glDisable(GL_TEXTURE_2D);

   restore();

   _currentTexture = -1;
   _batchImgIndex = 0;
   _drawImageBatchesPerFrameCount++;
}

//----------------------------------------------------------------------------------------
//! рисуем пакет прямоугольников
//
void GRenderOGL10::drawBatchedRects()
{
   if (_batchRectIndex==0)
      return;

   int vertexCount = _batchRectIndex*4;
   int vertexFirstIndex = _cacheVertIndex-vertexCount;

   save();

   glPushAttrib(GL_ALL_ATTRIB_BITS);

   loadIdentityMatrix();	// т.к. вершины уже в модельных координатах

   glDisable(GL_TEXTURE_2D);

   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, &_vertexData[vertexFirstIndex]);

   glEnableClientState(GL_COLOR_ARRAY);
   glColorPointer(4,  GL_FLOAT, 0, &_colourData[vertexFirstIndex]);

   glDrawElements(GL_QUADS, vertexCount, GL_UNSIGNED_SHORT, &_indexesData[vertexFirstIndex]);

   glPopAttrib();

   restore();

   _batchRectIndex = 0;
}


//----------------------------------------------------------------------------------------
//! нарисовать залитый триугольник
//
void GRenderOGL10::drawTri(float x1, float y1,float x2, float y2,float x3, float y3)
{
   drawPrevBatched(DB_RECT);

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
void GRenderOGL10::drawFrame(float x, float y, float w, float h)					
{
   drawPrevBatched();

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
//! Нарисовать линию (между вершинами - x1,y1 и x2,y2
//
void GRenderOGL10::drawLine(float x1, float y1, float x2, float y2)
{
   drawPrevBatched();

   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glPushAttrib(GL_ALL_ATTRIB_BITS);
   glColor4f(_lineR* _globalR, _lineG * _globalG, _lineB * _globalB, 1.0f);
   glBegin(GL_LINES);
   glVertex2f(x1, y1);
   glVertex2f(x2, y2);
   glEnd();
   glPopAttrib();
   glEnable(GL_BLEND);
}

//----------------------------------------------------------------------------------------
//! Нарисовать окружность
//
void GRenderOGL10::drawCircle(float x, float y, float r)
{
   //TODO
}

//----------------------------------------------------------------------------------------
//! Создание текстуры из буфера
//
unsigned int GRenderOGL10::createTexture(int format,  unsigned char* data, 
                                    int texWidth, int texHeight, 
                                    GThreadSafeErrors &err,bool upload/*=true*/ )
{
   switch(format)
   {
      case TPT_ABGR_8888:  format = GL_RGBA; break;
      case TPT_BGR_888:    format = GL_RGB; break;
   }
   if (format==GL_RGB && (texWidth%4)>0)	// текстуры с форматом RGB и шириной некратной 4-м загружаются некорректно
   {
      GLog::print("GRenderOGL10::createTexture: ERROR: format==GL_RGB && (texWidth%%4)>0\n");
      return -1;
   }

   unsigned int textureID;

   glEnable(GL_TEXTURE_2D);
   glGenTextures(1, &textureID);

   if( checkForGLErrors( err ) > 0 )
      return 0;

   glBindTexture(GL_TEXTURE_2D, textureID);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

   if( checkForGLErrors( err ) > 0 )
      return 0;

   glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data );

   if( checkForGLErrors( err ) > 0 )
   {
      GLog::print("GRenderOGL10::createTexture error: format %d, (w,h)=(%d,%d);\n",format,texWidth,texHeight);
      return 0;
   }

   _textureCount++;

   if (_maxTextureCount<_textureCount)
      _maxTextureCount = _textureCount;

   return textureID;
}

//----------------------------------------------------------------------------------------
//! Удаление текстуры
//
void GRenderOGL10::destroyTexture( unsigned int glId , bool unload/* = true*/)
{   
   if (unload)
   {
      glDeleteTextures(1, &glId );
      _textureCount--;
   }
}

//----------------------------------------------------------------------------------------
//! сохранение настроек рендера (текущие значения заталкиваются в стек)
//
void GRenderOGL10::save()
{
   Matrix _mat;
   glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat*)_mat.val );
   _matrixStack.push_back( _mat );

   //glPushAttrib(GL_ALL_ATTRIB_BITS);		//!!! очень дорогая операция, (после удаления прирост с 40 до 80 fps!!!) лучше сохранять те биты, которые меняем
   _alphaStack.push_back( _globalAlpha );
}
//----------------------------------------------------------------------------------------
//! востановление настроек рендера (текущими значениями становятся сохраненные в стеке )
//
void GRenderOGL10::restore()
{
   assert( _matrixStack.size() && "_matrixStack.size() == 0" );

   if( _matrixStack.size() )
   {
      Matrix _mat = _matrixStack[ _matrixStack.size() - 1 ];
      _matrixStack.pop_back();
      glLoadMatrixf( (GLfloat*)_mat.val );
   }

   //glPopAttrib();

   assert( _alphaStack.size() && "_alphaStack.size() == 0" );

   if( _alphaStack.size() )
   {
      _globalAlpha = _alphaStack[ _alphaStack.size() - 1 ];
      _alphaStack.pop_back();
   }
}

void GRenderOGL10::loadIdentityMatrix()
{
   glLoadIdentity();
}

//----------------------------------------------------------------------------------------
//! перемещение камеры
//
void GRenderOGL10::translate( float x, float y, float z )
{
   glTranslatef( x, y, z );
}

//----------------------------------------------------------------------------------------
//! поворот камеры
//
void GRenderOGL10::rotate( float ang )
{
   glRotatef( ang, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------------------------------
//! поворот камеры 3D
//
void GRenderOGL10::rotate( float angX, float angY, float angZ )
{
   glRotatef( angX, 1.0f, 0.0f, 0.0f );
   glRotatef( angY, 0.0f, 1.0f, 0.0f );
   glRotatef( angZ, 0.0f, 0.0f, 1.0f );
}

//----------------------------------------------------------------------------------------
// установить маштабирование
void GRenderOGL10::scale( float x, float y ) 
{
   glScalef( x, y, 1.0f );
}	

//----------------------------------------------------------------------------------------
//! Видим ли будет объект который собираемся рисовать
//
bool GRenderOGL10::visible()
{
   if (_globalAlpha<0.02f)
      return false;

   Matrix modelMatrix;

   glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat*)modelMatrix.val );

   if (fabsf(modelMatrix.val[0][0]*modelMatrix.val[1][1] - modelMatrix.val[0][1]*modelMatrix.val[1][0])<0.0001f)
      return false;

   return true;
}

//! Установить текущие настройки блендинга. ВНИМАНИЕ - старые НЕ сохраняются автоматически
void GRenderOGL10::setBlending(AlphaMode blending)
{
   if (blending==_curBlending || blending==ALPHA_PARENT)
      return;

   draw();

   switch (blending)
   {
   case ALPHA_NONE:	   glDisable(GL_BLEND);														               break;
   case ALPHA_HALF:	   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	glEnable(GL_BLEND);		break;
   case ALPHA_ADD:		glBlendFunc(GL_SRC_ALPHA, GL_LINES);				   glEnable(GL_BLEND);		break;
   case ALPHA_SUB:		glBlendFunc(GL_POINTS, GL_ONE_MINUS_SRC_COLOR);		glEnable(GL_BLEND);		break;
   case ALPHA_BLEND:	   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	glEnable(GL_BLEND);		break;
   }

   _curBlending = blending;
}	

//----------------------------------------------------------------------------------------
//! установить прямоулольник отсечения
//
void GRenderOGL10::setClipRect( int aX, int aY, int aWidth, int aHeight)
{
   drawPrevBatched();

   if (_clipRect.size()>0)
   {
      glDisable(GL_SCISSOR_TEST);
      glPopAttrib();
   }

   _clipRect.push_back(_curClipRect);

   aX = (aX<0)?0:aX;
   aY = (aY<0)?0:aY;

   aX = (aX>_width)?_width:aX;
   aY = (aY>_height)?_height:aY;

   aWidth  = (aX+aWidth) >_width? _width -aX:aWidth;
   aHeight = (aY+aHeight)>_height?_height-aY:aHeight;

   // вложенное отсечение должно быть внутри текущего
   if (aX<_curClipRect.X1())
   {
      aWidth -= _curClipRect.X1() - aX;
      aX = _curClipRect.X1();
   } 
   else if (aX + aWidth>_curClipRect.X1()+_curClipRect.X2())
   {
      aWidth -= aX + aWidth - _curClipRect.X1() - _curClipRect.X2();
   }

   if (aY<_curClipRect.Y1())
   {
      aHeight -= _curClipRect.Y1() - aY;
      aY = _curClipRect.Y1();
   }
   else if (aY + aHeight>_curClipRect.Y1()+_curClipRect.Y2())
   {
      aHeight -= aY + aHeight - _curClipRect.Y1() - _curClipRect.Y2();
   }

   aWidth  = (aWidth<0)?0:aWidth;
   aHeight = (aHeight<0)?0:aHeight;

   _curClipRect.Set(aX,aY,aWidth,aHeight);

   glPushAttrib(GL_SCISSOR_BIT);
   glEnable(GL_SCISSOR_TEST);
   glScissor((GLint)aX, (GLint)( _height - aY - aHeight ), (GLsizei)aWidth, (GLsizei)aHeight);
}
//----------------------------------------------------------------------------------------
//! убрать режим отсечения
//
void GRenderOGL10::clearClipRect()
{
   drawPrevBatched();

   if (_clipRect.size()>0)
   {
      glDisable(GL_SCISSOR_TEST);
      glPopAttrib();

      _curClipRect = _clipRect[ _clipRect.size() - 1 ];
      _clipRect.pop_back();	

      if (_clipRect.size()!=0)
      {
         glPushAttrib(GL_SCISSOR_BIT);
         glEnable(GL_SCISSOR_TEST);
         glScissor((GLint)_curClipRect.X1(), (GLint)( _height - _curClipRect.Y1() - _curClipRect.Y2() ), (GLsizei)_curClipRect.X2(), (GLsizei)_curClipRect.Y2());
      }
   }
}

//----------------------------------------------------------------------------------------
//! конвертируем координаты вершин в координатную систему модели
//
void GRenderOGL10::convertVertexDataToModelSpace()
{	
   static CFVec3 newVertexData;
   Matrix modelMatrix;

   glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat*)modelMatrix.val );

   for (int vertIndex=0; vertIndex<4; vertIndex++)
   {
      int vertItem = _cacheVertIndex + vertIndex;
      newVertexData.x = modelMatrix.val[0][0]*_vertexData[vertItem].x + modelMatrix.val[1][0]*_vertexData[vertItem].y + modelMatrix.val[2][0]*_vertexData[vertItem].z;
      newVertexData.y = modelMatrix.val[0][1]*_vertexData[vertItem].x + modelMatrix.val[1][1]*_vertexData[vertItem].y + modelMatrix.val[2][1]*_vertexData[vertItem].z;
      newVertexData.z = modelMatrix.val[0][2]*_vertexData[vertItem].x + modelMatrix.val[1][2]*_vertexData[vertItem].y + modelMatrix.val[2][2]*_vertexData[vertItem].z;
      newVertexData.x += modelMatrix.val[3][0];
      newVertexData.y += modelMatrix.val[3][1];
      newVertexData.z += modelMatrix.val[3][2];
      _vertexData[vertItem].x = newVertexData.x;
      _vertexData[vertItem].y = newVertexData.y;
      _vertexData[vertItem].z = newVertexData.z;
   }
}

//----------------------------------------------------------------------------------------
//! получить пиксели экрана  
//
void GRenderOGL10::captureScreenPixels(unsigned char** buf, int &bufLen)
{
   int scrW = _width;
   int scrH = _height;
   bufLen = scrW*scrH*4;
   *buf = (unsigned char*)malloc( bufLen );
   memset( *buf, 0, bufLen );

   glReadPixels(0,0,scrW,scrH,GL_RGBA,GL_UNSIGNED_BYTE,*buf);

   unsigned char* scrTmpBitmap= (unsigned char*)malloc( bufLen );

   for(int y = 0; y < scrH; y++)
   {
      for(int x = 0; x < scrW*4; x+=4)
      {
         int srcOffs = y * 4 * scrW + x;
         int dstOffs = ((scrH - 1) - y) * scrW * 4 + x;
         scrTmpBitmap[dstOffs]      = (*buf)[srcOffs];               // R
         scrTmpBitmap[dstOffs + 1]  = (*buf)[srcOffs + 1];           // G
         scrTmpBitmap[dstOffs + 2]  = (*buf)[srcOffs + 2];           // B
         //scrTmpBitmap[dstOffs + 3]  = scrBitmap[srcOffs + 3];      // A  
         scrTmpBitmap[dstOffs + 3]  = 255;                           // A
      }
   }
   free(*buf);
   *buf = scrTmpBitmap;
}

//----------------------------------------------------------------------------------------
//! очистить буффер пикселей экрана  
//
void GRenderOGL10::freeScreenPixels(unsigned char** buf)
{
   free(*buf);
}



GRenderOGL10::GRenderEffectAmbientWithLightPlatform::GRenderEffectAmbientWithLightPlatform()
{
}