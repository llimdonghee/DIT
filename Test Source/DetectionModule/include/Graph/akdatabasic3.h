#pragma once

//등간격 데이터가 아니고
//데이터가 여러개 존재할경우

#include "Graph/akDataBasic2.h"
#include "Graph/akGraphStruct.h"

class AFX_EXT_CLASS CakDataBasic3
{
public:
	CakDataBasic3(void);
	virtual ~CakDataBasic3(void);


public:

	virtual void SetSeriesNum(int subsetNum); //기존 데이터 초기화
	//기존 데이터 해당 인덱스에 추가(인덱스가 0보다 작거나 생성된 Subset갯수보다 많으면 젤 뒤에 추가된다)
	virtual void SetSeriesAdd(int index = -1); 
	virtual bool SetSeriesDelete(int index); //해당 인덱스 삭제

	int GetSeriesNum(){return m_SeriesNum;};

	void SetSelectSeries(int index);
	int GetSelectSeries(){return m_nSelectedSeriesindex;};

	virtual void SetDataNum(int nDataNum);
	virtual bool SetData(int index, double x, double y);

	virtual void SetHidden(bool enable, int startIndex, int endIndex);

	virtual void calculateMinMax(double* xmin, double* xmax, double* ymin, double* ymax);
protected:
	int m_SeriesNum;
	int m_nSelectedSeriesindex;
public:
	CakDataBasic2 *m_pSeriesData;
	bool *m_pSeriesHidden;
	CSeries* m_pSeries;
	CLegendOption m_LegendOption;
};
