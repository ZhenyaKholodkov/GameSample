#ifndef _GRENDER_H
#define _GRENDER_H

#include <windows.h>
#include <gl/gl.h>
#include "Types.h"

class GRender
{
public:

	class PlatformMatrix {
	public:
		PlatformMatrix();
		float m[4][4];
		void SetIdentity();
	};

	enum TexturePixType
	{
		TPT_BGRA_8888,
		TPT_ABGR_8888,
		TPT_BGR_888,
		TPT_A_8,
		TPT_COMPRESS_BC3,
		TPT_COMPRESS_PVRTC,
		TPT_COMPRESS_ETC1
	};

	GRender::GRender();

	void		init(int w, int h);										      //!< ���������������� �������� ���������
	void		setClearColor(unsigned int c);							      //!< ���������� ���� �������
	void		clear();													            //!< �������� ���

	void		drawTri(float x1, float y1,
		float x2, float y2,
		float x3, float y3);								      //!< ���������� ������� �����������

	void		drawLine(float x1, float y1, float z1, float x2, float y2, float z2);	//!< ���������� ����� (����� ��������� - x1,y1,z1 � x2,y2,z2)

	int      createTexture(TexturePixType format,
		unsigned char* data,
		int texWidth, int texHeight,
		int fid,
		bool upload = true);	               //!< �������� �������� �� ������
	void		destroyTexture(unsigned int glId, bool unload = true);	//!< �������� ��������
	void		setClipRect(int aX, int aY, int aWidth, int aHeight);	//!< ���������� ������������� ���������
	void		clearClipRect();											         //!< ������ ����� ���������
    void		clearDepthBuffer();	   			                        //!< �������� Z-�����  
	void  applyTransform();
protected:
	void		drawBatchedTris();											      //!< ������ ����� �������������

private:
	uint32				      _cacheVertIndex;								//!< ��������� �� ������������ ���� ���� ������
	uint32				      _batchVertIndex;								//!< ���������� ����� ������� � ������ ���������  
	int                       _vertexes;                             //!< ����� ������ � ����
	int			            _currentTextureID;							//!< ������� �������� ��������, ������� ������

	IGMatrix 	            _matrix;											//!< ������� ������������� ������� ������

	GLint                   _glMajorVer;                              //!< �������� ������ OpenGL
	GLint                   _glMinorVer;                              //!< ��������� ������ OpenGL

	int                     _width;
	int                     _height;

	float   _clearColorR ;
	float	_clearColorG ;
	float	_clearColorB ;


};

#endif