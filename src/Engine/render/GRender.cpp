#include "GRender.h"
#include <gl\GL.h>
#include <gl\GLU.h>
#include "ext/wglext.h"
#include "ext/olOpenGL.h"

IGRender* IGRender::Create()
{
   _instance = new GRender();
   return _instance;
}


GRender::GRender() :
_clearColorR(1.0f),
_clearColorG(1.0f),
_clearColorB(1.0f)
{
   _glMajorVer = 0;
   _glMinorVer = 0;
}

void GRender::init( int w, int h )
{                        
   IGRender::init(w,h);

   GThreadSafeErrors err;
   
   glGetIntegerv(GL_MAJOR_VERSION, &_glMajorVer);
   glGetIntegerv(GL_MINOR_VERSION, &_glMinorVer);

   glViewport( 0, 0, _width, _height );

   glClearColor( 100, 100, 100, 1 );        
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
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);

}

void GRender::setClearColor( unsigned int c )
{
   _clearColorR = ((c>>16)&0xFF)/255.0f;
   _clearColorG = ((c>> 8)&0xFF)/255.0f;
   _clearColorB = ((  c  )&0xFF)/255.0f;

   glClearColor( _clearColorR, _clearColorG, _clearColorB, 1 ); 
}

void GRender::clear()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GRender::drawBatchedTris()
{
   if (_batchVertIndex==0)
      return;

   if (_currentTextureID>=0)
   {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, _currentTextureID);
   }
   else
   {
      glDisable(GL_TEXTURE_2D);
   }

   int vertexCount      = _batchVertIndex;
   int vertexFirstIndex = _cacheVertIndex-vertexCount;

   save();

   _curTransformMatrix.SetIdentity();	

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

void GRender::drawTri(float x1, float y1,float x2, float y2,float x3, float y3)
{
   drawBatchedTris();

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

void GRender::drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
   drawBatchedTris();

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

void GRender::applyTransform()
{
   glLoadMatrixf((GLfloat*)_curTransformMatrix.m);
}

void GRender::startFrame()
{
	IGRender::startFrame();
	clear();
}

void GRender::endFrame()
{
	draw();

	_isDraw = false;
	_skipFrame = false;
}

void GRender::draw()
{
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
