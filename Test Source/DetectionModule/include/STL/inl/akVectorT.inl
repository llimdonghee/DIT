
//#include "../akvectorT.h"
#define AKINLINE inline



template<typename T> AKINLINE
CakVectorT<T> CakVectorT<T>::operator-( CakVectorT<T> vector ) const throw()
{
	return CakVectorT(x - vector.x, y - vector.y, z - vector.z);
}

template<typename T> AKINLINE
CakVectorT<T> CakVectorT<T>::operator+( CakVectorT<T> vector ) const throw()
{
	return CakVectorT(x + vector.x, y + vector.y, z + vector.z);
}

template<typename T> AKINLINE
CakVectorT<T> CakVectorT<T>::operator-() const throw()
{
	return CakVectorT(-x,-y,-z);
}

template<typename T> AKINLINE
void CakVectorT<T>::operator-=( CakVectorT<T> vector ) throw()
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
}

template<typename T> AKINLINE
void CakVectorT<T>::operator+=( CakVectorT<T> vector ) throw()
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
}

template<typename T> AKINLINE
bool CakVectorT<T>::operator!=( CakVectorT<T> vector ) const throw()
{
	return (x != vector.x || y != vector.y || z != vector.z);
}

template<typename T> AKINLINE
bool CakVectorT<T>::operator==( CakVectorT<T> vector ) const throw()
{
	return (x == vector.x && y == vector.y && z == vector.z);
}

template<typename T> AKINLINE
void CakVectorT<T>::set( T X, T Y, T Z ) throw()
{
	x = X; 
	y = Y;
	z = Z;
}

template<typename T> AKINLINE
void CakVectorT<T>::Offset( CakVectorT<T> vector ) throw()
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
}

template<typename T> AKINLINE
void CakVectorT<T>::Offset( T xOffset, T yOffset, T zOffset) throw()
{
	x += xOffset;
	y += yOffset;
	z += zOffset;
}

template<typename T> AKINLINE
CakVectorT<T> CakVectorT<T>::operator*(T& a) const throw()
{
	return CakVectorT(x * a, y * a, z * a);
}


template<typename T> AKINLINE
CakVectorT<T> CakVectorT<T>::operator*( CakVectorT<T> &vec ) const throw()
{
	CakVectorT<T> vc;
	vc.x = y*vec.z - z*vec.y;
	vc.y = z*vec.x - x*vec.z;
	vc.z = x*vec.y - y*vec.x;
	return vc;
};

template<typename T> AKINLINE
void CakVectorT<T>::Normalize()
{
	double length;

	//������ ���̸� ����Ѵ�.
	length = sqrt((x*x) + (y*y) +(z*z));
	// ���̰� 0�� ����� ���Ϳ��� ������ ���� �״��Ͽ� ���α׷��� �������� �ʵ��� �Ѵ�.
	if(length == 0.0f) length = 1.0f;

	// �� ������ ������ ���̷� ������ ���� ���Ͱ� �ȴ�.
	x = x / length;
	y = y / length;
	z = z / length;

}