#pragma once

//시리즈가 하나만 있고 등간격 데이터가 아닐경우


class AFX_EXT_CLASS CakDataMesh
{
public:
	CakDataMesh(void);
	~CakDataMesh(void);

public:
	CakDataMesh& operator=(CakDataMesh& data);

public:
	virtual void SetDataNum(int sizex, int sizey);
	virtual int GetDataNumX(){return m_dataNumX;};
	virtual int GetDataNumY(){return m_dataNumY;};
	
	virtual void SetNoDataValue(double val){m_NoDataValue = val;};

	virtual void SetDataValueX(double min, double max){m_DataValueXMin = min;m_DataValueXMax = max;};
	virtual void SetDataValueY(double min, double max){m_DataValueYMin = min;m_DataValueYMax = max;};
public:
	double m_DataValueYMin, m_DataValueYMax;//  <<--중요함.. 들어온 데이터의 가로축 범위가 어딘지 셋팅해줘야됨.
	double m_DataValueXMin, m_DataValueXMax;//  <<--중요함.. 들어온 데이터의 세로축 범위가 어딘지 셋팅해줘야됨.

	double **m_pData;

	double m_NoDataValue; //데이터가 없는 곳의 값 처리

protected:
	int m_dataNumX, m_dataNumY;
};
