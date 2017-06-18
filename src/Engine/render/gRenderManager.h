#ifndef GRENDERMANAGER_H
#define GRENDERMANAGER_H

#include <memory>
#include <vector>
#include <stack>
#include <stdio.h>
#include "Types.h"
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>


class GSprite;

#define DEGREE_TO_RAD 0.0174532925199f   
#define VERTEXES_COUNT 6;

class GRenderManager
{
public:
	GRenderManager();
	virtual ~GRenderManager() {}

	static GRenderManager* Instance();

	void	init(int w, int h);
	void	setClearColor(unsigned int c);
	void	startFrame() const;
	void	endFrame();
	void	drawLine(GVector3 posLeft, GVector3 posRight, IGColor color);
	void	drawSprite(GSprite* sprite);                 // calculates vertexes, texture coordanates and pushes it to array for rendering
	void    drawText(std::string text, GColor color, uint32 fontSize = 24);
	uint32  loadTexture(const unsigned char* bits, uint32 textureWidth, uint32 textureHegih); // loads texture to VRAM and returns the GL id.
	void    unloadTexture(uint32 textureId);                                                  // unloads the texture with GL id from VRAM.

	void	save();                              // puts the current matrix to stack
	void	restore();                           // gets the current matrix to stack

	void	translate(float x, float y, float z = 0.0f);
	void	rotate(float ang);
	void	rotate(float angX, float angY, float angZ);
	void	scale(float x, float y, float z = 1.0f);

	int		width() const  { return mWidth; }
	int		height() const { return mHeight; }

	GMatrix getTransform() const      { return mMatrix; }
	void  	setTransform(GMatrix val) { mMatrix = val; }

private:
	GMatrix 	          mMatrix;       

	std::stack<GMatrix>   mMatrixStack;
						   
	std::vector<GVector3> mVertexData;                 // vertexes for rendering
	std::vector<GVector2> mTextCoordData;              // texture coordinates for rendering		
	std::vector<uint16>   mIndexesData;

	float                 mClearColorR;
	float                 mClearColorG;
	float                 mClearColorB;

	const	uint32		  BATCHED_VERTEXES_SIZE;
	uint32				  mCacheVertLastIndex;
	uint32				  mBatchVertLastIndex;
	int                   mVertexes;
						  
	int			          mCurrentTextureID;        // current texture for rendering . If there are few textures we will switch them while rendering sprites from different textures.
						  
	int					  mWidth;
	int					  mHeight;

	/*text rendering */
private:
	void clear() const;
	bool visible() const;
	void convertVertexDataToModelSpace(GVector3 &vertex)	const;
	int  checkForGLErrors(GThreadSafeErrors &err) const;

	void loadIdentityMatrix();

	void drawBatchedTris();                       // draws the couple of triangle with mVertexData and mTextCoordData
	void drawAll(); 

	void allocVertexCache();
	void reallocVertexCache();
};


#endif
