/*
 *   Copyright (C) =USTC= Fu Li
 *
 *   Author   :  Fu Li
 *   Create   :  2005-9-29
 *   Home     :  http://www.crazy-bit.com/
 *   Mail     :  crazybit@263.net
 *   History  :  
 */
#ifdef WIN32
#ifndef __PCL_WIN32__2005_09_29__H__
#define __PCL_WIN32__2005_09_29__H__
#include "ObjLayer.h"
#include "ObjSelect.h"
#include <shellapi.h>

//=============================================================================
/**
 *  WIN32 API wrapper.
 */
class FCWin32
{
public:
    //=============================================================================
    /// Test vKey downing now.
    static BOOL IsKeyDown (int vKey)
    {
        assert (vKey!=VK_CAPITAL && vKey!=VK_NUMLOCK && vKey!=VK_SCROLL) ;
        return (::GetKeyState(vKey) & 0x8000) ;
    }

    /// Open URL use default browser.
    static void DefaultBrowserURL (LPCTSTR strURL)
    {
        // test default browser is IE
        BOOL   bIE = TRUE ;
        HKEY   hKey = NULL ;
        LONG   l = ::RegOpenKeyEx (HKEY_CLASSES_ROOT, TEXT("htmlfile\\shell\\open\\command"), 0, KEY_ALL_ACCESS, &hKey) ;
        if (l == ERROR_SUCCESS)
        {
            TCHAR   szPath[MAX_PATH] = {0} ;
            DWORD   dw = sizeof(TCHAR) * MAX_PATH,
                    dwType = REG_SZ ;
            ::RegQueryValueEx (hKey, NULL, NULL, &dwType, (BYTE*)szPath, &dw) ;
            ::RegCloseKey (hKey) ;

            std::string   s = (char*)bstr_t(szPath) ;
            bIE = (s.find("iexplore.exe") != std::string::npos) ;
        }

        if (bIE)
            ::ShellExecute (NULL, TEXT("open"), TEXT("iexplore.exe"), strURL, NULL, SW_SHOW) ;
        else
            ::ShellExecute (NULL, TEXT("open"), strURL, NULL, NULL, SW_SHOW) ;
    }

    //=============================================================================
    /**
     *  Register a COM server.
     */
    static BOOL RegisterCOMServer (LPCTSTR strComDllPath)
    {
        HMODULE   hModule = ::LoadLibrary(strComDllPath) ;
        BOOL      b = FALSE ;
        if (hModule)
        {
            typedef HRESULT (*pDLLRegisterServer)() ;
            pDLLRegisterServer   pfn = (pDLLRegisterServer)::GetProcAddress (hModule, "DllRegisterServer") ;
            if (pfn && (pfn() == S_OK))
                b = TRUE ;
            FreeLibrary(hModule) ;
        }
        return b ;
    }

    //=============================================================================
    /**
     *  UnRegister a COM server.
     */
    static BOOL UnRegisterCOMServer (LPCTSTR strComDllPath)
    {
        HMODULE   hModule = ::LoadLibrary(strComDllPath) ;
        BOOL      b = FALSE ;
        if (hModule)
        {
            typedef HRESULT (*pDLLUnRegisterServer)() ;
            pDLLUnRegisterServer   pfn = (pDLLUnRegisterServer)::GetProcAddress (hModule, "DllUnregisterServer") ;
            if (pfn && (pfn() == S_OK))
                b = TRUE ;
            FreeLibrary(hModule) ;
        }
        return b ;
    }

    /**
     *  @name Character Set convert.
     */
    //@{
    //=============================================================================
    /**
     *  Convert a UTF-8 string to a ASCII string.
     */
    static std::string UTF8_to_ANSI (const char* szUTF8)
    {
        if (szUTF8 == NULL)
            return "" ;

        int     nLen = ::MultiByteToWideChar (CP_UTF8, 0, szUTF8, -1, NULL, 0) ;
        WCHAR   * pWstr = new WCHAR[nLen+1] ;
        ZeroMemory (pWstr, sizeof(WCHAR) * (nLen+1)) ;
        ::MultiByteToWideChar (CP_UTF8, 0, szUTF8, -1, pWstr, nLen) ;
        std::string     strAnsi (_bstr_t((wchar_t*)pWstr)) ;
        delete[] pWstr ;
        return strAnsi ;
    }

    //=============================================================================
    /**
     *  Convert a UTF-8 string to a ASCII string.
     */
    static std::string ANSI_to_UTF8 (const char* szAnsi)
    {
        if (szAnsi == NULL)
            return "" ;
        
        _bstr_t   bstrTmp (szAnsi) ;
        int       nLen = ::WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)bstrTmp, -1, NULL, 0, NULL, NULL) ;
        char      * pUTF8 = new char[nLen+1] ;
        ZeroMemory (pUTF8, nLen + 1) ;
        ::WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)bstrTmp, -1, pUTF8, nLen, NULL, NULL) ;
        std::string     strUTF (pUTF8) ;
        delete[] pUTF8 ;
        return strUTF ;
    }
    //@}

    /**
     *  @name Autorun at startup of windows.
     */
    //@{
    //=============================================================================
    /**
     *  Get auto run value string.
     */
    static bstr_t GetAutoRunValue (LPCTSTR szValue)
    {
        HKEY     hKey = NULL ;
        TCHAR    sK[] = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run") ;
        LONG     l = ::RegOpenKeyEx (uis_GetRootKey(), sK, 0, KEY_ALL_ACCESS, &hKey) ;
        if (l != ERROR_SUCCESS)
            {assert(FALSE); return "";}

        TCHAR     szPath[MAX_PATH] = {0} ;
        DWORD     dw = sizeof(TCHAR) * MAX_PATH,
                  dwType = REG_SZ ;
        ::RegQueryValueEx (hKey, szValue, NULL, &dwType, (BYTE*)szPath, &dw) ;
        ::RegCloseKey (hKey) ;
        return szPath ;
    }

    //=============================================================================
    /**
     *  Set auto run value string.
     */
    static void SetAutoRunValue (LPCTSTR szValue, LPCTSTR szString)
    {
        HKEY     hKey = NULL ;
        TCHAR    sK[] = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run") ;
        LONG     l = ::RegOpenKeyEx (uis_GetRootKey(), sK, 0, KEY_ALL_ACCESS, &hKey) ;
        if (l != ERROR_SUCCESS)
            {assert(FALSE); return;}

        ::RegDeleteValue (hKey, szValue) ;
        if (lstrlen(szString))
        {
            ::RegSetValueEx (hKey, szValue, NULL, REG_SZ, (BYTE*)szString, (lstrlen(szString)+1)*sizeof(TCHAR)) ;
        }
        ::RegCloseKey (hKey) ;
    }

    //=============================================================================
    /**
     *  Is autorun enable.
     */
    static BOOL IsAutoRunEnable (LPCTSTR szValue)
    {
        return GetAutoRunValue(szValue).length() ? TRUE : FALSE ;
    }

    //=============================================================================
    /**
     *  Set autorun enable.
     */
    static void SetAutoRunEnable (LPCTSTR szValue, BOOL bEnable)
    {
        if (bEnable)
        {
            TCHAR     szPath[MAX_PATH] = {0} ;
            ::GetModuleFileName (NULL, szPath, MAX_PATH) ;
            SetAutoRunValue (szValue, szPath) ;
        }
        else
        {
            SetAutoRunValue (szValue, NULL) ;
        }
    }
    //@}

    /**
     *  @name IE's menu item.
     */
    //@{
    //=============================================================================
    /**
     *  Is IE's menu item enable.
     */
    static BOOL IsIEMenuItemEnable (LPCTSTR szItemShow)
    {
        TCHAR    szGroup[MAX_PATH] = TEXT("Software\\Microsoft\\Internet Explorer\\MenuExt\\") ;
        ::lstrcat (szGroup, szItemShow) ;

        HKEY     hKey = NULL ;
        BOOL     bRet = (::RegOpenKeyEx (HKEY_CURRENT_USER, szGroup, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ;
        ::RegCloseKey (hKey) ;
        return bRet ;
    }

    //=============================================================================
    /**
     *  Add/Remove IE's menu item.
     */
    static void SetIEMenuItem (BOOL bAdd, LPCTSTR szItemShow, LPCTSTR szHtmlPath)
    {
        TCHAR     szKey[MAX_PATH] = TEXT("Software\\Microsoft\\Internet Explorer\\MenuExt\\") ;
        ::lstrcat (szKey, szItemShow) ;
        ::RegDeleteKey (HKEY_CURRENT_USER, szKey) ;
        if (bAdd)
        {
            HKEY   hKey = NULL ;
            LONG   l = ::RegCreateKeyEx (HKEY_CURRENT_USER, szKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL) ;
            if (l != ERROR_SUCCESS)
                {assert(FALSE); return;}

            ::RegSetValue (hKey, NULL, REG_SZ, szHtmlPath, ::lstrlen(szHtmlPath) * sizeof(TCHAR)) ;
            ::RegCloseKey (hKey) ;
        }
    }
    //@}

    /**
     *  @name Window always on top.
     */
    //@{
    //=============================================================================
    /**
     *  Test a window have WS_EX_TOPMOST property.
     */
    static BOOL IsWindowAlwaysTop (HWND hWnd)
    {
        return (GetWindowLong(hWnd,GWL_EXSTYLE) & WS_EX_TOPMOST) ;
    }

    //=============================================================================
    /**
     *  Set/Clear a window's WS_EX_TOPMOST property.
     */
    static void SetWindowAlwaysTop (HWND hWnd, BOOL bTop)
    {
        if (!::IsWindow(hWnd) || (::GetDesktopWindow() == hWnd))
            return ;

        if (bTop)
        {
            if (!IsWindowAlwaysTop(hWnd))
                ::SetWindowPos (hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE) ;
        }
        else
        {
            if (IsWindowAlwaysTop(hWnd))
                ::SetWindowPos (hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE) ;
        }
    }
    //@}

    /**
     *  @name Convert between GDI HBITMAP(DDB) and FCObjImage.
     */
    //@{
	//=============================================================================
	/**
	 *  Create DDB handle from FCObjImage object.
	 */
	static HBITMAP CreateDDBHandle (const FCObjImage& img)
	{
		if (!img.IsValidImage())
			{assert(false); return NULL;}

		// prepare info
		PCL_array<BITMAPINFOHEADER>   bmfh (img.NewImgInfoWithPalette()) ;

		HDC       hdc = ::GetDC (NULL) ;
		HBITMAP   hDDB = CreateDIBitmap (hdc, bmfh.get(), CBM_INIT, (VOID*)img.GetMemStart(), (BITMAPINFO*)bmfh.get(), DIB_RGB_COLORS) ;
		::ReleaseDC (NULL, hdc) ;
		return hDDB ;
    }

	//=============================================================================
	/**
	 *  Create FCObjImage object from DDB handle.
	 */
    static void CreateImageFromDDB (HBITMAP hBitmap, FCObjImage& img)
    {
        BITMAP   bm ;
        ::GetObject (hBitmap, sizeof(BITMAP), &bm) ;

        HDC       hdc = ::CreateCompatibleDC(NULL) ;
        HGDIOBJ   hOld = ::SelectObject (hdc, hBitmap) ;
        RECT      rc = {0, 0, bm.bmWidth, bm.bmHeight} ;
        CaptureDC (img, hdc, rc) ;
        ::SelectObject (hdc, hOld) ;
        ::DeleteDC(hdc) ;
    }
    //@}

	//=============================================================================
	/**
	 *  Load FCObjImage object from resource.
	 */
	static void LoadImageRes (FCObjImage& img, LPCTSTR resName, LPCTSTR resType, IMAGE_TYPE imgType, HMODULE hMod=NULL)
	{
		HRSRC     res = ::FindResource (hMod, resName, resType) ;
		BYTE      * pImgData = (BYTE*)::LockResource (::LoadResource (hMod, res)) ;
		if (pImgData)
			img.Load (pImgData, ::SizeofResource (hMod, res), imgType) ;
	}

    //=============================================================================
    /**
     *  Load FCObjImage object from standard Bitmap resource.
     */
    static void LoadImageBitmapRes (FCObjImage& img, LPCTSTR resName, HMODULE hMod=NULL)
    {
        HRSRC     res = ::FindResource (hMod, resName, RT_BITMAP) ;
        BYTE      * pImgData = (BYTE*)::LockResource (::LoadResource (hMod, res)) ;
        if (pImgData)
            img.LoadDIBStream (pImgData, (int)::SizeofResource(hMod, res)) ;
    }

    /**
     *  @name Draw image.
     */
    //@{
	//=============================================================================
	/**
	 *  Draw entire image to hdc at position (x,y), no scale.
	 */
	static void DrawImage (const FCObjImage& img, HDC hdc, int x, int y)
	{
		if (!img.IsValidImage())
			return ;

		// prepare info
		PCL_array<BITMAPINFO>   bmfh (img.NewImgInfoWithPalette()) ;

		// start draw
		int     nOldMode = ::SetStretchBltMode (hdc, COLORONCOLOR) ;
		::SetDIBitsToDevice (hdc, x, y, img.Width(), img.Height(), 0, 0, 0, img.Height(),
							 img.GetMemStart(), bmfh.get(), DIB_RGB_COLORS) ;
		::SetStretchBltMode (hdc, nOldMode) ;
	}

	//=============================================================================
	/**
	 *  Draw rcImg of image to rcDrawDC of hdc.
	 */
	static void DrawImage (const FCObjImage& img, HDC hdc, RECT rcDrawDC, RECT rcImg)
	{
		if (!img.IsValidImage())
			return ;

		// prepare info
		PCL_array<BITMAPINFO>   bmfh (img.NewImgInfoWithPalette()) ;
		
		// start draw
		int     nOldMode = ::SetStretchBltMode (hdc, COLORONCOLOR),
		nYStart = img.Height() - (rcImg.top + RECTHEIGHT(rcImg)) ;
		::StretchDIBits (hdc, rcDrawDC.left, rcDrawDC.top, RECTWIDTH(rcDrawDC), RECTHEIGHT(rcDrawDC),
						 rcImg.left, nYStart, RECTWIDTH(rcImg), RECTHEIGHT(rcImg),
						 img.GetMemStart(), bmfh.get(), DIB_RGB_COLORS, SRCCOPY) ;
		::SetStretchBltMode (hdc, nOldMode) ;
	}

	//=============================================================================
	/**
	 *  Draw entire image to rcDrawDC of hdc.
	 */
	static void DrawImage (const FCObjImage& img, HDC hdc, RECT rcDrawDC)
	{
		RECT   rcImg = {0, 0, img.Width(), img.Height()} ;
		DrawImage (img, hdc, rcDrawDC, rcImg) ;
	}

	//=============================================================================
	/**
	 *  Draw(aspect) entire image to center of rcDrawDC.
	 */
	static void DrawImageAspect (const FCObjImage& img, HDC hdc, RECT rcDrawDC)
	{
		if (!img.IsValidImage())
			return ;

		int     nNewWidth = img.Width(),
				nNewHeight = img.Height() ;
		if ((img.Width() > RECTWIDTH(rcDrawDC)) || (img.Height() > RECTHEIGHT(rcDrawDC)))
		{
			double   fScaleX = RECTWIDTH(rcDrawDC) / (double)img.Width(),
					 fScaleY = RECTHEIGHT(rcDrawDC) / (double)img.Height(),
					 fScale = FMin (fScaleX, fScaleY) ;
			nNewWidth = FMax (1, (int)(img.Width() * fScale)) ;
			nNewHeight = FMax (1, (int)(img.Height() * fScale)) ;
		}
		RECT     rcShow = {0, 0, nNewWidth, nNewHeight} ;
		::OffsetRect (&rcShow, (RECTWIDTH(rcDrawDC) - nNewWidth) / 2 + rcDrawDC.left,
							   (RECTHEIGHT(rcDrawDC) - nNewHeight) / 2 + rcDrawDC.top) ;
		assert ((rcShow.left >= 0) && (rcShow.top >= 0)) ;
		DrawImage (img, hdc, rcShow) ;
    }

	//=============================================================================
	/**
	 *  AlphaBlend img32 on HDC.
	 */
	static void AlphaImageOnDC (const FCObjImage& img32, HDC hdc, RECT rcOnDC, int nPercent=100)
	{
		if (!img32.IsValidImage() || (img32.ColorBits() != 32))
			return ;

		FCObjImage   imgDC ;
		CaptureDC (imgDC, hdc, rcOnDC) ;
		RECT         rcImg = {0, 0, img32.Width(), img32.Height()},
					 rcDCImg = {0, 0, imgDC.Width(), imgDC.Height()} ;
		imgDC.AlphaBlend (img32, rcDCImg, rcImg, nPercent) ;
		DrawImage (imgDC, hdc, rcOnDC) ;
	}
    //@}

	//=============================================================================
	/**
	 *  Capture rcCap of hdc to FCObjImage.
	 *  <B>the received image color format is 24bpp</B>
	 */
	static BOOL CaptureDC (FCObjImage& img, HDC hdc, RECT rcCap)
	{
		if (IsRectEmpty(&rcCap) || (GetDeviceCaps (hdc, BITSPIXEL) <= 8)) // palette format
			{assert(false); return FALSE;}

		HDC       hMemDC = CreateCompatibleDC (hdc) ;
		HBITMAP   hDDB = CreateCompatibleBitmap (hdc, RECTWIDTH(rcCap), RECTHEIGHT(rcCap)) ;
		if (hDDB == NULL)
		{
			::DeleteDC (hMemDC) ;
			assert(false); return FALSE;
		}

		// start capture
		HGDIOBJ   hOldBmp = ::SelectObject (hMemDC, hDDB) ;
		::BitBlt (hMemDC, 0, 0, RECTWIDTH(rcCap), RECTHEIGHT(rcCap),
				  hdc, rcCap.left, rcCap.top, SRCCOPY) ;
		::SelectObject (hMemDC, hOldBmp) ;

		// attach FCObjImage to DDB
		BITMAP    bm ;
		GetObject (hDDB, sizeof(BITMAP), &bm) ;
		if (img.Create (bm.bmWidth, bm.bmHeight, 24))
		{
			// dest format
			PCL_array<BITMAPINFO>   bmfh (img.NewImgInfoWithPalette()) ;
			::GetDIBits (hdc, hDDB, 0, bm.bmHeight, img.GetMemStart(), bmfh.get(), DIB_RGB_COLORS) ;
		}

		DeleteObject (hDDB) ;
		DeleteDC (hMemDC) ;
		return img.IsValidImage() ;
	}

	//=============================================================================
	/**
	 *  Capture rcCap of screen to FCObjImage.
	 *  <B>the received image color format is 24bpp</B>
	 */
	static BOOL CaptureScreen (FCObjImage& img, RECT rcCap)
	{
		HDC     hSrcDC = ::GetDC (NULL) ;
		BOOL    bRet = CaptureDC (img, hSrcDC, rcCap) ;
		::ReleaseDC (NULL, hSrcDC) ;
		return bRet ;
	}

#ifdef PCL_3RD_LIBRARY_USE_GDIPLUS
    //=============================================================================
	/**
	 *  Create text layer object via GDI+.
	 */
    static void CreateTextLayer_GDIPlus (FCObjTextLayer& layer, PACK_TextLayer tp)
    {
        // calculate un-addshadow position
        const POINT   ptOldPos = {layer.GetGraphObjPos().x + layer.m_sizeToOldPos.cx,
                                  layer.GetGraphObjPos().y + layer.m_sizeToOldPos.cy} ;

        // --------------------------------------------------- create text image -------
        // calc font's Style
        INT       gpFontStyle = Gdiplus::FontStyleRegular ;
        if (tp.m_bBold && tp.m_bItalic)        gpFontStyle = Gdiplus::FontStyleBoldItalic ;
        else if (tp.m_bBold && !tp.m_bItalic)  gpFontStyle = Gdiplus::FontStyleBold ;
        else if (!tp.m_bBold && tp.m_bItalic)  gpFontStyle = Gdiplus::FontStyleItalic ;

        // create GDI+ font
        Gdiplus::Font   gpFont (&Gdiplus::FontFamily((WCHAR*)bstr_t(tp.m_strFace.c_str())),
                                (Gdiplus::REAL)tp.m_nFontSize,
                                gpFontStyle,
                                Gdiplus::UnitPoint) ;
        Gdiplus::TextRenderingHint   gpAA = tp.m_bAntiAliased ? Gdiplus::TextRenderingHintAntiAlias : Gdiplus::TextRenderingHintSingleBitPerPixel ;

        // calculate string's bound box
        const bstr_t     strText16 (tp.m_strText.c_str()) ;
        Gdiplus::RectF   gpBound ;
        HDC              hdcScreen = CreateCompatibleDC (NULL) ;
        {
            Gdiplus::Graphics    gpGra (hdcScreen) ;
            gpGra.SetTextRenderingHint (gpAA) ;
            gpGra.MeasureString ((WCHAR*)strText16, -1, &gpFont, Gdiplus::PointF(0,0), &gpBound) ;
            gpBound.Width++ ; gpBound.Height++ ; // avoid empty rect
        }
        DeleteObject(hdcScreen) ;

        // create alpha-channel, i can't use 8-bit image :-(
        FCObjImage   imgAlpha ((int)ceil(gpBound.Width), (int)ceil(gpBound.Height), 24) ;
        PCL_array<BITMAPINFO>   imgInfo (imgAlpha.NewImgInfoWithPalette()) ;
        {
            // GDI+ draw string
            Gdiplus::Bitmap       gpBmpMask (imgInfo.get(), imgAlpha.GetMemStart()) ;
            Gdiplus::Graphics     gpGra (&gpBmpMask) ;
            gpGra.SetTextRenderingHint (gpAA) ;
            gpGra.DrawString ((WCHAR*)strText16, -1, &gpFont, Gdiplus::PointF(0,0), &Gdiplus::SolidBrush(Gdiplus::Color(255,255,255))) ;
        }

        // combine
        layer.FCObjImage::Create (imgAlpha.Width(), imgAlpha.Height(), 32) ;
        for (int y=0 ; y < layer.Height() ; y++)
            for (int x=0 ; x < layer.Width() ; x++)
            {
                BYTE   * p = layer.GetBits(x,y) ;
                *(RGBQUAD*)p = tp.m_crFont ; // fill color
                PCL_A(p) = *imgAlpha.GetBits(x,y) ; // append alpha
            }

        // add shadow to image
        if (tp.m_bAddShadow)
        {
            FCPixelAddShadow   aCmd(tp.m_ShadowData) ;
            layer.SinglePixelProcessProc (aCmd) ;
        }
        // --------------------------------------------------- create text image -------

        // calculate layer's position
        if (tp.m_bAddShadow)
        {
            RECT     rcLayer = {0, 0, layer.Width(), layer.Height()} ;
            ::OffsetRect (&rcLayer, ptOldPos.x, ptOldPos.y) ;

            RECT     rcShadow = rcLayer ;
            ::OffsetRect (&rcShadow, tp.m_ShadowData.nOffsetX, tp.m_ShadowData.nOffsetY) ;
            ::InflateRect (&rcShadow, tp.m_ShadowData.nSmooth, tp.m_ShadowData.nSmooth) ;
            ::UnionRect (&rcShadow, &rcLayer, &rcShadow) ;
            layer.SetGraphObjPos (rcShadow.left, rcShadow.top) ;
        }
        else
            layer.SetGraphObjPos (ptOldPos) ;

        // can be edit.
        layer.m_TxtLayerProp = tp ;
        layer.ToolEditFlag() = false ;

        // update position offset
        layer.m_sizeToOldPos.cx = ptOldPos.x - layer.GetGraphObjPos().x ;
        layer.m_sizeToOldPos.cy = ptOldPos.y - layer.GetGraphObjPos().y ;
    }
#endif // PCL_3RD_LIBRARY_USE_GDIPLUS

    //=============================================================================
    /**
     *  Draw selection object.
     *  HDC's origin must have been located at top-left of canvas. <BR>
     *  this function calculate edge point by canvas's zoom scale
     *  @param rcCanvas : RECT of visible canvas, it's actual coordinate.
     */
    static void DrawSelection (HDC hdc, FCObjSelect& sel, const PCL_Interface_ZoomScale& canvas, RECT rcCanvas)
    {
        static const BYTE s_ant_data[8][8] =
        {
            {
                0xF0,   /*  ####----  */
                0xE1,   /*  ###----#  */
                0xC3,   /*  ##----##  */
                0x87,   /*  #----###  */
                0x0F,   /*  ----####  */
                0x1E,   /*  ---####-  */
                0x3C,   /*  --####--  */
                0x78,   /*  -####---  */
            },
            {
                0xE1,   /*  ###----#  */
                0xC3,   /*  ##----##  */
                0x87,   /*  #----###  */
                0x0F,   /*  ----####  */
                0x1E,   /*  ---####-  */
                0x3C,   /*  --####--  */
                0x78,   /*  -####---  */
                0xF0,   /*  ####----  */
            },
            {
                0xC3,   /*  ##----##  */
                0x87,   /*  #----###  */
                0x0F,   /*  ----####  */
                0x1E,   /*  ---####-  */
                0x3C,   /*  --####--  */
                0x78,   /*  -####---  */
                0xF0,   /*  ####----  */
                0xE1,   /*  ###----#  */
            },
            {
                0x87,   /*  #----###  */
                0x0F,   /*  ----####  */
                0x1E,   /*  ---####-  */
                0x3C,   /*  --####--  */
                0x78,   /*  -####---  */
                0xF0,   /*  ####----  */
                0xE1,   /*  ###----#  */
                0xC3,   /*  ##----##  */
            },
            {
                0x0F,   /*  ----####  */
                0x1E,   /*  ---####-  */
                0x3C,   /*  --####--  */
                0x78,   /*  -####---  */
                0xF0,   /*  ####----  */
                0xE1,   /*  ###----#  */
                0xC3,   /*  ##----##  */
                0x87,   /*  #----###  */
            },
            {
                0x1E,   /*  ---####-  */
                0x3C,   /*  --####--  */
                0x78,   /*  -####---  */
                0xF0,   /*  ####----  */
                0xE1,   /*  ###----#  */
                0xC3,   /*  ##----##  */
                0x87,   /*  #----###  */
                0x0F,   /*  ----####  */
            },
            {
                0x3C,   /*  --####--  */
                0x78,   /*  -####---  */
                0xF0,   /*  ####----  */
                0xE1,   /*  ###----#  */
                0xC3,   /*  ##----##  */
                0x87,   /*  #----###  */
                0x0F,   /*  ----####  */
                0x1E,   /*  ---####-  */
            },
            {
                0x78,   /*  -####---  */
                0xF0,   /*  ####----  */
                0xE1,   /*  ###----#  */
                0xC3,   /*  ##----##  */
                0x87,   /*  #----###  */
                0x0F,   /*  ----####  */
                0x1E,   /*  ---####-  */
                0x3C,   /*  --####--  */
            },
        };
        static const BYTE s_ant_xmask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01} ;

        // calculate edge points
        sel.RecalculateEdge (canvas.GetZoomScale(), false) ;

        // ==> scaled coordinate
        canvas.Actual_to_Scaled (rcCanvas) ;
        rcCanvas.right++ ; rcCanvas.bottom++ ; // ...division, float

        // selection object's position
        POINT     ptPos = sel.GetGraphObjPos() ;
        canvas.Actual_to_Scaled (ptPos) ;

        sel.m_nCurAnt = ++sel.m_nCurAnt % 8 ;
        for (size_t i=0 ; i < sel.m_ptEdge.size() ; i++)
        {
            POINT   ptDraw = {ptPos.x + sel.m_ptEdge[i].x, ptPos.y + sel.m_ptEdge[i].y} ;
            if (!::PtInRect(&rcCanvas, ptDraw))
                continue ;

            COLORREF   crFill ;
            if (s_ant_data[sel.m_nCurAnt][sel.m_ptEdge[i].y % 8] & s_ant_xmask[sel.m_ptEdge[i].x % 8])
                crFill = RGB(255,255,255) ;
            else
                crFill = RGB(0,0,0) ;
            ::SetPixel (hdc, ptDraw.x, ptDraw.y, crFill) ;
        }
    }

	//=============================================================================
	/**
	 *  Create HRGN object from a 32bpp image.
	 *  alpha=0 means outter of region.
	 */
	static HRGN ConvertAlphaImageToHRGN (const FCObjImage& img32)
	{
		if (!img32.IsValidImage() || (img32.ColorBits() != 32))
			{assert(false); return NULL;}

		HRGN     hRgn = ::CreateRectRgn (0, 0, img32.Width(), img32.Height()) ;
		for (int y=0 ; y < img32.Height() ; y++)
		{
			int      nLastX = 0 ;
			BOOL     bStartCount = FALSE ;
			for (int x=0 ; x < img32.Width() ; x++)
			{
				if (PCL_A(img32.GetBits (x,y)))
				{
					if (bStartCount)
					{
						// erase rect
						HRGN   hSingle = ::CreateRectRgn (nLastX, y, x, y+1) ;
						::CombineRgn (hRgn, hRgn, hSingle, RGN_DIFF) ;
						::DeleteObject (hSingle) ;
						bStartCount = FALSE ;
					}
				}
				else
				{
					if (!bStartCount)
					{
						bStartCount = TRUE ;
						nLastX = x ;
					}
				}
			}
			if (bStartCount)
			{
				// erase rect
				HRGN   hSingle = ::CreateRectRgn (nLastX, y, img32.Width(), y+1) ;
				::CombineRgn (hRgn, hRgn, hSingle, RGN_DIFF) ;
				::DeleteObject (hSingle) ;
			}
		}
		return hRgn ;
	}

    /**
     *  @name Clipboard.
     */
    //@{
	//=============================================================================
	/**
	 *  Whether current clipboard has a bitmap (DIB).
	 */
	static bool IsPasteAvailable()
	{
        return ::IsClipboardFormatAvailable(CF_DIB) ? true : false ;
	}

	//=============================================================================
	/**
	 *  Copy img to clipboard (the image must >= 24bpp).
	 */
	static void CopyToClipboard (const FCObjImage& img)
	{
		if (img.IsValidImage() && (img.ColorBits() >= 24))
			if (::OpenClipboard (NULL))
			{
				// create HGLOBAL
				HGLOBAL  hMem = GlobalAlloc (GMEM_MOVEABLE, img.GetPitch()*img.Height() + sizeof(BITMAPINFOHEADER) + 16) ;
				BYTE     * pClipData = (BYTE*)GlobalLock(hMem) ;

				// fill image's info
				PCL_array<BYTE>     imgInfo (img.NewImgInfoWithPalette()) ;
				CopyMemory (pClipData, imgInfo.get(), sizeof(BITMAPINFOHEADER)) ;
				pClipData += sizeof(BITMAPINFOHEADER) ;

				CopyMemory (pClipData, img.GetMemStart(), img.GetPitch()*img.Height()) ;
				GlobalUnlock (hMem) ;
				if (::EmptyClipboard())
					::SetClipboardData (CF_DIB, hMem) ;
				else
				{
					assert(false);
					GlobalFree (hMem) ;
				}
				::CloseClipboard() ;
				// GlobalFree (hMem) ; // it is a nightmare :-(
			}
	}

	//=============================================================================
	/**
	 *  Get clipboard's image (the returned image is 32bpp).
	 */
	static void GetClipboardImage (FCObjImage& img)
	{
		if (IsPasteAvailable())
			if (::OpenClipboard (NULL))
			{
				// get bitmap
				BITMAPINFOHEADER   * bmif = (BITMAPINFOHEADER*)::GetClipboardData(CF_DIB) ;
				if (img.Create (bmif))
				{
					// palette
					BYTE   * pCurr = ((BYTE*)bmif) + bmif->biSize ;
					if (img.ColorBits() <= 8)
					{
						int   nCount = 1 << img.ColorBits() ;
						img.SetColorTable (0, nCount, (RGBQUAD*)pCurr) ;
						pCurr += (4 * nCount) ;
					}
					else if ((bmif->biCompression == BI_BITFIELDS) && (bmif->biSize == sizeof(BITMAPINFOHEADER)))
						pCurr += 12 ;

					// copy pixel
					CopyMemory (img.GetMemStart(), pCurr, img.GetPitch()*img.Height()) ;

					if (img.ColorBits() == 32)
					{
						if (bmif->biCompression == BI_BITFIELDS)
							img.SetAlphaChannelValue (0xFF) ;
					}
					else
						img.ConvertTo32Bit() ;
				}
				::CloseClipboard() ;
			}
    }
    //@}

	//=============================================================================
	/**
	 *  Get a temp file path.
	 */
	static bstr_t QueryTempFilePath()
	{
        TCHAR   szTmpPath[MAX_PATH],
                szFilename[MAX_PATH] ;
        ::GetTempPath (MAX_PATH, szTmpPath) ;
        ::GetTempFileName (szTmpPath, TEXT("foo"), 0, szFilename) ;
        return szFilename ;
	}

	//=============================================================================
	/**
	 *  Get module path.
	 */
	static bstr_t GetModulePath (HMODULE hModule=NULL)
	{
        TCHAR     szApp[MAX_PATH],
                  szDriver[_MAX_DRIVE],
                  szPath[MAX_PATH] ;
        ::GetModuleFileName (hModule, szApp, MAX_PATH) ;
#if _MSC_VER >= 1400
        ::_tsplitpath_s (szApp, szDriver, _MAX_DRIVE, szPath, MAX_PATH, NULL, 0, NULL, 0) ;
        ::_tmakepath_s (szApp, MAX_PATH, szDriver, szPath, NULL, NULL) ;
#else
        ::_tsplitpath (szApp, szDriver, szPath, NULL, NULL) ;
        ::_tmakepath (szApp, szDriver, szPath, NULL, NULL) ;
#endif
        return szApp ;
	}

private:
    static HKEY uis_GetRootKey()
    {
        //OSVERSIONINFO   osvi = {sizeof(OSVERSIONINFO)} ;
        //::GetVersionEx (&osvi) ;
        //return (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER ;

		OSVERSIONINFOEX osvi = { 0 };
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		osvi.dwMajorVersion = 6;
		osvi.dwMinorVersion = 3;
		DWORDLONG dwlConditionMask = 0;
		VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
		VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
		return (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER ;
	}
};


#endif // __PCL_WIN32__2005_09_29__H__
#endif// WIN32
