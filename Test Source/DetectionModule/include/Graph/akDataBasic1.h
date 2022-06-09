#pragma once

//시리즈가 하나이고 등간격 데이터 일 경우

class AFX_EXT_CLASS CakDataBasic1
{
public:
	CakDataBasic1(void);
	~CakDataBasic1(void);



public:

	virtual void SetDataNum(int datanum);
	int GetDataNum(){return m_dataNum;};
	void SetData(int index, double value);

	void SetHidden(bool enable, int startIndex, int endIndex);
	
protected:
	int m_dataNum;
public:
	double *m_pData;
	bool *m_pHidden;
};
