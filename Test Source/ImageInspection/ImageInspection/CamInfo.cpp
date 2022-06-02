#include "stdafx.h"
#include "CamInfo.h"


CCamInfo::CCamInfo()
{
	m_dSpeedRipple = 0.0;

	CAMERA_CCD_SIZE = MAX_CAMERA_CCD_SIZE;


}
CCamInfo::~CCamInfo()
{

}

void CCamInfo::SetInfo(int x, int y, int iScanLength, double fScanResolution, int iForwardDir, double *fFOV, int base_x, int base_y)
{
	int i ;

	double fHoleFOV = 0. ;

	m_ptDDMOffset.x = x ;
	m_ptDDMOffset.y = y ;

	// Half Scan
	m_ptDDMBaseOffset.x = base_x ;
	m_ptDDMBaseOffset.y = base_y ;

	CopyMemory(m_fFOVEachChannel, fFOV, sizeof(double) * 4) ;

	m_fScanResolution = fScanResolution ;
	m_iScanLength = iScanLength ;
	m_iForwardDir = iForwardDir ;

	for (i = 0; i < 4; i++) {
		fHoleFOV += m_fFOVEachChannel[i] ;
	}

	m_fAverageChannelFOV = fHoleFOV / 4. ;
	m_fRealPixelFOV = m_fAverageChannelFOV ;

	int CCD_8 = MAX_CAMERA_CCD_SIZE / 8 ;
	fHoleFOV = fHoleFOV * CCD_8 ;

	m_iPosEachChannel[0] = static_cast<int>(m_ptDDMOffset.x - fHoleFOV + 0.5) ;

	int CCD_4 = MAX_CAMERA_CCD_SIZE / 4 ;

	for (i = 0; i < 3; i++) {
		m_iPosEachChannel[i+1] = m_iPosEachChannel[i] + static_cast<int>(CCD_4 * m_fFOVEachChannel[i]) ;
	}
	m_iStartX = m_iPosEachChannel[0] ;
	m_iEndX = m_iPosEachChannel[3] + static_cast<int>(m_fFOVEachChannel[3] * (CCD_4 - 1) + 0.5) ;

	m_iStartY = base_y + m_ptDDMOffset.y ;
	m_iEndY	= m_iStartY + iScanLength ;

	m_iScanOffset = 0 ;
}

void CCamInfo::GetPos(int &x, int &y)
{
	x = m_ptDDMOffset.x ;
	y = m_ptDDMOffset.y ;
}

void CCamInfo::SetScanOffset(int iOffset)
{
	m_iScanOffset = iOffset ;
}

void CCamInfo::SetSpeedRipple(double dRipple)
{
	m_dSpeedRipple = dRipple;
}

BOOL CCamInfo::Img2DDM(IN int ImgPosX, IN int ImgPosY, OUT int &GlsPosX, OUT int &GlsPosY)
{
	/*
	if (ImgPosX < 0) ImgPosX = 0;
	if (ImgPosX > 8192) ImgPosX = 8192;

	int iChannel = static_cast<int>(ImgPosX/2048);
	if (iChannel >= 4) iChannel=3;
	GlsPosX = static_cast<int>(m_iPosEachChannel[iChannel]+m_fFOVEachChannel[iChannel]*(ImgPosX-iChannel*2048)+0.5);

	if (m_iForwardDir)
		GlsPosY = static_cast<int>(ImgPosY*m_fScanResolution-m_iScanOffset+0.5);
	else
		GlsPosY = m_iScanLength - static_cast<int>(ImgPosY*m_fScanResolution-m_iScanOffset+0.5);
	*/

	if (ImgPosX < 0) {
		ImgPosX = 0 ;
	}

	if (ImgPosX >= CAMERA_CCD_SIZE) {
		ImgPosX = CAMERA_CCD_SIZE - 1 ;
	}

	int CCD_4 = CAMERA_CCD_SIZE / 4 ;
	//int iChannel = static_cast<int>(ImgPosX/2048) ;
	int iCH = static_cast<int>(ImgPosX / CCD_4) ;

	if (iCH > 3) {
		iCH = 3 ;
	}

	for (int ch = 0; ch < CHANNEL_COUNT; ch++) {
		if (m_fFOVEachChannel[ch] < 0.) {
			m_fFOVEachChannel[ch] = m_fAverageChannelFOV ;
		}
	}

	//GlsPosX = static_cast<int>(m_iPosEachChannel[iChannel]+m_fFOVEachChannel[iChannel]*(ImgPosX-iChannel*2048)+0.5);
	GlsPosX = static_cast<int>(m_iPosEachChannel[iCH] + (m_fFOVEachChannel[iCH] * (ImgPosX - (iCH * CCD_4))) + 0.5) ;

	if (m_iForwardDir) { 
		GlsPosY = m_ptDDMBaseOffset.y + static_cast<int>((ImgPosY * (m_fScanResolution + (m_fScanResolution * m_dSpeedRipple))) - m_iScanOffset + 0.5) ;
	}
	else {
		GlsPosY = m_ptDDMBaseOffset.y + m_iScanLength - static_cast<int>((ImgPosY * (m_fScanResolution + (m_fScanResolution * m_dSpeedRipple))) - m_iScanOffset + 0.5) ;
	}

	return TRUE ;
}

BOOL CCamInfo::DDM2Img(IN int GlsPosX, IN int GlsPosY, OUT int &ImgPosX, OUT int &ImgPosY)
{
	/*int i;

	if (!m_iForwardDir) GlsPosY = m_iScanLength - GlsPosY;
	ImgPosY = static_cast<int>((GlsPosY + m_iScanOffset) / m_fScanResolution + 0.5);

	for (i=0; i<3; i++)
	{
		if (m_iPosEachChannel[i+1] < GlsPosX) break;
	}
	ImgPosX = 2048*i+static_cast<int>((GlsPosX-m_iPosEachChannel[i])/m_fFOVEachChannel[i]+0.5);

	if (m_iStartX > GlsPosX) return FALSE;
	if (m_iEndX < GlsPosX) return FALSE;

	return TRUE;*/
	
	// Half Scan 
	int i ;

	int GlassPosY = GlsPosY - m_ptDDMBaseOffset.y ;

	if (!m_iForwardDir) {
		GlassPosY = m_iScanLength - GlassPosY ;
	}

	if (GlassPosY < 0) {
		GlassPosY = 0 ;
	}

	ImgPosY = static_cast<int>((GlassPosY + m_iScanOffset) / (m_fScanResolution + m_fScanResolution * m_dSpeedRipple) + 0.5) ;
	
	int CCD_4 = CAMERA_CCD_SIZE / 4 ;

	for (i = 0; i < 3; i++) {
		if (m_iPosEachChannel[i+1] < GlsPosX) {
			break ;
		}
	}
	//ImgPosX = 2048*i+static_cast<int>((GlsPosX-m_iPosEachChannel[i])/m_fFOVEachChannel[i]+0.5);
	ImgPosX = CCD_4 * i + static_cast<int>((GlsPosX - m_iPosEachChannel[i]) / m_fFOVEachChannel[i] + 0.5) ;

	
	if (m_iStartX > GlsPosX) {
	return FALSE ;
	}

	if (m_iEndX < GlsPosX) {
		return FALSE ;
	}

	return TRUE ;
}

void CCamInfo::GetXRange(int &iStart, int &iEnd)
{
	iStart = m_iStartX;
	iEnd = m_iEndX;
}

void CCamInfo::GetYRange(int &iStart, int &iEnd)
{
	iStart = m_iStartY;
	iEnd = m_iEndY;
}

BOOL CCamInfo::GetGlassPolygonToImagePolygon(int nGlassPolygon, POINT *ptGlassPolygon, CPolygon &ImagePolygon)
{
	int i;
	int ImgX, ImgY;

	for (i=0; i<nGlassPolygon; i++)
	{
		DDM2Img(ptGlassPolygon[i].x, ptGlassPolygon[i].y, ImgX, ImgY);
		ptGlassPolygon[i].x = ImgX;
		ptGlassPolygon[i].y = ImgY;
	}
	if (nGlassPolygon > 2)
	{
		ImagePolygon.SetPolygon(nGlassPolygon, ptGlassPolygon);

//		ImagePolygon.ExtractCamArea(0, CAMERA_CCD_SIZE) ;
		
		return TRUE;
	}

	return FALSE ;
}

void CCamInfo::GetAverageFOV(double &fAverageFOV, double &fScanResolution)
{
	fAverageFOV = m_fAverageChannelFOV ;
	fScanResolution = m_fScanResolution ;
}

double CCamInfo::GetAveragePixelFOV()
{
	return m_fAverageChannelFOV;
}

void CCamInfo::SetRealPixelFOV(double fPixelFOV)
{
	m_fRealPixelFOV = fPixelFOV ;
}

void CCamInfo::SetRealPixelFOV_EachChannel(double *fPixelFOV)
{
	memcpy(m_fFOVEachChannel, fPixelFOV, sizeof(double)*CHANNEL_COUNT);
}

void CCamInfo::GetCamFOV(double &fPixelFOV, double &fScanResolution)
{
	fPixelFOV = m_fRealPixelFOV ; 
	fScanResolution = m_fScanResolution ;
}

void CCamInfo::GetBaseOffset(int &x , int &y)
{
	x = m_ptDDMBaseOffset.x;
	y = m_ptDDMBaseOffset.y;
}
