#pragma once

#include "akGraph/akGraphLinker.h"

class AFX_EXT_CLASS CakPixel
{
public:
	CakPixel(CDC* pSrc);
	~CakPixel();
	COLORREF GetPixel(int x, int y);
	BYTE* GetPixelSrc(int x, int y);

	LPVOID	GetBitmapBits(){return m_pBits;};
	BITMAPINFO* GetBitmapInfo(){return m_pbmi;};

protected:
	CakPixel();
	LPVOID m_pBits; 
	int m_nBytePerLine;
	BITMAPINFO* m_pbmi;
	int m_nDepth;

	COLORREF (CakPixel::*m_pGetPixel)(int x, int y);

	COLORREF GetPixel8(int x, int y);

	COLORREF GetPixel16(int x, int y);
	COLORREF GetPixel24(int x, int y);
	COLORREF GetPixel32(int x, int y);

	COLORREF GetPixel16ByTable555(int x, int y);
	COLORREF GetPixel16ByTable565(int x, int y);
	COLORREF GetPixel32ByTable(int x, int y);
};