// HMFolderScheduling.cpp: implementation of the CHMFolderScheduling class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HMFolderScheduling.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHMFolderScheduling::CHMFolderScheduling()
{
	m_strMainPath = _T("");

	m_bTargetFolder = FALSE;
	m_bTargetFile = FALSE;

	//m_pPathInfo = NULL;
}

CHMFolderScheduling::CHMFolderScheduling(CString strPath)
{
	m_strMainPath = strPath;
	
	m_bTargetFolder = FALSE;
	m_bTargetFile = FALSE;

	//m_pPathInfo = NULL;
}

CHMFolderScheduling::CHMFolderScheduling(CString strPath, int nTargetType)
{
	m_strMainPath = strPath;
	
	if (nTargetType | TARGETTYPE_FOLDER)
		m_bTargetFolder = TRUE;
	else
		m_bTargetFolder = FALSE;

	if (nTargetType | TARGETTYPE_FILE)
		m_bTargetFile = TRUE;
	else
		m_bTargetFile = FALSE;
	
	//m_pPathInfo = NULL;
}

CHMFolderScheduling::~CHMFolderScheduling()
{
// 	if (m_pPathInfo)
// 	{
// 		delete [] m_pPathInfo;
// 		m_pPathInfo = NULL;
// 	}

	m_vecPathInfo.clear();
}

void CHMFolderScheduling::ResetSchedule()
{
	m_strMainPath.Empty();

	m_vecPathInfo.clear();
// 	if (m_pPathInfo)
// 	{
// 		delete [] m_pPathInfo;
// 		m_pPathInfo = NULL;
// 	}
// 	m_nSubItemCount = 0;

	m_bTargetFolder = FALSE;
	m_bTargetFile = FALSE;
	m_nRemainItemCount = 0;
}

BOOL CHMFolderScheduling::SetRemainDelete(CString strPath, int nTargetType, int nRemainItemCount)
{
	m_strMainPath = strPath;

	if (nTargetType & TARGETTYPE_FOLDER)
		m_bTargetFolder = TRUE;
	else
		m_bTargetFolder = FALSE;

	if (nTargetType & TARGETTYPE_FILE)
		m_bTargetFile = TRUE;
	else
		m_bTargetFile = FALSE;

	m_nRemainItemCount = nRemainItemCount;
	
	return TRUE;
}

BOOL CHMFolderScheduling::CommitSchedule()
{
	// ���� ����ŭ ����� �������� �����.
	try
	{
		// �׸��� �����ͼ�...
		SearchSubItem(m_strMainPath, FALSE);

		// �����.
// 		if (m_nRemainItemCount <= m_nSubItemCount)
// 		{
// 			for (int i = m_nRemainItemCount; i < m_nSubItemCount; i++)
// 			{
// 				if (m_pPathInfo[i].s_nItemType == TARGETTYPE_FILE)
// 					CFile::Remove(m_pPathInfo[i].s_strItemPath);
// 				else if (m_pPathInfo[i].s_nItemType == TARGETTYPE_FOLDER)
// 					DeleteFolder(m_pPathInfo[i].s_strItemPath);
// 				m_pPathInfo[i].Reset();
// 			}
// 		}
		if (m_nRemainItemCount <= m_vecPathInfo.size())
		{
			for (int i = m_nRemainItemCount; i < m_vecPathInfo.size(); i++)
			{
				if (m_vecPathInfo[i].s_nItemType == TARGETTYPE_FILE)
					CFile::Remove(m_vecPathInfo[i].s_strItemPath);
				else if (m_vecPathInfo[i].s_nItemType == TARGETTYPE_FOLDER)
					DeleteFolder(m_vecPathInfo[i].s_strItemPath);
				m_vecPathInfo[i].Reset();
			}
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Utility Function
BOOL CHMFolderScheduling::SearchSubItem(const CString &strMainPath, BOOL bIsAscending)
{
	// ���� ����Ʈ ����.
// 	if (m_pPathInfo)
// 	{
// 		delete [] m_pPathInfo;
// 		m_pPathInfo = NULL;
// 	}
	m_vecPathInfo.clear();

	if (strMainPath.IsEmpty())
		return FALSE;

	CString strPath;
	strPath = strMainPath;
	if(strPath.Right(1) != "\\") 
		strPath += "\\";
	
	CString strFindPath = strPath + "*.*";
	CString strDirName;
	CFileFind filefind;
	BOOL bContinue;	
	if(!(bContinue = filefind.FindFile(strFindPath)))
		return FALSE;

	
	//m_pPathInfo = new SSubPathInfo[m_nSubItemCount];
	int nCount = 0;
	CTime TMFileCreate;

	SSubPathInfo PathInfoTemp;
	while (bContinue)
	{
		bContinue = filefind.FindNextFile();
		if (filefind.IsDots())
			continue;

		//���丮�̸�
		if(m_bTargetFolder && filefind.IsDirectory())
		{
			strDirName = filefind.GetFileName();
			if (filefind.GetLastWriteTime(TMFileCreate))
			{
				PathInfoTemp.s_strItemPath = strPath + strDirName;
				PathInfoTemp.s_strItemName = strDirName;
				PathInfoTemp.s_nCreateDate = (long)TMFileCreate.GetTime();
				PathInfoTemp.s_nItemType = TARGETTYPE_FOLDER;
				nCount++;
				m_vecPathInfo.push_back(PathInfoTemp);
			}
		}
		if (m_bTargetFile && !filefind.IsDirectory())
		{
			strDirName = filefind.GetFileName();
			if (filefind.GetLastWriteTime(TMFileCreate))
			{
				PathInfoTemp.s_strItemPath = strPath + strDirName;
				PathInfoTemp.s_strItemName = strDirName;
				PathInfoTemp.s_nCreateDate = (long)TMFileCreate.GetTime();
				PathInfoTemp.s_nItemType = TARGETTYPE_FILE;
				nCount++;
				m_vecPathInfo.push_back(PathInfoTemp);
			}
		}
	}

	if(!SortSubItem(bIsAscending))		//Sub������ ��¥�� �°� ����
		return FALSE;

	filefind.Close();

	return TRUE;
}

BOOL CHMFolderScheduling::SortSubItem(BOOL bIsAscending)
{
	int i,j;

	// ������ �� �ֱ� ��¥��
	int nSubItemCount = m_vecPathInfo.size();
	SSubPathInfo SubPathInfo;
	for(i = 0; i < nSubItemCount; i++)
	{
		for(j = 0; j < nSubItemCount - i - 1 ; j++)
		{
			// ��������
			if (bIsAscending && m_vecPathInfo[j].s_nCreateDate > m_vecPathInfo[j + 1].s_nCreateDate)
			{
				SubPathInfo = m_vecPathInfo[j + 1];
				m_vecPathInfo[j + 1] = m_vecPathInfo[j];
				m_vecPathInfo[j] = SubPathInfo;
			}
			// ��������
			if (!bIsAscending && m_vecPathInfo[j].s_nCreateDate < m_vecPathInfo[j + 1].s_nCreateDate)
			{
				SubPathInfo = m_vecPathInfo[j + 1];
				m_vecPathInfo[j + 1] = m_vecPathInfo[j];
				m_vecPathInfo[j] = SubPathInfo;
			}
		}
	}

	return TRUE;
}

BOOL CHMFolderScheduling::DeleteFolder(const CString &strFolder)
{
	SHFILEOPSTRUCT FileOp = {0};
	char szTemp[MAX_PATH];

	strcpy_s(szTemp, MAX_PATH, strFolder);
	szTemp[strFolder.GetLength() + 1] = NULL; // NULL���ڰ� �ΰ� ���� �Ѵ�.

	FileOp.hwnd = NULL;
	FileOp.wFunc = FO_DELETE;
	FileOp.pFrom = NULL;
	FileOp.pTo = NULL;
	FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI; // Ȯ�θ޽����� �ȶߵ��� ����
	FileOp.fAnyOperationsAborted = false;
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = szTemp;

	SHFileOperation(&FileOp);

	return true;
}
