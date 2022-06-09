#pragma once

#include <vector>
#include <io.h>

using namespace std;
typedef vector<_finddatai64_t> VECFINDDATA;

class CakFileUtil
{
public:
	CakFileUtil(void);
	virtual ~CakFileUtil(void);

	//�м���� �ʱ�ȭ
	void clear();

	//���� ã��
	void FindFile(char* pTargetPathName, bool bFindSubDir = true);
	//���� �̸� ����
	void RenameFile(char* pTargetPathName, char* pNewFileName, bool bFindSubDir = true);
	void RenameFile(char* pTargetPathName, char* pFindWord, char* pReplaceWord, bool bFindSubDir = true);
	//���ϻ���
	void DeleteFile( char* pTargetPathName, bool bFindSubDir /*= true*/ );

	//���� ã��
	void FindFolder(char* pTargetPath, bool bFindSubDir = true);
	//���� �̸� ����
	void RenameFolder(char* pTargetPath, char* pNewFolderName, bool bFindSubDir = true);
	void RenameFolder(char* pTargetPath, char* pFindWord, char* pReplaceWord, bool bFindSubDir = true);
	//���� ����
	static	void DeleteFolder( char* pTargetPath, bool bFindSubDir /*= true*/ );

	//������ ���� ����
	int getLastError(){return m_nLastError;};

	//�м� ��� ���
	VECFINDDATA* getFindData(){return &m_vecFinds;};
	int getFindSubDirCount(){return m_nFindSubCount;};
	VECFINDDATA* getProcessData(){return &m_vecProcess;};
	_finddatai64_t* getFindData(char* pName);
	char* getProcessPath(){return m_nProcessPath;};
	int getFindDataNum(){return (int)m_vecFinds.size();};
	_finddatai64_t* getFindData(int nIndex){return &m_vecFinds[nIndex];};

	//������ ���� ����[2016/4/5]
	static int CopyFolder(char* strSrc, char* strDest, bool bSubDir = true);
	static int MoveFolder(char* strSrc, char* strDest, bool bSubDir = true);

	static void MakeDirectory(char* pPath);

	//�Ϲݱ���Լ�
	static char* getFileName(char* pPathFileName);
	static char* getFileExt(char* pPathFileName);
	static void getPath(char* strDest, int nDestSize, const char* strSource);
	static int getLastPath(char* strDest, int nDestSize, char* strSource);
	static char* getLastPath(char* pPath);


	//������� ����(ex. ./aaa/bb/./bbb/../ -> ./aaa/bb/)
	static void changeAbsolutePath(char* pPathFileName);

protected:
	void FindFileSubDir(char* pTargetPath, char* pWildcard);
	//void DeleteFolderSubDir(char* pTargetPath);
	void makeAbPathWildcard(char* pTargetPath, char* pNewPath, int nNewPathSize, char* pWildcard, int nWildcardSize); //���� ��� ������ ���ϵ� ī�带 �и� �ϴ� ����
	void FindFolderSubDir(const char* pTargetPath, const char* pWildcard);
	void makeAbPathWildcardFolder(char* pTargetPath, char* pNewPath, int nNewPathSize, char* pWildcard, int nWildcardSize); //���� ��� ������ ���ϵ� ī�带 �и� �ϴ� ����(������ �����Ǿ��� ���)
	int isNetworkPath( char* pPath );

protected:
	char m_nProcessPath[512];
	VECFINDDATA m_vecFinds;
	VECFINDDATA m_vecProcess;
	int m_nLastError;
	int m_nFindSubCount;
};
