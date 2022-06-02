#pragma once

#include "Polygon.h"

class CROI
{
public:
	CROI();
	~CROI();

	enum INSP_TYPE {
		ROI_INSP = 0,
		ROI_SAVE_CONCURRENT = 1,
		ROI_SAVE_PAD = 2,
		ROI_SAVE_GRAY = 3
	};

	void		SetType(INSP_TYPE type, int iIndex = -1);
	
	INSP_TYPE	GetType(){return m_iInspType;}
	int			GetIndex(){return m_iIndex;}

	int			SetROI(int nVertex, POINT *pVertex, int iOffset = 0);
	int			GetROI(POINT *pVertex);

	void		ApplyVisionAlign(double *RotateMatrix);

	BOOL		IsIncludeInXPos(int MinX, int MaxX) {return m_ROI.IsInXRange(MinX, MaxX);}
	BOOL		IsIncludeInYPos(int MinY, int MaxY) {return m_ROI.IsInYRange(MinY, MaxY);}


protected:
	CPolygon	m_ROI;
	INSP_TYPE	m_iInspType;
	int			m_iIndex;
};