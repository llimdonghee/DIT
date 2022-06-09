#pragma once

#include <vector>
#include "akButtonColor.h"
class CakRadio : public CWnd
{
public:
	CakRadio(void);
	~CakRadio(void);

public:
	void AddCheckBox(CakButtonColor* checkbox);
	//���� ������ üũ�ڽ��� ������ -1, 
	int GetCheckedIndex();
	
	void SetMode(int nMode);//0�� �⺻ ���, 1:�������� �������� �ٽ� �ö����!!

	BOOL IsCheckAllFalse();
private:
	std::vector<CakButtonColor*> m_vecCheckBox;
	int m_nMode;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
