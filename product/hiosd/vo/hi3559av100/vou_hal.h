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

HI_S32 SYS_HAL_DDRConifg(HI_VOID);
HI_S32 SYS_HAL_SelVoVgaPinConifg(HI_VOID);
HI_S32 SYS_HAL_SelVoHdmiPinConifg(HI_VOID);
HI_S32 SYS_HAL_SetVoHdmiReset(HI_BOOL bReset);
HI_S32 SYS_HAL_VouHdDacClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_SelVoHdDacClk(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouBt1120ClkEn(HI_BOOL bClkEn);
HI_S32 SYS_HAL_VouBt1120ClkSel(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouBt1120ChnSel(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_SelVoHDMIClk(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouSdDacClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_SelVoBt1120PinConifg(HI_VOID);
HI_S32 SYS_HAL_VouBtBpClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouBtClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_SelVoClk(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouSdClkSel(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_SelVoSdClkDiv(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_SelVoSdClkDiv(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouVDPLowPower(HI_BOOL bClkEn);
HI_S32 SYS_HAL_VouBusResetSel(HI_BOOL bReset);
HI_S32 SYS_HAL_VouDevClkEn(HI_S32 s32VoDev, HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouHdOutClkSel(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouHdmiClkEn(HI_S32 s32VoDev, HI_BOOL pbHdmiClkEn);
HI_S32 SYS_HAL_VouMipiTxChnSel(HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouMipiClkEn(HI_S32 s32VoDev, HI_BOOL pbMipiClkEn);
HI_S32 SYS_HAL_VouHd0DivMode(HI_S32 s32Dev, HI_U32 u32Hd0DivMod);
HI_S32 SYS_HAL_SetVoPllFrac(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoPllPostdiv1(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoPllPostdiv2(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoPllRefdiv(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoPllFbdiv(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoSPllFrac(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoSPllPostdiv1(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoSPllPostdiv2(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoSPllRefdiv(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_SetVoSPllFbdiv(HI_S32 s32Pll, HI_U32 u32BitsSet);
HI_S32 SYS_HAL_VouOutClkSel(HI_S32 s32Dev, HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouHdClkSel(HI_S32 s32Dev, HI_U32 u32ClkSel);
HI_S32 SYS_HAL_VouHd1DivMode(HI_S32 s32Dev, HI_U32 u32Hd1DivMod);
HI_S32 SYS_HAL_VouCFGClkEn(HI_S32 s32Dev, HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouCoreClkEn(HI_S32 s32Dev, HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouAPBClkEn(HI_S32 s32Dev, HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouBusClkEn(HI_BOOL pbClkEn);
HI_S32 SYS_HAL_VouMipiTxClkEn(HI_S32 s32VoDev, HI_BOOL pbMipiTxClkEn);
HI_VOID HAL_SYS_Control(HI_VOID);

/*****************************************************************************
 Prototype       : sys Relative
 Description     :

*****************************************************************************/
HI_VOID HAL_SYS_SetOutstanding(HI_VOID);
HI_VOID HAL_SYS_SetAxiMaster(HI_VOID);
HI_VOID HAL_SYS_SetArbMode(HI_U32 bMode);
HI_VOID HAL_SYS_SetRdBusId(HI_U32 bMode);
HI_VOID HAL_SYS_VdpResetClk(HI_U32 sel);

/*****************************************************************************
 Prototype       : device Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_DISP_SetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL enIntf);
HI_BOOL HAL_DISP_SetIntfCtrl(HAL_DISP_INTF_E enIntf, HI_U32* pu32CtrlInfo);
HI_BOOL HAL_DISP_SetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,
                                     HAL_DISP_SYNCINFO_S *pstSyncInfo);
HI_BOOL HAL_DISP_SetIntfDacEnable(HAL_DISP_INTF_E enIntf, HI_BOOL bDacEnable);

HI_BOOL HAL_DISP_SetDevMultiChnEn(HAL_DISP_OUTPUTCHANNEL_E enChn, HAL_MULTICHN_EN enMultiChnEn);
HI_BOOL HAL_DISP_SetIntfSyncInv(HAL_DISP_INTF_E enIntf, HAL_DISP_SYNCINV_S *pstInv);
HI_BOOL  HAL_DISP_SetIntfMuxDefaultSel(HI_VOID);
HI_BOOL  HAL_DISP_SetIntfMuxSel(HAL_DISP_OUTPUTCHANNEL_E enChan,HAL_DISP_INTF_E enIntf);
HI_BOOL HAL_DISP_SetBt1120Sel(HAL_DISP_OUTPUTCHANNEL_E enChan);
HI_BOOL HAL_DISP_VgaDacEn(HI_BOOL bEn);
HI_BOOL HAL_DISP_CvbsDacEn(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL bEn);
HI_BOOL HAL_DISP_SetVgaGc(HI_U32 u32Value);
HI_BOOL HAL_DISP_SetCvbsGc(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Value);
HI_BOOL HAL_DISP_SetIntfCSCEn(HAL_DISP_INTF_E enIntf,HI_BOOL bCscEn);
HI_BOOL HAL_DISP_SetIntfCscCoef(HAL_DISP_INTF_E enIntf,CscCoef_S *pstCoef);
HI_BOOL HAL_DISP_SetIntfClip(HAL_DISP_INTF_E enIntf, HI_BOOL enClip, HAL_DISP_CLIP_S *pstClipData);
HI_BOOL HAL_DISP_SetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 uFieldMode);
HI_BOOL HAL_DISP_SetVtThd(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 vtthd);

HI_BOOL HAL_DISP_SetIntMask(HI_U32 u32MaskEn);
HI_BOOL HAL_DISP_ClrIntMask(HI_U32 u32MaskEn);
HI_BOOL HAL_DISP_SetIntMask1(HI_U32 u32MaskEn);
HI_BOOL  HAL_DISP_ClrIntMask1(HI_U32 u32MaskEn);
HI_BOOL HAL_DISP_ClearIntStatus(HI_U32 u32IntMsk);
HI_BOOL HAL_DISP_SetClkGateEnable(HI_U32 u32Data);
HI_VOID HAL_DISP_DATE_OutCtrl(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32OutCtrl);

HI_BOOL HAL_DISP_SetDateCoeff(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff22(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff24(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff50(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff51(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff52(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff53(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff54(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff55(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff57(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_BOOL HAL_DISP_SetDateCoeff61(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data);
HI_VOID HAL_DISP_SetDateCoeffByIdx(HI_U32 u32Idx, HI_U32 u32Data);
HI_VOID VOU_HAL_DISP_SetDateNotchCoeff(const HI_S16 s16Coef[]);
HI_BOOL HAL_INTF_BT_SetDfirEn(HI_U32 dfir_en);
HI_VOID HAL_DISP_SetRegUp (HAL_DISP_OUTPUTCHANNEL_E enChan);


/*****************************************************************************
 Prototype       : video layer Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_VIDEO_SetLayerUpMode(HAL_DISP_LAYER_E enLayer, HI_U32 bUpMode);
HI_BOOL HAL_VIDEO_SetIfirMode(HAL_DISP_LAYER_E enLayer, HAL_IFIRMODE_E enMode);
HI_BOOL HAL_VIDEO_SetIfirCoef(HAL_DISP_LAYER_E enLayer, HI_S32 * s32Coef);
HI_BOOL HAL_VIDEO_SetHfirMode(HAL_DISP_LAYER_E enLayer, HAL_HFIRMODE_E enMode);
HI_BOOL HAL_VIDEO_SetHfirCoef(HAL_DISP_LAYER_E enLayer, HI_S32* s32Coef);
HI_BOOL HAL_VIDEO_SetLayerDispRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect);
HI_BOOL HAL_VIDEO_SetLayerVideoRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect);
HI_BOOL HAL_VIDEO_SetMultiAreaLAddr  (HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,HI_UL ulLAddr, HI_U16 stride);
HI_BOOL HAL_VIDEO_SetMultiAreaCAddr  (HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,HI_UL ulCAddr, HI_U16 stride);
HI_BOOL HAL_VIDEO_SetMultiAreaEnable(HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,HI_U32 bEnable);
HI_BOOL HAL_VIDEO_SetMultiAreaReso(HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,
                                              HI_U32 u32Width);
HI_BOOL HAL_VIDEO_SetMultiAreaRect(HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,RECT_S *pstVideoAreaRect);

/*****************************************************************************
 Prototype       : layer Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_LAYER_EnableLayer(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_LAYER_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer,
                                            HAL_DISP_PIXEL_FORMAT_E enDataFmt);
HI_BOOL HAL_LAYER_SetCscCoef(HAL_DISP_LAYER_E enLayer, CscCoef_S *pstCscCoef);
HI_BOOL HAL_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn);
HI_BOOL HAL_LAYER_SetLayerInRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect);
HI_BOOL HAL_LAYER_SetLayerOutRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect);
HI_BOOL HAL_LAYER_SetLayerGalpha(HAL_DISP_LAYER_E enLayer,
                                     HI_U8 u8Alpha0);
HI_BOOL HAL_LAYER_SetZmeInfo(HAL_DISP_LAYER_E enLayer, HI_U32 u32Width, HI_U32 u32Height,
                             HAL_DISP_ZME_OUTFMT_E enZmeOutFmt);
HI_BOOL HAL_LAYER_SetZmeEnable(HAL_DISP_LAYER_E enLayer,
                                    HAL_DISP_ZMEMODE_E enMode,
                                    HI_U32 bEnable);
HI_BOOL HAL_LAYER_SetZmeFirEnable(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 bEnable);
HI_BOOL HAL_LAYER_SetZmeMscEnable(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 bEnable);
HI_BOOL HAL_LAYER_SetZmeVerType(HAL_DISP_LAYER_E enLayer, HI_U32 uVerType);
HI_BOOL HAL_LAYER_SetVerRatio(HAL_DISP_LAYER_E enLayer, HI_U32 uRatio);
HI_BOOL  HAL_LAYER_SetRegUp(HAL_DISP_LAYER_E enLayer);

HI_BOOL HAL_LAYER_SetSrcResolution(HAL_DISP_LAYER_E enLayer,
                                   RECT_S*   pstRect);

/*****************************************************************************
 Prototype       : graphic layer Relative
 Description     :

*****************************************************************************/
HI_BOOL HAL_GRAPHIC_SetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 u64LAddr);
HI_BOOL HAL_GRAPHIC_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch);
HI_BOOL HAL_GRAPHIC_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                                HAL_GFX_BITEXTEND_E enMode);
HI_BOOL HAL_GRAPHIC_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable);
HI_BOOL HAL_GRAPHIC_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                   HI_U32 bAlphaEn,HI_U32 bArange,
                                   HI_U8 u8Alpha0,HI_U8 u8Alpha1);
HI_BOOL HAL_GRAPHIC_GetGfxPalpha(HAL_DISP_LAYER_E enLayer, HI_U32 *pbAlphaEn,
                         HI_U8 *pu8Alpha0, HI_U8 *pu8Alpha1);

HI_BOOL HAL_GRAPHIC_SetGfxPalphaRange(HAL_DISP_LAYER_E enLayer, HI_U32 bArange);

HI_BOOL HAL_GRAPHIC_SetGfxMskThd(HAL_DISP_LAYER_E enLayer, HAL_GFX_MASK_S stMsk);
HI_BOOL HAL_GFX_SetSrcResolution(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect);
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


HI_VOID HAL_V0_ZME_SetHzmeCkGtEn(HI_U32 hzme_ck_gt_en);
HI_VOID HAL_V0_ZME_SetOutWidth(HI_U32 out_width);
HI_VOID HAL_V0_ZME_SetLhfirEn(HI_U32 lhfir_en);
HI_VOID HAL_V0_ZME_SetChfirEn(HI_U32 chfir_en);
HI_VOID HAL_V0_ZME_SetLhmidEn(HI_U32 lhmid_en);
HI_VOID HAL_V0_ZME_SetChmidEn(HI_U32 chmid_en);
HI_VOID HAL_V0_ZME_SetNonLnrEn(HI_U32 non_lnr_en);
HI_VOID HAL_V0_ZME_SetLhfirMode(HI_U32 lhfir_mode);
HI_VOID HAL_V0_ZME_SetChfirMode(HI_U32 chfir_mode);
HI_VOID HAL_V0_ZME_SetHfirOrder(HI_U32 hfir_order);
HI_VOID HAL_V0_ZME_SetHratio(HI_U32 hratio);
HI_VOID HAL_V0_ZME_SetLhfirOffset(HI_U32 lhfir_offset);
HI_VOID HAL_V0_ZME_SetChfirOffset(HI_U32 chfir_offset);
HI_VOID HAL_V0_ZME_SetZone0Delta(HI_U32 zone0_delta);
HI_VOID HAL_V0_ZME_SetZone2Delta(HI_U32 zone2_delta);
HI_VOID HAL_V0_ZME_SetZone1End(HI_U32 zone1_end);
HI_VOID HAL_V0_ZME_SetZone0End(HI_U32 zone0_end);
HI_VOID HAL_V0_ZME_SetHlShootctrlEn(HI_U32 hl_shootctrl_en);
HI_VOID HAL_V0_ZME_SetHlShootctrlMode(HI_U32 hl_shootctrl_mode);
HI_VOID HAL_V0_ZME_SetHlFlatdectMode(HI_U32 hl_flatdect_mode);
HI_VOID HAL_V0_ZME_SetHlCoringadjEn(HI_U32 hl_coringadj_en);
HI_VOID HAL_V0_ZME_SetHlGain(HI_U32 hl_gain);
HI_VOID HAL_V0_ZME_SetHlCoring(HI_U32 hl_coring);
HI_VOID HAL_V0_ZME_SetHcShootctrlEn(HI_U32 hc_shootctrl_en);
HI_VOID HAL_V0_ZME_SetHcShootctrlMode(HI_U32 hc_shootctrl_mode);
HI_VOID HAL_V0_ZME_SetHcFlatdectMode(HI_U32 hc_flatdect_mode);
HI_VOID HAL_V0_ZME_SetHcCoringadjEn(HI_U32 hc_coringadj_en);
HI_VOID HAL_V0_ZME_SetHcGain(HI_U32 hc_gain);
HI_VOID HAL_V0_ZME_SetHcCoring(HI_U32 hc_coring);
HI_VOID HAL_V0_ZME_SetApbVhdHfLren(HI_U32 apb_vhd_hf_lren);
HI_VOID HAL_V0_ZME_SetApbVhdHfCren(HI_U32 apb_vhd_hf_cren);
HI_VOID HAL_V0_ZME_SetApbVhdHcoefRaddr(HI_U32 apb_vhd_hcoef_raddr);
HI_VOID HAL_V0_ZME_SetVzmeCkGtEn(HI_U32 vzme_ck_gt_en);
HI_VOID HAL_V0_ZME_SetOutPro(HI_U32 out_pro);
HI_VOID HAL_V0_ZME_SetOutFmt(HI_U32 out_fmt);
HI_VOID HAL_V0_ZME_SetOutHeight(HI_U32 out_height);
HI_VOID HAL_V0_ZME_SetLvfirEn(HI_U32 lvfir_en);
HI_VOID HAL_V0_ZME_SetCvfirEn(HI_U32 cvfir_en);
HI_VOID HAL_V0_ZME_SetLvmidEn(HI_U32 lvmid_en);
HI_VOID HAL_V0_ZME_SetCvmidEn(HI_U32 cvmid_en);
HI_VOID HAL_V0_ZME_SetVfir1tapEn(HI_U32 vfir_1tap_en);
HI_VOID HAL_V0_ZME_SetLvfirMode(HI_U32 lvfir_mode);
HI_VOID HAL_V0_ZME_SetCvfirMode(HI_U32 cvfir_mode);
HI_VOID HAL_V0_ZME_SetGraphdetEn(HI_U32 graphdet_en);
HI_VOID HAL_V0_ZME_SetVratio(HI_U32 vratio);
HI_VOID HAL_V0_ZME_SetVlumaOffset(HI_U32 vluma_offset);
HI_VOID HAL_V0_ZME_SetVchromaOffset(HI_U32 vchroma_offset);
HI_VOID HAL_V0_ZME_SetVblumaOffset(HI_U32 vbluma_offset);
HI_VOID HAL_V0_ZME_SetVbchromaOffset(HI_U32 vbchroma_offset);
HI_VOID HAL_V0_ZME_SetVlShootctrlEn(HI_U32 vl_shootctrl_en);
HI_VOID HAL_V0_ZME_SetVlShootctrlMode(HI_U32 vl_shootctrl_mode);
HI_VOID HAL_V0_ZME_SetVlFlatdectMode(HI_U32 vl_flatdect_mode);
HI_VOID HAL_V0_ZME_SetVlCoringadjEn(HI_U32 vl_coringadj_en);
HI_VOID HAL_V0_ZME_SetVlGain(HI_U32 vl_gain);
HI_VOID HAL_V0_ZME_SetVlCoring(HI_U32 vl_coring);
HI_VOID HAL_V0_ZME_SetVcShootctrlEn(HI_U32 vc_shootctrl_en);
HI_VOID HAL_V0_ZME_SetVcShootctrlMode(HI_U32 vc_shootctrl_mode);
HI_VOID HAL_V0_ZME_SetVcFlatdectMode(HI_U32 vc_flatdect_mode);
HI_VOID HAL_V0_ZME_SetVcCoringadjEn(HI_U32 vc_coringadj_en);
HI_VOID HAL_V0_ZME_SetVcGain(HI_U32 vc_gain);
HI_VOID HAL_V0_ZME_SetVcCoring(HI_U32 vc_coring);
HI_VOID HAL_V0_ZME_SetApbVhdVfLren(HI_U32 apb_vhd_vf_lren);
HI_VOID HAL_V0_ZME_SetApbVhdVfCren(HI_U32 apb_vhd_vf_cren);
HI_VOID HAL_V0_ZME_SetApbVhdVcoefRaddr(HI_U32 apb_vhd_vcoef_raddr);

HI_VOID HAL_V0_HFIR_SetCkGtEn(HI_U32 ck_gt_en);
HI_VOID HAL_V0_HFIR_SetMidEn(HI_U32 mid_en);
HI_VOID HAL_V0_HFIR_SetHfirMode(HI_U32 hfir_mode);
HI_VOID HAL_V0_HFIR_SetHfirEn(HI_U32 hfir_en);
HI_VOID HAL_V0_HFIR_SetCoef1(HI_U32 coef1);
HI_VOID HAL_V0_HFIR_SetCoef0(HI_U32 coef0);
HI_VOID HAL_V0_HFIR_SetCoef3(HI_U32 coef3);
HI_VOID HAL_V0_HFIR_SetCoef2(HI_U32 coef2);
HI_VOID HAL_V0_HFIR_SetCoef5(HI_U32 coef5);
HI_VOID HAL_V0_HFIR_SetCoef4(HI_U32 coef4);
HI_VOID HAL_V0_HFIR_SetCoef7(HI_U32 coef7);
HI_VOID HAL_V0_HFIR_SetCoef6(HI_U32 coef6);
HI_VOID HAL_V1_HFIR_SetCkGtEn(HI_U32 ck_gt_en);
HI_VOID HAL_V1_HFIR_SetMidEn(HI_U32 mid_en);
HI_VOID HAL_V1_HFIR_SetHfirMode(HI_U32 hfir_mode);
HI_VOID HAL_V1_HFIR_SetHfirEn(HI_U32 hfir_en);
HI_VOID HAL_V1_HFIR_SetCoef1(HI_U32 coef1);
HI_VOID HAL_V1_HFIR_SetCoef0(HI_U32 coef0);
HI_VOID HAL_V1_HFIR_SetCoef3(HI_U32 coef3);
HI_VOID HAL_V1_HFIR_SetCoef2(HI_U32 coef2);
HI_VOID HAL_V1_HFIR_SetCoef5(HI_U32 coef5);
HI_VOID HAL_V1_HFIR_SetCoef4(HI_U32 coef4);
HI_VOID HAL_V1_HFIR_SetCoef7(HI_U32 coef7);
HI_VOID HAL_V1_HFIR_SetCoef6(HI_U32 coef6);

HI_VOID HAL_V1_CVFIR_SetVzmeCkGtEn(HI_U32 vzme_ck_gt_en);
HI_VOID HAL_V1_CVFIR_SetOutPro(HI_U32 out_pro);
HI_VOID HAL_V1_CVFIR_SetOutFmt(HI_U32 out_fmt);
HI_VOID HAL_V1_CVFIR_SetOutHeight(HI_U32 out_height);
HI_VOID HAL_V1_CVFIR_SetCvfirEn(HI_U32 cvfir_en);
HI_VOID HAL_V1_CVFIR_SetCvmidEn(HI_U32 cvmid_en);
HI_VOID HAL_V1_CVFIR_SetCvfirMode(HI_U32 cvfir_mode);
HI_VOID HAL_V1_CVFIR_SetVratio(HI_U32 vratio);
HI_VOID HAL_V1_CVFIR_SetVchromaOffset(HI_U32 vchroma_offset);
HI_VOID HAL_V1_CVFIR_SetVbchromaOffset(HI_U32 vbchroma_offset);
HI_VOID HAL_V1_CVFIR_SetVccoef00(HI_U32 vccoef00);
HI_VOID HAL_V1_CVFIR_SetVccoef01(HI_U32 vccoef01);
HI_VOID HAL_V1_CVFIR_SetVccoef02(HI_U32 vccoef02);
HI_VOID HAL_V1_CVFIR_SetVccoef03(HI_U32 vccoef03);
HI_VOID HAL_V1_CVFIR_SetVccoef10(HI_U32 vccoef10);
HI_VOID HAL_V1_CVFIR_SetVccoef11(HI_U32 vccoef11);
HI_VOID HAL_V1_CVFIR_SetVccoef12(HI_U32 vccoef12);
HI_VOID HAL_V1_CVFIR_SetVccoef13(HI_U32 vccoef13);


HI_VOID HAL_G0_ZME_SetCkGtEn(HI_U32 ck_gt_en);
HI_VOID HAL_G0_ZME_SetOutWidth(HI_U32 out_width);
HI_VOID HAL_G0_ZME_SetHfirEn(HI_U32 hfir_en);
HI_VOID HAL_G0_ZME_SetAhfirMidEn(HI_U32 ahfir_mid_en);
HI_VOID HAL_G0_ZME_SetLhfirMidEn(HI_U32 lhfir_mid_en);
HI_VOID HAL_G0_ZME_SetChfirMidEn(HI_U32 chfir_mid_en);
HI_VOID HAL_G0_ZME_SetLhfirMode(HI_U32 lhfir_mode);
HI_VOID HAL_G0_ZME_SetAhfirMode(HI_U32 ahfir_mode);
HI_VOID HAL_G0_ZME_SetHfirOrder(HI_U32 hfir_order);
HI_VOID HAL_G0_ZME_SetHratio(HI_U32 hratio);
HI_VOID HAL_G0_ZME_SetLhfirOffset(HI_U32 lhfir_offset);
HI_VOID HAL_G0_ZME_SetChfirOffset(HI_U32 chfir_offset);
HI_VOID HAL_G0_ZME_SetApbG0HfLren(HI_U32 apb_g0_hf_lren);
HI_VOID HAL_G0_ZME_SetApbG0VfLren(HI_U32 apb_g0_vf_lren);
HI_VOID HAL_G0_ZME_SetApbVhdCoefRaddr(HI_U32 apb_vhd_coef_raddr);
HI_VOID HAL_G0_ZME_SetOutPro(HI_U32 out_pro);
HI_VOID HAL_G0_ZME_SetOutHeight(HI_U32 out_height);
HI_VOID HAL_G0_ZME_SetVfirEn(HI_U32 vfir_en);
HI_VOID HAL_G0_ZME_SetAvfirMidEn(HI_U32 avfir_mid_en);
HI_VOID HAL_G0_ZME_SetLvfirMidEn(HI_U32 lvfir_mid_en);
HI_VOID HAL_G0_ZME_SetCvfirMidEn(HI_U32 cvfir_mid_en);
HI_VOID HAL_G0_ZME_SetLvfirMode(HI_U32 lvfir_mode);
HI_VOID HAL_G0_ZME_SetVafirMode(HI_U32 vafir_mode);
HI_VOID HAL_G0_ZME_SetVratio(HI_U32 vratio);
HI_VOID HAL_G0_ZME_SetVtpOffset(HI_U32 vtp_offset);
HI_VOID HAL_G0_ZME_SetVbtmOffset(HI_U32 vbtm_offset);


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

