#include "IGRender.h"

IGRender* IGRender::_instance = NULL;

IGRender::IGRender():
_skipFrame(false),
_frameNumber( 0 ),
_width( 0 ),
_height( 0 ),
_globalR( 1.0f ),
_globalG( 1.0f ),
_globalB( 1.0f ),
_globalAlpha( 1.0f ),
_fontR(0.0f), _fontG(0.0f), _fontB(0.0f), _fontA(1.0f),        
_fillR(0.0f), _fillG(0.0f), _fillB(0.0f),                      
_lineR(0.0f), _lineG(0.0f), _lineB(0.0f),                      
_fontColor(0xFF000000),
_clearColor(0x00000000),
_vertexes(32000),
_cacheVertIndex(0),
_currentTextureID(-1),
_curDepthTest(false),
_drawDepthTest(false),
_batchVertIndex(0),
_BATCHED_VERTEXES_SIZE(4092),	
_isDraw(false)
{
   allocVertexCache();
	int val = 0;
}

void	IGRender::init( int w, int h )
{
   _width = w;
	_height = h;
   _batchMatrix.setIdentity();
}

void	IGRender::startFrame()
{
	_isDraw = true;
	_frameNumber++;
}

void	IGRender::endFrame()
{
   _currentTextureID = -1;
   _isDraw = false;
}

void IGRender::setLineColor( float r, float g, float b )
{
	_lineR = r;
	_lineG = g;
	_lineB = b;
}

void IGRender::setLineColor(UInt32 c )
{
	_lineR = ((c>>16)&0xFF)/255.0f;
	_lineG = ((c>> 8)&0xFF)/255.0f;
	_lineB = ((  c  )&0xFF)/255.0f;
}

void IGRender::setFillColor( float r, float g, float b )
{
	_fillR = r;
	_fillG = g;
	_fillB = b;
}

void IGRender::setFillColor(UInt32 c )
{
	_fillR = ((c>>16)&0xFF)/255.0f;
	_fillG = ((c>> 8)&0xFF)/255.0f;
	_fillB = ((  c  )&0xFF)/255.0f;
}

void IGRender::loadIdentityMatrix()
{
   _matrix.setIdentity();
}

void IGRender::save()
{
   _matrixStack.push( _matrix );
   _alphaStack.push( _globalAlpha );
}

void IGRender::restore()
{
   if( _matrixStack.size() )
   {
	   _matrix = _matrixStack.top();
	  _matrixStack.pop();
   }

   if( _alphaStack.size() )
   {
      _globalAlpha = _alphaStack.top();
	  _alphaStack.pop();
   }
}

void IGRender::translate( float x, float y, float z/* = 0.0f*/ )
{
   if (x==0.0f && y==0.0f && z==0.0f)
      return;

   _matrix.t.x = ((_matrix.m[0][0] * x) + (_matrix.m[1][0] * y) + (_matrix.m[2][0] * z)) + _matrix.t.x;
   _matrix.t.y = ((_matrix.m[0][1] * x) + (_matrix.m[1][1] * y) + (_matrix.m[2][1] * z)) + _matrix.t.y;
   _matrix.t.z = ((_matrix.m[0][2] * x) + (_matrix.m[1][2] * y) + (_matrix.m[2][2] * z)) + _matrix.t.z;
}

void IGRender::rotate( float ang )
{
   if (ang==0.0f)
      return;

   IGMatrix		rotZ;
   rotZ.SetRotZ( -ang * DEGREE_TO_RAD );

   _matrix = IGMatrix::matMul(rotZ, _matrix);
}

void IGRender::rotate( float angX, float angY, float angZ )
{
   if (angX!=0.0f)
   {
      IGMatrix		rotX;
      rotX.SetRotX( -angX * DEGREE_TO_RAD );

      _matrix = IGMatrix::matMul(rotX, _matrix);
   }

   if (angY!=0.0f)
   {
      IGMatrix		rotY;
      rotY.SetRotY( -angY * DEGREE_TO_RAD );

      _matrix = IGMatrix::matMul(rotY, _matrix);
   }

   if (angZ!=0.0f)
   {
      IGMatrix		rotZ;
      rotZ.SetRotZ( -angZ * DEGREE_TO_RAD );

      _matrix = IGMatrix::matMul(rotZ, _matrix);
   }
}

void IGRender::scale( float x, float y, float z/*=1.0f*/  )
{
   if (x==1.0f && y==1.0f && z==1.0f)
      return;

   _matrix.m[0][0] *= x;
   _matrix.m[0][1] *= x;
   _matrix.m[1][0] *= y;
   _matrix.m[1][1] *= y;
   _matrix.m[2][0] *= z;
   _matrix.m[2][1] *= z;
}

bool IGRender::visible()
{
   if (_globalAlpha<0.02f)
      return false;

   if (fabsf(_matrix.m[0][0]*_matrix.m[1][1] - _matrix.m[0][1]*_matrix.m[1][0])<0.0001f)
      return false;

   return true;
}

void IGRender::drawImage(	unsigned int textureId, 
                           int full_tex_w, int full_tex_h,
                           int tex_x, int tex_y, int tex_w, int tex_h,
                           float x, float y, float z/* = 0.0f*/, 
                           UInt32 color0/* = 0xffffffff*/,									
                           UInt32 color1/* = 0xffffffff*/,									
                           UInt32 color2/* = 0xffffffff*/,									
                           UInt32 color3/* = 0xffffffff*/									
                        )
{	   
   if (!visible())																			
      return;

   IGVector3 tmpVertexData0, tmpVertexData2;												

   tmpVertexData0.x = x;			tmpVertexData0.y = y;			tmpVertexData0.z = z;		
   tmpVertexData2.x = x+tex_w;	tmpVertexData2.y = y+tex_h;	tmpVertexData2.z = z;

   convertVertexDataToModelSpace(tmpVertexData0);
   convertVertexDataToModelSpace(tmpVertexData2);

   IGVector3 &vertexData0 = _vertexData[_cacheVertIndex];												
   IGVector3 &vertexData1 = _vertexData[_cacheVertIndex+1];
   IGVector3 &vertexData2 = _vertexData[_cacheVertIndex+2];
   IGVector3 &vertexData3 = _vertexData[_cacheVertIndex+3];
   IGVector3 &vertexData4 = _vertexData[_cacheVertIndex+4];
   IGVector3 &vertexData5 = _vertexData[_cacheVertIndex+5];

   vertexData0 = tmpVertexData0;
   vertexData2 = tmpVertexData2;

   vertexData1.x = x;			vertexData1.y = y+tex_h;	vertexData1.z = z;
   vertexData3.x = x+tex_w;	vertexData3.y = y;			vertexData3.z = z;

   convertVertexDataToModelSpace(vertexData1);
   convertVertexDataToModelSpace(vertexData3);

   vertexData5 = vertexData3; 
   vertexData3 = vertexData0; 
   vertexData4 = vertexData2; 

   if (																				
      (_currentTextureID < 0) ||													
      (_currentTextureID != (int)textureId) 										
      )
   {

      drawBatchedTris();
      _currentTextureID = textureId;

   }

   IGVector2 &uvData0 = _uvData[_cacheVertIndex];
   IGVector2 &uvData1 = _uvData[_cacheVertIndex+1];
   IGVector2 &uvData2 = _uvData[_cacheVertIndex+2];
   IGVector2 &uvData3 = _uvData[_cacheVertIndex+3];
   IGVector2 &uvData4 = _uvData[_cacheVertIndex+4];
   IGVector2 &uvData5 = _uvData[_cacheVertIndex+5];

   float texLeft  = (float)tex_x / (float)( full_tex_w );											
   float texUp    = (float)tex_y / (float)( full_tex_h );
   float texRight = (float)( tex_w + tex_x ) / (float)( full_tex_w );
   float texDown  = (float)( tex_h + tex_y ) / (float)( full_tex_h );

   uvData0.x = texLeft;	   uvData0.y = texUp;														
   uvData1.x = texLeft;	   uvData1.y = texDown;	
   uvData2.x = texRight;	uvData2.y = texDown;	
   uvData3.x = texRight;	uvData3.y = texUp;	

   uvData5 = uvData3; 
   uvData3 = uvData0; 
   uvData4 = uvData2; 

   IGColor &igcolour0 = _colourData[_cacheVertIndex];
   IGColor &igcolour1 = _colourData[_cacheVertIndex+3];
   IGColor &igcolour2 = _colourData[_cacheVertIndex+2];
   IGColor &igcolour3 = _colourData[_cacheVertIndex+1];
   IGColor &igcolour4 = _colourData[_cacheVertIndex+4];
   IGColor &igcolour5 = _colourData[_cacheVertIndex+5];

   if (_globalAlpha==1.0f && color0==0xffffffff && _globalR==1.0f && _globalG==1.0f && _globalB==1.0f)
      igcolour0.r = igcolour0.g = igcolour0.b = igcolour0.a = 255;
   else
      igcolour0.set((UInt8)(((color0 >> 16) & 0xff) * _globalR), (UInt8)(((color0 >> 8) & 0xff) * _globalG), (UInt8)(((color0) & 0xff) * _globalB), (UInt8)(((color0 >> 24) & 0xff) * _globalAlpha));											// устанавливаем цвет рисования

   if (color1==color0) igcolour1=igcolour0; else igcolour1.set((UInt8)(((color1 >> 16) & 0xff) * _globalR), (UInt8)(((color1 >> 8) & 0xff) * _globalG), (UInt8)(((color1) & 0xff) * _globalB), (UInt8)(((color1 >> 24) & 0xff) * _globalAlpha));			// устанавливаем цвет рисования
   if (color2==color0) igcolour2=igcolour0; else igcolour2.set((UInt8)(((color2 >> 16) & 0xff) * _globalR), (UInt8)(((color2 >> 8) & 0xff) * _globalG), (UInt8)(((color2) & 0xff) * _globalB), (UInt8)(((color2 >> 24) & 0xff) * _globalAlpha));			// устанавливаем цвет рисования
   if (color3==color0) igcolour3=igcolour0; else igcolour3.set((UInt8)(((color3 >> 16) & 0xff) * _globalR), (UInt8)(((color3 >> 8) & 0xff) * _globalG), (UInt8)(((color3) & 0xff) * _globalB), (UInt8)(((color3 >> 24) & 0xff) * _globalAlpha));			// устанавливаем цвет рисования

   igcolour5 = igcolour1; 
   igcolour3 = igcolour0; 
   igcolour4 = igcolour2; 

   int firstImgIndex = _batchVertIndex;

   _indexesData[_cacheVertIndex]	  = firstImgIndex++;
   _indexesData[_cacheVertIndex+1] = firstImgIndex++;
   _indexesData[_cacheVertIndex+2] = firstImgIndex++;
   _indexesData[_cacheVertIndex+3] = firstImgIndex++;
   _indexesData[_cacheVertIndex+4] = firstImgIndex++;
   _indexesData[_cacheVertIndex+5] = firstImgIndex++;
   
   addBatchTris(2);
}

void IGRender::drawRect(float x, float y, float w, float h)
{
   IGColor colour;
   colour.set( (UInt8)(255 * _fillR * _globalR), (UInt8)(255 * _fillG * _globalG), 
               (UInt8)(255 * _fillB * _globalB), (UInt8)(255 * _globalAlpha) );


   drawRect(x, y, w, h, colour, colour, colour, colour);
}

void	IGRender::drawRect(float x, float y, float w, float h, UInt32 color0, UInt32 color1, UInt32 color2, UInt32 color3)
{
   IGColor	igColour0,	igColour1, 	igColour2,	igColour3;

   igColour0.set(color0);
   igColour1.set(color1);
   igColour2.set(color2);
   igColour3.set(color3);

   igColour0.a = (UInt8)(igColour0.a * _globalAlpha);
   igColour1.a = (UInt8)(igColour1.a * _globalAlpha);
   igColour2.a = (UInt8)(igColour2.a * _globalAlpha);
   igColour3.a = (UInt8)(igColour3.a * _globalAlpha);

   drawRect(x, y, w, h, igColour0, igColour1, igColour2, igColour3);
}

inline void	IGRender::drawRect(float x, float y, float w, float h, IGColor color0, IGColor color1, IGColor color2, IGColor color3)
{
   IGVector3 &vertexData0 = _vertexData[_cacheVertIndex];												
   IGVector3 &vertexData1 = _vertexData[_cacheVertIndex+1];
   IGVector3 &vertexData2 = _vertexData[_cacheVertIndex+2];	
   IGVector3 &vertexData3 = _vertexData[_cacheVertIndex+3];
   IGVector3 &vertexData4 = _vertexData[_cacheVertIndex+4];
   IGVector3 &vertexData5 = _vertexData[_cacheVertIndex+5];

   vertexData0.x = x;		vertexData0.y = y;			vertexData0.z	= 0.0f;		
   vertexData1.x = x;		vertexData1.y = y+h;		   vertexData1.z	= 0.0f;
   vertexData2.x = x+w;		vertexData2.y = y+h;		   vertexData2.z	= 0.0f;
   vertexData3.x = x+w;		vertexData3.y = y;			vertexData3.z	= 0.0f;

   convertVertexDataToModelSpace(vertexData0);
   convertVertexDataToModelSpace(vertexData2);

   if (testOutsideScreen(vertexData0,vertexData2))											
      return;

   if (								
         _currentTextureID >=0
      )
   {
      drawBatchedTris();
      _currentTextureID = -1;
   }

   convertVertexDataToModelSpace(vertexData1);
   convertVertexDataToModelSpace(vertexData3);

   vertexData5 = vertexData3; 
   vertexData3 = vertexData0; 
   vertexData4 = vertexData2; 

   IGVector2 &uvData0 = _uvData[_cacheVertIndex];
   IGVector2 &uvData1 = _uvData[_cacheVertIndex+1];
   IGVector2 &uvData2 = _uvData[_cacheVertIndex+2];
   IGVector2 &uvData3 = _uvData[_cacheVertIndex+3];
   IGVector2 &uvData4 = _uvData[_cacheVertIndex+4];
   IGVector2 &uvData5 = _uvData[_cacheVertIndex+5];

   uvData0.x = 0.0f;		uvData0.y = 0.0f;						
   uvData1.x = 0.0f;		uvData1.y = 1.0f;
   uvData2.x	= 1.0f;	uvData2.y = 1.0f;
   uvData3.x	= 1.0f;	uvData3.y = 0.0f;

   uvData5 = uvData3; 
   uvData3 = uvData0; 
   uvData4 = uvData2; 

   IGColor &igcolour0 = _colourData[_cacheVertIndex];
   IGColor &igcolour1 = _colourData[_cacheVertIndex+1];
   IGColor &igcolour2 = _colourData[_cacheVertIndex+2];
   IGColor &igcolour3 = _colourData[_cacheVertIndex+3];
   IGColor &igcolour4 = _colourData[_cacheVertIndex+4];
   IGColor &igcolour5 = _colourData[_cacheVertIndex+5];

   igcolour0 =	color0;
   igcolour1 =	color3;
   igcolour2 =	color2;
   igcolour3 =	color1;

   igcolour5 = igcolour3; 
   igcolour3 = igcolour0; 
   igcolour4 = igcolour2; 

   int firstRectIndex = _batchVertIndex;

   _indexesData[_cacheVertIndex]	  = firstRectIndex++;
   _indexesData[_cacheVertIndex+1] = firstRectIndex++;
   _indexesData[_cacheVertIndex+2] = firstRectIndex++;
   _indexesData[_cacheVertIndex+3] = firstRectIndex++;
   _indexesData[_cacheVertIndex+4] = firstRectIndex++;
   _indexesData[_cacheVertIndex+5] = firstRectIndex++;

   addBatchTris(2);
}

void	IGRender::addBatchTris(int triCnt)
{
   int VertexesByTris = triCnt*3;
   _cacheVertIndex+=VertexesByTris;
   _batchVertIndex+=VertexesByTris;

   if ((int)_cacheVertIndex + VertexesByTris>=_vertexData.size())													
   {
      reallocVertexCache();
   }
   else
   {
      if (_batchVertIndex>=_BATCHED_VERTEXES_SIZE)
      {
         drawBatchedTris();
      }
   }
}

void	IGRender::allocVertexCache()
{
   _vertexData.resize(_vertexes);
   _uvData.resize(_vertexes);
   _colourData.resize(_vertexes);
   _indexesData.resize(_vertexes);
}

void	IGRender::reallocVertexCache()
{
   draw();
   _vertexes*=2;
   allocVertexCache();
}

void IGRender::skipFrame(bool val)
{
   _skipFrame = val; 
}