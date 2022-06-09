//LogCtrl.h
//LogCtrl.h
#ifndef __LOGCTRL_H
#define __LOGCTRL_H
#define __LOGCTRL_LIB_H

#ifndef __AFXTEMPL_H
#include <afxtempl.h>
#endif

//LOG CELL STATE
//MERGE
#define LCS_NORML		0x0000
#define LCS_MERGE		0x0001
#define LCS_UNUSE		0x0002

//LINE
#define LCS_TLINE		0x0001	//TOP LINE DRAW
#define LCS_BLINE		0x0002	//BOTTOM LINE DRAW
#define LCS_LLINE		0x0004	//LEFT LINE DRAW
#define LCS_RLINE		0x0008	//RIGHT LINE DRAW
#define LCS_RECT		LCS_TLINE|LCS_BLINE|LCS_LLINE|LCS_RLINE

//merge state
#define SetFlag(n)  DWORD(BYTE(n)<<16)
#define GetFlag(d)  int((d & 0x00FF0000)>>16)

#define SetDown(n)  DWORD(BYTE(n)<<8)
#define GetDown(d)  int((d & 0x0000FF00)>>8)

#define SetRight(n) DWORD(BYTE(n))
#define GetRight(d) int((d & 0x000000FF))

#define LOGCHECK(b) if(b){return;}
#define LOGCHECK0(b) if(b){return 0;}
#define LOGCHECK1(b) if(b){return -1;}

class CPrintDC;
class CLogCell;
class CLogPage;
class CLogCtrl;

typedef CArray<CLogCell,CLogCell&> CLogItem;
typedef CArray<CLogItem,CLogItem&> CLogItemArray;
typedef CArray<CLogPage,CLogPage&> CLogPageArray;

typedef struct tagLOGCELL
{
	BYTE		m_nLine; //�����¿� ���� 
	UINT		m_nState;//�ٸ����� ���յƴ��� ..
	UINT		m_nFormat;//���ĸ��
	RECT		m_rect;  //���� ����
	COLORREF	m_txtclr;//���ڻ�
	COLORREF	m_bkgclr;//����
	CString		m_strTxt;//����
	CFont*		m_pfont; //��Ʈ
	CBitmap*	m_pBitmap;//��Ʈ��
}LOGCELL;

//Orientation=����(1), ����(2)  bDialog=DoModal()
class CPrintDC
{
public:
	CPrintDC();
	~CPrintDC();
	CDC* GetPrintDC(int Orientation, BOOL bDialog=FALSE);
protected:
	CDC *m_pDC;
};

class CLogCell : public tagLOGCELL
{
public:
	CLogCell();
	CLogCell(LPCSTR lpstr,
			CRect	rect,
			int nFormat=DT_CENTER,
			COLORREF textColor=0x00000000,
			COLORREF backColor=0x00ffffff,
			CFont* pf=NULL);

	void operator=(const CLogCell& log);
	void InitCell();
	void Draw(CDC* pDC);
};

class CLogPage 
{
public:
	CLogPage();
	CLogPage(CPoint startPnt);
	~CLogPage();
	void RemoveAll();
	void operator=(CLogPage& page);

	//��,�� ������ �����Ѵ�.
	void SetColCount(int nCount);
	void SetRowCount(int nCount);
	BOOL RemoveRow(int nRow);

	//��ǥ �����ġ
	void SetStartPoint(CPoint startPnt);
	CPoint GetStartPoint() const;

	//��,�� ������ �����Ѵ�.
	int GetColCount() const;
	int GetRowCount() const;

	//��,���̸� �����Ѵ�.
	BOOL SetColWidth(int nCol, int nWidth);
	BOOL SetRowHeight(int nRow, int nHeight);

	//��Ʈ�� �����Ѵ�.
	BOOL SetCellFont(int nRow, int nCol, CFont* pf);
	BOOL SetCellFont(int nsRow, int nsCol, int neRow, int neCol, CFont* pf);
	CFont* GetCellFont(int nRow, int nCol);

	//������ �����Ѵ�.
	BOOL SetCellBkgColor(int nRow, int nCol, COLORREF color);
	COLORREF GetCellBkgColor(int nRow, int nCol);

	//���ڻ��� �����Ѵ�.
	BOOL SetCellTxtColor(int nRow, int nCol, COLORREF color);
	COLORREF GetCellTxtColor(int nRow, int nCol);

	//������ġ ����
	BOOL SetCellFormat(int nRow, int nCol, int nFormat);
	int GetCellFormat(int nRow, int nCol);

	//Rect�� �����Ѵ�.
	BOOL SetCellRect(int nRow, int nCol, CRect rect);
	BOOL GetCellRect(int nRow, int nCol, CRect& rect);

	//BITMAP�� �����Ѵ�.
	BOOL SetCellBmp(int nRow, int nCol, CBitmap* pBmp);
	CBitmap* GetCellBmp(int nRow, int nCol);

	//�簢�� �׸��� ��� ��ġ�� �׸� ���ΰ� �����Ѵ�. 
	BOOL SetCellLine(int nRow, int nCol, int nState=LCS_RECT/*LCS_RECT*/);
	int GetCellLine(int nRow, int nCol);

	//���� ������ �Է��Ѵ�.
	BOOL SetItemText(int nRow, int nCol, LPCTSTR lpstr, int nFormat=DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	LPCSTR GetItemText(int nRow, int nCol);

	BOOL SetCell(int nRow, int nCol, CLogCell lc);
	CLogCell* GetCell(int nRow, int nCol);

	//���� �����Ѵ�.
	BOOL CellMerge(int nsCol, int nsRow, int neCol ,int neRow );

	//����Ѵ�.(nPageNO�� ������ ��ȣ�̸� 0�� ��� ��µ��� �ʴ´�.)
	void Draw(CDC* pDC, int nPageNO=0);

	//SetPageRect(,)�Լ��� ���� 
	//Draw(CDC* pDC, int nPageNO=0);�Լ��� ȣ���ϱ� �ٷ����� ����ؾ���
	//�뵵: 
	//   <<page>>                                 <<device>>
	//  0                               100      0                               1000  
	//  ����������������������������������        ����������������������������������
	//  ��  (10,10)              (90,10)��        �� (100,100)           (900,100)��
	//  ��  ��������������������������  ��        ��  ��������������������������  ��
	//  ��  ��                      ��  ��        ��  ��                      ��  ��
	//  ��  ��                      �� ��������������>��                      ��  ��
	//  ��  ��                      ��  ��        ��  ��                      ��  ��
	//  ��  ��                      ��  ��        ��  ��                      ��  ��
	//  ��  ��������������������������  ��        ��  ��������������������������  ��
	//  ��  (10,40)              (90,40)��        �� (100,400)           (900,400)��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ��                              ��        ��                              ��
	//  ����������������������������������        ����������������������������������
	//  200                            100,200    2000                            1000,2000 
	//  
	//   CRect page(0,0,100,200);
	//   CRect device(0,0,1000,2000);
	//   ������ ���� �ַ���: CRect device(100,100,900,1900);
	//   SetPageRect(pDC, page, device);
	BOOL SetPageRect(CDC* pDC, CRect page, CRect device);
protected:
	bool m_IsViewPort;
	int m_RowCnt;
	int m_ColCnt;
	CPoint		  m_startPnt;
	CLogItemArray m_liAry;
};

//��ǥ ��ü�� �����Ѵ�.
//��ǥ�� ����ϱ� ���� �� �ʿ����� �ʴ�.
//CLogPage�� ����ص� �����ϴ�.
class CLogCtrl
{
public:
	CLogCtrl();
	~CLogCtrl();
	int  GetLogPageCount();
	void AddLogPage(CLogPage& lp);
	CLogPage* GetLogPage(int nPage);
	void DelLogPage(int nPage);
	void Draw(CDC* pDC, DOCINFO* pDocInfo);
protected:
	CLogPageArray m_log;
};

#endif
