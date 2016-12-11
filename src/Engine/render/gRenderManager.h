#ifndef GRENDERMANAGER_H
#define GRENDERMANAGER_H

#include <vector>
#include <stack>
#include <stdio.h>
#include "Types.h"



#define DEGREE_TO_RAD 0.0174532925199f                    

class GRenderManager
{
public:

	GRenderManager();
	virtual ~GRenderManager() {}

	static GRenderManager* Instance();

	void	init(int w, int h);
	void	setClearColor(unsigned int c);
	void	clear();
	void	startFrame();
	void	endFrame();
	void	draw();
	void	drawTri(float x1, float y1, float x2, float y2, float x3, float y3);
	void	drawLine(float x1, float y1, float z1, float x2, float y2, float z2);

	uint32  loadTexture(const unsigned char* bits, uint32 textureWidth, uint32 textureHegih); // loads texture to VRAM and returns the GL id.
	void    unloadTexture(uint32 textureId);                                                  // unloads the texture with GL id from VRAM.


	void	loadIdentityMatrix();
	void	save();                              // puts the current matrix to stack
	void	restore();                           // gets the current matrix to stack

	void	translate(float x, float y, float z = 0.0f);
	void	rotate(float ang);
	void	rotate(float angX, float angY, float angZ);
	void	scale(float x, float y, float z = 1.0f);
	void	drawImage(uint32 textureId, int full_tex_w, int full_tex_h,
		              int tex_x, int tex_y, int tex_w, int tex_h,
		              float x, float y, float z = 0.0f);                 // calculates vertexes, texture coordanates and pushes it to array for rendering



	GMatrix getTransform() { return mMatrix; }

	int		width() { return mWidth; }
	int		height() { return mHeight; }

	void  	setTransform(GMatrix val) { mMatrix = val; }
	void	setLineColor(float r, float g, float b);
	void	setLineColor(uint32 c);
	void	setFillColor(float r, float g, float b);
	void	setFillColor(uint32 c);

private:
	GMatrix 	           mMatrix;       

	std::stack<GMatrix>   mMatrixStack;     
						   
	std::vector<GVector3> mVertexData;                 // vertexes for rendering
	std::vector<GVector2> mTextCoordData;              // texture coordinates for rendering
	std::vector<uint16>    mIndexesData;

	float                  mClearColorR;
	float                  mClearColorG;
	float                  mClearColorB;
	const	uint32		   BATCHED_VERTEXES_SIZE;
	uint32				   mCacheVertIndex;
	uint32				   mBatchVertIndex;
	int                    mVertexes;

	int			           mCurrentTextureID;        // current texture for rendering . If there are few textures we will switch them while rendering sprites from different textures.

	int					   mWidth;
	int					   mHeight;

	float				   mFillR, mFillG, mFillB;
	float				   mLineR, mLineG, mLineB;

private:
	bool visible();
	void addBatchTris(int triCnt);
	void drawBatchedTris();                       // draws the couple of triangle with mVertexData and mTextCoordData

	void allocVertexCache();
	void reallocVertexCache();

	void convertVertexDataToModelSpace(GVector3 &vertex)	const;
	bool testOutsideScreen(GVector3 &vertex0, GVector3 &vertex2) const;

	int  checkForGLErrors(GThreadSafeErrors &err);
};


#endif
