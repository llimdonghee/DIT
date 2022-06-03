/*
 *   Copyright (C) =USTC= Fu Li
 *
 *   Author   :  Fu Li
 *   Create   :  2006-9-24
 *   Home     :  http://www.crazy-bit.com/
 *   Mail     :  crazybit@263.net
 *   History  :  
 */
#ifndef __FOO_IMAGE_PROPERTY__2006_09_24__H__
#define __FOO_IMAGE_PROPERTY__2006_09_24__H__
#include "StdDefine.h"

enum UIS_PROPERTY_TAG
{
    PROPERTY_TAG_UNKNOW = 0,

    // it's only for compatible
    PROPERTY_TAG_SAVE_FLAG = 500,

	// tga format image info
    // int value : 1 means use RLE / -1 means not use
    PROPERTY_TAG_TGA_USERLE = 1000,

    // time interval between two frame
    PROPERTY_TAG_GIF_FRAMEDELAY = 2000,
    // number of times to display the animation
    PROPERTY_TAG_GIF_LOOPCOUNT = 2001,
    // transparent color index in palette
    PROPERTY_TAG_GIF_TRANSPARENT_INDEX = 2002,

    // jpeg format image info
    PROPERTY_TAG_JPEG_QUALITY = 3000,

    // normal tag
    PROPERTY_TAG_EquipMake = 5000,
    PROPERTY_TAG_EquipModel = 5001,
    PROPERTY_TAG_Orientation = 5002,
    PROPERTY_TAG_XResolution = 5003,
    PROPERTY_TAG_YResolution = 5004,
    PROPERTY_TAG_ResolutionUnit = 5005,
    PROPERTY_TAG_DateTime = 5006,

    // EXIF info
    PROPERTY_TAG_EXIF_ExposureTime = 10000,
    PROPERTY_TAG_EXIF_FNumber = 10001,
    PROPERTY_TAG_EXIF_ExposureProg = 10002,
    PROPERTY_TAG_EXIF_SpectralSense = 10003,
    PROPERTY_TAG_EXIF_ISOSpeed = 10004,
    PROPERTY_TAG_EXIF_OECF = 10005,
    PROPERTY_TAG_EXIF_Ver = 10006,
    PROPERTY_TAG_EXIF_DTOrig = 10007, // Date & time of original
    PROPERTY_TAG_EXIF_DTDigitized = 10008, // Date & time of digital data generation
    PROPERTY_TAG_EXIF_CompConfig = 10009,
    PROPERTY_TAG_EXIF_CompBPP = 10010,
    PROPERTY_TAG_EXIF_ShutterSpeed = 10011,
    PROPERTY_TAG_EXIF_Aperture = 10012,
    PROPERTY_TAG_EXIF_Brightness = 10013,
    PROPERTY_TAG_EXIF_ExposureBias = 10014,
    PROPERTY_TAG_EXIF_MaxAperture = 10015,
    PROPERTY_TAG_EXIF_SubjectDist = 10016,
    PROPERTY_TAG_EXIF_MeteringMode = 10017,
    PROPERTY_TAG_EXIF_LightSource = 10018,
    PROPERTY_TAG_EXIF_Flash = 10019,
    PROPERTY_TAG_EXIF_FocalLength = 10020,
    PROPERTY_TAG_EXIF_MakerNote = 10021,
    PROPERTY_TAG_EXIF_UserComment = 10022,
    PROPERTY_TAG_EXIF_DTSubsec = 10023, // Date & Time subseconds
    PROPERTY_TAG_EXIF_DTOrigSS = 10024, // Date & Time original subseconds
    PROPERTY_TAG_EXIF_DTDigSS = 10025, // Date & TIme digitized subseconds
    PROPERTY_TAG_EXIF_FPXVer = 10026,
    PROPERTY_TAG_EXIF_ColorSpace = 10027,
    PROPERTY_TAG_EXIF_PixXDim = 10028,
    PROPERTY_TAG_EXIF_PixYDim = 10029,
    PROPERTY_TAG_EXIF_RelatedWav = 10030, // related sound file
    PROPERTY_TAG_EXIF_Interop = 10031,
    PROPERTY_TAG_EXIF_FlashEnergy = 10032,
    PROPERTY_TAG_EXIF_SpatialFR = 10033, // Spatial Frequency Response
    PROPERTY_TAG_EXIF_FocalXRes = 10034, // Focal Plane X Resolution
    PROPERTY_TAG_EXIF_FocalYRes = 10035, // Focal Plane Y Resolution
    PROPERTY_TAG_EXIF_FocalResUnit = 10036, // Focal Plane Resolution Unit
    PROPERTY_TAG_EXIF_SubjectLoc = 10037,
    PROPERTY_TAG_EXIF_ExposureIndex = 10038,
    PROPERTY_TAG_EXIF_SensingMethod = 10039,
    PROPERTY_TAG_EXIF_FileSource = 10040,
    PROPERTY_TAG_EXIF_SceneType = 10041,
    PROPERTY_TAG_EXIF_CfaPattern = 10042,
};
//=============================================================================
/**
 *  Property of image.
 */
class FCImageProperty : public PCL_TT_Convertor<UIS_PROPERTY_TAG, std::string>
{
public:
    /**
     *  @name Put/Get multi frame time interval.
     */
    //@{
    /// Get multi frame time interval millisecond.
    void GetFrameDelay (std::deque<int>& listTime) const
    {
        listTime.clear() ;
        std::string   s = QueryPropertyValue(PROPERTY_TAG_GIF_FRAMEDELAY) ;
        for (size_t i=0 ; i < s.length() ; i+=16)
        {
            int   n ;
            FCOXOHelper::A2X (s.substr(i,16), n) ;
            listTime.push_back (n) ;
        }
    }

    /// Put multi frame time interval into property package.
    void PutFrameDelay (int nTime)
    {
        std::string   s = QueryPropertyValue(PROPERTY_TAG_GIF_FRAMEDELAY) ;
        s += FCOXOHelper::X2A (nTime, 16) ;
        SetPropertyValue (PROPERTY_TAG_GIF_FRAMEDELAY, s.c_str()) ;
    }
    //@}

    /// Is exist nTga property.
    bool IsExistProperty (UIS_PROPERTY_TAG nTag) const
    {
        return (GetIndexT1(nTag) == -1) ;
    }

    /// Get property value, return "" if property not exist.
    std::string QueryPropertyValue (UIS_PROPERTY_TAG nTag) const
    {
        return First_to_Second(nTag, "") ;
    }

    /// Set property value.
    void SetPropertyValue (UIS_PROPERTY_TAG nTag, const char* strValue)
    {
        int   n = GetIndexT1(nTag) ;
        if (n == -1)
        {
            AddElement (nTag, strValue) ;
        }
        else
        {
            GetT2(n) = strValue ;
        }
    }
};

#endif
