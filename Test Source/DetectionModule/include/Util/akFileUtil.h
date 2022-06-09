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

	//분석결과 초기화
	void clear();

	//파일 찾기
	void FindFile(char* pTargetPathName, bool bFindSubDir = true);
	//파이 이름 변경
	void RenameFile(char* pTargetPathName, char* pNewFileName, bool bFindSubDir = true);
	void RenameFile(char* pTargetPathName, char* pFindWord, char* pReplaceWord, bool bFindSubDir = true);
	//파일삭제
	void DeleteFile( char* pTargetPathName, bool bFindSubDir /*= true*/ );

	//폴더 찾기
	void FindFolder(char* pTargetPath, bool bFindSubDir = true);
	//폴더 이름 변경
	void RenameFolder(char* pTargetPath, char* pNewFolderName, bool bFindSubDir = true);
	void RenameFolder(char* pTargetPath, char* pFindWord, char* pReplaceWord, bool bFindSubDir = true);
	//폴더 삭제
	static	void DeleteFolder( char* pTargetPath, bool bFindSubDir /*= true*/ );

	//마지막 에러 정보
	int getLastError(){return m_nLastError;};

	//분석 결과 얻기
	VECFINDDATA* getFindData(){return &m_vecFinds;};
	int getFindSubDirCount(){return m_nFindSubCount;};
	VECFINDDATA* getProcessData(){return &m_vecProcess;};
	_finddatai64_t* getFindData(char* pName);
	char* getProcessPath(){return m_nProcessPath;};
	int getFindDataNum(){return (int)m_vecFinds.size();};
	_finddatai64_t* getFindData(int nIndex){return &m_vecFinds[nIndex];};

	//폴더간 복사 태현[2016/4/5]
	static int CopyFolder(char* strSrc, char* strDest, bool bSubDir = true);
	static int MoveFolder(char* strSrc, char* strDest, bool bSubDir = true);

	static void MakeDirectory(char* pPath);

	//일반기능함수
	static char* getFileName(char* pPathFileName);
	static char* getFileExt(char* pPathFileName);
	static void getPath(char* strDest, int nDestSize, const char* strSource);
	static int getLastPath(char* strDest, int nDestSize, char* strSource);
	static char* getLastPath(char* pPath);


	//경로정리 변경(ex. ./aaa/bb/./bbb/../ -> ./aaa/bb/)
	static void changeAbsolutePath(char* pPathFileName);

protected:
	void FindFileSubDir(char* pTargetPath, char* pWildcard);
	//void DeleteFolderSubDir(char* pTargetPath);
	void makeAbPathWildcard(char* pTargetPath, char* pNewPath, int nNewPathSize, char* pWildcard, int nWildcardSize); //절대 경로 생성과 와일드 카드를 분리 하는 역할
	void FindFolderSubDir(const char* pTargetPath, const char* pWildcard);
	void makeAbPathWildcardFolder(char* pTargetPath, char* pNewPath, int nNewPathSize, char* pWildcard, int nWildcardSize); //절대 경로 생성과 와일드 카드를 분리 하는 역할(폴더로 구성되었을 경우)
	int isNetworkPath( char* pPath );

protected:
	char m_nProcessPath[512];
	VECFINDDATA m_vecFinds;
	VECFINDDATA m_vecProcess;
	int m_nLastError;
	int m_nFindSubCount;
};
