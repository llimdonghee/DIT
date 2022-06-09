#pragma once



class CakSliderCtrl : public CSliderCtrl
{
public:
	CakSliderCtrl(void);
	~CakSliderCtrl(void);

public:
	int m_testa;

	CBrush m_nBrushSlider;

	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor ( CDC* pDC, UINT nCtlColor );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
