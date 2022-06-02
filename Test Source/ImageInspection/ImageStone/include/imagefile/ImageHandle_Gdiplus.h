/*
 *   Copyright (C) =USTC= Fu Li
 *
 *   Author   :  Fu Li
 *   Create   :  2004-6-18
 *   Home     :  http://www.crazy-bit.com/
 *   Mail     :  crazybit@263.net
 *   History  :  
*/
#ifndef __FOO_IMAGE_HANDLE_GDIPLUS__2004_06_18__H__
#define __FOO_IMAGE_HANDLE_GDIPLUS__2004_06_18__H__
#include "../ObjImage.h"

//class FCImageHandle ;
    class FCImageHandle_Gdiplus ;

//=============================================================================
/**
 *  Auto load GDI+ module.
 *  load GDI+ module at constructor and unload at destructor.
 */
class FCAutoInitGDIPlus
{
    ULONG_PTR   m_GdiplusToken ;
public:
    /// Constructor (load GDI+ module).
    FCAutoInitGDIPlus()
    {
        Gdiplus::GdiplusStartupInput   gpSI ;
        Gdiplus::GdiplusStartup (&m_GdiplusToken, &gpSI, NULL) ;
    }
    /// Destructor (unload GDI+ module).
    virtual ~FCAutoInitGDIPlus()
    {
        Gdiplus::GdiplusShutdown (m_GdiplusToken) ;
    }

    /// Get bmp/jpeg/gif/tiff/png image's CLSID.
    static bool GetImageEncoderClsid (IMAGE_TYPE imgType, CLSID* pClsid)
    {
        bstr_t     strType ;
        switch (imgType)
        {
            case IMG_BMP : strType = "image/bmp"  ; break;
            case IMG_JPG : strType = "image/jpeg" ; break;
            case IMG_GIF : strType = "image/gif"  ; break;
            case IMG_TIF : strType = "image/tiff" ; break;
            case IMG_PNG : strType = "image/png"  ; break;
            default : return false ;
        }

        UINT     nNum=0, nSize=0 ;
        Gdiplus::GetImageEncodersSize (&nNum, &nSize) ;
        if (nSize <= 0)
            return false ;

        PCL_array<Gdiplus::ImageCodecInfo>   pICI (new BYTE[nSize]) ;
        Gdiplus::GetImageEncoders (nNum, nSize, pICI.get()) ;

        for (UINT i=0 ; i < nNum; i++)
        {
            if (bstr_t(pICI[i].MimeType) == strType)
            {
                *pClsid = pICI[i].Clsid ;
                return true ;
            }
        }
        return false ;
    }

    /// Get table of <GDI+ property tag, type>.
    static void GetGdiplusPropertyTypeTab (PCL_TT_Convertor<PROPID, WORD>& aTab)
    {
        aTab.Clear() ;
        aTab.AddElement (PropertyTagFrameDelay, PropertyTagTypeLong) ;
        aTab.AddElement (PropertyTagLoopCount, PropertyTagTypeShort) ;
//      aTab.AddElement (PropertyTagJPEGQuality, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagEquipMake, PropertyTagTypeASCII) ;
        aTab.AddElement (PropertyTagEquipModel, PropertyTagTypeASCII) ;
        aTab.AddElement (PropertyTagOrientation, PropertyTagTypeShort) ;
        aTab.AddElement (PropertyTagXResolution, PropertyTagTypeRational) ;
        aTab.AddElement (PropertyTagYResolution, PropertyTagTypeRational) ;
        aTab.AddElement (PropertyTagResolutionUnit, PropertyTagTypeShort) ;
        aTab.AddElement (PropertyTagDateTime, PropertyTagTypeASCII) ;
        aTab.AddElement (PropertyTagExifExposureTime, PropertyTagTypeRational) ;
        aTab.AddElement (PropertyTagExifFNumber, PropertyTagTypeRational) ;
        aTab.AddElement (PropertyTagExifExposureProg, PropertyTagTypeShort) ;
//      aTab.AddElement (PropertyTagExifSpectralSense, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagExifISOSpeed, PropertyTagTypeShort) ;
//      aTab.AddElement (PropertyTagExifOECF, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifVer, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagExifDTOrig, PropertyTagTypeASCII) ;
        aTab.AddElement (PropertyTagExifDTDigitized, PropertyTagTypeASCII) ;
//      aTab.AddElement (PropertyTagExifCompConfig, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagExifCompBPP, PropertyTagTypeRational) ;
        aTab.AddElement (PropertyTagExifShutterSpeed, PropertyTagTypeSRational) ;
        aTab.AddElement (PropertyTagExifAperture, PropertyTagTypeRational) ;
//      aTab.AddElement (PropertyTagExifBrightness, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagExifExposureBias, PropertyTagTypeSRational) ;
        aTab.AddElement (PropertyTagExifMaxAperture, PropertyTagTypeRational) ;
//      aTab.AddElement (PropertyTagExifSubjectDist, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagExifMeteringMode, PropertyTagTypeShort) ;
        aTab.AddElement (PropertyTagExifLightSource, PropertyTagTypeShort) ;
        aTab.AddElement (PropertyTagExifFlash, PropertyTagTypeShort) ;
        aTab.AddElement (PropertyTagExifFocalLength, PropertyTagTypeRational) ;
//      aTab.AddElement (PropertyTagExifMakerNote, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagExifUserComment, PropertyTagTypeASCII) ;
//      aTab.AddElement (PropertyTagExifDTSubsec, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifDTOrigSS, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifDTDigSS, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifFPXVer, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagExifColorSpace, PropertyTagTypeShort) ;
        aTab.AddElement (PropertyTagExifPixXDim, PropertyTagTypeShort) ;
        aTab.AddElement (PropertyTagExifPixYDim, PropertyTagTypeShort) ;
//      aTab.AddElement (PropertyTagExifRelatedWav, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifInterop, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifFlashEnergy, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifSpatialFR, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagExifFocalXRes, PropertyTagTypeRational) ;
        aTab.AddElement (PropertyTagExifFocalYRes, PropertyTagTypeRational) ;
        aTab.AddElement (PropertyTagExifFocalResUnit, PropertyTagTypeShort) ;
//      aTab.AddElement (PropertyTagExifSubjectLoc, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifExposureIndex, PropertyTagTypeUndefined) ;
        aTab.AddElement (PropertyTagExifSensingMethod, PropertyTagTypeShort) ;
//      aTab.AddElement (PropertyTagExifFileSource, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifSceneType, PropertyTagTypeUndefined) ;
//      aTab.AddElement (PropertyTagExifCfaPattern, PropertyTagTypeUndefined) ;
    }

    /// Get table of <GDI+ property tag, ImageStone property tag>.
    static void GetPropertyTagTab (PCL_TT_Convertor<PROPID, UIS_PROPERTY_TAG>& aTab)
    {
        aTab.Clear() ;
        aTab.AddElement (PropertyTagFrameDelay, PROPERTY_TAG_GIF_FRAMEDELAY) ;
        aTab.AddElement (PropertyTagLoopCount, PROPERTY_TAG_GIF_LOOPCOUNT) ;
        aTab.AddElement (PropertyTagJPEGQuality, PROPERTY_TAG_JPEG_QUALITY) ;
        aTab.AddElement (PropertyTagEquipMake, PROPERTY_TAG_EquipMake) ;
        aTab.AddElement (PropertyTagEquipModel, PROPERTY_TAG_EquipModel) ;
        aTab.AddElement (PropertyTagOrientation, PROPERTY_TAG_Orientation) ;
        aTab.AddElement (PropertyTagXResolution, PROPERTY_TAG_XResolution) ;
        aTab.AddElement (PropertyTagYResolution, PROPERTY_TAG_YResolution) ;
        aTab.AddElement (PropertyTagResolutionUnit, PROPERTY_TAG_ResolutionUnit) ;
        aTab.AddElement (PropertyTagDateTime, PROPERTY_TAG_DateTime) ;
        aTab.AddElement (PropertyTagExifExposureTime, PROPERTY_TAG_EXIF_ExposureTime) ;
        aTab.AddElement (PropertyTagExifFNumber, PROPERTY_TAG_EXIF_FNumber) ;
        aTab.AddElement (PropertyTagExifExposureProg, PROPERTY_TAG_EXIF_ExposureProg) ;
        aTab.AddElement (PropertyTagExifSpectralSense, PROPERTY_TAG_EXIF_SpectralSense) ;
        aTab.AddElement (PropertyTagExifISOSpeed, PROPERTY_TAG_EXIF_ISOSpeed) ;
        aTab.AddElement (PropertyTagExifOECF, PROPERTY_TAG_EXIF_OECF) ;
        aTab.AddElement (PropertyTagExifVer, PROPERTY_TAG_EXIF_Ver) ;
        aTab.AddElement (PropertyTagExifDTOrig, PROPERTY_TAG_EXIF_DTOrig) ;
        aTab.AddElement (PropertyTagExifDTDigitized, PROPERTY_TAG_EXIF_DTDigitized) ;
        aTab.AddElement (PropertyTagExifCompConfig, PROPERTY_TAG_EXIF_CompConfig) ;
        aTab.AddElement (PropertyTagExifCompBPP, PROPERTY_TAG_EXIF_CompBPP) ;
        aTab.AddElement (PropertyTagExifShutterSpeed, PROPERTY_TAG_EXIF_ShutterSpeed) ;
        aTab.AddElement (PropertyTagExifAperture, PROPERTY_TAG_EXIF_Aperture) ;
        aTab.AddElement (PropertyTagExifBrightness, PROPERTY_TAG_EXIF_Brightness) ;
        aTab.AddElement (PropertyTagExifExposureBias, PROPERTY_TAG_EXIF_ExposureBias) ;
        aTab.AddElement (PropertyTagExifMaxAperture, PROPERTY_TAG_EXIF_MaxAperture) ;
        aTab.AddElement (PropertyTagExifSubjectDist, PROPERTY_TAG_EXIF_SubjectDist) ;
        aTab.AddElement (PropertyTagExifMeteringMode, PROPERTY_TAG_EXIF_MeteringMode) ;
        aTab.AddElement (PropertyTagExifLightSource, PROPERTY_TAG_EXIF_LightSource) ;
        aTab.AddElement (PropertyTagExifFlash, PROPERTY_TAG_EXIF_Flash) ;
        aTab.AddElement (PropertyTagExifFocalLength, PROPERTY_TAG_EXIF_FocalLength) ;
        aTab.AddElement (PropertyTagExifMakerNote, PROPERTY_TAG_EXIF_MakerNote) ;
        aTab.AddElement (PropertyTagExifUserComment, PROPERTY_TAG_EXIF_UserComment) ;
        aTab.AddElement (PropertyTagExifDTSubsec, PROPERTY_TAG_EXIF_DTSubsec) ;
        aTab.AddElement (PropertyTagExifDTOrigSS, PROPERTY_TAG_EXIF_DTOrigSS) ;
        aTab.AddElement (PropertyTagExifDTDigSS, PROPERTY_TAG_EXIF_DTDigSS) ;
        aTab.AddElement (PropertyTagExifFPXVer, PROPERTY_TAG_EXIF_FPXVer) ;
        aTab.AddElement (PropertyTagExifColorSpace, PROPERTY_TAG_EXIF_ColorSpace) ;
        aTab.AddElement (PropertyTagExifPixXDim, PROPERTY_TAG_EXIF_PixXDim) ;
        aTab.AddElement (PropertyTagExifPixYDim, PROPERTY_TAG_EXIF_PixYDim) ;
        aTab.AddElement (PropertyTagExifRelatedWav, PROPERTY_TAG_EXIF_RelatedWav) ;
        aTab.AddElement (PropertyTagExifInterop, PROPERTY_TAG_EXIF_Interop) ;
        aTab.AddElement (PropertyTagExifFlashEnergy, PROPERTY_TAG_EXIF_FlashEnergy) ;
        aTab.AddElement (PropertyTagExifSpatialFR, PROPERTY_TAG_EXIF_SpatialFR) ;
        aTab.AddElement (PropertyTagExifFocalXRes, PROPERTY_TAG_EXIF_FocalXRes) ;
        aTab.AddElement (PropertyTagExifFocalYRes, PROPERTY_TAG_EXIF_FocalYRes) ;
        aTab.AddElement (PropertyTagExifFocalResUnit, PROPERTY_TAG_EXIF_FocalResUnit) ;
        aTab.AddElement (PropertyTagExifSubjectLoc, PROPERTY_TAG_EXIF_SubjectLoc) ;
        aTab.AddElement (PropertyTagExifExposureIndex, PROPERTY_TAG_EXIF_ExposureIndex) ;
        aTab.AddElement (PropertyTagExifSensingMethod, PROPERTY_TAG_EXIF_SensingMethod) ;
        aTab.AddElement (PropertyTagExifFileSource, PROPERTY_TAG_EXIF_FileSource) ;
        aTab.AddElement (PropertyTagExifSceneType, PROPERTY_TAG_EXIF_SceneType) ;
        aTab.AddElement (PropertyTagExifCfaPattern, PROPERTY_TAG_EXIF_CfaPattern) ;
    }
};

//=============================================================================
/**
 *  Read/Write image via Gdi+.
 *  if you load a gif image with transparent color, you will get a 32bpp image object, transparent color be converted to alpha=0 pixel
 */
class FCImageHandle_Gdiplus : public FCImageHandleBase,
                              public FCAutoInitGDIPlus
{
    /// Load image file via GDIPlus.
    virtual bool LoadImageFile (const char* szFileName,
                                PCL_Interface_Composite<FCObjImage>& rImageList,
                                std::auto_ptr<FCImageProperty>& rImageProp)
    {
        // load image file
        Gdiplus::Bitmap   * gp_Bmp = new Gdiplus::Bitmap (bstr_t(szFileName), FALSE) ;
        bool     bRet = StoreMultiFrame (gp_Bmp, rImageList, rImageProp) ;
        delete gp_Bmp ;
        return bRet ;
    }

    /// Load image memory via GDIPlus.
    virtual bool LoadImageMemory (const BYTE* pStart, int nMemSize,
                                  PCL_Interface_Composite<FCObjImage>& rImageList,
                                  std::auto_ptr<FCImageProperty>& rImageProp)
    {
        if (!pStart || (nMemSize <= 0))
            return false ;

        bool      bRet = false ;
        HGLOBAL   hBuffer = ::GlobalAlloc (GMEM_MOVEABLE, nMemSize) ;

        // copy to HGLOBAL then load
        ::CopyMemory (::GlobalLock(hBuffer), pStart, nMemSize) ;
        ::GlobalUnlock (hBuffer) ;

        IStream     * pStream = NULL ;
        if (::CreateStreamOnHGlobal (hBuffer, TRUE, &pStream) == S_OK)
        {
            Gdiplus::Bitmap   * gp_Bmp = new Gdiplus::Bitmap (pStream, FALSE) ;
            bRet = StoreMultiFrame (gp_Bmp, rImageList, rImageProp) ;
            delete gp_Bmp ;
            pStream->Release() ;
        }
        return bRet ;
    }

    static FCObjImage* CreateImageFromBitmap (Gdiplus::Bitmap* gp_Bmp) ;

    // Get all frames in gp_Bmp, add into rImageList.
    static bool StoreMultiFrame (Gdiplus::Bitmap* gp_Bmp,
                                 PCL_Interface_Composite<FCObjImage>& rImageList,
                                 std::auto_ptr<FCImageProperty>& rImageProp)
    {
        if (!gp_Bmp || (gp_Bmp->GetLastStatus() != Gdiplus::Ok))
        {
            assert(false); return false;
        }

        // get frame dimensions
        const UINT        nDim = gp_Bmp->GetFrameDimensionsCount() ;
        PCL_array<GUID>   listID (nDim) ;
        gp_Bmp->GetFrameDimensionsList (listID.get(), nDim) ;

        // get frame and store
        for (UINT i=0 ; i < nDim ; i++)
        {
            UINT   nFrame = gp_Bmp->GetFrameCount (&listID[i]) ;
            for (UINT j=0 ; j < nFrame ; j++)
            {
                gp_Bmp->SelectActiveFrame (&listID[i], j) ;
                FCObjImage   * pImg = CreateImageFromBitmap(gp_Bmp) ;
                if (pImg)
                {
                    rImageList.PCL_PushObject(pImg) ;
                }
            }
        }

        // store image property
        rImageProp = std::auto_ptr<FCImageProperty>(CreatePropertyFromBitmap(gp_Bmp)) ;
        return true ;
    }

    // create image property object base Bitmap
    static FCImageProperty* CreatePropertyFromBitmap (Gdiplus::Bitmap* gp_Bmp)
    {
        // GDI+ tag <==> UIS tag
        PCL_TT_Convertor<PROPID, UIS_PROPERTY_TAG>   aTab ;
        GetPropertyTagTab (aTab) ;

        // get all property
        UINT   nBytes, nNum ;
        gp_Bmp->GetPropertySize (&nBytes, &nNum) ;
        PCL_array<Gdiplus::PropertyItem>   gp_Item (new BYTE[nBytes]) ;
        gp_Bmp->GetAllPropertyItems (nBytes, nNum, gp_Item.get()) ;

        // fill tag
        FCImageProperty   * pProp = new FCImageProperty ;
        for (UINT i=0 ; i < nNum ; i++)
        {
            Gdiplus::PropertyItem   & rItem = gp_Item[i] ;
            UIS_PROPERTY_TAG        nTag = aTab.First_to_Second (rItem.id, PROPERTY_TAG_UNKNOW) ;
            if (nTag == PROPERTY_TAG_UNKNOW)
                continue ;

            // convert all type to string
            if (rItem.type == PropertyTagTypeASCII)
            {
                std::string   s ((char*)rItem.value, rItem.length) ;
                pProp->SetPropertyValue (nTag, s.c_str()) ;
            }
            else if (rItem.type == PropertyTagTypeRational)
            {
                assert (rItem.length == 8) ;
                if (rItem.length == 8)
                {
                    std::string   s = FCOXOHelper::X2A (*(unsigned long*)rItem.value) ;
                    s += "/" ;
                    s += FCOXOHelper::X2A (((unsigned long*)rItem.value)[1]) ;
                    pProp->SetPropertyValue (nTag, s.c_str()) ;
                }
            }
            else if (rItem.type == PropertyTagTypeSRational)
            {
                assert (rItem.length == 8) ;
                if (rItem.length == 8)
                {
                    std::string   s = FCOXOHelper::X2A (*(long*)rItem.value) ;
                    s += "/" ;
                    s += FCOXOHelper::X2A (((long*)rItem.value)[1]) ;
                    pProp->SetPropertyValue (nTag, s.c_str()) ;
                }
            }
            else if (rItem.type == PropertyTagTypeShort)
            {
                assert (rItem.length == 2) ;
                if (rItem.length == 2)
                {
                    std::string   s = FCOXOHelper::X2A (*(unsigned short*)rItem.value) ;
                    pProp->SetPropertyValue (nTag, s.c_str()) ;
                }
            }
            else if (rItem.type == PropertyTagTypeLong)
            {
                if (rItem.id == PropertyTagFrameDelay)
                {
                    assert (rItem.length % 4 == 0) ;
                    for (ULONG i=0 ; i < rItem.length/4 ; i++)
                    {
                        unsigned long   lv = ((unsigned long*)rItem.value)[i] ;
                        pProp->PutFrameDelay ((int)lv * 10) ;
                    }
                }
                else
                {
                    assert (rItem.length == 4) ;
                    if (rItem.length == 4)
                    {
                        std::string   s = FCOXOHelper::X2A (*(unsigned long*)rItem.value) ;
                        pProp->SetPropertyValue (nTag, s.c_str()) ;
                    }
                }
            }
        }
        return pProp ;
    }

    /// Put property into bitmap object.
    static void PutPropertyInBitmap (const FCImageProperty& rImageProp, Gdiplus::Bitmap& gpBmp)
    {
        PCL_TT_Convertor<PROPID, WORD>               tabType ;
        PCL_TT_Convertor<PROPID, UIS_PROPERTY_TAG>   tabProp ;
        GetGdiplusPropertyTypeTab (tabType) ;
        GetPropertyTagTab (tabProp) ;

        // put image's property
        for (int i=0 ; i < rImageProp.GetElementCount() ; i++)
        {
            // becayse PropertyTagExifVer return PropertyTagTypeUndefined
            Gdiplus::PropertyItem     aItem ;
            aItem.id = tabProp.Second_to_First (rImageProp.GetT1(i), PropertyTagExifVer) ;
            aItem.type = tabType.First_to_Second (aItem.id, PropertyTagTypeUndefined) ;
            if (aItem.type == PropertyTagTypeUndefined)
                continue ;

            const std::string   & s = rImageProp.GetT2(i) ;
            if (aItem.type == PropertyTagTypeASCII)
            {
                PCL_array<char>   pBuf (s.length() + 1) ;
                memset (pBuf.get(), 0, s.length() + 1) ;
                memcpy (pBuf.get(), s.c_str(), s.length()) ;
                aItem.length = s.length() + 1 ;
                aItem.value = pBuf.get() ;
                gpBmp.SetPropertyItem (&aItem) ;
            }
            else if (aItem.type == PropertyTagTypeRational)
            {
                size_t   nPos = s.find ("/") ;
                if (nPos != std::string::npos)
                {
                    unsigned long   p[2] ;
                    FCOXOHelper::A2X (s.substr(0,nPos), p[0]) ;
                    FCOXOHelper::A2X (s.substr(nPos+1), p[1]) ;
                    aItem.length = 8 ;
                    aItem.value = p ;
                    gpBmp.SetPropertyItem (&aItem) ;
                }
            }
            else if (aItem.type == PropertyTagTypeSRational)
            {
                size_t   nPos = s.find ("/") ;
                if (nPos != std::string::npos)
                {
                    long   p[2] ;
                    FCOXOHelper::A2X (s.substr(0,nPos), p[0]) ;
                    FCOXOHelper::A2X (s.substr(nPos+1), p[1]) ;
                    aItem.length = 8 ;
                    aItem.value = p ;
                    gpBmp.SetPropertyItem (&aItem) ;
                }
            }
            else if (aItem.type == PropertyTagTypeShort)
            {
                unsigned short   n ;
                FCOXOHelper::A2X (s, n) ;
                aItem.length = 2 ;
                aItem.value = &n ;
                gpBmp.SetPropertyItem (&aItem) ;
            }
            else if (aItem.type == PropertyTagTypeLong)
            {
                unsigned long   n ;
                FCOXOHelper::A2X (s, n) ;
                aItem.length = 4 ;
                aItem.value = &n ;
                gpBmp.SetPropertyItem (&aItem) ;
            }
        }
    }

    /**
     *  Save image to file via GDI+.
     */
    virtual bool SaveImageFile (const char* szFileName,
                                const std::deque<const FCObjImage*>& rImageList,
                                const FCImageProperty& rImageProp)
    {
        if (rImageList.empty() || !rImageList[0]->IsValidImage())
            return false ;
        const FCObjImage   &img = *rImageList[0] ;

        // get encoder's CLSID
        CLSID        clsID ;
        IMAGE_TYPE   imgType = FCObjImage::GetImageHandleFactory()->QueryImageFileType(szFileName) ;
        if (!GetImageEncoderClsid (imgType, &clsID))
            return false ;

        // if image is jpeg, set save quality
        std::auto_ptr<Gdiplus::EncoderParameters>   pEnParas ;
        ULONG      nQuality = JpegSaveQuality(rImageProp) ;
        if (imgType == IMG_JPG)
        {
            pEnParas = std::auto_ptr<Gdiplus::EncoderParameters>(new Gdiplus::EncoderParameters) ;
            pEnParas->Count = 1 ;
            pEnParas->Parameter[0].Guid = Gdiplus::EncoderQuality ;
            pEnParas->Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong ;
            pEnParas->Parameter[0].NumberOfValues = 1 ;
            pEnParas->Parameter[0].Value = &nQuality ;
        }

        // create a GDI+ bitmap and put property
        PCL_array<BITMAPINFO>   bmfh (img.NewImgInfoWithPalette()) ;
        Gdiplus::Bitmap         gpBmp (bmfh.get(), img.GetMemStart()) ;
        PutPropertyInBitmap (rImageProp, gpBmp) ;

        return (gpBmp.Save (bstr_t(szFileName), &clsID, pEnParas.get()) == Gdiplus::Ok) ;
    }
};

//=============================================================================
// inline Implement
//=============================================================================
inline FCObjImage* FCImageHandle_Gdiplus::CreateImageFromBitmap (Gdiplus::Bitmap* gp_Bmp)
{
    // get image's info
    BOOL              bInitAlpha = FALSE ;
    BYTE              __bufImgInfo[sizeof(BITMAPINFOHEADER) + 16] = {0} ;
    BITMAPINFOHEADER  * pInfo = (BITMAPINFOHEADER*)__bufImgInfo ;
    DWORD             * pMask = (DWORD*)(pInfo + 1) ;
    pInfo->biWidth = gp_Bmp->GetWidth() ;
    pInfo->biHeight = gp_Bmp->GetHeight() ;
    pInfo->biCompression = BI_RGB ;
    switch (gp_Bmp->GetPixelFormat())
    {
        case PixelFormat1bppIndexed : pInfo->biBitCount = 1 ; break ;
        case PixelFormat4bppIndexed : pInfo->biBitCount = 4 ; break ;
        case PixelFormat8bppIndexed : pInfo->biBitCount = 8 ; break ;
        case PixelFormat16bppRGB555 :
        case PixelFormat16bppARGB1555 :
            pInfo->biBitCount = 16 ;
            pInfo->biCompression = BI_BITFIELDS ;
            pMask[0] = MASK16_RED_555 ;
            pMask[1] = MASK16_GREEN_555 ;
            pMask[2] = MASK16_BLUE_555 ;
            break ;
        case PixelFormat16bppRGB565 :
            pInfo->biBitCount = 16 ;
            pInfo->biCompression = BI_BITFIELDS ;
            pMask[0] = MASK16_RED_565 ;
            pMask[1] = MASK16_GREEN_565 ;
            pMask[2] = MASK16_BLUE_565 ;
            break ;
        case PixelFormat24bppRGB : pInfo->biBitCount = 24 ; break ;
        case PixelFormat32bppRGB :
            pInfo->biBitCount = 32 ; // no alpha channel, fill 0xFF
            bInitAlpha = TRUE ;
            break ;
        case PixelFormat32bppPARGB :
        case PixelFormat32bppARGB : pInfo->biBitCount = 32 ; break ;
        default :
            return NULL ;
    }

    // create image
    FCObjImage   * pImg = new FCObjImage ;
    if (!pImg->Create(pInfo))
    {
        delete pImg; return NULL ;
    }

    // set palette
    if (pImg->ColorBits() <= 8)
    {
        const int     nByte = gp_Bmp->GetPaletteSize() ;
        PCL_array<Gdiplus::ColorPalette>   pPal (new BYTE[nByte]) ;
        gp_Bmp->GetPalette (pPal.get(), nByte) ;
        pImg->SetColorTable (0, pPal.get()->Count, (RGBQUAD*)&pPal.get()->Entries[0]) ;
    }

    // set pixel
    Gdiplus::Rect         rcLock (0, 0, pImg->Width(), pImg->Height()) ;
    Gdiplus::BitmapData   BmpData ;

// #if _MSC_VER > 1200 // VC7
    gp_Bmp->LockBits (&rcLock, Gdiplus::ImageLockModeRead, gp_Bmp->GetPixelFormat(), &BmpData) ;
// #else
//     gp_Bmp->LockBits (rcLock, Gdiplus::ImageLockModeRead, gp_Bmp->GetPixelFormat(), &BmpData) ;
// #endif

    const BYTE     * pPix = (BYTE*)BmpData.Scan0 ;
    const int      nPitch = FMin (pImg->GetPitch(), BmpData.Stride) ;
    for(int y=0 ; y < pImg->Height() ; y++)
    {
        CopyMemory (pImg->GetBits(y), &pPix[y * BmpData.Stride], nPitch) ;
    }
    gp_Bmp->UnlockBits (&BmpData) ;

    // over
    if (bInitAlpha)
        pImg->SetAlphaChannelValue(0xFF) ;
    return pImg ;
}
//-----------------------------------------------------------------------------

#endif
