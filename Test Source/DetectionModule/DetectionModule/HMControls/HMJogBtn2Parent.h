// by ChaeHM

typedef struct _HMJOGBTN2PARENT
{
	virtual BOOL SendMessageToParent(LPCTSTR message) = 0;
	virtual BOOL SendMessageToParent(int nMsg) = 0;
} HMJogBtn2Parent, *pHMJogBtn2Parent;