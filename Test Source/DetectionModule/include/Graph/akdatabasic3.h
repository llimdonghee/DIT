#pragma once

//��� �����Ͱ� �ƴϰ�
//�����Ͱ� ������ �����Ұ��

#include "Graph/akDataBasic2.h"
#include "Graph/akGraphStruct.h"

class AFX_EXT_CLASS CakDataBasic3
{
public:
	CakDataBasic3(void);
	virtual ~CakDataBasic3(void);


public:

	virtual void SetSeriesNum(int subsetNum); //���� ������ �ʱ�ȭ
	//���� ������ �ش� �ε����� �߰�(�ε����� 0���� �۰ų� ������ Subset�������� ������ �� �ڿ� �߰��ȴ�)
	virtual void SetSeriesAdd(int index = -1); 
	virtual bool SetSeriesDelete(int index); //�ش� �ε��� ����

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
