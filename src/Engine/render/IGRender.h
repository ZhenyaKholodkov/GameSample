//! Класс IGRender - платформонезависимый менеджер рисования
/*! \file IGRender.h

 *                                                   (с) absolutist.com (abeyn) 2013-07-04
 *****************************************************************************************/

#ifndef _IGRENDER_ABS_H
#define _IGRENDER_ABS_H

#include <vector>
#include <stack>
#include <stdio.h>
#include "Types.h"

class IGResModel;

using namespace std;

enum AlphaMode {																				// режимы блендинга в редакторе сцен
	ALPHA_PARENT = 0,
	ALPHA_BLEND = 1,
	ALPHA_HALF = 2,
	ALPHA_ADD = 3,
	ALPHA_SUB = 4,
	ALPHA_NONE = 5
};

enum CullMode {																				   // режимы отсучения примитивов
	CULL_FRONT = 0,                                                       // Polygons with a clockwise winding order are culled.
	CULL_BACK = 1,                                                       // Polygons with an anti-clockwise winding order are culled.
	CULL_NONE = 2,                                                       // Polygons are not culled (polygons are "double-sided").
};


#define DEGREE_TO_RAD 0.0174532925199f                         //!< перевод градусов в радианы (умножаем на градусы)

class IGRender																	//<! менеджер рисования
{
public:

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

   enum ProjectionType{
      PR_PERSPECTIVE,
      PR_ORTHOGRAPHIC
   };

   enum StageType{
      STAGE_2D,                                          //!< двухмерная отрисовка
      STAGE_3D                                           //!< трехмерная отрисовка
   };

   enum SpaceType{
      SPACE_WORLD,                                       //!< мировая система координат
      SPACE_MODEL                                        //!< система координат модели
   };

   class Stage
   {
   public:
      void        firstStage();                       //!< установить первую сцену                     
      void        switchStage(StageType newStage);    //!< переключение сцены

      StageType   tp;                                 //!< тип сцены  
      int         zPos;                               //!< позиция сцены
      static int  firstStageZPos;                     //!< позиция первой сцены
      static int  stage3DZDepth;                      //!< глубина сцены
   };
   
	IGRender();
	virtual ~IGRender(){}

	static IGRender* Create();													//!< создание экземпляра менеджер рисования
	static IGRender* Instance() {return _instance;}						//!< получение экземпляра менеджер рисования

	// abstract functions
	virtual	void	init( int w, int h ) = 0;									   //!< инициализировать контекст рисования
	virtual	void	setClearColor( unsigned int c ) = 0;						//!< установить цвет очистки
	virtual	void	clear() = 0;												      //!< очистить окно
	virtual	void	startFrame();											         //!< начать рисование нового кадра
	virtual	void	endFrame();												         //!< завершить рисование текущего кадра
	virtual	void	draw() = 0;													      //!< нарисовать все
	virtual	void	drawTri(float x1, float y1, 
					float x2, float y2, 
					float x3, float y3) = 0;									      //!< нарисовать залитый триугольник

	virtual	void	drawFrame(float x, float y, float w, float h) = 0;			                  //!< нарисовать прямоугольник (левый верхний угол - x,y; ширина,высота - w,h
   virtual	void	drawLine(float x1, float y1, float z1, float x2, float y2, float z2) = 0;	//!< нарисовать линию (между вершинами - x1,y1,z1 и x2,y2,z2)
	virtual	void	destroyTexture( unsigned int textureID , bool unload = true) = 0;//!< удаление указанной текстуры из массива текстур и если unload - выгружать из видеопамяти 
	virtual	void	setClipRect( int aX, int aY, int aWidth, int aHeight) = 0;	//!< установить прямоулольник отсечения
	virtual	void	clearClipRect() = 0;										            //!< убрать режим отсечения
	virtual	void	applyTransform() = 0;											         //!< установить матрицу трансформации
  virtual void   restoreContext(){}                                          //!< восстановление контекста

  virtual uint32 LoadTexture(const unsigned char* bits, uint32 textureWidth, uint32 textureHegih) = 0;
  virtual void   UnloadTexture(uint32 textureId) = 0;
			// platform independed functions
         int		width()  {return _width;}									      //!< получить ширину вьюпорта
			int		height() {return _height;}									      //!< получить высоту вьюпорта

         void  skipFrame(bool val);                                        //!< установить флаг пропуска текущего кадра
         bool  skipFrame()         { return _skipFrame; }                  //!< получить флаг пропуска текущего кадра
         int   frameNumber() { return _frameNumber; }                      //!< номер текущего кадра
         void	loadIdentityMatrix();									            //!< установка начальных настроек рисования
         void	save();													               //!< сохранение настроек рендера (текущие значения заталкиваются в стек)
         void	restore();												               //!< востановление настроек рендера (текущими значениями становятся сохраненные в стеке )
         void	translate( float x, float y, float z = 0.0f );			      //!< перемещение камеры
         void	rotate( float ang );									               //!< поворот камеры
         void	rotate( float angX, float angY, float angZ );			      //!< поворот камеры 3D
         void	scale( float x, float y, float z=1.0f );  		            //!< установить маштабирование
         void	setBlending(AlphaMode blending);						            //!< установить текущие настройки блендинга
         void	saveBlending();											            //!< cохранить текущие настройски блендинга
         void	restoreBlending();										            //!< восстановить сохраненные настройски блендинга
         void	setCullMode(CullMode cullMode);						            //!< установить текущие настройки отсечения примитивов
         void	saveCullMode();											            //!< cохранить текущие настройски отсечения примитивов
         void	restoreCullMode();										            //!< восстановить сохраненные настройски отсечения примитивов
         void	setDepthTest(bool val);	   				                     //!< включить/отключить Z-буфер
         void  switchStage(StageType newStage);                            //!< переключение сцены
         void  switchSpace(SpaceType val);                                 //!< переключить координатную систему
         void  updateRenderState();                                        //!< обновить состояние рендера
         void	drawImage(unsigned int textureId,
                           int full_tex_w, int full_tex_h,
                           int tex_x, int tex_y, int tex_w, int tex_h,
                           float x, float y, float z = 0.0f, 
                           UInt32 color0 = 0xffffffff,						   // 0 ---- 1
                           UInt32 color1 = 0xffffffff,						   //	|      |
                           UInt32 color2 = 0xffffffff,						   //	|      |
                           UInt32 color3 = 0xffffffff						      //	3 ---- 2
                        );												               //!< рисуем фрагмент текстуры в указанном месте
         void	drawImage(	unsigned int textureId, 
            int full_tex_w, int full_tex_h,
            int tex_x, int tex_y, int tex_w, int tex_h,
            const std::vector<IGVector3>& vertexes,
            UInt32 color0 = 0xffffffff,						   // 0 ---- 1
            UInt32 color1 = 0xffffffff,						   //	|      |
            UInt32 color2 = 0xffffffff,						   //	|      |
            UInt32 color3 = 0xffffffff						      //	3 ---- 2
            );												               //!< рисуем фрагмент текстуры в указанном месте
         void  drawRect(float x, float y, float w, float h);							      //!< нарисовать прямоугольник (левый верхний угол - x,y; ширина,высота - w,h
         void  drawRect(float x, float y, float w, float h, 
                        UInt32 color0, UInt32 color1, UInt32 color2, UInt32 color3);	                //!< нарисовать прямоугольник (левый верхний угол - x,y; ширина,высота - w,h с различными цветами вершин по часовой стрелке
         void	setFont( int fontID=0);										         //!< установить шрифт
			void	setFontSize( int size );									         //!< установить размер шрифта
			void	setTextColor( float r, float g, float b, float a = 1.0f );	//!< установить текущий цвет вывода текста по компонентам
			void	setTextColor( UInt32 c );									         //!< установить текущий цвет вывода текста 
			UInt32	fontColor() const {return _fontColor;}						   //!< получить цвет шрифта
         void	drawText( const char* aString , float x, float y);				//!< вывести текст
         void drawText( const std::string& utf16Str, float x, float y);
         void  drawLetter(UInt16 ch, float x = 0.0f, float y = 0.0f);      //!< вывести букву

		 virtual int       createTexture(TexturePixType format,
			 unsigned char* data,
			 int texWidth, int texHeight,
			 int fid,
			 GThreadSafeErrors &err,
			 bool upload = true) = 0;				               //!< создание текстуры из буфера

			float	getFontR() const {return _fontR;}							      //!< получить красную компоненту шрифта
			float	getFontG() const {return _fontG;}							      //!< получить зеленую компоненту шрифта
			float	getFontB() const {return _fontB;}							      //!< получить синююю компоненту шрифта
         float		getFontA() const {return _fontA;}							   //!< получить альфа компоненту шрифта
			IGMatrix	getTransform()					{return _matrix;}					//!< получить матрицу трансформации модели	 
			void  	setTransform(IGMatrix val)	{_matrix = val;}					//!< установить матрицу трансформации модели	 			f
			float	globalR() const { return _globalR; }						      //!< получить текущий коэффицент красного канала
			float	globalG() const { return _globalG; }						      //!< получить текущий коэффицент зеленого канала
			float	globalB() const { return _globalB; }						      //!< получить текущий коэффицент синего канала
			float	globalAlpha() const { return _globalAlpha; }				      //!< текущий коэффицент прозрачности
			void	setGlobalR( float val ){_globalR = val;}					      //!< установить текущий коэффицент красного канала
			void	setGlobalG( float val ){_globalG = val;}					      //!< установить текущий коэффицент зеленого канала
			void	setGlobalB( float val ){_globalB = val;}					      //!< установить текущий коэффицент синего канала
			void	setGlobalAlpha( float alpha ) {_globalAlpha = alpha;}		   //!< установить текущий коэффицент прозрачности (1 - непрозрачный)
			void	setLineColor( float r, float g, float b );					   //!< установить текущий цвет вывода линий по компонентам					
			void	setLineColor( UInt32 c );									         //!< установить текущий цвет вывода линий
			void	setFillColor( float r, float g, float b );					   //!< установить текущий цвет заполнения по компонентам
			void	setFillColor( UInt32 c );									         //!<  установить текущий цвет заполнения
			void	setRotate3DVertState(	float ang, float globalScale, 
													float farSideScale ){}		         //!< установить вертикальный поворот 3D
			void	clearRotate3DVertState(){}									         //!< убрать вертикальный поворот 3D

protected:   
   ProjectionType          _projectionType;                       //!< тип проекции
   Stage                   _curStage;                             //!< текущая рисуемая сцена

   IGMatrix 	            _matrix;											//!< матрица трансформации текущей модели
   IGMatrix 	            _batchMatrix;									//!< матрица трансформации текущей пачки
   float				         _globalAlpha;									//!< текущая альфа
   AlphaMode			      _drawBlending;									//!< блендинг, который сейчас используется для отрисовки
   bool     			      _drawDepthTest;								//!< Z-буфер, который сейчас используется для отрисовки
   AlphaMode			      _curBlending;									//!< текущий блендинг которым необходимо рисовать новые элементы
   bool     			      _curDepthTest;									//!< текущий Z-буфер
   CullMode			         _curCullMode;									//!< текущий режим отсечения которым необходимо рисовать новые элементы
   SpaceType               _curSpace;                             //!< текущая система координат

   std::stack<IGMatrix>	      _matrixStack;									//!< стек сохраненных матриц трансформации
   std::stack<float>		      _alphaStack;									//!< стек сохраненных коэффицентов прозрачности
   std::vector<AlphaMode>	      _alphaBlendStack;								//!< стек сохраненных значений блендинга
   std::vector<CullMode>	      _cullModeStack;								//!< стек сохраненных значений отсечений
   std::vector<bool>            _depthTestStack;                       //!< стек сохраненных значений Z буфера

   IGRect                  _currentClipRect;                      //!< текущее отсечение

   bool                    _isDraw;                               //!< идет рисование

   // параметры пакетной отрисовки
   const	UInt32		      _BATCHED_VERTEXES_SIZE;						//!< максимальное количество вершин, рисуемых за одну отрисовку
   UInt32				      _cacheVertIndex;								//!< указатель на используемый блок кеша вершин
   UInt32				      _batchVertIndex;								//!< порядковый номер вершины в партии отрисовки  
   int                     _vertexes;                             //!< число вершин в кеше
   std::vector<IGVector3>			_vertexData;									//!< кеш координат вершин 
   std::vector<IGVector2>		   _uvData;											//!< кеш текстурных координат вершин 
   std::vector<IGColor>			_colourData;								   //!< кеш цветов вершин
   std::vector<UInt16>          _indexesData;                          //!< кеш индексов
   
   int			            _currentTextureID;							//!< текущая основная текстура, которую рисуем

   bool              _skipFrame;                                  //!< проаустить кадр
   UInt32            _frameNumber;                                //!< номер текущего кадра
	int					_width;													//!< ширина вьюпорта
	int					_height;												   //!< высота вьюпорта
	UInt32				_clearColor;											//!< цвет очистки экрана
	float				   _globalR;												//!< текущий красный канал рисования
	float				   _globalG;												//!< текущий зеленый канал рисования
	float				   _globalB;												//!< текущий синий канал рисования
	float				   _fontR, _fontG, _fontB, _fontA;					//!< компоненты цвета шрифта
	float				   _fillR, _fillG, _fillB;								//!< компоненты цвета заливки областей
	float				   _lineR, _lineG, _lineB;								//!< компоненты цвета границ областей	
	UInt32				_fontColor;												//!< цвет шрифта

   bool                 visible();                                         //!< видим ли будет объект который собираемся рисовать
   void				      addBatchTris(int triCnt);									//!< добавить треугольники в пакет
   void				      drawRect(float x, float y, float w, float h, 
                                 IGColor color0, 
                                 IGColor color1, 
                                 IGColor color2, 
                                 IGColor color3);
   virtual void			drawBatchedTris() = 0;										//!< рисуем пакет прямоугольников
   virtual void			applyBlending() = 0;	   			                  //!< применить текущие настройки блендинга
   virtual void			applyCullMode() = 0;	   			                  //!< применить текущие настройки отсечения примитивов
   virtual void			applyDepthTest() = 0;	   			               //!< применить текущие настройки Z-буфера
   virtual void			clearDepthBuffer() = 0;	   			               //!< очистить Z-буфер

protected:              
  void				         convertVertexDataToModelSpace(IGVector3 &vertex)	const;			      //!< конвертируем координаты вершин в координатную систему модели
   bool				         testOutsideScreen(IGVector3 &vertex0, IGVector3 &vertex2) const;	      //!< проверяем, не попали ли координаты вершин за пределы экрана

private:
	static IGRender*	      _instance;												                           //!< экземпляр менеджера рисования
   void				         allocVertexCache();													               //!< резервируем кэш вершин
   void				         reallocVertexCache();													            //!< увеличиваем размер кэша
};

//---------------------------------------------------------------------------------------------
// конвертируем координаты вершин в координатную систему модели
//
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

   newVertexData.z+=_curStage.zPos;
   vertex = newVertexData;
}

//-----------------------------------------------------------------------------------------------
// проверяем, не попали ли координаты вершин за пределы экрана
//
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
