#include "gRenderManager.h"

#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>


#define GL_GENERATE_MIPMAP                0x8191

GRenderManager* GRenderManager::Instance()
{
	static GRenderManager* instance = new GRenderManager();
	return instance;
}

GRenderManager::GRenderManager():
mWidth( 0 ),
mHeight( 0 ),
mClearColorR(1.0f),
mClearColorG(1.0f),
mClearColorB(1.0f),
mFillR(0.0f),
mFillG(0.0f),
mFillB(0.0f),
mLineR(0.0f),
mLineG(0.0f),
mLineB(0.0f),
mVertexes(32000),
mCacheVertIndex(0),
mCurrentTextureID(-1),
mBatchVertIndex(0),
BATCHED_VERTEXES_SIZE(4092)
{
   allocVertexCache();
	int val = 0;
}

void	GRenderManager::init( int w, int h )
{
    mWidth = w;
	mHeight = h;

	GThreadSafeErrors err;

	glViewport(0, 0, mWidth, mHeight);

	glClearColor(100, 100, 100, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	loadIdentityMatrix();

	const GLfloat matrix[4][4] = {
		{ 1.0f,   0.0f, 0.0f, 0.0f },
		{ 0.0f,   -1.0f, 0.0f, 0.0f },
		{ 0.0f,   0.0f, 1.0f, 0.0f },
		{ 0.0f,   0.0f, 0.0f, 1.0f } };

	glLoadMatrixf((GLfloat*)matrix);

	glOrtho(0, (float)mWidth, 0, (float)mHeight, 2000, -50000);

	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void	GRenderManager::startFrame()
{
	clear();
}

void	GRenderManager::endFrame()
{
	draw();
   mCurrentTextureID = -1;
}

void GRenderManager::setLineColor( float r, float g, float b )
{
	mLineR = r;
	mLineG = g;
	mLineB = b;
}

void GRenderManager::setLineColor(UInt32 c )
{
	mLineR = ((c>>16)&0xFF)/255.0f;
	mLineG = ((c>> 8)&0xFF)/255.0f;
	mLineB = ((  c  )&0xFF)/255.0f;
}

void GRenderManager::setFillColor( float r, float g, float b )
{
	mFillR = r;
	mFillG = g;
	mFillB = b;
}

void GRenderManager::setFillColor(UInt32 c )
{
	mFillR = ((c>>16)&0xFF)/255.0f;
	mFillG = ((c>> 8)&0xFF)/255.0f;
	mFillB = ((  c  )&0xFF)/255.0f;
}

void GRenderManager::loadIdentityMatrix()
{
   mMatrix.setIdentity();
}

void GRenderManager::save()
{
	mMatrixStack.push( mMatrix );
}

void GRenderManager::restore()
{
   if(mMatrixStack.size() )
   {
	   mMatrix = mMatrixStack.top();
	  mMatrixStack.pop();
   }
}

void GRenderManager::translate( float x, float y, float z/* = 0.0f*/ )
{
   if (x==0.0f && y==0.0f && z==0.0f)
      return;

   mMatrix.t.x = ((mMatrix.m[0][0] * x) + (mMatrix.m[1][0] * y) + (mMatrix.m[2][0] * z)) + mMatrix.t.x;
   mMatrix.t.y = ((mMatrix.m[0][1] * x) + (mMatrix.m[1][1] * y) + (mMatrix.m[2][1] * z)) + mMatrix.t.y;
   mMatrix.t.z = ((mMatrix.m[0][2] * x) + (mMatrix.m[1][2] * y) + (mMatrix.m[2][2] * z)) + mMatrix.t.z;
}

void GRenderManager::rotate( float ang )
{
   if (ang==0.0f)
      return;

   IGMatrix		rotZ;
   rotZ.SetRotZ( -ang * DEGREE_TO_RAD );

   mMatrix = IGMatrix::matMul(rotZ, mMatrix);
}

void GRenderManager::rotate( float angX, float angY, float angZ )
{
   if (angX!=0.0f)
   {
      IGMatrix		rotX;
      rotX.SetRotX( -angX * DEGREE_TO_RAD );

      mMatrix = IGMatrix::matMul(rotX, mMatrix);
   }

   if (angY!=0.0f)
   {
      IGMatrix		rotY;
      rotY.SetRotY( -angY * DEGREE_TO_RAD );

      mMatrix = IGMatrix::matMul(rotY, mMatrix);
   }

   if (angZ!=0.0f)
   {
      IGMatrix		rotZ;
      rotZ.SetRotZ( -angZ * DEGREE_TO_RAD );

      mMatrix = IGMatrix::matMul(rotZ, mMatrix);
   }
}

void GRenderManager::scale( float x, float y, float z/*=1.0f*/  )
{
   if (x==1.0f && y==1.0f && z==1.0f)
      return;

   mMatrix.m[0][0] *= x;
   mMatrix.m[0][1] *= x;
   mMatrix.m[1][0] *= y;
   mMatrix.m[1][1] *= y;
   mMatrix.m[2][0] *= z;
   mMatrix.m[2][1] *= z;
}

bool GRenderManager::visible()
{
   if (fabsf(mMatrix.m[0][0]*mMatrix.m[1][1] - mMatrix.m[0][1]*mMatrix.m[1][0])<0.0001f)
      return false;

   return true;
}

void GRenderManager::drawImage( uint32 textureId, int full_tex_w, int full_tex_h,
                                int tex_x, int tex_y, int tex_w, int tex_h,
                                float x, float y, float z/* = 0.0f*/)
{	   
   if (!visible())																			
      return;

   IGVector3 tmpVertexData0, tmpVertexData2;												

   tmpVertexData0.x = x;			tmpVertexData0.y = y;			tmpVertexData0.z = z;		
   tmpVertexData2.x = x+tex_w;	tmpVertexData2.y = y+tex_h;	tmpVertexData2.z = z;

   convertVertexDataToModelSpace(tmpVertexData0);
   convertVertexDataToModelSpace(tmpVertexData2);

   IGVector3 &vertexData0 = mVertexData[mCacheVertIndex];												
   IGVector3 &vertexData1 = mVertexData[mCacheVertIndex+1];
   IGVector3 &vertexData2 = mVertexData[mCacheVertIndex+2];
   IGVector3 &vertexData3 = mVertexData[mCacheVertIndex+3];
   IGVector3 &vertexData4 = mVertexData[mCacheVertIndex+4];
   IGVector3 &vertexData5 = mVertexData[mCacheVertIndex+5];

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
      (mCurrentTextureID < 0) ||													
      (mCurrentTextureID != (int)textureId) 										
      )
   {

      drawBatchedTris();
      mCurrentTextureID = textureId;

   }

   IGVector2 &uvData0 = mTextCoordData[mCacheVertIndex];
   IGVector2 &uvData1 = mTextCoordData[mCacheVertIndex+1];
   IGVector2 &uvData2 = mTextCoordData[mCacheVertIndex+2];
   IGVector2 &uvData3 = mTextCoordData[mCacheVertIndex+3];
   IGVector2 &uvData4 = mTextCoordData[mCacheVertIndex+4];
   IGVector2 &uvData5 = mTextCoordData[mCacheVertIndex+5];

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

   int firstImgIndex = mBatchVertIndex;

   mIndexesData[mCacheVertIndex]	  = firstImgIndex++;
   mIndexesData[mCacheVertIndex+1] = firstImgIndex++;
   mIndexesData[mCacheVertIndex+2] = firstImgIndex++;
   mIndexesData[mCacheVertIndex+3] = firstImgIndex++;
   mIndexesData[mCacheVertIndex+4] = firstImgIndex++;
   mIndexesData[mCacheVertIndex+5] = firstImgIndex++;
   
   addBatchTris(2);
}

void	GRenderManager::addBatchTris(int triCnt)
{
   int VertexesByTris = triCnt*3;
   mCacheVertIndex+=VertexesByTris;
   mBatchVertIndex+=VertexesByTris;

   if ((int)mCacheVertIndex + VertexesByTris>=mVertexData.size())													
   {
      reallocVertexCache();
   }
   else
   {
      if (mBatchVertIndex>= BATCHED_VERTEXES_SIZE)
      {
         drawBatchedTris();
      }
   }
}

void	GRenderManager::allocVertexCache()
{
   mVertexData.resize(mVertexes);
   mTextCoordData.resize(mVertexes);
   mIndexesData.resize(mVertexes);
}

void	GRenderManager::reallocVertexCache()
{
   draw();
   mVertexes*=2;
   allocVertexCache();
}


void	GRenderManager::convertVertexDataToModelSpace(IGVector3 &vertex) const
{
	IGVector3 newVertexData;
	newVertexData.x = mMatrix.m[0][0] * vertex.x + mMatrix.m[1][0] * vertex.y + mMatrix.t.x;
	newVertexData.y = mMatrix.m[0][1] * vertex.x + mMatrix.m[1][1] * vertex.y + mMatrix.t.y;
	newVertexData.z = mMatrix.m[0][2] * vertex.x + mMatrix.m[1][2] * vertex.y + mMatrix.t.z;

	if (vertex.z != 0.0f)
	{
		newVertexData.x += mMatrix.m[2][0] * vertex.z;
		newVertexData.y += mMatrix.m[2][1] * vertex.z;
		newVertexData.z += mMatrix.m[2][2] * vertex.z;
	}
	vertex = newVertexData;
}

bool	GRenderManager::testOutsideScreen(IGVector3 &vertex0, IGVector3 &vertex2) const
{
	return (
		(vertex0.x < 0.0f		&& vertex2.x < 0.0f) ||
		(vertex0.x > mWidth	&& vertex2.x > mWidth) ||
		(vertex0.y < 0.0f		&& vertex2.y < 0.0f) ||
		(vertex0.y > mHeight	&& vertex2.y > mHeight)
		);
}

void GRenderManager::setClearColor(unsigned int c)
{
	mClearColorR = ((c >> 16) & 0xFF) / 255.0f;
	mClearColorG = ((c >> 8) & 0xFF) / 255.0f;
	mClearColorB = ((c) & 0xFF) / 255.0f;

	glClearColor(mClearColorR, mClearColorG, mClearColorB, 1);
}

void GRenderManager::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GRenderManager::draw()
{
	drawBatchedTris();

	mCacheVertIndex = 0;
}

void GRenderManager::drawBatchedTris()
{
	if (mBatchVertIndex == 0)
		return;

	if (mCurrentTextureID >= 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mCurrentTextureID);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	int vertexCount = mBatchVertIndex;
	int vertexFirstIndex = mCacheVertIndex - vertexCount;

	save();

	glDisable(GL_LIGHTING);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, &mTextCoordData[vertexFirstIndex]);

	glDisableClientState(GL_NORMAL_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &mVertexData[vertexFirstIndex]);

	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_SHORT, &mIndexesData[vertexFirstIndex]);

	if (mCurrentTextureID >= 0)
	{
		glDisable(GL_TEXTURE_2D);
	}

	restore();

	mCurrentTextureID = -1;
	mBatchVertIndex = 0;
}

void GRenderManager::drawTri(float x1, float y1, float x2, float y2, float x3, float y3)
{
	drawBatchedTris();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(mFillR, mFillG, mFillB, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x1, y1);
	glEnd();
	glPopAttrib();
	glEnable(GL_BLEND);
}

void GRenderManager::drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
	drawBatchedTris();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(mLineR, mLineG, mLineB, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();
	glPopAttrib();
	glEnable(GL_BLEND);
}


uint32 GRenderManager::loadTexture(const unsigned char* bits, uint32 textureWidth, uint32 textureHegih)
{
	int   glFormt = GL_RGBA;

	GThreadSafeErrors err;
	unsigned int glTextureID;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &glTextureID);
	/*if (checkForGLErrors(err) > 0)
	{
		return -1;
	}*/

	glBindTexture(GL_TEXTURE_2D, glTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

/*	if (checkForGLErrors(err) > 0)
		return -1;*/

	glTexImage2D(GL_TEXTURE_2D, 0, glFormt, textureWidth, textureHegih, 0, glFormt, GL_UNSIGNED_BYTE, bits);

/*	if (checkForGLErrors(err) > 0)
		return -1;*/

	return glTextureID;
}

void  GRenderManager::unloadTexture(uint32 textureId)
{
	glDeleteTextures(1, &textureId);
}

int GRenderManager::checkForGLErrors(GThreadSafeErrors &err)
{
	int errors = 0;

	while (true)
	{
		GLenum x = glGetError();
		if (x == GL_NO_ERROR)
			return errors;

		std::string aError;

		switch (x)
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
			sprintf(str, "GL_ERROR = %d", (int)x);
			aError = str;
		}
		}

		err.push(std::string("checkForGLErrors> ") + aError);
		errors++;
	}

	return errors;
}