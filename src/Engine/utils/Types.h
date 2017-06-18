#ifndef TEPYS_H
#define TEPYS_H

#include <stdlib.h>    
#include <math.h>
#include <string>
#include <vector>

using namespace std;

#define PI 3.141592653589793f
#define DEGREE_TO_RAD 0.0174532925199f 
#define MAX_ENTITY_COUNT 300
#define INVALID_ENTITY (MAX_ENTITY_COUNT + 1)

#define BIT(x) (1 << (x))   
#define SAFE_DELETE(x) { if(x){ delete x; x = nullptr; }}

typedef enum  GKey
{
	KEY_NONE  = BIT(1),
	KEY_UP    = BIT(2),
	KEY_DOWN  = BIT(3),
	KEY_LEFT  = BIT(4),
	KEY_RIGHT = BIT(5),
}GKey;

typedef int           int32;
typedef __int64       int64; 

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;
typedef unsigned long    ulong32;

typedef size_t Entity;

template <class T> inline T    Min(const T &x, const T &y) { return x<y ? x : y; }
template <class T> inline T    Max(const T &x, const T &y) { return x>y ? x : y; }
template <class T> inline T    Sqr(const T &x) { return x*x; }
template <class T> inline void Swap(T &x, T &y) { T z = x; x = y; y = z; }

class Rect;

struct GObjectRect
{
	GObjectRect() :
		mXPos(0), mYPos(0), mWidth(0), mHeight(0), mPivotX(0.0f), mPivotY(0.0f) {};

	GObjectRect(int xPos, int yPos, int width, int height, float pivotX, float pivotY) :
		mXPos(xPos), mYPos(yPos), mWidth(width), mHeight(height), mPivotX(pivotX), mPivotY(pivotY) {};

	int   mXPos;
	int   mYPos;
	int   mWidth;
	int   mHeight;
	float mPivotX;
	float mPivotY;
};

struct GColor
{
	float   r;  
	float   g;  
	float   b;
	GColor() :r(0), g(0), b(0) {}
	GColor(float red, float green, float blue) : r(red), g(green), b(blue) {}
	GColor(uint32 rgba) : r(((rgba >> 24) & 0xFF) / 255.0f), g(((rgba >> 16) & 0xFF) / 255.0f), b(((rgba >> 8) & 0xFF) / 255.0f) {}
};

class IGColor     
{
public:
	uint8   r;     //!< 8-bit red component.
	uint8   g;     //!< 8-bit green component.
	uint8   b;     //!< 8-bit blue component.
	uint8   a;     //!< 8-bit alpha component. 
	void set(uint8 rV, uint8 gV, uint8 bV, uint8 aV) { r = rV;g = gV;b = bV;a = aV; }
	void set(uint32 rgba) { *(uint32*)this = rgba; }
	uint32 getColor() { return  *(uint32*)this;	}
	IGColor() :r(0), g(0), b(0), a(0) {}
	IGColor(uint8 red, uint8 green, uint8 blue, uint8 alpha = 0) : r(red), g(green), b(blue), a(alpha) {}
	IGColor(uint32 rgba) : r((rgba >> 24) & 0xFF), g((rgba >> 16) & 0xFF), b((rgba >> 8) & 0xFF), a(rgba & 0xFF) {}
	static uint32  colorToUintARGB(uint8 r, uint8 g, uint8 b, uint8 a = 0) { return (a << 24) | (r << 16) | (g << 8) | (b); }
	static IGColor uintrgbaToColor(uint32 col) { return IGColor((col >> 24) & 0xFF, (col >> 16) & 0xFF, (col >> 8) & 0xFF, (col & 0xFF)); }
	static IGColor uintargbToColor(uint32 col) { return IGColor((col & 0xFF), (col >> 8) & 0xFF, (col >> 16) & 0xFF, ((col >> 24) & 0xFF)); }

};

class GThreadSafeErrors
{
public:
	void push(const std::string& aError)
	{
		mErrorsList.push_back(aError);
	}

	int size()
	{
		return mErrorsList.size();
	}

	void clear()
	{
		mErrorsList.clear();
	}

	std::string& operator [](int i)
	{
		return mErrorsList[i];
	}

private:
	std::vector<std::string> mErrorsList;
};


struct GCursor
{
	float x, y;  
	
	bool mWasPressed;

	GCursor() : x(0.0f), y(0.0f), mWasPressed(false) {}
	GCursor(float aX, float aY) : x(aX), y(aY), mWasPressed(false) {}
	GCursor(float aX, float aY, bool pressed) : x(aX), y(aY), mWasPressed(pressed) {}
};

struct  Pixel
{
	Pixel() {}
	Pixel(int32 xx, int32 yy) { x = xx; y = yy; }

	int32   X() { return x; }                
	int32   Y() { return y; }                
	void   X(int32 xx) { x = xx; }           
	void   Y(int32 yy) { y = yy; }           

	int32   W() { return x; }                
	int32   H() { return y; }                
	void   W(int32 w) { x = w; }             
	void   H(int32 h) { y = h; }             

	void Set() { x = 0; y = 0; }             
	void Set(int32 xx, int32 yy) { x = xx;  y = yy; }            

	void Shift(int32 dx, int32 dy) { x += dx; y += dy; }    
	void ShiftX(int32 dx) { x += dx; }                      
	void ShiftY(int32 dy) { y += dy; }                      

	bool Null() { return x == 0 && y == 0; }     

	friend bool operator ==(const Pixel &p1, const Pixel &p2) { return p1.x == p2.x && p1.y == p2.y; }
	friend bool operator !=(const Pixel &p1, const Pixel &p2) { return p1.x != p2.x || p1.y != p2.y; }
	friend bool operator < (const Pixel &p1, const Pixel &p2) { return p1.x<p2.x  && p1.y<p2.y; }
	friend bool operator > (const Pixel &p1, const Pixel &p2) { return p1.x>p2.x  && p1.y>p2.y; }
	friend bool operator <=(const Pixel &p1, const Pixel &p2) { return p1.x <= p2.x && p1.y <= p2.y; }
	friend bool operator >=(const Pixel &p1, const Pixel &p2) { return p1.x >= p2.x && p1.y >= p2.y; }

	Pixel operator +  (const Pixel &p) { return Pixel(x + p.x, y + p.y); }
	Pixel operator -  (const Pixel &p) { return Pixel(x - p.x, y - p.y); }
	int32    operator *  (const Pixel &a) { return x*a.x + y*a.y; }
	void   operator += (const Pixel &p) { x += p.x;  y += p.y; }
	void   operator -= (const Pixel &p) { x -= p.x;  y -= p.y; }

	Pixel operator *  (const int32   &a) { return Pixel(x*a, y*a); }
	Pixel operator /  (const int32   &a) { return Pixel(x / a, y / a); }
	void   operator *= (const int32   &a) { x *= a;    y *= a; }
	void   operator /= (const int32   &a) { x /= a;    y /= a; }

	int32 Norm(Pixel p1, Pixel p2)
	{
		Pixel dp = p2 - p1, dp0 = *this - p1; int32 dp2 = dp*dp;
		if (dp2 == 0) return dp0*dp0; 
		else {
			int32 t = (1000 * (dp0*dp)) / dp2;
			if (t<0)        return (*this - p1)*(*this - p1);              
			else if (t>1000)     return (*this - p2)*(*this - p2);         
			else                return (int32)sqrt(dp.x*dp0.y - dp.y*dp0.x) / dp2;
		}
	}

	friend int32 DistManh(Pixel p1, Pixel p2) { return abs(p2.X() - p1.X()) + abs(p2.Y() - p1.Y()); } 
	friend int32 DistSqr(Pixel p1, Pixel p2) { return Sqr(p2.X() - p1.X()) + Sqr(p2.Y() - p1.Y()); }  

	friend class Rect;
private:
	int32 x, y;                                                 
};

class Rect
{
public:
	Rect() {                     }
	Rect(int32 x1, int32 y1, int32 x2, int32 y2) { Set(x1, y1, x2, y2); }

	void Set() { x1 = x2 = y1 = y2 = 0; }                   
	void Set(const Rect  &r) { x1 = r.x1; y1 = r.y1; x2 = r.x2; y2 = r.y2; }
	void Set(const Pixel &p1, const Pixel &p2) { x1 = p1.x; y1 = p1.x; x2 = p2.x; y2 = p2.x; }
	void Set(int32 xx1, int32 yy1, int32 xx2, int32 yy2) { x1 = xx1;  y1 = yy1;  x2 = xx2;  y2 = yy2; }

	int32   W() { return abs(x2 - x1); }                     
	int32   H() { return abs(y2 - y1); }                   

	int32   X1() { return x1; }                             
	int32   Y1() { return y1; }                             
	int32   X2() { return x2; }                             
	int32   Y2() { return y2; }                             

	void   X1(int32 xx1) { x1 = xx1; }                      
	void   Y1(int32 yy1) { y1 = yy1; }                      
	void   X2(int32 xx2) { x2 = xx2; }                      
	void   Y2(int32 yy2) { y2 = yy2; }     

	bool   In(const Pixel &p) { return x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2; }
	bool   Empty() { return x1 >= x2 || y1 >= y2; }                         
		
private:
	int32 x1, y1;                
	int32 x2, y2;                
};


class GVector3
{
public:
	GVector3() {}
	GVector3(float xV, float yV, float zV) :x(xV), y(yV), z(zV) {}
	float x;             
	float y;             
	float z;               
	bool operator == (GVector3 const & v) const
	{
		return (
			v.x == x &&
			v.y == y &&
			v.z == z &&
			true);
	}
};

class GVector2
{
public:
	GVector2() {}
	GVector2(float xV, float yV) :x(xV), y(yV) {}
	float x;             
	float y;             
	bool operator == (GVector2 const & v) const
	{
		return (
			v.x == x &&
			v.y == y &&
			true);
	}
};


class GMatrix        
{
public:
	float             m[3][3];
	GVector3         t;
	void              setIdentity();
	void              SetRotX(float r = 0, bool resetTrans = true, bool setZeros = true);
	void              SetRotY(float r = 0, bool resetTrans = true, bool setZeros = true);
	void              SetRotZ(float r = 0, bool resetTrans = true, bool setZeros = true);
	static GMatrix   matMul(GMatrix const & A, GMatrix const & B);	                          
	static GVector3  matVecMul(GMatrix const & A, GVector3 const & V);	                      
};
#endif // !TEPYS_H