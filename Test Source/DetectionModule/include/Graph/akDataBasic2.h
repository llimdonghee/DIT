#pragma once

//시리즈가 하나만 있고 등간격 데이터가 아닐경우

struct _DataBasic2
{
	_DataBasic2(double x1, double y1)
	{
		x = x1;
		y = y1;
	}
	_DataBasic2()
	{
		x=y=0;
	}
	void set(double x1, double y1)
	{
		x = x1;
		y = y1;
	}
	double x;
	double y;
};

class AFX_EXT_CLASS CakDataBasic2
{
public:
	CakDataBasic2(void);
	~CakDataBasic2(void);

public:
	CakDataBasic2& operator=(CakDataBasic2& data);

public:

	virtual void SetDataNum(int datanum);
	int GetDataNum(){return m_dataNum;};
	void SetData(int index, double x, double y);

	void SetHidden(bool enable, int startIndex, int endIndex);
	
protected:
	int m_dataNum;
public:
	_DataBasic2 *m_pData;
	bool *m_pHidden;
};
