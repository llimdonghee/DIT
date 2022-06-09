#pragma once

#include "akLinker.h"
#include <Windows.h>


namespace akSTL
{
	class akSTL_DLLSPEC CakTimeManager
	{
	public:
		enum PlayMode
		{
			Play,
			Stop,
			Pause,
			Playing,
			PlayEnd,
			SettingTime
		};
	public:
		CakTimeManager(void);
		virtual ~CakTimeManager(void);

		//시작 중지(구버전) //배속 적용 안받음
		inline void SetBegin();
		inline void SetEnd();
		inline double GetRunTimeSec(); //시작시간 저장하고 종료시간까지의 타임(초단위)
		inline double GetRunTimeMsec(); //시작시간 저장하고 종료시간까지의 타임(초단위)
		inline double GetFrameRate();

		//플레이바 기능(신버전)
		void SetInit();//초기 설정으로 되돌림
		virtual inline void SetPlay();
		virtual inline void SetStop();
		virtual inline void SetTime(int msec);
		virtual inline void SetPause();
		
		
		virtual inline double GetTime(int type=0); //0=Sec, 1=MSec
		virtual inline void GetTime(int* h, int* m, int* s, int* ms);
		virtual inline PlayMode GetPlayMode();
		virtual inline void SetSpeedRate(double speedrate); //배속 설정(0보다 큰 실수 입력가능)
		virtual inline double GetSpeedRate(); //배속 설정값 읽기

	protected:
		LARGE_INTEGER m_Frequency;
		LARGE_INTEGER m_BeginTime;
		LARGE_INTEGER m_Endtime;

		double m_PauseTime;
		double m_LastTime;
		PlayMode m_PlayState;
		double m_SpeedRate; //배속

		//콜백 데이터 관리
	public:
		typedef void (*MsgCallbackFunc) (int msg, void*);
		void setMsgCallbackFunc(MsgCallbackFunc userFunc, void* data = NULL)
		{
			m_msgCallbackFunc = userFunc;
			m_MsgCallbackFuncData = data;
		};
	protected:
		void* m_MsgCallbackFuncData;
		MsgCallbackFunc m_msgCallbackFunc;
		int m_nSyncObject;
	};

	class akSTL_DLLSPEC CakTimeManagerAdv : public CakTimeManager
	{
	public:
		CakTimeManagerAdv(void);
		virtual ~CakTimeManagerAdv(void);

	public:
		virtual inline void SetPlay();
		virtual inline void SetTime(int msec);
		virtual inline void SetStop();
	protected:
		void setEnd();//플레이 시간이 최대 시간을 넘었을 경우 호출
	public:
		double m_timeMax;

	public:
		static void threadPlayer(void* arg);
		int m_flagThreadPlayer;

	private:
		bool m_bPlaying;
		
	};
}