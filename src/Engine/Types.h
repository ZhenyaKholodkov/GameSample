#ifndef TEPYS_H
#define TEPYS_H

#include <stdlib.h>    
#include <math.h>
#include <string>
#include <vector>

using namespace std;

#define WindowMutex "GameMutex"
#define WindowCaption "Game"

#define BIT(x) (1 << (x))   
#define SAFE_DELETE(x) { if(x){ delete x; x = nullptr; }}


typedef signed int int32;
typedef unsigned int uint32;
typedef unsigned long ulong32;
typedef int      Int32;
typedef unsigned char       uint8;
typedef unsigned char       UInt8;
typedef __int64             Int64;

typedef unsigned short      UInt16;
typedef unsigned int        UInt32;
typedef unsigned __int64    UInt64;

typedef uint32 Entity;

template <class T> inline T    Min(const T &x, const T &y) { return x<y ? x : y; }
template <class T> inline T    Max(const T &x, const T &y) { return x>y ? x : y; }
template <class T> inline T    Sqr(const T &x) { return x*x; }
template <class T> inline void Swap(T &x, T &y) { T z = x; x = y; y = z; }

class Rect;

//void closeApplication()
//{
//	exit(0);
//}

class IGColor     
{
public:
	uint8   r;     //!< 8-bit red component.
	uint8   g;     //!< 8-bit green component.
	uint8   b;     //!< 8-bit blue component.
	uint8   a;     //!< 8-bit alpha component. 
	void set(uint8 rV, uint8 gV, uint8 bV, uint8 aV) { r = rV;g = gV;b = bV;a = aV; }
	void set(UInt32 rgba) { *(UInt32*)this = rgba; }
	UInt32 getColor() { return  *(UInt32*)this;	}
	IGColor() :r(0), g(0), b(0), a(0) {}
	IGColor(uint8 red, uint8 green, uint8 blue, uint8 alpha = 0) : r(red), g(green), b(blue), a(alpha) {}
	IGColor(UInt32 rgba) : r((rgba >> 24) & 0xFF), g((rgba >> 16) & 0xFF), b((rgba >> 8) & 0xFF), a(rgba & 0xFF) {}
	static UInt32  colorToUintARGB(uint8 r, uint8 g, uint8 b, uint8 a = 0) { return (a << 24) | (r << 16) | (g << 8) | (b); }			     
	static IGColor uintrgbaToColor(UInt32 col) { return IGColor((col >> 24) & 0xFF, (col >> 16) & 0xFF, (col >> 8) & 0xFF, (col & 0xFF)); }	 
	static IGColor uintargbToColor(UInt32 col) { return IGColor((col & 0xFF), (col >> 8) & 0xFF, (col >> 16) & 0xFF, ((col >> 24) & 0xFF)); }

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


struct GPoint
{
	float x, y;  

	GPoint() : x(0.0f), y(0.0f) {}
	GPoint(float aX, float aY) : x(aX), y(aY) {}
};

struct  Pixel
{
	Pixel() {}
	Pixel(Int32 xx, Int32 yy) { x = xx; y = yy; }

	Int32   X() { return x; }                
	Int32   Y() { return y; }                
	void   X(Int32 xx) { x = xx; }           
	void   Y(Int32 yy) { y = yy; }           

	Int32   W() { return x; }                
	Int32   H() { return y; }                
	void   W(Int32 w) { x = w; }             
	void   H(Int32 h) { y = h; }             

	void Set() { x = 0; y = 0; }             
	void Set(Int32 xx, Int32 yy) { x = xx;  y = yy; }            

	void Shift(Int32 dx, Int32 dy) { x += dx; y += dy; }    
	void ShiftX(Int32 dx) { x += dx; }                      
	void ShiftY(Int32 dy) { y += dy; }                      

	bool Null() { return x == 0 && y == 0; }     

	friend bool operator ==(const Pixel &p1, const Pixel &p2) { return p1.x == p2.x && p1.y == p2.y; }
	friend bool operator !=(const Pixel &p1, const Pixel &p2) { return p1.x != p2.x || p1.y != p2.y; }
	friend bool operator < (const Pixel &p1, const Pixel &p2) { return p1.x<p2.x  && p1.y<p2.y; }
	friend bool operator > (const Pixel &p1, const Pixel &p2) { return p1.x>p2.x  && p1.y>p2.y; }
	friend bool operator <=(const Pixel &p1, const Pixel &p2) { return p1.x <= p2.x && p1.y <= p2.y; }
	friend bool operator >=(const Pixel &p1, const Pixel &p2) { return p1.x >= p2.x && p1.y >= p2.y; }

	Pixel operator +  (const Pixel &p) { return Pixel(x + p.x, y + p.y); }
	Pixel operator -  (const Pixel &p) { return Pixel(x - p.x, y - p.y); }
	Int32    operator *  (const Pixel &a) { return x*a.x + y*a.y; }
	void   operator += (const Pixel &p) { x += p.x;  y += p.y; }
	void   operator -= (const Pixel &p) { x -= p.x;  y -= p.y; }

	Pixel operator *  (const Int32   &a) { return Pixel(x*a, y*a); }
	Pixel operator /  (const Int32   &a) { return Pixel(x / a, y / a); }
	void   operator *= (const Int32   &a) { x *= a;    y *= a; }
	void   operator /= (const Int32   &a) { x /= a;    y /= a; }

	Int32 Norm(Pixel p1, Pixel p2)
	{
		Pixel dp = p2 - p1, dp0 = *this - p1; Int32 dp2 = dp*dp;
		if (dp2 == 0) return dp0*dp0; 
		else {
			Int32 t = (1000 * (dp0*dp)) / dp2;
			if (t<0)        return (*this - p1)*(*this - p1);              
			else if (t>1000)     return (*this - p2)*(*this - p2);         
			else                return sqrt(dp.x*dp0.y - dp.y*dp0.x) / dp2;
		}
	}

	friend Int32 DistManh(Pixel p1, Pixel p2) { return abs(p2.X() - p1.X()) + abs(p2.Y() - p1.Y()); } 
	friend Int32 DistSqr(Pixel p1, Pixel p2) { return Sqr(p2.X() - p1.X()) + Sqr(p2.Y() - p1.Y()); }  

	friend class Rect;
private:
	Int32 x, y;                                                 
};

class Rect
{
public:
	Rect() {                     }
	Rect(Pixel pp1, Pixel pp2) { Set(pp1, pp2); }
	Rect(Int32 x1, Int32 y1, Int32 x2, Int32 y2) { Set(x1, y1, x2, y2); }

	void Set() { x1 = x2 = y1 = y2 = 0; }                   
	void Set(const Rect  &r) { x1 = r.x1; y1 = r.y1; x2 = r.x2; y2 = r.y2; }
	void Set(const Pixel &p1, const Pixel &p2) { x1 = p1.x; y1 = p1.x; x2 = p2.x; y2 = p2.x; }
	void Set(Int32 xx1, Int32 yy1, Int32 xx2, Int32 yy2) { x1 = xx1;  y1 = yy1;  x2 = xx2;  y2 = yy2; }

	Int32   W() { return abs(x2 - x1); }                     
	Int32   H() { return abs(y2 - y1); }                     
	Pixel Size() { return Pixel(W(), H()); }                 

	Pixel P1() { return Pixel(x1, y1); }                    
	Pixel P2() { return Pixel(x2, y2); }                    

	Int32   X1() { return x1; }                             
	Int32   Y1() { return y1; }                             
	Int32   X2() { return x2; }                             
	Int32   Y2() { return y2; }                             

	void   X1(Int32 xx1) { x1 = xx1; }                      
	void   Y1(Int32 yy1) { y1 = yy1; }                      
	void   X2(Int32 xx2) { x2 = xx2; }                      
	void   Y2(Int32 yy2) { y2 = yy2; }                      

	Int32   LF() { return x1; }                             
	Int32   RT() { return x2; }                             
	Int32   UP() { return y1; }                             
	Int32   DN() { return y2; }                             

	void Shift(Int32  dx, Int32  dy) { x1 += dx; x2 += dx; y1 += dy; y2 += dy; } 
	void MoveTo(Pixel &p) { x2 = p.x + W(); y2 = p.y + H(); x1 = p.x; y1 = p.y; }
	void MoveTo(Int32 x, Int32 y) { MoveToX(x); MoveToY(y); } 
	void MoveToX(Int32 x) { x2 = x + W(); x1 = x; } 
	void MoveToY(Int32 y) { y2 = y + H(); y1 = y; } 
	void Scale(float sx, float sy) { x2 = Int32(x1 + W()*sx); y2 = Int32(y1 + H()*sy); } 
	void ScaleC(float sx, float sy) { Int32 x = (x2 + x1) / 2, y = (y2 + y1) / 2; Scale(sx, sy); Center(x, y); }  
	void Center(Int32 x, Int32 y) { Int32 x0 = x - W() / 2, y0 = y - H() / 2;MoveTo(x0, y0); }  
	void Center(Pixel &p) { Center(p.x, p.y); } 

	bool   In(const Pixel &p) { return x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2; }
	bool   Empty() { return x1 >= x2 || y1 >= y2; }                         
																		
	friend Rect operator |(const Rect &r1, const Rect &r2)  
	{
		return Rect(Min(r1.x1, r2.x1), Min(r1.y1, r2.y1),
			Max(r1.x2, r2.x2), Max(r1.y2, r2.y2));
	}
	friend Rect operator &(const Rect &r1, const Rect &r2)   
	{
		return Rect(Max(r1.x1, r2.x1), Max(r1.y1, r2.y1),
			Min(r1.x2, r2.x2), Min(r1.y2, r2.y2));
	}
private:
	Int32 x1, y1;                
	Int32 x2, y2;                
};


class IGVector3
{
public:
	IGVector3() {}
	IGVector3(float xV, float yV, float zV) :x(xV), y(yV), z(zV) {}
	float x;             //!< x-coordinate of the vector
	float y;             //!< y-coordinate of the vector
	float z;             //!< z-coordinate of the vector   
	bool operator == (IGVector3 const & v) const
	{
		return (
			v.x == x &&
			v.y == y &&
			v.z == z &&
			true);
	}
};

class IGVector2
{
public:
	IGVector2() {}
	IGVector2(float xV, float yV) :x(xV), y(yV) {}
	float x;             //!< x-coordinate of the vector
	float y;             //!< y-coordinate of the vector
	bool operator == (IGVector2 const & v) const
	{
		return (
			v.x == x &&
			v.y == y &&
			true);
	}
};


class IGMatrix        
{
public:
	float             m[3][3];
	IGVector3         t;
	void              setIdentity();
	void              SetRotX(float r = 0, bool resetTrans = true, bool setZeros = true);
	void              SetRotY(float r = 0, bool resetTrans = true, bool setZeros = true);
	void              SetRotZ(float r = 0, bool resetTrans = true, bool setZeros = true);
	static IGMatrix   matMul(IGMatrix const & A, IGMatrix const & B);	                          
	static IGVector3  matVecMul(IGMatrix const & A, IGVector3 const & V);	                      

	bool operator == (IGMatrix const & m0) const
	{
		return IsRotSame(m0) && IsTransSame(m0);
	}

	bool operator != (IGMatrix const & m0) const
	{
		return !IsRotSame(m0) || !IsTransSame(m0);
	}

	bool IsRotSame(IGMatrix const& m0) const
	{
		return  m[0][0] == m0.m[0][0] &&
			m[0][1] == m0.m[0][1] &&
			m[0][2] == m0.m[0][2] &&
			m[1][0] == m0.m[1][0] &&
			m[1][1] == m0.m[1][1] &&
			m[1][2] == m0.m[1][2] &&
			m[2][0] == m0.m[2][0] &&
			m[2][1] == m0.m[2][1] &&
			m[2][2] == m0.m[2][2];
	}

	bool IsTransSame(IGMatrix const& m0) const
	{
		return (t == m0.t);
	}
};
#endif // !TEPYS_H