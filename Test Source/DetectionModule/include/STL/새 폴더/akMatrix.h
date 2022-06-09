#pragma once


#include "akLinker.h"

namespace akSTL
{
	class akSTL_DLLSPEC CakMatrix4x4
	{
	public:
		CakMatrix4x4(void);
		~CakMatrix4x4(void);

		inline bool MatrixClear();	//��Ʈ���� ����
		inline bool SetMatrix(double *matrix);	//��Ʈ���� ����
		inline bool SetRotateH(double H);	//ȸ�� ��Ʈ������ ����
		inline bool SetRotateP(double P);	//ȸ�� ��Ʈ������ ����
		inline bool SetRotateR(double R);	//ȸ�� ��Ʈ������ ����
		inline bool SetTransform(double x, double y, double z);	//�̵� ��Ʈ������ ����
		inline bool MakeResult(double* x, double* y, double* z);//��Ʈ���� ���� ��ǥ�� ����
		inline bool MakeResult(float* x, float* y, float* z);//��Ʈ���� ���� ��ǥ�� ����
		inline void MatrixPrint();	//���� ��Ʈ���� ���
		inline CakMatrix4x4& operator=(const CakMatrix4x4& matrix);
		inline CakMatrix4x4& operator*(const CakMatrix4x4& matrix);
	protected:
		inline bool Multiplication(double* matrix);	//���� ��Ʈ������ �Է°� ����

	public:
		double m_Matrix[16];

		
	private:
		bool m_bSet;	//�ʱ�ȭ�� �Ǿ��ִ��� Ȯ��

	};
}