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

	static CakRect SetakRectAlign(CakRect rect); //������ ������ ������ ������ ������ �ٲ۴�.
	static bool GetCheckInArea(CakRect rect, double x, double y);//���� �����ȿ� ���� �ִ��� �˻�
	static bool GetCheckInArea(double x1, double y1, double x2, double y2, double x, double y);//���� �����ȿ� ���� �ִ��� �˻�
	static bool GetCheckInRangeX(CakRect rect, double x1); //�Ѽ����� ���� �ش� ���� �ȿ� �ִ��� �˻�
	static bool GetCheckInRangeY(CakRect rect, double y1); //�Ѽ����� ���� �ش� ���� �ȿ� �ִ��� �˻�
	static bool GetCheckInRange(double x1, double x2, double p1); //�Ѽ����� ���� �ش� ���� �ȿ� �ִ��� �˻�
	
	static CRect akRectToCRect(CakRect rect);
	
	static BOOL GetEncCLSID(WCHAR *mime, CLSID *pClsid);

	//DC�� �޾Ƽ� �̹��� ���Ϸ� ����(0:jpg, 1:bmp);
	static bool MakeImageFile(CDC* pDC, char* filename, unsigned int imageformat=0);

	static void CalculationTickPostionNormal(
		double minval,				//�ּҰ�
		double maxval,				//�ִ밪
		unsigned int length,					//������ ����
		unsigned int limitLength,			//�ּ� ������ ����
		OUT double* tickgabval,		//������ ����
		OUT int* decimal			//�Ҽ��������ڸ���
		);			
	static void CalculationTickPostionTime(
		double minval,				//�ּҰ�
		double maxval,				//�ִ밪
		unsigned int length,					//
		unsigned int limitLength,			//
		OUT double* tickgabval		//������ ����
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
