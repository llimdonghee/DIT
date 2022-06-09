#pragma once

#include "akSTLLinker.h"
#include "akRectT.h"
#include "akPoint.h"

#define CakRectToCRect(akRect) CRect(akRect.left, akRect.top, akRect.right, akRect.bottom)
#define CRectToCakRect(rect) CakRect(rect.left, rect.top, rect.right, rect.bottom)
#define CakRectSet(rectDes, rectSrc) rectDes.SetRect(rectSrc.left, rectSrc.top, rectSrc.right, rectSrc.bottom)

class AKSTL_DLLSPEC CakRect : public CakRectT<long>
{
public:

	CakRect(){};
	CakRect(const long l, const long t, const long r, const long b){
		left=l,top=t,right=r,bottom=b;
	};
	virtual ~CakRect(){};
	
public:
	
	// reference to the top-left point
	CakPoint TopLeft() throw();
	// reference to the bottom-right point
	CakPoint BottomRight() throw();
	// const reference to the top-left point
	const CakPoint TopLeft() const throw();
	// const reference to the bottom-right point
	const CakPoint BottomRight() const throw();
	// the geometric center point of the rectangle
	CakPoint CenterPoint() const throw();

	bool PtInRect(CakPoint point) const throw();
	
};

class AKSTL_DLLSPEC CakRectn : public CakRectT<int>
{
public:

	CakRectn(){};
	CakRectn(int l, int t, int r, int b){
		left=l,top=t,right=r,bottom=b;
	};
	virtual ~CakRectn(){};
	
public:

	// reference to the top-left point
	CakPoint TopLeft() throw();
	// reference to the bottom-right point
	CakPoint BottomRight() throw();
	// const reference to the top-left point
	const CakPoint TopLeft() const throw();
	// const reference to the bottom-right point
	const CakPoint BottomRight() const throw();
	// the geometric center point of the rectangle
	CakPoint CenterPoint() const throw();

	bool PtInRect(CakPoint point) const throw();

};

class AKSTL_DLLSPEC CakRectf : public CakRectT<float>
{
public:

	CakRectf(){};
	CakRectf(float l, float t, float r, float b){
		left=l,top=t,right=r,bottom=b;
	};
	virtual ~CakRectf(){};

public:

	// reference to the top-left point
	CakPointf& TopLeft() throw();
	// reference to the bottom-right point
	CakPointf& BottomRight() throw();
	// const reference to the top-left point
	const CakPointf& TopLeft() const throw();
	// const reference to the bottom-right point
	const CakPointf& BottomRight() const throw();
	// the geometric center point of the rectangle
	CakPointf CenterPoint() const throw();

	bool PtInRect(CakPointf point) const throw();

};

class AKSTL_DLLSPEC CakRectd : public CakRectT<double>
{
public:

	CakRectd(){};
	CakRectd(double l, double t, double r, double b){
		left=l,top=t,right=r,bottom=b;
	};
	virtual ~CakRectd(){};

public:

	// reference to the top-left point
	CakPointd TopLeft() throw();
	// reference to the bottom-right point
	CakPointd BottomRight() throw();
	// const reference to the top-left point
	const CakPointd TopLeft() const throw();
	// const reference to the bottom-right point
	const CakPointd BottomRight() const throw();
	// the geometric center point of the rectangle
	CakPointd CenterPoint() const throw();

	bool PtInRect(CakPointd point) const throw();

};