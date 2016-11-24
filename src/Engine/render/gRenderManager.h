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

	uint32  loadTexture(const unsigned char* bits, uint32 textureWidth, uint32 textureHegih);
	void    unloadTexture(uint32 textureId);


	void	loadIdentityMatrix();
	void	save();
	void	restore();

	void	translate(float x, float y, float z = 0.0f);
	void	rotate(float ang);
	void	rotate(float angX, float angY, float angZ);
	void	scale(float x, float y, float z = 1.0f);
	void	drawImage(uint32 textureId, int full_tex_w, int full_tex_h,
		              int tex_x, int tex_y, int tex_w, int tex_h,
		              float x, float y, float z = 0.0f);



	IGMatrix getTransform() { return mMatrix; }

	int		width() { return mWidth; }
	int		height() { return mHeight; }

	void  	setTransform(IGMatrix val) { mMatrix = val; }
	void	setLineColor(float r, float g, float b);
	void	setLineColor(UInt32 c);
	void	setFillColor(float r, float g, float b);
	void	setFillColor(UInt32 c);

private:
	IGMatrix 	           mMatrix;

	std::stack<IGMatrix>   mMatrixStack;
						   
	std::vector<IGVector3> mVertexData;
	std::vector<IGVector2> mTextCoordData;
	std::vector<UInt16>    mIndexesData;

	float                  mClearColorR;
	float                  mClearColorG;
	float                  mClearColorB;
	const	UInt32		   BATCHED_VERTEXES_SIZE;
	UInt32				   mCacheVertIndex;
	UInt32				   mBatchVertIndex;
	int                    mVertexes;

	int			           mCurrentTextureID;

	int					   mWidth;
	int					   mHeight;

	float				   mFillR, mFillG, mFillB;
	float				   mLineR, mLineG, mLineB;

private:
	bool visible();
	void addBatchTris(int triCnt);
	void drawBatchedTris();

	void allocVertexCache();
	void reallocVertexCache();

	void convertVertexDataToModelSpace(IGVector3 &vertex)	const;
	bool testOutsideScreen(IGVector3 &vertex0, IGVector3 &vertex2) const;

	int  checkForGLErrors(GThreadSafeErrors &err);
};


#endif
