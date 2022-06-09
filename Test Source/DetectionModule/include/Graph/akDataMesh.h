#pragma once

//�ø�� �ϳ��� �ְ� ��� �����Ͱ� �ƴҰ��


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
	double m_DataValueYMin, m_DataValueYMax;//  <<--�߿���.. ���� �������� ������ ������ ����� ��������ߵ�.
	double m_DataValueXMin, m_DataValueXMax;//  <<--�߿���.. ���� �������� ������ ������ ����� ��������ߵ�.

	double **m_pData;

	double m_NoDataValue; //�����Ͱ� ���� ���� �� ó��

protected:
	int m_dataNumX, m_dataNumY;
};
