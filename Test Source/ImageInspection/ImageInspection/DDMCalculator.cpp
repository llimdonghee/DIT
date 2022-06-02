// DDMCalculator.cpp: 구현 파일
//

#include "stdafx.h"
#include "DDMCalculator.h"
#include "ImageInspectionDlg.h"
#include "afxdialogex.h"


// DDMCalculator 대화 상자
IMPLEMENT_DYNAMIC(DDMCalculator, CDialogEx)
DDMCalculator::DDMCalculator(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DDMCALC_DIALOG, pParent)
	, m_iEditCalcLeft(0)
	, m_iEditCalcRight(0)
	, m_iEditCalcTop(0)
	, m_iEditCalcBottom(0)
{
}

DDMCalculator::~DDMCalculator()
{
}

void DDMCalculator::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CALC_LEFT, m_iEditCalcLeft);
	DDX_Text(pDX, IDC_EDIT_CALC_RIGHT, m_iEditCalcRight);
	DDX_Text(pDX, IDC_EDIT_CALC_TOP, m_iEditCalcTop);
	DDX_Text(pDX, IDC_EDIT_CALC_BOTTOM, m_iEditCalcBottom);
}

BEGIN_MESSAGE_MAP(DDMCalculator, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_RECIPE_FILE, &DDMCalculator::OnBnClickedButtonSelectRecipeFile)
	ON_BN_CLICKED(IDC_BUTTON_COORDINATE_TO_IMAGEPOSITION, &DDMCalculator::OnBnClickedButtonCoordinateToImageposition)
END_MESSAGE_MAP()

// DDMCalculator 메시지 처리기

BOOL DDMCalculator::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Clear();

	return TRUE;
}

void DDMCalculator::Clear()
{
	SetDlgItemInt(IDC_EDIT_X_COORDINATE, 0);
	SetDlgItemInt(IDC_EDIT_Y_COORDINATE, 0);
	SetDlgItemInt(IDC_EDIT_CALC_CAMERA_NO, 0);
	SetDlgItemInt(IDC_EDIT_CALC_SCAN_NO, 0);
	SetDlgItemInt(IDC_EDIT_CALC_FRAME_NO, 0);
	m_vecCamParam.clear();
	m_iGlassOrigin = m_iScanCnt = m_iGlassSizeX = m_iGlassSizeY = m_iFrameNo = 0;
	m_bCenter = FALSE; 
	m_fScanResolution = 1.5081593;
	m_dSpeedRipple = 0.;
	m_fEncoderResolution = 0.;
	m_iCamStage = 256;
}

void DDMCalculator::OnBnClickedButtonSelectRecipeFile()
{
	CImageInspectionDlg pImgInsDlg;
	CString strRecipeFolderName = "", strRecipeFileName = "";
	strRecipeFolderName = pImgInsDlg.FindRecipeFolderShell();
	strRecipeFileName = pImgInsDlg.FindRecipeFileName(strRecipeFolderName);
	if (strRecipeFileName.IsEmpty())
		return;
	Clear();
	LoadRecipe(strRecipeFileName);
	EncoderPositionCalc();
}

void DDMCalculator::LoadRecipe(CString _strRecipeFileName)
{
	CString strTemp = "", strValue = "", strParam = "";
	char chTemp[50] = "";
	m_iGlassOrigin = GetPrivateProfileInt("GLASS PARAMETERS", "GlassOrigin", 0, _strRecipeFileName);
	m_iScanCnt = GetPrivateProfileInt("GLASS PARAMETERS", "ScanCnt", 0, _strRecipeFileName);
	GetPrivateProfileString("EQP PARAMETERS", "ScanResolution", "1.508153", chTemp, 50, _strRecipeFileName);
	strTemp.Format("%s", chTemp);
	m_fScanResolution = atof(strTemp);	
	GetPrivateProfileString("EQP PARAMETERS", "SpeedRipple", "0.0", chTemp, 50, _strRecipeFileName);
	strTemp.Format("%s", chTemp);
	m_dSpeedRipple = atof(strTemp);
	m_iCamStage = GetPrivateProfileInt("CAMERA PARAMETERS", "TDIStageCnt", 256, _strRecipeFileName);

	GetPrivateProfileString("EQP PARAMETERS", "EncoderResolution", "0.0", chTemp, 50, _strRecipeFileName);
	strTemp.Format("%s", chTemp);
	m_fEncoderResolution = atof(strTemp);

	GetPrivateProfileString("GLASS PARAMETERS", "GlassSize", "2500000,2200000", chTemp, 50, _strRecipeFileName);
	strTemp.Format("%s", chTemp);
	AfxExtractSubString(strValue, strTemp, 0, ',');
	m_iGlassSizeX = atoi(strValue);
	AfxExtractSubString(strValue, strTemp, 1, ',');
	m_iGlassSizeY = atoi(strValue);

	m_iCamCnt = GetPrivateProfileInt("CAMERA PARAMETERS", "CamCnt", 0, _strRecipeFileName);
	for (int i = 0; i < m_iCamCnt; i++)
	{
		CamParam camParam;

		strParam.Format("DDMOffset[%d]", i);
		GetPrivateProfileString("CAMERA PARAMETERS", strParam, "0,0", chTemp, 50, _strRecipeFileName);
		strTemp.Format("%s", chTemp);
		AfxExtractSubString(strValue, strTemp, 0, ',');
		camParam.lDDMOffsetX = atoi(strValue);
		AfxExtractSubString(strValue, strTemp, 1, ',');
		camParam.lDDMOffsetY = atoi(strValue);

		strParam.Format("Foward[%d]", i);
		camParam.bForward = GetPrivateProfileInt("CAMERA PARAMETERS", strParam, 0, _strRecipeFileName);

		strParam.Format("DDMBaseOffset[%d]", i);
		GetPrivateProfileString("CAMERA PARAMETERS", strParam, "0,0", chTemp, 50, _strRecipeFileName);
		strTemp.Format("%s", chTemp);
		AfxExtractSubString(strValue, strTemp, 0, ',');
		camParam.lDDMBaseOffsetX = atoi(strValue);
		AfxExtractSubString(strValue, strTemp, 1, ',');
		camParam.lDDMBaseOffsetY = atoi(strValue);

		strParam.Format("ScanLength[%d]", i);
		camParam.iScanLength = GetPrivateProfileInt("CAMERA PARAMETERS", strParam, 0, _strRecipeFileName);

		strParam.Format("ChannelFOV[%d]", i);
		GetPrivateProfileString("CAMERA PARAMETERS", strParam, "1.510,1.510,1.510,1.510", chTemp, 50, _strRecipeFileName);
		strTemp.Format("%s", chTemp);
		AfxExtractSubString(strValue, strTemp, 0, ',');
		camParam.fFOV[0] = atof(strValue);
		AfxExtractSubString(strValue, strTemp, 1, ',');
		camParam.fFOV[1] = atof(strValue);
		AfxExtractSubString(strValue, strTemp, 2, ',');
		camParam.fFOV[2] = atof(strValue);
		AfxExtractSubString(strValue, strTemp, 3, ',');
		camParam.fFOV[3] = atof(strValue);

		m_vecCamParam.push_back(camParam);
		ADDCam(i, camParam.lDDMOffsetX, camParam.lDDMOffsetY, camParam.bForward, camParam.fFOV, camParam.iScanLength, camParam.lDDMBaseOffsetX, camParam.lDDMBaseOffsetY);
	}
}

void DDMCalculator::OnBnClickedButtonCoordinateToImageposition()
{
	if (m_iScanCnt == 0)
		return;
	CString strTemp;
	GetDlgItemText(IDC_EDIT_X_COORDINATE, strTemp);
	int ix = atoi(strTemp);
	GetDlgItemText(IDC_EDIT_Y_COORDINATE, strTemp);
	int iy = atoi(strTemp);
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

	if (m_bCenter)
	{
		x1 = (m_iGlassSizeX / 2) + ix - 200;
		x2 = (m_iGlassSizeX / 2) + ix + 200;
		y1 = (m_iGlassSizeY / 2) + iy - 200;
		y2 = (m_iGlassSizeY / 2) + iy + 200;
	}
	else
	{
		x1 = ix - 200;
		x2 = ix + 200;
		y1 = iy - 200;
		y2 = iy + 200;
	}

	int iPos[2] = { 0, };
	int iCurCamPos = 0;
	int iDDMCnt = 0;
	int iCurDDMNo = 0;
	int iCurScanNo = 0;
	if (x1 > 0 && y1 > 0 && x2 < m_iGlassSizeX && y2 < m_iGlassSizeY)
	{
		if (m_iGlassOrigin % 2 != 0)
		{
			y1 = m_iGlassSizeY - y1;
		}
		for (int i = 0; i < (int)m_vecCamParam.size() - 2; i++)
		{
			iPos[0] = (m_vecCamParam[i].lDDMOffsetX + m_vecCamParam[i + 1].lDDMOffsetX) / 2;
			iPos[1] = (m_vecCamParam[i + 1].lDDMOffsetX + m_vecCamParam[i + 2].lDDMOffsetX) / 2;


			if ((y1 > iPos[0] && y1 < iPos[1]) || (y1 < iPos[0] && y1 > iPos[1]))
			{
				if (m_vecCamParam[i].iScanLength < m_iGlassSizeX)
				{
					if ((x2 + x1) / 2 < m_iGlassSizeX / 2)
					{
						iCurCamPos = m_iCamCnt / 2 + i + 1;
						break;
					}
					else if ((x2 + x1) / 2 > m_iGlassSizeX / 2)
					{
						iCurCamPos = i + 1;
						break;
					}
				}
				else
				{
					iCurCamPos = i + 1;
					break;
				}
			}
		}
	}
	else
	{
		AfxMessageBox("좌표 정보 Reading 실패");
	}
	iDDMCnt = m_iCamCnt / m_iScanCnt;

	iCurDDMNo = iCurCamPos / m_iScanCnt + 1;
	iCurScanNo = iCurCamPos % m_iScanCnt + 1;

	POINT ptVertex[4];
	ptVertex[2].x = ptVertex[3].x = y1;
	ptVertex[0].x = ptVertex[1].x = y2;
	ptVertex[0].y = ptVertex[3].y = x1;
	ptVertex[1].y = ptVertex[2].y = x2;
	MakeInspectionFrameInfo(iCurCamPos, ptVertex);

	SetDlgItemInt(IDC_EDIT_CALC_CAMERA_NO, iCurDDMNo);
	SetDlgItemInt(IDC_EDIT_CALC_SCAN_NO, iCurScanNo);
	SetDlgItemInt(IDC_EDIT_CALC_FRAME_NO, m_iFrameNo);

	SetDlgItemInt(IDC_EDIT_CALC_LEFT, m_iEditCalcLeft);
	SetDlgItemInt(IDC_EDIT_CALC_RIGHT, m_iEditCalcRight);
	SetDlgItemInt(IDC_EDIT_CALC_TOP, m_iEditCalcTop);
	SetDlgItemInt(IDC_EDIT_CALC_BOTTOM, m_iEditCalcBottom);

}

void DDMCalculator::EncoderPositionCalc()
{
	vector<CamParam>::iterator	itr;
	vector<CCamInfo>::iterator	itrInfo;
	int x, y;

	int min = 0;
	int max = 0;

	for (itr = m_vecCamParam.begin(); itr != m_vecCamParam.end(); itr++)
	{
		if (itr->bForward)
		{
			if (min == 0)
			{
				min = itr->lDDMOffsetY;
			}
			else
			{
				if (min > itr->lDDMOffsetY)
				{
					min = itr->lDDMOffsetY;
				}
			}
		}
		else
		{
			if (max == 0)
			{
				max = itr->lDDMOffsetY;
			}
			else
			{
				if (max < itr->lDDMOffsetY)
				{
					max = itr->lDDMOffsetY;
				}
			}
		}
	}

	for (itrInfo = m_vCamInfo.begin(); itrInfo != m_vCamInfo.end(); itrInfo++) {

		itrInfo->GetPos(x, y);

		itrInfo->SetSpeedRipple(m_dSpeedRipple);

		if (itrInfo->IsForwardDirectrion()) {
			itrInfo->SetScanOffset(static_cast<int>(y - min + (MAX_FRAME_HEIGHT + m_iCamStage) * m_fScanResolution));
		}
		else {
			itrInfo->SetScanOffset(static_cast<int>(max - y + MAX_FRAME_HEIGHT * m_fScanResolution));
		}
	}
}

int DDMCalculator::ADDCam(int iCamNo, int iDDMOffset_x, int iDDMOffset_y, int IsForwardDir, double *fFOV, int iScanLength, int iDDMBaseOffsetX, int iDDMBaseOffsetY)
{
	CCamInfo NewCam;

	int iTempScanLength;
	if (iScanLength == 0)
		iTempScanLength = m_iGlassSizeX;
	else iTempScanLength = iScanLength;

	NewCam.SetInfo(iDDMOffset_x, iDDMOffset_y, iTempScanLength, m_fScanResolution, IsForwardDir, fFOV, iDDMBaseOffsetX, iDDMBaseOffsetY);
	m_vCamInfo.push_back(NewCam);
	return ERR_NONE;
}

int DDMCalculator::MakeInspectionFrameInfo(int iCamNo, POINT* _ptVertex)
{
	int iMinX, iMaxX;
	int iMinY, iMaxY;

	int		nVertexCnt = 4;
	POINT	ptVertex[4];
	for (int i = 0; i < 4; i++)
	{
		ptVertex[i].x = _ptVertex[i].x;
		ptVertex[i].y = _ptVertex[i].y;
	}

	RECT	rcPolygon;
	int		iStartFrame;
	int		iEndFrame;
	int		iFrmNo;
	CPolygon		CamROI;
	CString			str;

	m_vCamInfo[iCamNo].GetXRange(iMinX, iMaxX);
	m_vCamInfo[iCamNo].GetYRange(iMinY, iMaxY);

	
	m_vCamInfo[iCamNo].GetGlassPolygonToImagePolygon(nVertexCnt, ptVertex, CamROI);

	rcPolygon = CamROI.GetPolygonRect();

	m_iEditCalcLeft = rcPolygon.left;
	m_iEditCalcRight = rcPolygon.right;
	
	if (rcPolygon.top < 0)
	{
		rcPolygon.top = 0;
	}

	iStartFrame = rcPolygon.top / MAX_FRAME_HEIGHT;
	iEndFrame = rcPolygon.bottom / MAX_FRAME_HEIGHT;

	int y1, y2;
	for (iFrmNo = iStartFrame; iFrmNo <= iEndFrame; ++iFrmNo)
	{
		y1 = 0;
		y2 = MAX_FRAME_HEIGHT;

		if (iFrmNo == iStartFrame)
			y1 = rcPolygon.top % MAX_FRAME_HEIGHT;
		if (iFrmNo == iEndFrame)
			y2 = rcPolygon.bottom % MAX_FRAME_HEIGHT;

		m_iEditCalcTop = y1;
		m_iEditCalcBottom = y2;

		m_iFrameNo = iFrmNo;

		break;
	}

	return 1;
}