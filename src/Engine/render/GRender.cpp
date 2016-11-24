#include "GRender.h"
/*#include "ext/wglext.h"
#include "ext/olOpenGL.h"

IGRender* IGRender::Create()
{
   _instance = new GRender();
   return _instance;
}


GRender::GRender() :
mClearColorR(1.0f),
mClearColorG(1.0f),
mClearColorB(1.0f)
{
}

void GRender::init(int w, int h)
{
	IGRender::init(w, h);

	GThreadSafeErrors err;

	glViewport(0, 0, mWidth, mHeight);

	glClearColor(100, 100, 100, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	loadIdentityMatrix();

	const GLfloat matrix[4][4] = {
	   {1.0f,   0.0f, 0.0f, 0.0f},
	   {0.0f,   -1.0f, 0.0f, 0.0f},
	   {0.0f,   0.0f, 1.0f, 0.0f},
	   {0.0f,   0.0f, 0.0f, 1.0f} };

	glLoadMatrixf((GLfloat*)matrix);

	glOrtho(0, (float)mWidth, 0, (float)mHeight, 2000, -50000);

	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void GRender::setClearColor(unsigned int c)
{
	mClearColorR = ((c >> 16) & 0xFF) / 255.0f;
	mClearColorG = ((c >> 8) & 0xFF) / 255.0f;
	mClearColorB = ((c) & 0xFF) / 255.0f;

	glClearColor(mClearColorR, mClearColorG, mClearColorB, 1);
}

void GRender::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GRender::drawBatchedTris()
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

void GRender::drawTri(float x1, float y1, float x2, float y2, float x3, float y3)
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

void GRender::drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
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


uint32 GRender::loadTexture(const unsigned char* bits, uint32 textureWidth, uint32 textureHegih)
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

void  GRender::unloadTexture(uint32 textureId)
{
	glDeleteTextures(1, &textureId);
}

void GRender::startFrame()
{
	IGRender::startFrame();
	clear();
}

void GRender::endFrame()
{
	draw();
}

void GRender::draw()
{
	drawBatchedTris();

	mCacheVertIndex = 0;
}*/