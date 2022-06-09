#pragma once
#include <atlstr.h>
#include <vector>
#include "XIcon.h"

class CIconRadio : public CWnd
{
public:
	CIconRadio(void);
	~CIconRadio(void);

public:
	void AddCheckBox(CXIcon* checkbox);
	//���� ������ üũ�ڽ��� ������ -1, 
	int GetCheckedIndex();
	
	void SetMode(int nMode);//0�� �⺻ ���, 1:�������� �������� �ٽ� �ö����!!

	BOOL IsCheckAllFalse();
private:
	std::vector<CXIcon*> m_vecCheckBox;
	int m_nMode;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
