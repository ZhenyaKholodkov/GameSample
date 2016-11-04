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
typedef unsigned char       UInt8;
typedef __int64             Int64;

typedef unsigned short      UInt16;
typedef unsigned int        UInt32;
typedef unsigned __int64    UInt64;

template <class T> inline T    Min(const T &x, const T &y) { return x<y ? x : y; }
template <class T> inline T    Max(const T &x, const T &y) { return x>y ? x : y; }
template <class T> inline T    Sqr(const T &x) { return x*x; }
template <class T> inline void Swap(T &x, T &y) { T z = x; x = y; y = z; }

class Rect;

//void closeApplication()
//{
//	exit(0);
//}

class IGColor     //!< цвет
{
public:
	UInt8   r;     //!< 8-bit red component.
	UInt8   g;     //!< 8-bit green component.
	UInt8   b;     //!< 8-bit blue component.
	UInt8   a;     //!< 8-bit alpha component. 
	void set(UInt8 rV, UInt8 gV, UInt8 bV, UInt8 aV) { r = rV;g = gV;b = bV;a = aV; }
	void set(UInt32 rgba) { *(UInt32*)this = rgba; }
	IGColor() :r(0), g(0), b(0), a(0) {}
	IGColor(UInt8 red, UInt8 green, UInt8 blue, UInt8 alpha = 0) : r(red), g(green), b(blue), a(alpha) {}
	IGColor(UInt32 rgba) : r((rgba >> 24) & 0xFF), g((rgba >> 16) & 0xFF), b((rgba >> 8) & 0xFF), a(rgba & 0xFF) {}
	static UInt32  colorToUintARGB(UInt8 r, UInt8 g, UInt8 b, UInt8 a = 0) { return (a << 24) | (r << 16) | (g << 8) | (b); }			 //! вернуть цвет по его компонентам
	static IGColor uintrgbaToColor(UInt32 col) { return IGColor((col >> 24) & 0xFF, (col >> 16) & 0xFF, (col >> 8) & 0xFF, (col & 0xFF)); }	 //! разбить цвет на компоненты
	static IGColor uintargbToColor(UInt32 col) { return IGColor((col & 0xFF), (col >> 8) & 0xFF, (col >> 16) & 0xFF, ((col >> 24) & 0xFF)); }//! разбить цвет на компоненты

};

class IGRect
{
public:
	int x;             //!< x-coordinate of the rectangle
	int y;             //!< y-coordinate of the rectangle
	int w;             //!< width
	int h;             //!< height
	void set(int xV, int yV, int wV, int hV) { x = xV;y = yV;w = wV;h = hV; }
};

class CFColour
{
public:
	inline void Set(float _r, float _g, float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	inline void TransformFromUint32(UInt32 c)
	{
		a = (float)((c >> 24) & 0xFF) / 255.0f;
		r = (float)((c >> 16) & 0xFF) / 255.0f;
		g = (float)((c >> 8) & 0xFF) / 255.0f;
		b = (float)((c) & 0xFF) / 255.0f;
	}

	float   r;
	float   g;
	float   b;
	float   a;
};

//-----------------------------------------------------------------------------------------
//! Список строк в который можно потокобезопасно записывать строки
//
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
//----------------------------------------------------------------------------------------
//! Координаты мыши и признак её нажатия
//
struct GMouse
{
	float x, y;         //!< координаты мыши
	bool  press;        //!< нажата она или нет

	GMouse() : x(0.0f), y(0.0f), press(false) {}
	GMouse(float aX, float aY, bool aPress = false) : x(aX), y(aY), press(aPress) {}
};


//========================================================================================
//! Точка на экране в целочисленных координатах Int32.
//
struct  Pixel
{
	Pixel() {}
	Pixel(Int32 xx, Int32 yy) { x = xx; y = yy; }

	Int32   X() { return x; }                   //!< получить x-координату точки
	Int32   Y() { return y; }                   //!< получить y-координату точки
	void   X(Int32 xx) { x = xx; }                   //!< задать x-координату точки
	void   Y(Int32 yy) { y = yy; }                   //!< задать y-координату точки

	Int32   W() { return x; }                   //!< получить x-координату точки (для размеров чего-то)
	Int32   H() { return y; }                   //!< получить y-координату точки (для размеров чего-то)
	void   W(Int32 w) { x = w; }                   //!< задать x-координату точки (для размеров чего-то)
	void   H(Int32 h) { y = h; }                   //!< задать y-координату точки (для размеров чего-то)

	void Set() { x = 0; y = 0; }               //!< установить в 0 координаты
	void Set(Int32 xx, Int32 yy) { x = xx;  y = yy; }               //!< задать новые значения xx,yy для координат

	void Shift(Int32 dx, Int32 dy) { x += dx; y += dy; }               //!< сдвинуть точку на dx, dy
	void ShiftX(Int32 dx) { x += dx; }                      //!< сдвинуть точку на dx
	void ShiftY(Int32 dy) { y += dy; }                      //!< сдвинуть точку на dy

	bool Null() { return x == 0 && y == 0; }        //!< точка имеет нулевые координаты

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

	//--------------------------------------------------------------------------
	//! Квадрат расстояния от точки до отрезка (p1,p2)
	//
	Int32 Norm(Pixel p1, Pixel p2)
	{
		Pixel dp = p2 - p1, dp0 = *this - p1; Int32 dp2 = dp*dp;
		if (dp2 == 0) return dp0*dp0; // длина отрезка минимальна
		else {
			Int32 t = (1000 * (dp0*dp)) / dp2;
			if (t<0)        return (*this - p1)*(*this - p1);           // p - вне, ближе к левому концу
			else if (t>1000)     return (*this - p2)*(*this - p2);           // p - вне, ближе к правому концу
			else                return sqrt(dp.x*dp0.y - dp.y*dp0.x) / dp2;  // p - лежит "внутри отрезка"
		}
	}

	friend Int32 DistManh(Pixel p1, Pixel p2) { return abs(p2.X() - p1.X()) + abs(p2.Y() - p1.Y()); }  // Манхетенское расстояние
	friend Int32 DistSqr(Pixel p1, Pixel p2) { return Sqr(p2.X() - p1.X()) + Sqr(p2.Y() - p1.Y()); }  // Квадрат расстояния

	friend class Rect;
private:
	Int32 x, y;                                                 //!< координаты точки
};
//========================================================================================
//! Прямоугольник на экране.
//! Прямоугольник ограничен двумя точками: левый-верхний угол p1 и правый-нижний p2.
//! Визуально, полагается, что p2 расположен на пиксель правее и ниже правого-нижнего угла.
//! Поэтому ширина прямоугольника равна p2.x-p1.x а не p2.x-p1.x+1, аналогично для высоты.
//! Соответственно пустой прямоугольник, если p1.x>=p2.x || p1.y>=p2.y;
//
class Rect
{
public:
	Rect() {                     }
	Rect(Pixel pp1, Pixel pp2) { Set(pp1, pp2); }
	Rect(Int32 x1, Int32 y1, Int32 x2, Int32 y2) { Set(x1, y1, x2, y2); }

	void Set() { x1 = x2 = y1 = y2 = 0; }                    //!< создать пустой прямоугольник
	void Set(const Rect  &r) { x1 = r.x1; y1 = r.y1; x2 = r.x2; y2 = r.y2; }//!< создать копию прямоугольника r
	void Set(const Pixel &p1, const Pixel &p2) { x1 = p1.x; y1 = p1.x; x2 = p2.x; y2 = p2.x; }//!< задать точки левого верхнего и правого нижнего угла
	void Set(Int32 xx1, Int32 yy1, Int32 xx2, Int32 yy2) { x1 = xx1;  y1 = yy1;  x2 = xx2;  y2 = yy2; } //!< задать точки левого верхнего и правого нижнего угла

	Int32   W() { return abs(x2 - x1); }                         //!< Ширина прямоугольника
	Int32   H() { return abs(y2 - y1); }                         //!< Высота прямоугольника
	Pixel Size() { return Pixel(W(), H()); }                 //!< Ширина и высота

	Pixel P1() { return Pixel(x1, y1); }                    //!< левый верхний угол прямоугольника
	Pixel P2() { return Pixel(x2, y2); }                    //!< нижний правый угол прямоугольника

	Int32   X1() { return x1; }                                 //!< поучить левую x координату прямоугольника
	Int32   Y1() { return y1; }                                 //!< поучить верхнюю y координату прямоугольника
	Int32   X2() { return x2; }                                 //!< поучить правую x координату прямоугольника
	Int32   Y2() { return y2; }                                 //!< поучить нижнюю y координату прямоугольника

	void   X1(Int32 xx1) { x1 = xx1; }                             //!< задать левую x координату прямоугольника
	void   Y1(Int32 yy1) { y1 = yy1; }                             //!< задать верхнюю y координату прямоугольника
	void   X2(Int32 xx2) { x2 = xx2; }                             //!< задать правую x координату прямоугольника
	void   Y2(Int32 yy2) { y2 = yy2; }                             //!< задать нижнюю y координату прямоугольника

	Int32   LF() { return x1; }                                 //!< левая x координата прямоугольника
	Int32   RT() { return x2; }                                 //!< правая x координата прямоугольника
	Int32   UP() { return y1; }                                 //!< верхняя y координата прямоугольника
	Int32   DN() { return y2; }                                 //!< нижняя y координата прямоугольника

	void Shift(Int32  dx, Int32  dy) { x1 += dx; x2 += dx; y1 += dy; y2 += dy; } //!< сдвинуть прямоугольник на dx, dy
	void MoveTo(Pixel &p) { x2 = p.x + W(); y2 = p.y + H(); x1 = p.x; y1 = p.y; } //!< передвинуть прямоугольник к точке p
	void MoveTo(Int32 x, Int32 y) { MoveToX(x); MoveToY(y); } //!< передвинуть прямоугольник к точке p
	void MoveToX(Int32 x) { x2 = x + W(); x1 = x; } //!< сдвинуть прямоугольник вдоль оси X к точке x
	void MoveToY(Int32 y) { y2 = y + H(); y1 = y; } //!< сдвинуть прямоугольник вдоль оси X к точке x
	void Scale(float sx, float sy) { x2 = Int32(x1 + W()*sx); y2 = Int32(y1 + H()*sy); } //!< изменить размеры не торгая левый верхний угол
	void ScaleC(float sx, float sy) { Int32 x = (x2 + x1) / 2, y = (y2 + y1) / 2; Scale(sx, sy); Center(x, y); }  //!< изменить размеры не торгая центр прямоугольника
	void Center(Int32 x, Int32 y) { Int32 x0 = x - W() / 2, y0 = y - H() / 2;MoveTo(x0, y0); }  //!< отцентрировать прямоугольник относительно точки p
	void Center(Pixel &p) { Center(p.x, p.y); }                  //!< отцентрировать прямоугольник относительно точки p
	void Sort() { if (x1>x2) Swap(x1, x2); if (y1>y2) Swap(y1, y2); }// левый аерхний угол становится левее и выше нижнего правого :)

	bool   In(const Pixel &p) { return x1 <= p.x && p.x <= x2 && y1 <= p.y && p.y <= y2; } //!< точка p находится внутри прямоугольника
	bool   Empty() { return x1 >= x2 || y1 >= y2; }                           //!< прямоугольник пустой
																			  //--------------------------------------------------------------------------
	friend Rect operator |(const Rect &r1, const Rect &r2)   //!< Объединение прямоугольников
	{
		return Rect(Min(r1.x1, r2.x1), Min(r1.y1, r2.y1),
			Max(r1.x2, r2.x2), Max(r1.y2, r2.y2));
	}
	//--------------------------------------------------------------------------
	friend Rect operator &(const Rect &r1, const Rect &r2)   //!< Пересечение прямоугольников
	{
		return Rect(Max(r1.x1, r2.x1), Max(r1.y1, r2.y1),
			Min(r1.x2, r2.x2), Min(r1.y2, r2.y2));
	}
	//--------------------------------------------------------------------------
	void Add(Rect &r)                                          //!< Добавить прямоугольник увеличив исходный до их объединения
	{
		if (Empty()) Set(r);
		else {
			if (x1>r.x1) x1 = r.x1;  if (y1>r.y1) y1 = r.y1;
			if (x2<r.x2) x2 = r.x2;  if (y2<r.y2) y2 = r.y2;
		}
	}

	// Отпозиционировать прямоугольник r1 относительно заданного прямоугольника r2 в соответствии с маской.
	friend inline Rect & Align(Rect &r1, const Rect &r2, uint32 mask = 0xffffffff, Int32 dx = 0, Int32 dy = 0);
	// Разбить прямоугольник r2 по горизонтали на nim_x прямоугольников и применить Align к под-прямоугольнику с номерм k_x=0..num_x-1
	friend inline Rect & AlignX(Int32 num_x, Int32 k_x, Rect &r1, const Rect &r2, uint32 mask = 0xffffffff, Int32 dx = 0, Int32 dy = 0);
	// Разбить прямоугольник r2 по вертикале на nim_y частей и применить Align к под-прямоугольнику с номерм k_y=0..num_y-1
	friend inline Rect & AlignY(Int32 num_y, Int32 k_y, Rect &r1, const Rect &r2, uint32 mask = 0xffffffff, Int32 dx = 0, Int32 dy = 0);
	// Разбить прямоугольник r2 по горизонтали на nim_x частей а по вертикале на nim_y частей  и применить Align к под-прямоугольнику с номерм (k_x,k_y)=0..num_x-1
	friend inline Rect & AlignXY(Int32 num_x, Int32 num_y, Int32 k_x, Int32 k_y, Rect &r1, const Rect &r2, uint32 mask = 0xffffffff, Int32 dx = 0, Int32 dy = 0);

	// Вычисление прямоугольников пересечения des_r, src_r при сдвиге начал их систем координать на dx,dy
	friend Int32 Intersect(Rect &des_r, Rect &src_r, Int32 dx, Int32 dy);

	//--------------------------------------------------------------------------
private:
	Int32 x1, y1;                //!< левый верхний угол
	Int32 x2, y2;                //!< нижний правый угол
};


struct OColor
{
	OColor() {                         }
	OColor(const uint32 c) { iColor = c; }
	OColor(const UInt8 r, const UInt8 g, const UInt8 b) { iR = r; iG = g; iB = b; }
	OColor(const UInt8 r, const UInt8 g, const UInt8 b, const UInt8 a) { iR = r; iG = g; iB = b; iA = a; }
	OColor(const OColor &c1, const OColor c2, float t) { Interpol(c1, c2, t); }


	UInt8   R() const { return iR; }                        //<! красная компонена цвета
	UInt8   G() const { return iG; }                        //<! зеленая компонена цвета
	UInt8   B() const { return iB; }                        //<! синяя компонена цвета
	UInt8   A() const { return iA; }                        //<! канал прозрачности

	uint32  & Color() { return iColor; }                        //<! цвет упакованный в 32 битное беззнаковое целое

	void    operator +=(const OColor &c) { iR += c.R(); iG += c.G(); iB += c.B(); }
	void    operator -=(const OColor &c) { if (iR>c.R()) iR -= c.R(); if (iG>c.G()) iG -= c.G(); if (iB>c.B()) iB -= c.B(); }

	bool    operator == (uint32 color) { return color == iColor; }
	bool    operator != (uint32 color) { return color != iColor; }

	void    Set(UInt8 r, UInt8 g, UInt8 b) { iR = r; iG = g; iB = b; }
	void    Set(UInt8 r, UInt8 g, UInt8 b, UInt8 a) { iR = r; iG = g; iB = b; iA = a; }

	//!< интерполировать цвет между c1 и c2 параметром t=[0..1]
	void    Interpol(OColor c1, OColor c2, float t)
	{
		Set(UInt8(c1.R() + (int32(c2.R()) - int32(c1.R()))*t),
			UInt8(c1.G() + (int32(c2.G()) - int32(c1.G()))*t),
			UInt8(c1.B() + (int32(c2.B()) - int32(c1.B()))*t));
	}
protected:
	union {
		uint32  iColor;                        //<! цвет пикселя
		struct { UInt8 iR, iG, iB, iA; };       //<! его компоненты
	};
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


class IGMatrix          //!< текущая матрица трансформации
{
public:
	float             m[3][3];
	IGVector3         t;
	void              setIdentity();
	void              SetRotX(float r = 0, bool resetTrans = true, bool setZeros = true);
	void              SetRotY(float r = 0, bool resetTrans = true, bool setZeros = true);
	void              SetRotZ(float r = 0, bool resetTrans = true, bool setZeros = true);
	static IGMatrix   matMul(IGMatrix const & A, IGMatrix const & B);	                           //!< умножение матриц
	static IGVector3  matVecMul(IGMatrix const & A, IGVector3 const & V);	                       //!< умножение матрицы на вектор

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