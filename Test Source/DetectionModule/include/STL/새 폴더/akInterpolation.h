#pragma once


#include "akLinker.h"
#include <vector>

namespace akSTL
{
	class akSTL_DLLSPEC CakInterpolation
	{
	public:
		struct _Data
		{
			_Data()
			{
				x = y = 0;
			}
			double x;
			double y;
		};

	public:
		CakInterpolation();
		~CakInterpolation();

		//������̾� ������ ����(rate:������ ����)
		void CaculationPolinomial(double min, double max, double rate);
		//ť���ö��� ������ ����(rate:���� �������� ������ ����)
		void CaculationCubicSpline(double rate);
		//Linear(rate:���� �������� ������ ����)
		void CaculationLinear(int step);

		void AddPoint(double x, double y);
		inline void ClearPoint(){m_vecInterPoint.clear();};
		
		inline _Data GetData(int index);
		inline int GetDataNum(){return (int)m_vecInterPoint.size();};

		
		
		
	protected:
		std::vector<_Data> m_vecOrgPoint;
		std::vector<_Data> m_vecInterPoint;
	private:
		void solveTridiag(double* khb, double* diag, double* khp, double* b, int n)	;
	};
}