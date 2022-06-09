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
	//현재 설정된 체크박스가 없으면 -1, 
	int GetCheckedIndex();
	
	void SetMode(int nMode);//0은 기본 모드, 1:눌러진거 눌렀을때 다시 올라오게!!

	BOOL IsCheckAllFalse();
private:
	std::vector<CXIcon*> m_vecCheckBox;
	int m_nMode;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
