#pragma once

#include "akLinker.h"

namespace akSTL
{

	class akSTL_DLLSPEC CakCoordinate
	{
	public:
		CakCoordinate(void);
		~CakCoordinate(void);

		//Degree를 도분초로 변환
		static double GetDMStoDgree(int  d, int  m, double  s);
		//도분초를 Degree로 변환
		static void GetDgreetoDMS(double degree, int* d, int* m, double* s);
		//Degree값을 180방위 + 방향(W,E)로 변환
		static void GetMakeDMS180(int degree, int* rDeg, char* rDir);
		//Degree값을 90방위 + 방향(S,N)로 변환
		static void GetMakeDMS90(int degree, int* rDeg, char* rDir);
		//Degree값을 //-180~+180로 변환
		static double GetMakeDegree180(double degree);
		//Degree값을 //-90~+90로 변환
		static double GetMakeDegree90(double degree);
	};

}