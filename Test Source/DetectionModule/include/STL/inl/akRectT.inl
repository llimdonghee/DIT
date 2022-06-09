
#define AKINLINE inline

template<typename T> AKINLINE
CakRectT<T>::CakRectT( void )
{

}

// template<typename T> AKINLINE
// CakRectT<T>::CakRectT( T l, T t, T r, T b )
// {
// 	left = l;
// 	top = t;
// 	right = r;
// 	bottom = b;	
// }

// template<typename T> AKINLINE
// CakRectT<T>::CakRectT( CakRectT<T> rect )
// {
// 	left = rect.left;
// 	top = rect.top;
// 	right = rect.right;
// 	bottom = rect.bottom;	
// }

template<typename T> AKINLINE
CakRectT<T>::~CakRectT( void )
{

}

template<typename T> AKINLINE
T CakRectT<T>::Width() const throw()
{
	return right - left; 
}

template<typename T> AKINLINE
T CakRectT<T>::Height() const throw()
{
	return bottom - top;
}





template<typename T> AKINLINE
bool CakRectT<T>::PtInRect( T x, T y ) const throw()
{
	if( x <= left || x >= right ||
		y <= top  || y >= bottom )
	{
		return false;
	}

	return true;
}

template<typename T> AKINLINE
bool CakRectT<T>::IsRectEmpty() const throw()
{
	if(left-right == 0) return true;
	if(top-bottom == 0) return true;

	return false;
}

template<typename T> AKINLINE
bool CakRectT<T>::IsRectNull() const throw()
{
	return (left == 0 && right == 0 && top == 0 && bottom == 0);
}


template<typename T> AKINLINE
void CakRectT<T>::SwapLeftRight() throw()
{
	T temp = left; 
	left = right; 
	right = temp; 
}

template<typename T> AKINLINE
void CakRectT<T>::SwapTopBottom() throw()
{
	T temp = top; 
	top = bottom; 
	bottom = temp; 
}

template<typename T> AKINLINE
bool CakRectT<T>::EqualRect(const CakRectT<T>* lpRect ) const throw()
{
	if((left	== lpRect->left	&&
		top		== lpRect->top	&&
		right	== lpRect->right	&&
		bottom	== lpRect->bottom) == true)
		return true;

	return false;
}

template<typename T> AKINLINE
void CakRectT<T>::SetRect( T x1, T y1, T x2, T y2 ) throw()
{
	left = x1;
	top = y1;
	right = x2;
	bottom = y2;	
}


template<typename T> AKINLINE
void CakRectT<T>::SetRectEmpty() throw()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;	
}




template<typename T> AKINLINE
void CakRectT<T>::CopyRect(const CakRectT<T>* lpSrcRect ) throw()
{
	left	= lpSrcRect->left	;
	top		= lpSrcRect->top	;
	right	= lpSrcRect->right	;
	bottom	= lpSrcRect->bottom	;	
}


template<typename T> AKINLINE
void CakRectT<T>::InflateRect( T x, T y ) throw()
{
	left -= x;
	top -= y;
	right += x;
	bottom += y;
}

template<typename T> AKINLINE
void CakRectT<T>::InflateRect( T l, T t, T r, T b ) throw()
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}



template<typename T> AKINLINE
void CakRectT<T>::DeflateRect( T x, T y ) throw()
{
	left += x;
	top += y;
	right -= x;
	bottom -= y;
}

template<typename T> AKINLINE
void CakRectT<T>::DeflateRect( T l, T t, T r, T b ) throw()
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}


template<typename T> AKINLINE
void CakRectT<T>::OffsetRect( T x, T y ) throw()
{
	left += x;
	top += y;
	right += x;
	bottom += y;
}



template<typename T> AKINLINE
void CakRectT<T>::NormalizeRect() throw()
{
	T nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

template<typename T> AKINLINE
void CakRectT<T>::MoveToY( T y ) throw()
{
	bottom = Height() + y; top = y;
}

template<typename T> AKINLINE
void CakRectT<T>::MoveToX( T x ) throw()
{
	right = Width() + x; left = x;
}

template<typename T> AKINLINE
void CakRectT<T>::MoveToXY( T x, T y ) throw()
{
	MoveToX(x); MoveToY(y);
}

template<typename T> AKINLINE
void CakRectT<T>::DeflateRect( CakRectT<T>* lpRect ) throw()
{
	left += lpRect->left;	top += lpRect->top;
	right -= lpRect->right;	bottom -= lpRect->bottom;
}

template<typename T> AKINLINE
void CakRectT<T>::InflateRect( CakRectT<T>* lpRect ) throw()
{
	left -= lpRect->left;		top -= lpRect->top;
	right += lpRect->right;		bottom += lpRect->bottom;
}

template<typename T> AKINLINE
void CakRectT<T>::operator=( const CakRectT<T>& srcRect ) throw()
{
	CopyRect(&srcRect);
}
template<typename T> AKINLINE
bool CakRectT<T>::operator==( const CakRectT<T>& rect ) const throw()
{
	return EqualRect(&rect);
}

template<typename T> AKINLINE
bool CakRectT<T>::operator!=( const CakRectT<T>& rect ) const throw()
{
	return !EqualRect(&rect);
}

template<typename T> AKINLINE
void CakRectT<T>::operator+=( CakRectT<T>* lpRect ) throw()
{
	InflateRect(lpRect);
}


template<typename T> AKINLINE
void CakRectT<T>::operator-=( CakRectT<T>* lpRect ) throw()
{
	DeflateRect(lpRect);
}
template<typename T> AKINLINE
CakRectT<T> CakRectT<T>::operator+( CakRectT<T>* lpRect ) const throw()
{
	CakRectT<T> rect(left,top,right,bottom); 
	rect.InflateRect(lpRect); return rect;
}

template<typename T> AKINLINE
CakRectT<T> CakRectT<T>::operator-( CakRectT<T>* lpRect ) const throw()
{
	CakRectT<T> rect(left,top,right,bottom); 
	rect.DeflateRect(lpRect); 
	return rect;
}




template<typename T> AKINLINE
CakRectT<T> CakRectT<T>::MulDiv( T nMultiplier, T nDivisor ) const throw()
{
	//나중에 다시 보자!!
	return CakRectT<T>(
		T((float)left*(float)nMultiplier/(float)nDivisor),
		T((float)top*(float)nMultiplier/(float)nDivisor),
		T((float)right*(float)nMultiplier/(float)nDivisor),
		T((float)bottom*(float)nMultiplier/(float)nDivisor)
		);
	//::MulDiv(left, nMultiplier, nDivisor),
	//::MulDiv(top, nMultiplier, nDivisor),
	//::MulDiv(right, nMultiplier, nDivisor),
	//::MulDiv(bottom, nMultiplier, nDivisor));
}

// template<typename T> AKINLINE
// CakRectT<T>& CakRectT<T>::operator=(const CakRectT<T>& rect)
// {
// 	left	= rect.left		;
// 	top		= rect.top		;
// 	right	= rect.right	;
// 	bottom	= rect.bottom	;
// 
// 	return *this;
// }


template<typename T>
bool CakRectT<T>::getCheckHeightIn( double p1 )
{
	if(top < bottom)
	{
		if( (top <= p1 && bottom >= p1 ) )
		{
			return true;
		}
	}
	else
	{
		if( (bottom <= p1 && top >= p1 ) )
		{
			return true;
		}
	}

	return false;
}

template<typename T>
bool CakRectT<T>::getCheckWidthIn( double p1 )
{
	if(left < right)
	{
		if( (left <= p1 && right >= p1 ) )
		{
			return true;
		}
	}
	else
	{
		if( (right <= p1 && left >= p1 ) )
		{
			return true;
		}
	}
	return false;
}

template<typename T>
bool CakRectT<T>::getCheckAreaIn( double x, double y )
{
	CakRectT<T> rectTemp = *this;
	rectTemp.NormalizeRect();


	if( (rectTemp.left <= x && rectTemp.right >= x )
		&& (rectTemp.top <= y && rectTemp.bottom >= y ))
	{
		return true;
	}

	return false;
}
