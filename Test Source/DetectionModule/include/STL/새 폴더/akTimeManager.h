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

		//���� ����(������) //��� ���� �ȹ���
		inline void SetBegin();
		inline void SetEnd();
		inline double GetRunTimeSec(); //���۽ð� �����ϰ� ����ð������� Ÿ��(�ʴ���)
		inline double GetRunTimeMsec(); //���۽ð� �����ϰ� ����ð������� Ÿ��(�ʴ���)
		inline double GetFrameRate();

		//�÷��̹� ���(�Ź���)
		void SetInit();//�ʱ� �������� �ǵ���
		virtual inline void SetPlay();
		virtual inline void SetStop();
		virtual inline void SetTime(int msec);
		virtual inline void SetPause();
		
		
		virtual inline double GetTime(int type=0); //0=Sec, 1=MSec
		virtual inline void GetTime(int* h, int* m, int* s, int* ms);
		virtual inline PlayMode GetPlayMode();
		virtual inline void SetSpeedRate(double speedrate); //��� ����(0���� ū �Ǽ� �Է°���)
		virtual inline double GetSpeedRate(); //��� ������ �б�

	protected:
		LARGE_INTEGER m_Frequency;
		LARGE_INTEGER m_BeginTime;
		LARGE_INTEGER m_Endtime;

		double m_PauseTime;
		double m_LastTime;
		PlayMode m_PlayState;
		double m_SpeedRate; //���

		//�ݹ� ������ ����
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
		void setEnd();//�÷��� �ð��� �ִ� �ð��� �Ѿ��� ��� ȣ��
	public:
		double m_timeMax;

	public:
		static void threadPlayer(void* arg);
		int m_flagThreadPlayer;

	private:
		bool m_bPlaying;
		
	};
}