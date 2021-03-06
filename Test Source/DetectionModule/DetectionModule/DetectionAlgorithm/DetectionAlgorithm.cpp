#include "stdafx.h"
#include "DetectionAlgorithm.h"
#include "math.h"

#define ALG_DEBUG	1

void  CreateFolder(CString strPath);
void CreateFolder(CString strPath)
{
	int pos,len ;

	CString str = "" ;

	if (strPath.Right(1) != _T('\\')) { 
		strPath +='\\' ;
	}

	len = strPath.GetLength () ;
	pos = strPath.Find ('\\') ;

	do {
		pos = strPath.Find ('\\', pos+1) ;
		str = strPath.Left(pos) ;
		::CreateDirectory (str, NULL) ;

	} while (len!=str.GetLength ()+1) ;
}

CParam::CParam()
{
	m_bEnableInspection = TRUE;
	m_bMultiThreshold = FALSE;
	m_iMultiTHGray = 100;

	m_iVDist = m_iHDist = 50;
	m_iThresholdLow = 12, m_iThresholdHigh = 15, m_iThresholdLow2 = 12, m_iThresholdHigh2 = 15;
	m_iThresholdOffsetLow = 0, m_iThresholdOffsetHigh = 0, m_iThresholdOffsetLow2 = 0, m_iThresholdOffsetHigh2 = 0;
	m_iMinSize = 1, m_iMaxSize = 9999;
	m_iMergeDist = 10;
	m_bHoriCompare = FALSE;
	m_bDiagonalCompare = FALSE;
	m_bCrossCompare = FALSE;
	m_bAVACompare = FALSE;
	m_bSlopeFilter = FALSE;
	m_iSlopeFactor = 10;
	m_bGrayFilter = FALSE;
	m_bCellOffsetZero = FALSE;
	m_bBubbleInspection = FALSE;
	m_bEnableZone = FALSE;
	m_bNonPeriodicInspection = FALSE;
	m_bEnableMultiHorPitch = FALSE;
	m_iMultiHorPitch = 0;
	m_bEnableMultiVerPitch = FALSE;
	m_iMultiVerPitch = 0;

	for(int i = 0; i < MAX_ZONE_CNT; i++)
	{
		m_iZoneThresholdLow[i] = 12;
		m_iZoneThresholdHigh[i] = 15;
		m_iZoneThresholdLow2[i] = 12;
		m_iZoneThresholdHigh2[i] = 15;
		m_iZoneMinSize[i] = 1;
	}

	m_iImgSizeX = 50;
	m_iImgSizeY = 50;
	m_iZonePixelSizeX_um = 50;
	m_iZonePixelSizeY_um = 50;
	m_strPatImgName = "";
	m_strZoneImgName = "";

	m_rtROI = CRect(0, 0, MAX_CAMERA_CCD_SIZE, MAX_FRAME_HEIGHT);

	m_fScanResolution = m_fRealFOV = m_fFOV = 1.0;
	m_fPixelsize = 100;

	m_bAdjacentSlopeFilter = FALSE;
	m_iEdgeJudgment = 100;
	m_iEdgeSlopeTh = 15;
	m_bPeriodSlopeFilter = FALSE;
	m_iPeriodSlopeJudgment = 10;
	m_iPeriodSlopeTh = 15;
	m_bTrackingFilter = FALSE;
	m_iTrackingRange = 10;
	m_iTrackingTh = 12;
	m_bNearbyCheckFilter = FALSE;
	m_bMountainFilter = FALSE;

	CreateFolder("C:\\0_TEST\\DetectionModule");

	LoadParam(DETECT_PARAM_PATH);
}

CParam::~CParam()
{
}

BOOL CParam::LoadParam(CString strPath)
{
	CFileFind ff;
	CIni ini(strPath) ;

	if(ff.FindFile(strPath) == FALSE)
	{
		SaveParam(strPath);
	}
	else
	{
		int i = 0, iPosL = 0, iPosH = 0, iPosL2 = 0, iPosH2 = 0, iPosMin = 0, iPosPADComp = 0;
		CString strZoneThL, strZoneThH, strZoneThL2, strZoneThH2, strZoneMin, strPADComp, strTmp;
		char* pszZoneThL = NULL, *pszZoneThH = NULL, *pszZoneThL2 = NULL, *pszZoneThH2 = NULL, *pszZoneMin = NULL, *pszPADComp = NULL;

		m_bMultiThreshold = ini.GetBool("MultiThresholdEnable", 1, "CELL_INSPECTION");
		m_iVDist = ini.GetInt("DistV", 50, "CELL_INSPECTION");
		m_iHDist = ini.GetInt("DistH", 50, "CELL_INSPECTION");
		m_iThresholdLow = ini.GetInt("ThresholdL", 12, "CELL_INSPECTION");
		m_iThresholdHigh = ini.GetInt("ThresholdH", 15, "CELL_INSPECTION");
		m_iThresholdLow2 = ini.GetInt("ThresholdL2", 12, "CELL_INSPECTION");
		m_iThresholdHigh2 = ini.GetInt("ThresholdH2", 15, "CELL_INSPECTION");
		m_iThresholdOffsetLow = ini.GetInt("ThresholdOffsetL", 0, "CELL_INSPECTION");
		m_iThresholdOffsetHigh = ini.GetInt("ThresholdOffsetH", 0, "CELL_INSPECTION");
		m_iThresholdOffsetLow2 = ini.GetInt("ThresholdOffsetL2", 0, "CELL_INSPECTION");
		m_iThresholdOffsetHigh2 = ini.GetInt("ThresholdOffsetH2", 0, "CELL_INSPECTION");
		m_iMergeDist = ini.GetInt("MergeDist", 10, "CELL_INSPECTION");
		m_iMinSize = ini.GetInt("MinSize", 1, "CELL_INSPECTION");
		m_iMaxSize = ini.GetInt("MaxSize", 9999, "CELL_INSPECTION");
		m_bHoriCompare = ini.GetBool("EnableHorComp", 0, "CELL_INSPECTION");
		m_bDiagonalCompare = ini.GetBool("EnableDiagonal", 0, "CELL_INSPECTION");
		m_bCrossCompare = ini.GetBool("EnableCrossComp", 0, "CELL_INSPECTION");
		m_bAVACompare = ini.GetBool("EnableAVAComp", 0, "CELL_INSPECTION");
		m_bSlopeFilter = ini.GetBool("SlopeFilter", 0, "CELL_INSPECTION");
		m_iSlopeFactor = ini.GetInt("SlopeFactor", 0, "CELL_INSPECTION");
		m_bGrayFilter = ini.GetBool("GrayFilter", 0, "CELL_INSPECTION");
		m_bCellOffsetZero = ini.GetBool("CellOffsetZero", 0, "CELL_INSPECTION");
		m_bBubbleInspection = ini.GetBool("BubbleInspection", 0, "CELL_INSPECTION");
		m_bASGInspection = ini.GetBool("ASGInspection", 0, "CELL_INSPECTION");
		m_bPIInspection = ini.GetBool("PIInspection", 0, "CELL_INSPECTION");
		m_bNonPeriodicInspection = ini.GetBool("NonPeriodicInspection", 0, "CELL_INSPECTION");
		m_bEnableMultiHorPitch = ini.GetBool("EnableMultiHorPitch", 0, "CELL_INSPECTION");
		m_iMultiHorPitch = ini.GetInt("MultiHorPitch", 0, "CELL_INSPECTION");
		m_bEnableMultiVerPitch = ini.GetBool("EnableMultiVerPitch", 0, "CELL_INSPECTION");
		m_iMultiVerPitch = ini.GetInt("MultiVerPitch", 0, "CELL_INSPECTION");
		m_fPixelsize = ini.GetDouble("Pixelsize", 100, "CELL_INSPECTION");

		ini.SerGetString(1, strTmp, "FOV", "CELL_INSPECTION", "1.0");
		m_fFOV = atof(strTmp);

		m_bEnableZone = ini.GetBool("EnableZone", 0, "CELL_INSPECTION");
		ini.SerGetString(1, strZoneThL, "ZoneTH_L", "CELL_INSPECTION");
		ini.SerGetString(1, strZoneThH, "ZoneTH_L", "CELL_INSPECTION");
		ini.SerGetString(1, strZoneThL2, "ZoneTH_L", "CELL_INSPECTION");
		ini.SerGetString(1, strZoneThH2, "ZoneTH_L", "CELL_INSPECTION");
		ini.SerGetString(1, strZoneMin, "Zone_Min", "CELL_INSPECTION");

		pszZoneThL = static_cast<char *>(strZoneThL.GetBuffer());
		pszZoneThH = static_cast<char *>(strZoneThH.GetBuffer());
		pszZoneThL2 = static_cast<char *>(strZoneThL2.GetBuffer());
		pszZoneThH2 = static_cast<char *>(strZoneThH2.GetBuffer());
		pszZoneMin = static_cast<char *>(strZoneMin.GetBuffer());

		m_iZoneThresholdLow[1] = atoi(strZoneThL);
		m_iZoneThresholdHigh[1] = atoi(strZoneThH);
		m_iZoneThresholdLow2[1] = atoi(strZoneThL2);
		m_iZoneThresholdHigh2[1] = atoi(strZoneThH2);
		m_iZoneMinSize[1] = atoi(strZoneMin);

		for (i = 2; i < MAX_ZONE_CNT; i++)
		{
			iPosL = strZoneThL.Find(',', iPosL);
			m_iZoneThresholdLow[i] = atoi(&(pszZoneThL[++iPosL]));

			iPosH = strZoneThH.Find(',', iPosH);
			m_iZoneThresholdHigh[i] = atoi(&(pszZoneThH[++iPosH]));

			iPosL2 = strZoneThL2.Find(',', iPosL2);
			m_iZoneThresholdLow2[i] = atoi(&(pszZoneThL2[++iPosL2]));

			iPosH2 = strZoneThH2.Find(',', iPosH2);
			m_iZoneThresholdHigh2[i] = atoi(&(pszZoneThH2[++iPosH2]));

			iPosMin = strZoneMin.Find(',', iPosMin);
			m_iZoneMinSize[i] = atoi(&(pszZoneMin[++iPosMin]));
		}

		m_iImgSizeX = ini.GetInt("ImgSizeX", 50, "CELL_INSPECTION");
		m_iImgSizeY = ini.GetInt("ImgSizeY", 50, "CELL_INSPECTION");
		m_iZonePixelSizeX_um = ini.GetInt("ZonePixelSizeX", 50, "CELL_INSPECTION");
		m_iZonePixelSizeY_um = ini.GetInt("ZonePixelSizeY", 50, "CELL_INSPECTION");
		m_strPatImgName = ini.GetString("PatImgName", "pattern0.bmp", "CELL_INSPECTION");
		m_strZoneImgName = ini.GetString("ZoneImgName", "zone0.bmp", "CELL_INSPECTION");
		m_rtROI = ini.GetRect("ROI", CRect(0, 0, 0, 0), "CELL_INSPECTION");

		/*
		m_PadParam.m_bEnablePAD = ini.GetBool("EnablePAD",0,"PAD_INSPECTION");
		m_PadParam.m_bPADCellToCell = ini.GetBool("PADCellToCell",0,"PAD_INSPECTION");

		ini.SerGetString(1, strPADComp, "PADCompareDist", "PAD_INSPECTION") ;
		pszPADComp = static_cast<char *>(strPADComp.GetBuffer());

		for(i = 0; i < PAD_COMP_CNT; i++)
		{
			iPosPADComp = strPADComp.Find(',', iPosPADComp);
			m_PadParam.m_iPADCompareDist[i] = atoi(&(pszPADComp[++iPosPADComp]));
		}

		m_PadParam.m_iPADThL = ini.GetInt("PADThL",12,"PAD_INSPECTION");
		m_PadParam.m_iPADThL = ini.GetInt("PADThH",15,"PAD_INSPECTION");
		m_PadParam.m_iPADMergeDistance = ini.GetInt("PADMergeDistance",10,"PAD_INSPECTION");
		m_PadParam.m_iPADMergeMinSize = ini.GetInt("PADMergeMinSize",2,"PAD_INSPECTION");
		m_PadParam.m_rtROI = ini.GetRect("ROI",CRect(0,0,0,0),"PAD_INSPECTION");

		m_EdgeCrackParam.m_bEnableEdgeCrack = ini.GetBool("EnableEdgeCrack",0,"EDGECRACK_INSPECTION");
		m_EdgeCrackParam.m_iEdgeCrackTh = ini.GetInt("Threshold",30,"EDGECRACK_INSPECTION");
		m_EdgeCrackParam.m_iEdgeCrackTh2 = ini.GetInt("Threshold2",20,"EDGECRACK_INSPECTION");
		m_EdgeCrackParam.m_iMergeDistance = ini.GetInt("MergeDist",10,"EDGECRACK_INSPECTION");
		m_EdgeCrackParam.m_iMinSize = ini.GetInt("MinSize",10,"EDGECRACK_INSPECTION");
		m_EdgeCrackParam.m_rtROI = ini.GetRect("ROI",CRect(0,0,0,0),"EDGECRACK_INSPECTION");
		*/

		m_bAdjacentSlopeFilter = ini.GetBool("EnableAdjacentSlopeFilter", 0, "CELL_INSPECTION");
		m_iEdgeJudgment = ini.GetInt("EdgeJudgment", 0, "CELL_INSPECTION");
		m_iEdgeSlopeTh = ini.GetInt("EdgeSlopeTh", 0, "CELL_INSPECTION");
		m_bPeriodSlopeFilter = ini.GetBool("EnablePeriodSlopeFilter", 0, "CELL_INSPECTION");
		m_iPeriodSlopeJudgment = ini.GetInt("PeriodSlopeJudgment", 0, "CELL_INSPECTION");
		m_iPeriodSlopeTh = ini.GetInt("PeriodSlopeTh", 0, "CELL_INSPECTION");
		m_bTrackingFilter = ini.GetBool("EnableTrackingFilter", 0, "CELL_INSPECTION");
		m_iTrackingRange = ini.GetInt("TrackingRange", 0, "CELL_INSPECTION");
		m_iTrackingTh = ini.GetInt("TrackingTh", 0, "CELL_INSPECTION");
		m_bNearbyCheckFilter = ini.GetBool("EnableNearbyCheckFilter", 0, "CELL_INSPECTION");
		m_bMountainFilter = ini.GetBool("EnableMountainFilter", 0, "CELL_INSPECTION");
	}

	return TRUE;
}

BOOL CParam::SaveParam(CString strPath)
{
	CIni ini(strPath) ;

	int i = 0;
	CString strZoneThL, strZoneThH, strZoneThL2, strZoneThH2, strZoneMin, strPADComp, strTmp;

	ini.WriteBool("MultiThresholdEnable", m_bMultiThreshold, "CELL_INSPECTION");
	ini.WriteInt("DistV", m_iVDist, "CELL_INSPECTION");
	ini.WriteInt("DistH", m_iHDist, "CELL_INSPECTION");
	ini.WriteInt("ThresholdL", m_iThresholdLow, "CELL_INSPECTION");
	ini.WriteInt("ThresholdH", m_iThresholdHigh, "CELL_INSPECTION");
	ini.WriteInt("ThresholdL2", m_iThresholdLow2, "CELL_INSPECTION");
	ini.WriteInt("ThresholdH2", m_iThresholdHigh2, "CELL_INSPECTION");
	ini.WriteInt("ThresholdOffsetL", m_iThresholdOffsetLow, "CELL_INSPECTION");
	ini.WriteInt("ThresholdOffsetH", m_iThresholdOffsetHigh, "CELL_INSPECTION");
	ini.WriteInt("ThresholdOffsetL2", m_iThresholdOffsetLow2, "CELL_INSPECTION");
	ini.WriteInt("ThresholdOffsetH2", m_iThresholdOffsetHigh2, "CELL_INSPECTION");
	ini.WriteInt("MergeDist", m_iMergeDist, "CELL_INSPECTION");
	ini.WriteInt("MinSize", m_iMinSize, "CELL_INSPECTION");
	ini.WriteInt("MaxSize", m_iMaxSize, "CELL_INSPECTION");
	ini.WriteBool("EnableHorComp", m_bHoriCompare, "CELL_INSPECTION");
	ini.WriteBool("EnableDiagonal", m_bDiagonalCompare, "CELL_INSPECTION");
	ini.WriteBool("EnableCrossComp", m_bCrossCompare, "CELL_INSPECTION");
	ini.WriteBool("EnableAVAComp", m_bAVACompare, "CELL_INSPECTION");
	ini.WriteBool("SlopeFilter", m_bSlopeFilter, "CELL_INSPECTION");
	ini.WriteInt("SlopeFactor", m_iSlopeFactor, "CELL_INSPECTION");
	ini.WriteBool("GrayFilter", m_bGrayFilter, "CELL_INSPECTION");
	ini.WriteBool("CellOffsetZero", m_bCellOffsetZero, "CELL_INSPECTION");
	ini.WriteBool("BubbleInspection", m_bBubbleInspection, "CELL_INSPECTION");
	ini.WriteBool("ASGInspection", m_bASGInspection, "CELL_INSPECTION");
	ini.WriteBool("PIInspection", m_bPIInspection, "CELL_INSPECTION");
	ini.WriteBool("NonPeriodicInspection", m_bNonPeriodicInspection, "CELL_INSPECTION");
	ini.WriteBool("EnableMultiHorPitch", m_bEnableMultiHorPitch, "CELL_INSPECTION");
	ini.WriteInt("MultiHorPitch", m_iMultiHorPitch, "CELL_INSPECTION");
	ini.WriteBool("EnableMultiVerPitch", m_bEnableMultiVerPitch, "CELL_INSPECTION");
	ini.WriteInt("MultiVerPitch", m_iMultiVerPitch, "CELL_INSPECTION");
	ini.WriteFloat("PixelSize", m_fPixelsize, "CELL_INSPECTION");
	ini.WriteFloat("FOV", m_fFOV, "CELL_INSPECTION");
	ini.WriteBool("EnableZone", m_bEnableZone, "CELL_INSPECTION");

	for (i = 0; i < MAX_ZONE_CNT; i++)
	{
		strTmp.Format("%d,", m_iZoneThresholdLow[i]);
		strZoneThL += strTmp;

		strTmp.Format("%d,", m_iZoneThresholdHigh[i]);
		strZoneThH += strTmp;

		strTmp.Format("%d,", m_iZoneThresholdLow2[i]);
		strZoneThL2 += strTmp;

		strTmp.Format("%d,", m_iZoneThresholdHigh2[i]);
		strZoneThH2 += strTmp;

		strTmp.Format("%d,", m_iZoneMinSize[i]);
		strZoneMin += strTmp;
	}

	ini.WriteString("ZoneTH_L", strZoneThL, "CELL_INSPECTION");
	ini.WriteString("ZoneTH_H", strZoneThH, "CELL_INSPECTION");
	ini.WriteString("ZoneTH_L2", strZoneThL2, "CELL_INSPECTION");
	ini.WriteString("ZoneTH_H2", strZoneThH2, "CELL_INSPECTION");
	ini.WriteString("Zone_Min", strZoneMin, "CELL_INSPECTION");

	ini.WriteInt("ImgSizeX", m_iImgSizeX, "CELL_INSPECTION");
	ini.WriteInt("ImgSizeY", m_iImgSizeY, "CELL_INSPECTION");
	ini.WriteInt("ZonePixelSizeX", m_iZonePixelSizeX_um, "CELL_INSPECTION");
	ini.WriteInt("ZonePixelSizeY", m_iZonePixelSizeY_um, "CELL_INSPECTION");
	ini.WriteString("PatImgName", m_strPatImgName, "CELL_INSPECTION");
	ini.WriteString("ZoneImgName", m_strZoneImgName, "CELL_INSPECTION");
	ini.WriteRect("ROI", m_rtROI, "CELL_INSPECTION");

	/*
	ini.WriteBool("EnablePAD",m_PadParam.m_bEnablePAD,"PAD_INSPECTION");
	ini.WriteBool("PADCellToCell",m_PadParam.m_bPADCellToCell,"PAD_INSPECTION");

	for(i = 0; i < PAD_COMP_CNT; i++)
	{
		strTmp.Format("%d,", m_PadParam.m_iPADCompareDist[i]);
		strPADComp += strTmp;
	}
	ini.WriteString("PADCompareDist",strPADComp,"PAD_INSPECTION");

	ini.WriteInt("PADThL",m_PadParam.m_iPADThL,"PAD_INSPECTION");
	ini.WriteInt("PADThH",m_PadParam.m_iPADThL,"PAD_INSPECTION");
	ini.WriteInt("PADMergeDistance",m_PadParam.m_iPADMergeDistance,"PAD_INSPECTION");
	ini.WriteInt("PADMergeMinSize",m_PadParam.m_iPADMergeMinSize,"PAD_INSPECTION");
	ini.WriteRect("ROI",m_PadParam.m_rtROI,"PAD_INSPECTION");


	ini.WriteBool("EnableEdgeCrack",m_EdgeCrackParam.m_bEnableEdgeCrack,"EDGECRACK_INSPECTION");
	ini.WriteInt("Threshold",m_EdgeCrackParam.m_iEdgeCrackTh,"EDGECRACK_INSPECTION");
	ini.WriteInt("Threshold2",m_EdgeCrackParam.m_iEdgeCrackTh2,"EDGECRACK_INSPECTION");
	ini.WriteInt("MergeDist",m_EdgeCrackParam.m_iMergeDistance,"EDGECRACK_INSPECTION");
	ini.WriteInt("MinSize",m_EdgeCrackParam.m_iMinSize,"EDGECRACK_INSPECTION");
	ini.WriteRect("ROI",m_EdgeCrackParam.m_rtROI,"EDGECRACK_INSPECTION");
	*/

	ini.WriteBool("EnableAdjacentSlopeFilter", m_bAdjacentSlopeFilter, "CELL_INSPECTION");
	ini.WriteInt("EdgeJudgment", m_iEdgeJudgment, "CELL_INSPECTION");
	ini.WriteInt("EdgeSlopeTh", m_iEdgeSlopeTh, "CELL_INSPECTION");
	ini.WriteBool("EnablePeriodSlopeFilter", m_bPeriodSlopeFilter, "CELL_INSPECTION");
	ini.WriteInt("PeriodSlopeJudgment", m_iPeriodSlopeJudgment, "CELL_INSPECTION");
	ini.WriteInt("PeriodSlopeTh", m_iPeriodSlopeTh, "CELL_INSPECTION");
	ini.WriteBool("EnableTrackingFilter", m_bTrackingFilter, "CELL_INSPECTION");
	ini.WriteInt("TrackingRange", m_iTrackingRange, "CELL_INSPECTION");
	ini.WriteInt("TrackingTh", m_iTrackingTh, "CELL_INSPECTION");
	ini.WriteBool("EnableNearbyCheckFilter", m_bNearbyCheckFilter, "CELL_INSPECTION");
	ini.WriteBool("EnableMountainFilter", m_bMountainFilter, "CELL_INSPECTION");

	return TRUE;
}

void CParam::ZoomRate(float fZoom)
{
	m_fParamZoomValue = fZoom;
}

CMergedDefect::CMergedDefect()
{

}

CMergedDefect::~CMergedDefect()
{
	Release();
}

void CMergedDefect::Release()
{
	iPixelCnt = 0;
}

BOOL CMergedDefect::Set(int x, int y, int Gray, int Reference, int iMergeDist)
{
	if (iPixelCnt >= MAX_1_MERGE_SIZE_BY_PIXEL) return FALSE;

	PixelList[iPixelCnt].iX = x ;
	PixelList[iPixelCnt].iY = y ;
	PixelList[iPixelCnt].iGray = Gray;
	PixelList[iPixelCnt].iReference = Reference;
	iPixelCnt ++;

	m_iMergeDist = iMergeDist;
	m_iRealLeft = x;
	m_iRealRight = x;
	m_iRealTop = y;
	m_iRealBottom = y;

	m_iBoundaryLeft = x - m_iMergeDist;
	m_iBoundaryRight = x + m_iMergeDist;
	m_iBoundaryTop = y - m_iMergeDist;
	m_iBoundaryBottom = y + m_iMergeDist;

	bDelete = FALSE ;

	return TRUE;
}

BOOL CMergedDefect::Add(int x, int y, int Gray, int Reference)
{
	if (iPixelCnt >= MAX_1_MERGE_SIZE_BY_PIXEL) return FALSE;

	PixelList[iPixelCnt].iX = x;
	PixelList[iPixelCnt].iY = y;
	PixelList[iPixelCnt].iGray = Gray;
	PixelList[iPixelCnt].iReference = Reference;

	iPixelCnt ++;

	if (m_iRealLeft > x) {
		m_iRealLeft = x ;
		m_iBoundaryLeft = x - m_iMergeDist;
	}

	if (m_iRealRight < x) {
		m_iRealRight = x ;
		m_iBoundaryRight = x + m_iMergeDist;
	}

	if (m_iRealTop > y) {
		m_iRealTop = y ;
		m_iBoundaryTop = y - m_iMergeDist;
	}

	if (m_iRealBottom < y) {
		m_iRealBottom = y ;
		m_iBoundaryBottom = y + m_iMergeDist;
	}

	return TRUE;
}

void CMergedDefect::Merge(CMergedDefect* pDefect)
{
	if (pDefect != NULL) 
	{
		if (pDefect->iPixelCnt + iPixelCnt > MAX_1_MERGE_SIZE_BY_PIXEL) {
			pDefect->iPixelCnt = MAX_1_MERGE_SIZE_BY_PIXEL - iPixelCnt ;
		}

		memcpy(&(PixelList[iPixelCnt]), pDefect->PixelList, sizeof(CDefectPixel) * pDefect->iPixelCnt) ;

		iPixelCnt += pDefect->iPixelCnt;

		if (m_iRealLeft > pDefect->m_iRealLeft) {
			m_iRealLeft = pDefect->m_iRealLeft ;
			m_iBoundaryLeft = pDefect->m_iRealLeft - m_iMergeDist ;
		}

		if (m_iRealRight < pDefect->m_iRealRight) {
			m_iRealRight = pDefect->m_iRealRight ;
			m_iBoundaryRight = pDefect->m_iRealRight + m_iMergeDist ;
		}

		if (m_iRealTop > pDefect->m_iRealTop) {
			m_iRealTop = pDefect->m_iRealTop ;
			m_iBoundaryTop = pDefect->m_iRealTop - m_iMergeDist ;
		}

		if (m_iRealBottom < pDefect->m_iRealBottom) {
			m_iRealBottom = pDefect->m_iRealBottom ;
			m_iBoundaryBottom = pDefect->m_iRealBottom + m_iMergeDist ;
		}

		pDefect->bDelete = TRUE ;
	}
}

CMerge::CMerge()
{
	ResetContiner();
}


CMerge::~CMerge()
{

}

void CMerge::ResetContiner()
{
	for (int i = 0; i < MAX_1_MERGE_SIZE_BY_PIXEL; i++) {
		DefectList[i].Release();
	}

	iDefectCnt = 0 ;
}

BOOL CMerge::AddPixel(int x, int y, char Gray, char Reference)
{
	if (iDefectCnt >= MAX_1_MERGE_SIZE_BY_PIXEL) {
		return FALSE ;
	}

	for (int i=0; i<iDefectCnt; i++) {
		if (DefectList[i].m_iBoundaryBottom < y) continue;
		if (DefectList[i].m_iBoundaryRight < x) continue;
		if (DefectList[i].m_iBoundaryTop > y) continue;
		if (DefectList[i].m_iBoundaryLeft > x) continue;
		DefectList[i].Add(x, y, static_cast<int>(static_cast<unsigned char>(Gray)), static_cast<int>(static_cast<unsigned char>(Reference)));
		return TRUE;
	}

	DefectList[iDefectCnt].Set(x, y, static_cast<int>(static_cast<unsigned char>(Gray)), static_cast<int>(static_cast<unsigned char>(Reference)), iMergeDistance);
	iDefectCnt++;
	return TRUE;
}

int CMerge::GetExponent(int num, int under)
{
	int i = 0;
	while (1)
	{
		if (num == 1)
			return i;
		i++;
		if ((num%under) == 0)
			num = num / under;
		else
		{
			return 0;
		}
	}
}

int CMerge::Merge(int iCnt, DefectPos *pPixelResult, int iMergeDist, int iMinSize, int iMaxSize, int iMaxCnt, int side)
{
	int i ;
	int iReturnDefectCnt = 0 ;

	ResetContiner();

	iMergeDistance = iMergeDist;

	int iSquare = GetExponent(SSE_PROCESS_PIXELS, 2);
	int iMergeStep = SSE_PROCESS_PIXELS / 4;

	for (i = 0; i < iCnt; ++i)
	{
		for(int iMergeCount = 0; iMergeCount < iMergeStep; iMergeCount++)
		{
			if (pPixelResult[i].POS.m256i_i32[iMergeCount])
			{
				for (int iPixelCount = 0; iPixelCount < SSE_PROCESS_PIXELS; iPixelCount++)
				{
					if (pPixelResult[i].POS.m256i_u8[iPixelCount]) AddPixel((pPixelResult[i].x << iSquare) + iPixelCount, pPixelResult[i].y, 
						pPixelResult[i].DefectGray.m256i_i8[iPixelCount], pPixelResult[i].ReferenceGray.m256i_i8[iPixelCount]);
				}
			}
		}
	}

	for (i = 0; i < iDefectCnt; i++) {
		if (DefectList[i].bDelete) continue ;

		for (int j = i + 1; j < iDefectCnt; j++) {
			if (DefectList[j].bDelete) continue ;
			if (DefectList[i].m_iBoundaryLeft > DefectList[j].m_iRealRight) continue ;
			if (DefectList[i].m_iBoundaryRight < DefectList[j].m_iRealLeft) continue ;
			if (DefectList[i].m_iBoundaryTop > DefectList[j].m_iRealBottom) continue ;
			if (DefectList[i].m_iBoundaryBottom < DefectList[j].m_iRealTop) continue ;

			DefectList[i].Merge(&(DefectList[j])) ;

			j = i+1 ;
		}
		
		if (DefectList[i].iPixelCnt < iMinSize || DefectList[i].iPixelCnt > iMaxSize)
		{
			DefectList[i].bDelete = TRUE ;
		}
		else 
		{
			iReturnDefectCnt ++ ;
		}

		if (iReturnDefectCnt > iMaxCnt) 
		{
			DefectList[i].bDelete = TRUE ;
		}
	}

	return iReturnDefectCnt ;
}

CDetectionAlgorithm::CDetectionAlgorithm(void)
{
	m_pImage = NULL;
	m_pSubtractLowImage = NULL;
	m_pSubtractHighImage = NULL;
	m_iImageWidth = m_iImageHeight = 0;
	m_iDefectCnt = 0;
	m_iPixelCnt = 0;

	m_iVerticalCnt = 0;
	m_iHorizontalCnt = 0;
}

CDetectionAlgorithm::~CDetectionAlgorithm(void)
{
}

void CDetectionAlgorithm::Inspection(BYTE* pImage, BYTE* pSubtractLowImage, BYTE* pSubtractHighImage, int iWidth, int iHeight)
{
	if(pImage == NULL || pSubtractLowImage == NULL || pSubtractHighImage == NULL)
		return;

	m_pImage = pImage;
	m_pSubtractLowImage = pSubtractLowImage;
	m_pSubtractHighImage = pSubtractHighImage;
	m_iImageWidth = iWidth;
	m_iImageHeight = iHeight;

	if(m_Param.m_rtROI.left < 0)
		m_Param.m_rtROI.left = 0;

	if(m_Param.m_rtROI.right > iWidth)
		m_Param.m_rtROI.right = iWidth;

	if(m_Param.m_rtROI.top < 0)
		m_Param.m_rtROI.top = 0;

	if(m_Param.m_rtROI.bottom > iHeight)
		m_Param.m_rtROI.bottom = iHeight;

	if(m_Param.m_bEnableInspection)
	{
		int iCnt = 0;
		iCnt = InspectionProcess();

		m_iDefectCnt = MakeFeature(iCnt);

		//if(m_Param.m_bEnableZone)
		//	iCnt = ZoneInspection();
		//else if(m_Param.m_bASGInspection)
		//	iCnt = ASGInspection();
		//else if(m_Param.m_bNonPeriodicInspection)
		//{
		//	iCnt = NonPeriodicInspection();
		//}
		//else
		//{
		//	if(m_Param.m_bAVACompare == TRUE)
		//	{
		//		/*
		//		int iOffset = 0;
		//		int iSearchTh = m_Param.m_iThresholdLow;
		//		int iInspTh = m_Param.m_iThresholdHigh;
		//		iCnt = EdgeInspection(m_pImage, m_Param.m_rtROI.left, m_Param.m_rtROI.right, m_Param.m_rtROI.top, m_Param.m_rtROI.bottom,
		//		iOffset, iSearchTh, iInspTh, m_pPixelResult, MAX_DEFECT_PIXELCNT, 1, TRUE);
		//		*/
		//	}
		//	else
		//	{
		//		iCnt = PitchInspection();
		//	}
		//}

		//if(m_Param.m_bPIInspection == FALSE)
		//{
		//	m_iDefectCnt = MakeFeature(iCnt);
		//}
		//else
		//{
		//}
	}
}

int CDetectionAlgorithm::ZoneInspection()
{
	if(m_pImage == NULL)
		return 0;

	__m256i *CurrentFrame = reinterpret_cast<__m256i*>(m_pImage);
	int left = m_Param.m_rtROI.left, right = m_Param.m_rtROI.right, top = m_Param.m_rtROI.top, bottom = m_Param.m_rtROI.bottom;	

	int iDistV = m_Param.m_iVDist, iDistH = m_Param.m_iHDist, iThL = m_Param.m_iThresholdLow, iThH = m_Param.m_iThresholdHigh,
		iMergeDist = m_Param.m_iMergeDist, iMinSize = m_Param.m_iMinSize;
	int iThL2 = m_Param.m_iThresholdLow2, iThH2 = m_Param.m_iThresholdHigh2;

	BYTE* pZoneImg = NULL;
	int* pZoneTh_L = NULL;
	int* pZoneTh_H = NULL;
	int* pZoneMinSize = NULL;
	int* pZoneTh_L2 = NULL;
	int* pZoneTh_H2 = NULL;

	BOOL bPM_SMALL_ROI = FALSE;
	BOOL bPM_FAIL = FALSE;
	
	pZoneImg = m_Param.m_pZoneImage ;

	pZoneTh_L = m_Param.m_iZoneThresholdLow;
	pZoneTh_H = m_Param.m_iZoneThresholdHigh;
	pZoneMinSize = m_Param.m_iZoneMinSize;

	BYTE* pRealZoneMaskL = new BYTE[((MAX_PATTERNWIDTH * 4) + m_iImageWidth) * MAX_PATTERNHEIGHT] ;
	BYTE* pRealZoneMaskH = new BYTE[((MAX_PATTERNWIDTH * 4) + m_iImageWidth) * MAX_PATTERNHEIGHT] ;
	BYTE* pRealZoneMaskL2 = new BYTE[((MAX_PATTERNWIDTH * 4) + m_iImageWidth) * MAX_PATTERNHEIGHT] ;
	BYTE* pRealZoneMaskH2 = new BYTE[((MAX_PATTERNWIDTH * 4) + m_iImageWidth) * MAX_PATTERNHEIGHT] ;

	if (m_Param.m_bMultiThreshold)
	{
		pZoneTh_L2 = m_Param.m_iZoneThresholdLow2;
		pZoneTh_H2 = m_Param.m_iZoneThresholdHigh2;
	}

	int	res = ERR_NONE ;
	
	m_PMResult.iStatus = ERR_NONE ;

	for (int i = 0; i < 32; i++) {
		m_PMResult.dVectorxx[i] = m_Param.m_iImgSizeX;
		m_PMResult.dVectorxy[i] = 0. ;

		m_PMResult.iMatchx[i] = -1 ;
		m_PMResult.iMatchy[i] = -1 ;
	}

	MakeVector(CurrentFrame, left, right, top, bottom, &m_PMResult);

	res = res | m_PMResult.iStatus;

	if (ERR_NONE != m_PMResult.iStatus) 
	{
		CString strD = "" ;

		if (ERR_PM_ZOOM == m_PMResult.iStatus) {			
			strD.Format("ERR_PM_ZOOM") ;
		}
		else if (ERR_PM_DET == m_PMResult.iStatus) {
			strD.Format("ERR_PM_DET") ;
		}
		else if (ERR_PM_LOW_RATE == m_PMResult.iStatus) {			
			strD.Format("ERR_PM_LOW_RATE - after makevector") ;
		}
		else {
			if (ERR_PM_SKIP == m_PMResult.iStatus) {
				bPM_SMALL_ROI = 1 ;

				strD.Format("ERR_PM_SKIP") ;
			}
			else {
				strD.Format("!= ERR_NONE") ;
			}			
		}

		res = res | ERR_REAL_ZONE_SKIP | ERR_ZONE_SORT_SKIP ;

		bPM_FAIL = 1 ;
	}
	else
	{
		memset(pRealZoneMaskL, m_Param.m_iThresholdLow, ((MAX_PATTERNWIDTH * 4) + m_iImageWidth) * MAX_PATTERNHEIGHT);
		memset(pRealZoneMaskH, m_Param.m_iThresholdHigh, ((MAX_PATTERNWIDTH * 4) + m_iImageWidth) * MAX_PATTERNHEIGHT);

		memset(pRealZoneMaskL2, m_Param.m_iThresholdLow2, ((MAX_PATTERNWIDTH * 4) + m_iImageWidth) * MAX_PATTERNHEIGHT);
		memset(pRealZoneMaskH2, m_Param.m_iThresholdHigh2, ((MAX_PATTERNWIDTH * 4) + m_iImageWidth) * MAX_PATTERNHEIGHT);					

		
		BOOL bMask = MakeRealZoneMask(pRealZoneMaskL, pRealZoneMaskH, pRealZoneMaskL2, pRealZoneMaskH2, &m_PMResult, left, (right-left) + (m_Param.m_iImgSizeX*2)) ;

		if (bMask == 0 || m_PMResult.iStatus != ERR_NONE) {
			bPM_FAIL = 1 ;
		}
	}
 
	// Real Zone 검사
	int iPatternWidth = m_Param.m_iImgSizeX, iPatternHeight = m_Param.m_iImgSizeY;	
	BOOL bMultiTH = m_Param.m_bMultiThreshold;
	int iMultiTHGray = 0;
	BOOL bEnableHorComp = m_Param.m_bHoriCompare;
	BOOL bSlopeFilter = m_Param.m_bSlopeFilter;

	int ZONE_MASK_WIDTH = ((MAX_PATTERNWIDTH * 4) + m_iImageWidth);
	int ZONE_MASK_HEIGHT = MAX_PATTERNHEIGHT;

	double fZoneOrgX = double(left - m_PMResult.iOrgPosX) ;
	double fZoneOrgY = double(top  - m_PMResult.iOrgPosY) ;

	int	CCD_DEVIDE = m_iImageWidth/DEVIDE_NUMBER;
	int numerator = left ;
	int denominator = CCD_DEVIDE;
	int vN = numerator / denominator ;
	int RPOCESS_WIDTH_32 = m_iImageWidth/16;
	int iLeft = left >> 4 ;
	int iRight = right >> 4 ;
	int	iCnt = 0;

	__m256i	*_pInsp = NULL, *_pUp = NULL, *_pDown = NULL ;
	__m256i	*pInsp = NULL, *pUp = NULL, *pDown = NULL ;
	__m256i	xImg, xUp, xDown ;
	__m256i xMask ;
	__m256i	xDifference;

	__m256i	*pThresholdMask = NULL ;
	__m256i	*pThresholdMaskH = NULL;

	__m256i	*pThresholdMask2 = NULL ;
	__m256i	*pThresholdMaskH2 = NULL;

	__m256i xThreshold, xThresholdH, xThreshold_UseGray ;
	__m256i xThreshold2, xThresholdH2;

	__m256i xThreshold_UseGrayLow, xThreshold_UseGrayHigh ;

	__m256i	xDifferenceLow, xDifferenceHigh;
	__m256i	xResultLow, xResultHigh;

	__m256i xZeros ;
	__m256i xResult ;

	__m256i xOne;

	__m256i xSlope;

	__m256i xSlopeLow, xSlopeHigh;

	__m256i	xTemp ;

	__m256i	xTemp1, xTemp2 ;

	xZeros = _mm256_set1_epi8(static_cast<char>(0)) ;
	xOne   = _mm256_set1_epi8(static_cast<char>(1)) ;
	xMask = _mm256_set1_epi8(0xFF) ;
	xSlope = _mm256_set1_epi16(static_cast<INT16>(iThH + 1 - iThL)) ;

	int x, y ;

	if (vN >= DEVIDE_NUMBER) {
		vN = DEVIDE_NUMBER - 1 ;
	}

	while (fZoneOrgX < 0.) {
		fZoneOrgX += m_PMResult.dVectorxx[vN] ;
		fZoneOrgY += m_PMResult.dVectorxy[vN] ;
	}

	while (fZoneOrgY < 0.) {
		fZoneOrgX += m_PMResult.dVectoryx ;
		fZoneOrgY += m_PMResult.dVectoryy ;
	}

	double fMoveByLineX = (m_PMResult.dVectoryx / (double)iPatternHeight) ;
	double fMoveByLineY = (m_PMResult.dVectoryy / (double)iPatternHeight) ;

	for (y = top; y < bottom; ++y) {

		pInsp = _pInsp += RPOCESS_WIDTH_32 ;
		pUp   = _pUp   += RPOCESS_WIDTH_32 ;
		pDown = _pDown += RPOCESS_WIDTH_32 ;

		pThresholdMask	= reinterpret_cast<__m256i*>(&(pRealZoneMaskL[(static_cast<int>(fZoneOrgY + 0.5)) % iPatternHeight * ZONE_MASK_WIDTH + static_cast<int>(fZoneOrgX + 0.5)])) ;
		pThresholdMaskH = reinterpret_cast<__m256i*>(&(pRealZoneMaskH[(static_cast<int>(fZoneOrgY + 0.5)) % iPatternHeight * ZONE_MASK_WIDTH + static_cast<int>(fZoneOrgX + 0.5)])) ;

		if (bMultiTH)
		{
			pThresholdMask2	 = reinterpret_cast<__m256i*>(&(pRealZoneMaskL2[(static_cast<int>(fZoneOrgY + 0.5)) % iPatternHeight * ZONE_MASK_WIDTH + static_cast<int>(fZoneOrgX + 0.5)])) ;
			pThresholdMaskH2 = reinterpret_cast<__m256i*>(&(pRealZoneMaskH2[(static_cast<int>(fZoneOrgY + 0.5)) % iPatternHeight * ZONE_MASK_WIDTH + static_cast<int>(fZoneOrgX + 0.5)])) ;
		}

		for (x = iLeft; x < iRight; ++x) {

			xImg = _mm256_load_si256(pInsp) ;
			xUp = _mm256_load_si256(pUp) ;
			xDown = _mm256_load_si256(pDown) ;

			xThreshold	= _mm256_loadu_si256(pThresholdMask++) ;
			xThresholdH = _mm256_loadu_si256(pThresholdMaskH++) ;

			//xThreshold = _mm256_add_epi8(xThreshold, xThresholdOffsetL);
			//xThresholdH = _mm256_add_epi8(xThresholdH, xThresholdOffsetH);

			if (bMultiTH)
			{
				xThreshold2	= _mm256_loadu_si256(pThresholdMask2++) ;
				xThresholdH2 = _mm256_loadu_si256(pThresholdMaskH2++) ;

				//xThreshold2 = _mm256_add_epi8(xThreshold2, xThresholdOffsetL2);
				//xThresholdH2 = _mm256_add_epi8(xThresholdH2, xThresholdOffsetH2);

				xSlopeLow = _mm256_subs_epi16(xThresholdH, xThreshold);
				xSlopeLow = _mm256_adds_epi16(xSlopeLow, xOne);
				xSlopeHigh = _mm256_subs_epi16(xThresholdH2, xThreshold2);
				xSlopeHigh = _mm256_adds_epi16(xSlopeHigh, xOne);
			}

			xResultHigh = xZeros;
			xResultLow  = xZeros;
			xResult = xZeros;

			if(bMultiTH)
			{
				xDifferenceHigh =					
					_mm256_min_epi8(
					_mm256_subs_epu8(xImg, xUp),
					_mm256_subs_epu8(xImg, xDown)
					);

				xDifferenceLow = 
					_mm256_min_epi8(
					_mm256_subs_epu8(xUp, xImg),
					_mm256_subs_epu8(xDown, xImg)
					);

				xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold2), xZeros) ;
				xResultLow  = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold), xZeros) ;
			}
			else
			{
				xDifference =
					_mm256_max_epu8(
					_mm256_min_epi8(
					_mm256_subs_epu8(xImg, xUp),
					_mm256_subs_epu8(xImg, xDown)
					),
					_mm256_min_epi8(
					_mm256_subs_epu8(xUp, xImg),
					_mm256_subs_epu8(xDown, xImg)
					)
					) ;

				xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros) ;
			}

			if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
			{
				if(bMultiTH)
				{
					if(xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4])
					{
						xTemp1 = _mm256_packus_epi16(
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros), 
							_mm256_unpacklo_epi8(xSlopeLow, xZeros)), 8), 
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros), 
							_mm256_unpackhi_epi8(xSlopeLow, xZeros)), 8));
						xThreshold_UseGrayLow  = _mm256_adds_epi8(xThreshold, xTemp1) ;
						xResultLow  = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold_UseGrayLow), xZeros) ;
					}
					if(xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
					{
						xTemp2 = _mm256_packus_epi16(
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros), 
							_mm256_unpacklo_epi8(xSlopeHigh, xZeros)), 8), 
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros), 
							_mm256_unpackhi_epi8(xSlopeHigh, xZeros)), 8));
						xThreshold_UseGrayHigh = _mm256_adds_epi8(xThreshold2, xTemp2) ;
						xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold_UseGrayHigh), xZeros) ;
					}						
				}
				else
				{
					xSlope	= _mm256_subs_epi16(xThresholdH, xThreshold);
					xSlope	= _mm256_adds_epi16(xSlope, xOne);

					xTemp = _mm256_packus_epi16(
						_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros), 
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8), 
						_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros), 
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8));

					xThreshold_UseGray  = _mm256_adds_epi8(xThreshold, xTemp) ;

					xResult  = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold_UseGray), xZeros) ;
				}					

				if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
				{
					if (bEnableHorComp)
					{
						if (x*16-iDistH >= left)
						{
							xTemp = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp)-iDistH));

							if(bMultiTH)
							{
								if(xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
								{
									xDifferenceHigh = _mm256_min_epi8(xDifferenceHigh, _mm256_add_epi8( _mm256_subs_epu8(xImg, xTemp), _mm256_subs_epu8(xTemp, xImg)));
									xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold2), xZeros) ;
								}
								if(xResultLow.m256i_u16[0]  + xResultLow.m256i_u16[4])
								{
									xDifferenceLow  = _mm256_min_epi8(xDifferenceLow , _mm256_add_epi8( _mm256_subs_epu8(xImg, xTemp), _mm256_subs_epu8(xTemp, xImg)));
									xResultLow  = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold), xZeros) ;			
								}									
							}
							else
							{
								xDifference = _mm256_min_epi8(xDifference, _mm256_add_epi8(_mm256_subs_epu8(xImg, xTemp), _mm256_subs_epu8(xTemp, xImg)));
								xResult  = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros) ;
							}
						}

						if (x*16+iDistH <= right-16)
						{
							xTemp = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp)+iDistH));

							if(bMultiTH)
							{
								if(xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
								{
									xDifferenceHigh = _mm256_min_epi8(xDifferenceHigh, _mm256_add_epi8( _mm256_subs_epu8(xImg, xTemp), _mm256_subs_epu8(xTemp, xImg)));
									xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold2), xZeros) ;
								}
								if(xResultLow.m256i_u16[0]  + xResultLow.m256i_u16[4])
								{
									xDifferenceLow  = _mm256_min_epi8(xDifferenceLow , _mm256_add_epi8( _mm256_subs_epu8(xImg, xTemp), _mm256_subs_epu8(xTemp, xImg)));
									xResultLow  = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold), xZeros) ;			
								}		
							}
							else
							{
								xDifference = _mm256_min_epi8(xDifference, _mm256_add_epi8(_mm256_subs_epu8(xImg, xTemp), _mm256_subs_epu8(xTemp, xImg)));
								xResult  = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros) ;
							}
						}
					}

					if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
					{
						if(bMultiTH)
						{
							if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4]) 
							{
								xResultLow = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayLow, xDifferenceLow), xMask), xMask);
							}								

							if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
							{
								xResultHigh = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayHigh, xDifferenceHigh), xMask), xMask);
							}	

							m_pPixelResult[iCnt].x = x ;
							m_pPixelResult[iCnt].y = y ;
							m_pPixelResult[iCnt].POS = _mm256_or_si256(xResultLow, xResultHigh);
							m_pPixelResult[iCnt].DefectGray = xImg;
							m_pPixelResult[iCnt].ReferenceGray = xUp;

							iCnt++ ;

							if (iCnt >= MAX_DEFECT_PIXELCNT) 
								goto ZONE_SKIP;

						}
						else
						{
							if (xResult.m256i_u16[0] + xResult.m256i_u16[4]) 
							{
								xResult = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGray, xDifference), xMask), xMask);

								m_pPixelResult[iCnt].x = x ;
								m_pPixelResult[iCnt].y = y ;
								m_pPixelResult[iCnt].POS = xResult;
								m_pPixelResult[iCnt].DefectGray = xImg;
								m_pPixelResult[iCnt].ReferenceGray = xUp;

								iCnt++ ;

								if (iCnt >= MAX_DEFECT_PIXELCNT) 
									goto ZONE_SKIP;
							}
						}

					}
				}
			}
			pInsp ++ ;
			pUp ++ ;
			pDown ++ ;
		}

		fZoneOrgX += fMoveByLineX ;
		fZoneOrgY += fMoveByLineY ;

		if (fZoneOrgX < 0.) {
			fZoneOrgX += m_PMResult.dVectorxx[vN] ;
			fZoneOrgY += m_PMResult.dVectorxy[vN] ;
		}

		if (fZoneOrgY < 0.) {
			fZoneOrgX += m_PMResult.dVectoryx ;
			fZoneOrgY += m_PMResult.dVectoryy ;
		}
	}

ZONE_SKIP:
	if(pRealZoneMaskL != NULL)
		delete[] pRealZoneMaskL;
	if(pRealZoneMaskH != NULL)
		delete[] pRealZoneMaskH;
	if(pRealZoneMaskL2 != NULL)
		delete[] pRealZoneMaskL2;
	if(pRealZoneMaskH2 != NULL)
		delete[] pRealZoneMaskH2;

	return iCnt;
}

int CDetectionAlgorithm::SubInspection(BYTE* pImage1, BYTE* pImage2, int iWidth, int iHeight, int iThL, int iThH, int iThL2, int iThH2, BOOL bMultiTH)
{
	if(pImage1 == NULL || pImage2 == NULL)
		return 0;

	__m256i *Frame1 = reinterpret_cast<__m256i*>(pImage1);
	__m256i *Frame2 = reinterpret_cast<__m256i*>(pImage2);

	int MAX_RPOCESS_WIDTH_32 = iWidth/16;
	int left = 0, right = iWidth, top = 0, bottom = iHeight;

	int x = 0, y = 0;
	int iIterateCnt = (right - left) / 16 ;
	int	iLeft = 0, iRight = 0;
	int iLeft_Outside = 0, iRight_Outside = 0;
	int iCnt = 0;

	iLeft = left >> 4 ;
	iRight = right >> 4 ;

	__m256i	*_pInsp1, *_pInsp2;
	__m256i	*pInsp1, *pInsp2;
	__m256i	xImg1, xImg2;

	__m256i	xDifference, xThreshold, xThresholdH, xThreshold2, xThresholdH2, xThreshold_UseGray ;	
	__m256i xThreshold_UseGrayLow, xThreshold_UseGrayHigh ;
	__m256i	xDifferenceLow, xDifferenceHigh;
	__m256i	xResultLow, xResultHigh;
	__m256i xResult, xMask, xZeros, xOne, xSlope, xSlopeLow, xSlopeHigh, xLeft , xRight, xTemp, xTemp1, xTemp2 ;

	xZeros = _mm256_set1_epi8(static_cast<char>(0)) ;
	xOne   = _mm256_set1_epi8(static_cast<char>(1)) ;
	xMask = _mm256_set1_epi8(0xFF) ;
	xSlope = _mm256_set1_epi16(static_cast<INT16>(iThH + 1 - iThL)) ;

	xThreshold = _mm256_set1_epi8(static_cast<char>(iThL)) ;
	xThresholdH = _mm256_set1_epi8(static_cast<char>(iThH)) ;

	if(bMultiTH)
	{
		xThreshold2 = _mm256_set1_epi8(static_cast<char>(iThL2)) ;
		xThresholdH2 = _mm256_set1_epi8(static_cast<char>(iThH2)) ;

		xSlopeLow = _mm256_subs_epi16(xThresholdH, xThreshold);
		xSlopeLow = _mm256_adds_epi16(xSlopeLow, xOne);

		xSlopeHigh = _mm256_subs_epi16(xThresholdH2, xThreshold2);
		xSlopeHigh = _mm256_adds_epi16(xSlopeHigh, xOne);
	}
	else
	{
		xSlope	= _mm256_subs_epi16(xThresholdH, xThreshold);
		xSlope	= _mm256_adds_epi16(xSlope, xOne);
	}

	_pInsp1 = Frame1 + ((top - 1) * iWidth + left) / 16 ;
	_pInsp2 = Frame2 + ((top - 1) * iWidth + left) / 16 ;
	
	for (y = top; y < bottom; ++y)
	{
		pInsp1 = _pInsp1 += MAX_RPOCESS_WIDTH_32 ;
		pInsp2 = _pInsp2 += MAX_RPOCESS_WIDTH_32 ;

		for (x = iLeft; x < iRight; ++x) 
		{
			xImg1 = _mm256_load_si256(pInsp1) ;
			xImg2 = _mm256_load_si256(pInsp2) ;

			xResultHigh = xZeros;
			xResultLow  = xZeros;
			xResult = xZeros;
			xDifference = xZeros;

			if(bMultiTH)
			{
				xDifferenceHigh = _mm256_subs_epu8(xImg1, xImg2);
				xDifferenceLow = _mm256_subs_epu8(xImg2, xImg1);				

				xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold2), xZeros) ;
				xResultLow  = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold), xZeros) ;
			}
			else
			{
				xDifference = _mm256_max_epu8(_mm256_subs_epu8(xImg1, xImg2), _mm256_subs_epu8(xImg2, xImg1));
				
				xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros) ;
			}

			if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
			{
				if(bMultiTH)
				{
					if(xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4])
					{
						xTemp1 = _mm256_packus_epi16(
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg1, xZeros), 
							_mm256_unpacklo_epi8(xSlopeLow, xZeros)), 8), 
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg1, xZeros), 
							_mm256_unpackhi_epi8(xSlopeLow, xZeros)), 8));
						xThreshold_UseGrayLow  = _mm256_adds_epi8(xThreshold, xTemp1) ;
						xResultLow  = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold_UseGrayLow), xZeros) ;
					}
					if(xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
					{
						xTemp2 = _mm256_packus_epi16(
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg1, xZeros), 
							_mm256_unpacklo_epi8(xSlopeHigh, xZeros)), 8), 
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg1, xZeros), 
							_mm256_unpackhi_epi8(xSlopeHigh, xZeros)), 8));
						xThreshold_UseGrayHigh = _mm256_adds_epi8(xThreshold2, xTemp2) ;
						xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold_UseGrayHigh), xZeros) ;
					}						
				}
				else
				{
					xTemp = _mm256_packus_epi16(
						_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg1, xZeros), 
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8), 
						_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg1, xZeros), 
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8));

					xThreshold_UseGray  = _mm256_adds_epi8(xThreshold, xTemp) ;

					xResult  = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold_UseGray), xZeros) ;
				}

				if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
				{
					if(bMultiTH)
					{
						if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4]) 
						{
							xResultLow = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayLow, xDifferenceLow), xMask), xMask);
						}								

						if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
						{
							xResultHigh = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayHigh, xDifferenceHigh), xMask), xMask);
						}
							
						m_pPixelResult[iCnt].x = x ;
						m_pPixelResult[iCnt].y = y ;
						m_pPixelResult[iCnt].POS = _mm256_or_si256(xResultLow, xResultHigh);
						m_pPixelResult[iCnt].DefectGray = xImg1;
						m_pPixelResult[iCnt].ReferenceGray = xImg2;

						for(int k = 0; k < 16; k++)
						{
							if (m_pPixelResult[iCnt].POS.m256i_u8[k])
								++m_iPixelCnt;
						}

						iCnt++ ;

						if (iCnt >= MAX_DEFECT_PIXELCNT) 
							return iCnt ;
					}
					else
					{
						if (xResult.m256i_u16[0] + xResult.m256i_u16[4]) 
						{
							xResult = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGray, xDifference), xMask), xMask);

							for(int k = 0; k < 16; k++)
							{
								if (xResult.m256i_u8[k])
									++m_iPixelCnt;
							}

							m_pPixelResult[iCnt].x = x ;
							m_pPixelResult[iCnt].y = y ;
							m_pPixelResult[iCnt].POS = xResult;
							m_pPixelResult[iCnt].DefectGray = xImg1;
							m_pPixelResult[iCnt].ReferenceGray = xImg2;

							iCnt++ ;

							if (iCnt >= MAX_DEFECT_PIXELCNT) 
								return iCnt ;							
							
						}
					}
				}
			}

			pInsp1++ ;
			pInsp2++ ;
		}
	}

	return iCnt;
}

int	CDetectionAlgorithm::InspectionProcess()
{
	if (m_pImage == NULL)
		return 0;

	m_iPixelCnt = 0;
	int iCnt = 0;

	int MAX_RPOCESS_WIDTH_32 = m_iImageWidth / SSE_PROCESS_PIXELS;
	int left = m_Param.m_rtROI.left, right = m_Param.m_rtROI.right, top = m_Param.m_rtROI.top, bottom = m_Param.m_rtROI.bottom;
	CRect rtROI = m_Param.m_rtROI;

	BOOL bMultiTH = m_Param.m_bMultiThreshold;
	BOOL bEnableHorComp = m_Param.m_bHoriCompare;
	BOOL bEnableDiagonalComp = m_Param.m_bDiagonalCompare;

	BOOL bEnableMultiVerPitch = m_Param.m_bEnableMultiVerPitch;
	int	iMultiVerPitch = m_Param.m_iMultiVerPitch;

	__m256i xZeros;
	__m256i xOne;
	__m256i xMask;
	__m256i	xTemp, xTemp1, xTemp2, xTempResult;

	__m256i *CurrentFrame = reinterpret_cast<__m256i*>(m_pImage);
	__m256i	*_pInsp, *_pUp, *_pDown;
	__m256i	*pInsp, *pUp, *pDown, *pLeft, *pRight;
	__m256i	xImg, xUp, xDown, xLeft, xRight;

	__m256i xThreshold, xThresholdH, xThreshold_UseGray;
	__m256i xThreshold2, xThresholdH2, xThreshold_UseGrayLow, xThreshold_UseGrayHigh;
	__m256i xSlope, xSlopeLow, xSlopeHigh;

	__m256i	xDifference, xDifferenceLow, xDifferenceHigh;
	__m256i	xResult, xResultLow, xResultHigh;
	__m256i xDeleteGray;
	
	int x = 0, y = 0;

	int iDistV = m_Param.m_iVDist, 
		iDistH = m_Param.m_iHDist, 
		iThL = m_Param.m_iThresholdLow, 
		iThH = m_Param.m_iThresholdHigh, 
		iThL2 = m_Param.m_iThresholdLow2,
		iThH2 = m_Param.m_iThresholdHigh2,
		iMergeDist = m_Param.m_iMergeDist, 
		iMinSize = m_Param.m_iMinSize,
		iThOffsetL = m_Param.m_iThresholdOffsetLow, 
		iThOffsetH = m_Param.m_iThresholdOffsetHigh,
		iThOffsetL2 = m_Param.m_iThresholdOffsetLow2, 
		iThOffsetH2 = m_Param.m_iThresholdOffsetHigh2;

	xZeros = _mm256_set1_epi8(static_cast<char>(0)) ;
	xOne   = _mm256_set1_epi8(static_cast<char>(1)) ;
	xMask = _mm256_set1_epi8(0xFF) ;

	xSlope = _mm256_set1_epi16(static_cast<INT16>(iThH + 1 - iThL));
	xSlopeLow = _mm256_set1_epi16(static_cast<INT16>(iThH + 1 - iThL));
	xSlopeHigh = _mm256_set1_epi16(static_cast<INT16>(iThH2 + 1 - iThL2));

	xThreshold = _mm256_set1_epi8(static_cast<char>(iThL)) ;
	xThresholdH = _mm256_set1_epi8(static_cast<char>(iThH)) ;
	xThreshold2 = _mm256_set1_epi8(static_cast<char>(iThL2));
	xThresholdH2 = _mm256_set1_epi8(static_cast<char>(iThH2));

	xThreshold_UseGray = xThreshold_UseGrayLow = xThreshold_UseGrayHigh = xZeros;
	xDifference = xDifferenceLow = xDifferenceHigh = xZeros;
	xResultHigh = xResultLow = xResult = xZeros;
	xLeft = xRight = xZeros;
	xDeleteGray = xZeros;

	left += SSE_PROCESS_PIXELS;
	right -= SSE_PROCESS_PIXELS;

	int iIterateCnt = (right - left) / SSE_PROCESS_PIXELS;
	int	iLeft = 0, iRight = 0;

	BOOL bTopCellEdge = FALSE, bBottomCellEdge = FALSE;
	//BOOL bVerticalIns = FALSE, bHorizontalIns = FALSE;
	int iVerticalDefectCnt = 0, iHorizontalDefectCnt = 0;

	if (top == 0 && bottom < m_iImageHeight - 1)
		bBottomCellEdge = TRUE;
	else if (top != 0 && bottom >= m_iImageHeight - 1)
		bTopCellEdge = TRUE;

	iLeft = left >> 5;
	iRight = right >> 5;

	_pInsp = CurrentFrame + (top * m_iImageWidth + left) / SSE_PROCESS_PIXELS;
	
	pLeft = pRight = NULL;
	int iTempY = 0;

	for (y = top; y < bottom; ++y)
	{
		pInsp = (CurrentFrame + (y * m_iImageWidth + left) / SSE_PROCESS_PIXELS);
		// 상하, 상상, 하하 영역 참조
		if (y - iDistV < rtROI.top)
		{
			if (y + iDistV * 2 > rtROI.bottom - 1)
				continue;

			pUp = pInsp + iDistV * MAX_RPOCESS_WIDTH_32;
			pDown = pInsp + (iDistV * 2)*MAX_RPOCESS_WIDTH_32;
		}
		else if (y + iDistV > rtROI.bottom - 1)
		{
			if (y - iDistV * 2 < rtROI.top)
				continue;

			pUp = pInsp - iDistV * MAX_RPOCESS_WIDTH_32;
			pDown = pInsp - (iDistV * 2)*MAX_RPOCESS_WIDTH_32;
		}
		else
		{
			pUp = pInsp - iDistV * MAX_RPOCESS_WIDTH_32;
			pDown = pInsp + iDistV * MAX_RPOCESS_WIDTH_32;
		}

		for (x = iLeft; x < iRight; ++x)
		{
			xImg = _mm256_loadu_si256(pInsp);
			xUp = _mm256_loadu_si256(pUp);
			xDown = _mm256_loadu_si256(pDown);

			iVerticalDefectCnt += VerticalInspection(pInsp, pUp, pDown, xImg, xUp, xDown, bMultiTH, xZeros, xOne, xMask, xThreshold, xThresholdH, xThreshold2, xThresholdH2, xSlope, xSlopeLow, xSlopeHigh,
				xThreshold_UseGray, xThreshold_UseGrayLow, xThreshold_UseGrayHigh, xDifference, xDifferenceLow, xDifferenceHigh, xResult, xResultLow, xResultHigh);

			if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
				xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
				xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
			{
				// 좌우, 좌좌, 우우 참조
				if (x * SSE_PROCESS_PIXELS - iDistH < left)
				{
					if (x * SSE_PROCESS_PIXELS + iDistH * 2 > rtROI.right - 1)
						continue;

					xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) + iDistH));
					xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) + iDistH * 2));
				}
				else if (x * SSE_PROCESS_PIXELS + iDistH > right - SSE_PROCESS_PIXELS)
				{
					if (x * SSE_PROCESS_PIXELS - iDistH * 2 < rtROI.left)
						continue;

					xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) - iDistH));
					xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) - iDistH * 2));
				}
				else
				{
					xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) - iDistH));
					xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) + iDistH));
				}

				if (bEnableHorComp)
				{
					iHorizontalDefectCnt += HorizontalInspection(pInsp, pLeft, pRight, xImg, xLeft, xRight, bMultiTH, xZeros, xOne, xMask, xThreshold, xThresholdH, xThreshold2, xThresholdH2, xSlope, xSlopeLow, xSlopeHigh,
						xThreshold_UseGray, xThreshold_UseGrayLow, xThreshold_UseGrayHigh, xDifference, xDifferenceLow, xDifferenceHigh, xResult, xResultLow, xResultHigh);
				}

				if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
					xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
					xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
				{
					if (bEnableDiagonalComp)
					{
						int iSubMultiCnt = 10;
						int iSubMultiDir[20] = { 2,2,-2,-2,2,-2,-2,2,2,0,-2,0,1,1,-1,-1,1,-1,-1,1 };
						int iSubMultiFilter[10] = { 0,0,0,0,0,0,0,0,0,0 };

						if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
							xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
							xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
						{
							if (iSubMultiCnt != 0)
							{
								if (DiagonalInspection(x, y, rtROI, pInsp, pUp, pDown, xImg, xLeft, xRight, bMultiTH, xZeros, xOne, xMask, xThreshold, xThresholdH, xThreshold2, xThresholdH2, xSlope, xSlopeLow, xSlopeHigh,
									xThreshold_UseGray, xThreshold_UseGrayLow, xThreshold_UseGrayHigh, xDifference, xDifferenceLow, xDifferenceHigh, xResult, xResultLow, xResultHigh,
									MAX_RPOCESS_WIDTH_32, iDistV, iDistH, iSubMultiCnt, iSubMultiDir, iSubMultiFilter) == FALSE)
								{
									goto FILTERED_NORMAL;
								}
							}
						}
					}

					if (bMultiTH)
					{
						if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
							xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
						{
							m_pPixelResult[iCnt].x = x;
							m_pPixelResult[iCnt].y = y;
							m_pPixelResult[iCnt].POS = _mm256_or_si256(xResultLow, xResultHigh);
							m_pPixelResult[iCnt].DefectGray = xImg;
							m_pPixelResult[iCnt].ReferenceGray = _mm256_avg_epu8(xUp, xDown);
							m_pPixelResult[iCnt].DeleteGray = xDeleteGray;

							for (int k = 0; k < SSE_PROCESS_PIXELS; k++)
							{
								if (m_pPixelResult[iCnt].POS.m256i_u8[k])
									++m_iPixelCnt;
							}

							iCnt++;

							if (iCnt >= MAX_DEFECT_PIXELCNT)
								return iCnt;
						}
					}
					else
					{
						if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12])
						{
							xTempResult = xResult;
							for (int k = 0; k < SSE_PROCESS_PIXELS - 1; k++)
							{
								//if(y == top || y == bottom - 1)
								//	continue; // editing

								if (xTempResult.m256i_u8[k] != 0)
								{
									if ((xTempResult.m256i_u8[k] != 0) && (xResult.m256i_u8[k + 1] != 0))
									{
										xResult.m256i_u8[k + 1] = 0;
										xDeleteGray.m256i_u8[k + 1] = xTempResult.m256i_u8[k + 1];
									}
									else
									{
										xResult.m256i_u8[k] = xTempResult.m256i_u8[k];
									}
								}
							}

							for (int k = 0; k < SSE_PROCESS_PIXELS; k++)
							{
								if (xResult.m256i_u8[k])
									++m_iPixelCnt;
							}

							if (m_iPixelCnt > 0)
							{
								m_pPixelResult[iCnt].x = x;
								m_pPixelResult[iCnt].y = y;
								m_pPixelResult[iCnt].POS = xResult;
								m_pPixelResult[iCnt].DefectGray = xImg;
								m_pPixelResult[iCnt].ReferenceGray = _mm256_avg_epu8(xUp, xDown);
								m_pPixelResult[iCnt].DeleteGray = xDeleteGray;

								iCnt++;

								if (iCnt >= MAX_DEFECT_PIXELCNT)
									return iCnt;
							}
						}
					}
				}
			}
		FILTERED_NORMAL:
			pInsp++;
			pUp++;
			pDown++;
		}
	}

	return iCnt;
}

int CDetectionAlgorithm::VerticalInspection(__m256i* pInsp, __m256i* pUp, __m256i* pDown, __m256i xImg, __m256i xUp, __m256i xDown, BOOL bMultiTH,
	__m256i xZeros, __m256i xOne, __m256i xMask,
	__m256i xThreshold, __m256i xThresholdH, __m256i xThreshold2, __m256i xThresholdH2,
	__m256i xSlope, __m256i xSlopeLow, __m256i xSlopeHigh,
	__m256i &xThreshold_UseGray, __m256i &xThreshold_UseGrayLow, __m256i &xThreshold_UseGrayHigh, 
	__m256i &xDifference, __m256i &xDifferenceLow, __m256i &xDifferenceHigh, 
	__m256i &xResult, __m256i &xResultLow, __m256i &xResultHigh)
{
	__m256i xTemp, xTemp1, xTemp2;
	int iCnt = 0;

	if (bMultiTH)
	{
		xDifferenceHigh = _mm256_min_epu8(_mm256_subs_epu8(xImg, xUp), _mm256_subs_epu8(xImg, xDown));
		xDifferenceLow = _mm256_min_epu8(_mm256_subs_epu8(xUp, xImg), _mm256_subs_epu8(xDown, xImg));
		xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold2), xZeros);
		xResultLow = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold), xZeros);
	}
	else
	{
		xDifference = _mm256_max_epu8(_mm256_min_epu8(_mm256_subs_epu8(xImg, xUp), _mm256_subs_epu8(xImg, xDown)),
			_mm256_min_epu8(_mm256_subs_epu8(xUp, xImg), _mm256_subs_epu8(xDown, xImg)));
		xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros);
	}

	if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
		xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
		xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
	{
		if (bMultiTH)
		{
			if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12])
			{
				xTemp1 = _mm256_packus_epi16(
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros),
						_mm256_unpacklo_epi8(xSlopeLow, xZeros)), 8),
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros),
						_mm256_unpackhi_epi8(xSlopeLow, xZeros)), 8));

				xThreshold_UseGrayLow = _mm256_adds_epi8(xThreshold, xTemp1);
				xResultLow = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold_UseGrayLow), xZeros);
			}
			if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
			{
				xTemp2 = _mm256_packus_epi16(
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros),
						_mm256_unpacklo_epi8(xSlopeHigh, xZeros)), 8),
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros),
						_mm256_unpackhi_epi8(xSlopeHigh, xZeros)), 8));

				xThreshold_UseGrayHigh = _mm256_adds_epi8(xThreshold2, xTemp2);
				xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold_UseGrayHigh), xZeros);
			}
		}
		else
		{
			if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12])
			{
				xTemp = _mm256_packus_epi16(
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros),
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8),
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros),
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8));

				xThreshold_UseGray = _mm256_adds_epi8(xThreshold, xTemp);
				xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold_UseGray), xZeros);
			}
		}

		if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
		{
			if (bMultiTH)
			{
				if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12])
				{
					xResultLow = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayLow, xDifferenceLow), xMask), xMask);
				}
				if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
				{
					xResultHigh = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayHigh, xDifferenceHigh), xMask), xMask);
				}
			}
			else
			{
				if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12])
				{
					xResult = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGray, xDifference), xMask), xMask);
				}
			}

			if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
				xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
				xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
			{
				iCnt++;
			}
		}
	}

	return iCnt;
}

int CDetectionAlgorithm::HorizontalInspection(__m256i* pInsp, __m256i* pLeft, __m256i* pRight, __m256i xImg, __m256i xLeft, __m256i xRight, BOOL bMultiTH,
	__m256i xZeros, __m256i xOne, __m256i xMask,
	__m256i xThreshold, __m256i xThresholdH, __m256i xThreshold2, __m256i xThresholdH2,
	__m256i xSlope, __m256i xSlopeLow, __m256i xSlopeHigh,
	__m256i &xThreshold_UseGray, __m256i &xThreshold_UseGrayLow, __m256i &xThreshold_UseGrayHigh,
	__m256i &xDifference, __m256i &xDifferenceLow, __m256i &xDifferenceHigh,
	__m256i &xResult, __m256i &xResultLow, __m256i &xResultHigh)
{
	__m256i xTemp, xTemp1, xTemp2;
	int iCnt = 0;

	if (bMultiTH)
	{
		xDifferenceHigh = _mm256_min_epu8(_mm256_subs_epu8(xImg, xLeft), _mm256_subs_epu8(xImg, xRight));
		xDifferenceLow = _mm256_min_epu8(_mm256_subs_epu8(xLeft, xImg), _mm256_subs_epu8(xRight, xImg));
		xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold2), xZeros);
		xResultLow = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold), xZeros);
	}
	else
	{
		xDifference = _mm256_max_epu8(_mm256_min_epu8(_mm256_subs_epu8(xImg, xLeft), _mm256_subs_epu8(xImg, xRight)),
										_mm256_min_epu8(_mm256_subs_epu8(xLeft, xImg), _mm256_subs_epu8(xRight, xImg)));
		xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros);
	}

	if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
		xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
		xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
	{
		if (bMultiTH)
		{
			if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12])
			{
				xTemp1 = _mm256_packus_epi16(
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros),
						_mm256_unpacklo_epi8(xSlopeLow, xZeros)), 8),
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros),
						_mm256_unpackhi_epi8(xSlopeLow, xZeros)), 8));

				xThreshold_UseGrayLow = _mm256_adds_epi8(xThreshold, xTemp1);
				xResultLow = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold_UseGrayLow), xZeros);
			}
			if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
			{
				xTemp2 = _mm256_packus_epi16(
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros),
						_mm256_unpacklo_epi8(xSlopeHigh, xZeros)), 8),
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros),
						_mm256_unpackhi_epi8(xSlopeHigh, xZeros)), 8));

				xThreshold_UseGrayHigh = _mm256_adds_epi8(xThreshold2, xTemp2);
				xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold_UseGrayHigh), xZeros);
			}
		}
		else
		{
			if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12])
			{
				xTemp = _mm256_packus_epi16(
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros),
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8),
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros),
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8));

				xThreshold_UseGray = _mm256_adds_epi8(xThreshold, xTemp);
				xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold_UseGray), xZeros);
			}
		}

		if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
			xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
			xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
		{
			if (bMultiTH)
			{
				if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4])
				{
					xResultLow = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayLow, xDifferenceLow), xMask), xMask);
				}
				if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
				{
					xResultHigh = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayHigh, xDifferenceHigh), xMask), xMask);
				}
			}
			else
			{
				if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12])
				{
					xResult = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGray, xDifference), xMask), xMask);
				}
			}

			if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
				xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
				xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
			{
				iCnt++;
			}
		}
	}

	return iCnt;
}

int CDetectionAlgorithm::DiagonalInspection(int x, int y, CRect &rtInsp,
	__m256i *pInsp, __m256i *pUp, __m256i *pDown, __m256i xImg, __m256i xUp, __m256i xDown, BOOL bMultiTh,
	__m256i xZeros, __m256i xOne, __m256i xMask,
	__m256i xThreshold, __m256i xThresholdH, __m256i xThreshold2, __m256i xThresholdH2,
	__m256i xSlope, __m256i xSlopeLow, __m256i xSlopeHigh,
	__m256i &xThreshold_UseGray, __m256i &xThreshold_UseGrayLow, __m256i &xThreshold_UseGrayHigh,
	__m256i &xDifference, __m256i &xDifferenceLow, __m256i &xDifferenceHigh,
	__m256i &xResult, __m256i &xResultLow, __m256i &xResultHigh, int MAX_RPOCESS_WIDTH_32,
	int iDistV, int iDistH, int iSubMultiCnt, int *iSubMultiDir, int *iSubMultiFilter)
{
	
	__m256i xRef1, xRef2;
	__m256i xDifferenceE, xDifferenceELow, xDifferenceEHigh;
	__m256i xPos, xPosLow, xPosHigh;
	__m256i xThreshold_UseGrayE;

	xRef1 = xRef2 = xZeros;
	xDifferenceE = xDifferenceELow = xDifferenceEHigh = xThreshold_UseGrayE = xZeros;

	int iCnt = 0;

	CvPoint pDir[10];
	int iIdx = 0;
	for (int i = 0; i < iSubMultiCnt * 2; i += 2)
	{
		//0,1 /2,3 /4,5 /6,7 /8,9 /10,11 /12,13 /14,15 /16,17 /18,19 
		pDir[iIdx].x = iSubMultiDir[i];
		pDir[iIdx].y = iSubMultiDir[i + 1];
		iIdx++;
	}

	for (int i = 0; i < iSubMultiCnt; i += 2)
	{
		if (y - abs(pDir[i].y * iDistV) > rtInsp.top && y + abs(pDir[i].y * iDistV) < rtInsp.bottom &&
			y - abs(pDir[i + 1].y * iDistV) > rtInsp.top && y + abs(pDir[i + 1].y * iDistV) < rtInsp.bottom)
		{
			if ((x*SSE_PROCESS_PIXELS) - abs(pDir[i].x * iDistH) > rtInsp.left && (x*SSE_PROCESS_PIXELS) + abs(pDir[i].x * iDistH) < rtInsp.right - SSE_PROCESS_PIXELS &&
				(x*SSE_PROCESS_PIXELS) - abs(pDir[i + 1].x * iDistH) > rtInsp.left && (x*SSE_PROCESS_PIXELS) + abs(pDir[i + 1].x * iDistH) < rtInsp.right - SSE_PROCESS_PIXELS)
			{
				xRef1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + (pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32)) + pDir[i].x * iDistH));
				xRef2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + (pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32)) + pDir[i + 1].x * iDistH));

				if (bMultiTh)
				{
					if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12])
					{
						xDifferenceELow = _mm256_min_epi8(_mm256_subs_epu8(xRef1, xImg), _mm256_subs_epu8(xRef2, xImg));
						xPosLow = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayLow, xDifferenceLow), xMask), xMask);
						xDifferenceLow = _mm256_and_si256(xDifferenceELow, xPosLow);
						xResultLow = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold_UseGrayLow), xZeros);
					}
					if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
					{
						xDifferenceEHigh = _mm256_min_epi8(_mm256_subs_epu8(xImg, xRef1), _mm256_subs_epu8(xImg, xRef2));
						xPosHigh = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayHigh, xDifferenceHigh), xMask), xMask);
						xDifferenceHigh = _mm256_and_si256(xDifferenceEHigh, xPosHigh);
						xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold_UseGrayHigh), xZeros);
					}
				}
				else
				{
					if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12])
					{
						xDifferenceE = _mm256_max_epu8(_mm256_min_epi8(_mm256_subs_epu8(xImg, xRef1), _mm256_subs_epu8(xImg, xRef2)),
							_mm256_min_epi8(_mm256_subs_epu8(xRef1, xImg), _mm256_subs_epu8(xRef2, xImg)));
						xPos = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGray, xDifference), xMask), xMask);
						xDifference = _mm256_min_epi8(xDifferenceE, xPos);
						xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold_UseGray), xZeros);
					}
				}

				if (!(xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
					xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
					xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12]))
				{	//filtering 대상
					return iCnt;
				}
				else
				{
					if (bMultiTh)
					{
						if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12])
						{
							for (int k = 0; k < SSE_PROCESS_PIXELS; k++)
							{	// False 제거용 Filter
								if (xResultLow.m256i_u8[k])
								{
									xResultLow = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayLow, xDifferenceLow), xMask), xMask);
									int iThreshold_UseGray = xThreshold_UseGrayLow.m256i_u8[k];

									if (iSubMultiFilter[0] && Cell2PixelMergeFilter((x*SSE_PROCESS_PIXELS) + k, y,
										(x*SSE_PROCESS_PIXELS) + (pDir[i].x * iDistH) + k, y + pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32,
										(x*SSE_PROCESS_PIXELS) + (pDir[i + 1].x * iDistH) + k, y + pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32, iThreshold_UseGray,
										reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp)))
									{
										xResultLow.m256i_u8[k] = 0;
										continue;
									}
									if (iSubMultiFilter[1] && Cell2PixelVertMergeFilter((x*SSE_PROCESS_PIXELS) + k, y,
										(x*SSE_PROCESS_PIXELS) + (pDir[i].x * iDistH) + k, y + pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32,
										(x*SSE_PROCESS_PIXELS) + (pDir[i + 1].x * iDistH) + k, y + pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32, iThreshold_UseGray))
									{
										xResultLow.m256i_u8[k] = 0;
										continue;
									}
									if (iSubMultiFilter[2] && Convolution2x2Filter((x*SSE_PROCESS_PIXELS) + k, y,
										(x*SSE_PROCESS_PIXELS) + (pDir[i].x * iDistH) + k, y + pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32,
										(x*SSE_PROCESS_PIXELS) + (pDir[i + 1].x * iDistH) + k, y + pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32, iThreshold_UseGray,
										reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp)))
									{
										xResultLow.m256i_u8[k] = 0;
										continue;
									}
								}
							}
						}
						if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
						{
							for (int k = 0; k < SSE_PROCESS_PIXELS; k++)
							{
								if (xResultHigh.m256i_u8[k])
								{
									xResultHigh = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayHigh, xDifferenceHigh), xMask), xMask);
									int iThreshold_UseGray = xThreshold_UseGrayHigh.m256i_u8[k];

									if (iSubMultiFilter[0] && Cell2PixelMergeFilter((x*SSE_PROCESS_PIXELS) + k, y,
										(x*SSE_PROCESS_PIXELS) + (pDir[i].x * iDistH) + k, y + pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32,
										(x*SSE_PROCESS_PIXELS) + (pDir[i + 1].x * iDistH) + k, y + pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32, iThreshold_UseGray,
										reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp)))
									{
										xResultHigh.m256i_u8[k] = 0;
										continue;
									}
									if (iSubMultiFilter[1] && Cell2PixelVertMergeFilter((x*SSE_PROCESS_PIXELS) + k, y,
										(x*SSE_PROCESS_PIXELS) + (pDir[i].x * iDistH) + k, y + pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32,
										(x*SSE_PROCESS_PIXELS) + (pDir[i + 1].x * iDistH) + k, y + pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32, iThreshold_UseGray))
									{
										xResultHigh.m256i_u8[k] = 0;
										continue;
									}
									if (iSubMultiFilter[2] && Convolution2x2Filter((x*SSE_PROCESS_PIXELS) + k, y,
										(x*SSE_PROCESS_PIXELS) + (pDir[i].x * iDistH) + k, y + pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32,
										(x*SSE_PROCESS_PIXELS) + (pDir[i + 1].x * iDistH) + k, y + pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32, iThreshold_UseGray,
										reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp)))
									{
										xResultHigh.m256i_u8[k] = 0;
										continue;
									}
								}
							}
						}
					}
					else
					{
						if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12])
						{
							for (int k = 0; k < SSE_PROCESS_PIXELS; k++)
							{	// False 제거용 Filter
								if (xResult.m256i_u8[k])
								{
									xResult = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGray, xDifference), xMask), xMask);
									int iThreshold_UseGray = xThreshold_UseGray.m256i_u8[k];

									if (iSubMultiFilter[0] && Cell2PixelMergeFilter((x*SSE_PROCESS_PIXELS) + k, y,
										(x*SSE_PROCESS_PIXELS) + (pDir[i].x * iDistH) + k, y + pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32,
										(x*SSE_PROCESS_PIXELS) + (pDir[i + 1].x * iDistH) + k, y + pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32, iThreshold_UseGray,
										reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp)))
									{
										xResult.m256i_u8[k] = 0;
										continue;
									}
									if (iSubMultiFilter[1] && Cell2PixelVertMergeFilter((x*SSE_PROCESS_PIXELS) + k, y,
										(x*SSE_PROCESS_PIXELS) + (pDir[i].x * iDistH) + k, y + pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32,
										(x*SSE_PROCESS_PIXELS) + (pDir[i + 1].x * iDistH) + k, y + pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32, iThreshold_UseGray))
									{
										xResult.m256i_u8[k] = 0;
										continue;
									}
									if (iSubMultiFilter[2] && Convolution2x2Filter((x*SSE_PROCESS_PIXELS) + k, y,
										(x*SSE_PROCESS_PIXELS) + (pDir[i].x * iDistH) + k, y + pDir[i].y * iDistV*MAX_RPOCESS_WIDTH_32,
										(x*SSE_PROCESS_PIXELS) + (pDir[i + 1].x * iDistH) + k, y + pDir[i + 1].y * iDistV*MAX_RPOCESS_WIDTH_32, iThreshold_UseGray,
										reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp), reinterpret_cast<BYTE*>(pInsp)))
									{
										xResult.m256i_u8[k] = 0;
										continue;
									}
								}
							}
						}
					}

					if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResult.m256i_u16[8] + xResult.m256i_u16[12] +
						xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultLow.m256i_u16[8] + xResultLow.m256i_u16[12] +
						xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4] + xResultHigh.m256i_u16[8] + xResultHigh.m256i_u16[12])
					{
						iCnt++;
					}
				}
			}
			else
			{
				return iCnt;
			}
		}
		else
		{
			return iCnt;
		}
	}

	return iCnt;
}

int CDetectionAlgorithm::ASGInspection()
{
	if(m_pImage == NULL)
		return 0;

	__m256i *CurrentFrame = reinterpret_cast<__m256i*>(m_pImage);

	__m256i	*_pInsp = NULL;
	__m256i	*pInsp = NULL;
	__m256i	xImg;

	__m256i	*_pUp = NULL, *_pDown = NULL ;
	__m256i	*pUp = NULL, *pDown = NULL ;
	__m256i	xUp, xDown ;

	__m256i	*_pLeft = NULL, *_pRight = NULL ;
	__m256i	*pLeft = NULL, *pRight = NULL;
	__m256i	xLeft, xRight ;

	__m256i xCompare1, xCompare2;

	__m256i xMask ;

	__m256i xThreshold, xThresholdH;
	__m256i xThreshold2, xThresholdH2;

	__m256i xThreshold_UseGray;
	__m256i xThreshold_UseGrayLow, xThreshold_UseGrayHigh ;

	__m256i	xDifference;
	__m256i	xDifferenceLow, xDifferenceHigh;

	__m256i xResult;
	__m256i	xResultLow, xResultHigh;

	__m256i xSlope;
	__m256i xSlopeLow, xSlopeHigh;

	__m256i xZeros;
	__m256i xOne;

	__m256i xTemp, xTemp1, xTemp2;

	int iDistV = m_Param.m_iVDist, iDistH = m_Param.m_iHDist;
	int RPOCESS_WIDTH_32 = m_iImageWidth/16;
	int CAMERA_CCD_SIZE = m_iImageWidth, FRAME_HEIGHT = m_iImageHeight;
	int iTh_L = m_Param.m_iThresholdLow, iTh_H = m_Param.m_iThresholdHigh, iTh_L2 = m_Param.m_iThresholdLow2, iTh_H2 = m_Param.m_iThresholdHigh2;
	int left = m_Param.m_rtROI.left, right = m_Param.m_rtROI.right, top = m_Param.m_rtROI.top, bottom = m_Param.m_rtROI.bottom;
	BOOL bMultiTh= m_Param.m_bMultiThreshold;
	int iCnt = 0 ;
	int iIterateCnt = (right - left) / 16 ;
	int	iLeft, iRight ;
	int x, y;

	int iCompareOffset;

	iLeft = left >> 4 ;
	iRight = right >> 4 ;

	if(iDistV == 0) // Horizontal ASG
	{
		if(right - left < 3*iDistH)
			return 0;
	}

	iCompareOffset = iDistV * RPOCESS_WIDTH_32;

	_pInsp = CurrentFrame + ((top - 1) * CAMERA_CCD_SIZE + left) / 16 ;

	xZeros = _mm256_set1_epi8(static_cast<char>(0)) ;
	xOne   = _mm256_set1_epi8(static_cast<char>(1)) ;
	xMask = _mm256_set1_epi8(0xFF) ;
	xSlope = _mm256_set1_epi16(static_cast<INT16>(iTh_H + 1 - iTh_L)) ;

	xThreshold = _mm256_set1_epi8(static_cast<char>(iTh_L)) ;
	xThresholdH = _mm256_set1_epi8(static_cast<char>(iTh_H)) ;

	if(bMultiTh)
	{
		xThreshold2 = _mm256_set1_epi8(static_cast<char>(iTh_L2)) ;
		xThresholdH2 = _mm256_set1_epi8(static_cast<char>(iTh_H2)) ;

		xSlopeLow = _mm256_subs_epi16(xThresholdH, xThreshold);
		xSlopeLow = _mm256_adds_epi16(xSlopeLow, xOne);

		xSlopeHigh = _mm256_subs_epi16(xThresholdH2, xThreshold2);
		xSlopeHigh = _mm256_adds_epi16(xSlopeHigh, xOne);
	}
	else
	{
		xSlope	= _mm256_subs_epi16(xThresholdH, xThreshold);
		xSlope	= _mm256_adds_epi16(xSlope, xOne);
	}

	if(bottom - top < 3*iDistV && iDistH == 0)
	{
		if(top != 0 && bottom != 0)
			return 0;
	}

	for (y = top; y < bottom; ++y) 
	{
		pInsp = _pInsp += RPOCESS_WIDTH_32 ;

		if(iDistH == 0)
		{
			/*
			if(bottom - top < 3*iDistV)
			{
				if(top == 0)
				{
					return 0;

					if(y - iDistV < 0)
					{
						pDown = PreviousFrame + ((FRAME_HEIGHT - (2*iDistV - y) - 1) * CAMERA_CCD_SIZE + left) / 16 ;
						pUp   = PreviousFrame + ((FRAME_HEIGHT - (iDistV - y) - 1) * CAMERA_CCD_SIZE + left) / 16 ;				
					}
					else if(y - 2*iDistV < 0)
					{
						pUp = _pInsp - 1*iCompareOffset ;
						pDown = PreviousFrame + ((FRAME_HEIGHT - (2*iDistV - y) - 1) * CAMERA_CCD_SIZE + left) / 16 ;
					}
				}
				else if(bottom == 0)
				{
					return 0;

					if(NextFrame == NULL)
					{						
						return 0;
					}

					if(y + iDistV > FRAME_HEIGHT - 1)
					{
						pDown = NextFrame + ((iDistV - y - 1) * CAMERA_CCD_SIZE + left) / 16 ;
						pUp   = NextFrame + ((2*iDistV - y - 1) * CAMERA_CCD_SIZE + left) / 16 ;
					}
					else if(y + 2*iDistV > FRAME_HEIGHT - 1)
					{
						pDown = _pInsp + 1*iCompareOffset ;
						pUp   = NextFrame + ((2*iDistV - y - 1) * CAMERA_CCD_SIZE + left) / 16 ;
					}
				}		
			}
			else
			{
				if(y - iDistV <= top)
				{
					pUp   = _pInsp + 1*iCompareOffset ;
					pDown = _pInsp + 2*iCompareOffset ;

				}
				else if(y + iDistV >= bottom)
				{
					pUp   = _pInsp - 1*iCompareOffset ;
					pDown = _pInsp - 2*iCompareOffset ;

				}
				else
				{					
					pUp   = _pInsp - 1*iCompareOffset ;
					pDown = _pInsp + 1*iCompareOffset ;					
				}
			}
			*/
		}		

		for (x = iLeft; x < iRight; ++x) 
		{
			xResultHigh = xResultLow  = xResult = xZeros;

			xImg = _mm256_load_si256(pInsp) ;

			if(iDistV == 0)
			{
				if(x * 16 - iDistH < left)
				{
					xLeft  = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp)+1*iDistH));
					xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp)+2*iDistH));
				}
				else if(x * 16 + iDistH > right - 16)
				{
					xLeft  = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp)-1*iDistH));
					xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp)-2*iDistH));
				}
				else
				{
					xLeft  = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp)-1*iDistH));
					xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp)+1*iDistH));
				}
				xCompare1 = xLeft;
				xCompare2 = xRight;
			}
			if(iDistH == 0)
			{
				xUp = _mm256_load_si256(pUp) ;
				xDown = _mm256_load_si256(pDown) ;

				xCompare1 = xUp;
				xCompare2 = xDown;
			}	

			if(bMultiTh)
			{
				xDifferenceHigh =					
					_mm256_min_epi8(
					_mm256_subs_epu8(xImg, xCompare1),
					_mm256_subs_epu8(xImg, xCompare2)
					);

				xDifferenceLow = 
					_mm256_min_epi8(
					_mm256_subs_epu8(xCompare1, xImg),
					_mm256_subs_epu8(xCompare2, xImg)
					);				

				xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold2), xZeros) ;
				xResultLow  = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold), xZeros) ;
			}
			else
			{					
				xDifference =
					_mm256_max_epu8(
					_mm256_min_epi8(
					_mm256_subs_epu8(xImg, xCompare1),
					_mm256_subs_epu8(xImg, xCompare2)
					),
					_mm256_min_epi8(
					_mm256_subs_epu8(xCompare1, xImg),
					_mm256_subs_epu8(xCompare2, xImg)
					)
					) ;						

				xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros) ;
			}
			if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
			{
				if(bMultiTh)
				{
					if(xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4])
					{
						xTemp1 = _mm256_packus_epi16(
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros), 
							_mm256_unpacklo_epi8(xSlopeLow, xZeros)), 8), 
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros), 
							_mm256_unpackhi_epi8(xSlopeLow, xZeros)), 8));
						xThreshold_UseGrayLow  = _mm256_adds_epi8(xThreshold, xTemp1) ;
						xResultLow  = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold_UseGrayLow), xZeros) ;
					}
					if(xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
					{
						xTemp2 = _mm256_packus_epi16(
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros), 
							_mm256_unpacklo_epi8(xSlopeHigh, xZeros)), 8), 
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros), 
							_mm256_unpackhi_epi8(xSlopeHigh, xZeros)), 8));
						xThreshold_UseGrayHigh = _mm256_adds_epi8(xThreshold2, xTemp2) ;
						xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold_UseGrayHigh), xZeros) ;
					}						
				}
				else
				{
					xTemp = _mm256_packus_epi16(
						_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros), 
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8), 
						_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros), 
						_mm256_unpacklo_epi8(xSlope, xZeros)), 8));

					xThreshold_UseGray  = _mm256_adds_epi8(xThreshold, xTemp) ;

					xResult  = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold_UseGray), xZeros) ;
				}

				if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
				{
					__m256i xResultPos, xResultLowPos, xResultHighPos;
					xResultLowPos = xResultHighPos = xResultPos = xZeros;

					if(bMultiTh)
					{
						if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4]) 
						{
							xResultHighPos = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayHigh, xDifferenceHigh), xMask), xMask);
						}
						if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4]) 
						{
							xResultLowPos = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayLow, xDifferenceLow), xMask), xMask);
						}							
					}
					else
					{
						if (xResult.m256i_u16[0] + xResult.m256i_u16[4]) 
						{
							xResultPos = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGray, xDifference), xMask), xMask);
						}
					}
					xResultPos = _mm256_or_si256(_mm256_or_si256(xResultHighPos, xResultLowPos), xResultPos);

					int iDefectCnt = 0;

					for (int k = 0; k < 16; k++)
					{
						if (xResultPos.m256i_u8[k])
						{
							if(ASGSlopeFilter(reinterpret_cast<BYTE*>(CurrentFrame), x*16+k, y, iDistH, iDistV, left, right, top, bottom))
							{
								xResultPos.m256i_u8[k] = 0;
							}
							else
							{
								iDefectCnt++;
							}			
						}
					}
					if(iDefectCnt != 0)
					{
						m_pPixelResult[iCnt].x = x ;
						m_pPixelResult[iCnt].y = y ;
						m_pPixelResult[iCnt].POS = xResultPos;
						m_pPixelResult[iCnt].DefectGray = xImg;
						m_pPixelResult[iCnt].ReferenceGray = xCompare1;

						iCnt++ ;

						if (iCnt >= MAX_DEFECT_PIXELCNT) 
							return iCnt ;
					}
				}
			}			
			pInsp++ ;

			if(iDistH == 0)
			{
				pUp++ ;
				pDown++ ;
			}
		}
	}			
	return iCnt ;
}

int CDetectionAlgorithm::QDASGInspection()
{
	if (m_pImage == NULL)
		return 0;

	__m256i *CurrentFrame = reinterpret_cast<__m256i*>(m_pImage);

	__m256i	*_pInsp = NULL;
	__m256i	*pInsp = NULL;
	__m256i	xImg;

	__m256i	*_pUp = NULL, *_pDown = NULL;
	__m256i	*pUp = NULL, *pDown = NULL;
	__m256i	xUp, xDown;

	__m256i	*_pLeft = NULL, *_pRight = NULL;
	__m256i	*pLeft = NULL, *pRight = NULL;
	__m256i	xLeft, xRight;

	__m256i xCompare1, xCompare2;

	__m256i xMask;

	__m256i xThreshold, xThresholdH;
	__m256i xThreshold2, xThresholdH2;

	__m256i xThreshold_UseGray;
	__m256i xThreshold_UseGrayLow = _mm256_set1_epi8(static_cast<char>(0)), xThreshold_UseGrayHigh = _mm256_set1_epi8(static_cast<char>(0));

	__m256i	xDifference;
	__m256i	xDifferenceLow, xDifferenceHigh;

	__m256i xResult;
	__m256i	xResultLow, xResultHigh;

	__m256i xSlope;
	__m256i xSlopeLow, xSlopeHigh;

	__m256i xZeros;
	__m256i xOne;

	__m256i xTemp, xTemp1, xTemp2;

	__m256i	*pUpUp, *pUpDown;
	__m256i	*pDownUp, *pDownDown;
	__m256i	xUpUp, xUpDown;
	__m256i	xDownUp, xDownDown;

	int iDistV = m_Param.m_iVDist, iDistH = m_Param.m_iHDist;
	if (m_Param.m_bHoriCompare)
		iDistV = 0;
	else
		iDistH = 0;

	int RPOCESS_WIDTH_32 = m_iImageWidth / 16;
	int CAMERA_CCD_SIZE = m_iImageWidth, FRAME_HEIGHT = m_iImageHeight;
	int iTh_L = m_Param.m_iThresholdLow, iTh_H = m_Param.m_iThresholdHigh, iTh_L2 = m_Param.m_iThresholdLow2, iTh_H2 = m_Param.m_iThresholdHigh2;
	int left = m_Param.m_rtROI.left, right = m_Param.m_rtROI.right, top = m_Param.m_rtROI.top, bottom = m_Param.m_rtROI.bottom;
	BOOL bMultiTh = m_Param.m_bMultiThreshold;
	int iCnt = 0;
	int iIterateCnt = (right - left) / 16;
	int iLeftDist = 0, iRightDist = 0;
	int	iLeft, iRight;
	int x, y;
	int iDifferenceUpUp = 0, iDifferenceUpDown = 0, iDifferenceDownUp = 0, iDifferenceDownDown = 0;
	int iShiftY = 0;
	int nShiftLeftGray = 0, nShiftRightGray = 0;
	int iCurGray = 0;
	int nTh = 0;

	EdgeSlopeFilterParam stEdgeSlopeFilter;
	PeriodSlopeFilterParam stPeriodSlopeFilter;

	int iCompareOffset;

	iLeft = left >> 4;
	iRight = right >> 4;

	if (iDistV == 0) // Horizontal ASG
	{
		if (right - left < 3 * iDistH)
			return 0;
	}

	iCompareOffset = iDistV * RPOCESS_WIDTH_32;

	_pInsp = CurrentFrame + ((top - 1) * CAMERA_CCD_SIZE + left) / SSE_PROCESS_PIXELS;

	xZeros = _mm256_set1_epi8(static_cast<char>(0));
	xOne = _mm256_set1_epi8(static_cast<char>(1));
	xMask = _mm256_set1_epi8(0xFF);
	xSlope = _mm256_set1_epi16(static_cast<INT16>(iTh_H + 1 - iTh_L));

	xThreshold = _mm256_set1_epi8(static_cast<char>(iTh_L));
	xThresholdH = _mm256_set1_epi8(static_cast<char>(iTh_H));

	if (bMultiTh)
	{
		xThreshold2 = _mm256_set1_epi8(static_cast<char>(iTh_L2));
		xThresholdH2 = _mm256_set1_epi8(static_cast<char>(iTh_H2));

		xSlopeLow = _mm256_subs_epi16(xThresholdH, xThreshold);
		xSlopeLow = _mm256_adds_epi16(xSlopeLow, xOne);

		xSlopeHigh = _mm256_subs_epi16(xThresholdH2, xThreshold2);
		xSlopeHigh = _mm256_adds_epi16(xSlopeHigh, xOne);
	}
	else
	{
		xSlope = _mm256_subs_epi16(xThresholdH, xThreshold);
		xSlope = _mm256_adds_epi16(xSlope, xOne);
	}

	if (bottom - top < 3 * iDistV && iDistH == 0)
	{
		if (top != 0 && bottom != 0)
			return 0;
	}

	for (y = top; y < bottom; ++y)
	{
		pInsp = _pInsp += RPOCESS_WIDTH_32;
		pUp = pDown = pUpUp = pUpDown = pDownUp = pDownDown = _pInsp;

		if (iDistH == 0)
		{
			if (bottom - top < 3 * iDistV + 2)
				return 0;
			else if (y - iDistV - 1 < top)
				continue;
			else if (y + iDistV + 1 > bottom)
				continue;
			else
			{
				pUp = _pInsp - 1 * iCompareOffset;
				pDown = _pInsp + 1 * iCompareOffset;

				pUpUp = _pInsp - 1 * iCompareOffset - RPOCESS_WIDTH_32;
				pUpDown = _pInsp - 1 * iCompareOffset + RPOCESS_WIDTH_32;
				pDownUp = _pInsp + 1 * iCompareOffset - RPOCESS_WIDTH_32;
				pDownDown = _pInsp + 1 * iCompareOffset + RPOCESS_WIDTH_32;
			}
		}

		for (x = iLeft; x < iRight; ++x)
		{
			xResultHigh = xResultLow = xResult = xZeros;

			xImg = _mm256_load_si256(pInsp);

			if (iDistV == 0)
			{
				if (x * SSE_PROCESS_PIXELS - iDistH < left)
				{
					if (x * SSE_PROCESS_PIXELS + iDistH * 2 < right)
					{
						xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) + 1 * iDistH));
						xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) + 2 * iDistH));
					}
				}
				else if (x * SSE_PROCESS_PIXELS + iDistH > right)
				{
					if (x * SSE_PROCESS_PIXELS - iDistH * 2 > left)
					{
						xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) - 1 * iDistH));
						xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) - 2 * iDistH));
					}
				}
				else
				{
					if ((x * SSE_PROCESS_PIXELS - iDistH > left) && (x * SSE_PROCESS_PIXELS + iDistH < right))
					{
						xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) - 1 * iDistH));
						xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp) + 1 * iDistH));
					}
				}
				xCompare1 = xLeft;
				xCompare2 = xRight;
			}
			if (iDistH == 0)
			{
				xUp = _mm256_load_si256(pUp);
				xDown = _mm256_load_si256(pDown);

				xCompare1 = xUp;
				xCompare2 = xDown;
			}

			if (bMultiTh)
			{
				xDifferenceHigh =
					_mm256_min_epi8(
						_mm256_subs_epu8(xImg, xCompare1),
						_mm256_subs_epu8(xImg, xCompare2)
					);

				xDifferenceLow =
					_mm256_min_epi8(
						_mm256_subs_epu8(xCompare1, xImg),
						_mm256_subs_epu8(xCompare2, xImg)
					);

				xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold2), xZeros);
				xResultLow = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold), xZeros);
			}
			else
			{
				xDifference =
					_mm256_max_epu8(
						_mm256_min_epi8(
							_mm256_subs_epu8(xImg, xCompare1),
							_mm256_subs_epu8(xImg, xCompare2)
						),
						_mm256_min_epi8(
							_mm256_subs_epu8(xCompare1, xImg),
							_mm256_subs_epu8(xCompare2, xImg)
						)
					);

				xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros);
			}
			if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
			{
				if (bMultiTh)
				{
					if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4])
					{
						xTemp1 = _mm256_packus_epi16(
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros),
								_mm256_unpacklo_epi8(xSlopeLow, xZeros)), 8),
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros),
								_mm256_unpackhi_epi8(xSlopeLow, xZeros)), 8));
						xThreshold_UseGrayLow = _mm256_adds_epi8(xThreshold, xTemp1);
						xResultLow = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceLow, xThreshold_UseGrayLow), xZeros);
					}
					if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
					{
						xTemp2 = _mm256_packus_epi16(
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros),
								_mm256_unpacklo_epi8(xSlopeHigh, xZeros)), 8),
							_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros),
								_mm256_unpackhi_epi8(xSlopeHigh, xZeros)), 8));
						xThreshold_UseGrayHigh = _mm256_adds_epi8(xThreshold2, xTemp2);
						xResultHigh = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceHigh, xThreshold_UseGrayHigh), xZeros);
					}
				}
				else
				{
					xTemp = _mm256_packus_epi16(
						_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros),
							_mm256_unpacklo_epi8(xSlope, xZeros)), 8),
						_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros),
							_mm256_unpacklo_epi8(xSlope, xZeros)), 8));

					xThreshold_UseGray = _mm256_adds_epi8(xThreshold, xTemp);

					xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold_UseGray), xZeros);
				}

				if (xResult.m256i_u16[0] + xResult.m256i_u16[4])
				{
					if (m_Param.m_bNearbyCheckFilter && iDistH == 0)
					{
						xUpUp = _mm256_load_si256(pUpUp);
						xUpDown = _mm256_load_si256(pUpDown);
						xDownUp = _mm256_load_si256(pDownUp);
						xDownDown = _mm256_load_si256(pDownDown);

						if (!bMultiTh)
						{
							__m256i xDifferenceUpUp = _mm256_max_epu8(_mm256_subs_epu8(xImg, xUpUp), _mm256_subs_epu8(xUpUp, xImg));
							__m256i xDifferenceUpDown = _mm256_max_epu8(_mm256_subs_epu8(xImg, xUpDown), _mm256_subs_epu8(xUpDown, xImg));
							__m256i xDifferenceDownUp = _mm256_max_epu8(_mm256_subs_epu8(xImg, xDownUp), _mm256_subs_epu8(xDownUp, xImg));
							__m256i xDifferenceDownDown = _mm256_max_epu8(_mm256_subs_epu8(xImg, xDownDown), _mm256_subs_epu8(xDownDown, xImg));

							iDifferenceUpUp = iDifferenceUpDown = iDifferenceDownUp = iDifferenceDownDown = 0;

							for (int iIndex = 0; iIndex < SSE_PROCESS_PIXELS; iIndex++)
							{
								iDifferenceUpUp += xDifferenceUpUp.m256i_u8[iIndex];
								iDifferenceUpDown += xDifferenceUpDown.m256i_u8[iIndex];
								iDifferenceDownUp += xDifferenceDownUp.m256i_u8[iIndex];
								iDifferenceDownDown += xDifferenceDownDown.m256i_u8[iIndex];
							}

							__m256i xCompareUp, xCompareDown;

							if (iDifferenceUpUp > iDifferenceUpDown)
							{
								xCompareUp = xUpDown;
							}
							else
							{
								xCompareUp = xUpUp;
							}

							if (iDifferenceDownUp > iDifferenceDownDown)
							{
								xCompareDown = xDownDown;
							}
							else
							{
								xCompareDown = xDownUp;
							}

							__m256i xReInsDifference =
								_mm256_max_epu8(
									_mm256_min_epi8(
										_mm256_subs_epu8(xImg, xCompareUp),
										_mm256_subs_epu8(xImg, xCompareDown)
									),
									_mm256_min_epi8(
										_mm256_subs_epu8(xCompareUp, xImg),
										_mm256_subs_epu8(xCompareDown, xImg)
									)
								);

							xDifference = _mm256_min_epi8(xDifference, xReInsDifference);

							xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold_UseGray), xZeros);
						}
					}
				}

				stEdgeSlopeFilter.Set(left, right, x, y, iDistH, iDistV, RPOCESS_WIDTH_32, pInsp, pUp, pDown, xImg,
					xThreshold_UseGray, xThreshold_UseGrayLow, xThreshold_UseGrayHigh,
					&xDifference, &xDifferenceLow, &xDifferenceHigh,
					bMultiTh, m_Param.m_bAdjacentSlopeFilter, m_Param.m_iEdgeJudgment, m_Param.m_iEdgeSlopeTh,
					xZeros, xMask, &xResult, &xResultLow, &xResultHigh);

				EdgeSlopeFilter(stEdgeSlopeFilter);

				stPeriodSlopeFilter.Set(left, right, x, y, iDistH, iDistV, pInsp, xImg,
					xThreshold_UseGray, xThreshold_UseGrayLow, xThreshold_UseGrayHigh,
					&xDifference, &xDifferenceLow, &xDifferenceHigh,
					bMultiTh, m_Param.m_bPeriodSlopeFilter, m_Param.m_iPeriodSlopeJudgment, m_Param.m_iPeriodSlopeTh,
					xZeros, xMask, &xResult, &xResultLow, &xResultHigh);

				PeriodSlopeFilter(stPeriodSlopeFilter);

				// Dot Filter
				if (m_Param.m_bMountainFilter)
				{
					if (m_Param.m_bHoriCompare)
					{
						if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
						{
							// 비교 대상이 가로 일경우 Cur Pixel 이 100 Ref1 60, Ref2 60 일경우 Ref 의 상하 1Pixel 값이 60 보다 작고 Cur 의 상하 1Pixel 이 100 보다 작으면 False 로 간주하여
							// 제거한다.
							__m256i xCurUp, xCurDown;
							__m256i xRef1Up, xRef1Down;
							__m256i xRef2Up, xRef2Down;

							xCurUp = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - RPOCESS_WIDTH_32)));
							xCurDown = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + RPOCESS_WIDTH_32)));

							if (x * SSE_PROCESS_PIXELS - iDistH < left)
							{
								if (x * SSE_PROCESS_PIXELS + iDistH * 2 < right)
								{
									xRef1Up = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - RPOCESS_WIDTH_32) + 1 * iDistH));
									xRef1Down = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + RPOCESS_WIDTH_32) + 1 * iDistH));
									xRef2Up = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - RPOCESS_WIDTH_32) + 2 * iDistH));
									xRef2Down = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + RPOCESS_WIDTH_32) + 2 * iDistH));
								}
							}
							else if (x * SSE_PROCESS_PIXELS + iDistH > right)
							{
								if (x * SSE_PROCESS_PIXELS - iDistH * 2 > left)
								{
									xRef1Up = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - RPOCESS_WIDTH_32) - 1 * iDistH));
									xRef1Down = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + RPOCESS_WIDTH_32) - 1 * iDistH));
									xRef2Up = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - RPOCESS_WIDTH_32) - 2 * iDistH));
									xRef2Down = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + RPOCESS_WIDTH_32) - 2 * iDistH));
								}
							}
							else
							{
								if ((x * SSE_PROCESS_PIXELS - iDistH > left) && (x * SSE_PROCESS_PIXELS + iDistH < right))
								{
									xRef1Up = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - RPOCESS_WIDTH_32) - 1 * iDistH));
									xRef1Down = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + RPOCESS_WIDTH_32) - 1 * iDistH));
									xRef2Up = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - RPOCESS_WIDTH_32) + 1 * iDistH));
									xRef2Down = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + RPOCESS_WIDTH_32) + 1 * iDistH));
								}
							}

							__m256i xCurDiff = _mm256_min_epi8(_mm256_subs_epu8(xImg, xCurUp), _mm256_subs_epu8(xImg, xCurDown));

							__m256i xRef1Diff = _mm256_min_epi8(_mm256_subs_epu8(xLeft, xRef1Up), _mm256_subs_epu8(xLeft, xRef1Down));

							__m256i xRef2Diff = _mm256_min_epi8(_mm256_subs_epu8(xRight, xRef2Up), _mm256_subs_epu8(xRight, xRef2Down));

							xCurDiff = _mm256_sad_epu8(_mm256_subs_epu8(xCurDiff, xThreshold_UseGray), xZeros);
							xRef1Diff = _mm256_sad_epu8(_mm256_subs_epu8(xRef1Diff, xThreshold_UseGray), xZeros);
							xRef2Diff = _mm256_sad_epu8(_mm256_subs_epu8(xRef2Diff, xThreshold_UseGray), xZeros);

							if (xCurDiff.m256i_u16[0] + xCurDiff.m256i_u16[4] &&
								xRef1Diff.m256i_u16[0] + xRef1Diff.m256i_u16[4] &&
								xRef2Diff.m256i_u16[0] + xRef2Diff.m256i_u16[4])
							{
								xResult = xZeros;
							}
						}
					}
					else
					{
						if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
						{
							__m256i xCurLeft, xCurRight;
							__m256i xRef1Left, xRef1Right;
							__m256i xRef2Left, xRef2Right;

							xCurLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - 1)));
							xCurRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + 1)));
							xRef1Left = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - 1) - 1 * iDistH));
							xRef1Right = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + 1) - 1 * iDistH));
							xRef2Left = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp - 1) + 1 * iDistH));
							xRef2Right = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pInsp + 1) + 1 * iDistH));

							__m256i xCurDiff = _mm256_min_epi8(_mm256_subs_epu8(xImg, xCurLeft), _mm256_subs_epu8(xImg, xCurRight));

							__m256i xRef1Diff = _mm256_min_epi8(_mm256_subs_epu8(xLeft, xRef1Left), _mm256_subs_epu8(xLeft, xRef1Right));

							__m256i xRef2Diff = _mm256_min_epi8(_mm256_subs_epu8(xRight, xRef2Left), _mm256_subs_epu8(xRight, xRef2Right));

							xCurDiff = _mm256_sad_epu8(_mm256_subs_epu8(xCurDiff, xThreshold_UseGray), xZeros);
							xRef1Diff = _mm256_sad_epu8(_mm256_subs_epu8(xRef1Diff, xThreshold_UseGray), xZeros);
							xRef2Diff = _mm256_sad_epu8(_mm256_subs_epu8(xRef2Diff, xThreshold_UseGray), xZeros);

							if (xCurDiff.m256i_u16[0] + xCurDiff.m256i_u16[4] &&
								xRef1Diff.m256i_u16[0] + xRef1Diff.m256i_u16[4] &&
								xRef2Diff.m256i_u16[0] + xRef2Diff.m256i_u16[4])
							{
								xResult = xZeros;
							}
						}
					}	
				}

				if (xResult.m256i_u16[0] + xResult.m256i_u16[4] + xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4] + xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
				{
					__m256i xResultPos, xResultLowPos, xResultHighPos;
					xResultLowPos = xResultHighPos = xResultPos = xZeros;

					if (bMultiTh)
					{
						if (xResultHigh.m256i_u16[0] + xResultHigh.m256i_u16[4])
						{
							xResultHighPos = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayHigh, xDifferenceHigh), xMask), xMask);
						}
						if (xResultLow.m256i_u16[0] + xResultLow.m256i_u16[4])
						{
							xResultLowPos = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGrayLow, xDifferenceLow), xMask), xMask);
						}
					}
					else
					{
						if (xResult.m256i_u16[0] + xResult.m256i_u16[4])
						{
							xResultPos = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGray, xDifference), xMask), xMask);
						}
					}
					xResultPos = _mm256_or_si256(_mm256_or_si256(xResultHighPos, xResultLowPos), xResultPos);

					int iDefectCnt = 0;

					for (int k = 0; k < SSE_PROCESS_PIXELS; k++)
					{
						if (xResultPos.m256i_u8[k])
						{
							if (ASGSlopeFilter(reinterpret_cast<BYTE*>(CurrentFrame), x * 16 + k, y, iDistH, iDistV, left, right, top, bottom))
							{
								xResultPos.m256i_u8[k] = 0;
							}

						//	PADFilter(int iFrameNo, int PosX, int PosY, POINT *pt, int ThL, int ThH)
							POINT pt[10];
							
							for (int i = 0; i < 10; i++)
							{
								pt[i].x = iDistH;
								pt[i].y = iDistV;
							}

							if (PADFilter(reinterpret_cast<BYTE*>(CurrentFrame), x * 16 + k, y, pt, iTh_L, iTh_H))
							{
								xResultPos.m256i_u8[k] = 0;
							}

							if (m_Param.m_bTrackingFilter)
							{
								if (xResultPos.m256i_u8[k] && TrackingHoriFilter(x * SSE_PROCESS_PIXELS + k, y, m_Param.m_iTrackingRange, m_Param.m_iTrackingTh))
									xResultPos.m256i_u8[k] = 0;
								if (xResultPos.m256i_u8[k] && TrackingVertFilter(x * SSE_PROCESS_PIXELS + k, y, m_Param.m_iTrackingRange, m_Param.m_iTrackingTh))
									xResultPos.m256i_u8[k] = 0;
								if (xResultPos.m256i_u8[k] && TrackingLT2RBFilter(x * SSE_PROCESS_PIXELS + k, y, m_Param.m_iTrackingRange, m_Param.m_iTrackingTh))
									xResultPos.m256i_u8[k] = 0;
								if (xResultPos.m256i_u8[k] && TrackingRT2LBFilter(x * SSE_PROCESS_PIXELS + k, y, m_Param.m_iTrackingRange, m_Param.m_iTrackingTh))
									xResultPos.m256i_u8[k] = 0;
							}
							if (m_Param.m_bNearbyCheckFilter && iDistV == 0 && xResultPos.m256i_u8[k])
							{
								// iShiftY = y방향 ROI 내부로 이끄는 값 계산
								if (y - 16 <= top)			iShiftY = y - top;
								else if (y + 15 >= bottom)	iShiftY = bottom - y;
								else						iShiftY = 16;

								// x 방향 ROI 내부로 이끄는 값 계산
								if (x * SSE_PROCESS_PIXELS - iDistH < left)
								{
									iLeftDist = iDistH;
									iRightDist = 2 * iDistH;
								}
								else if (x * SSE_PROCESS_PIXELS + iDistH > right)
								{
									iLeftDist = (-1) * iDistH;
									iRightDist = (-2) * iDistH;
								}
								else
								{
									iLeftDist = (-1) * iDistH;
									iRightDist = iDistH;
								}

								// Summary 및 분류
								iDifferenceUpUp = iDifferenceUpDown = iDifferenceDownUp = iDifferenceDownDown = 0;
								nTh = xThreshold_UseGray.m256i_u8[k];
								unsigned char* pFrame = reinterpret_cast<unsigned char*>(m_pImage);
								for (int iIndex = 0; iIndex < SSE_PROCESS_PIXELS; iIndex++)
								{
									iCurGray = pFrame[(y - iShiftY + iIndex)* CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k];
									iDifferenceUpUp += abs(pFrame[(y - iShiftY + iIndex)* CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k + iLeftDist - 1] - iCurGray);
									iDifferenceUpDown += abs(pFrame[(y - iShiftY + iIndex)* CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k + iLeftDist + 1] - iCurGray);
									iDifferenceDownUp += abs(pFrame[(y - iShiftY + iIndex)* CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k + iRightDist - 1] - iCurGray);
									iDifferenceDownDown += abs(pFrame[(y - iShiftY + iIndex)* CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k + iRightDist + 1] - iCurGray);
								}
								iCurGray = pFrame[y* CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k];
								if (iDifferenceUpUp > iDifferenceUpDown)
									nShiftLeftGray = pFrame[y * CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k + iLeftDist + 1];
								else
									nShiftLeftGray = pFrame[y * CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k + iLeftDist - 1];
								if (iDifferenceDownUp > iDifferenceDownDown)
									nShiftRightGray = pFrame[y * CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k + iRightDist + 1];
								else
									nShiftRightGray = pFrame[y * CAMERA_CCD_SIZE + x * SSE_PROCESS_PIXELS + k + iRightDist - 1];

								// 평가
								if (abs(nShiftLeftGray - iCurGray) <= nTh || abs(nShiftRightGray - iCurGray) <= nTh)
									xResultPos.m256i_u8[k] = 0;
							}
							if (xResultPos.m256i_u8[k])
							{
								iDefectCnt++;
							}
						}
					}
					if (iDefectCnt != 0)
					{
						m_pPixelResult[iCnt].x = x;
						m_pPixelResult[iCnt].y = y;
						m_pPixelResult[iCnt].POS = xResultPos;
						m_pPixelResult[iCnt].DefectGray = xImg;
						m_pPixelResult[iCnt].ReferenceGray = xCompare1;

						iCnt++;

						if (iCnt >= MAX_DEFECT_PIXELCNT)
							return iCnt;
					}
				}
			}
			pInsp++;

			if (iDistH == 0)
			{
				pUp++;
				pDown++;
				pUpUp++;
				pUpDown++;
				pDownUp++;
				pDownDown++;
			}
		}
	}
	return iCnt;
}

BOOL _PADFiltering(int *c, int *d, int ThresL, int ThresH)
{
	int thresh, count, diff, k, slope, maxerr, diff2;
	int e[5], f[5];
	double shift, sumdiff, sumshift, weight;

	thresh = static_cast<int>(ThresL + ((ThresH - ThresL)*d[2]) / 255.0);

	if (abs(c[3] - c[2]) > abs(c[2] - c[1])) slope = (c[3] - c[2]);
	else slope = (c[2] - c[1]);

	count = 0;
	diff = 0;
	for (k = 1; k <= 3; k++)
	{
		if (255 != c[k] && 255 != d[k])
		{
			diff += (d[k] - c[k]);
			count++;
		}
	}

	if (count) diff /= count;
	else diff = (d[2] - c[2]);

	shift = static_cast<float>(diff) / slope;

	sumdiff = 0;
	sumshift = 0;

	if (shift < 0)
	{
		for (k = 1; k <= 3; k++)
		{
			diff = d[k] - c[k];
			slope = (c[k] - c[k - 1]);

			if (slope == 0) continue;

			shift = static_cast<float>(diff) / slope;
			if (255 == c[k] || 255 == c[k - 1] || 255 == d[k]) continue;

			if (shift < -1.0) diff = -slope;
			else if (shift > 1.0) diff = slope;

			if (slope < 0)
			{
				sumshift -= diff;
				sumdiff -= slope;
			}
			else
			{
				sumshift += diff;
				sumdiff += slope;
			}
		}
		if (0 != sumdiff) sumshift /= sumdiff;
		else sumshift = 0;
	}
	else
	{
		for (k = 1; k <= 3; k++)
		{
			diff = d[k] - c[k];
			slope = (c[k + 1] - c[k]);

			if (0 == slope) continue;

			shift = static_cast<float>(diff) / slope;
			if (255 == c[k] || 255 == c[k + 1] || 255 == d[k]) continue;

			if (shift < -1.0) diff = -slope;
			else if (shift > 1.0) diff = slope;

			if (slope < 0)
			{
				sumshift -= diff;
				sumdiff -= slope;
			}
			else
			{
				sumshift += diff;
				sumdiff += slope;
			}
		}
		if (0 != sumdiff) sumshift /= sumdiff;
		else sumshift = 0;
	}

	shift = sumshift;


	if (fabs(static_cast<double>(shift)) > 1.0) return FALSE;

	weight = fabs(static_cast<double>(shift));
	if (shift < 0)
	{
		for (k = 1; k <= 3; k++)
		{
			e[k] = static_cast<int>((1 - weight)*c[k] + weight * c[k - 1]);
			if (255 == c[k] || 255 == c[k - 1]) e[k] = 0;
		}
	}
	else
	{
		for (k = 1; k <= 3; k++)
		{
			e[k] = static_cast<int>((1 - weight)*c[k] + weight * c[k + 1]);
			if (255 == c[k] || 255 == c[k + 1]) e[k] = 0;
		}
	}
	shift = -shift;
	weight = fabs(static_cast<double>(shift));

	if (shift < 0)
	{
		for (k = 1; k <= 3; k++)
		{
			f[k] = static_cast<int>((1 - weight)*d[k] + weight * d[k - 1]);
			if (255 == d[k] || 255 == d[k - 1]) f[k] = 0;
		}
	}
	else
	{
		for (k = 1; k <= 3; k++)
		{
			f[k] = static_cast<int>((1 - weight)*d[k] + weight * d[k + 1]);
			if (255 == d[k] || 255 == d[k + 1]) f[k] = 0;
		}
	}
	maxerr = 0;
	for (k = 1; k <= 3; k++)
	{
		if (255 != d[k] && 0 != e[k]) diff = abs(d[k] - e[k]);
		else diff = 0;

		if (255 != c[k] && 0 != f[k]) diff2 = abs(c[k] - f[k]);
		else diff2 = 0;

		if (diff2 < diff) diff = diff2;
		if (diff > maxerr) maxerr = diff;
	}

	if (maxerr < thresh) return TRUE;
	else return FALSE;
}

BOOL CDetectionAlgorithm::PADFilter(BYTE* _pImg, int PosX, int PosY, POINT *pt, int ThL, int ThH)
{

	int i;
	int xdiff;
	int ydiff;
	BYTE *pImg;
	BOOL bHor;

	int Def[5];
	int Ref[5];

	//if (PosX < 2 || PosX > 8190)
	if (PosX < 2 || PosX > m_iImageWidth - 2)
	{
		xdiff = 0;
	}
	else
	{
		pImg = _pImg;
		xdiff = abs(static_cast<int>(pImg[PosY*m_iImageWidth + PosX - 1]) - static_cast<int>(pImg[PosY*m_iImageWidth + PosX + 1]));
	}

// 	if (PosY == 0)
// 	{
// 		pImg = pFrameImg[iFrameNo - 1];
// 		ydiff = static_cast<int>(pImg[(FRAME_HEIGHT - 1)*CAMERA_CCD_SIZE + PosX]);
// 		pImg = pFrameImg[iFrameNo];
// 		ydiff = abs(ydiff - static_cast<int>(pImg[CAMERA_CCD_SIZE + PosX]));
// 	}
// 	else if (PosY == FRAME_HEIGHT - 1)
// 	{
// 		pImg = pFrameImg[iFrameNo];
// 		ydiff = static_cast<int>(pImg[(PosY - 1)*CAMERA_CCD_SIZE + PosX]);
// 		pImg = pFrameImg[iFrameNo + 1];
// 		ydiff = abs(ydiff - static_cast<int>(pImg[PosX]));
// 	}
// 	else
// 	{
//		pImg = pFrameImg[iFrameNo];
		ydiff = abs(static_cast<int>(pImg[(PosY - 1)*m_iImageWidth + PosX]) - static_cast<int>(pImg[(PosY + 1)*m_iImageWidth + PosX]));
//	}

	if (xdiff > ydiff) bHor = TRUE;
	else bHor = FALSE;

	if (!GetGray(pImg, PosX, PosY, Def, bHor)) return FALSE;
	if (abs(Def[2] - Def[1]) < 5 && abs(Def[3] - Def[2]) < 5) return FALSE;

	for (i = 0; i < 10; i++)
	{
		if (!GetGray(pImg, PosX + pt[i].x, PosY + pt[i].y, Ref, bHor)) continue;
		if (_PADFiltering(Def, Ref, ThL, ThH)) return TRUE;
	}
	return FALSE;
}


BOOL CDetectionAlgorithm::GetGray(BYTE* _pImg, int x, int y, int *gray, BOOL bIsHor)
{
	BYTE *pImg;

	int Line, Frame;

//	Line = iFrameNo * FRAME_HEIGHT + y;
//	Frame = static_cast<int>(Line / FRAME_HEIGHT);
//	y = Line - (Frame * FRAME_HEIGHT);

	if (bIsHor) {
		if (x < 2 || x > m_iImageWidth - 2) return FALSE;

		pImg = _pImg;		if (NULL == pImg) return FALSE;
		gray[0] = pImg[y*m_iImageWidth + x - 2];
		gray[1] = pImg[y*m_iImageWidth + x - 1];
		gray[2] = pImg[y*m_iImageWidth + x];
		gray[3] = pImg[y*m_iImageWidth + x + 1];
		gray[4] = pImg[y*m_iImageWidth + x + 2];
	}
	else {
		if (y > 2 && y < m_iImageWidth-2)
		{
			pImg = _pImg;		if (NULL == pImg) return FALSE;
			gray[0] = pImg[(y - 2)*m_iImageWidth + x];
			gray[1] = pImg[(y - 1)*m_iImageWidth + x];
			gray[2] = pImg[(y)*m_iImageWidth + x];
			gray[3] = pImg[(y - 1)*m_iImageWidth + x];
			gray[4] = pImg[(y - 2)*m_iImageWidth + x];
		}
	}
	return TRUE;
}


int CDetectionAlgorithm::NonPeriodicInspection()
{
	if(m_pImage == NULL)
		return 0;

	int iCnt = 0;

	// Load Golden Template Image Start

	CString strGoldenImage;
	strGoldenImage.Format("%s", "D:\\Test Image\\NonPeriodic Inspection\\2\\GoldenImage1.bmp");
	IplImage* pGoldenImage = cvLoadImage(strGoldenImage, CV_LOAD_IMAGE_GRAYSCALE);
	if(pGoldenImage == NULL)
		return 0;

	CString str;
	int iGWidth = pGoldenImage->width, iGHeight = pGoldenImage->height;
	int iSearchWidth = iGWidth/2, iSearchHeight = iGHeight/2;
	CRect rtSearch, rtROI, rtInsp;;
	rtROI = m_Param.m_rtROI;
	rtSearch = m_Param.m_rtROI;
	rtSearch.left -= iSearchWidth;
	rtSearch.top -= iSearchHeight;
	rtSearch.right += iSearchWidth;
	rtSearch.bottom += iSearchHeight;

	if(rtSearch.left < 0) rtSearch.left = 0;
	if(rtSearch.right > m_iImageWidth) rtSearch.right = m_iImageWidth;
	if(rtSearch.top < 0) rtSearch.top = 0;
	if(rtSearch.bottom > m_iImageHeight) rtSearch.bottom = m_iImageHeight;
		
	IplImage* pOriginImage = cvCreateImage(cvSize(rtSearch.Width() + (iSearchWidth*2), rtSearch.Height() + (iSearchHeight*2)), IPL_DEPTH_8U, 1);
	if(pOriginImage == NULL)
		return 0;
	
	for(int v = 0; v < rtSearch.Height() + (iSearchHeight*2); v++)
	{
		memcpy(&pOriginImage->imageData[v*pOriginImage->widthStep], &m_pImage[(rtSearch.top+v)*m_iImageWidth + rtSearch.left], rtSearch.Width());
	}

	double min = 0.0, max = 0.0;
	CvPoint ptFindPos = cvPoint(0, 0);

	int search_width = pOriginImage->width - pGoldenImage->width + 1;
	int search_height = pOriginImage->height - pGoldenImage->height + 1;

	IplImage* pCoeffImage = cvCreateImage(cvSize(search_width, search_height), IPL_DEPTH_32F, 1);	

	cvMatchTemplate(pOriginImage, pGoldenImage, pCoeffImage, CV_TM_CCOEFF_NORMED);
	cvMinMaxLoc(pCoeffImage, &min, &max, NULL, &ptFindPos);

	IplImage* pResultImage = cvCreateImage(cvSize(iGWidth, iGHeight), IPL_DEPTH_8U, 1);
	cvSetImageROI(pOriginImage, cvRect(ptFindPos.x, ptFindPos.y, iGWidth, iGHeight));
	cvCopyImage(pOriginImage, pResultImage);
	cvResetImageROI(pOriginImage);

	if(max < 0.3)
	{
		str.Format("Low Matching Rate : %.3lf", max);
		AfxMessageBox(str);
		goto SKIP;
	}

	//cvDrawRect(pOriginImage, cvPoint(ptFindPos.x, ptFindPos.y), cvPoint(ptFindPos.x+iGWidth, ptFindPos.y+iGHeight), cvScalar(255));
	cvDrawLine(pOriginImage, cvPoint(ptFindPos.x - 10, ptFindPos.y), cvPoint(ptFindPos.x + 10, ptFindPos.y), cvScalar(255));
	cvDrawLine(pOriginImage, cvPoint(ptFindPos.x, ptFindPos.y - 10), cvPoint(ptFindPos.x, ptFindPos.y + 10), cvScalar(255));
	cvSaveImage("D:\\Test Image\\roi.bmp", pOriginImage);
	//cvSaveImage("D:\\Test Image\\coff.bmp", pCoeffImage);
	cvSaveImage("D:\\Test Image\\roi.bmp", pResultImage);
		
	//int iSearchPosX = rtSearch.left + ptFindPos.x;
	//int iSearchPosY = rtSearch.top + ptFindPos.y;

	rtInsp.left = rtSearch.left + ptFindPos.x;
	rtInsp.top = rtSearch.top + ptFindPos.y;
	rtInsp.right = rtInsp.left + iGWidth;
	rtInsp.bottom = rtInsp.top + iGHeight;

	if(rtInsp.left < 0) rtInsp.left = 0;
	if(rtInsp.top < 0) rtInsp.top = 0;
	if(rtInsp.right > rtROI.right) rtInsp.right = rtROI.right;
	if(rtInsp.bottom > rtROI.bottom) rtInsp.bottom = rtROI.bottom;

	int x = 0, y = 0;
	int iWidth = rtInsp.Width() >> 4;
	int iHeight = rtInsp.Height();
	int RPOCESS_WIDTH_32 = pResultImage->widthStep/16;
	int GOLDEN_RPOCESS_WIDTH_32 = pGoldenImage->widthStep/16;
	int iThL = m_Param.m_iThresholdLow;
	int iThH = m_Param.m_iThresholdHigh;

	int iGoldenStartX = iGWidth - rtInsp.Width();
	int iGoldenStartY = iGHeight - rtInsp.Height();

	__m256i *CurrentFrame = reinterpret_cast<__m256i*>(m_pImage);
	__m256i *GoldenFrame = reinterpret_cast<__m256i*>(pGoldenImage->imageData);
	__m256i *_pInsp = NULL, *_pGolden = NULL;
	__m256i *pInsp = NULL, *pGolden = NULL;
	__m256i xImg, xGolden, xDifference, xResult, xZeros, xOne, xMask, xTemp;
	__m256i xThreshold, xThresholdH, xThreshold_UseGray, xSlope;

	xZeros = _mm256_set1_epi8(static_cast<char>(0));
	xOne   = _mm256_set1_epi8(static_cast<char>(1));
	xMask = _mm256_set1_epi8(0xFF);

	xSlope = _mm256_set1_epi16(static_cast<INT16>(iThH + 1 - iThL));
	xThreshold = _mm256_set1_epi8(static_cast<char>(iThL));
	xThresholdH = _mm256_set1_epi8(static_cast<char>(iThH));

	xSlope	= _mm256_subs_epi16(xThresholdH, xThreshold);
	xSlope	= _mm256_adds_epi16(xSlope, xOne);
	
	//_pInsp = CurrentFrame + ((top - 1) * m_iImageWidth + left) / 16 ;
	_pInsp = CurrentFrame + ((rtInsp.top-1) * pResultImage->widthStep + rtInsp.left) / 16;
	_pGolden = GoldenFrame + ((iGoldenStartY-1) * pGoldenImage->widthStep + iGoldenStartX) / 16;

	for (y = 0; y < iHeight; ++y)
	{
		pInsp = _pInsp += RPOCESS_WIDTH_32;
		pGolden = _pGolden += GOLDEN_RPOCESS_WIDTH_32;

		for (x = 0; x < iWidth; ++x) 
		{
			xImg = _mm256_load_si256(pInsp);
			xGolden = _mm256_load_si256(pGolden);

			//xDifference = _mm256_max_epu8(_mm256_subs_epu8(xImg, xGolden), _mm256_subs_epu8(xGolden, xImg));
			xDifference = _mm256_subs_epu8(xImg, xGolden);
			
			xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros) ;

			if(xResult.m256i_u16[0] + xResult.m256i_u16[4])
			{
				xTemp = _mm256_packus_epi16(
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpacklo_epi8(xImg, xZeros), 
					_mm256_unpacklo_epi8(xSlope, xZeros)), 8), 
					_mm256_srai_epi16(_mm256_mullo_epi16(_mm256_unpackhi_epi8(xImg, xZeros), 
					_mm256_unpacklo_epi8(xSlope, xZeros)), 8));

				xThreshold_UseGray  = _mm256_adds_epi8(xThreshold, xTemp) ;

				xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold_UseGray), xZeros) ;

				if(xResult.m256i_u16[0] + xResult.m256i_u16[4])
				{
					xResult = _mm256_cmpeq_epi8(_mm256_andnot_si256(_mm256_subs_epu8(xThreshold_UseGray, xDifference), xMask), xMask);

					m_pPixelResult[iCnt].x = x + (rtInsp.left >> 4);
					m_pPixelResult[iCnt].y = y + rtInsp.top ;
					m_pPixelResult[iCnt].POS = xResult;
					m_pPixelResult[iCnt].DefectGray = xImg;
					m_pPixelResult[iCnt].ReferenceGray = xGolden;

					iCnt++;

					if (iCnt >= MAX_DEFECT_PIXELCNT) 
						return iCnt ;
				}
			}

			pInsp++;
			pGolden++;
		}
	}

SKIP:
	cvReleaseImage(&pResultImage);
	cvReleaseImage(&pGoldenImage);
	cvReleaseImage(&pCoeffImage);
	cvReleaseImage(&pOriginImage);	

	// Load Golden Template Image End

	return iCnt;
}

int	CDetectionAlgorithm::EdgeInspection(BYTE *CurrentFrame, int left, int right, int top, int bottom, int offset, int th, int thH, DefectPos *pPixelResult, int iMAXCnt, int side, BOOL bSaveEdgeCrackImage)
{
	if(side == -1)
		return 0;

	int iCnt = 0;

	IplImage* pIplImage = NULL;
	IplImage* pIplResult = NULL;

	int CAMERA_CCD_SIZE = m_iImageWidth, FRAME_HEIGHT = m_iImageHeight;

	__m256i xResult;

	if (left < 0 || left >= CAMERA_CCD_SIZE)	return 0;
	if (right <= 0 || right > CAMERA_CCD_SIZE)	return 0;
	if (top < 0 || top >= FRAME_HEIGHT)			return 0;
	if (bottom <= 0 || bottom > FRAME_HEIGHT)	return 0;
	if (right - left <= 0 )						return 0;
	if ( bottom - top <= 0 )					return 0;

	int width, height;

	width	= right - left;
	height	= bottom - top;

	if (width < offset || height < offset)
		return 0;

	int iLineteratecnt = ((right - left) >> 4);

	width = iLineteratecnt*16;

	int xOffset;
	int yOffset;
	xOffset = left >> 4;
	xOffset += 1;
	yOffset = top;

	int margin = offset;

	pIplImage  = cvCreateImage(cvSize(width, height), 8, 1);
	if (pIplImage == NULL)
	{
		return 0;
	}
	pIplResult = cvCreateImage(cvSize(width, height), 8, 1);
	if (pIplResult == NULL)
	{
		return 0;
	}

	CvMemStorage* contour_storage = cvCreateMemStorage(0);
	CvSeq* contour = 0; 
	CvSeq* max_contour = 0;

	for(int v = 0; v < height; v++)
	{
		CopyMemory(&(pIplImage->imageData[v * pIplImage->widthStep]), &(CurrentFrame[((top+v) * CAMERA_CCD_SIZE) + left]), width) ;		
	}

	cvThreshold(pIplImage, pIplResult, th, 255, CV_THRESH_BINARY_INV );
	cvSaveImage("D:\\DDMDemo\\EdgeImage\\binary.bmp", pIplResult);

	int count;
	count = cvFindContours(pIplResult, contour_storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	CvScalar inColor = CV_RGB(255, 255, 255);
	CvScalar outColor = CV_RGB(0, 0, 0);

	int max_total = 0;
	for(int c = 0; c < count; c++)
	{
		if(max_total < contour->total)
		{
			max_total = contour->total;
			max_contour = contour;
		}
		contour = contour->h_next;
	}
	cvRectangle(pIplResult, cvPoint(0, 0), cvPoint(pIplResult->width-1, pIplResult->height-1), outColor, CV_FILLED);

	if(count > 0 && max_total > 100)
	{
		cvDrawContours(pIplResult, max_contour, inColor, outColor, -1, CV_FILLED, 8);
	}
	else cvRectangle(pIplResult, cvPoint(0, 0), cvPoint(pIplResult->width-1, pIplResult->height-1), outColor, CV_FILLED);

	int edge_1;
	int edge_2;
	BOOL find_edge_1;
	BOOL find_edge_2;

	int top_offset    = 0;
	int bottom_offset = height;
	int left_offset   = 0;
	int right_offset  = width;

	CString strSide;

	if(side == 0) // top
	{
		strSide = "TOP";
		find_edge_1  = FALSE;
		find_edge_2  = FALSE;

		edge_1 = edge_2 = 0;

		for(int v = 2; v < height-2; v++)
		{
			if(pIplResult->imageData[v*width+3] == 0 && pIplResult->imageData[v*width+4] == 0 && !find_edge_1)
			{
				find_edge_1 = TRUE;
				edge_1 = v;
			}
			if(pIplResult->imageData[v*width+width-3] == 0 && pIplResult->imageData[v*width+width-4] == 0 && !find_edge_2)
			{
				find_edge_2 = TRUE;
				edge_2 = v;
			}
			if(find_edge_1&&find_edge_2) break;
		}

		if(edge_1 > edge_2)
			top_offset = edge_2;
		else top_offset = edge_1;

		cvLine(pIplImage, cvPoint(0, top_offset), cvPoint(width-1, top_offset), inColor);

		top_offset += margin;	

		if(top_offset > height-1)	top_offset = height-1;

		cvLine(pIplImage, cvPoint(0, top_offset), cvPoint(width-1, top_offset), inColor);

		if((pIplResult->imageData[2*width+3] != 0 && pIplResult->imageData[2*width+4] != 0) && 
			(pIplResult->imageData[(height-2)*width+width-3] != 0 && pIplResult->imageData[(height-2)*width+width-4] != 0))
		{
			cvRectangle(pIplResult, cvPoint(0, 0), cvPoint(pIplResult->width-1, pIplResult->height-1), outColor, CV_FILLED);
		}
		cvRectangle(pIplResult, cvPoint(0, 0), cvPoint(pIplResult->width-1, top_offset), outColor, CV_FILLED);
	}

	if(side == 1) // left
	{
		strSide = "LEFT";

		find_edge_1  = FALSE;
		find_edge_2  = FALSE;

		edge_1 = edge_2 = 0;

		for(int h = 1; h < width-1; h++)
		{
			if(pIplResult->imageData[2*width+h] == 0 && pIplResult->imageData[3*width+h] == 0 && !find_edge_1)
			{
				find_edge_1 = TRUE;
				edge_1 = h;
			}
			if(pIplResult->imageData[(height-2)*width+h] == 0 && pIplResult->imageData[(height-3)*width+h] == 0 && !find_edge_2)
			{
				find_edge_2 = TRUE;
				edge_2 = h;
			}		
			if(find_edge_1&&find_edge_2) break;
		}

		if(edge_1>edge_2)
			left_offset = edge_2;
		else left_offset = edge_1;

		cvLine(pIplImage, cvPoint(left_offset, 0), cvPoint(left_offset, height-1), inColor);

		left_offset += margin;
		if(left_offset > width-1)	left_offset = width-1;

		cvLine(pIplImage, cvPoint(left_offset, 0), cvPoint(left_offset, height-1), inColor);		

		if((pIplResult->imageData[2*width+3] != 0 && pIplResult->imageData[2*width+4] != 0) && 
			(pIplResult->imageData[(height-2)*width+width-3] != 0 && pIplResult->imageData[(height-2)*width+width-4] != 0))
		{
			cvRectangle(pIplResult, cvPoint(0, 0), cvPoint(pIplResult->width-1, pIplResult->height-1), outColor, CV_FILLED);
		}
		cvRectangle(pIplResult, cvPoint(0, 0), cvPoint(left_offset, pIplResult->height-1), outColor, CV_FILLED);
	}

	if(side == 2)  // bottom
	{
		strSide = "BOTTOM";

		find_edge_1  = FALSE;
		find_edge_2  = FALSE;

		edge_1 = edge_2 = pIplResult->height;

		for(int v = 1; v < height-1; v++)
		{
			if(pIplResult->imageData[v*width+3] != 0 && pIplResult->imageData[v*width+4] != 0 && !find_edge_1)
			{
				find_edge_1 = TRUE;
				edge_1 = v;
			}
			if(pIplResult->imageData[v*width+width-3] != 0 && pIplResult->imageData[v*width+width-4] != 0 && !find_edge_2)
			{
				find_edge_2 = TRUE;
				edge_2 = v;
			}
			if(find_edge_1&&find_edge_2) break;			
		}	

		if(edge_1 > edge_2)
			bottom_offset = edge_1;
		else bottom_offset = edge_2;

		cvLine(pIplImage, cvPoint(0, bottom_offset), cvPoint(width-1, bottom_offset), inColor);

		bottom_offset -= margin;		
		if(bottom_offset < 0) bottom_offset = 0;

		cvLine(pIplImage, cvPoint(0, bottom_offset), cvPoint(width-1, bottom_offset), inColor);

		if((pIplResult->imageData[2*width+3] != 0 && pIplResult->imageData[2*width+4] != 0) && 
			(pIplResult->imageData[(height-2)*width+width-3] != 0 && pIplResult->imageData[(height-2)*width+width-4] != 0))
		{
			cvRectangle(pIplResult, cvPoint(0, 0), cvPoint(pIplResult->width-1, pIplResult->height-1), outColor, CV_FILLED);
		}
		cvRectangle(pIplResult, cvPoint(0, bottom_offset), cvPoint(pIplResult->width-1, pIplResult->height-1), outColor, CV_FILLED);		
	}
	if(side == 3) // right
	{
		strSide = "RIGHT";

		find_edge_1  = FALSE;
		find_edge_2  = FALSE;

		edge_1 = edge_2 = width;

		for(int h = 1; h < width-1; h++)
		{
			if(pIplResult->imageData[2*width+h] != 0 && pIplResult->imageData[3*width+h] != 0 && !find_edge_1)
			{
				find_edge_1 = TRUE;
				edge_1 = h;
			}
			if(pIplResult->imageData[(height-2)*width+h] != 0 && pIplResult->imageData[(height-3)*width+h] != 0 && !find_edge_2)
			{
				find_edge_2 = TRUE;
				edge_2 = h;
			}
			if(find_edge_1&&find_edge_2) break;	
		}

		if(edge_1 > edge_2)
			right_offset = edge_1;
		else right_offset = edge_2;

		cvLine(pIplImage, cvPoint(right_offset, 0), cvPoint(right_offset, height-1), inColor);

		right_offset -= margin;
		if(right_offset < 0) right_offset = 0;

		cvLine(pIplImage, cvPoint(right_offset, 0), cvPoint(right_offset, height-1), inColor);

		if((pIplResult->imageData[2*width+3] != 0 && pIplResult->imageData[2*width+4] != 0) && 
			(pIplResult->imageData[(height-2)*width+width-3] != 0 && pIplResult->imageData[(height-2)*width+width-4] != 0))
		{
			cvRectangle(pIplResult, cvPoint(0, 0), cvPoint(pIplResult->width-1, pIplResult->height-1), outColor, CV_FILLED);
		}		
		cvRectangle(pIplResult, cvPoint(right_offset, 0), cvPoint(pIplResult->width-1, pIplResult->height-1), outColor, CV_FILLED);
	}
	__m256i *pInsp;
	__m256i *pInspBase;

	cvFindContours(pIplResult, contour_storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	cvRectangle(pIplResult, cvPoint(0, 0), cvPoint(pIplResult->width-1, pIplResult->height-1), outColor, CV_FILLED);

	CString file;

	cvDrawContours(pIplResult, contour, inColor, outColor, 1, 1, 8);

	pInsp		= reinterpret_cast<__m256i*>(pIplResult->imageData);

	BOOL bFilter = FALSE;

	__m256i xImg;
	__m256i xTemp1;
	__m256i xTemp2;
	__m256i xThreshold;
	__m256i xDifference;
	__m256i xResultHor;
	__m256i xZeros;

	xThreshold = _mm256_set1_epi8(static_cast<char>(thH)) ;
	xZeros	= _mm256_set1_epi8(static_cast<char>(0)) ;

	xResultHor = xZeros;

	if(side == 0 || side == 1 || side == 2 || side ==3)
	{
		for(int v = 0; v < height; v++)
		{
			for(int x = 0 ; x < iLineteratecnt; x++)
			{
				bFilter = TRUE;

				xResult = _mm256_load_si256(pInsp);

				if ((xResult.m256i_u64[0] + xResult.m256i_u64[1]) && (~xResult.m256i_u64[0] + ~xResult.m256i_u64[1]))
				{	
					if( (top_offset    +2)  < height &&
						(left_offset   +2)  < width  &&
						(bottom_offset -2)  > 0      &&
						(right_offset  -2)  > 0)
					{
						BYTE cMask[16];						

						memset(cMask, 0x00, sizeof(BYTE)*16);

						// 연결 영역 검색
						if(side == 0) // top
							memcpy(cMask, &pIplResult->imageData[(top_offset+1)*width+x*16],			sizeof(BYTE)*16);
						if(side == 1) // left
							memcpy(cMask, &pIplResult->imageData[v*width+(left_offset+1)],				sizeof(BYTE)*1);
						if(side == 2) // bottom
							memcpy(cMask, &pIplResult->imageData[(bottom_offset-1)*width+x*16],			sizeof(BYTE)*16);
						if(side == 3) // right
							memcpy(cMask, &pIplResult->imageData[v*pIplResult->width+(right_offset-1)], sizeof(BYTE)*1);

						pInspBase = reinterpret_cast<__m256i*>(cMask);

						xResult = _mm256_loadu_si256(pInspBase);

						xImg = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&pIplImage->imageData[v*width+x*16]));						

						if(side == 0 || side == 2) // top / bottom
						{
							xTemp1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&pIplImage->imageData[v*width]));
							xTemp2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&pIplImage->imageData[v*width+width - 16 - 1]));							
						}
						if(side == 1 || side == 3) // left // right
						{
							xTemp1 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&pIplImage->imageData[0*width+x*16]));
							xTemp2 = _mm256_loadu_si256(reinterpret_cast<__m256i*>(&pIplImage->imageData[(height-1)*width+x*16]));							
						}	
						xDifference = _mm256_max_epu8(_mm256_add_epi8(_mm256_subs_epu8(xImg, xTemp1), _mm256_subs_epu8(xTemp1, xImg)), _mm256_add_epi8(_mm256_subs_epu8(xImg, xTemp2), _mm256_subs_epu8(xTemp2, xImg)));
						xResultHor	= _mm256_sad_epu8(_mm256_subs_epu8(xDifference, xThreshold), xZeros) ;
					}
					if ((xResult.m256i_u64[0] + xResult.m256i_u64[1]) && (xResultHor.m256i_u16[0] + xResultHor.m256i_u16[4]))
					{
						pPixelResult[iCnt].x = x + xOffset;
						pPixelResult[iCnt].y = v + yOffset;

						pPixelResult[iCnt].POS = xResult;
						pPixelResult[iCnt].DefectGray = xResult;

						iCnt ++;

						if (iCnt > iMAXCnt) 
						{
							if(bSaveEdgeCrackImage)
							{
								cvDrawContours(pIplImage, contour, inColor, outColor, 1, 1, 8);

								CString strFolder;
								strFolder.Format("%s", "D:\\DDMDemo\\EdgeImage");
								CreateFolder(strFolder);

								file.Format("%s\\%d_%d_%d_%d_%s_%d_edge1_%d_%d_edge2_%d_%d_th_%d.bmp", strFolder, left_offset, right_offset, top_offset, bottom_offset, strSide, iCnt, find_edge_1, edge_1, find_edge_2, edge_2, th);
								cvSaveImage(file, pIplImage);

								file.Format("%s\\%d_%d_%d_%d_%s_%d_edge1_%d_%d_edge2_%d_%d_th_%d_result.bmp", strFolder, left_offset, right_offset, top_offset, bottom_offset, strSide, iCnt, find_edge_1, edge_1, find_edge_2, edge_2, th);
								cvSaveImage(file, pIplResult);
							}

							cvReleaseImage(&pIplImage);
							cvReleaseImage(&pIplResult);							
							cvReleaseMemStorage(&contour_storage);

							return iCnt;
						}
					}					
				}
				pInsp++;
			}
		}
	}

	if(bSaveEdgeCrackImage)
	{
		cvDrawContours(pIplImage, contour, inColor, outColor, 1, 1, 8);

		CString strFolder;
		strFolder.Format("%s", "D:\\DDMDemo\\EdgeImage");
		CreateFolder(strFolder);

		file.Format("%s\\%d_%d_%d_%d_%s_%d_edge1_%d_%d_edge2_%d_%d_th_%d.bmp", strFolder, left_offset, right_offset, top_offset, bottom_offset, strSide, iCnt, find_edge_1, edge_1, find_edge_2, edge_2, th);
		cvSaveImage(file, pIplImage);

		file.Format("%s\\%d_%d_%d_%d_%s_%d_edge1_%d_%d_edge2_%d_%d_th_%d_result.bmp", strFolder, left_offset, right_offset, top_offset, bottom_offset, strSide, iCnt, find_edge_1, edge_1, find_edge_2, edge_2, th);
		cvSaveImage(file, pIplResult);
	}

	cvReleaseImage(&pIplImage);
	cvReleaseImage(&pIplResult);
	cvReleaseMemStorage(&contour_storage);

	return iCnt;
}

BOOL CDetectionAlgorithm::CellSlopeFilter(BYTE* pImg, int PosX, int PosY, int left, int right, int top, int bottom, int iDistV, int iDistH, int ThL, int ThH)
{
	int CAMERA_CCD_SIZE = m_iImageWidth;
	int i=0;

	if (PosX < 2 || PosX > CAMERA_CCD_SIZE-2)
	{
		return TRUE;
	}

	int iDiff = 0;

	int iTh = 10;
	int iDist;

	int sub_gray1;
	int sub_gray2;

	iTh = (ThH - ThL) * (double)(unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] / 255 + ThL;

	iDist = iDistH;

	if(right - left > 3*iDist)
	{
		if(PosX + iDist >= right)
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - 2*iDist];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
		else if(PosX - iDist >= left)
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist] < 0)
					return TRUE;
			}
		}
		else
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + 2*iDist];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
	}

	iDist = iDistV;

	if(bottom - top > 3*iDist)
	{
		if(PosY + iDist >= bottom)
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - iDist) * CAMERA_CCD_SIZE + PosX];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - 2*iDist) * CAMERA_CCD_SIZE + PosX];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if((unsigned char)pImg[(PosY - iDist*2) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - iDist*1) * CAMERA_CCD_SIZE + PosX] > 0)
			{
				if((unsigned char)pImg[(PosY - iDist*1) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[(PosY - iDist*2) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - iDist*1) * CAMERA_CCD_SIZE + PosX] < 0)
			{
				if((unsigned char)pImg[(PosY - iDist*1) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
		else if(PosY - iDist >= top)
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - iDist) * CAMERA_CCD_SIZE + PosX];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist) * CAMERA_CCD_SIZE + PosX];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if((unsigned char)pImg[(PosY - iDist) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist) * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[(PosY - iDist) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist) * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}

		}
		else
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist) * CAMERA_CCD_SIZE + PosX];

			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + 2*iDist) * CAMERA_CCD_SIZE + PosX];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if((unsigned char)pImg[(PosY + iDist*2) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist*1) * CAMERA_CCD_SIZE + PosX] > 0)
			{
				if((unsigned char)pImg[(PosY + iDist*1) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[(PosY + iDist*2) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist*1) * CAMERA_CCD_SIZE + PosX] < 0)
			{
				if((unsigned char)pImg[(PosY + iDist*1) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CDetectionAlgorithm::CellSlopeFilter2(BYTE* pImg, int PosX, int PosY, int left, int right, int top, int bottom, int iDistV, int iDistH, int ThL, int ThH, int iSlopeFactor)
{
	int CAMERA_CCD_SIZE = m_iImageWidth;
	int i=0;

	if (PosX < 2 || PosX > CAMERA_CCD_SIZE-2)
	{
		return TRUE;
	}

	int iDiff = 0;

	int iTh = 10, iTh2 = 10;
	int iDist;

	int sub_gray1 = 0;
	int sub_gray2 = 0;
	int sub_gray3 = 0;
	int sub_gray4 = 0;
	int sub_gray5 = 0;
	int sub_gray6 = 0;

	iTh = iTh2 = (ThH - ThL) * (double)(unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] / 255.0 + ThL;

	if(iSlopeFactor > 0) 
		iTh = iSlopeFactor;
		
	iDist = iDistH;

	if(right - left > 3*iDist+2)
	{
		if(PosX + iDist >= right)
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - 2*iDist];

			sub_gray3 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist + 1];
			sub_gray4 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - 2*iDist + 1];

			sub_gray5 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist - 1];
			sub_gray6 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - 2*iDist - 1];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else 
				iDiff = abs(sub_gray2);
			
			if(iDiff < iTh)
				return TRUE;

			if(abs(sub_gray1) < iTh || abs(sub_gray2) < iTh || abs(sub_gray3) < iTh || abs(sub_gray4) < iTh || abs(sub_gray5) < iTh || abs(sub_gray6) < iTh)
				return TRUE;

			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
		else if(PosX - iDist >= left)
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist];

			sub_gray3 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist + 1];
			sub_gray4 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist + 1];

			sub_gray5 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist - 1];
			sub_gray6 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist - 1];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else 
				iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if(abs(sub_gray1) < iTh || abs(sub_gray2) < iTh || abs(sub_gray3) < iTh || abs(sub_gray4) < iTh || abs(sub_gray5) < iTh || abs(sub_gray6) < iTh)
				return TRUE;

			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist] < 0)
					return TRUE;
			}
		}
		else
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + 2*iDist];

			sub_gray3 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist + 1];
			sub_gray4 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + 2*iDist + 1];

			sub_gray5 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist - 1];
			sub_gray6 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + 2*iDist - 1];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else
				iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if(abs(sub_gray1) < iTh || abs(sub_gray2) < iTh || abs(sub_gray3) < iTh || abs(sub_gray4) < iTh || abs(sub_gray5) < iTh || abs(sub_gray6) < iTh)
				return TRUE;

			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
	}

	iDist = iDistV;
	iTh = iTh2;

	if(bottom - top > 3*iDist)
	{
		if(PosY + iDist >= bottom)
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - iDist) * CAMERA_CCD_SIZE + PosX];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - 2*iDist) * CAMERA_CCD_SIZE + PosX];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if((unsigned char)pImg[(PosY - iDist*2) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - iDist*1) * CAMERA_CCD_SIZE + PosX] > 0)
			{
				if((unsigned char)pImg[(PosY - iDist*1) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[(PosY - iDist*2) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - iDist*1) * CAMERA_CCD_SIZE + PosX] < 0)
			{
				if((unsigned char)pImg[(PosY - iDist*1) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
		else if(PosY - iDist >= top)
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY - iDist) * CAMERA_CCD_SIZE + PosX];
			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist) * CAMERA_CCD_SIZE + PosX];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if((unsigned char)pImg[(PosY - iDist) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist) * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[(PosY - iDist) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist) * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
		else
		{
			sub_gray1 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist) * CAMERA_CCD_SIZE + PosX];

			sub_gray2 = (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + 2*iDist) * CAMERA_CCD_SIZE + PosX];

			if(abs(sub_gray1) <= abs(sub_gray2))
				iDiff = abs(sub_gray1);
			else iDiff = abs(sub_gray2);

			if(iDiff < iTh)
				return TRUE;

			if((unsigned char)pImg[(PosY + iDist*2) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist*1) * CAMERA_CCD_SIZE + PosX] > 0)
			{
				if((unsigned char)pImg[(PosY + iDist*1) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[(PosY + iDist*2) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[(PosY + iDist*1) * CAMERA_CCD_SIZE + PosX] < 0)
			{
				if((unsigned char)pImg[(PosY + iDist*1) * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CDetectionAlgorithm::ASGSlopeFilter(BYTE* pImg, int PosX, int PosY, int iDistH, int iDistV, int left, int right, int top, int bottom)
{
	int CAMERA_CCD_SIZE = m_iImageWidth;

	if (PosX < 2 || PosX > CAMERA_CCD_SIZE - 2)
	{
		return TRUE;
	}
	int iDist;
	if(iDistH == 0) // Vertical
	{

	}

	if(iDistV == 0) // Horizontal
	{
		iDist = iDistH;

		if(PosX + iDist >= right)
		{
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
		else if(PosX - iDist < left)
		{
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*2] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist*1] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
					return TRUE;
			}
		}
		else
		{
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] > 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist] > 0)
					return TRUE;
			}
			if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX - iDist] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] < 0)
			{
				if((unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX] - (unsigned char)pImg[PosY * CAMERA_CCD_SIZE + PosX + iDist] < 0)
					return TRUE;
			}

		}

	}

	return FALSE;
}

int CDetectionAlgorithm::MakeFeature(int iPixelCnt)
{
	int iMerge = m_Merge.Merge(iPixelCnt, m_pPixelResult, m_Param.m_iMergeDist, m_Param.m_iMinSize, m_Param.m_iMaxSize, MAX_MERGE_PIXELCNT) ;

	int iDefectFeatureCnt = 0;

	for (int iDefect = 0; iDefect < m_Merge.iDefectCnt; iDefect++) 
	{
		if (m_Merge.DefectList[iDefect].bDelete) 
		{
			continue ;
		}

		if(iDefectFeatureCnt >= MAX_DEFECT_CNT)
		{
			break;
		}

		CDefectFeature* pNewFeature = NULL;

		pNewFeature = &(m_DefectFeature[iDefectFeatureCnt]) ;

		if (pNewFeature == NULL) 
		{
			break;
		}

		pNewFeature->Init() ;

		pNewFeature->iFrameCount = 0 ;

		pNewFeature->iArea = m_Merge.DefectList[iDefect].iPixelCnt ;

		pNewFeature->iMinx = m_Merge.DefectList[iDefect].m_iRealLeft ;
		pNewFeature->iMaxx = m_Merge.DefectList[iDefect].m_iRealRight + 1 ;
		pNewFeature->iMiny = m_Merge.DefectList[iDefect].m_iRealTop ;
		pNewFeature->iMaxy = m_Merge.DefectList[iDefect].m_iRealBottom + 1 ;

		pNewFeature->iDefectMinGray = 255 ;
		pNewFeature->iDefectMaxGray = 0 ;
		pNewFeature->iReferenceMinGray = 255 ;
		pNewFeature->iReferenceMaxGray = 0 ;
		pNewFeature->iMatchingReferenceMinGray = 255 ;
		pNewFeature->iMatchingReferenceMaxGray = 0 ; 
		pNewFeature->iIsPAD = 0 ;

		if(m_Param.m_bEnableZone) // Zone 검사
		{
			int iZoneOffsetX = 0 ;
			int iZoneOffsetY = 0 ;
			int iZoneFirstX = 0 ;
			int iZoneFirstY = 0 ;
			int	CCD_DEVIDE = m_iImageWidth/DEVIDE_NUMBER;
			int vN, numerator, denominator = CCD_DEVIDE ;			
			int iVirtualZoneOrgX = 0, iVirtualZoneOrgY = 0 ;
			int iZoneOrgResultX = 0, iZoneOrgResultY = 0 ;
			int iZoneX = 0, iZoneY = 0 ;
			double fZoneOrgX =0.0;
			int iZoneOrgX = 0, iZoneOrgY = 0;
			int iZoneNO = 0;
			int iZoneWidth = m_Param.m_iImgSizeX;
			int iZoneHeight = m_Param.m_iImgSizeY;
			int iExtraS_W = 8, iExtraS_H = 4;
			BYTE* pPatternImage = NULL;
			CvPoint	ptFindPos ;
			CString strXY = "" ;

			PMResult* pPMResult = &m_PMResult;
			BYTE* pImg = m_pImage;
			BYTE* pZoneImg = m_Param.m_pZoneImage;
			double min = 0.0, max = 0.0;
			int iTemp = 0;			
			int* pZoneMin = m_Param.m_iZoneMinSize;
			int* pZoneThL = m_Param.m_iZoneThresholdLow;
			int* pZoneThH = m_Param.m_iZoneThresholdHigh;
			double ZoneThSlope[MAX_ZONE_CNT];
			
			for (int k = 0; k < MAX_ZONE_CNT; k++) 
			{
				ZoneThSlope[k] = (double)(pZoneThH[k] - pZoneThL[k] + 1.) / 256. ;
			}

			for (int iPixel = 0; iPixel < m_Merge.DefectList[iDefect].iPixelCnt; iPixel++) 
			{
				pNewFeature->iCenterx += m_Merge.DefectList[iDefect].PixelList[iPixel].iX ;
				pNewFeature->iCentery += m_Merge.DefectList[iDefect].PixelList[iPixel].iY ;
				
				double fSum_X = double(pPMResult->iOrgPosX) ;
				double fSum_Y = double(pPMResult->iOrgPosY) ;

				for (int test = 0; test < 2; test++) {
					while (fSum_X < double(m_Merge.DefectList[iDefect].PixelList[iPixel].iX)) {
						numerator = (int)fSum_X ;
						
						vN = numerator / denominator ;

						if (vN >= DEVIDE_NUMBER) {
							vN = DEVIDE_NUMBER - 1 ;
						}

						fSum_X += pPMResult->dVectorxx[vN] ;
						fSum_Y += pPMResult->dVectorxy[vN] ;
					}

					while (fSum_Y < double(m_Merge.DefectList[iDefect].PixelList[iPixel].iY)) {
						fSum_Y += pPMResult->dVectoryy ;
						fSum_X += pPMResult->dVectoryx ;
					}

					while (fSum_X > double(m_Merge.DefectList[iDefect].PixelList[iPixel].iX)) {
						numerator = (int)fSum_X ;

						vN = numerator / denominator ;

						if (vN >= DEVIDE_NUMBER) {
							vN = DEVIDE_NUMBER - 1 ;
						}

						fSum_X -= pPMResult->dVectorxx[vN] ;
						fSum_Y -= pPMResult->dVectorxy[vN] ;
					}

					while (fSum_Y > double(m_Merge.DefectList[iDefect].PixelList[iPixel].iY)) {
						fSum_Y -= pPMResult->dVectoryy ;
						fSum_X -= pPMResult->dVectoryx ;
					}
				}

				if (iPixel == 0) {
					if (iExtraS_W || iExtraS_H) {
						iVirtualZoneOrgX = int(fSum_X + 0.5) ;
						iVirtualZoneOrgY = int(fSum_Y + 0.5) ;

						if (iVirtualZoneOrgX >= iExtraS_W && (iVirtualZoneOrgX + iZoneWidth + iExtraS_W) < m_iImageWidth && iVirtualZoneOrgY >= iExtraS_H && (iVirtualZoneOrgY + iZoneHeight + iExtraS_H) < m_iImageHeight) {
							IplImage* pTemplate = NULL ;
							IplImage* pImage_2 = NULL ;
							IplImage* pResultImg_2 = NULL ;

							pTemplate = cvCreateImage(cvSize(iZoneWidth, iZoneHeight), 8, 1) ;
							if (pTemplate == NULL) {
								goto SKIP ;
							}

							pImage_2 = cvCreateImage(cvSize(iZoneWidth + (iExtraS_W * 2), iZoneHeight + (iExtraS_H * 2)), 8, 1) ;
							if (pImage_2 == NULL) {
								goto SKIP ;
							}

							pResultImg_2 = cvCreateImage(cvSize((iExtraS_W*2)+1, (iExtraS_H*2)+1), IPL_DEPTH_32F, 1) ;
							if (pResultImg_2 == NULL) {
								goto SKIP ;
							}

							pPatternImage = m_Param.m_pPatternImage;

							for (int i = 0; i < iZoneHeight; i++) {
								CopyMemory(&(pTemplate->imageData[i*pTemplate->widthStep]), &(pPatternImage[i*iZoneWidth]), iZoneWidth) ;
							} // TEMPLATE

							int SearchX = 0 ;
							int SearchY = 0 ;

							SearchX = iVirtualZoneOrgX - iExtraS_W ;
							SearchY = iVirtualZoneOrgY - iExtraS_H ;

							for (int i = 0; i < iZoneHeight + (iExtraS_H * 2); i++) {
								CopyMemory(&(pImage_2->imageData[i*pImage_2->widthStep]), &(pImg[(i+SearchY)*m_iImageWidth+SearchX]), iZoneWidth + (2 * iExtraS_W)) ;
							}

							cvMatchTemplate(pImage_2, pTemplate, pResultImg_2, CV_TM_CCOEFF_NORMED) ;
							cvMinMaxLoc(pResultImg_2, &min, &max, NULL, &ptFindPos) ;

							iZoneOffsetX = (int)ptFindPos.x - iExtraS_W ;
							iZoneOffsetY = (int)ptFindPos.y - iExtraS_H ;

							if (iZoneOffsetX != 0) {
								strXY.Format("X:%d", iZoneOffsetX) ;
								
							}
							else {
								
							}

							if (iZoneOffsetY != 0) {
								strXY.Format("Y:%d", iZoneOffsetY) ;
								
							}
							else {
								
							}

SKIP :

							if (pResultImg_2 != NULL) {
								cvReleaseImage(&pResultImg_2) ;
							}
							if (pImage_2 != NULL) {
								cvReleaseImage(&pImage_2) ;
							}
							if (pTemplate != NULL) {
								cvReleaseImage(&pTemplate) ;
							}
						}
					}
				}

				fSum_X += double(iZoneOffsetX) ;
				fSum_Y += double(iZoneOffsetY) ;

				iZoneOrgResultX = int(fSum_X + 0.5) ;
				iZoneOrgResultY = int(fSum_Y + 0.5) ;

				if (iPixel == 0) {
					iZoneFirstX = iZoneOrgResultX ;
					iZoneFirstY = iZoneOrgResultY ;
				}
				else {
					if (iZoneFirstX != iZoneOrgResultX ||
						iZoneFirstY != iZoneOrgResultY) {							
					}
				}

				
				iZoneX = m_Merge.DefectList[iDefect].PixelList[iPixel].iX - iZoneOrgResultX ;
				iZoneY = m_Merge.DefectList[iDefect].PixelList[iPixel].iY - iZoneOrgResultY ;
				
				
				while (iZoneX < 0) {
					iZoneX += iZoneWidth ;
				}

				while (iZoneX >= iZoneWidth) {
					iZoneX -= iZoneWidth ;
				}

				while (iZoneY < 0) {
					iZoneY += iZoneHeight ;
				}

				while (iZoneY >= iZoneHeight) {
					iZoneY -= iZoneHeight ;
				}
				
				iZoneOrgX = iZoneX % iZoneWidth ;
				iZoneOrgY = iZoneY % iZoneHeight ;

				if (iZoneOrgX >= 0 && iZoneOrgX < iZoneWidth && iZoneOrgY >= 0 && iZoneOrgY < iZoneHeight) {
					iZoneNO = static_cast<int>(pZoneImg[(iZoneOrgY * iZoneWidth) + iZoneOrgX]) ;

					
					if (iZoneNO >= 0 && iZoneNO < MAX_ZONE_CNT) {
						pNewFeature->iZoneArea[iZoneNO]++ ;
						pNewFeature->iZoneGray[iZoneNO] += m_Merge.DefectList[iDefect].PixelList[iPixel].iGray ;
						pNewFeature->iZoneReferenceGray[iZoneNO] += m_Merge.DefectList[iDefect].PixelList[iPixel].iReference ;

						pNewFeature->iDefectGraySum += m_Merge.DefectList[iDefect].PixelList[iPixel].iGray;
						pNewFeature->iDefectGraySquareSum += (m_Merge.DefectList[iDefect].PixelList[iPixel].iGray*m_Merge.DefectList[iDefect].PixelList[iPixel].iGray);

						if (pNewFeature->iDefectMinGray > m_Merge.DefectList[iDefect].PixelList[iPixel].iGray) 
						{
							pNewFeature->iDefectMinGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iGray;
							pNewFeature->iMatchingReferenceMinGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iReference; // eto 20130108
						}

						if (pNewFeature->iDefectMaxGray < m_Merge.DefectList[iDefect].PixelList[iPixel].iGray)
						{
							pNewFeature->iDefectMaxGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iGray;
							pNewFeature->iMatchingReferenceMaxGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iReference; // eto 20130108
						}

						pNewFeature->iReferenceGraySum += m_Merge.DefectList[iDefect].PixelList[iPixel].iReference;
						pNewFeature->iReferenceGraySquareSum += (m_Merge.DefectList[iDefect].PixelList[iPixel].iReference*m_Merge.DefectList[iDefect].PixelList[iPixel].iReference);

						if (pNewFeature->iReferenceMinGray > m_Merge.DefectList[iDefect].PixelList[iPixel].iReference) pNewFeature->iReferenceMinGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iReference;
						if (pNewFeature->iReferenceMaxGray < m_Merge.DefectList[iDefect].PixelList[iPixel].iReference) pNewFeature->iReferenceMaxGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iReference;
					}
				}
			}

			if (pNewFeature->iArea > 0) {
				pNewFeature->iCenterx /= pNewFeature->iArea ;
				pNewFeature->iCentery /= pNewFeature->iArea ;
			}
			else {
				pNewFeature->iCenterx = 0 ;
				pNewFeature->iCentery = 0 ;
			}

			iTemp = 0 ;

			for (int k = 0; k < MAX_ZONE_CNT; k++) {
				if (pNewFeature->iZoneArea[k]) {
					if (pNewFeature->iZoneArea[k] < pZoneMin[k]) {
						iTemp += pNewFeature->iZoneArea[k] ;
					}
					else {
						fZoneOrgX = abs(pNewFeature->iZoneGray[k] - pNewFeature->iZoneReferenceGray[k]) / static_cast<double>(pNewFeature->iZoneArea[k]) ;

						if (fZoneOrgX < (pZoneThL[k] + fZoneOrgX * ZoneThSlope[k])) {
							iTemp += pNewFeature->iZoneArea[k] ;
						}
					}
				}
			}

			if (iTemp > pNewFeature->iArea) {
				if(iDefectFeatureCnt > 0) --iDefectFeatureCnt;
				continue ;
			}
			else
			{
				int iSizeCnt = 0;
				for (int k = 0; k < MAX_ZONE_CNT; k++) 
				{
					if (pNewFeature->iZoneArea[k]) 
					{
						if (pNewFeature->iZoneArea[k] < pZoneMin[k]) 
							pNewFeature->iZoneArea[k] = 0;
						else
							iSizeCnt++;
					}
				}

				if(iSizeCnt == 0)
				{
					if(iDefectFeatureCnt > 0) --iDefectFeatureCnt;
					continue ;
				}
			}

			++iDefectFeatureCnt;
		}
		else // 일반 검사
		{
			for (int iPixel = 0; iPixel < m_Merge.DefectList[iDefect].iPixelCnt; iPixel++) 
			{
				pNewFeature->iCenterx += m_Merge.DefectList[iDefect].PixelList[iPixel].iX ;
				pNewFeature->iCentery += m_Merge.DefectList[iDefect].PixelList[iPixel].iY ;

				pNewFeature->iDefectGraySum += m_Merge.DefectList[iDefect].PixelList[iPixel].iGray;

				pNewFeature->iGrayCount[m_Merge.DefectList[iDefect].PixelList[iPixel].iGray/10]++;

				pNewFeature->iDefectGraySquareSum += (m_Merge.DefectList[iDefect].PixelList[iPixel].iGray*m_Merge.DefectList[iDefect].PixelList[iPixel].iGray);

				if (pNewFeature->iDefectMinGray > m_Merge.DefectList[iDefect].PixelList[iPixel].iGray) 
				{
					pNewFeature->iDefectMinGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iGray;
					pNewFeature->iMatchingReferenceMinGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iReference; // eto 20130108
				}
				if (pNewFeature->iDefectMaxGray < m_Merge.DefectList[iDefect].PixelList[iPixel].iGray)
				{
					pNewFeature->iDefectMaxGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iGray;
					pNewFeature->iMatchingReferenceMaxGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iReference; // eto 20130108
				}

				pNewFeature->iReferenceGraySum += m_Merge.DefectList[iDefect].PixelList[iPixel].iReference;
				pNewFeature->iReferenceGraySquareSum += (m_Merge.DefectList[iDefect].PixelList[iPixel].iReference*m_Merge.DefectList[iDefect].PixelList[iPixel].iReference);

				if (pNewFeature->iReferenceMinGray > m_Merge.DefectList[iDefect].PixelList[iPixel].iReference)
					pNewFeature->iReferenceMinGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iReference;
				if (pNewFeature->iReferenceMaxGray < m_Merge.DefectList[iDefect].PixelList[iPixel].iReference)
					pNewFeature->iReferenceMaxGray = m_Merge.DefectList[iDefect].PixelList[iPixel].iReference;

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//계산식 : sqrt((Threshold*fov*fov)/(Background-(Defect/Background)*Background))
				int gray = m_Merge.DefectList[iDefect].PixelList[iPixel].iGray;
				int back = m_Merge.DefectList[iDefect].PixelList[iPixel].iReference;
				if(back <= 0) back = 1;
				int th = ((m_Param.m_iThresholdHigh - m_Param.m_iThresholdLow)/255)*gray + m_Param.m_iThresholdLow;
				double fov = m_Param.m_fFOV;
				double fArea = sqrt(fabs((double)th*fov*fov/((double)back-((double)gray/(double)back)*(double)back))); // Calc Method 1
				//double fArea = sqrt(fabs((double)th*fov*fov/((double)back-(((double)gray-(double)back)/255.0)*(double)back))); // Calc Method 2
				
				pNewFeature->fRealArea += fArea;
				//pNewFeature->fRealArea += (fArea/(th/10.0)); // Threshold 10 기준
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// Perimeter -> Pixel 최외곽 찾는 부분이 빠져 있음, 구현 필요
				if (iPixel < m_Merge.DefectList[iDefect].iPixelCnt - 1)
				{
					pNewFeature->fPerimeter += sqrtf((float)(m_Merge.DefectList[iDefect].PixelList[iPixel].iX - m_Merge.DefectList[iDefect].PixelList[iPixel+1].iX)						
						* (m_Merge.DefectList[iDefect].PixelList[iPixel].iX - m_Merge.DefectList[iDefect].PixelList[iPixel+1].iX)
						+ (float)(m_Merge.DefectList[iDefect].PixelList[iPixel].iY - m_Merge.DefectList[iDefect].PixelList[iPixel+1].iY)
						* (m_Merge.DefectList[iDefect].PixelList[iPixel].iY - m_Merge.DefectList[iDefect].PixelList[iPixel+1].iY));
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			double dRatio = pNewFeature->fRealArea/pNewFeature->iArea;
			double dx = pNewFeature->fRealDx = (pNewFeature->iMaxx-pNewFeature->iMinx) * dRatio;
			double dy = pNewFeature->fRealDy = (pNewFeature->iMaxy-pNewFeature->iMiny) * dRatio;					
			pNewFeature->fRealArea = sqrt(pow(dx,2)+pow(dy,2));

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Fiber Width
			//pNewFeature->fFiberWidth = (pNewFeature->fPerimeter
			//	- sqrtf(pNewFeature->fPerimeter * pNewFeature->fPerimeter - 16.f / 1000000.f * pNewFeature->iArea)) / (4.f / 1000000.f);
			pNewFeature->fFiberWidth = (pNewFeature->fPerimeter
				- sqrtf(pNewFeature->fPerimeter * pNewFeature->fPerimeter - 16.0 * (double)pNewFeature->iArea)) / (4.0);

			// Fiber Length
			pNewFeature->fFiberLength = pNewFeature->iArea / pNewFeature->fFiberWidth;

			// Curl
			int iMajor = (pNewFeature->iMaxx-pNewFeature->iMinx) > (pNewFeature->iMaxy-pNewFeature->iMiny) ? (pNewFeature->iMaxx-pNewFeature->iMinx) : (pNewFeature->iMaxy-pNewFeature->iMiny);
			pNewFeature->fCurl = (double)(iMajor) / pNewFeature->fFiberLength;
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////			
			
			if (pNewFeature->iArea > 0)
			{
				pNewFeature->iCenterx /= pNewFeature->iArea ;
				pNewFeature->iCentery /= pNewFeature->iArea ;
			}
			else 
			{
				pNewFeature->iCenterx = 0 ;
				pNewFeature->iCentery = 0 ;
			}

			int cenX = (m_Merge.DefectList[iDefect].m_iRealRight + m_Merge.DefectList[iDefect].m_iRealLeft) / 2 ;
			int cenY = (m_Merge.DefectList[iDefect].m_iRealBottom + m_Merge.DefectList[iDefect].m_iRealTop) / 2 ;

			int left = cenX - (100 / 2) ;
			int right = left + 100 ;
			int top = cenY - (100 / 2) ;
			int bottom = top + 100 ;

			++iDefectFeatureCnt;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//qsort((void*)m_DefectFeature, (size_t)iDefectFeatureCnt, sizeof(CDefectFeature), CompareDefectSize);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return iDefectFeatureCnt;
}

int CDetectionAlgorithm::CompareDefectSize(const void* arg1, const void* arg2)
{
	//if((*(CDefectFeature**)arg1)->iArea - (*(CDefectFeature**)arg2)->iArea < 0)
	if(((CDefectFeature*)arg1)->iArea - ((CDefectFeature*)arg2)->iArea <= 0)
		return 1;
	else
		return -1;

	return 1;
}

void CDetectionAlgorithm::MakeVector(__m256i *img, int left, int right, int top, int bottom, PMResult *pResult)
{
	BOOL	bSkipFlag[32] = {0, };
	BOOL	bLowRateSaveFlag = FALSE;
	CString strPMImageSaveFolder = "D:\\DDMDemo\\Zone\\ZoneFail\\";
	int		iSaveFlag = 0;
	int		CCD_DEVIDE = m_iImageWidth/DEVIDE_NUMBER;

	CString strV = "", strNum = "", strD = "" ;

	double vF = 0. ;

	int curStep = 0 ;
	int i ;
	int	iPatWidth, iPatHeight ;
	int findStep = - 1 ;

	BYTE *pPatternImage = NULL;

	double min, max ;

	CvPoint	ptFindPos ;

	BYTE *pByteImg = reinterpret_cast<BYTE*>(img) ;

	int	SearchX, SearchY ;

	double fPixelWidth_um = 0, fPixelHeight_um = 0 ;
	double fZonePixelWidth_um = 0, fZonePixelHeight_um = 0 ;

	double fPixelFOV = 0, fScanResolution = 0 ;

	int Nx[DEVIDE_NUMBER], Ny ;

	int nPixelCnt = 0;
	int nPatternCnt = 0;
	CString str;


	iPatWidth = m_Param.m_iImgSizeX, iPatHeight = m_Param.m_iImgSizeY;

	if (iPatWidth == 0 || iPatHeight == 0) 
	{
		if (bSkipFlag[0]) 
		{
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else 
		{
			pResult->iStatus = ERR_PM_LOW_RATE ;
		}

		goto SKIP ;
	}

	int W_10 = m_Param.m_iPMSearchWidth;
	int H_10 = m_Param.m_iPMSearchHeight;

	pPatternImage = m_Param.m_pPatternImage ;

	//*/
	IplImage* pTemplate = NULL ;
	IplImage* pImage = NULL ;
	IplImage* pImage_2 = NULL ;
	IplImage* pResultImg = NULL ;
	IplImage* pResultImg_2 = NULL ;
	//*/

	pTemplate = cvCreateImage(cvSize(iPatWidth, iPatHeight), 8, 1) ;
	if (pTemplate == NULL) 
	{
		if (bSkipFlag[1]) 
		{
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else 
		{
			pResult->iStatus = ERR_PM_LOW_RATE ;
		}

		goto SKIP ;
	}

	pImage = cvCreateImage(cvSize(iPatWidth * 2, iPatHeight * 2), 8, 1) ;
	if (pImage == NULL) {
		if (bSkipFlag[2]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_LOW_RATE ;
		}

		goto SKIP ;
	}

	pImage_2 = cvCreateImage(cvSize(iPatWidth + (W_10 * 2), iPatHeight + (H_10 * 2)), 8, 1) ;
	if (pImage_2 == NULL) {
		if (bSkipFlag[3]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_LOW_RATE ;
		}

		
		goto SKIP ;
	}

	pResultImg = cvCreateImage(cvSize(iPatWidth+1, iPatHeight+1), IPL_DEPTH_32F, 1);
	if (pResultImg == NULL) {
		if (bSkipFlag[4]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_LOW_RATE ;
		}

		goto SKIP ;
	}

	pResultImg_2 = cvCreateImage(cvSize((W_10*2)+1, (H_10*2)+1), IPL_DEPTH_32F, 1);
	if (pResultImg_2 == NULL) {
		if (bSkipFlag[5]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_LOW_RATE ;
		}

		goto SKIP ;
	}

	for (i = 0; i < iPatHeight; i++) {
		CopyMemory(&(pTemplate->imageData[i*pTemplate->widthStep]), &(pPatternImage[i*iPatWidth]), iPatWidth) ;
	} // TEMPLATE

	fZonePixelWidth_um = m_Param.m_iZonePixelSizeX_um, fZonePixelHeight_um = m_Param.m_iZonePixelSizeY_um ;
	
	fPixelFOV = m_Param.m_fRealFOV, fScanResolution = m_Param.m_fScanResolution ;

	//SearchX = left + 1 ;
	SearchX = left + W_10 + 1 ;
	SearchY = top + H_10 + 1 ;

	if ((SearchX + (iPatWidth * 2)) >= (right - 1)) {
		if (bSkipFlag[8]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_SKIP ;
		}

		goto SKIP ;
	}

	if ((SearchY + (iPatHeight * 2)) >= (bottom - 1)) {
		if (bSkipFlag[9]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_SKIP ;
		}

		goto SKIP ;
	}

	for (i = 0; i < iPatHeight * 2; i++) {
		CopyMemory(&(pImage->imageData[i * pImage->widthStep]), &(pByteImg[(i + SearchY) * m_iImageWidth + SearchX]), iPatWidth * 2) ;
	}

	cvMatchTemplate(pImage, pTemplate, pResultImg, CV_TM_CCOEFF_NORMED) ;
	cvMinMaxLoc(pResultImg, &min, &max, NULL, &ptFindPos) ;

	double rel = 999999. ;

	int numerator = SearchX + (int)ptFindPos.x ;
	int denominator = CCD_DEVIDE ;

	curStep = numerator / denominator ;

	int firstStep = curStep ;

	pResult->dReliabilityx[firstStep] = max ;
	pResult->dReliabilityy[firstStep] = max ;

	if (max < 0.3f) 
	{
		if (bSkipFlag[10]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_LOW_RATE ;
		}

		strD.Format("ERR_PM_LOW_RATE - first step[%02d]", curStep) ;
		
		if (bLowRateSaveFlag) 
		{
			bLowRateSaveFlag = 0 ;

			SYSTEMTIME time ;
			GetLocalTime(&time) ;

			CString strFile = "" ;
			strFile.Format("%s%02d%02d%02d.%03d_S%02d_X%04d_Y%04d_%.3lf_%.3lf.bmp",
				strPMImageSaveFolder,
				time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
				curStep, SearchX, SearchY, fPixelFOV, max) ;

			if (pImage != NULL)
			{
				CreateFolder(strPMImageSaveFolder) ;
				cvSaveImage(strFile, pImage) ;
			}
			
			if (GetPrivateProfileInt("ZONE", "zone_fail_frame_save", 0, "D:\\DDMCore\\DDMCore.ini")) {
				IplImage* pFullFrame = NULL ;
				pFullFrame = cvCreateImage(cvSize(m_iImageWidth, m_iImageHeight), 8, 1) ;

				if (pFullFrame != NULL) 
				{
					for (int i = 0; i < m_iImageHeight; i++) 
					{
						CopyMemory(&(pFullFrame->imageData[i * pFullFrame->widthStep]), &(pByteImg[i * m_iImageWidth]), m_iImageWidth) ;
					}

					strFile.Format("%s%02d%02d%02d.%03d_S%02d_X%04d_Y%04d.bmp",
						strPMImageSaveFolder,
						time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
						curStep, SearchX, SearchY) ;

					CreateFolder(strPMImageSaveFolder) ;
					cvSaveImage(strFile, pFullFrame) ;

					cvReleaseImage(&pFullFrame) ;
				}
			}
		}

		goto SKIP ;
	}
	else
	{
		pResult->iMatchx[firstStep] = SearchX + ptFindPos.x ;
		pResult->iMatchy[firstStep] = SearchY + ptFindPos.y ;

		if (iSaveFlag > 0) 
		{
			if ((firstStep == 0)) 
			{
				iSaveFlag-- ;

				SYSTEMTIME time ;
				GetLocalTime(&time) ;

				CString strFile = "" ;
				strFile.Format("%s%02d%02d%02d.%03d_%02d_X%04d_Y%04d_%.6lf_%.6lf.bmp",
					strPMImageSaveFolder,
					time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
					curStep, SearchX, SearchY, fPixelFOV, max) ;

				if (pImage != NULL) 
				{
					CreateFolder(strPMImageSaveFolder) ;
					cvSaveImage(strFile, pImage) ;
				}
			}
		}
	}

	pResult->iTempCount = 0 ;

	SearchX = pResult->iMatchx[firstStep] ;
	SearchY = pResult->iMatchy[firstStep] ;

	pResult->iOrgPosX = pResult->iMatchx[firstStep] ;
	pResult->iOrgPosY = pResult->iMatchy[firstStep] ;

	int last, tmpX ;

	last = right / denominator ;

	int maxStep = -1 ;

	double dSearchX_um = 0. ;

	

	for ( ; SearchX < right - (iPatWidth * 3); ) {
		dSearchX_um = (double)SearchX * fPixelFOV ;

		if (curStep >= DEVIDE_NUMBER) {
			break ;
		}

		tmpX = SearchX ;

		if (curStep < last) {
			while ((SearchX < ((curStep + 1) * CCD_DEVIDE)) && (SearchX < (right - (iPatWidth * 3)))) {
				dSearchX_um += fZonePixelWidth_um ;
				SearchX = int(dSearchX_um / fPixelFOV) ;
			}
		}
		else {
			while (SearchX < (right - (iPatWidth * 3))) {
				dSearchX_um += fZonePixelWidth_um ;
				SearchX = int(dSearchX_um / fPixelFOV) ;
			}			
		}

		if (SearchX == tmpX) {
			break ;
		}

		curStep++ ;

		SearchX -= W_10 ;
		SearchY -= H_10 ;

		if (SearchX < 0) 
		{
			break ;
		}

		if (SearchY < 0) {
			SearchY += iPatHeight ;
		}

		if ((SearchX + (iPatWidth + (2 * W_10))) >= (right - 1)) {
			break ;
		}

		if ((SearchY + (iPatHeight + (2 * H_10))) >= (bottom - 1)) {
			break ;
		}

		for (i = 0; i < iPatHeight + (H_10 * 2); i++) {
			CopyMemory(&(pImage_2->imageData[i*pImage_2->widthStep]), &(pByteImg[(i+SearchY)*m_iImageWidth+SearchX]), iPatWidth + (2 * W_10)) ;
		}

		cvMatchTemplate(pImage_2, pTemplate, pResultImg_2, CV_TM_CCOEFF_NORMED) ;
		cvMinMaxLoc(pResultImg_2, &min, &max, NULL, &ptFindPos) ;

		nPixelCnt = (SearchX + ptFindPos.x) - tmpX;
		nPatternCnt = (int)((double)(nPixelCnt) / (double)iPatWidth + 0.5);

		fPixelFOV = (double)(fZonePixelWidth_um * nPatternCnt) / (double)nPixelCnt;
		str.Format("fZonePixelWidth_um : %f, nPixelCnt : %d, iPatWidth : %d, nPatternCnt : %d, fPixelFOV :%f", fZonePixelWidth_um, nPixelCnt, iPatWidth, nPatternCnt, fPixelFOV);
		
		pResult->dReliabilityx[curStep] = max ;
		pResult->dReliabilityy[curStep] = max ;

		if (maxStep < curStep) {
			maxStep = curStep ;
		}

		if (max < 0.3f) 
		{
			strD.Format("ERR_PM_LOW_RATE - step[%02d]", curStep) ;			

			if (bLowRateSaveFlag) 
			{
				bLowRateSaveFlag = 0 ;

				SYSTEMTIME time ;
				GetLocalTime(&time) ;

				CString strFile = "" ;
				strFile.Format("%s%02d%02d%02d.%03d_S%02d_X%04d_Y%04d_%.3lf_%.3lf.bmp",
					strPMImageSaveFolder,
					time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
					curStep, SearchX, SearchY, fPixelFOV, max) ;

				if (pImage_2 != NULL) {
					CreateFolder(strPMImageSaveFolder) ;
					cvSaveImage(strFile, pImage_2) ;
				}

				if (GetPrivateProfileInt("ZONE", "zone_fail_frame_save", 0, "D:\\DDMCore\\DDMCore.ini"))
				{
					IplImage* pFullFrame = NULL ;
					pFullFrame = cvCreateImage(cvSize(m_iImageWidth, m_iImageHeight), 8, 1) ;

					if (pFullFrame != NULL) {
						for (int i = 0; i < m_iImageHeight; i++) {
							CopyMemory(&(pFullFrame->imageData[i * pFullFrame->widthStep]), &(pByteImg[i * m_iImageWidth]), m_iImageWidth) ;
						}

						strFile.Format("%s%02d%02d%02d.%03d_S%02d_X%04d_Y%04d.bmp",
							strPMImageSaveFolder,
							time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
							curStep, SearchX, SearchY) ;

						CreateFolder(strPMImageSaveFolder) ;
						cvSaveImage(strFile, pFullFrame) ;

						cvReleaseImage(&pFullFrame) ;
					}
				}
			}

			break ;
		}
		else 
		{
			if (iSaveFlag > 0) 
			{
				iSaveFlag-- ;

				SYSTEMTIME time ;
				GetLocalTime(&time) ;

				CString strFile = "" ;
				strFile.Format("%s%02d%02d%02d.%03d_%02d_X%04d_Y%04d_%.6lf_%.6lf.bmp",
					strPMImageSaveFolder,
					time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
					curStep, SearchX, SearchY, fPixelFOV, max) ;

				if (pImage_2 != NULL) {
					CreateFolder(strPMImageSaveFolder) ;
					cvSaveImage(strFile, pImage_2) ;
				}
			}

			pResult->iMatchx[curStep] = SearchX + ptFindPos.x ;
			pResult->iMatchy[curStep] = SearchY + ptFindPos.y ;

			SearchX = pResult->iMatchx[curStep] ;
			SearchY = pResult->iMatchy[curStep] ;
		}
	}

	SearchX = pResult->iMatchx[firstStep] ;

	int tmpY = SearchY = pResult->iMatchy[firstStep] ;

	double dSearchY_um = SearchY * fScanResolution ;

	while (SearchY < (bottom - (iPatHeight * 3) - 1)) {
		dSearchY_um += fZonePixelHeight_um ;
		SearchY = int(dSearchY_um / fScanResolution) ;
	}

	if (tmpY == SearchY) 
	{
		goto LAST_Y_PM_SKIP ;
	}

	curStep = DEVIDE_NUMBER + 1 ;

	SearchX -= W_10 ;
	SearchY -= H_10 ;

	if (SearchX < 0) {
		goto LAST_Y_PM_SKIP ;
	}

	if (SearchY < 0) {
		SearchY += iPatHeight ;
	}

	if ((SearchX + (iPatWidth + (2 * W_10))) >= (right - 1)) {
		goto LAST_Y_PM_SKIP ;
	}

	if ((SearchY + (iPatHeight + (2 * H_10))) >= (bottom - 1)) {
		goto LAST_Y_PM_SKIP ;
	}

	for (i = 0; i < iPatHeight + (H_10 * 2); i++) {
		CopyMemory(&(pImage_2->imageData[i*pImage_2->widthStep]), &(pByteImg[(i+SearchY)*m_iImageWidth+SearchX]), iPatWidth + (2 * W_10));
	}

	cvMatchTemplate(pImage_2, pTemplate, pResultImg_2, CV_TM_CCOEFF_NORMED) ;
	cvMinMaxLoc(pResultImg_2, &min, &max, NULL, &ptFindPos) ;
	
	pResult->dReliabilityx[curStep] = max ;
	pResult->dReliabilityy[curStep] = max ;

	if (max < 0.3f) 
	{
		if (bSkipFlag[21]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_LOW_RATE ;
		}

		strD.Format("ERR_PM_LOW_RATE - step[%02d]", curStep) ;		

		if (bLowRateSaveFlag) 
		{
			bLowRateSaveFlag = 0 ;

			SYSTEMTIME time ;
			GetLocalTime(&time) ;

			CString strFile = "" ;
			strFile.Format("%s%02d%02d%02d.%03d_S%02d_X%04d_Y%04d_%.3lf_%.3lf.bmp",
				strPMImageSaveFolder,
				time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
				curStep, SearchX, SearchY, fPixelFOV, max) ;

			if (pImage_2 != NULL) {
				CreateFolder(strPMImageSaveFolder) ;
				cvSaveImage(strFile, pImage_2) ;
			}

			if (GetPrivateProfileInt("ZONE", "zone_fail_frame_save", 0, "D:\\DDMCore\\DDMCore.ini")) 
			{
				IplImage* pFullFrame = NULL ;
				pFullFrame = cvCreateImage(cvSize(m_iImageWidth, m_iImageHeight), 8, 1) ;

				if (pFullFrame != NULL) {
					for (int i = 0; i < m_iImageHeight; i++) {
						CopyMemory(&(pFullFrame->imageData[i * pFullFrame->widthStep]), &(pByteImg[i * m_iImageWidth]), m_iImageWidth) ;
					}

					strFile.Format("%s%02d%02d%02d.%03d_S%02d_X%04d_Y%04d.bmp",
						strPMImageSaveFolder,
						time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
						curStep, SearchX, SearchY) ;

					CreateFolder(strPMImageSaveFolder) ;
					cvSaveImage(strFile, pFullFrame) ;

					cvReleaseImage(&pFullFrame) ;
				}
			}
		}

		goto LAST_Y_PM_SKIP ;
	}
	else {
		pResult->iMatchx[curStep] = ptFindPos.x + SearchX ;
		pResult->iMatchy[curStep] = ptFindPos.y + SearchY ;

		if (iSaveFlag > 0)
		{
			int frameNO = GetPrivateProfileInt("ZONE", "save_start_frame", 1, "D:\\DDMCore\\DDMCore.ini") ;

			iSaveFlag-- ;

			SYSTEMTIME time ;
			GetLocalTime(&time) ;

			CString strFile = "" ;
			strFile.Format("%s%02d%02d%02d.%03d_%02d_X%04d_Y%04d_%.6lf_%.6lf.bmp",
				strPMImageSaveFolder,
				time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
				curStep, SearchX, SearchY, fPixelFOV, max) ;

			if (pImage_2 != NULL) {
				CreateFolder(strPMImageSaveFolder) ;
				cvSaveImage(strFile, pImage_2) ;
			}
		}
	}

LAST_Y_PM_SKIP :

	double xyDist[DEVIDE_NUMBER], yxDist ;
	double zoneH, zoneW ;

	zoneH = fZonePixelHeight_um / fScanResolution ;

	for (int i = firstStep; i < DEVIDE_NUMBER; i++) {
		if (pResult->iMatchy[i] == -1 || pResult->iMatchy[i+1] == -1) {
			break ;
		}

		xyDist[i] = int(pResult->iMatchy[i+1] - pResult->iMatchy[i]) ;

		while (xyDist[i] > (zoneH / 2.)) {
			xyDist[i] -= zoneH ;
		}

		while (xyDist[i] < -(zoneH / 2.)) {
			xyDist[i] += zoneH ;
		}
	}

	zoneW = fZonePixelWidth_um / fPixelFOV ;

	if (pResult->iMatchx[firstStep] != -1 && pResult->iMatchx[DEVIDE_NUMBER+1] != -1) {
		yxDist = pResult->iMatchx[DEVIDE_NUMBER+1] - pResult->iMatchx[firstStep] ;

		while (yxDist > (zoneW / 2.)) {
			yxDist -= zoneW ;
		}

		while (yxDist < -(zoneW / 2.)) {
			yxDist += zoneW ;
		}
	}

	for (int i = firstStep; i < DEVIDE_NUMBER; i++) {
		if (pResult->iMatchx[i] == -1 || pResult->iMatchx[i+1] == -1) {
			CString strNum = "" ;

			CString strDefaultV = "" ;

			double defaultV = 1. ;

			char temp[50] = "" ;

			defaultV = fZonePixelWidth_um / fPixelFOV / (double)iPatWidth ;

			strNum.Format("xx_%02d", i) ;
			strDefaultV.Format("%.6lf", defaultV) ;

			GetPrivateProfileString("VECTOR", strNum, strDefaultV, temp, 50, "D:\\DDMCore\\DDMCore.ini") ;
			pResult->dVectorxx[i] = atof(temp) * (double)iPatWidth ;

			strNum.Format("xy_%02d", i) ;

			GetPrivateProfileString("VECTOR", strNum, "0.000000", temp, 50, "D:\\DDMCore\\DDMCore.ini") ;
			pResult->dVectorxy[i] = atof(temp) * (double)iPatWidth ;
		}
		else {
			Nx[i] = static_cast<int>(double(pResult->iMatchx[i+1] - pResult->iMatchx[i]) / (fZonePixelWidth_um / fPixelFOV) + 0.5) ;

			if (Nx[i] != 0) {
				pResult->dVectorxx[i] = static_cast<double>(double(pResult->iMatchx[i+1] - pResult->iMatchx[i]) / double(Nx[i])) ;
				pResult->dVectorxy[i] = xyDist[i] / double(Nx[i]) ;
			}
			else {
				pResult->dVectorxx[i] = fZonePixelWidth_um / fPixelFOV ;
				pResult->dVectorxy[i] = 0. ;
			}
		}
	}

	if (pResult->iMatchy[firstStep] == -1 || pResult->iMatchy[DEVIDE_NUMBER+1] == -1) {
		CString strDefaultV = "" ;

		double defaultV = 1. ;

		char temp[50] = "" ;

		GetPrivateProfileString("VECTOR", "yx", "0.000000", temp, 50, "D:\\DDMCore\\DDMCore.ini") ;
		pResult->dVectoryx = atof(temp) * (double)iPatHeight ;

		defaultV = fZonePixelHeight_um / fScanResolution / (double)iPatHeight ;

		strDefaultV.Format("%.6lf", defaultV) ;

		GetPrivateProfileString("VECTOR", "yy", strDefaultV, temp, 50, "D:\\DDMCore\\DDMCore.ini") ;
		pResult->dVectoryy = atof(temp) * (double)iPatHeight ;
	}
	else {
		Ny = static_cast<int>(double(pResult->iMatchy[DEVIDE_NUMBER+1] - pResult->iMatchy[firstStep]) / (fZonePixelHeight_um / fScanResolution) + 0.5) ;

		if (Ny != 0) {
			pResult->dVectoryx = yxDist / (double)Ny ;
			pResult->dVectoryy = static_cast<double>(double(pResult->iMatchy[DEVIDE_NUMBER+1] - pResult->iMatchy[firstStep]) / (double)Ny) ;
		}
		else {
			pResult->dVectoryx = 0. ;
			pResult->dVectoryy = fZonePixelHeight_um / fScanResolution ;
		}
	}

	double sum = 0. ;
	double count = 0. ;

	for (i = firstStep; i < DEVIDE_NUMBER; i++) {
		pResult->dDeterminant[i] = (pResult->dVectorxx[i] * pResult->dVectoryy) - (pResult->dVectorxy[i] * pResult->dVectoryx) ;

		if (Nx[i] != 0) {
			sum += pResult->dDeterminant[i] ;
			count += 1. ;
		}
	}
	
	if (count > 0. && sum != 0.) {
		pResult->dDeterminant_AVRG = sum / count ;
	}
	else {
		pResult->dDeterminant_AVRG = 1. ;

		if (bSkipFlag[22]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_LOW_RATE ;
		}

		goto SKIP ;
	}

	if (fabs(pResult->dVectorxx[firstStep] - iPatWidth) > 4) {
		if (bSkipFlag[23]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_ZOOM ;
		}

		goto SKIP ;
	}

	if (fabs(pResult->dDeterminant[firstStep]) < 0.001) {
		if (bSkipFlag[24]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_DET ;
		}

		goto SKIP ;
	}

	if (fabs(pResult->dVectoryy - iPatHeight) > 1.0) {
		if (bSkipFlag[25]) {
			pResult->iStatus = ERR_PM_SKIP ;
		}
		else {
			pResult->iStatus = ERR_PM_SCANRESOLTION ;
		}

		goto SKIP ;
	}

	pResult->iStatus = ERR_NONE ;


	for (int i = 0; i < DEVIDE_NUMBER; i++) {
		vF = pResult->dVectorxx[i] / (double)iPatWidth ;

		strV.Format("%.6lf", vF) ;
		strNum.Format("xx_%02d", i) ;

		if (strV != "1.000000") {
			WritePrivateProfileString("VECTOR", strNum, strV, "D:\\DDMCore\\DDMCore.ini") ;

			vF = pResult->dVectorxy[i] / (double)iPatWidth ;

			strV.Format("%.6lf", vF) ;
			strNum.Format("xy_%02d", i) ;

			WritePrivateProfileString("VECTOR", strNum, strV, "D:\\DDMCore\\DDMCore.ini") ;
		}
	}

	vF = pResult->dVectoryx / (double)iPatHeight ;

	strV.Format("%.6lf", vF) ;

	WritePrivateProfileString("VECTOR", "yx", strV, "D:\\DDMCore\\DDMCore.ini") ;

	vF = pResult->dVectoryy / (double)iPatHeight ;

	strV.Format("%.6lf", vF) ;

	WritePrivateProfileString("VECTOR", "yy", strV, "D:\\DDMCore\\DDMCore.ini") ;
	//}
	//*/

SKIP :
	if (pResultImg != NULL) {
		cvReleaseImage(&pResultImg) ;
	}
	if (pResultImg_2 != NULL) {
		cvReleaseImage(&pResultImg_2) ;
	}
	if (pImage != NULL) {
		cvReleaseImage(&pImage) ;
	}
	if (pImage_2 != NULL) {
		cvReleaseImage(&pImage_2) ;
	}
	if (pTemplate != NULL) {
		cvReleaseImage(&pTemplate) ;
	}
}

BOOL CDetectionAlgorithm::MakeRealZoneMask(BYTE* pRealZoneMaskL, BYTE* pRealZoneMaskH, BYTE* pRealZoneMaskL2, BYTE* pRealZoneMaskH2, PMResult *pmResult, int left, int iGenerateWidth)
{
	int i, j ;
	int x, y ;
	int iWidth, iHeight ;
	int		CCD_DEVIDE = m_iImageWidth/DEVIDE_NUMBER;
	int vN, numerator, denominator = CCD_DEVIDE ;

	int *pZoneThresholdL = m_Param.m_iZoneThresholdLow;
	int *pZoneThresholdH = m_Param.m_iZoneThresholdHigh;

	int *pZoneThresholdL2 = m_Param.m_iZoneThresholdLow2;
	int *pZoneThresholdH2 = m_Param.m_iZoneThresholdHigh2;

	BYTE *pZoneImg = NULL ;

	pZoneImg = m_Param.m_pZoneImage ;

	int iOutside_Pos = 0, iOutside_Offset = 0;
	int ZONE_MASK_WIDTH = ((MAX_PATTERNWIDTH * 4) + m_iImageWidth);
	int ZONE_MASK_HEIGHT = MAX_PATTERNHEIGHT;

	if (pZoneImg == NULL) {
		return 0 ;
	}

	iWidth = m_Param.m_iImgSizeX;
	iHeight = m_Param.m_iImgSizeY;
	
	int iWidth_Mid = iWidth + 4 ;

	if (iHeight * iWidth > MAX_ZONE_IMAGE) {
		return FALSE ;
	}

	BYTE pZoneTHImg[MAX_ZONE_IMAGE] ;
	BYTE pZoneTHImgHigh[MAX_ZONE_IMAGE] ;

	BYTE pZoneTHImg2[MAX_ZONE_IMAGE] ;
	BYTE pZoneTHImgHigh2[MAX_ZONE_IMAGE] ;

	for (i = 0; i < (iHeight * iWidth); ++i) 
	{
		if (pZoneThresholdL[pZoneImg[i]] > 220) 
		{
			//pZoneTHImg[i] = 220 ;
			pZoneTHImg[i] = 255 ;
		}
		else
		{
			pZoneTHImg[i] = pZoneThresholdL[pZoneImg[i]] ;		
		}
		if(pZoneThresholdL2[pZoneImg[i]] > 220 )
		{
			//pZoneTHImg2[i] = 220;
			pZoneTHImg2[i] = 255;
		}
		else 
			pZoneTHImg2[i] = pZoneThresholdL2[pZoneImg[i]] ;

		if (pZoneThresholdH[pZoneImg[i]] > 220) {
			//pZoneTHImgHigh[i] = 220 ;
			pZoneTHImgHigh[i] = 255 ;
		}
		else {
			pZoneTHImgHigh[i]  = pZoneThresholdH[pZoneImg[i]] ;			
		}
		if (pZoneThresholdH2[pZoneImg[i]] > 220) {
			//pZoneTHImgHigh2[i] = 220 ;
			pZoneTHImgHigh2[i] = 255 ;
		}
		else {			
			pZoneTHImgHigh2[i] = pZoneThresholdH2[pZoneImg[i]] ;
		}
	}

	BYTE pZoneTHImg_Mid[MAX_ZONE_IMAGE] ;
	BYTE pZoneTHImg_Mid_High[MAX_ZONE_IMAGE] ;

	BYTE pZoneTHImg_Mid2[MAX_ZONE_IMAGE] ;
	BYTE pZoneTHImg_Mid_High2[MAX_ZONE_IMAGE] ;

	BYTE v = 0 ;
	BYTE v_High;

	BYTE v2 = 0 ;
	BYTE v_High2;

	for (i = 0; i < iHeight; ++i) {
		CopyMemory(&(pZoneTHImg_Mid[iWidth_Mid*i]),			&(pZoneTHImg[iWidth*i]),		iWidth) ;
		CopyMemory(&(pZoneTHImg_Mid_High[iWidth_Mid*i]),	&(pZoneTHImgHigh[iWidth*i]),	iWidth) ;

		CopyMemory(&(pZoneTHImg_Mid2[iWidth_Mid*i]),			&(pZoneTHImg2[iWidth*i]),	iWidth) ;
		CopyMemory(&(pZoneTHImg_Mid_High2[iWidth_Mid*i]),	&(pZoneTHImgHigh2[iWidth*i]),	iWidth) ;

		v		= pZoneTHImg_Mid[iWidth_Mid*i+iWidth-1] ;
		v_High	= pZoneTHImg_Mid_High[iWidth_Mid*i+iWidth-1] ;

		v2		= pZoneTHImg_Mid2[iWidth_Mid*i+iWidth-1] ;
		v_High2	= pZoneTHImg_Mid_High2[iWidth_Mid*i+iWidth-1] ;

		pZoneTHImg_Mid[iWidth_Mid*i+iWidth+0] = v ;
		pZoneTHImg_Mid[iWidth_Mid*i+iWidth+1] = v ;
		pZoneTHImg_Mid[iWidth_Mid*i+iWidth+2] = v ;
		pZoneTHImg_Mid[iWidth_Mid*i+iWidth+3] = v ;

		pZoneTHImg_Mid_High[iWidth_Mid*i+iWidth+0] = v_High ;
		pZoneTHImg_Mid_High[iWidth_Mid*i+iWidth+1] = v_High ;
		pZoneTHImg_Mid_High[iWidth_Mid*i+iWidth+2] = v_High ;
		pZoneTHImg_Mid_High[iWidth_Mid*i+iWidth+3] = v_High ;

		pZoneTHImg_Mid2[iWidth_Mid*i+iWidth+0] = v2 ;
		pZoneTHImg_Mid2[iWidth_Mid*i+iWidth+1] = v2 ;
		pZoneTHImg_Mid2[iWidth_Mid*i+iWidth+2] = v2 ;
		pZoneTHImg_Mid2[iWidth_Mid*i+iWidth+3] = v2 ;

		pZoneTHImg_Mid_High2[iWidth_Mid*i+iWidth+0] = v_High2 ;
		pZoneTHImg_Mid_High2[iWidth_Mid*i+iWidth+1] = v_High2 ;
		pZoneTHImg_Mid_High2[iWidth_Mid*i+iWidth+2] = v_High2 ;
		pZoneTHImg_Mid_High2[iWidth_Mid*i+iWidth+3] = v_High2 ;
	}
	CopyMemory(&(pZoneTHImg_Mid[iWidth_Mid*(iHeight+0)]), &(pZoneTHImg_Mid[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid[iWidth_Mid*(iHeight+1)]), &(pZoneTHImg_Mid[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid[iWidth_Mid*(iHeight+2)]), &(pZoneTHImg_Mid[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid[iWidth_Mid*(iHeight+3)]), &(pZoneTHImg_Mid[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;

	CopyMemory(&(pZoneTHImg_Mid_High[iWidth_Mid*(iHeight+0)]), &(pZoneTHImg_Mid_High[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid_High[iWidth_Mid*(iHeight+1)]), &(pZoneTHImg_Mid_High[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid_High[iWidth_Mid*(iHeight+2)]), &(pZoneTHImg_Mid_High[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid_High[iWidth_Mid*(iHeight+3)]), &(pZoneTHImg_Mid_High[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CopyMemory(&(pZoneTHImg_Mid2[iWidth_Mid*(iHeight+0)]), &(pZoneTHImg_Mid2[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid2[iWidth_Mid*(iHeight+1)]), &(pZoneTHImg_Mid2[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid2[iWidth_Mid*(iHeight+2)]), &(pZoneTHImg_Mid2[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid2[iWidth_Mid*(iHeight+3)]), &(pZoneTHImg_Mid2[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;

	CopyMemory(&(pZoneTHImg_Mid_High2[iWidth_Mid*(iHeight+0)]), &(pZoneTHImg_Mid_High2[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid_High2[iWidth_Mid*(iHeight+1)]), &(pZoneTHImg_Mid_High2[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid_High2[iWidth_Mid*(iHeight+2)]), &(pZoneTHImg_Mid_High2[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	CopyMemory(&(pZoneTHImg_Mid_High2[iWidth_Mid*(iHeight+3)]), &(pZoneTHImg_Mid_High2[iWidth_Mid*(iHeight-1)]), iWidth_Mid) ;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int tY = 0 ;

	int	iPatWidth, iPatHeight ;

	double fPixelWidth_um = 0, fPixelHeight_um = 0 ;
	double fPixelFOV = 0, fScanResolution = 0 ;

	iPatWidth = m_Param.m_iImgSizeX, iPatHeight = m_Param.m_iImgSizeY ;

	if (iPatWidth == 0 || iPatHeight == 0) {
		return 0 ;
	}

	int iCnt = iGenerateWidth / iPatWidth ;

	double fX = 0., fY = 0. ;

	pmResult->iTempCount = 0 ;

	for (i = 0; i < iCnt + 3; i++) {
		numerator = left + (i * iPatWidth) ;

		vN = numerator / denominator ;

		if (vN > DEVIDE_NUMBER) {
			break ;
		}
		else if (vN == DEVIDE_NUMBER) {
			vN = DEVIDE_NUMBER - 1 ;
		}

		x = static_cast<int>(fX + 0.5) ;
		y = static_cast<int>(fY + 0.5) ;

		for (j = 0; j < (iHeight + 4); j++) {
			tY = j - y ;

			while (tY < 0) {
				tY += iHeight ;
			}

			//*/
			if ((x < iOutside_Pos) || (x > (m_iImageWidth - iOutside_Pos - 1))) 
			{
				if (iOutside_Offset <= 220 &&
					iOutside_Offset > 0 &&
					iOutside_Pos > 0 &&
					iOutside_Pos <= (m_iImageWidth / 2)) {
						//*/
						for (int k = 0; k < iWidth_Mid; k++) {
							if (pZoneTHImg_Mid[tY%iHeight*iWidth_Mid+k] < (255 - iOutside_Offset)) {
								pRealZoneMaskL[j*ZONE_MASK_WIDTH+x+k] = pZoneTHImg_Mid[tY%iHeight*iWidth_Mid+k] + iOutside_Offset ;
								pRealZoneMaskL2[j*ZONE_MASK_WIDTH+x+k] = pZoneTHImg_Mid2[tY%iHeight*iWidth_Mid+k] + iOutside_Offset ;
							}
							else {
								pRealZoneMaskL[j*ZONE_MASK_WIDTH+x+k] = pZoneTHImg_Mid[tY%iHeight*iWidth_Mid+k] ;
								pRealZoneMaskL2[j*ZONE_MASK_WIDTH+x+k] = pZoneTHImg_Mid2[tY%iHeight*iWidth_Mid+k] ;
							}
						}

						for (int k = 0; k < iWidth_Mid; k++) {
							if (pZoneTHImg_Mid_High[tY%iHeight*iWidth_Mid+k] < (255 - iOutside_Offset)) {
								pRealZoneMaskH[j*ZONE_MASK_WIDTH+x+k] = pZoneTHImg_Mid_High[tY%iHeight*iWidth_Mid+k] + iOutside_Offset ;
								pRealZoneMaskH2[j*ZONE_MASK_WIDTH+x+k] = pZoneTHImg_Mid_High2[tY%iHeight*iWidth_Mid+k] + iOutside_Offset ;
							}
							else {
								pRealZoneMaskH[j*ZONE_MASK_WIDTH+x+k] = pZoneTHImg_Mid_High[tY%iHeight*iWidth_Mid+k] ;
								pRealZoneMaskH2[j*ZONE_MASK_WIDTH+x+k] = pZoneTHImg_Mid_High2[tY%iHeight*iWidth_Mid+k] ;
							}
						}
						//*/
				}
				else {
					CopyMemory(&(pRealZoneMaskL[j*ZONE_MASK_WIDTH+x]), &(pZoneTHImg_Mid[tY%iHeight*iWidth_Mid]), iWidth_Mid) ;
					CopyMemory(&(pRealZoneMaskH[j*ZONE_MASK_WIDTH+x]), &(pZoneTHImg_Mid_High[tY%iHeight*iWidth_Mid]), iWidth_Mid) ;

					CopyMemory(&(pRealZoneMaskL2[j*ZONE_MASK_WIDTH+x]), &(pZoneTHImg_Mid2[tY%iHeight*iWidth_Mid]), iWidth_Mid) ;
					CopyMemory(&(pRealZoneMaskH2[j*ZONE_MASK_WIDTH+x]), &(pZoneTHImg_Mid_High2[tY%iHeight*iWidth_Mid]), iWidth_Mid) ;
				}
			}
			else {
				//*/
				CopyMemory(&(pRealZoneMaskL[j*ZONE_MASK_WIDTH+x]), &(pZoneTHImg_Mid[tY%iHeight*iWidth_Mid]), iWidth_Mid) ;
				CopyMemory(&(pRealZoneMaskH[j*ZONE_MASK_WIDTH+x]), &(pZoneTHImg_Mid_High[tY%iHeight*iWidth_Mid]), iWidth_Mid) ;

				CopyMemory(&(pRealZoneMaskL2[j*ZONE_MASK_WIDTH+x]), &(pZoneTHImg_Mid2[tY%iHeight*iWidth_Mid]), iWidth_Mid) ;
				CopyMemory(&(pRealZoneMaskH2[j*ZONE_MASK_WIDTH+x]), &(pZoneTHImg_Mid_High2[tY%iHeight*iWidth_Mid]), iWidth_Mid) ;
			}
		}


		fX += pmResult->dVectorxx[vN] ;
		fY += pmResult->dVectorxy[vN] ;

		if (pmResult->iTempCount < 200) {
			if (GetPrivateProfileInt("CROSS", "show", 0, "D:\\DDMCore\\DDMCore.ini")) {
				if (x + pmResult->iOrgPosX < left + iGenerateWidth &&
					x + pmResult->iOrgPosX < m_iImageWidth) {
						pmResult->iMatchx_TEMP[pmResult->iTempCount] = x + pmResult->iOrgPosX ;
						pmResult->iMatchy_TEMP[pmResult->iTempCount] = y + pmResult->iOrgPosY ;

						pmResult->iTempCount++ ;
				}
			}
		}
	}

	return TRUE ;
}

BOOL CDetectionAlgorithm::MakeMacroImage(__m256i *pImg, BYTE *pImage, int iWidth, int iHeight, int iMacroRatio)
{
	int RPOCESS_WIDTH_32 = iWidth/16;
	int CAMERA_CCD_SIZE = iWidth, FRAME_HEIGHT = iHeight;
	int MACRO_RATIO = iMacroRatio;

	INT16 *pTemp = NULL;

	int x, y;
	int yGroup;

	__m256i	*_pResult;
	__m256i	*pResult;
	__m256i	xImg, xImg16;
	__m256i xResult;
	__m256i xZeros;
	__m256i xSum;

	xZeros = _mm256_set1_epi8(0) ;

	//INT16 _pTemp[0x6009];
	INT16 _pTemp[0x17700];

	pTemp = reinterpret_cast<INT16 *>(((reinterpret_cast<INT64>(_pTemp)) & ~(0x0F))+16);
	_pResult = reinterpret_cast<__m256i*>(pTemp);

	for (yGroup = 0; yGroup < (int)(FRAME_HEIGHT/MACRO_RATIO); yGroup++) 
	{
		ZeroMemory(pTemp, sizeof(INT16)*CAMERA_CCD_SIZE) ;

		for (y = 0; y < MACRO_RATIO; y++) 
		{
			pResult = _pResult;

			for (x = 0; x < RPOCESS_WIDTH_32; x++) 
			{
				xImg = _mm256_load_si256(pImg++) ;
				xResult = _mm256_loadu_si256(pResult) ;
				_mm256_store_si256(pResult++, _mm256_add_epi16(xResult, _mm256_unpacklo_epi8(xImg, xZeros))) ;
				xResult = _mm256_loadu_si256(pResult) ;
				_mm256_store_si256(pResult++, _mm256_add_epi16(xResult, _mm256_unpackhi_epi8(xImg, xZeros))) ;
			}
		}

		pResult = _pResult;

		for (x = 0; x < (int)(CAMERA_CCD_SIZE/MACRO_RATIO); ++x)
		{
			xSum = xZeros;
			//for (y=0; y < 8; ++y)
			for (y=0; y < 4; ++y)
			{
				xImg16 = _mm256_loadu_si256(pResult++);
				xSum = _mm256_add_epi32(xSum, _mm256_unpacklo_epi16(xImg16, xZeros));
				xSum = _mm256_add_epi32(xSum, _mm256_unpackhi_epi16(xImg16, xZeros));
			}
			pImage[yGroup*((int)(CAMERA_CCD_SIZE/MACRO_RATIO))+x] = static_cast<BYTE>((xSum.m256i_u32[0]+xSum.m256i_u32[1]+xSum.m256i_u32[2]+xSum.m256i_u32[3]) >> 10);
		}
	}

	return TRUE ;
}

BOOL CDetectionAlgorithm::SaveAsBMP(CString strFileName, BYTE *pImg, int iWidth, int iHeight, BOOL bFLR, BOOL bFUD)
{
	BITMAPFILEHEADER	hFile;
	BITMAPINFOHEADER	hInfo;
	RGBQUAD				hRGB[256];
	CFile				FileImg;

	int i, j;
	ZeroMemory(hRGB, sizeof(RGBQUAD)*256);

	for (i=0; i<256; i++)
	{
		hRGB[i].rgbBlue = i;
		hRGB[i].rgbRed = i;
		hRGB[i].rgbGreen = i;
		hRGB[i].rgbReserved = 0;
	}

	int iWidth4 = iWidth & ~(0x03);
	if (iWidth & 0x03) iWidth4+=4;

	hFile.bfType = 0x4d42;
	hFile.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256;
	hFile.bfSize = iWidth4*iHeight+hFile.bfOffBits;
	hFile.bfReserved1 = 0;
	hFile.bfReserved2 = 0;

	hInfo.biSize = sizeof(BITMAPINFOHEADER);
	hInfo.biWidth = iWidth;
	hInfo.biHeight = iHeight;
	hInfo.biPlanes = 1;
	hInfo.biBitCount = 8;
	hInfo.biCompression = 0; 
	hInfo.biSizeImage = 0;
	hInfo.biXPelsPerMeter = 0;
	hInfo.biYPelsPerMeter = 0;
	hInfo.biClrUsed = 0;
	hInfo.biClrImportant = 0;

	if (!FileImg.Open((LPCTSTR)strFileName, CFile::modeCreate|CFile::modeWrite)) return FALSE;

	FileImg.Write(&hFile, sizeof(BITMAPFILEHEADER));
	FileImg.Write(&hInfo, sizeof(BITMAPINFOHEADER));
	FileImg.Write(hRGB, sizeof(RGBQUAD)*256);

	if (bFLR) {
		BYTE *pTemp = NULL ;
		pTemp = new BYTE[iWidth4] ;

		if (pTemp == NULL) {
			return 0 ;
		}

		ZeroMemory(pTemp, sizeof(BYTE) * iWidth4) ;

		if (bFUD) {
			for (i = 0; i < iHeight; ++i) {
				for (j = 0; j < iWidth; ++j) {
					pTemp[j] = pImg[iWidth*i+iWidth-j-1] ;
				}
				FileImg.Write(pTemp, iWidth4) ;
			}
		}
		else {
			for (i = 0; i < iHeight; ++i) {
				for (j = 0; j < iWidth; ++j) {
					pTemp[j] = pImg[iWidth*(iHeight-i-1)+iWidth-j-1] ;
				}
				FileImg.Write(pTemp, iWidth4) ;
			}
		}

		if (pTemp != NULL) {
			delete[] pTemp ;
		}
	}
	else {
		if (bFUD) {
			for (i=0; i<iHeight; ++i)
			{
				FileImg.Write(&(pImg[iWidth*i]), iWidth4);
			}
		}
		else
		{
			for (i=0; i<iHeight; ++i)
			{
				FileImg.Write(&(pImg[iWidth*(iHeight-i-1)]), iWidth4);
			}
		}
	}

	FileImg.Close() ;

	return TRUE ;
}

BOOL CDetectionAlgorithm::SobelEdge( unsigned char *pSour, unsigned char *pDest, int iWidth, int iHeight, int nPitch)
{
	if( pDest == NULL )
	{
		return FALSE;
	}

	if( pSour == NULL )
	{
		return FALSE;
	}

	if( iWidth <= 3 || iHeight <= 3 )
	{
		return FALSE;
	}

	int y=0, x=0, i=0, SSE_PROCESS_PIXEL = 16;
	int iLoopx = iWidth / (SSE_PROCESS_PIXEL - 2 );
	int iRemain = iWidth % ( SSE_PROCESS_PIXEL - 2 );

	if(iRemain < 2)
		iLoopx--;

	__m256i		Current, Down, Up, ZeroMask;

	Current		= _mm256_setzero_si256();
	Down		= _mm256_setzero_si256();
	Up			= _mm256_setzero_si256();
	ZeroMask	= _mm256_setzero_si256();

	BYTE	byteTemp = 0;

	for( y = 1 ; y < iHeight - 1 ; ++y )
	{
		for( x = 0 ; x < iLoopx ; ++x )
		{
			__m256i		UpHigh, UpLow, UpShiftLeft, UpShiftRight, UpShiftLeftHigh, UpShiftLeftLow, UpShiftRightHigh, UpShiftRightLow; 
			__m256i		CurrentShiftLeft, CurrentShiftRight, CurrentShiftLeftHigh, CurrentShiftLeftLow, CurrentShiftRightHigh, CurrentShiftRightLow; 
			__m256i		DownHigh, DownLow, DownShiftLeft, DownShiftRight, DownShiftLeftHigh, DownShiftLeftLow, DownShiftRightHigh, DownShiftRightLow; 
			__m256i		VResultRowHigh, VResultRowLow, HResultRowHigh, HResultRowLow, Result;
			__m256i		Temp1, Temp2, Temp3;

			Up				= _mm256_loadu_si256( reinterpret_cast<__m256i *>( pSour + (y-1) * nPitch + x * ( SSE_PROCESS_PIXEL - 2 ) ) );
			Current			= _mm256_loadu_si256( reinterpret_cast<__m256i *>( pSour + y * nPitch + x * ( SSE_PROCESS_PIXEL - 2 ) ) );
			Down			= _mm256_loadu_si256( reinterpret_cast<__m256i *>( pSour + (y+1) * nPitch + x * ( SSE_PROCESS_PIXEL - 2 ) ) );

			UpShiftLeft			= _mm256_slli_si256( Up, 1 );
			UpShiftLeftHigh		= _mm256_unpackhi_epi8( UpShiftLeft, ZeroMask );
			UpShiftLeftLow		= _mm256_unpacklo_epi8( UpShiftLeft, ZeroMask );

			UpShiftRight		= _mm256_srli_si256( Up, 1 );
			UpShiftRightHigh	= _mm256_unpackhi_epi8( UpShiftRight, ZeroMask );
			UpShiftRightLow		= _mm256_unpacklo_epi8( UpShiftRight, ZeroMask );	

			VResultRowHigh		= _mm256_sub_epi16( UpShiftRightHigh, UpShiftLeftHigh);
			VResultRowLow		= _mm256_sub_epi16( UpShiftRightLow, UpShiftLeftLow);

			CurrentShiftLeft		= _mm256_slli_si256( Current, 1 );
			CurrentShiftLeftHigh	= _mm256_unpackhi_epi8( CurrentShiftLeft, ZeroMask );
			CurrentShiftLeftLow	= _mm256_unpacklo_epi8( CurrentShiftLeft, ZeroMask );

			CurrentShiftRight	= _mm256_srli_si256( Current, 1 );
			CurrentShiftRightHigh	= _mm256_unpackhi_epi8( CurrentShiftRight, ZeroMask );
			CurrentShiftRightLow	= _mm256_unpacklo_epi8( CurrentShiftRight, ZeroMask );

			VResultRowHigh		= _mm256_add_epi16( _mm256_sub_epi16( _mm256_add_epi16( CurrentShiftRightHigh, CurrentShiftRightHigh ), _mm256_add_epi16( CurrentShiftLeftHigh, CurrentShiftLeftHigh )), VResultRowHigh );
			VResultRowLow		= _mm256_add_epi16( _mm256_sub_epi16( _mm256_add_epi16( CurrentShiftRightLow, CurrentShiftRightLow ), _mm256_add_epi16( CurrentShiftLeftLow, CurrentShiftLeftLow )), VResultRowLow );

			DownShiftLeft		= _mm256_slli_si256( Down, 1 );
			DownShiftLeftHigh	= _mm256_unpackhi_epi8( DownShiftLeft, ZeroMask );
			DownShiftLeftLow	= _mm256_unpacklo_epi8( DownShiftLeft, ZeroMask );

			DownShiftRight		= _mm256_srli_si256( Down, 1 );
			DownShiftRightHigh	= _mm256_unpackhi_epi8( DownShiftRight, ZeroMask );
			DownShiftRightLow	= _mm256_unpacklo_epi8( DownShiftRight, ZeroMask );	

			VResultRowHigh		= _mm256_add_epi16( _mm256_sub_epi16( DownShiftRightHigh, DownShiftLeftHigh ), VResultRowHigh );
			VResultRowLow		= _mm256_add_epi16( _mm256_sub_epi16( DownShiftRightLow, DownShiftLeftLow ), VResultRowLow );

			Temp1			= _mm256_cmpgt_epi16( ZeroMask, VResultRowHigh );
			Temp2			= _mm256_andnot_si256( VResultRowHigh, Temp1 );
			Temp3			= _mm256_andnot_si256( Temp1, VResultRowHigh );
			VResultRowHigh		= _mm256_or_si256( Temp2, Temp3 );

			Temp1			= _mm256_cmpgt_epi16( ZeroMask, VResultRowLow );
			Temp2			= _mm256_andnot_si256( VResultRowLow, Temp1 );
			Temp3			= _mm256_andnot_si256( Temp1, VResultRowLow );
			VResultRowLow	= _mm256_or_si256( Temp2, Temp3 );

			UpHigh			= _mm256_unpackhi_epi8( Up, ZeroMask );
			UpLow			= _mm256_unpacklo_epi8( Up, ZeroMask );
			HResultRowHigh		= _mm256_sub_epi16( ZeroMask, _mm256_add_epi16( _mm256_add_epi16( UpShiftLeftHigh, UpShiftRightHigh ), _mm256_add_epi16( UpHigh, UpHigh ) ) );
			HResultRowLow		= _mm256_sub_epi16( ZeroMask, _mm256_add_epi16( _mm256_add_epi16( UpShiftLeftLow, UpShiftRightLow ), _mm256_add_epi16( UpLow, UpLow ) ) );

			DownHigh		= _mm256_unpackhi_epi8( Down, ZeroMask );
			DownLow			= _mm256_unpacklo_epi8( Down, ZeroMask );
			HResultRowHigh		= _mm256_add_epi16( _mm256_add_epi16( _mm256_add_epi16( DownShiftLeftHigh, DownShiftRightHigh ), _mm256_add_epi16( DownHigh, DownHigh ) ), HResultRowHigh );
			HResultRowLow		= _mm256_add_epi16( _mm256_add_epi16( _mm256_add_epi16( DownShiftLeftLow, DownShiftRightLow ), _mm256_add_epi16( DownLow, DownLow ) ), HResultRowLow );

			Temp1			= _mm256_cmpgt_epi16( ZeroMask, HResultRowHigh );
			Temp2			= _mm256_andnot_si256( HResultRowHigh, Temp1 );
			Temp3			= _mm256_andnot_si256( Temp1, HResultRowHigh );
			HResultRowHigh		= _mm256_or_si256( Temp2, Temp3 );

			Temp1			= _mm256_cmpgt_epi16( ZeroMask, HResultRowLow );
			Temp2			= _mm256_andnot_si256( HResultRowLow, Temp1 );
			Temp3			= _mm256_andnot_si256( Temp1, HResultRowLow );
			HResultRowLow		= _mm256_or_si256( Temp2, Temp3 );

			VResultRowHigh		= _mm256_adds_epu16( VResultRowHigh, HResultRowHigh );
			VResultRowLow		= _mm256_adds_epu16( VResultRowLow, HResultRowLow );

			Result			= _mm256_srli_si256( _mm256_packus_epi16( VResultRowLow, VResultRowHigh ), 1 );

			_mm256_storeu_si256( reinterpret_cast< __m256i * >( pDest + y * nPitch + x * ( SSE_PROCESS_PIXEL - 2 ) + 1 ), Result );
		}


		if( iRemain )
		{			
			__m256i		UpHigh, UpLow, UpShiftLeft, UpShiftRight, UpShiftLeftHigh, UpShiftLeftLow, UpShiftRightHigh, UpShiftRightLow; 
			__m256i		CurrentShiftLeft, CurrentShiftRight, CurrentShiftLeftHigh, CurrentShiftLeftLow, CurrentShiftRightHigh, CurrentShiftRightLow; 
			__m256i		DownHigh, DownLow, DownShiftLeft, DownShiftRight, DownShiftLeftHigh, DownShiftLeftLow, DownShiftRightHigh, DownShiftRightLow; 
			__m256i		VResultRowHigh, VResultRowLow, HResultRowHigh, HResultRowLow, Result;
			__m256i		Temp1, Temp2, Temp3;

			Up			= _mm256_loadu_si256( reinterpret_cast<__m256i *>( pSour + (y-1) * nPitch + iWidth - SSE_PROCESS_PIXEL) );
			Current		= _mm256_loadu_si256( reinterpret_cast<__m256i *>( pSour + y * nPitch + iWidth - SSE_PROCESS_PIXEL) );
			Down		= _mm256_loadu_si256( reinterpret_cast<__m256i *>( pSour + (y+1) * nPitch + iWidth - SSE_PROCESS_PIXEL) );

			UpShiftLeft			= _mm256_slli_si256( Up, 1 );
			UpShiftLeftHigh		= _mm256_unpackhi_epi8( UpShiftLeft, ZeroMask );
			UpShiftLeftLow		= _mm256_unpacklo_epi8( UpShiftLeft, ZeroMask );

			UpShiftRight		= _mm256_srli_si256( Up, 1 );
			UpShiftRightHigh	= _mm256_unpackhi_epi8( UpShiftRight, ZeroMask );
			UpShiftRightLow		= _mm256_unpacklo_epi8( UpShiftRight, ZeroMask );	

			VResultRowHigh		= _mm256_sub_epi16( UpShiftLeftHigh, UpShiftRightHigh );
			VResultRowLow		= _mm256_sub_epi16( UpShiftLeftLow, UpShiftRightLow );

			CurrentShiftLeft	= _mm256_slli_si256( Current, 1 );
			CurrentShiftLeftHigh	= _mm256_unpackhi_epi8( CurrentShiftLeft, ZeroMask );
			CurrentShiftLeftLow	= _mm256_unpacklo_epi8( CurrentShiftLeft, ZeroMask );

			CurrentShiftRight	= _mm256_srli_si256( Current, 1 );
			CurrentShiftRightHigh	= _mm256_unpackhi_epi8( CurrentShiftRight, ZeroMask );
			CurrentShiftRightLow	= _mm256_unpacklo_epi8( CurrentShiftRight, ZeroMask );

			VResultRowHigh	= _mm256_add_epi16( _mm256_sub_epi16( _mm256_add_epi16( CurrentShiftLeftHigh, CurrentShiftLeftHigh ), _mm256_add_epi16( CurrentShiftRightHigh, CurrentShiftRightHigh ) ), VResultRowHigh );
			VResultRowLow	= _mm256_add_epi16( _mm256_sub_epi16( _mm256_add_epi16( CurrentShiftLeftLow, CurrentShiftLeftLow ), _mm256_add_epi16( CurrentShiftRightLow, CurrentShiftRightLow ) ), VResultRowLow );

			DownShiftLeft		= _mm256_slli_si256( Down, 1 );
			DownShiftLeftHigh	= _mm256_unpackhi_epi8( DownShiftLeft, ZeroMask );
			DownShiftLeftLow	= _mm256_unpacklo_epi8( DownShiftLeft, ZeroMask );

			DownShiftRight		= _mm256_srli_si256( Down, 1 );
			DownShiftRightHigh	= _mm256_unpackhi_epi8( DownShiftRight, ZeroMask );
			DownShiftRightLow	= _mm256_unpacklo_epi8( DownShiftRight, ZeroMask );	

			VResultRowHigh		= _mm256_add_epi16( _mm256_sub_epi16( DownShiftLeftHigh, DownShiftRightHigh ), VResultRowHigh );
			VResultRowLow		= _mm256_add_epi16( _mm256_sub_epi16( DownShiftLeftLow, DownShiftRightLow ), VResultRowLow );

			Temp1			= _mm256_cmpgt_epi16( ZeroMask, VResultRowHigh );
			Temp2			= _mm256_andnot_si256( VResultRowHigh, Temp1 );
			Temp3			= _mm256_andnot_si256( Temp1, VResultRowHigh );
			VResultRowHigh		= _mm256_or_si256( Temp2, Temp3 );

			Temp1			= _mm256_cmpgt_epi16( ZeroMask, VResultRowLow );
			Temp2			= _mm256_andnot_si256( VResultRowLow, Temp1 );
			Temp3			= _mm256_andnot_si256( Temp1, VResultRowLow );
			VResultRowLow		= _mm256_or_si256( Temp2, Temp3 );

			UpHigh			= _mm256_unpackhi_epi8( Up, ZeroMask );
			UpLow			= _mm256_unpacklo_epi8( Up, ZeroMask );
			HResultRowHigh		= _mm256_sub_epi16( ZeroMask, _mm256_add_epi16( _mm256_add_epi16( UpShiftLeftHigh, UpShiftRightHigh ), _mm256_add_epi16( UpHigh, UpHigh ) ) );
			HResultRowLow		= _mm256_sub_epi16( ZeroMask, _mm256_add_epi16( _mm256_add_epi16( UpShiftLeftLow, UpShiftRightLow ), _mm256_add_epi16( UpLow, UpLow ) ) );

			DownHigh		= _mm256_unpackhi_epi8( Down, ZeroMask );
			DownLow			= _mm256_unpacklo_epi8( Down, ZeroMask );
			HResultRowHigh		= _mm256_add_epi16( _mm256_add_epi16( _mm256_add_epi16( DownShiftLeftHigh, DownShiftRightHigh ), _mm256_add_epi16( DownHigh, DownHigh ) ), HResultRowHigh );
			HResultRowLow		= _mm256_add_epi16( _mm256_add_epi16( _mm256_add_epi16( DownShiftLeftLow, DownShiftRightLow ), _mm256_add_epi16( DownLow, DownLow ) ), HResultRowLow );

			Temp1			= _mm256_cmpgt_epi16( ZeroMask, HResultRowHigh );
			Temp2			= _mm256_andnot_si256( HResultRowHigh, Temp1 );
			Temp3			= _mm256_andnot_si256( Temp1, HResultRowHigh );
			HResultRowHigh		= _mm256_or_si256( Temp2, Temp3 );

			Temp1			= _mm256_cmpgt_epi16( ZeroMask, HResultRowLow );
			Temp2			= _mm256_andnot_si256( HResultRowLow, Temp1 );
			Temp3			= _mm256_andnot_si256( Temp1, HResultRowLow );
			HResultRowLow		= _mm256_or_si256( Temp2, Temp3 );

			VResultRowHigh		= _mm256_adds_epu16( VResultRowHigh, HResultRowHigh );
			VResultRowLow		= _mm256_adds_epu16( VResultRowLow, HResultRowLow );

			//Result			= _mm256_srli_si256( _mm256_packus_epi16( VResultRowLow, VResultRowHigh ), 1 );
			Result			= _mm256_packus_epi16( VResultRowLow, VResultRowHigh );
			byteTemp = pDest[y * nPitch + iWidth  - SSE_PROCESS_PIXEL];

			_mm256_storeu_si256( reinterpret_cast< __m256i * >( pDest + y * nPitch + iWidth - SSE_PROCESS_PIXEL), Result );

			pDest[y * nPitch + iWidth  - SSE_PROCESS_PIXEL] = byteTemp;
			pDest[y * nPitch + iWidth  - 1] = pDest[0];
		}
	}

	return TRUE;
}
// Listbox Control Horizontal Scroller Add
BOOL CDetectionAlgorithm::TrackingHoriFilter(int _iCurPosX, int _iCurPosY, int _iSearchRange, const int _constDNCap)
{
	int CAMERA_CCD_SIZE = m_iImageWidth;
	int FRAME_HEIGHT = m_iImageHeight;
	BYTE *pCurImg = m_pImage;
	const int constCompareArea = _iSearchRange + 1;		// 10Pixel 이면 11
	int iException = constCompareArea + 1;
	// 	int iCureDN = pCurImg[ (_iCurPosY) * CAMERA_CCD_SIZE + _iCurPosX ];
	int j = 0;

	BOOL bLeft = TRUE;	//방향 판단 True 면 왼쪽으로

	if (_iCurPosX <= iException || _iCurPosX >= CAMERA_CCD_SIZE - iException)
		return FALSE;

	for (int i = 1; i < constCompareArea; i++)
	{
		if (bLeft && _constDNCap > abs(pCurImg[(_iCurPosY)* CAMERA_CCD_SIZE + _iCurPosX - i + 1] - pCurImg[(_iCurPosY)* CAMERA_CCD_SIZE + _iCurPosX - i]))
		{
			j = i;
			continue;
		}
		else
		{
			bLeft = FALSE;
			if (_constDNCap > abs(pCurImg[(_iCurPosY)* CAMERA_CCD_SIZE + _iCurPosX + i - j - 1] - pCurImg[(_iCurPosY)* CAMERA_CCD_SIZE + _iCurPosX + i - j]))
			{
				continue;
			}
			else	return FALSE;
		}
	}
	return TRUE;
}

BOOL CDetectionAlgorithm::TrackingVertFilter(int _iCurPosX, int _iCurPosY, int _iSearchRange, const int _constDNCap)
{
	int CAMERA_CCD_SIZE = m_iImageWidth;
	int FRAME_HEIGHT = m_iImageHeight;
	BYTE *pCurImg = m_pImage;
	const int constCompareArea = _iSearchRange + 1;		// 10Pixel 이면 11
	int iException = constCompareArea + 1;
	// 	int iCureDN = pCurImg[ (_iCurPosY) * CAMERA_CCD_SIZE + _iCurPosX ];
	int j = 0;

	BOOL bTop = TRUE;	//방향 판단 True 면 위쪽으로

	if (_iCurPosY <= iException || _iCurPosY >= FRAME_HEIGHT - iException)
		return FALSE;

	for (int i = 1; i < constCompareArea; i++)
	{
		if (bTop && _constDNCap > abs(pCurImg[(_iCurPosY - i + 1) * CAMERA_CCD_SIZE + _iCurPosX] - pCurImg[(_iCurPosY - i) * CAMERA_CCD_SIZE + _iCurPosX]))
		{
			j = i;
			continue;
		}
		else
		{
			bTop = FALSE;
			if (_constDNCap > abs(pCurImg[(_iCurPosY + i - j - 1) * CAMERA_CCD_SIZE + _iCurPosX] - pCurImg[(_iCurPosY + i - j) * CAMERA_CCD_SIZE + _iCurPosX]))
			{
				continue;
			}
			else	return FALSE;
		}
	}
	return TRUE;
}

BOOL CDetectionAlgorithm::TrackingLT2RBFilter(int _iCurPosX, int _iCurPosY, int _iSearchRange, const int _constDNCap)
{
	int CAMERA_CCD_SIZE = m_iImageWidth;
	int FRAME_HEIGHT = m_iImageHeight;
	BYTE *pCurImg = m_pImage;
	const int constCompareArea = _iSearchRange + 1;		// 10Pixel 이면 11
	int iException = constCompareArea + 1;
	// 	int iCureDN = pCurImg[ (_iCurPosY) * CAMERA_CCD_SIZE + _iCurPosX ];
	int j = 0;

	BOOL bLeft = TRUE;	//방향 판단 True 면 LT로

	if (_iCurPosX <= iException || _iCurPosX >= CAMERA_CCD_SIZE - iException)
		return FALSE;
	if (_iCurPosY <= iException || _iCurPosY >= FRAME_HEIGHT - iException)
		return FALSE;

	for (int i = 1; i < constCompareArea; i++)
	{
		if (bLeft && _constDNCap > abs(pCurImg[(_iCurPosY - i + 1)* CAMERA_CCD_SIZE + _iCurPosX - i + 1] - pCurImg[(_iCurPosY - i)* CAMERA_CCD_SIZE + _iCurPosX - i]))
		{
			j = i;
			continue;
		}
		else
		{
			bLeft = FALSE;
			if (_constDNCap > abs(pCurImg[(_iCurPosY + i - j - 1)* CAMERA_CCD_SIZE + _iCurPosX + i - j - 1] - pCurImg[(_iCurPosY + i - j)* CAMERA_CCD_SIZE + _iCurPosX + i - j]))
			{
				continue;
			}
			else	return FALSE;
		}
	}
	return TRUE;
}

BOOL CDetectionAlgorithm::TrackingRT2LBFilter(int _iCurPosX, int _iCurPosY, int _iSearchRange, const int _constDNCap)
{
	int CAMERA_CCD_SIZE = m_iImageWidth;
	int FRAME_HEIGHT = m_iImageHeight;
	BYTE *pCurImg = m_pImage;
	const int constCompareArea = _iSearchRange + 1;		// 10Pixel 이면 11
	int iException = constCompareArea + 1;
	// 	int iCureDN = pCurImg[ (_iCurPosY) * CAMERA_CCD_SIZE + _iCurPosX ];
	int j = 0;

	BOOL bTop = TRUE;	//방향 판단 True 면 RT로

	if (_iCurPosX <= iException || _iCurPosX >= CAMERA_CCD_SIZE - iException)
		return FALSE;
	if (_iCurPosY <= iException || _iCurPosY >= FRAME_HEIGHT - iException)
		return FALSE;

	for (int i = 1; i < constCompareArea; i++)
	{
		if (bTop && _constDNCap > abs(pCurImg[(_iCurPosY - i + 1)* CAMERA_CCD_SIZE + _iCurPosX + i - 1] - pCurImg[(_iCurPosY - i)* CAMERA_CCD_SIZE + _iCurPosX + i]))
		{
			j = i;
			continue;
		}
		else
		{
			bTop = FALSE;
			if (_constDNCap > abs(pCurImg[(_iCurPosY + i - j - 1)* CAMERA_CCD_SIZE + _iCurPosX - i + j + 1] - pCurImg[(_iCurPosY + i - j)* CAMERA_CCD_SIZE + _iCurPosX - i + j]))
			{
				continue;
			}
			else	return FALSE;
		}
	}
	return TRUE;
}

void CDetectionAlgorithm::EdgeSlopeFilter(EdgeSlopeFilterParam stEdgeSlopeFilter)
{
	if (stEdgeSlopeFilter.bEnableEdgeSlope)
	{
		// Slope Filter
		if (stEdgeSlopeFilter.xResult->m256i_u16[0] + stEdgeSlopeFilter.xResult->m256i_u16[4] + 
			stEdgeSlopeFilter.xResultLow->m256i_u16[0] + stEdgeSlopeFilter.xResultLow->m256i_u16[4] +
			stEdgeSlopeFilter.xResultHigh->m256i_u16[0] + stEdgeSlopeFilter.xResultHigh->m256i_u16[4])
		{
			// 초기화
			__m256i xEdgeJudgment, xEdgeThreshold;
			__m256i xLeft, xRight, xUp, xDown;

			xEdgeJudgment = _mm256_set1_epi8(static_cast<char>(stEdgeSlopeFilter.iEdgeJudgment));

			__m256i xEdgeMask = stEdgeSlopeFilter.xZeros;
			__m256i xEdgeMaskH = stEdgeSlopeFilter.xZeros;
			__m256i xEdgeMaskV = stEdgeSlopeFilter.xZeros;
			__m256i	xResultEdge = stEdgeSlopeFilter.xZeros;
			__m256i xResultEdgeH = stEdgeSlopeFilter.xZeros;
			__m256i xResultEdgeV = stEdgeSlopeFilter.xZeros;
			__m256i xThreshold_UseGrayH = stEdgeSlopeFilter.xZeros;
			__m256i xThreshold_UseGrayV = stEdgeSlopeFilter.xZeros;
			__m256i xDifferenceH = stEdgeSlopeFilter.xMask;
			__m256i xDifferenceV = stEdgeSlopeFilter.xMask;

			// 좌우 한칸 비교
			xLeft = stEdgeSlopeFilter.xZeros;
			xRight = stEdgeSlopeFilter.xZeros;

			xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stEdgeSlopeFilter.pInsp) - 1));
			xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stEdgeSlopeFilter.pInsp) + 1));

			xEdgeMaskH = _mm256_max_epu8(_mm256_subs_epu8(xLeft, xRight), _mm256_subs_epu8(xRight, xLeft));
			xEdgeMaskH = _mm256_subs_epu8(xEdgeMaskH, xEdgeJudgment);
			xEdgeMaskH = _mm256_cmpeq_epi8(xEdgeMaskH, stEdgeSlopeFilter.xZeros);
			xEdgeMaskH = _mm256_andnot_si256(xEdgeMaskH, stEdgeSlopeFilter.xMask);
			xResultEdgeH = _mm256_sad_epu8(xEdgeMaskH, stEdgeSlopeFilter.xZeros);

			// 상하 한칸 비교
			xUp = stEdgeSlopeFilter.xZeros;
			xDown = stEdgeSlopeFilter.xZeros;

			__m256i *pUpInspe = stEdgeSlopeFilter.pInsp - stEdgeSlopeFilter.iMAX_RPOCESS_WIDTH;
			__m256i *pDownInspe = stEdgeSlopeFilter.pInsp + stEdgeSlopeFilter.iMAX_RPOCESS_WIDTH;

			xUp = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pUpInspe)));
			xDown = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(pDownInspe)));

			xEdgeMaskV = _mm256_max_epu8(_mm256_subs_epu8(xUp, xDown), _mm256_subs_epu8(xDown, xUp));
			xEdgeMaskV = _mm256_subs_epu8(xEdgeMaskV, xEdgeJudgment);
			xEdgeMaskV = _mm256_cmpeq_epi8(xEdgeMaskV, stEdgeSlopeFilter.xZeros);
			xEdgeMaskV = _mm256_andnot_si256(xEdgeMaskV, stEdgeSlopeFilter.xMask);
			xResultEdgeV = _mm256_sad_epu8(xEdgeMaskV, stEdgeSlopeFilter.xZeros);			

			// 결과에 다른 Th 보정 재검사
			if (xResultEdgeH.m256i_u16[0] + xResultEdgeH.m256i_u16[4] + xResultEdgeV.m256i_u16[0] + xResultEdgeV.m256i_u16[4])
			{
				xThreshold_UseGrayV = xThreshold_UseGrayH = stEdgeSlopeFilter.xThreshold_UseGray;
				xEdgeThreshold = _mm256_set1_epi8(static_cast<char>(stEdgeSlopeFilter.iEdgeSlopeTh));
				xEdgeMask = _mm256_max_epu8(xEdgeMaskH, xEdgeMaskV);
				xEdgeThreshold = _mm256_and_si256(xEdgeThreshold, xEdgeMask);
				xThreshold_UseGrayV = xThreshold_UseGrayH = _mm256_adds_epu8(stEdgeSlopeFilter.xThreshold_UseGray, xEdgeThreshold);

				if (stEdgeSlopeFilter.iDistV == 0)
				{
					if (stEdgeSlopeFilter.x * SSE_PROCESS_PIXELS - stEdgeSlopeFilter.iDistH < stEdgeSlopeFilter.left)
					{
						xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stEdgeSlopeFilter.pInsp) + stEdgeSlopeFilter.iDistH));
						xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stEdgeSlopeFilter.pInsp) + stEdgeSlopeFilter.iDistH * 2));
					}
					else if (stEdgeSlopeFilter.x * SSE_PROCESS_PIXELS + stEdgeSlopeFilter.iDistH > stEdgeSlopeFilter.right)
					{
						xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stEdgeSlopeFilter.pInsp) - stEdgeSlopeFilter.iDistH));
						xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stEdgeSlopeFilter.pInsp) - stEdgeSlopeFilter.iDistH * 2));
					}
					else
					{
						xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stEdgeSlopeFilter.pInsp) - stEdgeSlopeFilter.iDistH));
						xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stEdgeSlopeFilter.pInsp) + stEdgeSlopeFilter.iDistH));
					}

					if (stEdgeSlopeFilter.bMultiTH)
						xDifferenceH = stEdgeSlopeFilter.xZeros;
					else
					{
						xDifferenceH =
							_mm256_max_epu8(
								_mm256_min_epi8(
									_mm256_subs_epu8(stEdgeSlopeFilter.xImg, xLeft),
									_mm256_subs_epu8(stEdgeSlopeFilter.xImg, xRight)
								),
								_mm256_min_epi8(
									_mm256_subs_epu8(xLeft, stEdgeSlopeFilter.xImg),
									_mm256_subs_epu8(xRight, stEdgeSlopeFilter.xImg)
								)
							);
					}

					xDifferenceH = _mm256_min_epi8(*(stEdgeSlopeFilter.xDifference), xDifferenceH);
					xResultEdgeH = _mm256_subs_epu8(xDifferenceH, xThreshold_UseGrayH);
				}
				else
					xResultEdgeH = stEdgeSlopeFilter.xZeros;

				if (stEdgeSlopeFilter.iDistH == 0)
				{
					xUp = _mm256_loadu_si256(stEdgeSlopeFilter.pUp);
					xDown = _mm256_loadu_si256(stEdgeSlopeFilter.pDown);

					if (stEdgeSlopeFilter.bMultiTH)
						xDifferenceV = stEdgeSlopeFilter.xZeros;
					else
					{
						xDifferenceV =
							_mm256_max_epu8(
								_mm256_min_epi8(
									_mm256_subs_epu8(stEdgeSlopeFilter.xImg, xUp),
									_mm256_subs_epu8(stEdgeSlopeFilter.xImg, xDown)
								),
								_mm256_min_epi8(
									_mm256_subs_epu8(xUp, stEdgeSlopeFilter.xImg),
									_mm256_subs_epu8(xDown, stEdgeSlopeFilter.xImg)
								)
							);
					}

					xDifferenceV = _mm256_min_epi8(*(stEdgeSlopeFilter.xDifference), xDifferenceV);
					xResultEdgeV = _mm256_subs_epu8(xDifferenceV, xThreshold_UseGrayV);
				}
				else
					xResultEdgeV = stEdgeSlopeFilter.xZeros;
			}

			*stEdgeSlopeFilter.xDifference = _mm256_min_epi8(xDifferenceH, xDifferenceV);
			xResultEdge = _mm256_or_si256(xResultEdgeH, xResultEdgeV);
			xResultEdge = _mm256_cmpeq_epi8(xResultEdge, stEdgeSlopeFilter.xZeros);
			*stEdgeSlopeFilter.xDifference = _mm256_subs_epu8(*stEdgeSlopeFilter.xDifference, xResultEdge);

			*stEdgeSlopeFilter.xResult = _mm256_sad_epu8(_mm256_or_si256(xResultEdgeH, xResultEdgeV), stEdgeSlopeFilter.xZeros);
		}
	}
}

void CDetectionAlgorithm::PeriodSlopeFilter(PeriodSlopeFilterParam stPeriodSlopeFilter)
{
	if (stPeriodSlopeFilter.iDistH != 0 && stPeriodSlopeFilter.iDistV == 0)
	{
		if (stPeriodSlopeFilter.bEnablePeriodSlope)
		{
			// Period Slope filter
			if (stPeriodSlopeFilter.xResult->m256i_u16[0] + stPeriodSlopeFilter.xResult->m256i_u16[4] +
				stPeriodSlopeFilter.xResultLow->m256i_u16[0] + stPeriodSlopeFilter.xResultLow->m256i_u16[4] +
				stPeriodSlopeFilter.xResultHigh->m256i_u16[0] + stPeriodSlopeFilter.xResultHigh->m256i_u16[4])
			{
				CString strstr;
				strstr.Format("Judgment : %d,SlopeTh : %d", stPeriodSlopeFilter.iPeriodSlopeJudgment, stPeriodSlopeFilter.iPeriodSlopeTh);
	//			AfxMessageBox(strstr);
				__m256i xJudgment, xJudgmentResult, xEdgeThreshold;
				__m256i xThreshold_UseGrayE = stPeriodSlopeFilter.xZeros;
				__m256i xDifferenceE = stPeriodSlopeFilter.xZeros;
				__m256i xTempImg = stPeriodSlopeFilter.xZeros;


				__m256i xLeft = stPeriodSlopeFilter.xZeros;
				__m256i xRight = stPeriodSlopeFilter.xZeros;

				xTempImg = stPeriodSlopeFilter.xImg;

				__m256i xPeriodJudgment = stPeriodSlopeFilter.xZeros;
				xPeriodJudgment = _mm256_set1_epi8(static_cast<char>(stPeriodSlopeFilter.iPeriodSlopeJudgment));

				if (stPeriodSlopeFilter.x * SSE_PROCESS_PIXELS - stPeriodSlopeFilter.iDistH < stPeriodSlopeFilter.left)
				{
					xLeft = stPeriodSlopeFilter.xImg;
					stPeriodSlopeFilter.xImg = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stPeriodSlopeFilter.pInsp) + stPeriodSlopeFilter.iDistH));
					xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stPeriodSlopeFilter.pInsp) + stPeriodSlopeFilter.iDistH * 2));
				}
				else if (stPeriodSlopeFilter.x * SSE_PROCESS_PIXELS + stPeriodSlopeFilter.iDistH > stPeriodSlopeFilter.right)
				{
					xRight = stPeriodSlopeFilter.xImg;
					stPeriodSlopeFilter.xImg = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stPeriodSlopeFilter.pInsp) - stPeriodSlopeFilter.iDistH));
					xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stPeriodSlopeFilter.pInsp) - stPeriodSlopeFilter.iDistH * 2));
				}
				else
				{
					xLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stPeriodSlopeFilter.pInsp) - stPeriodSlopeFilter.iDistH));
					xRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stPeriodSlopeFilter.pInsp) + stPeriodSlopeFilter.iDistH));
				}

				if (stPeriodSlopeFilter.bMultiTH)
				{
					xDifferenceE = stPeriodSlopeFilter.xZeros;
				}
				else
				{
					xDifferenceE =
						_mm256_max_epu8(
							_mm256_min_epi8(
								_mm256_subs_epu8(stPeriodSlopeFilter.xImg, xLeft),
								_mm256_subs_epu8(stPeriodSlopeFilter.xImg, xRight)
							),
							_mm256_min_epi8(
								_mm256_subs_epu8(xLeft, stPeriodSlopeFilter.xImg),
								_mm256_subs_epu8(xRight, stPeriodSlopeFilter.xImg)
							)
						);

					xDifferenceE = _mm256_min_epi8(*stPeriodSlopeFilter.xDifference, xDifferenceE);

					xJudgment = _mm256_cmpeq_epi8(_mm256_andnot_si256(xDifferenceE, stPeriodSlopeFilter.xMask), stPeriodSlopeFilter.xMask);

					xJudgmentResult = _mm256_sad_epu8(xJudgment, stPeriodSlopeFilter.xZeros);

					xThreshold_UseGrayE = stPeriodSlopeFilter.xThreshold_UseGray;

					if (xJudgmentResult.m256i_u16[0] + xJudgmentResult.m256i_u16[4])
					{
						xEdgeThreshold = _mm256_set1_epi8(static_cast<char>(stPeriodSlopeFilter.iPeriodSlopeTh));
						xEdgeThreshold = _mm256_and_si256(xEdgeThreshold, xJudgment);
						xThreshold_UseGrayE = _mm256_add_epi8(stPeriodSlopeFilter.xThreshold_UseGray, xEdgeThreshold);
					}

					*stPeriodSlopeFilter.xDifference = _mm256_min_epi8(*stPeriodSlopeFilter.xDifference, xDifferenceE);

					*stPeriodSlopeFilter.xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceE, xThreshold_UseGrayE), stPeriodSlopeFilter.xZeros);
				}

				/*
				//////////////////////////////////////////////////////////////////////////
				//  정세진 시작
				if ((stPeriodSlopeFilter.xResult->m256i_u16[0] + stPeriodSlopeFilter.xResult->m256i_u16[4]))
				{
					// 2배 주기 슬롭 확인 및 
					__m256i xMoreLeft, xMoreRight, xDiffSlop;
					__m256i xPeriodSlopeThreshold;
					xPeriodSlopeThreshold = _mm256_set1_epi8(static_cast<char>(stPeriodSlopeFilter.iPeriodSlopeTh));

					if (stPeriodSlopeFilter.x * SSE_PROCESS_PIXELS - stPeriodSlopeFilter.iDistH * 2 > stPeriodSlopeFilter.left &&
						stPeriodSlopeFilter.x * SSE_PROCESS_PIXELS + stPeriodSlopeFilter.iDistH * 2 < stPeriodSlopeFilter.right)
					{
						xMoreLeft = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stPeriodSlopeFilter.pInsp) - stPeriodSlopeFilter.iDistH * 2));
						xMoreRight = _mm256_loadu_si256(reinterpret_cast<__m256i*>(reinterpret_cast<BYTE*>(stPeriodSlopeFilter.pInsp) + stPeriodSlopeFilter.iDistH * 2));

						xEdgeThreshold = _mm256_adds_epi8(xEdgeThreshold, xPeriodSlopeThreshold);

						__m256i xDiffLeft =
							_mm256_max_epu8(
								_mm256_min_epi8(
									_mm256_subs_epu8(xLeft, xMoreLeft),
									_mm256_subs_epu8(xLeft, stPeriodSlopeFilter.xImg)
								),
								_mm256_min_epi8(
									_mm256_subs_epu8(xMoreLeft, xLeft),
									_mm256_subs_epu8(stPeriodSlopeFilter.xImg, xLeft)
								)
							);

						__m256i xDiffRight =
							_mm256_max_epu8(
								_mm256_min_epi8(
									_mm256_subs_epu8(xRight, stPeriodSlopeFilter.xImg),
									_mm256_subs_epu8(xRight, xMoreRight)
								),
								_mm256_min_epi8(
									_mm256_subs_epu8(stPeriodSlopeFilter.xImg, xRight),
									_mm256_subs_epu8(xMoreRight, xRight)
								)
							);

						__m256i xsadLeft = _mm256_sad_epu8(_mm256_min_epi8(xDifferenceE, xDiffLeft), stPeriodSlopeFilter.xZeros);
						__m256i xsadRight = _mm256_sad_epu8(_mm256_min_epi8(xDifferenceE, xDiffRight), stPeriodSlopeFilter.xZeros);

						if (!(xsadLeft.m256i_u16[0] + xsadLeft.m256i_u16[4]))
						{
							xDiffLeft =
								_mm256_max_epu8(
									_mm256_max_epu8(
										_mm256_subs_epu8(xLeft, xMoreLeft),
										_mm256_subs_epu8(xMoreLeft, xLeft)
									),
									_mm256_max_epu8(
										_mm256_subs_epu8(stPeriodSlopeFilter.xImg, xLeft),
										_mm256_subs_epu8(xLeft, stPeriodSlopeFilter.xImg)
									)
								);
							xsadLeft = _mm256_sad_epu8(_mm256_min_epi8(xDifferenceE, xDiffLeft), stPeriodSlopeFilter.xZeros);
							xDiffLeft = _mm256_min_epi8(xDifferenceE, xDiffLeft);
						}
						else
						{
							xDiffLeft = stPeriodSlopeFilter.xZeros;
						}
						
						if (!(xsadRight.m256i_u16[0] + xsadRight.m256i_u16[4]))
						{
							xDiffRight =
								_mm256_max_epu8(
									_mm256_max_epu8(
										_mm256_subs_epu8(xRight, xMoreRight),
										_mm256_subs_epu8(xMoreRight, xRight)
									),
									_mm256_max_epu8(
										_mm256_subs_epu8(stPeriodSlopeFilter.xImg, xRight),
										_mm256_subs_epu8(xRight, stPeriodSlopeFilter.xImg)
									)
								);
							xsadRight = _mm256_sad_epu8(_mm256_min_epi8(xDifferenceE, xDiffRight), stPeriodSlopeFilter.xZeros);
							xDiffRight = _mm256_min_epi8(xDifferenceE, xDiffRight);
						}
						else
						{
							xDiffRight = stPeriodSlopeFilter.xZeros;
						}

						if (xsadLeft.m256i_u8[0] + xsadLeft.m256i_u8[4] + xsadRight.m256i_u8[0] + xsadRight.m256i_u8[4])
						{

							// Slope 이긴 한데 threshold 이하이면 제거 
							xThreshold_UseGrayE = _mm256_adds_epi8(xThreshold_UseGrayE, xPeriodJudgment);

							__m256i xTempLeft = _mm256_subs_epu8(xDiffLeft, xPeriodJudgment);
							__m256i xTempRight = _mm256_subs_epu8(xDiffRight, xPeriodJudgment);

							xDifferenceE = _mm256_min_epi8(xTempLeft, xTempRight);


							*stPeriodSlopeFilter.xDifference = _mm256_min_epi8(*stPeriodSlopeFilter.xDifference, xDifferenceE);
							//*stPeriodSlopeFilter.xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceE, xThreshold_UseGrayE), stPeriodSlopeFilter.xZeros);
							*stPeriodSlopeFilter.xResult = _mm256_sad_epu8(_mm256_subs_epu8(xDifferenceE, *stPeriodSlopeFilter.xDifference), stPeriodSlopeFilter.xZeros);
						}
					}
				}
				stPeriodSlopeFilter.xImg = xTempImg;
				*/
			}
		}
	}
}

BOOL CDetectionAlgorithm::Cell2PixelMergeFilter(int _iCurPosX, int _iCurPosY, int _iRef1PosX, int _iRef1PosY, int _iRef2PosX, int _iRef2PosY, int _iThreshold_UseGray, BYTE* Tar, BYTE* Ref1, BYTE* Ref2)
{

	BYTE *pCurImg;
	BYTE *pRef1Img;
	BYTE *pRef2Img;


	int iCurGray = 0, iRef1Gray = 0, iRef2Gray = 0;
	int iCurSum = 0, iRef1Sum = 0, iRef2Sum = 0;
	int iCurValue, iRef1Value, iRef2Value;
	int iThreshold_UseGray = 0;

	if (_iThreshold_UseGray != 0) 
		iThreshold_UseGray = _iThreshold_UseGray / 4 * 3;
	else
		return TRUE;


	CString str;


	//	int MaskFilter[3][3] = {{1, 4, 1}, {4, 20, 4}, {1, 4, 1}};
	//	int MaskFilter[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
	int iMaskSum = 0;

	int iResult;

	if((_iCurPosX < 3 || _iCurPosX > MAX_CAMERA_CCD_SIZE - 3) || (_iRef1PosX < 3 || _iRef1PosX > MAX_CAMERA_CCD_SIZE - 3) || (_iRef2PosX < 3 || _iRef2PosX > MAX_CAMERA_CCD_SIZE - 3))
		return TRUE;

	if((_iCurPosY < 3 || _iCurPosY > MAX_FRAME_HEIGHT - 3) || (_iRef1PosY < 3 || _iRef1PosY > MAX_FRAME_HEIGHT - 3) || (_iRef2PosY < 3 || _iRef2PosY > MAX_FRAME_HEIGHT - 3))
		return TRUE;

	pCurImg = Tar;
	pRef1Img = Ref1;
	pRef2Img = Ref2;

	//////////////////////////////////////////////////////////////////////////
	//
	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX + 1)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX)];
	if (iCurSum != 0)
		iCurValue = iCurSum / 2;
	else
		return FALSE;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX + 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX + 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 1)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 1)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX + 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX + 2)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX + 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX + 2)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 2)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 2)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


//////////////////////////////////////////////////////////////////////////
	//
	iCurSum = 0;
	iRef1Sum = 0;
	iRef2Sum = 0;

	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX - 1)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX)];
	if (iCurSum != 0)
		iCurValue = iCurSum / 2;
	else
		return FALSE;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX + 1)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX + 1)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 2)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 2)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX + 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX + 2)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX + 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX + 2)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	////////////////jcwjcwjcwjcw////////////////////

	iCurSum = 0;
	iRef1Sum = 0;
	iRef2Sum = 0;
	
	iCurValue = (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX)];
	
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX + 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX + 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 1)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 1)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX + 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX + 2)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX + 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX + 2)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 2)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 2)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}
///	/...
	//////////////////////////////////////////////////////////////////////////
	// 중복 Cur 2 Ref 1

	iCurSum = 0;
	iRef1Sum = 0;
	iRef2Sum = 0;

	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX+1)];
	if (iCurSum != 0)
		iCurValue = iCurSum / 2;
	else
		return FALSE;

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX+1)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX+1)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX-1)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX-1)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX+2)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX+2)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX-2)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX-2)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

		//////////////////////////////////////////////////////////////////////////
	// 중복 Cur 2 Ref 1

	iCurSum = 0;
	iRef1Sum = 0;
	iRef2Sum = 0;

	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX-1)];
	if (iCurSum != 0)
		iCurValue = iCurSum / 2;
	else
		return FALSE;

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX+1)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX+1)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX-1)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX-1)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX-2)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX-2)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX+2)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX+2)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CDetectionAlgorithm::Cell2PixelVertMergeFilter(int _iCurPosX, int _iCurPosY, int _iRef1PosX, int _iRef1PosY, int _iRef2PosX, int _iRef2PosY, int _iThreshold_UseGray)
{
/*
	BYTE *pCurImg;
	BYTE *pRef1Img;
	BYTE *pRef2Img;


	int iCurGray = 0, iRef1Gray = 0, iRef2Gray = 0;
	int iCurSum = 0, iRef1Sum = 0, iRef2Sum = 0;
	int iCurValue, iRef1Value, iRef2Value;
	int iThreshold_UseGray = 0;
	if (_iThreshold_UseGray != 0) 
		iThreshold_UseGray = _iThreshold_UseGray / 4 * 3;
	else
		return TRUE;


	CString str;


	//	int MaskFilter[3][3] = {{1, 4, 1}, {4, 20, 4}, {1, 4, 1}};
	//	int MaskFilter[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
	int iMaskSum = 0;

	int iResult;

	if((_iCurPosX < 3 || _iCurPosX > CAMERA_CCD_SIZE - 3) || (_iRef1PosX < 3 || _iRef1PosX > CAMERA_CCD_SIZE - 3) || (_iRef2PosX < 3 || _iRef2PosX > CAMERA_CCD_SIZE - 3))
		return TRUE;

	if((_iCurPosY < 3 || _iCurPosY > FRAME_HEIGHT - 3) || (_iRef1PosY < 3 || _iRef1PosY > FRAME_HEIGHT - 3) || (_iRef2PosY < 3 || _iRef2PosY > FRAME_HEIGHT - 3))
		return TRUE;

	pCurImg = pFrameImg[_iCurFrameNo];
	pRef1Img = pFrameImg[_iRef1FrameNo];
	pRef2Img = pFrameImg[_iRef2FrameNo];

	//////////////////////////////////////////////////////////////////////////
	// 0 + 1
	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * CAMERA_CCD_SIZE + (_iCurPosX)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY + 1) * CAMERA_CCD_SIZE + (_iCurPosX)];
	if (iCurSum != 0)
		iCurValue = iCurSum / 2;
	else
		return FALSE;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY + 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY + 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;
	// 0 - 1
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;
	// 1 + 2
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY + 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY + 2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY + 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY + 2) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	// -1 - 2
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 2) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	iCurSum = 0;
	iRef1Sum = 0;
	iRef2Sum = 0;

	iCurSum += (unsigned char)pCurImg[(_iCurPosY - 1) * CAMERA_CCD_SIZE + (_iCurPosX)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * CAMERA_CCD_SIZE + (_iCurPosX)];
	if (iCurSum != 0)
		iCurValue = iCurSum / 2;
	else
		return FALSE;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY + 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY + 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 2) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY + 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY + 2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY + 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY + 2) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////jcwjcwjcwjcwjcw///////////////

	iCurSum = 0;
	iRef1Sum = 0;
	iRef2Sum = 0;

	iCurValue = (unsigned char)pCurImg[(_iCurPosY) * CAMERA_CCD_SIZE + (_iCurPosX)];

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY + 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY + 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY + 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY + 2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY + 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY + 2) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	iRef1Sum = 0;
	iRef2Sum = 0;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	if (iRef1Sum != 0)
		iRef1Value = iRef1Sum / 2;
	else
		return FALSE;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 1) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 2) * CAMERA_CCD_SIZE + (_iRef2PosX)];
	if (iRef2Sum != 0)
		iRef2Value = iRef2Sum / 2;
	else
		return FALSE;

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 중복 Cur 2 Ref 1

	iCurSum = 0;
	iRef1Sum = 0;
	iRef2Sum = 0;

	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * CAMERA_CCD_SIZE + (_iCurPosX)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY + 1) * CAMERA_CCD_SIZE + (_iCurPosX)];
	if (iCurSum != 0)
		iCurValue = iCurSum / 2;
	else
		return FALSE;

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * CAMERA_CCD_SIZE + (_iRef2PosX)];

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY+1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY+1) * CAMERA_CCD_SIZE + (_iRef2PosX)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY-1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY-1) * CAMERA_CCD_SIZE + (_iRef2PosX)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY+2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY+2) * CAMERA_CCD_SIZE + (_iRef2PosX)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY-2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY-2) * CAMERA_CCD_SIZE + (_iRef2PosX)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

		//////////////////////////////////////////////////////////////////////////
	// 중복 Cur 2 Ref 1

	iCurSum = 0;
	iRef1Sum = 0;
	iRef2Sum = 0;

	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * CAMERA_CCD_SIZE + (_iCurPosX)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY - 1) * CAMERA_CCD_SIZE + (_iCurPosX)];
	if (iCurSum != 0)
		iCurValue = iCurSum / 2;
	else
		return FALSE;

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY) * CAMERA_CCD_SIZE + (_iRef2PosX)];

	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY+1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY+1) * CAMERA_CCD_SIZE + (_iRef2PosX)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY-1) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY-1) * CAMERA_CCD_SIZE + (_iRef2PosX)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY-2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY-2) * CAMERA_CCD_SIZE + (_iRef2PosX)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//

	iRef1Value = (unsigned char)pRef1Img[(_iRef1PosY+2) * CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef2Value = (unsigned char)pRef2Img[(_iRef2PosY+2) * CAMERA_CCD_SIZE + (_iRef2PosX)];


	iResult = max(min(Subtract(iCurValue, iRef1Value), Subtract(iCurValue, iRef2Value)), min(Subtract(iRef1Value, iCurValue), Subtract(iRef2Value, iCurValue)));

	if (iResult < iThreshold_UseGray)
	{
		return TRUE;
	}
*/
	return FALSE;
}

BOOL CDetectionAlgorithm::Convolution2x2Filter(int _iCurPosX, int _iCurPosY, int _iRef1PosX, int _iRef1PosY, int _iRef2PosX, int _iRef2PosY, int _iThreshold_UseGray, BYTE* Tar, BYTE* Ref1, BYTE* Ref2)
{

	BYTE *pCurImg;
	BYTE *pRef1Img;
	BYTE *pRef2Img;


	int iCurGray = 0, iRef1Gray = 0, iRef2Gray = 0;
	int iCurSum = 0, iRef1Sum = 0, iRef2Sum = 0;
	int iCurValue, iRef1Value, iRef2Value;

	CString str;


//	int MaskFilter[3][3] = {{1, 4, 1}, {4, 20, 4}, {1, 4, 1}};
//	int MaskFilter[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
	int iMaskSum = 0;

	int iResult;

	if((_iCurPosX < 3 || _iCurPosX > MAX_CAMERA_CCD_SIZE - 3) || (_iRef1PosX < 3 || _iRef1PosX > MAX_CAMERA_CCD_SIZE - 3) || (_iRef2PosX < 3 || _iRef2PosX > MAX_CAMERA_CCD_SIZE - 3))
		return TRUE;

	if((_iCurPosY < 3 || _iCurPosY > MAX_FRAME_HEIGHT - 3) || (_iRef1PosY < 3 || _iRef1PosY > MAX_FRAME_HEIGHT - 3) || (_iRef2PosY < 3 || _iRef2PosY > MAX_FRAME_HEIGHT - 3))
		return TRUE;

	pCurImg = Tar;
	pRef1Img = Ref1;
	pRef2Img = Ref2;

	iCurSum += (unsigned char)pCurImg[(_iCurPosY - 1) * MAX_CAMERA_CCD_SIZE + (_iCurPosX - 1)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY - 1) * MAX_CAMERA_CCD_SIZE + (_iCurPosX)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX - 1)];
	iCurSum += (unsigned char)pCurImg[(_iCurPosY) * MAX_CAMERA_CCD_SIZE + (_iCurPosX)];
	iCurValue = iCurSum / 4;

	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 1) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY - 1) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX - 1)];
	iRef1Sum += (unsigned char)pRef1Img[(_iRef1PosY) * MAX_CAMERA_CCD_SIZE + (_iRef1PosX)];
	iRef1Value = iRef1Sum / 4;

	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 1) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY - 1) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX - 1)];
	iRef2Sum += (unsigned char)pRef2Img[(_iRef2PosY) * MAX_CAMERA_CCD_SIZE + (_iRef2PosX)];
	iRef2Value = iRef2Sum / 4;

	iResult = max(min((iCurValue - iRef1Value), (iCurValue - iRef2Value)), min((iRef1Value - iCurValue), (iRef2Value - iCurValue)));

	if (iResult < _iThreshold_UseGray)
	{
		return TRUE;
	}

	return FALSE;
}

int CDetectionAlgorithm::Subtract(int _iGray1, int _iGray2)
{
	if (_iGray1 - _iGray2 < 0)
		return 0;
	else
		return _iGray1 - _iGray2;
}

