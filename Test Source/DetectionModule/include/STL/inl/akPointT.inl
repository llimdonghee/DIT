
//#include "../akPointT.h"
#define AKINLINE inline



template<typename T> AKINLINE
CakPointT<T> CakPointT<T>::operator-( CakPointT<T> point ) const throw()
{
	return CakPointT(x - point.x, y - point.y);
}

template<typename T> AKINLINE
CakPointT<T> CakPointT<T>::operator+( CakPointT<T> point ) const throw()
{
	return CakPointT(x + point.x, y + point.y);
}

template<typename T> AKINLINE
CakPointT<T> CakPointT<T>::operator-() const throw()
{
	return CakPointT(-x,-y);
}

template<typename T> AKINLINE
void CakPointT<T>::operator-=( CakPointT<T> point ) throw()
{
	x -= point.x;
	y -= point.y;
}

template<typename T> AKINLINE
void CakPointT<T>::operator+=( CakPointT<T> point ) throw()
{
	x += point.x;
	y += point.y;
}

template<typename T> AKINLINE
bool CakPointT<T>::operator!=( CakPointT<T> point ) const throw()
{
	return (x != point.x || y != point.y);
}

template<typename T> AKINLINE
bool CakPointT<T>::operator==( CakPointT<T> point ) const throw()
{
	return (x == point.x && y == point.y);
}

template<typename T> AKINLINE
void CakPointT<T>::SetPoint( T X, T Y ) throw()
{
	x = X; 
	y = Y;
}

template<typename T> AKINLINE
void CakPointT<T>::Offset( CakPointT<T> point ) throw()
{
	x += point.x;
	y += point.y;
}

template<typename T> AKINLINE
void CakPointT<T>::Offset( T xOffset, T yOffset ) throw()
{
	x += xOffset;
	y += yOffset;
}
