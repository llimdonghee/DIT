/*
 *   Copyright (C) =USTC= Fu Li
 *
 *   Author   :  Fu Li
 *   Create   :  2003-3-30
 *   Home     :  http://www.crazy-bit.com/
 *   Mail     :  crazybit@263.net
 *   History  :  
 */
#ifndef __PCL_OBJECT_LAYER__2003_03_30__H__
#define __PCL_OBJECT_LAYER__2003_03_30__H__
#include "ObjImage.h"

class FCMemoLayer ;
    class FCMemoTextLayer ;

class FCObjLayer ; // image layer
    class FCObjTextLayer ; // text image layer

//=============================================================================
/// Layer object's property.
struct PACK_Layer
{
    /// layer's visible flag.
    int m_bLayerVisible ;
    /// layer's limit flag.
    int m_bLayerLimited ;
    /// layer's transparency percentage.
    int m_nAlphaPercent ;
};
/// Layer's memo.
class FCMemoLayer : public PACK_Layer
{
public:
    enum MEMO_TYPE
    {
        MEMO_LAYER_NORMAL,
        MEMO_LAYER_TEXT,
    };
    virtual MEMO_TYPE GetMemoType() const {return MEMO_LAYER_NORMAL;}
};
/**
 *  Layer object.
 *  layer's bpp must be 32bit.
 */
class FCObjLayer : public FCObjImage
{
public:
    /// Constructor
    FCObjLayer()
    {
        m_LayerProp.m_bLayerVisible = true ;
        m_LayerProp.m_bLayerLimited = false ;
        m_LayerProp.m_nAlphaPercent = 100 ;
    }

    FCObjLayer& operator= (const FCObjLayer& layer)
    {
        FCObjImage::operator= (layer) ;
        m_LayerProp = layer.m_LayerProp ;
        return *this ;
    }

    virtual int Serialize (bool bSave, BYTE* pSave)
    {
        const BYTE   * pStartBak = pSave ;
        bSave ? ( *(PACK_Layer*)pSave = m_LayerProp )
              : ( m_LayerProp = *(PACK_Layer*)pSave ) ;
        pSave += sizeof(PACK_Layer);
        pSave += FCObjImage::Serialize (bSave, pSave) ;
        return pSave - pStartBak ;
    }

    /**
     *  @name memo exchange.
     */
    //@{
    /// Create a FCMemoLayer object. you must use <B>delete</B> to delete it. high recommend using <B>std::auto_ptr</B> to wrap it.
    virtual FCMemoLayer* CreateMemoObj() const
    {
        FCMemoLayer   * pMemo = new FCMemoLayer ;
        *(PACK_Layer*)pMemo = m_LayerProp ;
        return pMemo ;
    }
    /// Set layer's property.
    virtual void SetMemoObj (const FCMemoLayer* pMemo)
    {
        if (pMemo)
            m_LayerProp = *(const PACK_Layer*)pMemo ;
    }
    //@}

    /** Is layer visible. */
    bool IsLayerVisible() const {return m_LayerProp.m_bLayerVisible ? true : false;}
    /** Layer's transparent percent. [0..100] */
    int GetLayerTransparent() const {return m_LayerProp.m_nAlphaPercent;}
    /** Is layer limited. */
    bool IsLayerLimited() const {return m_LayerProp.m_bLayerLimited ? true : false;}
    /// Get/Set layer's name.
    std::string& LayerName() {return m_strLayerName;}

protected:
    PACK_Layer   m_LayerProp ;
    std::string  m_strLayerName ;
};

//=============================================================================
/// Text layer object's property.
struct PACK_TextLayer
{
    std::string   m_strText ;
    std::string   m_strFace ;
    int           m_nFontSize ;
    RGBQUAD       m_crFont ;
    bool          m_bAntiAliased ;
    bool          m_bBold ;
    bool          m_bItalic ;
    bool          m_bAddShadow ; // ??????????
    SHADOWDATA    m_ShadowData ; // ????????
    bool          m_bToolEdited ; // ??????tool????????????????????create??????????????????????
};
/// Text layer's memo.
class FCMemoTextLayer : public FCMemoLayer,
                        public PACK_TextLayer
{
    virtual MEMO_TYPE GetMemoType() const {return MEMO_LAYER_TEXT;}
};
//-----------------------------------------------------------------------------
/**
 *  Text layer object.
 *  the layer initialized to limited when constructor.
 */
class FCObjTextLayer : public FCObjLayer
{
public:
    /// Constructor
    FCObjTextLayer()
    {
        m_TxtLayerProp.m_bBold = false ;
        m_TxtLayerProp.m_bItalic = false ;
        m_TxtLayerProp.m_bAntiAliased = true ;
        m_TxtLayerProp.m_nFontSize = 24 ;
        m_TxtLayerProp.m_crFont = FCColor::crWhite() ;
        m_TxtLayerProp.m_bAddShadow = false ;
        m_LayerProp.m_bLayerLimited = true ; // init text-layer is limited
        m_TxtLayerProp.m_bToolEdited = false ;
        m_sizeToOldPos.cx = m_sizeToOldPos.cy = 0 ;
    }

    FCObjTextLayer& operator= (const FCObjTextLayer& layer)
    {
        FCObjLayer::operator= (layer) ;
        m_TxtLayerProp = layer.m_TxtLayerProp ;
        m_sizeToOldPos = layer.m_sizeToOldPos ;
        return *this ;
    }

    virtual FCMemoLayer* CreateMemoObj() const
    {
        FCMemoTextLayer   * pMemo = new FCMemoTextLayer ;

        // set basic property
        std::auto_ptr<FCMemoLayer>   pLM (FCObjLayer::CreateMemoObj()) ;
        *static_cast<FCMemoLayer*>(pMemo) = *pLM ;
        *static_cast<PACK_TextLayer*>(pMemo) = m_TxtLayerProp ;
        return pMemo ;
    }
    virtual void SetMemoObj (const FCMemoLayer* pMemo)
    {
        FCObjLayer::SetMemoObj (pMemo) ;
        if (pMemo->GetMemoType() != FCMemoLayer::MEMO_LAYER_TEXT)
        {
            assert(false) ;
            return ;
        }
        const FCMemoTextLayer   * pTxt = (const FCMemoTextLayer*)pMemo ;
        m_TxtLayerProp = *(const PACK_TextLayer*)pTxt ;
    }

    bool& ToolEditFlag() {return m_TxtLayerProp.m_bToolEdited;}

private:
    PACK_TextLayer   m_TxtLayerProp ;
    SIZE             m_sizeToOldPos ;

friend class FCWin32 ;
};

//=============================================================================
// inline Implement
//=============================================================================

#endif
