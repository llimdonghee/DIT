#pragma once


#include "akGraph/khSyncObject.h"

class AFX_EXT_CLASS CkhDataBase
{
public:
	CkhDataBase(void);
	virtual ~CkhDataBase(void);

public:
	CkhSyncObject m_khSyncObject;
};
