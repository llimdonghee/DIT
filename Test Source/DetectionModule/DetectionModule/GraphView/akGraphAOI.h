#pragma once

#include "Graph/akGraphBasic3.h"

class CakGraphAOI : public CakGraphBasic3
{
public:
	CakGraphAOI(void);
	~CakGraphAOI(void);

public:
	virtual void RenderBegin(CDC* pDC, CRect rectDC);
	virtual void RenderLabel(Graphics* grfx, CDC* pDC);
	virtual void RenderMouseMoveInfo(Graphics* grfx, CDC* pDC);//���콺 �����϶� ��ġ�� ǥ���ϴºκ�
	virtual void RenderSeries(Graphics* grfx, CDC* pDC); //������ ��/�� �׸���
	virtual void SetAutoScale();
	//void SetAutoScaleHeight();
public:
	void setDataNum(int nSeriseNum, int nDataNum);
	void setData(int nSeriseIndex, int nDataIndex, int val);
	void setAxisXStringNum(int nNum);
	void setColorType( int nType, CakGraphAOI* pGraph );

public:
	int			m_nWaringLine;
	int			m_nAxisXTextNum;
	CString*	m_strAxisXText;
};
