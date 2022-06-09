
#pragma once


class CakMemDC : public CDC
{
public:

	CakMemDC(CDC* pDC, CRect rectDC) : CDC()
	{
		ASSERT(pDC != NULL);

		m_pDC = pDC;
		m_pOldBitmap = NULL;
		m_bMemDC = false;
		m_hDC = NULL;
		if(pDC == NULL)
		{
			CreateCompatibleDC(pDC);
			return;
		}

#ifndef WCE_NO_PRINTING
		m_bMemDC = !pDC->IsPrinting();
#else
		m_bMemDC = FALSE;
#endif

		if (m_bMemDC)    
		{
			//pDC->GetClipBox(&m_rect);
			m_rect = rectDC;
			CreateCompatibleDC(pDC);
			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_pOldBitmap = SelectObject(&m_bitmap);
#ifndef _WIN32_WCE
			//SetWindowOrg(m_rect.left, m_rect.top);
			SetWindowOrg(0,0);
#endif
			FillSolidRect(m_rect, pDC->GetBkColor());
		}
		else   
		{
#ifndef WCE_NO_PRINTING
			m_bPrinting = pDC->m_bPrinting;
#endif
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}

	}

	virtual ~CakMemDC()
	{
		if (m_bMemDC)
		{
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom,
				this, m_rect.left, m_rect.top, SRCCOPY);
			

			SelectObject(m_pOldBitmap);
			//DeleteDC(); 
			//m_bitmap.DeleteObject();
			
		}
		else
		{
			m_hDC = m_hAttribDC = NULL;
		}
	}

	CakMemDC* operator->()
	{
		return this;
	}

	operator CakMemDC*()
	{
		return this;
	}

private:
	CBitmap  m_bitmap;      // Offscreen bitmap
	CBitmap* m_pOldBitmap;  // bitmap originally found in CakMemDC
	CDC*     m_pDC;         // Saves CDC passed in constructor
	BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
public:
	CRect    m_rect;        // Rectangle of drawing area.
}
;
