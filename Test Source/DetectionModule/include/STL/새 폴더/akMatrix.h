#pragma once


#include "akLinker.h"

namespace akSTL
{
	class akSTL_DLLSPEC CakMatrix4x4
	{
	public:
		CakMatrix4x4(void);
		~CakMatrix4x4(void);

		inline bool MatrixClear();	//매트릭스 리셋
		inline bool SetMatrix(double *matrix);	//매트릭스 세팅
		inline bool SetRotateH(double H);	//회전 매트리릭스 곱셈
		inline bool SetRotateP(double P);	//회전 매트리릭스 곱셈
		inline bool SetRotateR(double R);	//회전 매트리릭스 곱셈
		inline bool SetTransform(double x, double y, double z);	//이동 매트리릭스 곱셈
		inline bool MakeResult(double* x, double* y, double* z);//매트릭스 적용 좌표값 산출
		inline bool MakeResult(float* x, float* y, float* z);//매트릭스 적용 좌표값 산출
		inline void MatrixPrint();	//현재 매트릭스 출력
		inline CakMatrix4x4& operator=(const CakMatrix4x4& matrix);
		inline CakMatrix4x4& operator*(const CakMatrix4x4& matrix);
	protected:
		inline bool Multiplication(double* matrix);	//현재 매트릭스에 입력값 곱셈

	public:
		double m_Matrix[16];

		
	private:
		bool m_bSet;	//초기화가 되어있는지 확인

	};
}