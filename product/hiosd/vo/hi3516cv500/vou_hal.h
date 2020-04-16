/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef __VOU_HAL_H__
#define __VOU_HAL_H__

#include "vou_reg.h"
#include "vou_def.h"
#include "vou_coef_org.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_VOID HAL_VOU_Init(HI_VOID);
HI_VOID HAL_VOU_Exit(HI_VOID);

HI_VOID HAL_WriteReg(HI_U32 *pAddress, HI_U32 Value);
HI_U32 HAL_ReadReg(HI_U32 *pAddress);


HI_VOID HAL_VIDEO_HFIR_SetCkGtEn(HAL_DISP_LAYER_E enLayer, HI_U32 ck_gt_en);
HI_BOOL HAL_LAYER_SetSrcResolution(HAL_DISP_LAYER_E enLayer,
                                   RECT_S*   pstRect);


HI_BOOL HAL_INTF_BT_SetDfirEn(HI_U32 dfir_en);

HI_S32 SYS_HAL_SelVoBt1120PinConifg(HI_VOID);

 HI_S32 SYS_HAL_VouBusResetSel(HI_BOOL bReset);
HI_S32 SYS_HAL_VouHdOutPctrl(HI_BOOL pbClkReverse);
HI_S32 SYS_HAL_VouCFGClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouCoreClkEn(HI_S32 s32Dev, HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouPPCClkEn(HI_S32 s32Dev, HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouOutClkEn(HI_S32 s32Dev, HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouAPBClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouBusClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouDevClkEn(HI_S32 s32VoDev, HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouHd0PPCSel(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouHd0DivMode(HI_U32 u32Hd0DivMod);
HI_S32 SYS_HAL_VouHdClkSel(HI_U32 u32ClkSel);

HI_S32 SYS_HAL_VouHd1PPCSel(HI_U32 pbBtClkChSel);
HI_S32 SYS_HAL_VouHd1DivMode(HI_U32 u32Hd1DivMod);
HI_S32 SYS_HAL_VouOutClkSel(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouHdHdmiClkDiv(HI_U32 u32HdmiClkDiv);

HI_S32 SYS_HAL_VouHdDacClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouLcdClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_LcdMclkDiv(HI_U32 u32MclkDiv);
HI_S32 SYS_HAL_LcdDataMode(HI_U32 u32DataMode);
HI_S32 SYS_HAL_VouBtClkEn(HI_BOOL pbBtClkEn);
HI_S32 SYS_HAL_VouHdmiClkEn(HI_BOOL pbHdmiClkEn);
HI_S32 SYS_HAL_VouBtClkSel(HI_U32 pbBtClkChSel);
HI_S32 SYS_HAL_VouMipiClkEn(HI_BOOL pbMipiClkEn);
HI_S32 SYS_HAL_VouMipiChnSel(HI_U32 pbMipiChSel);
HI_S32 SYS_HAL_SetVoLowPowerCtrl(HI_BOOL bLowPowerClkEn);

HI_S32 SYS_HAL_SetVoPllFrac(HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoPllPostdiv1(HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoPllPostdiv2(HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoPllRefdiv(HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoPllFbdiv(HI_U32 u32BitsSet);

HI_VOID HAL_SYS_Control(HI_VOID);
HI_BOOL HAL_DISP_SetIntvsyncTeMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL bSlave);

/*****************************************************************************
 Prototype       : sys Relative
 Description     :

*****************************************************************************/
HI_VOID HAL_SYS_SetArbMode(HI_U32 bMode);
HI_VOID HAL_SYS_VdpResetClk(HI_U32 sel);

/*****************************************************************************
 Prototype       : device Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_DISP_SetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL enIntf);
HI_BOOL HAL_DISP_SetIntfCtrl(HAL_DISP_INTF_E enIntf, HI_U32* pu32CtrlInfo);
HI_BOOL HAL_DISP_SetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,
                             HAL_DISP_SYNCINFO_S* pstSyncInfo, HAL_DISP_SYNCINV_S* pstInv);

HI_BOOL HAL_DISP_SetDevMultiChnEn(HAL_DISP_OUTPUTCHANNEL_E enChn, HAL_MULTICHN_EN enMultiChnEn);
HI_BOOL HAL_DISP_SetIntfSyncInv(HAL_DISP_INTF_E enIntf, HAL_DISP_SYNCINV_S *pstInv);
HI_BOOL  HAL_DISP_SetIntfMuxSel(HAL_DISP_OUTPUTCHANNEL_E enChan,HAL_DISP_INTF_E enIntf);
HI_BOOL HAL_DISP_SetBt1120Sel(HAL_DISP_OUTPUTCHANNEL_E enChan);

HI_BOOL HAL_DISP_SetIntfCSCEn(HAL_DISP_INTF_E enIntf,HI_BOOL bCscEn);
HI_BOOL HAL_DISP_SetIntfClip(HAL_DISP_INTF_E enIntf, HI_BOOL enClip, HAL_DISP_CLIP_S *pstClipData);
HI_BOOL HAL_DISP_SetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 uFieldMode);
HI_BOOL HAL_DISP_SetVtThd(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 vtthd);

HI_BOOL HAL_DISP_SetIntMask(HI_U32 u32MaskEn);
HI_BOOL HAL_DISP_ClrIntMask(HI_U32 u32MaskEn);
HI_BOOL HAL_DISP_SetIntMask1(HI_U32 u32MaskEn);
HI_BOOL  HAL_DISP_ClrIntMask1(HI_U32 u32MaskEn);


HI_VOID HAL_DISP_SetRegUp (HAL_DISP_OUTPUTCHANNEL_E enChan);


/*****************************************************************************
 Prototype       : video layer Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_VIDEO_SetLayerUpMode(HAL_DISP_LAYER_E enLayer, HI_U32 bUpMode);
HI_BOOL HAL_VIDEO_SetHfirMode(HAL_DISP_LAYER_E enLayer, HAL_HFIRMODE_E enMode);
HI_BOOL HAL_VIDEO_SetHfirCoef(HAL_DISP_LAYER_E enLayer, HI_S32* s32Coef);
HI_BOOL HAL_VIDEO_SetLayerDispRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect);
HI_BOOL HAL_VIDEO_SetLayerVideoRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect);
HI_BOOL HAL_VIDEO_SetMultiAreaLAddr  (HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,HI_UL ulLAddr, HI_U16 stride);
HI_BOOL HAL_VIDEO_SetMultiAreaCAddr  (HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,HI_UL ulCAddr, HI_U16 stride);
HI_VOID HAL_VIDEO_CVFIR_SetOutHeight(HAL_DISP_LAYER_E enLayer, HI_U32 out_height);
HI_VOID HAL_VIDEO_CVFIR_SetOutFmt(HAL_DISP_LAYER_E enLayer, HI_U32 out_fmt);
HI_VOID HAL_VIDEO_CVFIR_SetOutPro(HAL_DISP_LAYER_E enLayer, HI_U32 out_pro);
HI_VOID HAL_VIDEO_CVFIR_SetVzmeCkGtEn(HAL_DISP_LAYER_E enLayer, HI_BOOL vzme_ck_gt_en);

HI_VOID HAL_VIDEO_CVFIR_SetCvfirEn(HAL_DISP_LAYER_E enLayer, HI_U32 cvfir_en);
HI_VOID HAL_VIDEO_CVFIR_SetCvmidEn(HAL_DISP_LAYER_E enLayer, HI_U32 cvmid_en);
HI_VOID HAL_VIDEO_CVFIR_SetCvfirMode(HAL_DISP_LAYER_E enLayer, HI_U32 cvfir_mode);
HI_VOID HAL_VIDEO_CVFIR_SetVratio(HAL_DISP_LAYER_E enLayer, HI_U32 vratio);

HI_VOID HAL_VIDEO_CVFIR_SetVChromaOffset(HAL_DISP_LAYER_E enLayer, HI_U32 vchroma_offset);
HI_VOID HAL_VIDEO_CVFIR_SetVbChromaOffset(HAL_DISP_LAYER_E enLayer, HI_U32 vbchroma_offset);


/*****************************************************************************
 Prototype       : layer Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_LAYER_EnableLayer(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_LAYER_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer,
                                            HAL_DISP_PIXEL_FORMAT_E enDataFmt);
HI_BOOL HAL_LAYER_SetLayerInRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect);
HI_BOOL HAL_LAYER_SetLayerGalpha(HAL_DISP_LAYER_E enLayer,
                                     HI_U8 u8Alpha0);
HI_BOOL HAL_LAYER_SetZmeInfo(HAL_DISP_LAYER_E enLayer, HI_U32 u32Width, HI_U32 u32Height,
                             HAL_DISP_ZME_OUTFMT_E enZmeOutFmt);
HI_BOOL HAL_LAYER_SetZmeEnable(HAL_DISP_LAYER_E enLayer,
                                    HAL_DISP_ZMEMODE_E enMode,
                                    HI_U32 bEnable);
HI_BOOL HAL_LAYER_SetZmeFirEnable(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 bEnable);
HI_BOOL  HAL_LAYER_SetRegUp(HAL_DISP_LAYER_E enLayer);




/*****************************************************************************
 Prototype       : cbm layer Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_CBM_SetCbmAttr(HAL_DISP_LAYER_E enLayer, HAL_DISP_OUTPUTCHANNEL_E enChan);
HI_BOOL HAL_CBM_SetCbmBkg(HI_U32 bMixerId, HAL_DISP_BKCOLOR_S *pstBkg);
HI_BOOL HAL_CBM_SetCbmMixerPrio(HAL_DISP_LAYER_E enLayer, HI_U8 u8Prio, HI_U8 u8MixerId);
HI_BOOL HAL_DISP_SetVgaCSCEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn);
HI_VOID HAL_HIHDR_G_SetHihdrR2yScale2p(HI_U32 hihdr_r2y_scale2p);
HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMin(HI_U32 hihdr_r2y_clip_min);
HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMax(HI_U32 hihdr_r2y_clip_max);

HI_BOOL HAL_GRAPHIC_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                              HAL_GFX_BITEXTEND_E enMode);

HI_BOOL HAL_GRAPHIC_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_GRAPHIC_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U32 bAlphaEn, HI_U32 bArange,
                                 HI_U8 u8Alpha0, HI_U8 u8Alpha1);

HI_BOOL HAL_GRAPHIC_SetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 u64LAddr);
HI_BOOL HAL_GRAPHIC_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch);
HI_BOOL HAL_LAYER_SetCscCoef(HAL_DISP_LAYER_E enLayer, CscCoef_S* pstCscCoef);
HI_BOOL HAL_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn);
HI_BOOL HAL_GFX_SetSrcResolution(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect);

__inline static HI_U32 GetLowAddr(HI_U64  u64Phyaddr)
{
	return (HI_U32)u64Phyaddr;
}


__inline static HI_U32 GetHighAddr(HI_U64  u64Phyaddr)
{
	return (HI_U32)(u64Phyaddr>>32);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* End of __VOU_HAL_H__ */

