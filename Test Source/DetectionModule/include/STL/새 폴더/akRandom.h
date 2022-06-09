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

		//���Ժ���, Normal, ����þ�... (�� ������)
		double Gaussian(float stddev, float mean); //stddev:���, mean:ǥ������

		static int GetRandom(int min, int max);
		
	private:
		static int ms_randomseed;
	};
}