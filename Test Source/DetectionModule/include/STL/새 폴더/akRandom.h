#pragma once

#include "akLinker.h"

namespace akSTL
{
//#define GetRandom( min, max ) ((rand() % (int)(((max)+1) - (min))) + (min))

	class akSTL_DLLSPEC CakRandom
	{
	public:
		CakRandom(void);
		~CakRandom(void);

		//정규분포, Normal, 가우시안... (다 같은말)
		double Gaussian(float stddev, float mean); //stddev:평균, mean:표준편차

		static int GetRandom(int min, int max);
		
	private:
		static int ms_randomseed;
	};
}