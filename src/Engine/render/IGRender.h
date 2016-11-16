#ifndef _IGRENDER_ABS_H
#define _IGRENDER_ABS_H

#include <vector>
#include <stack>
#include <stdio.h>
#include "Types.h"

#define DEGREE_TO_RAD 0.0174532925199f                    

class IGRender
{
public:

	IGRender();
	virtual ~IGRender() {}

	static IGRender* Create();
	static IGRender* Instance() { return _instance; }

	virtual	void	init(int w, int h) = 0;
	virtual	void	setClearColor(unsigned int c) = 0;
	virtual	void	clear() = 0;
	virtual	void	startFrame();
	virtual	void	endFrame();
	virtual	void	draw() = 0;
	virtual	void	drawTri(float x1, float y1, float x2, float y2, float x3, float y3) = 0;

	virtual	void	drawLine(float x1, float y1, float z1, float x2, float y2, float z2) = 0;
	virtual	void	applyTransform() = 0;
	virtual void   restoreContext() {}
	virtual uint32 LoadTexture(const unsigned char* bits, uint32 textureWidth, uint32 textureHegih) = 0;
	virtual void   UnloadTexture(uint32 textureId) = 0;
	int		width() { return _width; }
	int		height() { return _height; }

	void  skipFrame(bool val);
	bool  skipFrame() { return _skipFrame; }
	int   frameNumber() { return _frameNumber; }
	void	loadIdentityMatrix();
	void	save();
	void	restore();
	void	translate(float x, float y, float z = 0.0f);
	void	rotate(float ang);
	void	rotate(float angX, float angY, float angZ);
	void	scale(float x, float y, float z = 1.0f);
	void	drawImage(unsigned int textureId,
		int full_tex_w, int full_tex_h,
		int tex_x, int tex_y, int tex_w, int tex_h,
		float x, float y, float z = 0.0f,
		UInt32 color0 = 0xffffffff,
		UInt32 color1 = 0xffffffff,
		UInt32 color2 = 0xffffffff,
		UInt32 color3 = 0xffffffff
		);
	void  drawRect(float x, float y, float w, float h);
	void  drawRect(float x, float y, float w, float h,
		UInt32 color0, UInt32 color1, UInt32 color2, UInt32 color3);

	float	getFontR() const { return _fontR; }
	float	getFontG() const { return _fontG; }
	float	getFontB() const { return _fontB; }
	float	getFontA() const { return _fontA; }
	IGMatrix	getTransform() { return _matrix; }
	void  	setTransform(IGMatrix val) { _matrix = val; }
	float	globalR() const { return _globalR; }
	float	globalG() const { return _globalG; }
	float	globalB() const { return _globalB; }
	float	globalAlpha() const { return _globalAlpha; }
	void	setGlobalR(float val) { _globalR = val; }
	void	setGlobalG(float val) { _globalG = val; }
	void	setGlobalB(float val) { _globalB = val; }
	void	setGlobalAlpha(float alpha) { _globalAlpha = alpha; }
	void	setLineColor(float r, float g, float b);
	void	setLineColor(UInt32 c);
	void	setFillColor(float r, float g, float b);
	void	setFillColor(UInt32 c);

protected:

	IGMatrix 	            _matrix;
	IGMatrix 	            _batchMatrix;
	float				         _globalAlpha;
	bool     			      _drawDepthTest;
	bool     			      _curDepthTest;

	std::stack<IGMatrix>	      _matrixStack;
	std::stack<float>		      _alphaStack;
	std::vector<bool>            _depthTestStack;


	bool                    _isDraw;

	const	UInt32		      _BATCHED_VERTEXES_SIZE;
	UInt32				      _cacheVertIndex;
	UInt32				      _batchVertIndex;
	int                     _vertexes;
	std::vector<IGVector3>			_vertexData;
	std::vector<IGVector2>		   _uvData;
	std::vector<IGColor>			_colourData;
	std::vector<UInt16>          _indexesData;

	int			            _currentTextureID;

	bool              _skipFrame;
	UInt32            _frameNumber;
	int					_width;
	int					_height;
	UInt32				_clearColor;
	float				   _globalR;
	float				   _globalG;
	float				   _globalB;
	float				   _fontR, _fontG, _fontB, _fontA;
	float				   _fillR, _fillG, _fillB;
	float				   _lineR, _lineG, _lineB;
	UInt32				_fontColor;

	bool                 visible();
	void				      addBatchTris(int triCnt);
	void				      drawRect(float x, float y, float w, float h,
		IGColor color0,
		IGColor color1,
		IGColor color2,
		IGColor color3);
	virtual void			drawBatchedTris() = 0;

protected:
	void				         convertVertexDataToModelSpace(IGVector3 &vertex)	const;
	bool				         testOutsideScreen(IGVector3 &vertex0, IGVector3 &vertex2) const;

private:
	static IGRender*	      _instance;
	void				         allocVertexCache();
	void				         reallocVertexCache();
};


inline void	IGRender::convertVertexDataToModelSpace(IGVector3 &vertex) const						
{	
   IGVector3 newVertexData;
   newVertexData.x  = _matrix.m[0][0]*vertex.x + _matrix.m[1][0]*vertex.y + _matrix.t.x;
   newVertexData.y  = _matrix.m[0][1]*vertex.x + _matrix.m[1][1]*vertex.y + _matrix.t.y;
   newVertexData.z  = _matrix.m[0][2]*vertex.x + _matrix.m[1][2]*vertex.y + _matrix.t.z; 

   if (vertex.z!=0.0f)		
   {	
      newVertexData.x += _matrix.m[2][0]*vertex.z;
      newVertexData.y += _matrix.m[2][1]*vertex.z;
      newVertexData.z += _matrix.m[2][2]*vertex.z;
   }
   vertex = newVertexData;
}

inline bool	IGRender::testOutsideScreen(IGVector3 &vertex0, IGVector3 &vertex2) const					
{	
   return (
         (vertex0.x < 0.0f		&& vertex2.x < 0.0f)	   ||	
         (vertex0.x > _width	&& vertex2.x > _width)	||
         (vertex0.y < 0.0f		&& vertex2.y < 0.0f)	   ||
         (vertex0.y > _height	&& vertex2.y > _height)
      );
}


#endif
//========================================================================================
