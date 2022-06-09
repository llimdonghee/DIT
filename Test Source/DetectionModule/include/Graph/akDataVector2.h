#pragma once

#include <vector>
#include "akGraphStruct.h"

struct _DataVector2
{
	_DataVector2(double x1, double y1)
	{
		x = x1;
		y = y1;
		hidden = false;
	}
	_DataVector2()
	{
		x=y=0;
		hidden = false;
	}
	void set(double x1, double y1)
	{
		x = x1;
		y = y1;
	}
	double x;
	double y;
	bool hidden;
};

class AFX_EXT_CLASS CakDataVector2
{
public:
	CakDataVector2(void);
	~CakDataVector2(void);

public:
	CakDataVector2& operator=(CakDataVector2& data);

public:

	
	int GetDataNum(){return int(m_vecData.size());};

	virtual void Clear();
	//지정 인덱스가 존재하지 않는다면 데이터변경을 하지 않는다.
	virtual bool SetData(int index, double x, double y);
	//인덱스가 현제 존재하는 데이터 범위를 벗어나면 젤끝에 추가한다. 
	virtual void AddData(double x, double y, int index = -1); 

	virtual void SetHidden(bool enable, int startIndex, int endIndex);
	
protected:
	
public:
	std::vector<_DataVector2> m_vecData;
	CSeries m_SeriesOption;
	
};
