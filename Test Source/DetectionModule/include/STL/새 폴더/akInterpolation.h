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

		//폴리노미얼 데이터 생성(rate:데이터 간격)
		void CaculationPolinomial(double min, double max, double rate);
		//큐빅스플라인 데이터 생성(rate:점과 점사이의 데이터 갯수)
		void CaculationCubicSpline(double rate);
		//Linear(rate:점과 점사이의 데이터 갯수)
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