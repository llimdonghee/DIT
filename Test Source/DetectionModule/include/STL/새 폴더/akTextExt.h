#pragma once

#include "akText.h"

//ÀÀ¿ë1
#define AKTEXT CakTextExt::getInstance()
class akSTL_DLLSPEC CakTextExt : public CakText
{
public:
	CakTextExt(void){};
	virtual ~CakTextExt(void){};

	static CakText* getInstance(){return &m_akText;};
private:
	static CakText m_akText;
};
