#include "StdAfx.h"
#include "akFileUtil.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h> // exit()
#include <io.h>
#include <direct.h>
#include <errno.h>


CakFileUtil::CakFileUtil(void)
{
	m_nLastError = 1;
	m_nFindSubCount=0;
}

CakFileUtil::~CakFileUtil(void)
{
}


void CakFileUtil::clear()
{
	// 	int i,nSize = (int)m_vecFiles.size();
	// 	for(i=0; i<nSize; i++)
	// 	{
	// 		delete [] m_vecFiles[i];
	// 	}
	m_vecFinds.clear();
	m_vecProcess.clear();
	m_nFindSubCount = 0;
	m_nLastError = 1;
	//memset(m_nProcessPath, 0, 512);
}



void CakFileUtil::makeAbPathWildcardFolder( char* pTargetPath, char* pNewPath, int nNewPathSize, char* pWildcard, int nWildcardSize )
{
	//char wildcard[256];char targetPath[512];
	//{
	char drive[_MAX_DRIVE];               // 드라이브 명
	char dir[_MAX_DIR];                      // 디렉토리 경로
	char fname[_MAX_FNAME];           // 파일명
	char ext[_MAX_EXT];                    // 확장자 명

	// 입력한 전체 경로를 각각 쪼개서 저장한다.
	_splitpath(pTargetPath, drive, dir, fname, ext);
	int nNetPathPos = isNetworkPath(pTargetPath);


	//getPath(pNewPath, nNewPathSize, pTargetPath);
	//strcpy(pWildcard, getFileName(pTargetPath));
	//int pathpoint = getLastPath(pWildcard, nWildcardSize, pTargetPath);
	//strncpy(pNewPath, pTargetPath, pathpoint+1);

	strcpy(pNewPath, drive);
	strcat(pNewPath, dir);

	strcpy(pWildcard, fname);
	strcat(pWildcard, ext);

	//상대경로일경우
	if(drive[0] == 0 && nNetPathPos <=0 )
	{
		char pathTemp[512];
		GetCurrentDirectory(512, pathTemp);
		strcat(pathTemp, "\\");
		strcat(pathTemp, pNewPath);
		strcpy(pNewPath, pathTemp);
	}
	else//절대경로일경우
	{
		int enpoint = (int)strlen(pNewPath)-1;
		if(pNewPath[enpoint] != '\\' ||
			pNewPath[enpoint] != '/' )
		{
			pNewPath[enpoint] = '\\';
			pNewPath[enpoint+1] = 0;
		}
	}

	if(nNetPathPos<=0)
	{
		changeAbsolutePath(pNewPath);
	}

	//}
}



void CakFileUtil::makeAbPathWildcard( char* pTargetPath, char* pNewPath, int nNewPathSize, char* pWildcard, int nWildcardSize )
{
	//char wildcard[256];char targetPath[512];
	//{
	char drive[_MAX_DRIVE];               // 드라이브 명
	char dir[_MAX_DIR];                      // 디렉토리 경로
	char fname[_MAX_FNAME];           // 파일명
	char ext[_MAX_EXT];                    // 확장자 명

	// 입력한 전체 경로를 각각 쪼개서 저장한다.
	_splitpath(pTargetPath, drive, dir, fname, ext);
	int nNetPathPos = isNetworkPath(pTargetPath);

	// 		getPath(pNewPath, nNewPathSize, pTargetPath);
	// 		strcpy(pWildcard, getFileName(pTargetPath));
	strcpy(pNewPath, drive);
	strcat(pNewPath, dir);

	strcpy(pWildcard, fname);
	strcat(pWildcard, ext);

	//상대경로일경우
	if(drive[0] == 0 && nNetPathPos <=0 )
	{
		char pathTemp[512];
		GetCurrentDirectory(512, pathTemp);
		strcat(pathTemp, "\\");
		strcat(pathTemp, pNewPath);
		strcpy(pNewPath, pathTemp);
	}
	else//절대경로일경우
	{
		int enpoint = (int)strlen(pNewPath)-1;
		if(pNewPath[enpoint] != '\\' ||
			pNewPath[enpoint] != '/' )
		{
			pNewPath[enpoint] = '\\';
			pNewPath[enpoint+1] = 0;
		}
	}

	if(nNetPathPos<=0)
	{
		changeAbsolutePath(pNewPath);
	}
	//}
}


void CakFileUtil::FindFolderSubDir(const char* pTargetPath, const char* pWildcard )
{
	m_nFindSubCount++;

	_finddatai64_t c_file;
	_finddatai64_t c_fileTemp;
	intptr_t hFile;

	char path[512]={0};


	strcpy(path, pTargetPath);
	strcat(path, "\\");
	strcat(path, pWildcard);


	//파일이 있는지 검사
	hFile = _findfirsti64(path, &c_file);
	if (hFile != -1L)
	{
		do
		{
			if (
				(c_file.attrib & _A_SUBDIR)
				&&(c_file.name[0] != '.')
				)
			{
				c_fileTemp = c_file;
				strcpy(c_fileTemp.name, pTargetPath);
				strcat(c_fileTemp.name, c_file.name);
				m_vecFinds.push_back(c_fileTemp);
			}


		} while(_findnexti64(hFile, &c_file) == 0);
		_findclose(hFile);
	}

	//if(bFindSubDir)
	{
		char* pathSub = path+strlen(pTargetPath);
		strcpy(pathSub, "*.*");
		hFile = _findfirsti64(path, &c_file);
		if (hFile != -1L)
		{
			do
			{
				if ( c_file.attrib & _A_SUBDIR )
				{
					if (c_file.name[0] != '.')
					{
						strcpy(pathSub, c_file.name);
						strcat(pathSub, "\\");
						FindFolderSubDir(path, pWildcard);

						pathSub[0] = 0;
						//_chdir("..");
					}

				}


			} while(_findnexti64(hFile, &c_file) == 0);
			_findclose(hFile);
		}
	}



	m_nLastError = 1;
}

void CakFileUtil::FindFolder( char* pTargetPath, bool bFindSubDir /*= true*/ )
{
	//초기화
	clear();

	//경로 편집
	char path[512]={0};
	_finddatai64_t c_file;
	_finddatai64_t c_fileTemp;
	intptr_t hFile;


	char wildcard[256];char targetPath[512];
	makeAbPathWildcardFolder(pTargetPath, targetPath, 512, wildcard, 256);


	//현재 폴더에 파일 검사
	strcpy(path, targetPath);
	strcat(path, wildcard);
	TRACE("%s\n%s\n", targetPath, wildcard);
	strcpy(m_nProcessPath, targetPath);
	hFile = _findfirsti64(path, &c_file);
	if (hFile != -1L)
	{
		m_nFindSubCount++;
		char* pFileName;
		if(bFindSubDir) pFileName = getFileName(path);

		do
		{
			if (
				(c_file.attrib & _A_SUBDIR)
				&&(c_file.name[0] != '.')
				)
			{
				c_fileTemp = c_file;
				strcpy(c_fileTemp.name, targetPath);
				strcat(c_fileTemp.name, c_file.name);
				m_vecFinds.push_back(c_fileTemp);
			}


		} while(_findnexti64(hFile, &c_file) == 0);
		_findclose(hFile);
	}

	//하위 폴더 검사
	if(bFindSubDir)
	{
		char* pathSub = path+strlen(targetPath);
		strcpy(pathSub, "*.*");
		hFile = _findfirsti64(path, &c_file);
		if (hFile != -1L)
		{
			do
			{
				if ( c_file.attrib & _A_SUBDIR )
				{
					if (c_file.name[0] != '.')
					{
						strcpy(pathSub, c_file.name);
						strcat(pathSub, "\\");
						FindFolderSubDir(path, wildcard);
						pathSub[0] = 0;
					}

				}


			} while(_findnexti64(hFile, &c_file) == 0);
			_findclose(hFile);
		}
	}



	m_nLastError = errno;
	//m_nLastError = 1;
}



void CakFileUtil::FindFile( char* pTargetPathName, bool bFindSubDir /*= true*/ )
{
	//초기화
	clear();

	//경로 편집
	char path[512]={0};
	_finddatai64_t c_file;
	_finddatai64_t c_fileTemp;
	intptr_t hFile;


	char wildcard[256];char targetPath[512];
	makeAbPathWildcard(pTargetPathName, targetPath, 512, wildcard, 256);


	//현재 폴더에 파일 검사
	strcpy(path, targetPath);
	strcat(path, wildcard);
	//TRACE("%s\n%s\n", targetPath, wildcard);
	strcpy(m_nProcessPath, targetPath);
	hFile = _findfirsti64(path, &c_file);
	if (hFile != -1L)
	{
		m_nFindSubCount++;
		char* pFileName;
		if(bFindSubDir) pFileName = getFileName(path);

		do
		{
			if ((c_file.attrib & _A_SUBDIR) == false )
			{
				c_fileTemp = c_file;
				strcpy(c_fileTemp.name, targetPath);
				strcat(c_fileTemp.name, c_file.name);
				m_vecFinds.push_back(c_fileTemp);

			}


		} while(_findnexti64(hFile, &c_file) == 0);
		_findclose(hFile);
	}

	//하위 폴더 검사
	if(bFindSubDir)
	{
		char* pathSub = path+strlen(targetPath);
		strcpy(pathSub, "*.*");
		hFile = _findfirsti64(path, &c_file);
		if (hFile != -1L)
		{
			do
			{
				if ( c_file.attrib & _A_SUBDIR )
				{
					if (c_file.name[0] != '.')
					{
						strcpy(pathSub, c_file.name);
						strcat(pathSub, "\\");
						FindFileSubDir(path, wildcard);
						pathSub[0] = 0;
					}

				}


			} while(_findnexti64(hFile, &c_file) == 0);
			_findclose(hFile);
		}
	}

	if(0)
	{
		switch (errno)
		{
		case ENOENT:
			printf(":: 파일이 없음 ::\n"); break;
		case EINVAL:
			fprintf(stderr, "Invalid path name.\n"); exit(1); break;
		case ENOMEM:
			fprintf(stderr, "Not enough memory or file name too long.\n"); exit(1); break;
		default:
			fprintf(stderr, "Unknown error.\n"); exit(1); break;
		}

	}

	m_nLastError = errno;
	//m_nLastError = 1;
}




void CakFileUtil::FindFileSubDir( char* pTargetPath, char* pWildcard )
{
	m_nFindSubCount++;

	_finddatai64_t c_file;
	_finddatai64_t c_fileTemp;
	intptr_t hFile;

	char path[512]={0};


	strcpy(path, pTargetPath);
	//strcat(path, "\\");
	strcat(path, pWildcard);


	//파일이 있는지 검사
	hFile = _findfirsti64(path, &c_file);
	if (hFile != -1L)
	{
		do
		{
			if ( (c_file.attrib & _A_SUBDIR) == false)
			{
				c_fileTemp = c_file;
				strcpy(c_fileTemp.name, pTargetPath);
				strcat(c_fileTemp.name, c_file.name);
				m_vecFinds.push_back(c_fileTemp);
			}


		} while(_findnexti64(hFile, &c_file) == 0);
		_findclose(hFile);
	}

	//if(bFindSubDir)
	{
		char* pathSub = path+strlen(pTargetPath);
		strcpy(pathSub, "*.*");
		hFile = _findfirsti64(path, &c_file);
		if (hFile != -1L)
		{
			do
			{
				if ( (c_file.attrib & _A_SUBDIR) && c_file.name[0] != '.')
				{
					strcpy(pathSub, c_file.name);
					strcat(pathSub, "\\");
					FindFileSubDir(path, pWildcard);

					pathSub[0] = 0;
					//_chdir("..");
				}




			} while(_findnexti64(hFile, &c_file) == 0);
			_findclose(hFile);
		}
	}



	m_nLastError = 1;
}


char* CakFileUtil::getFileName(char* pPathFileName )
{
	int nSourceLength = (int)strlen(pPathFileName);
	bool finddot=false;

	int i = nSourceLength-1;
	for(i; i >= 0; i--)
	{
		if(pPathFileName[i] == '.')
		{
			finddot = true;
		}
		if(pPathFileName[i] == '/'
			|| pPathFileName[i] == '\\')
		{
			return pPathFileName+i+1;
		}
	}

	if(finddot)	return pPathFileName;

	return pPathFileName+nSourceLength;
}
char* CakFileUtil::getLastPath( char* pPath )
{
	int nSourceLength = (int)strlen(pPath);

	bool bOk=false;
	int i = nSourceLength-1;
	for(i; i >= 0; i--)
	{
		if(pPath[i] == '/'
			|| pPath[i] == '\\')
		{
			if(bOk == true) return pPath+i+1;
		}
		else
		{
			bOk = true;
		}
	}

	return pPath+nSourceLength;
}

char* CakFileUtil::getFileExt( char* pPathFileName )
{
	int nSourceLength = (int)strlen(pPathFileName);

	int i = nSourceLength-1;
	for(i; i >= 0; i--)
	{
		if(pPathFileName[i] == '.')
		{
			return pPathFileName+i+1;
		}

		if(pPathFileName[i] == '/'
			|| pPathFileName[i] == '\\')
		{
			return &pPathFileName[nSourceLength];
		}
	}

	return pPathFileName;
}

void CakFileUtil::getPath( char* strDest, int nDestSize, const char* strSource )
{
	memset(strDest, 0, sizeof(char)*nDestSize);
	bool finddot=false;

	int nSourceLength = (int)strlen(strSource);
	int i = nSourceLength-1;
	for(i; i >= 0; i--)
	{
		if(strSource[i] == '.')
		{
			finddot = true;
		}
		if(strSource[i] == '/'
			|| strSource[i] == '\\')
		{
			break;
		}
	}


	if(finddot == true) strncpy(strDest, strSource, i+1);
	else strcpy(strDest, strSource);

}

//void CakFileUtil::changeAbsolutePath( char* pPathFileName )
//{
//	char flaguse='\\';
//	char flagnuse='/';
//
//	int i,k, nLen = strlen(pPathFileName);
//	for(i=0; i<nLen; i++)
//	{
//		if(pPathFileName[i] == flagnuse) pPathFileName[i] = flaguse;
//	}
//	int writepoint=0;
//	for(i=0; i<nLen; i++)
//	{
//		// ./ 나오는 경우
//		if(pPathFileName[i] == '.'
//			&& pPathFileName[i+1] == flaguse)
//		{
//			i++;
//		}
//		// .. 나오는 경우
//		else if(pPathFileName[i] == '.'
//			&& pPathFileName[i+1] == '.')
//		{
//			for(k=writepoint-2; k>=0; k--)
//			{
//				if(pPathFileName[k] == flaguse)
//				{
//					writepoint = k+1;
//					i++;
//					break;
//				}
//			}
//		}
//		else
//		{
//			if(pPathFileName[i] == flaguse &&
//				pPathFileName[writepoint-1] == flaguse
//				 )
//				continue;
//
//			pPathFileName[writepoint++] = pPathFileName[i];
//		}
//
//	}
//
//	pPathFileName[writepoint] = 0;
//}


//pPathFileName이 512(버퍼크기)보다 크면 안됨!!
void CakFileUtil::changeAbsolutePath( char* pPathFileName )
{
	char flaguse='\\';
	char flagnuse='/';

	int i,k, nLen = (int)strlen(pPathFileName);
	for(i=nLen-1; i>=0; i--)
	{
		if(pPathFileName[i] == flagnuse) pPathFileName[i] = flaguse;
	}

	char temp[512]={0}; //위험요인!!(pPathFileName보다 큰값이 들어오면 안됨!!
	strcpy(temp, pPathFileName);

	char* pNameTok=NULL;

	if(pPathFileName[0] == '\\') strcpy(pPathFileName, "\\");
	else strcpy(pPathFileName, "");

	bool endflag = false;
	if(pPathFileName[nLen-1] == flaguse) endflag = true;

	pNameTok = strtok( temp, "\\");
	while(pNameTok)
	{
		if(!strcmp(pNameTok, ".."))
		{
			int nLen = (int)strlen(pPathFileName);
			for(k=nLen-2; k>=0; k--)
			{
				if(pPathFileName[k] == flaguse)
				{
					pPathFileName[k+1] = 0;
					break;
				}
			}
			//TRACE("");
		}
		else if(!strcmp(pNameTok, "."))
		{
			//TRACE("");
		}
		else
		{
			strcat( pPathFileName, pNameTok);
			strcat( pPathFileName, "\\");
		}

		pNameTok = strtok( NULL, "\\");
	}
	if(endflag == false)
	{
		pPathFileName[strlen(pPathFileName)-1] = 0;
	}
}


int CakFileUtil::getLastPath( char* strDest, int nDestSize, char* strSource )
{
	int p1,p2;
	p1 = p2 = -1;

	int nLen = (int)strlen(strSource);

	for(int i=nLen-1; i>=0; i--)
	{
		if(p2 == -1 && (strSource[i] == '\\' || strSource[i] == '/') == false )
		{
			p2 = i;
		}

		if(p2 != -1 && (strSource[i] == '\\' || strSource[i] == '/') == true )
		{
			p1 = i+1;
			break;
		}
	}

	memset(strDest, 0, sizeof(nDestSize));
	strncpy(strDest, strSource+p1, p2-p1+1);
	return p1;
}


void CakFileUtil::RenameFile( char* pTargetPathName, char* pNewFileName, bool bFindSubDir /*= true*/ )
{
	FindFile(pTargetPathName, bFindSubDir);
	m_vecProcess.clear();
	int i=0, nSize = (int)m_vecFinds.size();

	_finddatai64_t nameData;
	char* name;
	for(i; i<nSize; i++)
	{
		nameData = m_vecFinds[i];
		name = getFileName(nameData.name);
		strcpy(name, pNewFileName);
		if(rename(m_vecFinds[i].name, nameData.name)>=0)
		{
			m_vecProcess.push_back(nameData);
			//TRACE("%s\n", nameData.name);
		}

	}
}

#include <string>
int ReplaceString( std::string & strCallId, const char * pszBefore, const char * pszAfter )
{
	size_t iPos = strCallId.find( pszBefore );
	size_t iBeforeLen = strlen( pszBefore );

	if( iPos < std::string::npos )
	{
		strCallId.replace( iPos, iBeforeLen, pszAfter );
		//iPos = strCallId.find( pszBefore, iPos );
	}

	return (int)iPos;

	// 	while( iPos < std::string::npos )
	// 	{
	// 		strCallId.replace( iPos, iBeforeLen, pszAfter );
	// 		iPos = strCallId.find( pszBefore, iPos );
	// 	}
}

void CakFileUtil::RenameFile( char* pTargetPathName, char* pFindWord, char* pReplaceWord, bool bFindSubDir /*= true*/ )
{
	FindFile(pTargetPathName, bFindSubDir);
	m_vecProcess.clear();
	int i=0, nSize = (int)m_vecFinds.size();

	std::string str;
	_finddatai64_t nameData;
	char* name;
	for(i; i<nSize; i++)
	{
		nameData = m_vecFinds[i];
		name = getFileName(nameData.name);
		str = name;

		if(ReplaceString(str, pFindWord, pReplaceWord) >= 0)
		{
			strcpy(name, str.c_str());

			//정확하게 된 후에!!
			if(rename(m_vecFinds[i].name, nameData.name) >= 0)
			{
				m_vecProcess.push_back(nameData);
				//TRACE("%s\n", nameData.name);
			}

		}

	}
}

void CakFileUtil::RenameFolder( char* pTargetPath, char* pNewFolderName, bool bFindSubDir /*= true*/ )
{
	FindFolder(pTargetPath, bFindSubDir);
	m_vecProcess.clear();
	int i=0, nSize = (int)m_vecFinds.size();

	_finddatai64_t nameData;
	char* name;
	for(i; i<nSize; i++)
	{
		nameData = m_vecFinds[i];
		name = getLastPath(nameData.name);
		strcpy(name, pNewFolderName);
		strcat(name, "\\");
		if(rename(m_vecFinds[i].name, nameData.name)>=0)
		{
			m_vecProcess.push_back(nameData);
			//TRACE("%s\n", nameData.name);
		}

	}
}

void CakFileUtil::RenameFolder( char* pTargetPath, char* pFindWord, char* pReplaceWord, bool bFindSubDir /*= true*/ )
{
	FindFolder(pTargetPath, bFindSubDir);
	m_vecProcess.clear();
	int i=0, nSize = (int)m_vecFinds.size();

	std::string str;
	_finddatai64_t nameData;
	char* name;
	for(i; i<nSize; i++)
	{
		nameData = m_vecFinds[i];
		name = getLastPath(nameData.name);
		str = name;

		if(ReplaceString(str, pFindWord, pReplaceWord) >= 0)
		{
			strcpy(name, str.c_str());

			//정확하게 된 후에!!
			if(rename(m_vecFinds[i].name, nameData.name) >= 0)
			{
				m_vecProcess.push_back(nameData);
				//TRACE("%s\n", nameData.name);
			}

		}

	}
}

void CakFileUtil::DeleteFile( char* pTargetPathName, bool bFindSubDir /*= true*/ )
{
	FindFile(pTargetPathName, bFindSubDir);

	int i=0, nSize = (int)m_vecFinds.size();


	//char* name;

	for(i; i<nSize; i++)
	{
		if(remove(m_vecFinds[i].name) != -1)
		{
			m_vecProcess.push_back(m_vecFinds[i]);
		}
	}
}

void CakFileUtil::DeleteFolder( char* pTargetPath, bool bFindSubDir /*= true*/ )
{
	_finddatai64_t c_file;
	//_finddatai64_t c_fileTemp;
	intptr_t hFile;

	char path[512]={0};
	char subpath[512]={0};



	//strcat(path, pWildcard);
	{
		sprintf(path, "%s\\*.*", pTargetPath);
		hFile = _findfirsti64(path, &c_file);
		if (hFile != -1L)
		{
			do
			{
				//폴더일경우
				if ( (c_file.attrib & _A_SUBDIR) && c_file.name[0] != '.')
				{
					sprintf(subpath,"%s\\%s",pTargetPath,c_file.name);
					DeleteFolder(subpath, bFindSubDir);

					//if(_rmdir(subpath) != -1)
					{
						//c_fileTemp = c_file;
						//strcpy(c_fileTemp.name, path);

						//m_vecProcess.push_back(c_fileTemp);
					}

				}
				else if((c_file.attrib & _A_SUBDIR) == false)
				{
					sprintf(subpath,"%s\\%s",pTargetPath,c_file.name);

					if(remove(subpath) != -1)
					{
						//c_fileTemp = c_file;
						//strcpy(c_fileTemp.name, path);
						//m_vecProcess.push_back(c_fileTemp);
					}
				}

			} while(_findnexti64(hFile, &c_file) == 0);
			_findclose(hFile);
		}
	}
	sprintf(path, "%s\\", pTargetPath);
	_rmdir(path);

	//return TRUE;
}


int CakFileUtil::CopyFolder( char* strSrc, char* strDest, bool bSubDir )
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult=TRUE;
	char fname[MAX_PATH];
	char WildCard[MAX_PATH];
	char SrcFile[MAX_PATH];
	char DestFile[MAX_PATH];

	strcpy(WildCard, strSrc);
	strcat(WildCard, "\\*.*");
	CreateDirectory(strDest,NULL);
	hSrch=FindFirstFile(WildCard,&wfd);
	if (hSrch == INVALID_HANDLE_VALUE) return FALSE;
	while (bResult)
	{
		sprintf(fname,"%s\\%s",strSrc,wfd.cFileName);
		// 서브 디렉토리가 발견되면 서브 디렉토리를 복사 한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (wfd.cFileName[0] != '.')
			{
				sprintf(SrcFile,"%s\\%s",strSrc,wfd.cFileName);
				sprintf(DestFile,"%s\\%s",strDest,wfd.cFileName);
				CopyFolder(SrcFile, DestFile);
			}
		}
		else
		{
			sprintf(DestFile,"%s\\%s",strDest,wfd.cFileName);
			CopyFile(fname,DestFile,FALSE);
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
	return TRUE;
}


int CakFileUtil::MoveFolder( char* strSrc, char* strDest, bool bSubDir /*= true*/ )
{
	CopyFolder(strSrc, strDest, bSubDir);

	DeleteFolder(strSrc, true);


	return 0;
}

_finddatai64_t* CakFileUtil::getFindData( char* pName )
{
	int nSize = (int)m_vecFinds.size();

	for(int i=0; i<nSize; i++)
	{
		if(m_vecFinds[i].attrib & _A_SUBDIR)
		{
			if(!strcmp(pName, getLastPath(m_vecFinds[i].name)))
			{
				return &m_vecFinds[i];
			}
		}
		else
		{
			if(!strcmp(pName, getFileName(m_vecFinds[i].name)))
			{
				return &m_vecFinds[i];
			}
		}

	}

	return NULL;
}

void CakFileUtil::MakeDirectory( char* pPath )
{
	char temp[256], *sp;
	strcpy(temp, pPath); // 경로문자열을 복사
	sp = temp; // 포인터를 문자열 처음으로
	BOOL bCrate;
	while( (sp = strchr(sp, '\\')) )
	{ // 디렉토리 구분자를 찾았으면
		if(sp > temp && *(sp - 1) != ':')
		{ // 루트디렉토리가 아니면
			*sp = '\0'; // 잠시 문자열 끝으로 설정
			//mkdir(temp, S_IFDIR);
			bCrate = CreateDirectory(temp, NULL);
			// 디렉토리를 만들고 (존재하지 않을 때)
			*sp = '\\'; // 문자열을 원래대로 복귀
		}
		sp++; // 포인터를 다음 문자로 이동
	}
	if(pPath[strlen(pPath)-1] != '\\' || pPath[strlen(pPath)-1] != '/')
	{
		bCrate = CreateDirectory(pPath, NULL);
	}

}

int CakFileUtil::isNetworkPath( char* pPath )
{
	int nCntSeperator=0;
	int nCntDot=0;

	if(pPath[0]=='\\' && pPath[1]=='\\'  == FALSE)
	{
		return -1;
	}
	else
	{
		int nLen = (int)strlen(pPath);
		for(int i=2; i<nLen; i++)
		{
			if(pPath[i] == '/'
				||pPath[i] == '\\') nCntSeperator++;
			else if(pPath[i] == '.') nCntDot++;

			if(nCntDot == 3 && nCntSeperator == 1)
			{
				return i+1;
			}
		}
	}
	return -1;
}

