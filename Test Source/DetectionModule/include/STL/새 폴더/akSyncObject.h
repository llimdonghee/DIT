#pragma once

#include "akLinker.h"

namespace akSTL
{
	class akSTL_DLLSPEC CakSyncObject
	{
	public:
		CakSyncObject(void);
		virtual ~CakSyncObject(void);

	public:
		//�б� ������ �Ͽ�����... �̹� ��� ���� �ִٸ�.. ��ٷȴٰ� ������ �����Ѵ�.
		//bWait = false �ϰ��.. ��򰡿��� ���� �ִٸ� false�� ��ȯ�ϰ� ����������.
		bool SetRead(bool bWait = true);  
		//���� ������ �Ͽ�����... �̹� ��� �а� �ִٸ�.. ��ٷȴٰ� ������ �����Ѵ�.
		bool SetWrite(bool bWait = true);
		int SetReadRelease(); //���� �����ִ� �б� ī��Ʈ ��ȯ
		void SetWriteRelease();

		int GetReadCount(){return m_nReadCount;}; //�б� ȣ���� Ƚ�� ��ȯ
		bool GetWriteState(){return m_bWrite;}; //0�� �ɶ����� �׸��� ��ƾ�� ���� ������ ����.
		

	protected:
		
	private:
		bool m_bWrite; 
		int m_nReadCount;
		
	};
}