#pragma once

#include "akSTLLinker.h"


#define _USE_MATH_DEFINES
#include <math.h>

template<typename T>
class CakVectorT
{
public:
	CakVectorT(void){};
	CakVectorT(const T& X, const T& Y, const T& Z):x(X),y(Y),z(Z) {};
	//~CakVectorT(void){};

	// Operations

	// translate the point
	void Offset(T xOffset, T yOffset, T zOffset) throw();
	void Offset(CakVectorT<T> vector) throw();
	void set(T X, T Y, T Z) throw();

	bool operator==(CakVectorT<T> vector) const throw();
	bool operator!=(CakVectorT<T> vector) const throw();
	void operator+=(CakVectorT<T> vector) throw();
	void operator-=(CakVectorT<T> vector) throw();

	// Operators returning CakVector values
	CakVectorT<T> operator-() const throw();
	CakVectorT<T> operator+(CakVectorT<T> vector) const throw();
	CakVectorT<T> operator-(CakVectorT<T> vector) const throw();

	CakVectorT<T> operator*(T& a) const throw();
	CakVectorT<T> operator*(CakVectorT<T> &vec) const throw();
	

	//³»Àû
	T Dot(CakVectorT<T> vec){	return (vec.x * x + vec.y * y + vec.z * z);	};
	T Mag()					{	return sqrt(x*x+y*y);		};
	T getLength()			{	return sqrt(x*x+y*y+z*z);	};
	void Normalize();
public:
	T x,y,z;
};




#include "inl/akVectorT.inl"