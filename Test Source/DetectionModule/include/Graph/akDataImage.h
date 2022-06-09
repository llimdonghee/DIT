#pragma once

#include "akGraph/akGraphStruct.h"
#include <vector>

class AFX_EXT_CLASS CakDataImage
{
public:
	CakDataImage(void);
	virtual ~CakDataImage(void);

	struct _DataImage
	{
		_DataImage()
		{
			m_nDrawPosX = m_nDrawPosY = 0;
			m_nRoate = 0;
			m_nDrawWidth = m_nDrawHeight = 0;

			m_bReverseX = m_bReverseY = false;
		}
		CString m_strBitmapFilename;
		CBitmap m_Bitmap;
		BITMAP	m_Bits;

		unsigned char* m_pBitmapData;

		//그리기 정보 [김태현 2018/12/7]
		int m_nProjectionType;
		int m_nDrawPosX,m_nDrawPosY;
		int m_nDrawWidth,m_nDrawHeight;
		float m_nRoate;
		bool m_bReverseX, m_bReverseY;
	};

public:
	int SetImageData(char* pFileName, int nImageIndex = 0); //add가 아니면 0번 인덱스에 넣는다.
	int AddImageData(char* pFileName); //add가 아니면 0번 인덱스에 넣는다.
	_DataImage* GetImageData(int nImageIndex = 0);
	int GetImageDataNum(){return m_vecImageData.size();};
public:
	virtual void calculateMinMax(double* xmin, double* xmax, double* ymin, double* ymax);
protected:
	
public:
	std::vector<_DataImage*> m_vecImageData;
};
