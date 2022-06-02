#include "stdafx.h"
#include "ROI.h"

CROI::CROI()
{

}

CROI::~CROI()
{

}

void CROI::SetType(INSP_TYPE type, int iIndex)
{
	m_iInspType = type;
	m_iIndex = iIndex;
}

int CROI::SetROI(int nVertex, POINT *pVertex, int iOffset)
{
	int res = m_ROI.SetPolygon(nVertex, pVertex);
	if (res != ERR_NONE) return res;
	m_ROI.ApplyOffset(iOffset);
	return res;
}

int CROI::GetROI(POINT *pVertex)
{
	return m_ROI.GetPolygon(pVertex);
}

void CROI::ApplyVisionAlign(double *RotateMatrix)
{
	m_ROI.ApplyVisionAlign(RotateMatrix);
}
