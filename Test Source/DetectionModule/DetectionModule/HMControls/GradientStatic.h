#pragma once


// CGradientStatic
typedef UINT (CALLBACK* LPFNDLLFUNC1)(HDC,CONST PTRIVERTEX,DWORD,CONST PVOID,DWORD,DWORD);

class CDoubleBuff : public CDC {
private:	
	CBitmap		m_bitmap;		// Offscreen bitmap
	CBitmap*	m_oldBitmap;	// bitmap originally found in CDoubleBuff
	CDC*		m_pDC;			// Saves CDC passed in constructor
	CRect		m_rect;			// Rectangle of drawing area.
	BOOL		m_bMemDC;		// TRUE if CDC really is a Memory DC.
public:

	CDoubleBuff(CDC* pDC, const CRect* pRect = NULL) : CDC()
	{
		ASSERT(pDC != NULL); 

		// Some initialization
		m_pDC = pDC;
		m_oldBitmap = NULL;
		m_bMemDC = !pDC->IsPrinting();

		// Get the rectangle to draw
		if (pRect == NULL) {
			pDC->GetClipBox(&m_rect);
		} else {
			m_rect = *pRect;
		}

		if (m_bMemDC) {
			// Create a Memory DC
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);

			SetMapMode(pDC->GetMapMode());

			SetWindowExt(pDC->GetWindowExt());
			SetViewportExt(pDC->GetViewportExt());

			pDC->DPtoLP(&m_rect);
			SetWindowOrg(m_rect.left, m_rect.top);
		} else {
			// Make a copy of the relevent parts of the current DC for printing
			m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}

		// Fill background 
		FillSolidRect(m_rect, pDC->GetBkColor());
	}

	~CDoubleBuff()	
	{		
		if (m_bMemDC) {
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);			

			//Swap back the original bitmap.
			SelectObject(m_oldBitmap);
		} else {
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.			
			m_hDC = m_hAttribDC = NULL;
		}	
	}

	// Allow usage as a pointer	
	CDoubleBuff* operator->() 
	{
		return this;
	}	

	// Allow usage as a pointer	
	operator CDoubleBuff*() 
	{
		return this;
	}

	CDC *GetOriginalDC()
	{
		return m_pDC;
	}
};

class AFX_EXT_CLASS CGradientStatic : public CStatic
{
	DECLARE_DYNAMIC(CGradientStatic)

public:
	enum	EN_ANI_MODE{EN_ANI_NONE=0,EN_ANI_LEFTRIGHT,EN_ANI_LAMP};

	CGradientStatic();
	virtual ~CGradientStatic();
	void SetWindowText(LPCTSTR a_lpstr);
	void SetColor(long cl) {clLeft=cl;};
	BOOL SetControlFont(int nHeight, int nWidth, int nEscapement,
		int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
		BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
		BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
		LPCTSTR lpszFacename);
	void SetGradientColor(long cl) {clRight=cl; SetDrawColor(clLeft, clRight); };
	void SetTextColor(long cl) {clText=cl;};
	void SetReverseGradient();
	void SetLeftSpacing(int iNoOfPixels) { m_iLeftSpacing = iNoOfPixels; };
	void SetTextAlign(int iAlign ) { m_iAlign = iAlign; }; //0 - left, 1 - center, 2 -right
	void SetVerticalGradient(BOOL a_bVertical = TRUE) { m_bVertical = a_bVertical; };
	void SetOulineColor(long cl1, long cl2) { clOut1 = cl1; clOut2 = cl2; m_bDrawOutLine = TRUE; }
	void SetDrawColor(long cl1, long cl2);
	void MoveDrawColor();
	void SetAnimationMode(int nMode, int nTime, int nIndex);
	static void DrawGradRect(CDC *pDC, CRect r, COLORREF cLeft, COLORREF cRight, BOOL a_bVertical);
	void DrawGradRect(CDC *pDC, CRect r, BOOL a_bVertical, COLORREF *cl);

protected:
	CString m_sTEXT;
	int m_iLeftSpacing;
	long clLeft;
	long clRight;
	long clText;
	long clOut1;
	long clOut2;
	COLORREF clDrawColor[510];
	int m_iAlign;
	BOOL m_bDrawOutLine;
	EN_ANI_MODE m_enAnimation;
	int m_nAniColorCount;
	HINSTANCE hinst_msimg32;
	BOOL m_bCanDoGradientFill;
	BOOL m_bVertical;
	LPFNDLLFUNC1 dllfunc_GradientFill;

	CFont m_font;

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
};


