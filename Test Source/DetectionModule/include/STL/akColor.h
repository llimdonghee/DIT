#pragma once

#include "akSTLLinker.h"


//namespace akSTL
//{
	class AKSTL_DLLSPEC akColor4f
	{
	public:
		akColor4f(void);
		akColor4f(float R, float G, float B, float A=0.0f);
		~akColor4f(void);


		void set(float R, float G, float B, float A=0.0f);
		inline akColor4f& operator=(const akColor4f& ksgcolor);

	public:
		float r,g,b,a;
	};

//};
