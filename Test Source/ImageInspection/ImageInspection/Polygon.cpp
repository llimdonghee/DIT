#include "stdafx.h"
#include "Polygon.h"
#include "math.h"

#define ROUND_ERROR		0.001



CPolygon::CPolygon()
{
	m_nVertex = 0;
	ZeroMemory(m_ptVertex, sizeof(POINT)*MAX_VERTEX);
}

CPolygon::~CPolygon()
{
}


double CPolygon::ArrangeVertexAsCCW()
{
	int x, y;
	int i;
	double area = GetArea();
	if (area > 0)
	{
		for (i=0; i<m_nVertex/2; i++)
		{
			x = m_ptVertex[i].x;
			y = m_ptVertex[i].y;
			
			m_ptVertex[i].x = m_ptVertex[m_nVertex-i-1].x;
			m_ptVertex[i].y = m_ptVertex[m_nVertex-i-1].y;

			m_ptVertex[m_nVertex-i-1].x = x;
			m_ptVertex[m_nVertex-i-1].y = y;
		}
	}
	return area;
}

double CPolygon::GetArea()
{
	int i;
	double area = 0;
	for (i=0; i<m_nVertex-1; i++) area += ((static_cast<double>(m_ptVertex[i].x)*static_cast<double>(m_ptVertex[i+1].y))-(static_cast<double>(m_ptVertex[i].y)*static_cast<double>(m_ptVertex[i+1].x)));
	area += ((static_cast<double>(m_ptVertex[i].x)*static_cast<double>(m_ptVertex[0].y))-(static_cast<double>(m_ptVertex[i].y)*static_cast<double>(m_ptVertex[0].x)));
	area /= 2;
	return area;
}

BOOL CPolygon::IsConvexPolygon()
{
	int i;
	int BottomLeftPoint = 0;
	CPoint ptTemp;
	double fTemp;

	double v1x, v1y;
	double v2x, v2y;

	CString str;

	if (m_nVertex == 3) return TRUE;

	for (i=0; i<m_nVertex; i++)
	{
		if (i == 0)
		{
			v1x = static_cast<double>(m_ptVertex[m_nVertex-1].x - m_ptVertex[i].x);
			v1y = static_cast<double>(m_ptVertex[m_nVertex-1].y - m_ptVertex[i].y);
			v2x = static_cast<double>(m_ptVertex[i+1].x - m_ptVertex[i].x);
			v2y = static_cast<double>(m_ptVertex[i+1].y - m_ptVertex[i].y);	
		}
		else if (i == m_nVertex-1)
		{
			v1x = static_cast<double>(m_ptVertex[i-1].x - m_ptVertex[i].x);
			v1y = static_cast<double>(m_ptVertex[i-1].y - m_ptVertex[i].y);
			v2x = static_cast<double>(m_ptVertex[0].x - m_ptVertex[i].x);
			v2y = static_cast<double>(m_ptVertex[0].y - m_ptVertex[i].y);
		}
		else
		{
			v1x = static_cast<double>(m_ptVertex[i-1].x - m_ptVertex[i].x);
			v1y = static_cast<double>(m_ptVertex[i-1].y - m_ptVertex[i].y);
			v2x = static_cast<double>(m_ptVertex[i+1].x - m_ptVertex[i].x);
			v2y = static_cast<double>(m_ptVertex[i+1].y - m_ptVertex[i].y);			
		}


		fTemp = (v1x*v2y - v1y*v2x);
		if (fTemp < 0) return FALSE;
	}
	return TRUE;
}


int CPolygon::SetPolygon(int nVertex, POINT *ptVertex, int iBlockHeight)
{
	int i;
	m_nVertex = nVertex;
	CopyMemory(m_ptVertex, ptVertex, sizeof(POINT)*m_nVertex);

	m_rcPolygon.left = m_rcPolygon.right = m_ptVertex[0].x;
	m_rcPolygon.top = m_rcPolygon.bottom = m_ptVertex[0].y;

	for (i=1; i<m_nVertex; ++i)
	{
		if (m_rcPolygon.left > m_ptVertex[i].x) m_rcPolygon.left = m_ptVertex[i].x;
		if (m_rcPolygon.right < m_ptVertex[i].x) m_rcPolygon.right = m_ptVertex[i].x;

		if (m_rcPolygon.top > m_ptVertex[i].y) m_rcPolygon.top = m_ptVertex[i].y;
		if (m_rcPolygon.bottom < m_ptVertex[i].y) m_rcPolygon.bottom = m_ptVertex[i].y;
	}
	m_iBlockHeight = iBlockHeight;
	return ERR_NONE;
}

void CPolygon::ApplyOffset(int iOffset)
{
	
}

int CPolygon::GetPolygon(POINT *ptVertex)
{
	CopyMemory(ptVertex, m_ptVertex, sizeof(POINT)*m_nVertex);
	return m_nVertex;
}

void CPolygon::ApplyVisionAlign(double *RotateMatrix)
{
	int i;
	int x, y;

	for (i=0; i<m_nVertex; i++)
	{
		x = static_cast<int>(m_ptVertex[i].x * RotateMatrix[0] + m_ptVertex[i].y * RotateMatrix[1] + RotateMatrix[2]);
		y = static_cast<int>(m_ptVertex[i].x * RotateMatrix[3] + m_ptVertex[i].y * RotateMatrix[4] + RotateMatrix[5]);
		m_ptVertex[i].x = x;
		m_ptVertex[i].y = y;

		if (i==0)
		{
			m_rcPolygon.left = m_rcPolygon.right = m_ptVertex[0].x;
			m_rcPolygon.top = m_rcPolygon.bottom = m_ptVertex[0].y;
		}
		else
		{
			if (m_rcPolygon.left > m_ptVertex[i].x) m_rcPolygon.left = m_ptVertex[i].x;
			if (m_rcPolygon.right < m_ptVertex[i].x) m_rcPolygon.right = m_ptVertex[i].x;

			if (m_rcPolygon.top > m_ptVertex[i].y) m_rcPolygon.top = m_ptVertex[i].y;
			if (m_rcPolygon.bottom < m_ptVertex[i].y) m_rcPolygon.bottom = m_ptVertex[i].y;
		}
	}
}

BOOL CPolygon::IsInXRange(int MinX, int MaxX)
{
	if (m_rcPolygon.left >= MaxX) return FALSE;
	if (m_rcPolygon.right <= MinX) return FALSE;
	return TRUE;
}

BOOL CPolygon::IsInYRange(int MinY, int MaxY)
{
	if (m_rcPolygon.top >= MaxY) return FALSE;
	if (m_rcPolygon.bottom <= MinY) return FALSE;
	return TRUE;
}

BOOL CPolygon::FindInstersection(POINT pt1, POINT pt2, int x1, int x2, int y, OUT int &FindedX)
{
	double fTemp_X, fTemp_Y ;

	double p1, p2, p3 ;
	double c1, c2 ;

	if (max(pt1.x, pt2.x) < min(x1, x2))
		return FALSE ;

	if (max(pt1.y, pt2.y) < y)
		return FALSE ;

	if (max(x1, x2) < min(pt1.x, pt2.x))
		return FALSE ;

	if (y < min(pt1.y, pt2.y))
		return FALSE ;

	double z1, z2, z3, z4 ;

	z1 = (double)(((x1 - pt1.x) * (pt2.y - pt1.y)) - ((y - pt1.y) * (pt2.x - pt1.x))) ;
	z2 = (double)(((x2 - pt1.x) * (pt2.y - pt1.y)) - ((y - pt1.y) * (pt2.x - pt1.x))) ;
	z3 = (double)(-(pt1.y - y) * (x2 - x1)) ;
	z4 = (double)(-(pt2.y - y) * (x2 - x1)) ;

	if (((z1 * z2) <= 0) && ((z3 * z4) > 0))
		return FALSE ;

	double Vx, Vy ;

	Vx = pt2.x - pt1.x ;
	Vy = pt2.y - pt1.y ;

	if ((y == pt1.y) && (y == pt2.y)) {
		if ((pt1.x >= min(x1, x2)) && (pt1.x <= max(x1, x2))) {
			FindedX = pt1.x ;

			return TRUE ;
		}
		else {
			return FALSE ;
		}
	}

	p1 = pt1.y - pt2.y ;
	p2 = pt2.x - pt1.x ;
	p3 = x2 - x1 ;

	c1 = (p1 * pt1.x) + (p2 * pt1.y) ;
	c2 = p3 * y ;

	if ((p1 * p3) == 0)
		return FALSE ;

	//fTemp = 1 / (p1 * p3) * ((p3 * c1) - (p2 * c2)) ; // 2011.05.26
	fTemp_X = ((p3 * c1) - (p2 * c2)) / (p1 * p3) ;

	//fTempY = 1 / (p1 * p3) * (p1 * c2) ; // 2011.05.26
	fTemp_Y = (p1 * c2) / (p1 * p3) ;

	if (fabs(fTemp_Y - pt2.y) < ROUND_ERROR) {
		if (fabs(fTemp_X - pt2.x) < ROUND_ERROR)
			return FALSE ;
	}

	FindedX = static_cast<int>(fTemp_X + 0.5) ;

	return TRUE ;
}

int CPolygon::ExtractCamArea(int StartPixel, int EndPixel)
{
	int nOutVertex;
	POINT ptOutVertex[4];
	if (DividePolygonBy2Line(m_nVertex, m_ptVertex, CPoint(StartPixel, 0), CPoint(StartPixel, 10), CPoint(EndPixel, 0), CPoint(EndPixel, 10), nOutVertex, ptOutVertex))
	{
		SetPolygon(nOutVertex, ptOutVertex);
	}
	return ERR_NONE;
}


int CPolygon::GetHorizontalIntersection(int y, int &x1, int &x2)
{
	int i;
	int iInterSectionCnt = 0;

	int x[3];

	for (i=0; i<m_nVertex-1; ++i)
	{
		if (!FindInstersection(m_ptVertex[i], m_ptVertex[i+1], m_rcPolygon.left, m_rcPolygon.right, y, x[iInterSectionCnt])) continue;
		iInterSectionCnt ++;
		if (iInterSectionCnt == 3) return 3;
	}
	if (FindInstersection(m_ptVertex[i], m_ptVertex[0], m_rcPolygon.left, m_rcPolygon.right, y, x[iInterSectionCnt]))
	{
		iInterSectionCnt ++;
		if (iInterSectionCnt == 3) return 3;
	}

	if (x[0] < x[1]) {x1 = x[0];	x2 = x[1];}
	else {x1 = x[1];	x2 = x[0];}

	return iInterSectionCnt;
}

BOOL CPolygon::GetFirstInspRect16(RECT &rcInsp)
{
	if (!GetFirstInspRect(rcInsp)) return FALSE;

	if (rcInsp.left & 0x0F) rcInsp.left = (rcInsp.left & ~(0x0F)) + 16;
	else rcInsp.left = rcInsp.left & ~(0x0F);

	if (rcInsp.right & 0x0F) rcInsp.right = (rcInsp.right & ~(0x0F)) - 16;
	else rcInsp.right = rcInsp.right & ~(0x0F);

	if (rcInsp.left >= rcInsp.right) return FALSE;
	if (rcInsp.top >= rcInsp.bottom) return FALSE;
	return TRUE;
}

BOOL CPolygon::GetNextInspRect16(RECT &rcInsp)
{
	if (!GetNextInspRect(rcInsp)) return FALSE;

	if (rcInsp.left & 0x0F) rcInsp.left = (rcInsp.left & ~(0x0F)) + 16;
	else rcInsp.left = rcInsp.left & ~(0x0F);
	
	if (rcInsp.right & 0x0F) rcInsp.right = (rcInsp.right & ~(0x0F)) - 16;
	else rcInsp.right = rcInsp.right & ~(0x0F);

	if (rcInsp.left >= rcInsp.right) return FALSE;
	if (rcInsp.top >= rcInsp.bottom) return FALSE;

	return TRUE;
}


BOOL CPolygon::GetFirstInspRect(RECT &rcInsp)
{
	m_y1 = m_rcPolygon.top;
	
	if (GetHorizontalIntersection(m_y1, m_x11, m_x12) != 2)
	{
		m_y1 += m_iBlockHeight;
		if (GetHorizontalIntersection(m_y1, m_x11, m_x12) != 2) return FALSE;
	}

	m_y2 = m_y1 + m_iBlockHeight < m_rcPolygon.bottom ? m_y1 + m_iBlockHeight : m_rcPolygon.bottom ;
	if (GetHorizontalIntersection(m_y2, m_x21, m_x22) != 2) return FALSE;

	rcInsp.top = m_y1;
	rcInsp.bottom = m_y2;

	rcInsp.left = max(m_x11, m_x21);
	rcInsp.right = min(m_x12, m_x22);

	if (rcInsp.left >= rcInsp.right) return FALSE;
	if (rcInsp.top >= rcInsp.bottom) return FALSE;
	return TRUE;
}

BOOL CPolygon::GetNextInspRect(RECT &rcInsp)
{
	if (m_y2 == m_rcPolygon.bottom) return FALSE;
	m_y1 = m_y2;
	m_x11 = m_x21;
	m_x12 = m_x22;
	m_y2 = m_y2 + m_iBlockHeight < m_rcPolygon.bottom ? m_y2 + m_iBlockHeight : m_rcPolygon.bottom;

	if (GetHorizontalIntersection(m_y2, m_x21, m_x22) != 2) return FALSE;

	rcInsp.top = m_y1;
	rcInsp.bottom = m_y2;

	rcInsp.left = max(m_x11, m_x21);
	rcInsp.right = min(m_x12, m_x22);

	if (rcInsp.left >= rcInsp.right) return FALSE;
	if (rcInsp.top >= rcInsp.bottom) return FALSE;

	return TRUE;
}

BOOL CPolygon::FindInstersection(fLine L1, fLine L2, tag_fPoint &pt, BOOL bIsL1Segment, BOOL bIsL2Segment)
{
	double a, b, c, d;
	double c1, c2;

	a = -(L1.p2.y - L1.p1.y);
	b = (L1.p2.x - L1.p1.x);

	c = -(L2.p2.y - L2.p1.y);
	d = (L2.p2.x - L2.p1.x);

	c1 = a * L1.p1.x + b * L1.p1.y;
	c2 = c * L2.p1.x + d * L2.p1.y;

	if (a*d-b*c == 0) return FALSE;
	pt.x = 1/(a*d-b*c)*(d*c1-b*c2);
	pt.y = 1/(a*d-b*c)*(-c*c1+a*c2);

	if (bIsL1Segment)
	{
		if (min(L1.p1.x, L1.p2.x)-ROUND_ERROR > pt.x) return FALSE;
		if (max(L1.p1.x, L1.p2.x)+ROUND_ERROR < pt.x) return FALSE;
		if (min(L1.p1.y, L1.p2.y)-ROUND_ERROR > pt.y) return FALSE;
		if (max(L1.p1.y, L1.p2.y)+ROUND_ERROR < pt.y) return FALSE;
	}
	if (bIsL2Segment)
	{
		if (min(L2.p1.x, L2.p2.x)-ROUND_ERROR > pt.x) return FALSE;
		if (max(L2.p1.x, L2.p2.x)+ROUND_ERROR < pt.x) return FALSE;
		if (min(L2.p1.y, L2.p2.y)-ROUND_ERROR > pt.y) return FALSE;
		if (max(L2.p1.y, L2.p2.y)+ROUND_ERROR < pt.y) return FALSE;
	}
	return TRUE;
}



BOOL CPolygon::FindInstersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, tag_fPoint &pt, BOOL bIsL1Segment, BOOL bIsL2Segment)
{
	fLine L1, L2;
	L1.p1.x = x1;
	L1.p1.y = y1;
	L1.p2.x = x2;
	L1.p2.y = y2;

	L2.p1.x = x3;
	L2.p1.y = y3;
	L2.p2.x = x4;
	L2.p2.y = y4;

	return FindInstersection(L1, L2, pt, bIsL1Segment, bIsL2Segment);
}

int  CPolygon::DividePolygon(int iVertexCnt, POINT *pptVertex, CPoint pt1, CPoint pt2, BOOL bIsLineSegment, int *pVertexCnt, POINT *ptOutVertex, int iMaxSectionCnt)
{
	int i, j ;
	int iFindPt ;

	tag_fPoint FindedPt[8] ;

	int iNextPtS[8] ;

	tag_fPoint pt ;

	fLine line[MAX_VERTEX] ;

	BOOL bInclude[MAX_VERTEX] ;

	fLine Segment ;

	double Mx, mx, My, my ;

	tag_fPoint ptTemp[MAX_VERTEX] ;

	int iVertexStorage = 0 ;
	int CurPosition = 0 ;

	BOOL bCurIsSepratePt = FALSE ;
	BOOL bPreIsSepratePt = FALSE ;

	ZeroMemory(ptTemp, sizeof(tag_fPoint) * MAX_VERTEX) ;

	Mx = mx = pptVertex[0].x ;
	My = my = pptVertex[0].y ;

	for (i = 0; i < (iVertexCnt - 1); i++) {
		if (Mx < pptVertex[i].x) Mx = pptVertex[i].x ;
		if (mx > pptVertex[i].x) mx = pptVertex[i].x ;
		if (My < pptVertex[i].y) My = pptVertex[i].y ;
		if (my > pptVertex[i].y) my = pptVertex[i].y ;

		line[i].p1.x = pptVertex[i].x ;
		line[i].p1.y = pptVertex[i].y ;

		line[i].p2.x = pptVertex[i+1].x ;
		line[i].p2.y = pptVertex[i+1].y ;
	}

	if (Mx < pptVertex[i].x) Mx = pptVertex[i].x ;
	if (mx > pptVertex[i].x) mx = pptVertex[i].x ;
	if (My < pptVertex[i].y) My = pptVertex[i].y ;
	if (my > pptVertex[i].y) my = pptVertex[i].y ;

	line[i].p1.x = pptVertex[i].x ;
	line[i].p1.y = pptVertex[i].y ;

	line[i].p2.x = pptVertex[0].x ;
	line[i].p2.y = pptVertex[0].y ;

	/*/ 2011.05.26
	if (!bIsLineSegment) {
		mx -= (Mx-mx)*0.1;
		Mx += (Mx-mx)*0.1;
		my -= (My-my)*0.1;
		My += (My-my)*0.1;
	}
	//*/

	//*/
	double xGap, yGap ;

	if (!bIsLineSegment) {
		xGap = (Mx - mx) * 0.1 ;
		yGap = (My - my) * 0.1 ;

		mx -= xGap ;
		Mx += xGap ;
		my -= yGap ;
		My += yGap ;
	}
	//*/

	iFindPt = 0 ;

	if (FindInstersection(pt1.x, pt1.y, pt2.x, pt2.y, mx, my, Mx, my, pt, bIsLineSegment, TRUE)) {
		FindedPt[iFindPt].x = pt.x ;
		FindedPt[iFindPt++].y = pt.y ;
	}

	if (FindInstersection(pt1.x, pt1.y, pt2.x, pt2.y, mx, my, mx, My, pt, bIsLineSegment, TRUE)) {
		FindedPt[iFindPt].x = pt.x ;
		FindedPt[iFindPt++].y = pt.y ;
	}

	if (FindInstersection(pt1.x, pt1.y, pt2.x, pt2.y, Mx, My, Mx, my, pt, bIsLineSegment, TRUE)) {
		FindedPt[iFindPt].x = pt.x ;
		FindedPt[iFindPt++].y = pt.y ;
	}

	if (FindInstersection(pt1.x, pt1.y, pt2.x, pt2.y, Mx, My, mx, My, pt, bIsLineSegment, TRUE)) {
		FindedPt[iFindPt].x = pt.x ;
		FindedPt[iFindPt++].y = pt.y ;
	}

	if (iFindPt < 2) {
		memcpy(ptOutVertex, pptVertex, sizeof(CPoint) * iVertexCnt) ;
		pVertexCnt[0] = iVertexCnt ;

		return 1 ;
	}

	if (iFindPt > 2) {
		for (i = 0; i < iFindPt; i++) {
			for (j = (i + 1); j < iFindPt; j++) {
				if (fabs(FindedPt[i].x - FindedPt[j].x) > ROUND_ERROR)
					continue ;

				if (fabs(FindedPt[i].y - FindedPt[j].y) > ROUND_ERROR)
					continue ;

				if (j == (iFindPt - 1)) {
					iFindPt -- ;
				}
				else {
					iFindPt -- ;
					//memcpy(&(FindedPt[j]), &(FindedPt[j+1]), sizeof(tag_fPoint)*iFindPt-j) ; // 2011.05.26
					memcpy(&(FindedPt[j]), &(FindedPt[j+1]), sizeof(tag_fPoint) * (iFindPt-j)) ;
				}
			}
		}
	}

	Segment.p1.x = FindedPt[0].x ;
	Segment.p1.y = FindedPt[0].y ;

	Segment.p2.x = FindedPt[1].x ;
	Segment.p2.y = FindedPt[1].y ;

	iFindPt = 0 ;

	for (i = 0; i < iVertexCnt; i++) {
		if (!FindInstersection(line[i], Segment, pt, TRUE, TRUE))
			continue ;

		FindedPt[iFindPt].x = pt.x ;
		FindedPt[iFindPt].y = pt.y ;

		iNextPtS[iFindPt] = i + 1 ;
		iFindPt ++ ;

		if (iFindPt >= 8)
			return 0 ; // ??
	}

	if (iFindPt == 0) {
		memcpy(ptOutVertex, pptVertex, sizeof(CPoint) * iVertexCnt) ;
		pVertexCnt[0] = iVertexCnt ;

		return 1 ;
	}

	for (i = 0; i < (iFindPt - 1); i++) {
		for (j = (i + 1); j < iFindPt; j++) {
			if ((((Segment.p1.x - FindedPt[i].x) * (Segment.p1.x - FindedPt[i].x)) + ((Segment.p1.y - FindedPt[i].y) * (Segment.p1.y - FindedPt[i].y))) >
				(((Segment.p1.x - FindedPt[j].x) * (Segment.p1.x - FindedPt[j].x)) + ((Segment.p1.y - FindedPt[j].y) * (Segment.p1.y - FindedPt[j].y)))) {
				Swap(FindedPt[i].x, FindedPt[j].x) ;
				Swap(FindedPt[i].y, FindedPt[j].y) ;
				Swap(iNextPtS[i], iNextPtS[j]) ;
			}
		}
	}

	do {
		bCurIsSepratePt = FALSE ;

		for (i = 0; i < (iFindPt - 1); i++) {
			if (fabs(FindedPt[i].x - FindedPt[i+1].x) > ROUND_ERROR)
				continue ;

			if (fabs(FindedPt[i].y - FindedPt[i+1].y) > ROUND_ERROR)
				continue ;

			iFindPt -- ;

			if (i != iFindPt) {
				memcpy(&(FindedPt[i]), &(FindedPt[i+1]), sizeof(tag_fPoint) * (iFindPt-i)) ;
				memcpy(&(iNextPtS[i]), &(iNextPtS[i+1]), sizeof(int) * (iFindPt-i)) ;
			}			
		}
	} while (bCurIsSepratePt) ;


	for (i = 0; i < iVertexCnt; i++)
		bInclude[i] = FALSE ;

	int iAreaCnt = (iFindPt / 2) + 1 ;
	int iTotalVertexCnt = 0 ;

	for (int k = 0; k < iAreaCnt; k++) {
		CurPosition = -1 ;

		for (i = 0; i < iVertexCnt; i++) {
			if (bInclude[i] == TRUE)
				continue ;

			CurPosition = i ;

			bCurIsSepratePt = FALSE ;
			bPreIsSepratePt = FALSE ;

			break ;
		}

		if (CurPosition == -1)
			break ;

		iVertexStorage = 0 ;

		while (1) {
			if (bCurIsSepratePt) {
				ptTemp[iVertexStorage].x = FindedPt[CurPosition].x ;
				ptTemp[iVertexStorage].y = FindedPt[CurPosition].y ;

				iVertexStorage ++ ;

				if (bPreIsSepratePt) {
					bCurIsSepratePt = FALSE ;
					CurPosition = iNextPtS[CurPosition] ;
				}
				else {
					if ((CurPosition % 2) == 0)
						CurPosition ++ ;
					else
						CurPosition -- ;

					bPreIsSepratePt = TRUE ;
				}
			}
			else {
				bPreIsSepratePt = FALSE ;

				CurPosition = CurPosition % iVertexCnt ;

				if ((pptVertex[CurPosition].x == ptTemp[0].x) && (pptVertex[CurPosition].y == ptTemp[0].y))
					break ;

				ptTemp[iVertexStorage].x = pptVertex[CurPosition].x ;
				ptTemp[iVertexStorage].y = pptVertex[CurPosition].y ;

				bInclude[CurPosition] = TRUE ;

				iVertexStorage ++ ;

				CurPosition ++ ;

				for (i = 0; i < iFindPt; i++) {
					if (iNextPtS[i] == CurPosition) {
						CurPosition = i ;

						bCurIsSepratePt = TRUE ;
						bPreIsSepratePt = FALSE ;
						break ;
					}
				}
			}

			if (iVertexStorage > 1) {
				if ((fabs(ptTemp[iVertexStorage-1].x - ptTemp[iVertexStorage-2].x) < ROUND_ERROR) &&
					(fabs(ptTemp[iVertexStorage-1].y - ptTemp[iVertexStorage-2].y) < ROUND_ERROR))
					iVertexStorage -- ;
			}

			if (iVertexStorage == MAX_VERTEX)
				break ;
		}

		for (i = 0; i < iVertexStorage; i++) {
			ptOutVertex[iTotalVertexCnt].x = static_cast<int>(ptTemp[i].x) ;
			ptOutVertex[iTotalVertexCnt].y = static_cast<int>(ptTemp[i].y) ;

			iTotalVertexCnt ++ ;
		}

		pVertexCnt[k] = iVertexStorage ;

		if (k == (iMaxSectionCnt - 1))
			break ;
	}

	return iAreaCnt ;
}

BOOL CPolygon::DividePolygonBy2Line(int iVertexCnt, POINT *pptVertex, CPoint pt11, CPoint pt12, CPoint pt21, CPoint pt22,int &iOutVertexCnt, POINT *ptOutVertex)
{
	int i;

	int iVertexCntTemp[3] ;
	CPoint pVertexTemp[4] ;

	int iVertexCntTemp1 ;
	CPoint pVertexTemp1[MAX_VERTEX] ;

	int iVertexCntTemp2 ;
	CPoint pVertexTemp2[MAX_VERTEX] ;

	int iCnt1, iCnt2 ;

	if (pt11.x == pt12.x) {
		if (pt11.x > pt21.x) {
			Swap(pt11, pt21) ;
			Swap(pt12, pt22) ;
		}

		iCnt1 = DividePolygon(iVertexCnt, pptVertex, pt11, pt12, FALSE, iVertexCntTemp, pVertexTemp, 3) ;

		if (iCnt1 == 2) {
			iVertexCntTemp1 = iVertexCntTemp[0] ;
			iVertexCntTemp2 = iVertexCntTemp[1] ;

			memcpy(pVertexTemp1, pVertexTemp, sizeof(CPoint) * iVertexCntTemp1) ;
			memcpy(pVertexTemp2, &(pVertexTemp[iVertexCntTemp1]), sizeof(CPoint) * iVertexCntTemp2) ;

			for (i = 0; i < iVertexCntTemp1; i++) {
				if (pVertexTemp1[i].x < (pt11.x - 1)) {
					iVertexCntTemp1 = iVertexCntTemp2 ;
					memcpy(pVertexTemp1, pVertexTemp2, sizeof(CPoint) * iVertexCntTemp2) ;
					break ;
				}
			}
		}
		else {
			iVertexCntTemp1 = iVertexCnt ;
			memcpy(pVertexTemp1, pptVertex, sizeof(CPoint)*iVertexCnt) ;
		}

		iCnt2 = DividePolygon(iVertexCntTemp1, pVertexTemp1, pt21, pt22, FALSE, iVertexCntTemp, pVertexTemp, 3) ;

		if (iCnt2 == 2) {
			iVertexCntTemp1 = iVertexCntTemp[0] ;
			iVertexCntTemp2 = iVertexCntTemp[1] ;

			memcpy(pVertexTemp1, pVertexTemp, sizeof(CPoint) * iVertexCntTemp1) ;
			memcpy(pVertexTemp2, &(pVertexTemp[iVertexCntTemp1]), sizeof(CPoint) * iVertexCntTemp2) ;

			for (i = 0; i < iVertexCntTemp1; i++) {
				if (pVertexTemp1[i].x > (pt21.x + 1)) {
					iVertexCntTemp1 = iVertexCntTemp2 ;
					memcpy(pVertexTemp1, pVertexTemp2, sizeof(CPoint)*iVertexCntTemp2) ;
					break ;
				}
			}
		}
	}
	else {
		if (pt11.y > pt21.y) {
			Swap(pt11, pt21) ;
			Swap(pt12, pt22) ;
		}

		iCnt1 = DividePolygon(iVertexCnt, pptVertex, pt11, pt12, FALSE, iVertexCntTemp, pVertexTemp, 3) ;

		if (iCnt1 == 2) {
			iVertexCntTemp1 = iVertexCntTemp[0] ;
			iVertexCntTemp2 = iVertexCntTemp[1] ;

			memcpy(pVertexTemp1, pVertexTemp, sizeof(CPoint) * iVertexCntTemp1) ;
			memcpy(pVertexTemp2, &(pVertexTemp[iVertexCntTemp1]), sizeof(CPoint) * iVertexCntTemp2) ;

			for (i = 0; i < iVertexCntTemp1; i++) {
				if (pVertexTemp1[i].y < (pt11.y - 1)) {
					iVertexCntTemp1 = iVertexCntTemp2 ;
					memcpy(pVertexTemp1, pVertexTemp2, sizeof(CPoint) * iVertexCntTemp2) ;
					break ;
				}
			}
		}
		else {
			iVertexCntTemp1 = iVertexCnt ;
			memcpy(pVertexTemp1, pptVertex, sizeof(CPoint) * iVertexCnt) ;
		}

		iCnt2 = DividePolygon(iVertexCntTemp1, pVertexTemp1, pt21, pt22, FALSE, iVertexCntTemp, pVertexTemp, 3) ;

		if (iCnt2 == 2) {
			iVertexCntTemp1 = iVertexCntTemp[0] ;
			iVertexCntTemp2 = iVertexCntTemp[1] ;

			memcpy(pVertexTemp1, pVertexTemp, sizeof(CPoint) * iVertexCntTemp1) ;
			memcpy(pVertexTemp2, &(pVertexTemp[iVertexCntTemp1]), sizeof(CPoint) * iVertexCntTemp2) ;

			for (i = 0; i < iVertexCntTemp1; i++) {
				if (pVertexTemp1[i].y > (pt21.y + 1)) {
					iVertexCntTemp1 = iVertexCntTemp2 ;
					memcpy(pVertexTemp1, pVertexTemp2, sizeof(CPoint)*iVertexCntTemp2) ;
					break ;
				}
			}
		}
	}

	if (iCnt1 == 1 && iCnt2 == 1) {
		int min, max ;

		if (pt11.x == pt12.x) {
			min = max = pptVertex[0].x ;

			for (i = 1; i < iVertexCnt; ++i) {
				if (min > pptVertex[i].x)
					min = pptVertex[i].x ;

				if (max < pptVertex[i].x)
					max = pptVertex[i].x ;
			}

			if (min >= pt11.x && max <= pt21.x) {
				iOutVertexCnt = iVertexCntTemp1 ;
				memcpy(ptOutVertex, pVertexTemp1, sizeof(CPoint) * iVertexCntTemp1) ;

				return TRUE ;
			}
		}
		else {
			min = max = pptVertex[0].y ;

			for (i = 1; i < iVertexCnt; ++i) {
				if (min > pptVertex[i].y)
					min = pptVertex[i].y ;

				if (max < pptVertex[i].y)
					max = pptVertex[i].y ;
			}

			if (min >= pt11.y && max <= pt21.y) {
				iOutVertexCnt = iVertexCntTemp1 ;
				memcpy(ptOutVertex, pVertexTemp1, sizeof(CPoint) * iVertexCntTemp1) ;

				return TRUE ;				
			}
		}

		iOutVertexCnt = 0 ;

		return FALSE ;
	}
	else {
		iOutVertexCnt = iVertexCntTemp1 ;
		memcpy(ptOutVertex, pVertexTemp1, sizeof(CPoint) * iVertexCntTemp1) ;
	}
	
	return TRUE ;
}