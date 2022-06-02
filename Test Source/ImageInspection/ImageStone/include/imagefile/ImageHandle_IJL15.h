/*
 *   Copyright (C) =USTC= Fu Li
 *
 *   Author   :  Fu Li
 *   Create   :  2006-2-21
 *   Home     :  http://www.crazy-bit.com/
 *   Mail     :  crazybit@263.net
 *   History  :  
 */
#ifndef __FOO_IMAGE_HANDLE_IJL15__2006_02_21__H__
#define __FOO_IMAGE_HANDLE_IJL15__2006_02_21__H__
#include "../ObjImage.h"

#include "../../lib/ijl15/ijl.h"

//class FCImageHandle ;
    class FCImageHandle_IJL15 ;

//=============================================================================
/**
 *  Read/Write Jpeg file via intel jpeg library.
 */
class FCImageHandle_IJL15 : public FCImageHandleBase
{
    // auto init and free helper class.
    struct FCAutoIJL_Image : public JPEG_CORE_PROPERTIES
    {
        FCAutoIJL_Image()   { ijlInit(this); }
        ~FCAutoIJL_Image()  { ijlFree(this); }
    };

    /// Read JPEG image via IJL1.5.
    virtual bool LoadImageMemory (const BYTE* pStart, int nMemSize,
                                  PCL_Interface_Composite<FCObjImage>& rImageList,
                                  std::auto_ptr<FCImageProperty>& rImageProp)
    {
        if (!pStart)
            return false ;

        std::auto_ptr<FCAutoIJL_Image>   pIJL (new FCAutoIJL_Image) ;

        // read image info
        pIJL->JPGBytes = (unsigned char*)pStart ;
        pIJL->JPGSizeBytes = nMemSize ;
        if (ijlRead (pIJL.get(), IJL_JBUFF_READPARAMS) != IJL_OK)
            return false ;

        // dest image
        pIJL->DIBChannels = 3 ;
        pIJL->DIBColor = IJL_BGR ;
        switch (pIJL->JPGChannels)
        {
            case 1 : pIJL->JPGColor = IJL_G ; break ;
            case 3 : pIJL->JPGColor = IJL_YCBCR ; break ;
            case 4 : pIJL->JPGColor = IJL_YCBCRA_FPX ;
                     pIJL->DIBChannels = 4 ;
                     pIJL->DIBColor = IJL_RGBA_FPX ;
                     break ;
        }

        // create image object
        FCObjImage   * pImg = new FCObjImage ;
        if (!pImg->Create (pIJL->JPGWidth, pIJL->JPGHeight, pIJL->DIBChannels * 8))
        {
            delete pImg ;
            return false ;
        }

        pIJL->DIBWidth    = pIJL->JPGWidth ;
        pIJL->DIBHeight   = -pIJL->JPGHeight ; // DIB Format
        pIJL->DIBBytes    = pImg->GetMemStart() ;
        pIJL->DIBPadBytes = pImg->GetPitch() - pImg->Width() * pImg->ColorBits()/8 ;

        // read whole image
        if (ijlRead (pIJL.get(), IJL_JBUFF_READWHOLEIMAGE) != IJL_OK)
        {
            delete pImg ;
            return false ;
        }

        // 4-Channel need BGRA_to_RGBA
        if (pIJL->DIBChannels == 4)
        {
            for (int y=0 ; y < pImg->Height() ; y++)
                for (int x=0 ; x < pImg->Width() ; x++)
                {
                    BYTE   * p = pImg->GetBits(x,y) ;
                    FSwap (p[0], p[2]) ;
                }
        }

        // add to list
        rImageList.PCL_PushObject (pImg) ;
        return true ;
    }

    /// Write JPEG image via IJL1.5.
    virtual bool SaveImageFile (const char* szFileName,
                                const std::deque<const FCObjImage*>& rImageList,
                                const FCImageProperty& rImageProp)
    {
        if (rImageList.empty() || !rImageList[0]->IsValidImage())
            return false ;
        const FCObjImage   &img = *rImageList[0] ;

        // must 24bpp image's format
        if (img.ColorBits() != 24)
            {assert(false); return false;}

        std::auto_ptr<FCAutoIJL_Image>   pIJL (new FCAutoIJL_Image) ;
        pIJL->DIBWidth  = img.Width() ;
        pIJL->DIBHeight = -img.Height() ; // DIB Format
        pIJL->DIBChannels = 3 ;
        pIJL->DIBPadBytes = img.GetPitch() - img.Width() * 3 ;
        pIJL->DIBColor  = IJL_BGR ;
        pIJL->JPGFile   = const_cast<PTSTR>(szFileName) ;
        pIJL->JPGWidth  = img.Width() ;
        pIJL->JPGHeight = img.Height() ;
        pIJL->jquality  = JpegSaveQuality(rImageProp) ;
        pIJL->JPGChannels = 3 ;
        pIJL->JPGColor = IJL_YCBCR ;
        pIJL->JPGSubsampling = IJL_411 ;
        pIJL->DIBBytes = img.GetMemStart() ;
        return (ijlWrite (pIJL.get(), IJL_JFILE_WRITEWHOLEIMAGE) == IJL_OK) ;
    }
};

#endif
