#pragma once

#include "akLinker.h"

namespace akSTL
{

	class akSTL_DLLSPEC CakCoordinate
	{
	public:
		CakCoordinate(void);
		~CakCoordinate(void);

		//Degree�� �����ʷ� ��ȯ
		static double GetDMStoDgree(int  d, int  m, double  s);
		//�����ʸ� Degree�� ��ȯ
		static void GetDgreetoDMS(double degree, int* d, int* m, double* s);
		//Degree���� 180���� + ����(W,E)�� ��ȯ
		static void GetMakeDMS180(int degree, int* rDeg, char* rDir);
		//Degree���� 90���� + ����(S,N)�� ��ȯ
		static void GetMakeDMS90(int degree, int* rDeg, char* rDir);
		//Degree���� //-180~+180�� ��ȯ
		static double GetMakeDegree180(double degree);
		//Degree���� //-90~+90�� ��ȯ
		static double GetMakeDegree90(double degree);
	};

}