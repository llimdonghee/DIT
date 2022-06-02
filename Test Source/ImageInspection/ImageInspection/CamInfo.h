#pragma once

#include "Polygon.h"

class CCamInfo
{
public:
	CCamInfo();
	~CCamInfo();

	void	SetInfo(int x, int y, int iScanLength, double fScanResolution, int iForwardDir, double *fFOV, int base_x = 0, int base_y = 0);
	void	GetPos(int &x, int &y);

	BOOL	IsForwardDirectrion()
	{
		if (m_iForwardDir) return TRUE;
		return FALSE;
	}

	void	SetRealPixelFOV_EachChannel(double *fPixelFOV);
	void	SetScanOffset(int iOffset);
	void	SetSpeedRipple(double dRipple);
	BOOL	Img2DDM(IN int ImgPosX, IN int ImgPosY, OUT int &GlsPosX, OUT int &GlsPosY);
	BOOL	DDM2Img(IN int GlsPosX, IN int GlaPosY, OUT int &ImgPosX, OUT int &ImgPosY);
	void	GetXRange(int &fStart, int &fEnd);
	void	GetYRange(int &iStart, int &iEnd);
	BOOL	GetGlassPolygonToImagePolygon(int nGlassPolygon, POINT *ptGlassPolygon, CPolygon &ImagePolygon);
	void	GetAverageFOV(double &fAverageFOV, double &fScanResolution);
	void	GetCamFOV(double &fPixelFOV, double &fScanResolution);
	double	GetAveragePixelFOV();
	void	SetRealPixelFOV(double fPixelFOV);
	void	GetBaseOffset(int &x , int &y);
	int		GetForwardDir(){return m_iForwardDir;};

protected:
	int		m_iForwardDir;
	int		m_iScanLength;
	POINT	m_ptDDMOffset;
	int		m_iScanOffset;

	double  m_dSpeedRipple;
	int		m_iPosEachChannel[4];
	double	m_fFOVEachChannel[4];
	int		m_iStartX;
	int		m_iEndX;
	// HalfScan
	int		m_iStartY;
	int		m_iEndY;
	POINT	m_ptDDMBaseOffset;

	double	m_fScanResolution;
	double	m_fAverageChannelFOV;
	double	m_fRealPixelFOV;
	
	int CAMERA_CCD_SIZE;

};
