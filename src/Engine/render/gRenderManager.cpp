#include "gRenderManager.h"

#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include "GSprite.h"
#include "Utils.h"


#define GL_GENERATE_MIPMAP                0x8191

GRenderManager* GRenderManager::Instance()
{
	static GRenderManager instance;
	return &instance;
}

GRenderManager::GRenderManager():
mWidth( 0 ),
mHeight( 0 ),
mClearColorR(1.0f),
mClearColorG(1.0f),
mClearColorB(1.0f),
mVertexes(32000),
mCacheVertLastIndex(0),
mCurrentTextureID(-1),
mBatchVertLastIndex(0),
BATCHED_VERTEXES_SIZE(4092)
{
   allocVertexCache();
   int val = 0;
}

void GRenderManager::init( int w, int h )
{
    mWidth = w;
	mHeight = h;

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

	glOrtho(0, (float)mWidth, 0, (float)mHeight, 1, -1);

	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void GRenderManager::startFrame() const
{
	clear();
}

void GRenderManager::endFrame()
{
	drawAll();
	mCurrentTextureID = -1;
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

   GMatrix		rotZ;
   rotZ.SetRotZ( -ang * DEGREE_TO_RAD );

   mMatrix = GMatrix::matMul(rotZ, mMatrix);
}

void GRenderManager::rotate( float angX, float angY, float angZ )
{
   if (angX!=0.0f)
   {
      GMatrix		rotX;
      rotX.SetRotX( -angX * DEGREE_TO_RAD );

      mMatrix = GMatrix::matMul(rotX, mMatrix);
   }

   if (angY!=0.0f)
   {
      GMatrix		rotY;
      rotY.SetRotY( -angY * DEGREE_TO_RAD );

      mMatrix = GMatrix::matMul(rotY, mMatrix);
   }

   if (angZ!=0.0f)
   {
      GMatrix		rotZ;
      rotZ.SetRotZ( -angZ * DEGREE_TO_RAD );

      mMatrix = GMatrix::matMul(rotZ, mMatrix);
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

bool GRenderManager::visible() const
{
   if (fabsf(mMatrix.m[0][0]*mMatrix.m[1][1] - mMatrix.m[0][1]*mMatrix.m[1][0])<0.0001f)
      return false;

   return true;
}

void GRenderManager::drawSprite(GSprite* sprite)
{
	if (!visible())
		return;

	uint32 textureId = sprite->getTextureGLId();
	if (mCurrentTextureID != (int)textureId)
	{
		drawBatchedTris();
		mCurrentTextureID = textureId;
	}

	uint32 textureWidth = sprite->getTextureWidth();
	uint32 textureHeight = sprite->getTextureHeight();
	float imageXPos = (float)sprite->getXPos();
	float imageYPos = (float)sprite->getYPos();
	float imageWidth = (float)sprite->getWidth();
	float imageHeight = (float)sprite->getHeight();
	float x = -sprite->getPivotX();
	float y = -sprite->getPivotY();

	mVertexData[mCacheVertLastIndex].x = x;
	mVertexData[mCacheVertLastIndex].y = y;
	mVertexData[mCacheVertLastIndex].z = 0;
	mVertexData[mCacheVertLastIndex + 1].x = x;
	mVertexData[mCacheVertLastIndex + 1].y = y + imageHeight;
	mVertexData[mCacheVertLastIndex + 1].z = 0;
	mVertexData[mCacheVertLastIndex + 2].x = x + imageWidth;
	mVertexData[mCacheVertLastIndex + 2].y = y + imageHeight;
	mVertexData[mCacheVertLastIndex + 2].z = 0;
	mVertexData[mCacheVertLastIndex + 5].x = x + imageWidth;
	mVertexData[mCacheVertLastIndex + 5].y = y;
	mVertexData[mCacheVertLastIndex + 5].z = 0;

	convertVertexDataToModelSpace(mVertexData[mCacheVertLastIndex]);
	convertVertexDataToModelSpace(mVertexData[mCacheVertLastIndex + 1]);
	convertVertexDataToModelSpace(mVertexData[mCacheVertLastIndex + 2]);
	convertVertexDataToModelSpace(mVertexData[mCacheVertLastIndex + 5]);

	mVertexData[mCacheVertLastIndex + 3] = mVertexData[mCacheVertLastIndex];
	mVertexData[mCacheVertLastIndex + 4] = mVertexData[mCacheVertLastIndex + 2];

	float texLeft = imageXPos / textureWidth;
	float texUp = imageYPos / textureHeight;
	float texRight = (imageWidth + imageXPos) / textureWidth;
	float texDown = (imageHeight + imageYPos) / textureHeight;

	mTextCoordData[mCacheVertLastIndex].x = mTextCoordData[mCacheVertLastIndex + 3].x = texLeft;
	mTextCoordData[mCacheVertLastIndex].y = mTextCoordData[mCacheVertLastIndex + 3].y = texUp;
	mTextCoordData[mCacheVertLastIndex + 1].x = texLeft;
	mTextCoordData[mCacheVertLastIndex + 1].y = texDown;
	mTextCoordData[mCacheVertLastIndex + 2].x = mTextCoordData[mCacheVertLastIndex + 4].x = texRight;
	mTextCoordData[mCacheVertLastIndex + 2].y = mTextCoordData[mCacheVertLastIndex + 4].y = texDown;
	mTextCoordData[mCacheVertLastIndex + 5].x = texRight;
	mTextCoordData[mCacheVertLastIndex + 5].y = texUp;

	mIndexesData[mCacheVertLastIndex] = mBatchVertLastIndex++;
	mIndexesData[mCacheVertLastIndex + 1] = mBatchVertLastIndex++;
	mIndexesData[mCacheVertLastIndex + 2] = mBatchVertLastIndex++;
	mIndexesData[mCacheVertLastIndex + 3] = mBatchVertLastIndex++;
	mIndexesData[mCacheVertLastIndex + 4] = mBatchVertLastIndex++;
	mIndexesData[mCacheVertLastIndex + 5] = mBatchVertLastIndex++;

	mCacheVertLastIndex += 6;

	if (mCacheVertLastIndex + 6 >= mVertexData.size())
	{
		reallocVertexCache();
	}
}

void GRenderManager::allocVertexCache()
{
   mVertexData.resize(mVertexes);
   mTextCoordData.resize(mVertexes);
   mIndexesData.resize(mVertexes);
}

void GRenderManager::reallocVertexCache()
{
	drawBatchedTris();
	mVertexes *= 2;
	allocVertexCache();
}

void GRenderManager::convertVertexDataToModelSpace(GVector3 &vertex) const
{
	GVector3 newVertexData;
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


void GRenderManager::setClearColor(unsigned int c)
{
	mClearColorR = ((c >> 16) & 0xFF) / 255.0f;
	mClearColorG = ((c >> 8) & 0xFF) / 255.0f;
	mClearColorB = ((c) & 0xFF) / 255.0f;

	glClearColor(mClearColorR, mClearColorG, mClearColorB, 1);
}

void GRenderManager::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GRenderManager::drawAll()
{
	drawBatchedTris();
	mCacheVertLastIndex = 0;
}

void GRenderManager::drawBatchedTris()
{
	if (mBatchVertLastIndex == 0)
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

	int vertexFirstIndex = mCacheVertLastIndex - mBatchVertLastIndex;

	save();

	glDisable(GL_LIGHTING);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, &mTextCoordData[vertexFirstIndex]);

	glDisableClientState(GL_NORMAL_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &mVertexData[vertexFirstIndex]);

	glDrawElements(GL_TRIANGLES, mBatchVertLastIndex, GL_UNSIGNED_SHORT, &mIndexesData[vertexFirstIndex]);

	if (mCurrentTextureID >= 0)
	{
		glDisable(GL_TEXTURE_2D);
	}

	restore();

	mCurrentTextureID = -1;
	mBatchVertLastIndex = 0;
}

void GRenderManager::drawLine(GVector3 posLeft, GVector3 posRight, IGColor color)
{
	drawBatchedTris();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(posLeft.x, posLeft.y, posLeft.z);
	glVertex3f(posRight.x, posRight.y, posRight.z);
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

	glBindTexture(GL_TEXTURE_2D, glTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	glTexImage2D(GL_TEXTURE_2D, 0, glFormt, textureWidth, textureHegih, 0, glFormt, GL_UNSIGNED_BYTE, bits);

	return glTextureID;
}

void  GRenderManager::unloadTexture(uint32 textureId)
{
	glDeleteTextures(1, &textureId);
}

int GRenderManager::checkForGLErrors(GThreadSafeErrors &err) const
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