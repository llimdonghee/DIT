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
	//현재 설정된 체크박스가 없으면 -1, 
	int GetCheckedIndex();
	
	void SetMode(int nMode);//0은 기본 모드, 1:눌러진거 눌렀을때 다시 올라오게!!

	BOOL IsCheckAllFalse();
private:
	std::vector<CakButtonColor*> m_vecCheckBox;
	int m_nMode;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
