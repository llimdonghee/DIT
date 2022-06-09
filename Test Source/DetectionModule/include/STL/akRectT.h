#pragma once



template<typename T>
class CakRectT
{
public:
	T left;
	T top;
	T right;
	T bottom;

public:	
	CakRectT(void);
	//CakRectT(T l, T t, T r, T b);
	CakRectT(const T l, const T t, const T r, const T b):left(l),top(t),right(r),bottom(b) {};
	virtual ~CakRectT(void);
	
 
 	//Attirbute
 	// retrieves the width
 	T Width() const throw();
 	// returns the height
 	T Height() const throw();
 	
	//{
	//	// reference to the top-left point
	//	CPoint& TopLeft() throw();
	//	// reference to the bottom-right point
	//	CPoint& BottomRight() throw();
	//	// const reference to the top-left point
	//	const CPoint& TopLeft() const throw();
	//	// const reference to the bottom-right point
	//	const CPoint& BottomRight() const throw();
	//	// the geometric center point of the rectangle
	//	CPoint CenterPoint() const throw();
	//
	//	bool PtInRect(POINT point) const throw();
	//}

 	// swap the left and right
	void SwapLeftRight() throw();
 	void SwapTopBottom() throw();
 
 
 
 	// returns TRUE if rectangle has no area
 	bool IsRectEmpty() const throw();
 	// returns TRUE if rectangle is at (0,0) and has no area
 	bool IsRectNull() const throw();
 	// returns TRUE if point is within rectangle
 	
	bool PtInRect(T x, T y) const throw();
 
 	// Operations
 
 	// set rectangle from left, top, right, and bottom
 	void SetRect(T x1, T y1, T x2, T y2) throw();
 	//void SetRect(POINT topLeft, POINT bottomRight) throw();
 	// empty the rectangle
 	void SetRectEmpty() throw();
 	// copy from another rectangle
 	void CopyRect(const CakRectT<T>* lpSrcRect) throw();
 	// TRUE if exactly the same as another rectangle
 	bool EqualRect(const CakRectT<T>* lpRect) const throw();

	// Inflate rectangle's width and height by
	// x units to the left and right ends of the rectangle
	// and y units to the top and bottom.
	void InflateRect(T x, T y) throw();
	void InflateRect(T l, T t, T r, T b) throw();
	void InflateRect(CakRectT<T>* lpRect) throw();

	// deflate the rectangle's width and height without
	// moving its top or left
	void DeflateRect(T x, T y) throw();
	void DeflateRect(T l, T t, T r, T b) throw();
	//void DeflateRect(SIZE size) throw();
	void DeflateRect(CakRectT<T>* lpRect) throw();

	// translate the rectangle by moving its top and left
	void OffsetRect(T x, T y) throw();
	//void OffsetRect(SIZE size) throw();
	//void OffsetRect(POINT point) throw();
	void NormalizeRect() throw();
 
	// absolute position of rectangle
	void MoveToY(T y) throw();
	void MoveToX(T x) throw();
	void MoveToXY(T x, T y) throw();
	//void MoveToXY(POINT point) throw();

 	// 두사각형이 겹치는 부분만 검출 없을시 0으로 셋팅(Return은 겹치는 부분이 있으면 True)
 	bool IntersectRect(CakRectT<T>* lpRect1, CakRectT<T>* lpRect2) throw();
 
 	// 두 사각형의 테두리 부분 검출(Return은 겹치는 부분이 있으면 True)
 	bool UnionRect(CakRectT<T>* lpRect1, CakRectT<T>* lpRect2) throw();
	
 	// set this rectangle to minimum of two others
 	//bool SubtractRect(CakRectT<T>* lpRectSrc1, CakRectT<T>* lpRectSrc2) throw();

	// Additional Operations
	void operator=(const CakRectT<T>& srcRect) throw();
	bool operator==(const CakRectT<T>& rect) const throw();
	bool operator!=(const CakRectT<T>& rect) const throw();
	void operator+=(CakRectT<T>* lpRect) throw();
	void operator-=(CakRectT<T>* lpRect) throw();
	//void operator&=(const CakRectT<T>& rect) throw();
	//void operator|=(const CakRectT<T>& rect) throw();

  	// Operators returning CakRectT<T> values
  	//CakRectT<T> operator+(POINT point) const throw();
  	//CakRectT<T> operator-(POINT point) const throw();
  	CakRectT<T> operator+(CakRectT<T>* lpRect) const throw();
  	//CakRectT<T> operator+(SIZE size) const throw();
  	//CakRectT<T> operator-(SIZE size) const throw();
  	CakRectT<T> operator-(CakRectT<T>* lpRect) const throw();
  	//CakRectT<T> operator&(const CakRectT<T>& rect2) const throw();
  	//CakRectT<T> operator|(const CakRectT<T>& rect2) const throw();

 	CakRectT<T> MulDiv(T nMultiplier, T nDivisor) const throw();

	//예전에 쓰던 함수들
public:
	inline void set(T l, T t, T r, T b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;	
	};
	inline T getWidth(){return right-left;};
	inline T getHeight(){return bottom-top;};
	inline T getCenter(){return T(left+(right-left)/2.0);};
	inline T getVCenter(){return T(top+(bottom-top)/2.0);};
	void setAlign(){NormalizeRect();}; //작은값이 왼쪽, 위, 큰값이 오른쪽 아래로 가게 한다.
	bool getCheckAreaIn(double x, double y); //사각형 영역에 x,y포인트가 있는지 검사
	bool getCheckWidthIn(double p1); //x포인트 가운데 점이 있는지 검사
	bool getCheckHeightIn(double p1); //y포인트 가운데 점이 있는지 검사



};


template<typename T>
bool CakRectT<T>::IntersectRect( CakRectT<T>* lpRect1, CakRectT<T>* lpRect2 ) throw()
{
	
	if ( lpRect1->left	<	lpRect2->right	&&
		lpRect1->top	<	lpRect2->bottom	&&
		lpRect1->right	>	lpRect2->left	&&
		lpRect1->bottom	>	lpRect2->top		)
	{
		//*this = *lpRect2;

		lpRect1->left	> lpRect2->left   ? left	 = lpRect1->left	: left  = lpRect2->left		;	
		lpRect1->top	> lpRect2->top	  ? top		 = lpRect1->top		: top	= lpRect2->top		;
		lpRect1->right	< lpRect2->right  ? right	 = lpRect1->right	: right	= lpRect2->right	;
		lpRect1->bottom < lpRect2->bottom ? bottom	 = lpRect1->bottom	: bottom= lpRect2->bottom	;

		return true;
	}

	set(0,0,0,0);

	return false;
}

template<typename T>
bool CakRectT<T>::UnionRect( CakRectT<T>* lpRect1, CakRectT<T>* lpRect2 ) throw()
{

	lpRect1->left	< lpRect2->left   ? left	 = lpRect1->left	: left  = lpRect2->left	;	
	lpRect1->top	< lpRect2->top	  ? top		 = lpRect1->top		: top	= lpRect2->top		;
	lpRect1->right	> lpRect2->right  ? right	 = lpRect1->right	: right	= lpRect2->right	;
	lpRect1->bottom > lpRect2->bottom ? bottom	 = lpRect1->bottom	: bottom= lpRect2->bottom	;
	
	if ( lpRect1->left	<	lpRect2->right	&&
		lpRect1->top	<	lpRect2->bottom	&&
		lpRect1->right	>	lpRect2->left	&&
		lpRect1->bottom	>	lpRect2->top		)
	{
		return true;
	}

	return false;
}






#include "inl/akRectT.inl"