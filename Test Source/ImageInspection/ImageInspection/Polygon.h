#pragma once

#include "stdafx.h"


class CPolygon
{
public:
	CPolygon();
	~CPolygon();

	int		SetPolygon(int nVertex, POINT *ptVertex, int iBlockHeight = 10);
	void	ApplyOffset(int iOffset);
	int		GetPolygon(POINT* ptVertex);

	void	ApplyVisionAlign(double *RotateMatrix);

	BOOL	IsInXRange(int MinX, int MaxX);
	BOOL	IsInYRange(int MinY, int MaxY);

	RECT	GetPolygonRect(){return m_rcPolygon;}

	BOOL	GetFirstInspRect16(RECT &rcInsp);
	BOOL	GetNextInspRect16(RECT &rcInsp);
	BOOL	GetFirstInspRect(RECT &rcInsp);
	BOOL	GetNextInspRect(RECT &rcInsp);

	int		ExtractCamArea(int StartPixel, int EndPixel);


	BOOL	DividePolygonBy2Line(int iVertexCnt, POINT *pptVertex, CPoint pt11, CPoint pt12, CPoint pt21, CPoint pt22,int &iOutVertexCnt, POINT *ptOutVertex);

protected:
	int		m_nVertex;
	POINT	m_ptVertex[4];
	RECT	m_rcPolygon;

	double	ArrangeVertexAsCCW();
	double	GetArea();
	BOOL	IsConvexPolygon();

	int		m_x11, m_x12, m_x21, m_x22, m_y1, m_y2;
	int		m_iBlockHeight;

	BOOL	FindInstersection(POINT pt1, POINT pt2, int x1, int x2, int y, OUT int &FindedX);
	int		GetHorizontalIntersection(int y, int &x1, int &x2);

	BOOL	FindInstersection(fLine L1, fLine L2, tag_fPoint &pt, BOOL bIsL1Segment, BOOL bIsL2Segment);
	BOOL	FindInstersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, tag_fPoint &pt, BOOL bIsL1Segment, BOOL bIsL2Segment);
	int		DividePolygon(int iVertexCnt, POINT *pptVertex, CPoint pt1, CPoint pt2, BOOL bIsLineSegment, int *pVertexCnt, POINT *ptOutVertex, int iMaxSectionCnt);
};