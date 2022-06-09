#pragma once

#include "akSTLLinker.h"

template<typename T>
class CakPointT
{
public:
	CakPointT(void){};
	CakPointT(const T& X, const T& Y):x(X),y(Y) {};
	//~CakPointT(void){};

	// Operations

	// translate the point
	void Offset(T xOffset, T yOffset) throw();
	void Offset(CakPointT<T> point) throw();
	void SetPoint(T X, T Y) throw();

	bool operator==(CakPointT<T> point) const throw();
	bool operator!=(CakPointT<T> point) const throw();
	void operator+=(CakPointT<T> point) throw();
	void operator-=(CakPointT<T> point) throw();

	// Operators returning CPoint values
	CakPointT<T> operator-() const throw();
	CakPointT<T> operator+(CakPointT<T> point) const throw();
	CakPointT<T> operator-(CakPointT<T> point) const throw();



public:
	T x;
	T y;
};

#include "inl/akPointT.inl"