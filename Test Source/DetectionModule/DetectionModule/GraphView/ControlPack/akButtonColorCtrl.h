#pragma once
#include "afxcmn.h"
#include <map>

#include "akButtonColor.h"


class CakButtonColorCtrl
{
public:
	CakButtonColorCtrl(void);
	~CakButtonColorCtrl(void);

	void clear();	//DestroyWindow()���� clearȣ���ؾߵ�!!

	void setCtrlOption(_ColorButtonOption CtrlOption);
	_ColorButtonOption getCtrlOption(){return m_CtrlOption;};

	void addControl(int nIDCtl, CWnd* pParent);


	CButton* GetDlgItem(int nIDCtl);

protected:
	std::map<int,void*> m_mapControls;
	_ColorButtonOption m_CtrlOption;
};

/*
����

//����� �Ʒ��� ���� �ϸ��..  ����[2016/9/22]
	BOOL CButtonTestDlg::OnInitDialog()
	{
		CDialog::OnInitDialog();

		_ColorButtonOption  op = m_bcFlatGradition.getCtrlOption();
		op.nStyle = 1;
		op.nEdgeStyle = 1;
		m_bcFlatGradition.setCtrlOption(op);

		m_bcFlatGradition.addControl(IDC_BUTTON_TYPE3DGRAD_FLAT_1, this);
		m_bcFlatGradition.addControl(IDC_BUTTON_TYPE3DGRAD_FLAT_2, this);
		m_bcFlatGradition.addControl(IDC_BUTTON_TYPE3DGRAD_FLAT_3, this);

		...
	}

//����(�ݵ��� destorywindow���� clearȣ���ؾߵ�) ����[2016/9/22]
	BOOL CButtonTestDlg::DestroyWindow()
	{
		m_bcFlatGradition.clear();

		return CDialog::DestroyWindow();
	}


*/
