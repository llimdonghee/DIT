#pragma once

#include "Graph/akGraphLinker.h"
#include "STL/akStruct.h"

#define AKSWAP_INT(a,b) { int t=a; a=b; b=t; }
#define AKSWAP_DOUBLE(a,b) { double t=a; a=b; b=t; }
#define AKSWAP_FLOAT(a,b) { float t=a; a=b; b=t; }

class AFX_EXT_CLASS CakGraphUtil
{
public:
	CakGraphUtil(void);
	~CakGraphUtil(void);

	static CakRect SetakRectAlign(CakRect rect); //왼쪽이 작은값 윗쪽이 작은값 순으로 바꾼다.
	static bool GetCheckInArea(CakRect rect, double x, double y);//현재 영역안에 점이 있는지 검사
	static bool GetCheckInArea(double x1, double y1, double x2, double y2, double x, double y);//현재 영역안에 점이 있는지 검사
	static bool GetCheckInRangeX(CakRect rect, double x1); //한선상의 점이 해당 범위 안에 있는지 검사
	static bool GetCheckInRangeY(CakRect rect, double y1); //한선상의 점이 해당 범위 안에 있는지 검사
	static bool GetCheckInRange(double x1, double x2, double p1); //한선상의 점이 해당 범위 안에 있는지 검사
	
	static CRect akRectToCRect(CakRect rect);
	
	static BOOL GetEncCLSID(WCHAR *mime, CLSID *pClsid);

	//DC를 받아서 이미지 파일로 제작(0:jpg, 1:bmp);
	static bool MakeImageFile(CDC* pDC, char* filename, unsigned int imageformat=0);

	static void CalculationTickPostionNormal(
		double minval,				//최소값
		double maxval,				//최대값
		unsigned int length,					//윈도우 길이
		unsigned int limitLength,			//최소 윈도우 길이
		OUT double* tickgabval,		//값들의 간격
		OUT int* decimal			//소수점이하자릿수
		);			
	static void CalculationTickPostionTime(
		double minval,				//최소값
		double maxval,				//최대값
		unsigned int length,					//
		unsigned int limitLength,			//
		OUT double* tickgabval		//값들의 간격
		);			
	static void CalculationTickPostionMSecTime(
		double minval,				
		double maxval, 
		unsigned int length, 
		unsigned int limitLength, 
		OUT double* tickgabval
		);
	static void CalculationTickPostionDegree(
		double minval,
		double maxval,
		int length,
		int limitLength, 
		OUT double* tickgabval 
		);
};
