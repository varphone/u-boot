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

#ifndef __VOU_DRV_H__
#define __VOU_DRV_H__

#include "hi_type.h"
#include <hi3519av100_vo.h>
#include "vou_def.h"
#include "vou_coef.h"
#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define VOU_INTCLEAR_ALL        0xffffffff
#define RGB(r,g,b)   (((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff))
#define RGB_RED(c)   ((c & 0xff0000) >> 16)
#define RGB_GREEN(c) ((c & 0xff00) >> 8)
#define RGB_BLUE(c)  (c & 0xff)

/******************************************************************************
** YUV(y,u,v)    assemble the y,u,v to 24bit color
** YUV_Y(c)      get Y from 24bit color
** YUV_U(c)      get U from 24bit color
** YUV_V(c)      get V from 24bit color
******************************************************************************/
#define YUV(y,u,v) ((((y) & 0x03ff) << 20) | (((u) & 0x03ff) << 10) | ((v) & 0x03ff))
#define YUV_Y(c)   ( ((c) & 0x3ff00000) >> 20)
#define YUV_U(c)   ( ((c) & 0x000ffc00) >> 10)
#define YUV_V(c)   (  (c) & 0x000003ff)
#define GFX_CSC_SCALE      0xd
#define GFX_CSC_CLIP_MIN   0x0
#define GFX_CSC_CLIP_MAX   0x3ff


typedef HI_S32 VO_DEV;
typedef HI_S32 VO_LAYER;
typedef HI_S32 VO_CHN;
typedef HI_S32 VO_WBC;
typedef HI_S32 GRAPHIC_LAYER;
typedef HI_U32 VO_INTF_TYPE_E;


typedef enum hiVOU_INT_MODE_E
{
    VOU_INT_MODE_FRAME      = 0x0,
    VOU_INT_MODE_FIELD      = 0x1,
    VOU_INT_MODE_BUTT
} VOU_INT_MODE_E;

/* vou mixer prio id */
typedef enum tagVOU_MIX_PRIO_E
{
    VOU_MIX_PRIO0 = 0,
    VOU_MIX_PRIO1,
    VOU_MIX_PRIO2,
    VOU_MIX_PRIO3,
    VOU_MIX_PRIO4,
    VOU_MIX_PRIO5,
    VOU_MIX_PRIO_CLR,
    VOU_MIX_BUTT
}VOU_MIX_PRIO_E;

typedef enum hiVOU_LAYER_PIXERL_FORMAT_E
{
    /* for video surface interface */
    VOU_LAYER_PIXERL_FORMAT_SPYCbCr_400       = 0x1,
    VOU_LAYER_PIXERL_FORMAT_SPYCbCr_420       = 0x2,
    VOU_LAYER_PIXERL_FORMAT_SPYCbCr_422       = 0x3,
    VOU_LAYER_PIXERL_FORMAT_BUTT
} VOU_LAYER_PIXERL_FORMAT_E;

typedef enum hiVOU_INTF_PIXERL_FORMAT_E
{
    /* for device interface */
    VOU_INTF_PIXERL_FORMAT_YCBCR422          = 0x0,
    VOU_INTF_PIXERL_FORMAT_YCBCR444          = 0xC,
    VOU_INTF_PIXERL_FORMAT_RGB888            = 0xC,
    VOU_INTF_PIXERL_FORMAT_BUTT
} VOU_INTF_PIXERL_FORMAT_E;

typedef enum hiVO_CSC_RANGE_E
{
    VO_CSC_DEF_VAL          = 50,
    VO_CSC_LUMA_MAX         = 100,
    VO_CSC_LUMA_MIN         = 0,

    VO_CSC_CONT_MAX         = 100,
    VO_CSC_CONT_MIN         = 0,

    VO_CSC_HUE_MAX         = 100,
    VO_CSC_HUE_MIN         = 0,

    VO_CSC_SAT_MAX         = 100,
    VO_CSC_SAT_MIN         = 0,

}VO_CSC_RANGE_E;


typedef struct hiVOU_ACM_BLKINFO_S
{
    HI_U32 bAcmBlkEn;

    HI_U8  u8UIndex;
    HI_U8  u8VIndex;

    HI_S8  s8UOffsetA;
    HI_S8  s8UOffsetB;
    HI_S8  s8UOffsetC;
    HI_S8  s8UOffsetD;

    HI_S8  s8VOffsetA;
    HI_S8  s8VOffsetB;
    HI_S8  s8VOffsetC;
    HI_S8  s8VOffsetD;
} VOU_ACM_BLKINFO_S;

/* vou zoom mode */
typedef enum
{
    VOU_DISP_ZMEMODE_HORL = 0,
    VOU_DISP_ZMEMODE_HORC,
    VOU_DISP_ZMEMODE_VERL,
    VOU_DISP_ZMEMODE_VERC,

    VOU_DISP_ZMEMODE_HOR,
    VOU_DISP_ZMEMODE_VER,
    VOU_DISP_ZMEMODE_ALPHA,
    VOU_DISP_ZMEMODE_ALPHAV,
    VOU_DISP_ZMEMODE_VERT,
    VOU_DISP_ZMEMODE_VERB,

    VOU_DISP_ZMEMODE_ALL,
    VOU_DISP_ZMEMODE_NONL,
    VOU_DISP_ZMEMODE_BUTT

}VOU_DISP_ZMEMODE_E;

typedef struct hiVOU_ACM_S
{
    VOU_ACM_BLKINFO_S stAcmBlk[4];
} VOU_ACM_S;

typedef enum hiVOU_DEV_TYPE_E
{
    VOU_HD_HW_DEV = 0,
    VOU_SD_HW_DEV,
    VOU_CAS_DEV,
    VOU_VIRT_DEV,
    VOU_DEV_TYPE_BUTT,
} VOU_DEV_TYPE_E;

typedef struct hiVOU_DEV_CAPABILITY_S
{
    VOU_DEV_TYPE_E  enVoDevType;
    HI_BOOL         bHdVpssRciv;
    HI_BOOL         bSdTdeDisp;
    HI_BOOL         bSupportWbc;
    HI_BOOL         bSptGetScreenFrm;
} VOU_DEV_CAPABILITY_S;

typedef enum
{
    VOU_SW_LAYER_VHD0  = 0,
    VOU_SW_LAYER_VHD1  = 1,
    VOU_SW_LAYER_VP    = 2,
    //VOU_SW_LAYER_VHD2  = 2,
    VOU_SW_LAYER_VSD0  = 3,
    //VOU_SW_LAYER_VSD1  = 2,
    //VOU_SW_LAYER_VSD2  = 5,
    //VOU_SW_LAYER_VSD3  = 6,
    //VOU_SW_LAYER_VSD4  = 7,
    //VOU_SW_LAYER_CAS1  = 8,
    //VOU_SW_LAYER_CAS2  = 9,
    VOU_SW_LAYER_VIRT0 = 4,
    VOU_SW_LAYER_VIRT1 = 5,
    VOU_SW_LAYER_VIRT2 = 6,
    VOU_SW_LAYER_VIRT3 = 7,
    VOU_SW_LAYER_BUTT
}VOU_SW_LAYER_E;

typedef struct tagVO_SYNC_INFO_S
{
    HI_BOOL  bSynm;     /* sync mode(0:timing,as BT.656; 1:signal,as LCD) */
    HI_BOOL  bIop;      /* interlaced or progressive display(0:i; 1:p) */
    HI_U8    u8Intfb;   /* interlace bit width while output */

    HI_U16   u16Vact ;  /* vertical active area */
    HI_U16   u16Vbb;    /* vertical back blank porch */
    HI_U16   u16Vfb;    /* vertical front blank porch */

    HI_U16   u16Hact;   /* herizontal active area */
    HI_U16   u16Hbb;    /* herizontal back blank porch */
    HI_U16   u16Hfb;    /* herizontal front blank porch */
    HI_U16   u16Hmid;   /* bottom herizontal active area */

    HI_U16   u16Bvact;  /* bottom vertical active area */
    HI_U16   u16Bvbb;   /* bottom vertical back blank porch */
    HI_U16   u16Bvfb;   /* bottom vertical front blank porch */

    HI_U16   u16Hpw;    /* horizontal pulse width */
    HI_U16   u16Vpw;    /* vertical pulse width */

    HI_BOOL  bIdv;      /* inverse data valid of output */
    HI_BOOL  bIhs;      /* inverse horizontal synch signal */
    HI_BOOL  bIvs;      /* inverse vertical synch signal */

} VO_SYNC_INFO_S;

typedef enum hiVOU_LAYER_DDR_E
{
    VOU_LAYER_DDR0 = 0,
    VOU_LAYER_DDR1 = 1,
    VOU_LAYER_DDR_BUTT
}VOU_LAYER_DDR_E;

typedef struct hiVO_CSC_S
{
    VO_CSC_MATRIX_E enCscMatrix;
    HI_U32 u32Luma;                     /* luminance:   0 ~ 100 default: 50 */
    HI_U32 u32Contrast;                 /* contrast :   0 ~ 100 default: 50 */
    HI_U32 u32Hue;                      /* hue      :   0 ~ 100 default: 50 */
    HI_U32 u32Satuature;                /* satuature:   0 ~ 100 default: 50 */
} VO_CSC_S;

typedef struct hiVO_VGA_PARAM_S
{
    VO_CSC_S stCSC;
    HI_U32 u32Gain;                     /* current gain of VGA signals. [0, 64). default:0x30 */
} VO_VGA_PARAM_S;


typedef struct hiSIZE_S
{
    HI_U32 u32Width;
    HI_U32 u32Height;
} SIZE_S;

typedef struct hiVO_PUB_ATTR_S
{
    HI_U32                   u32BgColor;          /* Background color of a device, in RGB format. */
    VO_INTF_TYPE_E           enIntfType;          /* Type of a VO interface */
    VO_INTF_SYNC_E           enIntfSync;          /* Type of a VO interface timing */
    VO_SYNC_INFO_S           stSyncInfo;          /* Information about VO interface timings */
} VO_PUB_ATTR_S;

typedef enum
{
    VHDR_HDR10_IN_SDR_OUT = 0,
    VHDR_HDR10_IN_HLG_OUT     ,
    VHDR_HLG_IN_SDR_OUT      ,
    VHDR_HLG_IN_HDR10_OUT     ,
    VHDR_SDR10_IN_SDR8_OUT  ,
    VHDR_XVYCC                 ,
    VHDR_SDR2020CL_IN_709_OUT,
    VHDR_BUTT
}VHDR_SCENE_MODE_E;


typedef struct
{
    //Module ctrl
    HI_U32 vhdr_en                   ;
    HI_U32 vhdr_ck_gt_en           ;
    HI_U32 vhdr_degmm_en           ;
    HI_U32 vhdr_gamut_en           ;
    HI_U32 vhdr_tmap_en            ;
    HI_U32 vhdr_gmm_en               ;
    HI_U32 vhdr_dither_en           ;
    HI_U32 vhdr_r2y_en               ;
    HI_U32 vhdr_cadj_en            ;
    HI_U32 vhdr_gamut_bind           ;

    //Dither ctrl
    HI_U32 vhdr_dither_round_unlim ;
    HI_U32 vhdr_dither_round       ;
    HI_U32 vhdr_dither_domain_mode ;
    HI_U32 vhdr_dither_tap_mode    ;

    //HiHDR Scene
    VHDR_SCENE_MODE_E vhdr_scene_mode;

}VO_HIHDR_V_CFG_S;

typedef struct
{
    //Module ctrl
    HI_U32 hihdr_y2r_en       ;
    HI_U32 hihdr_y2r_ck_gt_en ;

    //PIP ctrl
    HI_U32 hihdr_v0_y2r_en      ;
    HI_U32 hihdr_v1_y2r_en      ;
    HI_U32 hihdr_v0_vhdr_en   ;
    HI_U32 hihdr_v1_vhdr_en   ;
    HI_U32 hihdr_v0_cl_en      ;
    HI_U32 hihdr_v1_cl_en      ;

    //Demo ctrl
    HI_U32 hihdr_v_demo_en      ;
    HI_U32 hihdr_v_demo_mode  ;
    HI_U32 hihdr_v_demo_pos   ;

    //HiHDR Scene
    VHDR_SCENE_MODE_E vhdr_scene_mode;

}VO_CSC_V0_CFG_S;

typedef struct
{
    HI_U32 ck_gt_en         ;
    HI_U32 hfir_order       ;
    HI_U32 lhfir_en         ;
    HI_U32 chfir_en         ;
    HI_U32 lhmid_en         ;
    HI_U32 chmid_en         ;
    HI_U32 non_lnr_en       ;
    HI_U32 lhfir_mode       ;
    HI_U32 chfir_mode       ;
    HI_U32 hl_shootctrl_en  ;
    HI_U32 hl_shootctrl_mode;
    HI_U32 hc_shootctrl_en  ;
    HI_U32 hc_shootctrl_mode;

    HI_U32 in_pro           ;
    HI_U32 out_pro          ;
    HI_U32 out_fmt          ;
    HI_U64 in_height        ;
    HI_U64 out_height       ;
    HI_U64 in_width         ;
    HI_U64 out_width        ;
    HI_U32 graphdet_en      ;

    HI_U32 lvfir_en         ;
    HI_U32 cvfir_en         ;
    HI_U32 lvmid_en         ;
    HI_U32 cvmid_en         ;
    HI_U32 vfir_1tap_en     ;
    HI_U32 lvfir_mode       ;
    HI_U32 cvfir_mode       ;
    HI_U32 vl_shootctrl_en  ;
    HI_U32 vl_shootctrl_mode;
    HI_U32 vc_shootctrl_en  ;
    HI_U32 vc_shootctrl_mode;

}VO_V0_ZME_CFG_S;

typedef struct
{
    HI_U32 vluma_offset       ;
    HI_U32 vchroma_offset     ;
    HI_U32 vbluma_offset      ;
    HI_U32 vbchroma_offset    ;
    HI_U32 lhfir_offset       ;
    HI_U32 chfir_offset       ;
    HI_U32 vl_flatdect_mode   ;
    HI_U32 vl_coringadj_en    ;
    HI_U32 vl_gain            ;
    HI_U32 vl_coring          ;
    HI_U32 vc_flatdect_mode   ;
    HI_U32 vc_coringadj_en    ;
    HI_U32 vc_gain            ;
    HI_U32 vc_coring          ;
    HI_U32 hl_flatdect_mode   ;
    HI_U32 hl_coringadj_en    ;
    HI_U32 hl_gain            ;
    HI_U32 hl_coring          ;
    HI_U32 hc_flatdect_mode   ;
    HI_U32 hc_coringadj_en    ;
    HI_U32 hc_gain            ;
    HI_U32 hc_coring          ;
}VO_ZME_COMM_PQ_CFG_S;


typedef struct
{
    HI_U32 zme_vprec ;
    HI_U32 zme_hprec ;
}VO_ZME_DS_INFO_S;


typedef enum
{
    VO_ZME_TYP   = 0 ,
    VO_ZME_TYP1      ,
    VO_ZME_RAND      ,
    VO_ZME_MAX       ,
    VO_ZME_MIN       ,
    VO_ZME_ZERO      ,
    VO_ZME_BUTT
}VO_ZME_MODE_E;


typedef enum
{
    VDP_V0_HFIR_TYP =0,
    VDP_V0_HFIR_TYP1 ,
    VDP_V0_HFIR_RAND ,
    VDP_V0_HFIR_MAX ,
    VDP_V0_HFIR_MIN ,
    VDP_V0_HFIR_ZERO ,
    VDP_V0_HFIR_BUTT
}VO_V0_HFIR_MODE_E;
typedef enum
{
    XDP_V0_HFIR_BYPASS =0,
    XDP_V0_HFIR_COPY ,
    XDP_V0_HFIR_BILT ,
    XDP_V0_HFIR_FILT ,
    XDP_V0_HFIR_BUTT
}XDP_V0_HFIR_MODE_E;

typedef struct
{
    //HI_U32 v0_hfir_en ;
    HI_U32 ck_gt_en         ;//use for cfg ck_gt_en
    HI_U32 mid_en         ;
    XDP_V0_HFIR_MODE_E hfir_mode       ;//use for cfg mode
}VO_V0_HFIR_CFG_S;

typedef struct
{
    HI_U32            vid_iw          ;
    HI_U32            vid_ih          ;
    HI_U32            vid_ow          ;
    HI_U32            vid_oh          ;

    HI_U32            u32ZmeEn        ;
    HI_U32            u32HfirEn       ;
    HI_U32            u32CscEn        ;
    HI_U32            u32HdrEn        ;

    VO_V0_ZME_CFG_S                stZmeCfg    ;
    VO_V0_HFIR_CFG_S           stHfirCfg   ;
    VO_CSC_V0_CFG_S            stYuv2RgbCfg;
    VO_HIHDR_V_CFG_S           stVHihdrCfg ;

} VDP_VID_IP_CFG_S;

//+++++++++++++++++++++
// For V1
//+++++++++++++++++++++
typedef struct
{
    HI_U32 ck_gt_en         ;
    HI_U32 out_pro          ;
    HI_U32 out_fmt          ;
    HI_U64 in_height        ;
    HI_U64 out_height       ;
    HI_U64 in_width         ;
    HI_U64 out_width        ;
    HI_U32 cvfir_en         ;
    HI_U32 cvmid_en         ;
    HI_U32 cvfir_mode       ;

    HI_U32 hfir_order       ;
    HI_U32 lhfir_en         ;
    HI_U32 chfir_en         ;
    HI_U32 lhmid_en         ;
    HI_U32 chmid_en         ;
    HI_U32 non_lnr_en       ;
    HI_U32 lhfir_mode       ;
    HI_U32 chfir_mode       ;
    HI_U32 hl_shootctrl_en  ;
    HI_U32 hl_shootctrl_mode;
    HI_U32 hc_shootctrl_en  ;
    HI_U32 hc_shootctrl_mode;

    HI_U32 in_pro           ;
    HI_U32 graphdet_en      ;

    HI_U32 lvfir_en         ;
    HI_U32 lvmid_en         ;
    HI_U32 vfir_1tap_en     ;
    HI_U32 lvfir_mode       ;
    HI_U32 vl_shootctrl_en  ;
    HI_U32 vl_shootctrl_mode;
    HI_U32 vc_shootctrl_en  ;
    HI_U32 vc_shootctrl_mode;

}VDP_V1_CVFIR_CFG_S;
typedef enum
{
    VDP_V1_HFIR_TYP =0,
    VDP_V1_HFIR_TYP1 ,
    VDP_V1_HFIR_RAND ,
    VDP_V1_HFIR_MAX ,
    VDP_V1_HFIR_MIN ,
    VDP_V1_HFIR_ZERO ,
    VDP_V1_HFIR_BUTT
}VDP_V1_HFIR_MODE_E;

typedef enum
{
    XDP_HFIR_BYPASS =0,
    XDP_HFIR_COPY ,
    XDP_HFIR_BILT ,
    XDP_HFIR_FILT ,
    XDP_HFIR_BUTT
}XDP_HFIR_MODE_E;

typedef struct
{
    //HI_U32 v0_hfir_en ;
    HI_U32 ck_gt_en         ;//use for ck_gt_en
    HI_U32 mid_en         ;
    XDP_HFIR_MODE_E hfir_mode       ;//use for mode

}VDP_V1_HFIR_CFG_S;

typedef struct
{
    HI_U32 csc_en;
    HI_U32 csc_ck_gt_en;
}VDP_CSC_V1_CFG_S;

typedef enum tagVDP_DATA_RMODE_E
{
    VDP_RMODE_INTERFACE   = 0,
    VDP_RMODE_INTERLACE   = 0,
    VDP_RMODE_PROGRESSIVE = 1,
    VDP_RMODE_TOP         = 2,
    VDP_RMODE_BOTTOM      = 3,
    VDP_RMODE_PRO_TOP     = 4,
    VDP_RMODE_PRO_BOTTOM  = 5,
    VDP_RMODE_BUTT

} VDP_DATA_RMODE_E;

typedef enum
{
    VDP_G0_ZME_TYP =0,
    VDP_G0_ZME_TYP1 ,
    VDP_G0_ZME_RAND ,
    VDP_G0_ZME_MAX ,
    VDP_G0_ZME_MIN ,
    VDP_G0_ZME_ZERO ,
    VDP_G0_ZME_BUTT
}GF_G0_ZME_MODE_E;

typedef enum
{
    VDP_G0_ZME_FIR_COPY = 0,
    VDP_G0_ZME_FIR_FILT = 1
}GF_G0_ZME_FIR_MODE_E;

typedef struct
{
    HI_U32 ck_gt_en         ;
    HI_U32 in_width         ;
    HI_U32 out_width        ;
    HI_U32 out_pro          ;

    HI_U32 hfir_en          ;
    HI_U32 lhmid_en         ;
    HI_U32 ahmid_en         ;
    HI_U32 lhfir_mode       ;
    HI_U32 ahfir_mode       ;

    HI_U32 in_height        ;
    HI_U32 out_height       ;

    HI_U32  vfir_en         ;
    HI_U32 lvmid_en         ;
    HI_U32 avmid_en         ;
    HI_U32 lvfir_mode       ;
    HI_U32 avfir_mode       ;

}GF_G0_ZME_CFG_S;


typedef enum tagGF_RM_COEF_MODE_E
{
    GF_RM_COEF_MODE_TYP  = 0x0,
    GF_RM_COEF_MODE_RAN  = 0x1,
    GF_RM_COEF_MODE_MIN  = 0x2,
    GF_RM_COEF_MODE_MAX  = 0x3,
    GF_RM_COEF_MODE_ZRO  = 0x4,
    GF_RM_COEF_MODE_CUS  = 0x5,
    GF_RM_COEF_MODE_UP   = 0x6,
//    GF_RM_COEF_MODE_DOWN = 0x7,
    GF_RM_COEF_MODE_BUTT
} GF_RM_COEF_MODE_E;

typedef enum
{
    GFX_DCMP_SRC_MODE_ARGB8888 = 0,
    GFX_DCMP_SRC_MODE_ARGB6888    ,
    GFX_DCMP_SRC_MODE_ARGB1555    ,
    GFX_DCMP_SRC_MODE_ARGB4444    ,

    GFX_DCMP_SRC_MODE_BUTT
}GFX_DCMP_SRC_MODE_E;


HI_VOID VOU_DRV_WriteReg(HI_U64 u64Addr, HI_U32 u32Value);
HI_VOID VOU_DRV_ReadReg(HI_U64 u64Addr, HI_U32 *pu32Value);
HI_VOID VOU_DRV_BoardInit(HI_VOID);
HI_VOID VOU_DRV_IntRegUpMode(VO_LAYER VoLayer, VOU_INT_MODE_E IntMode);
HI_VOID VOU_DRV_SetDevIntfType(VO_DEV VoDev, VO_INTF_TYPE_E enIntfType);
HI_VOID VOU_DRV_SetDevBkGrd(VO_DEV VoDev, HI_U32 u32BgColor);
HI_VOID VOU_DRV_SetDevOutSync(VO_DEV VoDev, VO_INTF_SYNC_E enVoOutMode);
HI_VOID VOU_DRV_DateSetting(VO_DEV VoDev, VO_INTF_SYNC_E enOutSync);

/* interrupt relative                                                       */
HI_VOID VOU_DRV_DevIntEnable(VO_DEV VoDev, HI_BOOL Enable);
HI_VOID VOU_DRV_IntSetMode(VO_DEV VoDev, VOU_INT_MODE_E IntMode);

/* surface relative                                                         */
HI_VOID VOU_DRV_LayerEnable(VO_LAYER VoLayer, HI_BOOL Enable);
/* mixer relative                                                           */
HI_VOID VO_DRV_SetAllCrgClk(HI_BOOL bClkEn);
HI_U32 VO_DRV_GetDevIntfType(VO_DEV VoDev);
HI_VOID VOU_DRV_Open(VO_DEV VoDev);
HI_VOID VOU_DRV_Close(VO_DEV VoDev);
HI_VOID VOU_DRV_DefaultSetting(HI_VOID);

HI_VOID  VOU_DRV_SetDevClk(VO_DEV VoDev);
HI_VOID VO_VID_SetZmeEnable(HI_U32 enLayer, VDP_VID_IP_CFG_S* pst_vid_cfg);

HI_S32 GRAPHIC_DRV_SetCscCoef(HAL_DISP_LAYER_E gfxLayer, VO_CSC_S *pstGfxCsc, CscCoefParam_S * pstCscCoefParam);
HI_VOID VOU_DRV_DefLayerBindDev(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of __VOU_DRV_H__ */

