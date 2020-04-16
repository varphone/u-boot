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

#include <common.h>
#include <command.h>
#include <malloc.h>
//#include <devices.h>
#include <version.h>

#include <asm/arch/platform.h>
//#include <asm/sizes.h>

#include "hi_type.h"
#include "vou_reg.h"
#include "vou_def.h"
#include "vou_hal.h"
#include "vou_drv.h"
#include "vou_coef_org.h"
//#include "vou_hdmi.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define VOU_DEV_MAX_NUM             2
#define VOU_LAYER_MAX_NUM           2

#define VOU_SD_VTTH_WATERLINE       100
#define VOU_HD_VTTH_WATERLINE       240

#define VOU_BACKGROUD_BLACK         0x8080
#define VOU_BACKGROUD_GREEN         0x804D3A42
#define VOU_BACKGROUD_WHITE         0x3fffffff
#define VOU_BACKGROUD_DEFAULT       VOU_BACKGROUD_WHITE

/* vou interrupt mask type */
typedef enum tagVO_INT_MASK_E
{
    VO_INTMSK_NONE         =   0,
    VO_INTMSK_DHD0_VTTHD1  = 0x1,
    VO_INTMSK_DHD0_VTTHD2  = 0x2,
    VO_INTMSK_DHD0_VTTHD3  = 0x4,
    VO_INTMSK_DHD0_UFINT   = 0x8,

    VO_INTMSK_DHD1_VTTHD1  = 0x10,
    VO_INTMSK_DHD1_VTTHD2  = 0x20,
    VO_INTMSK_DHD1_VTTHD3  = 0x40,
    VO_INTMSK_DHD1_UFINT   = 0x80,

    VO_INTMSK_DSD_VTTHD1   = 0x100,
    VO_INTMSK_DSD_VTTHD2   = 0x200,
    VO_INTMSK_DSD_VTTHD3   = 0x400,
    VO_INTMSK_DSD_UFINT    = 0x800,

    VO_INTMSK_B0_ERR       = 0x1000,
    VO_INTMSK_B1_ERR       = 0x2000,
    VO_INTMSK_B2_ERR       = 0x4000,

    VO_INTMSK_WBC_DHDOVER = 0x8000,

    //VOU_INTMSK_DSDVTTHD3 = 0x100,
    //VOU_INTMSK_DSDVTTHD4 = 0x200,
    //VOU_INTMSK_G0WBCVTE  = 0x400,
    //VOU_INTMSK_G4WBCVTE  = 0x800,


    //VOU_INTMSK_LNKTEINT  = 0x1000,
    //VO_INTMSK_DHD1_UFINT= 0x2000,

    // VOU_INTMSK_DSD1UFINT= 0x4000,
    //VOU_INTMSK_DSD2UFINT = 0x8000,

    //VOU_INTMSK_DSD3UFINT = 0x10000,
    //VOU_INTMSK_DSD4UFINT = 0x20000,
    //VOU_INTMSK_DSD5UFINT = 0x40000,

    //VOU_INTMSK_VSDRRINT = 0x100000,
    //VOU_INTMSK_VADRRINT = 0x200000,
    //VOU_INTMSK_VHDRRINT = 0x400000,
    //VOU_INTMSK_G0RRINT  = 0x800000,

    //VOU_INTMSK_G1RRINT = 0x1000000,

    //VOU_INTMSK_RRERRINT = 0x10000000,
    //VOU_INTMSK_VDAC_CVBS_ICINT = 0x1000,
    //VOU_INTMSK_VDACICINT = 0x20000000,
    //VOU_INTMSK_UTENDINT = 0x40000000,
    //VOU_INTMSK_BEINT    = 0x80000000,

    VO_INTREPORT_ALL = 0xffffffff
} VO_INT_MASK_E;



typedef struct hiHAL_DEV_CONFIG_S
{
    HI_BOOL         bEnable;
    HI_U32          u32BkGrd;
    VO_INTF_TYPE_E  enIntfType;
    VO_INTF_SYNC_E  enOutSync;
    HAL_DISP_PIXEL_FORMAT_E enPixelFmt;
} HAL_DEV_CONFIG_S;

typedef struct hiHAL_LAYER_CONFIG_S
{
    HI_U32             u32BkGrd;
} HAL_LAYER_CONFIG_S;

typedef struct hiHAL_CSC_VALUE_S
{
    HI_U32 u32Luma;
    HI_U32 u32Cont;
    HI_U32 u32Hue;
    HI_U32 u32Satu;

} HAL_CSC_VALUE_S;

typedef struct hiHAL_COEF_ADDR_S
{
    HI_U32 u32BasePhys;
    HI_VOID *pBaseVirt;
    HI_U32 u32Hor;
    HI_U32 u32Ver422;
    HI_U32 u32Ver420;
    HI_U32 u32Lut;
    HI_U32 u32Gam;
    HI_U32 u32Acc;
} HAL_COEF_ADDR_S;

/****************************************************************************
 * EXTERN VARIABLES                                                         *
 ****************************************************************************/


/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
//static const HI_S16 g_stNotchCoef_960h_lowpass[23] = { 2, -3,   4, -4,   5, -5,   6,  -7,  7, -8,  8, 246,  8, -8,  7,  -7,   6, -5,   5, -4,   4, -3,  2};  //lowpass 7.75


HAL_DISP_SYNCINFO_S g_stSyncTiming[VO_OUTPUT_BUTT] =
{
    /* |--INTFACE---||-----TOP-----||----HORIZON--------||----BOTTOM-----||-PULSE-||-INVERSE-| */
    /*Synm, Iop, Itf,   Vact, Vbb,  Vfb,  Hact,  Hbb,  Hfb, Hmid,Bvact,Bvbb, Bvfb, Hpw, Vpw,Idv, Ihs, Ivs */
    {    0,     0,    0,    288,   22,    2,    720,   132,   12,    1,     288,   23,      2,    126,    3,   0,    0,    0},     /* 576I(PAL)  */  // HDMI:hmid 300; YPbPr:hmid 0;
    {    0,     0,    0,    240,   18,    4,    720,   119,   19,    1,     240,   19,      4,    124,    3,   0,    0,    0},     /* 480I(NTSC) */  // HDMI:hmid 310; YPbPr:hmid 0;

    {    0,     1,    1,   1080,  41,    4,    1920, 192,   638,   1,       1,      1,      1,      44,    5,   0,    0,    0},    /* 1080P@24Hz */
    {    0,     1,    1,   1080,  41,    4,    1920, 192,   528,   1,       1,      1,      1,      44,    5,   0,    0,    0},    /* 1080P@25Hz */
    {    0,     1,    1,   1080,  41,    4,    1920, 192,    88,   1,       1,      1,      1,      44,    5,   0,    0,    0},    /* 1080P@30Hz */

    {0,   1,   1,   720,  25,  5,  1280, 260, 440,   1,    1,   1,  1,  40,  5,  0,  0,  0},    /* 720P@50Hz */
    {0,   1,   1,   720,  25,  5,  1280, 260, 110,   1,    1,   1,  1,  40,  5,  0,  0,  0},    /* 720P@60Hz */
    {0,   0,   1,   540,  20,  2,  1920, 192, 528, 1128,  540,  21,  2,  44,  5,  0,  0,  0},   /* 1080I@50Hz */
    //{0,   0,   1,   540,  20,  3,  1920, 192, 528, 1128,  540,  20,  2,  44,  5,  0,  0,  0},   /* 1080I@50Hz */
    {0,   0,   1,   540,  20,  2,  1920, 192,  88, 908,  540,  21,  2,  44,  5,  0,  0,  0},    /* 1080I@60Hz */
    {0,   1,   1,  1080,  41,  4,  1920, 192, 528,   1,    1,   1,  1,  44,  5,  0,  0,  0},    /* 1080P@50Hz */
    {0,   1,   1,  1080,  41,  4,  1920, 192,  88,   1,    1,   1,  1,  44,  5,  0,  0,  0},    /* 1080P@60Hz */

    {1,   1,   1,   576,   44, 5,   720, 132, 12,   1,    1,   1,  1,  64,  5,  0,  0,  0},     /* 576P@50Hz */
    {1,   1,   1,   480,   36, 9,   720, 122, 16,   1,    1,   1,  1,  62,  6,  0,  0,  0},     /* 480P@60Hz */

    {1,   1,   2,   600,  27,  1,   800, 216,  40,   1,    1,   1,  1, 128,  4,  0,  0,  0},    /* 800*600@60Hz VGA@60Hz*/
    {1,   1,   2,   768,  35,  3,  1024, 296,  24,   1,    1,   1,  1, 136,  6,  0,  1,  1},    /* 1024x768@60Hz */
    {1,   1,   2,  1024,  41,  1,  1280, 360,  48,   1,    1,   1,  1, 112,  3,  0,  0,  0},    /* 1280x1024@60Hz */
    {1,   1,   2,   768,  27,  3,  1366, 356,  70,   1,    1,   1,  1, 143,  3,  0,  0,  0},    /* 1366x768@60Hz */
    {1,   1,   2,   900,  31,  3,  1440, 384,  80,   1,    1,   1,  1, 152,  6,  0,  1,  0},    /* 1440x900@60Hz */
    {1,   1,   2,   800,  28,  3,  1280, 328,  72,   1,    1,   1,  1, 128,  6,  0,  1,  0},    /* 1280*800@60Hz VGA@60Hz*/
    {1,   1,   2,  1200,  49,  1,  1600, 496,  64,   1,    1,   1,  1, 192,  3,  0,  0,  0},    /* 1600*1200@60Hz */
    {1,   1,   2,  1050,  36,  3,  1680, 456, 104,   1,    1,   1,  1, 176,  6,  0,  1,  0},    /* 1680*1050@60Hz */
    //{1,   1,   2,  1200,  42,  3,  1920, 536, 136,   1,    1,   1,  1, 200,  6,  0,  1,  0},    /* 1920*1200@60Hz */
    {1,   1,   2,  1200,  32,  3,  1920, 112,  48,   1,    1,   1,  1,  32,  6,  0,  0,  1},    /* 1920*1200@60Hz CVT (Reduced Blanking) */
    {1,   1,   2,   480,  35, 10,   640, 144,  16,   1,    1,   1,  1,  96,  2,  0,  1,  1},    /* 640*480@60Hz CVT */

    {0,   0,   0,   288,  22,  2,   960, 176,  16,   1,  288,  23,  2, 168,  3,  0,  0,  0},    /* 960H(PAL) */
    {0,   0,   0,   240,  18,  4,   960, 163,  21,   1,  240,  19,  4, 168,  3,  0,  0,  0},    /* 960H(NTSC) */

    {0,   1,   1,  2160,  72,  8,  1920, 192,  88,   1,    1,   1,  1,  44,  5,  0,  0,  0},    /* 1920*2160@30Hz*/
    //{0,   1,   1,  1440,  37,  3,  2560, 80,   48,   1,    1,   1,  1,  32,  6,  0,  0,  0},    /* 2560*1440@30Hz*/
    {1,   1,   2,  1440,  39,  2,  2560, 112,  48,   1,    1,   1,  1,  32,  5,  0,  0,  0},    /* 2560*1440@30Hz*/
    {1,   1,   2,  1440,  39,  2,  2560, 112,  48,   1,    1,   1,  1,  32,  5,  0,  0,  0},    /* 2560*1440@60Hz */
    {0,   1,   2,  1600,  43,  3,  2560, 112,  48,   1,    1,   1,  1,  32,  6,  0,  0,  1},    /* 2560*1600@60Hz CVT (Reduced Blanking)*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,  1276,  1,    1,   1,  1,  88,  10, 0,  0,  0},   /* 3840*2160@24Hz*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,  1056,  1,    1,   1,  1,  88,  10, 0,  0,  0},   /* 3840*2160@25Hz*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,   176,  1,    1,   1,  1,  88,  10, 0,  0,  0},   /* 3840*2160@30Hz*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,  1056,  1,    1,   1,  1,  88,  10, 0,  0,  0},   /* 3840*2160@50Hz*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,   176,  1,    1,   1,  1,  88,  10, 0,  0,  0},   /* 3840*2160@60Hz*/

    {0,   1,   1,  2160,  82,  8,  4096, 384, 1020,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@24 */
    {0,   1,   1,  2160,  82,  8,  4096, 216,  968,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@25 */
    {0,   1,   1,  2160,  82,  8,  4096, 216,   88,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@30 */
    {0,   1,   1,  2160,  82,  8,  4096, 216,  968,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@50 */
    {0,   1,   1,  2160,  82,  8,  4096, 216,   88,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@60 */
    //{0 ,  1,   1,  2160,  82,  8,  5120, 216,   164,  1,    1,   1,  1,  88,  10, 0,  0,  0},   /* 5120x2160@60 */



    /*Synm,  Iop,   Itf, Vact,  Vbb,    Vfb,     Hact,   Hbb,    Hfb,  Hmid,    Bvact,    Bvbb,      Bvfb, Hpw,  Vpw, Idv,    Ihs, Ivs */
    {    0,   1,  1, 240,   15,    9,   320,   65,    7,    1,    240,    14,      9,   1,   1,  0,   0,  0},   /* 320X240@60  8bit LCD*/
    {    0,   1,  1, 240,    2,    2,   320,    5,   10,    1,      1,     1,      1,  10,   1,  0,   0,  0},   /* 320X240@50  6bit LCD */
    {    0,   1,  1, 320,    10,    4,   240,   30,   10,    1,      1,     1,      1,   10,   2,  0,   0,  0}, /* 240X320@50  6bit LCD */
    {    0,   1,  1, 320,    2,    2,   240,   20,   10,    1,      1,     1,      1,   2,   1,  0,   0,  0},   /* 240X320@60 16bit LCD */
    {    0,   1,  1, 600,   23,   12,   800,  210,   46,    1,      1,     1,      1,   2,   1,  0,   0,  0},   /* 800X600@60 24bit LCD*/

    {    0 ,  1,  1, 1280,   24,    8,   720,  123,   99,    1,      1,     1,      1,  24,   4,  0,   0,  0},  /* For MIPI DSI Tx 720 x1280 at 60 Hz */
    {    0 ,  1,  1, 1920,   36,   16,   1080,  28,  130,    1,      1,     1,      1,  24,   4,  0,   0,  0},  /* For MIPI DSI Tx 1080 x1920 at 60 Hz */
    //{    0 ,  1,  1,4320,  164,   16,   7680, 768,  352,    1,      1,     1,      1, 352,  16,  0,   0,  0},   /* 7680x4320@30 */
    {    0 ,  1,  1, 4320,   64,   16,   7680, 768,  552,    1,      1,     1,      1, 176,  20,  0,   0,  0},  /* 7680x4320@30 */
    {}/* User */
};

static HAL_DEV_CONFIG_S g_stHalDevCfg[VOU_DEV_MAX_NUM] =
{
    {
        .bEnable        = HI_FALSE,
        .u32BkGrd       = VOU_BACKGROUD_DEFAULT,
        .enIntfType     = VO_INTF_HDMI | VO_INTF_BT1120 | VO_INTF_BT656,
        .enOutSync      = VO_OUTPUT_1080P60,           /* for debug: VO_OUTPUT_800x600_60 */
        .enPixelFmt     = HAL_INPUTFMT_YCbCr_SEMIPLANAR_422,     /* VOU_PIXERL_FORMAT_RGB888, VOU_PIXERL_FORMAT_YCBCR422*/
    },
    {
        .bEnable        = HI_FALSE,
        .u32BkGrd       = VOU_BACKGROUD_DEFAULT,
        .enIntfType     = VO_INTF_BT1120 | VO_INTF_BT656 | VO_INTF_LCD,
        .enOutSync      = VO_OUTPUT_1080P60,
        .enPixelFmt     = HAL_INPUTFMT_YCbCr_SEMIPLANAR_422,
    }
};

/****************************************************************************
 * EXTERN FUNCTION DECLARATION                                              *
 ****************************************************************************/

/****************************************************************************
 * INTERNAL FUNCTION DECLARATION                                            *
 ****************************************************************************/

/****************************************************************************
 * VOU FUNCTION                                                             *
 *            ---- Assistant Function Area                                  *
 ****************************************************************************/
#if 0
HI_VOID VOU_DRV_WriteReg(HI_U64 u64Addr, HI_U32 u32Value)
{
    HAL_WriteReg((HI_U32*)u64Addr, u32Value);
}

HI_VOID VOU_DRV_ReadReg(HI_U64 u64Addr, HI_U32 *pu32Value)
{
    *pu32Value = HAL_ReadReg((HI_U32*)u64Addr);
}
#endif

HI_VOID VOU_DRV_BoardInit(HI_VOID)
{
    HAL_VOU_Init();
    VOU_DRV_DefaultSetting();

    return;

}

HI_VOID VOU_DRV_IntRegUpMode(VO_LAYER VoLayer, VOU_INT_MODE_E IntMode)
{
    HAL_VIDEO_SetLayerUpMode(VoLayer, IntMode);
    //HAL_LAYER_SetRegUp (VoLayer);

    return;
}

HI_VOID VOU_DRV_SetDevIntfType(HI_S32 VoDev, VO_INTF_TYPE_E enIntfType)
{
    g_stHalDevCfg[VoDev].enIntfType = enIntfType;

    return;
}

HI_VOID VOU_DRV_SetDevBkGrd(HI_S32 VoDev, HI_U32 u32BgColor)
{
    g_stHalDevCfg[VoDev].u32BkGrd = u32BgColor;

    return;
}

HI_VOID VOU_DRV_SetDevOutSync(HI_S32 VoDev, VO_INTF_SYNC_E enVoOutMode)
{
    g_stHalDevCfg[VoDev].enOutSync = enVoOutMode;

    return;
}



/* interrupt relative                                                       */
HI_VOID VOU_DRV_DevIntEnable(VO_DEV VoDev, HI_BOOL Enable)
{
    VO_INT_MASK_E IntType;
    VO_INT_MASK_E hifbIntType = 0x0;

    switch ( VoDev )
    {
        case VOU_DEV_DHD0:
            IntType = VO_INTMSK_DHD0_VTTHD1;
            hifbIntType = VO_INTMSK_DHD0_VTTHD2 | VO_INTMSK_DHD0_VTTHD3;
            break;

        case VOU_DEV_DHD1 :
            IntType = VO_INTMSK_DHD1_VTTHD1;
            hifbIntType = VO_INTMSK_DHD1_VTTHD2 | VO_INTMSK_DHD1_VTTHD3;
            break;

        default:
            return;
    }

    if (HI_TRUE == Enable)
    {
        HAL_DISP_SetIntMask(IntType);
        HAL_DISP_SetIntMask1(hifbIntType);
    }
    else
    {
        HAL_DISP_ClrIntMask(IntType);
        HAL_DISP_ClrIntMask1(hifbIntType);
    }

    return;
}


HI_VOID VOU_DRV_IntSetMode(HI_S32 VoDev, VOU_INT_MODE_E IntMode)
{
    HAL_DISP_SetVtThdMode(VoDev, IntMode);

    return;
}

HI_VOID VOU_DRV_LayerEnable(VO_LAYER VoLayer, HI_BOOL Enable)
{
    HAL_LAYER_EnableLayer(VoLayer, Enable);
    //HAL_LAYER_SetRegUp(VoLayer);

    return;
}




HI_VOID VOU_DRV_DefLayerBindDev(HI_VOID)
{
    HAL_CBM_SetCbmMixerPrio(HAL_DISP_LAYER_VHD0, VOU_MIX_PRIO0, HAL_CBMMIX1);
    HAL_CBM_SetCbmMixerPrio(HAL_DISP_LAYER_GFX0, VOU_MIX_PRIO2, HAL_CBMMIX1);

    HAL_CBM_SetCbmMixerPrio(HAL_DISP_LAYER_VHD1, VOU_MIX_PRIO0, HAL_CBMMIX2);
    HAL_CBM_SetCbmMixerPrio(HAL_DISP_LAYER_GFX1, VOU_MIX_PRIO2, HAL_CBMMIX2);

    return;
}

HI_VOID VOU_DRV_SetDevClk(VO_DEV VoDev)
{
    HI_S32          s32Pll = VoDev;

    HI_U32          u32PPCSel = 1;

    SYS_HAL_VouHd1PPCSel(u32PPCSel);

    if (VoDev == 0)
    {
        HI_U32 u32Frac=0, u32Postdiv1=0, u32Postdiv2=0, u32Fbdiv=0, u32Refdiv=0;

        HI_U32 u32VdpHdClkSel  = 0x0;
        HI_U32 u32LcdMclkDiv   = 0x015E4C3;


        switch (g_stHalDevCfg[VoDev].enOutSync)
        {
            case VO_OUTPUT_PAL :
            case VO_OUTPUT_NTSC :
            case VO_OUTPUT_576P50 :
            case VO_OUTPUT_480P60 :
            {
                //27MHz
                u32Fbdiv       = 81;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 6;
                u32Postdiv2    = 6;
                u32VdpHdClkSel = 0x2;
                break;
            }

            case VO_OUTPUT_1080P24 :
            case VO_OUTPUT_1080P25 :
            case VO_OUTPUT_1080P30 :
            case VO_OUTPUT_720P50 :
            case VO_OUTPUT_720P60 :
            case VO_OUTPUT_1080I50 :
            case VO_OUTPUT_1080I60 :
            case VO_OUTPUT_720x1280_60:    // For MIPI Tx interface.
            {
                //74.25MHz
                u32Fbdiv       = 99;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 4;
                u32Postdiv2    = 4;
                u32VdpHdClkSel = 0x2;
                break;
            }

            case VO_OUTPUT_1080P50 :
            case VO_OUTPUT_1080P60 :
            case VO_OUTPUT_1920x2160_30:
            case VO_OUTPUT_1080x1920_60:   // For MIPI Tx interface.
            {
                //148.5MHz
                u32Fbdiv       = 99;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 4;
                u32Postdiv2    = 2;
                u32VdpHdClkSel = 0x2;
                break;
            }

            case VO_OUTPUT_800x600_60:
            {
                //40MHz
                u32Fbdiv       = 80;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 6;
                u32Postdiv2    = 4;
                u32VdpHdClkSel = 0x2;

                break;
            }

            case VO_OUTPUT_1024x768_60:
            {
                //65MHz
                u32Fbdiv       = 65;
                u32Frac        = 0;
                u32Refdiv      = 1;
                u32Postdiv1    = 6;
                u32Postdiv2    = 4;
                u32VdpHdClkSel = 0x2;
                break;
            }

            case VO_OUTPUT_1280x1024_60:
            {
                //108MHz
                u32Fbdiv       = 72;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 4;
                u32Postdiv2    = 2;
                u32VdpHdClkSel = 0x2;

                break;
            }

            case VO_OUTPUT_1366x768_60:
            {
                //85.5MHz
                u32Fbdiv       = 171;
                u32Frac        = 0;
                u32Refdiv      = 4;
                u32Postdiv1    = 6;
                u32Postdiv2    = 2;
                u32VdpHdClkSel = 0x2;

                break;
            }

            case VO_OUTPUT_1440x900_60:
            {
                //106.5MHz
                u32Fbdiv       = 213;
                u32Frac        = 0;
                u32Refdiv      = 4;
                u32Postdiv1    = 6;
                u32Postdiv2    = 2;
                u32VdpHdClkSel = 0x2;

                break;
            }

            case VO_OUTPUT_1280x800_60:
            {
                //83.5MHz
                u32Fbdiv       = 167;
                u32Frac        = 0;
                u32Refdiv      = 4;
                u32Postdiv1    = 6;
                u32Postdiv2    = 2;
                u32VdpHdClkSel = 0x2;

                break;
            }

            case VO_OUTPUT_1600x1200_60:
            {
                //162MHz
                u32Fbdiv       = 81;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 3;
                u32Postdiv2    = 2;
                u32VdpHdClkSel = 0x1;

                break;
            }

            case VO_OUTPUT_1680x1050_60:
            {
                //146.25MHz
                u32Fbdiv       = 585;
                u32Frac        = 0;
                u32Refdiv      = 8;
                u32Postdiv1    = 6;
                u32Postdiv2    = 2;
                u32VdpHdClkSel = 0x2;

                break;
            }

            case VO_OUTPUT_1920x1200_60:
            {
#if 0
                //193.25MHz,VESA 1920 x 1200 at 60 Hz (non-interlaced) CVT Compliant
                u32Fbdiv = 773;
                u32Frac = 0;
                u32Refdiv = 16;
                u32Postdiv1 = 3;
                u32Postdiv2 = 2;
#else
                // 154MHz,VESA 1920 x 1600 at 60 Hz (non-interlaced) CVT (Reduced Blanking)
                u32Fbdiv    = 154;
                u32Frac     = 0;
                u32Refdiv   = 4;
                u32Postdiv1 = 3;
                u32Postdiv2 = 2;
#endif
                u32VdpHdClkSel = 0x1;    // 0x1 for FHD@120=<sync<=4k60

                break;
            }

            case VO_OUTPUT_640x480_60:
            {
                //25.175MHz,2^24=16777216
                u32Fbdiv    = 113;
                u32Frac     = 4823450;
                u32Refdiv   = 3;
                u32Postdiv1 = 6;
                u32Postdiv2 = 6;
                u32VdpHdClkSel = 0x2;

                break;
            }

            case VO_OUTPUT_320x240_60:
            {
                //u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel  = 0x3;
                u32LcdMclkDiv   = 0x2ad11d;
                //CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_320x240_50:
            {
                //u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel  = 0x2;
                u32LcdMclkDiv   = 0x152306;
               // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_240x320_50:
            {
               // u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel  = 0x2;
                u32LcdMclkDiv   = 0x169f00;
               // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_240x320_60:
            {
                //u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel  = 0x0;
                u32LcdMclkDiv   = 0x90c54;
               // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_800x600_50:
            {
               // u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel  = 0x0;
                u32LcdMclkDiv   = 0x39cc92;
               // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_2560x1440_30:
            {
                //120.9MHz
                u32Fbdiv       = 1209;
                u32Frac        = 0;
                u32Refdiv      = 24;
                u32Postdiv1    = 5;
                u32Postdiv2    = 2;
                u32VdpHdClkSel = 0x2;    // 0x2 for sync<=FHD@60
                break;
            }

            case VO_OUTPUT_2560x1440_60:
            {
                //241.5MHz
                u32Fbdiv       = 161;
                u32Frac        = 0;
                u32Refdiv      = 4;
                u32Postdiv1    = 4;
                u32Postdiv2    = 1;
                u32VdpHdClkSel = 0x1;    // 0x2 for sync<=FHD@60
                break;
            }

            case VO_OUTPUT_2560x1600_60:
            {


                u32Fbdiv       = 179;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 4;
                u32Postdiv2    = 2;
                u32VdpHdClkSel = 0x1;    // 0x1 for FHD@120=<sync<=4k60
                break;
            }

            case VO_OUTPUT_3840x2160_24:
            case VO_OUTPUT_3840x2160_25:
            case VO_OUTPUT_3840x2160_30:
            case VO_OUTPUT_4096x2160_24:
            case VO_OUTPUT_4096x2160_25:
            case VO_OUTPUT_4096x2160_30:
            {
                //297MHz
                u32Fbdiv       = 99;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 4;
                u32Postdiv2    = 1;
                u32VdpHdClkSel = 0x1;
                break;
            }

            case VO_OUTPUT_3840x2160_50:
            case VO_OUTPUT_3840x2160_60:
            case VO_OUTPUT_4096x2160_50:
            case VO_OUTPUT_4096x2160_60:
            {
                //594MHz
                u32Fbdiv       = 99;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 2;
                u32Postdiv2    = 1;

                u32VdpHdClkSel = 0x0;
                break;
            }

            case VO_OUTPUT_7680x4320_30:
            {
                //594MHz
                u32Fbdiv       = 99;
                u32Frac        = 0;
                u32Refdiv      = 2;
                u32Postdiv1    = 2;
                u32Postdiv2    = 1;
                u32VdpHdClkSel = 0x0;
                break;
            }


            default :
            {

                return;
            }
        }

        SYS_HAL_LcdMclkDiv(u32LcdMclkDiv);

        SYS_HAL_VouHd0PPCSel(u32VdpHdClkSel);

        SYS_HAL_SetVoPllFbdiv(s32Pll, u32Fbdiv);
        SYS_HAL_SetVoPllFrac(s32Pll, u32Frac);
        SYS_HAL_SetVoPllRefdiv(s32Pll, u32Refdiv);
        SYS_HAL_SetVoPllPostdiv1(s32Pll, u32Postdiv1);
        SYS_HAL_SetVoPllPostdiv2(s32Pll, u32Postdiv2);


    }
    else
    {

        HI_U32 u32SPLLFrac = 0, u32SPLLPostdiv1 = 0, u32SPLLPostdiv2 = 0, u32SPLLFbdiv = 0, u32SPLLRefdiv = 0;
        HI_U32 u32VdpOutClkSel = 0x0;
        VO_INTF_SYNC_E  enIntfSync;
        HI_U32 u32Hd1DIVMod    = 0x0;
        HI_U32 u32LcdMclkDiv   = 0x015E4C3;

        switch (g_stHalDevCfg[VoDev].enOutSync)
        {
            case VO_OUTPUT_PAL :
            case VO_OUTPUT_NTSC :
            {
                //27MHz
                u32VdpOutClkSel = 0x5;
                break;
            }

            case VO_OUTPUT_1080P24 :
            case VO_OUTPUT_1080P25 :
            case VO_OUTPUT_1080P30 :
            case VO_OUTPUT_720P50 :
            case VO_OUTPUT_720P60 :
            case VO_OUTPUT_1080I50 :
            case VO_OUTPUT_1080I60 :
            case VO_OUTPUT_720x1280_60:   // For MIPI Tx interface.
            {
                //74.25MHz
                u32VdpOutClkSel = 0x1;
                break;
            }

            case VO_OUTPUT_1080P50 :
            case VO_OUTPUT_1080P60 :
            case VO_OUTPUT_1920x2160_30:
            case VO_OUTPUT_1080x1920_60:  // For MIPI Tx interface.
            {
                //148.5MHz
                u32VdpOutClkSel = 0x0;
                break;
            }

            case VO_OUTPUT_576P50 :
            case VO_OUTPUT_480P60 :
            {
                //27MHz
                u32VdpOutClkSel = 0x5;
                break;
            }

            case VO_OUTPUT_800x600_60:
            {
                //40MHz
                u32SPLLFbdiv    = 80;
                u32SPLLFrac     = 0;
                u32SPLLRefdiv   = 2;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 4;

                u32VdpOutClkSel = 0x7;

                break;
            }

            case VO_OUTPUT_1024x768_60:
            {
                // 65MHz
                u32SPLLFbdiv    = 65;
                u32SPLLFrac     = 0;
                u32SPLLRefdiv   = 1;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 4;
                u32VdpOutClkSel = 0x7;
                break;
            }

            case VO_OUTPUT_1280x1024_60:
            {
                //108MHz
                u32SPLLFbdiv    = 72;
                u32SPLLFrac     = 0;
                u32SPLLRefdiv   = 2;
                u32SPLLPostdiv1 = 4;
                u32SPLLPostdiv2 = 2;
                u32VdpOutClkSel = 0x7;
                break;
            }

            case VO_OUTPUT_1366x768_60:
            {
                //85.5MHz
                u32SPLLFbdiv    = 171;
                u32SPLLFrac     = 0;
                u32SPLLRefdiv   = 4;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 2;
                u32VdpOutClkSel = 0x7;
                break;
            }

            case VO_OUTPUT_1440x900_60:
            {
                //106.5MHz
                u32SPLLFbdiv    = 213;
                u32SPLLFrac     = 0;
                u32SPLLRefdiv   = 4;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 2;
                u32VdpOutClkSel = 0x7;
                break;
            }

            case VO_OUTPUT_1280x800_60:
            {
                //83.5MHz
                u32SPLLFbdiv = 167;
                u32SPLLFrac = 0;
                u32SPLLRefdiv = 4;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 2;
                u32VdpOutClkSel = 0x7;
                break;
            }

            case VO_OUTPUT_1600x1200_60:
            {
                //162MHz
                u32SPLLFbdiv = 81;
                u32SPLLFrac = 0;
                u32SPLLRefdiv = 2;
                u32SPLLPostdiv1 = 3;
                u32SPLLPostdiv2 = 2;
                u32VdpOutClkSel = 0x7;

                break;
            }

            case VO_OUTPUT_1680x1050_60:
            {
                //146.25MHz
                u32SPLLFbdiv = 585;
                u32SPLLFrac = 0;
                u32SPLLRefdiv = 8;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 2;
                u32VdpOutClkSel = 0x7;
                break;
            }

            case VO_OUTPUT_1920x1200_60:
            {
                // 154MHz,VESA 1920 x 1600 at 60 Hz (non-interlaced) CVT (Reduced Blanking)
                u32SPLLFbdiv = 154;
                u32SPLLFrac = 0;
                u32SPLLRefdiv = 4;
                u32SPLLPostdiv1 = 3;
                u32SPLLPostdiv2 = 2;
                u32VdpOutClkSel = 0x7;
                break;
            }

            case VO_OUTPUT_640x480_60:
            {
                //25.175MHz,2^24=16777216
                u32SPLLFbdiv = 113;
                u32SPLLFrac = 4823450;
                u32SPLLRefdiv = 3;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 6;
                u32VdpOutClkSel = 0x7;
                break;
            }

            case VO_OUTPUT_960H_PAL:
            {
                // 35.9424MHz
                u32VdpOutClkSel = 0x6;
                u32Hd1DIVMod  = 0x1;
                u32LcdMclkDiv   = 0x83df618;
                //CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_960H_NTSC:
            {
                // 35.967360MHz
                u32VdpOutClkSel = 0x6;
                u32Hd1DIVMod = 0x1;
                u32LcdMclkDiv = 0x83e011c;
                //CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }


            case VO_OUTPUT_320x240_60:
            {
                u32VdpOutClkSel = 0x6;
                u32Hd1DIVMod    = 0x3;
                u32LcdMclkDiv   = 0x82aE4C3;
                //CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_320x240_50:
            {
                u32VdpOutClkSel = 0x6;
                u32Hd1DIVMod    = 0x2;
                u32LcdMclkDiv   = 0x152306;
                //CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_240x320_50:
            {
                u32VdpOutClkSel = 0x6;
                u32Hd1DIVMod    = 0x2;
                u32LcdMclkDiv   = 0x172dfd;
               // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_240x320_60:
            {
                u32VdpOutClkSel = 0x6;
                u32Hd1DIVMod    = 0x0;
                u32LcdMclkDiv   = 0x90c54;
               // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_800x600_50:
            {
                u32VdpOutClkSel = 0x6;
                u32Hd1DIVMod    = 0x0;
                u32LcdMclkDiv   = 0x39cc92;
              //  CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            default :
            {
                return ;
            }
        }

        SYS_HAL_LcdMclkDiv(u32LcdMclkDiv);

        SYS_HAL_VouOutClkSel(u32VdpOutClkSel);
        SYS_HAL_VouHd1DivMode(u32Hd1DIVMod);



        enIntfSync = g_stHalDevCfg[VoDev].enOutSync;

        if ((g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI) &&
            (enIntfSync == VO_OUTPUT_800x600_60   ||
             enIntfSync == VO_OUTPUT_1024x768_60  ||
             enIntfSync == VO_OUTPUT_1280x1024_60 ||
             enIntfSync == VO_OUTPUT_1366x768_60  ||
             enIntfSync == VO_OUTPUT_1440x900_60  ||
             enIntfSync == VO_OUTPUT_1280x800_60  ||
             enIntfSync == VO_OUTPUT_1600x1200_60 ||
             enIntfSync == VO_OUTPUT_1680x1050_60 ||
             enIntfSync == VO_OUTPUT_1920x1200_60 ||
             enIntfSync == VO_OUTPUT_640x480_60))
        {
            SYS_HAL_SetVoSPllFbdiv(s32Pll, u32SPLLFbdiv);
            SYS_HAL_SetVoSPllFrac(s32Pll, u32SPLLFrac);

           SYS_HAL_SetVoSPllPostdiv1(s32Pll, u32SPLLPostdiv1);
           SYS_HAL_SetVoSPllPostdiv2(s32Pll, u32SPLLPostdiv2);
           SYS_HAL_SetVoSPllRefdiv(s32Pll, u32SPLLRefdiv);

        }

    }

    return ;
}




HI_U32 VouDrvGetDevVtth(VO_DEV VoDev)
{
    HI_U32 u32DevVtth;

    switch ( g_stHalDevCfg[VoDev].enOutSync )
    {
        case VO_OUTPUT_3840x2160_60 :
        case VO_OUTPUT_3840x2160_50 :
        case VO_OUTPUT_4096x2160_60 :
        case VO_OUTPUT_4096x2160_50 :
            u32DevVtth = 4 * VOU_HD_VTTH_WATERLINE;
            break;
        case VO_OUTPUT_2560x1600_60 :
        case VO_OUTPUT_3840x2160_24 :
        case VO_OUTPUT_3840x2160_25 :
        case VO_OUTPUT_3840x2160_30 :
        case VO_OUTPUT_4096x2160_24 :
        case VO_OUTPUT_4096x2160_25 :
        case VO_OUTPUT_4096x2160_30 :
            u32DevVtth = 2 * VOU_HD_VTTH_WATERLINE;
            break;
        default:
            u32DevVtth = VOU_HD_VTTH_WATERLINE;
    }



    return u32DevVtth;
}

/* turn on clk*/
HI_VOID VO_DRV_SetAllCrgClk(HI_BOOL bClkEn)
{

    HI_BOOL   bVoClkEn = bClkEn;

    SYS_HAL_VouCFGClkEn( bVoClkEn);
    SYS_HAL_VouAPBClkEn( bVoClkEn);
    SYS_HAL_VouBusClkEn(bVoClkEn);
    return;
}

HI_U32 VO_DRV_GetDevIntfType(VO_DEV VoDev)
{
    return g_stHalDevCfg[VoDev].enIntfType;
}


HI_VOID VOU_DRV_Open(HI_S32 VoDev)
{
    HAL_DISP_BKCOLOR_S       stBkg;
    HAL_DISP_SYNCINFO_S      stSyncInfo;
    HI_U16                   u16VtthLine   = 0;
//    VO_CSC_S                 stVgaCSC;
    HAL_DISP_SYNCINV_S       stInv         = {0};
    VOU_INT_MODE_E           IntMode;
    U_INTF_LCD_CTRL          LCD_CTRL;
    HI_BOOL                  bHdmiClkEn  = 0x1;
    HI_BOOL                  bMipiClkEn  = 0x1;
    HAL_DEV_CONFIG_S*        pstHalDevCfg  = &g_stHalDevCfg[VoDev];
    HAL_MULTICHN_EN          MultiChnEn  = 0x1;
    HI_U32                   u32Hd0DivMod= 0x0;
    VO_INTF_TYPE_E           enIntfType;
    VO_DRV_SetAllCrgClk(HI_TRUE);

    HAL_DISP_SetIntfEnable(VoDev, HI_FALSE);

    SYS_HAL_VouDevClkEn(0, HI_TRUE);
    SYS_HAL_VouDevClkEn(1, HI_TRUE);

    enIntfType = pstHalDevCfg->enIntfType;

    if (VO_INTF_LCD           & enIntfType
        || VO_INTF_LCD_6BIT   & enIntfType
        || VO_INTF_LCD_8BIT   & enIntfType
        || VO_INTF_LCD_16BIT  & enIntfType
        || VO_INTF_LCD_18BIT  & enIntfType
        || VO_INTF_LCD_24BIT  & enIntfType
        || VO_INTF_MIPI       & enIntfType
        || VO_INTF_MIPI_SLAVE & enIntfType
       )
    {
        stBkg.u16Bkg_y  = RGB_R(pstHalDevCfg->u32BkGrd);
        stBkg.u16Bkg_cb = RGB_G(pstHalDevCfg->u32BkGrd);
        stBkg.u16Bkg_cr = RGB_B(pstHalDevCfg->u32BkGrd);
    }else{
        stBkg.u16Bkg_y  = YUV_Y(pstHalDevCfg->u32BkGrd);
        stBkg.u16Bkg_cb = YUV_U(pstHalDevCfg->u32BkGrd);
        stBkg.u16Bkg_cr = YUV_V(pstHalDevCfg->u32BkGrd);
    }

    if (VOU_DEV_DHD0 == VoDev)
    {
        HAL_CBM_SetCbmBkg(HAL_CBMMIX1, &stBkg);
    }
    else if(VOU_DEV_DHD1 == VoDev)
    {
        HAL_CBM_SetCbmBkg(HAL_CBMMIX2, &stBkg);
    }

    /* set interface property */
    memcpy(&stSyncInfo, &g_stSyncTiming[pstHalDevCfg->enOutSync], sizeof(stSyncInfo));
    stInv.u32hs_inv = stSyncInfo.bIhs ? 1 : 0;
    stInv.u32vs_inv = stSyncInfo.bIvs ? 1 : 0;
    stInv.u32dv_inv = stSyncInfo.bIdv ? 1 : 0;

    if (VO_INTF_HDMI & pstHalDevCfg->enIntfType)
    {
        SYS_HAL_VouHdmiClkEn(bHdmiClkEn);       // [27] VDP hdmiclk en.
        SYS_HAL_VouHd0DivMode(VoDev, u32Hd0DivMod);   // [12:13] VDP HD0 div mode

        if ((VO_OUTPUT_576P50 == pstHalDevCfg->enOutSync)
            || (VO_OUTPUT_480P60 == pstHalDevCfg->enOutSync))
        {
            stInv.u32hs_inv = 1 - stInv.u32hs_inv;
            stInv.u32vs_inv = 1 - stInv.u32vs_inv;
        }

    }

    if (VO_INTF_BT1120 & pstHalDevCfg->enIntfType)
    {
        SYS_HAL_VouHdOutPctrl(HI_TRUE);
        SYS_HAL_VouBtClkSel(VoDev);
        SYS_HAL_VouBtClkEn(HI_TRUE);

        HAL_DISP_SetIntfMuxSel(VoDev,VO_INTF_BT1120);
        HAL_INTF_BT_SetDfirEn(0x1);

    }


    if (VO_INTF_MIPI & pstHalDevCfg->enIntfType)
    {
        SYS_HAL_VouMipiClkEn(bMipiClkEn);
        SYS_HAL_VouMipiChnSel(VoDev);

        HAL_DISP_SetIntfMuxSel(VoDev, VO_INTF_MIPI);


        LCD_CTRL.bits.hdmi_mode          = 1;
        HAL_DISP_SetIntfCtrl(pstHalDevCfg->enIntfType, &(LCD_CTRL.u32));
        HAL_DISP_SetIntvsyncTeMode(VoDev, HI_FALSE);
        stInv.u32hs_inv = 0;
        stInv.u32vs_inv = 0;
        stInv.u32dv_inv = 0;
    }

    if (VO_INTF_MIPI_SLAVE & pstHalDevCfg->enIntfType)
    {
        SYS_HAL_VouMipiClkEn(bMipiClkEn);
        SYS_HAL_VouMipiChnSel(VoDev);
        HAL_DISP_SetIntfMuxSel(VoDev, VO_INTF_MIPI);

        LCD_CTRL.bits.hdmi_mode          = 1;
        HAL_DISP_SetIntfCtrl(pstHalDevCfg->enIntfType, &(LCD_CTRL.u32));
        HAL_DISP_SetIntvsyncTeMode(VoDev, HI_TRUE);
        stInv.u32hs_inv = 0;
        stInv.u32vs_inv = 0;
        stInv.u32dv_inv = 0;
    }

    switch (pstHalDevCfg->enIntfType)
    {
        case VO_INTF_BT1120:
            stSyncInfo.bSynm   = HAL_DISP_SYNC_MODE_TIMING;
            stSyncInfo.u8Intfb = HAL_DISP_BIT_WIDTH_2;
            break;

        case VO_INTF_BT656:
            stSyncInfo.bSynm   = HAL_DISP_SYNC_MODE_TIMING;
            stSyncInfo.u8Intfb = HAL_DISP_BIT_WIDTH_1;
            break;

        default:
            break;

    }


    HAL_DISP_SetIntfSync(VoDev, &stSyncInfo, &stInv);

    MultiChnEn = HAL_MULTICHN_EN_1P1C;
    HAL_DISP_SetDevMultiChnEn(VoDev,MultiChnEn);

    /* set clip */
    if (VO_INTF_HDMI & g_stHalDevCfg[VoDev].enIntfType)
    {
        HAL_DISP_CLIP_S stClipData = {0x10, 0x10, 0x10, 0x3ff, 0x3ff, 0x3ff};
        HAL_DISP_SetIntfClip(VO_INTF_HDMI, HI_TRUE, &stClipData);
    }
    if (VO_INTF_BT1120 & g_stHalDevCfg[VoDev].enIntfType)
    {
        HAL_DISP_CLIP_S stClipData = {0x40, 0x40, 0x40, 0x3ac, 0x3ac, 0x3ac};
        HAL_DISP_SetIntfClip(VO_INTF_BT1120, HI_TRUE, &stClipData);
    }

    if (0 == stSyncInfo.bIop)
    {
        IntMode = VOU_INT_MODE_FIELD;
    }
    else
    {
        IntMode = VOU_INT_MODE_FRAME;
    }
    VOU_DRV_IntSetMode(VoDev, IntMode);
    VOU_DRV_IntRegUpMode(VoDev, IntMode);

    u16VtthLine = stSyncInfo.u16Vact + stSyncInfo.u16Vfb + stSyncInfo.u16Vbb - VouDrvGetDevVtth(VoDev);
    HAL_DISP_SetVtThd(VoDev, u16VtthLine);

    HAL_DISP_SetIntfEnable(VoDev, HI_TRUE);
    VOU_DRV_DevIntEnable(VoDev, HI_TRUE);
    HAL_DISP_SetRegUp(VoDev);
    g_stHalDevCfg[VoDev].bEnable = HI_TRUE;

    return;
}

HI_VOID VOU_DRV_Close(HI_S32 VoDev)
{
    HI_U32            i;
   // HI_BOOL           bHdmiClkEn  = 0x0;

    HAL_DISP_SetIntfEnable(VoDev, HI_FALSE);
    HAL_DISP_SetRegUp(VoDev);

    udelay(25 * 1000);

    if (VO_INTF_HDMI & g_stHalDevCfg[VoDev].enIntfType)
    {
       // SYS_HAL_VouHdmiClkEn(bHdmiClkEn);
    }

    g_stHalDevCfg[VoDev].bEnable = HI_FALSE;

    for (i=0;i<VOU_DEV_MAX_NUM;i++)
    {
        if(g_stHalDevCfg[VoDev].bEnable)
        {
            break;
        }
    }

    if (VOU_DEV_MAX_NUM == i)
    {
        VO_DRV_SetAllCrgClk(HI_FALSE);
    }

    return;
}

HI_VOID VOU_DRV_DefaultSetting(HI_VOID)
{
    HI_U32 i;

    //set each layer global alpha
    for (i=HAL_DISP_LAYER_VHD0; i<=HAL_DISP_LAYER_VHD1; i++)
    {
        HAL_LAYER_SetLayerGalpha(i, 255);
    }

    /* outstanding */
    for (i = HAL_DISP_LAYER_VHD0; i <= HAL_DISP_LAYER_VHD1; i++)
    {
        VOU_DRV_LayerEnable(i, HI_FALSE);

    }


    return;
}



HI_VOID VDP_FUNC_GetCvfirPqCfg(VO_ZME_DS_INFO_S * pstDsInfo, VO_ZME_MODE_E enZmeMode, VO_ZME_COMM_PQ_CFG_S * pstCommPqCfg)
{
    HI_U32 zme_vprec = pstDsInfo->zme_vprec;
    HI_U32 zme_hprec = pstDsInfo->zme_hprec;

    if (enZmeMode == VO_ZME_TYP)
    {
        pstCommPqCfg->vluma_offset        = 0                         ; //  v_phase_rand   ;
        pstCommPqCfg->vchroma_offset      = 0                         ; //  v_phase_rand_c ;
        pstCommPqCfg->vbluma_offset       = -1 * zme_vprec / 2 ;
        pstCommPqCfg->vbchroma_offset     = -1 * zme_vprec / 2 ;
        pstCommPqCfg->vl_flatdect_mode    = 1                         ;
        pstCommPqCfg->vl_coringadj_en     = 1                         ;
        pstCommPqCfg->vl_gain             = 32                        ;
        pstCommPqCfg->vl_coring           = 16                        ;
        pstCommPqCfg->vc_flatdect_mode    = 1                         ;
        pstCommPqCfg->vc_coringadj_en     = 1                         ;
        pstCommPqCfg->vc_gain             = 32                        ;
        pstCommPqCfg->vc_coring           = 16                        ;
        pstCommPqCfg->lhfir_offset        = 0                         ;
        pstCommPqCfg->chfir_offset        = 0                         ;
        pstCommPqCfg->hl_flatdect_mode    =  1                        ;
        pstCommPqCfg->hl_coringadj_en     =  1                        ;
        pstCommPqCfg->hl_gain             =  32                       ;
        pstCommPqCfg->hl_coring           =  16                       ;
        pstCommPqCfg->hc_flatdect_mode    =  1                        ;
        pstCommPqCfg->hc_coringadj_en     =  1                        ;
        pstCommPqCfg->hc_gain             =  32                       ;
        pstCommPqCfg->hc_coring           =  16                       ;

    }
    else if (enZmeMode == VO_ZME_MAX)
    {
        pstCommPqCfg->vluma_offset        =  MAX_OFFSET * zme_vprec ;
        pstCommPqCfg->vchroma_offset      =  MAX_OFFSET * zme_vprec ;
        pstCommPqCfg->vbluma_offset       =  MAX_OFFSET * zme_vprec ;
        pstCommPqCfg->vbchroma_offset     =  MAX_OFFSET * zme_vprec ;
        pstCommPqCfg->vl_flatdect_mode    =  1  ;
        pstCommPqCfg->vl_coringadj_en     =  1  ;
        pstCommPqCfg->vl_gain             =  63 ;
        pstCommPqCfg->vl_coring           =  255;
        pstCommPqCfg->vc_flatdect_mode    =  1  ;
        pstCommPqCfg->vc_coringadj_en     =  1  ;
        pstCommPqCfg->vc_gain             =  63 ;
        pstCommPqCfg->vc_coring           =  255;
        pstCommPqCfg->lhfir_offset        =  MAX_OFFSET * zme_hprec;
        pstCommPqCfg->chfir_offset        =  MAX_OFFSET * zme_hprec;
        pstCommPqCfg->hl_flatdect_mode    =  1    ;
        pstCommPqCfg->hl_coringadj_en     =  1    ;
        pstCommPqCfg->hl_gain             =  63   ;
        pstCommPqCfg->hl_coring           =  255  ;
        pstCommPqCfg->hc_flatdect_mode    =  1    ;
        pstCommPqCfg->hc_coringadj_en     =  1    ;
        pstCommPqCfg->hc_gain             =  63   ;
        pstCommPqCfg->hc_coring           =  255  ;
    }
    else if (enZmeMode == VO_ZME_MIN)
    {
        pstCommPqCfg->vluma_offset        =  MIN_OFFSET * zme_vprec ;
        pstCommPqCfg->vchroma_offset      =  MIN_OFFSET * zme_vprec ;
        pstCommPqCfg->vbluma_offset       =  MIN_OFFSET * zme_vprec ;
        pstCommPqCfg->vbchroma_offset     =  MIN_OFFSET * zme_vprec ;
        pstCommPqCfg->vl_flatdect_mode    =  0;
        pstCommPqCfg->vl_coringadj_en     =  0;
        pstCommPqCfg->vl_gain             =  0;
        pstCommPqCfg->vl_coring           =  0;
        pstCommPqCfg->vc_flatdect_mode    =  0;
        pstCommPqCfg->vc_coringadj_en     =  0;
        pstCommPqCfg->vc_gain             =  0;
        pstCommPqCfg->vc_coring           =  0;
        pstCommPqCfg->lhfir_offset        =  MIN_OFFSET * zme_hprec;
        pstCommPqCfg->chfir_offset        =  MIN_OFFSET * zme_hprec;
        pstCommPqCfg->hl_flatdect_mode    =  0  ;
        pstCommPqCfg->hl_coringadj_en     =  0  ;
        pstCommPqCfg->hl_gain             =  0  ;
        pstCommPqCfg->hl_coring           =  0  ;
        pstCommPqCfg->hc_flatdect_mode    =  0  ;
        pstCommPqCfg->hc_coringadj_en     =  0  ;
        pstCommPqCfg->hc_gain             =  0  ;
        pstCommPqCfg->hc_coring           =  0  ;
    }
    else if (enZmeMode == VO_ZME_RAND)
    {
    }
    else
    {
    }
}


static HI_VOID VOU_DRV_SetLayerCvfirMode(HI_U32 enLayer , VO_ZME_MODE_E enZmeMode, VDP_V1_CVFIR_CFG_S* pstCfg)
{
    //filed declare
    HI_U32 vzme_ck_gt_en     = 0;
    HI_U32 out_pro           = 0;
    HI_U32 out_fmt           = 0;
    HI_U32 out_height        = 0;
    HI_U32 cvfir_en          = 0;
    HI_U32 cvmid_en          = 0;
    HI_U32 cvfir_mode        = 0;
    HI_U32 vratio            = 0;
    HI_U32 vchroma_offset    = 0;
    HI_U32 vbchroma_offset   = 0;
    VO_ZME_DS_INFO_S        stDsInfo    = {0};
    VO_ZME_COMM_PQ_CFG_S    stCommPqCfg = {0};

    stDsInfo.zme_vprec       = ZME_VPREC  ;
    stDsInfo.zme_hprec       = ZME_HPREC  ;

    vzme_ck_gt_en            = pstCfg->ck_gt_en         ;
    cvfir_en                 = pstCfg->cvfir_en         ;
    cvfir_mode               = pstCfg->cvfir_mode       ;
    cvmid_en                 = pstCfg->cvmid_en         ;

    out_pro                  = pstCfg->out_pro          ;
    out_fmt                  = pstCfg->out_fmt          ;
    out_height               = pstCfg->in_height  ;
    vratio                   = stDsInfo.zme_vprec;


    VDP_FUNC_GetCvfirPqCfg(&stDsInfo, enZmeMode, &stCommPqCfg);

    vchroma_offset           =  stCommPqCfg.vchroma_offset  ; //  v_phase_rand_c ;
    vbchroma_offset          =  stCommPqCfg.vbchroma_offset ;


    //drv transfer
    HAL_VIDEO_CVFIR_SetOutHeight(enLayer, out_height);
    HAL_VIDEO_CVFIR_SetOutFmt(enLayer, out_fmt);
    HAL_VIDEO_CVFIR_SetOutPro(enLayer, out_pro);
    HAL_VIDEO_CVFIR_SetVzmeCkGtEn(enLayer, vzme_ck_gt_en);

    HAL_VIDEO_CVFIR_SetCvfirEn(enLayer, cvfir_en);
    HAL_VIDEO_CVFIR_SetCvmidEn(enLayer, cvmid_en);
    HAL_VIDEO_CVFIR_SetCvfirMode(enLayer, cvfir_mode);
    HAL_VIDEO_CVFIR_SetVratio(enLayer, vratio);

    HAL_VIDEO_CVFIR_SetVChromaOffset(enLayer, vchroma_offset);
    HAL_VIDEO_CVFIR_SetVbChromaOffset(enLayer, vbchroma_offset);
}


HI_VOID VO_VID_SetZmeEnable(HI_U32 enLayer, VDP_VID_IP_CFG_S* pst_vid_cfg)
{
    VDP_V1_CVFIR_CFG_S stCvfirCfg;
    //VDP_V1_HFIR_CFG_S  stHfirCfg ;
    //VDP_CSC_V1_CFG_S   stV1CscCfg ;
    stCvfirCfg.hfir_order   =    0;
    stCvfirCfg.lhfir_en     =    0;
    stCvfirCfg.chfir_en     =    0;
    stCvfirCfg.lhmid_en     =    0;
    stCvfirCfg.chmid_en     =    0;
    stCvfirCfg.lhfir_mode   =    0;
    stCvfirCfg.chfir_mode   =    0;
    stCvfirCfg.hl_shootctrl_en   =    0;
    stCvfirCfg.hl_shootctrl_mode =    0;
    stCvfirCfg.hc_shootctrl_en   =    0;
    stCvfirCfg.hc_shootctrl_mode =    0;
    stCvfirCfg.lvfir_en     =    0;
    stCvfirCfg.lvmid_en     =    0;
    stCvfirCfg.lvfir_mode   =    0;
    stCvfirCfg.vl_shootctrl_en   =    0;
    stCvfirCfg.vl_shootctrl_mode =    0;
    stCvfirCfg.vc_shootctrl_en   =    0;
    stCvfirCfg.vc_shootctrl_mode =    0;

    /* CVFIR */
    stCvfirCfg.ck_gt_en     =    0;/*rand()%2*/
    stCvfirCfg.cvfir_en     =    1;
    stCvfirCfg.cvmid_en     =    0;/*rand()%2*/
    stCvfirCfg.cvfir_mode   =    0;/*rand()%2*/
    stCvfirCfg.out_pro      =    VDP_RMODE_PROGRESSIVE;
    stCvfirCfg.out_fmt      =    VDP_PROC_FMT_SP_422;
    stCvfirCfg.in_width     =    pst_vid_cfg->vid_iw;
    stCvfirCfg.in_height    =    pst_vid_cfg->vid_ih;
    stCvfirCfg.out_width    =    pst_vid_cfg->vid_ow;
    stCvfirCfg.out_height   =    pst_vid_cfg->vid_oh;
    VOU_DRV_SetLayerCvfirMode(enLayer, VO_ZME_TYP, &stCvfirCfg);
   // VOU_DRV_SetLayerCvfirCoef(enLayer, VO_RM_COEF_MODE_TYP);



    //osal_printk("[func]pst_vid_cfg->vid_iw   = %d, for layer : %d, %s, %d\n",pst_vid_cfg->vid_iw,enLayer, __FUNCTION__,__LINE__);
    //osal_printk("[func]pst_vid_cfg->vid_ih   = %d, for layer : %d, %s, %d\n",pst_vid_cfg->vid_ih,enLayer, __FUNCTION__,__LINE__);
    //osal_printk("[func]pst_vid_cfg->vid_ow   = %d, for layer : %d, %s, %d\n",pst_vid_cfg->vid_ow,enLayer, __FUNCTION__,__LINE__);
    //osal_printk("[func]pst_vid_cfg->vid_oh   = %d, for layer : %d, %s, %d\n",pst_vid_cfg->vid_oh,enLayer, __FUNCTION__,__LINE__);
}

HI_S32 VOU_DRV_CalcCscMatrix(HI_U32 u32Luma, HI_U32 u32Contrast,
                             HI_U32 u32Hue, HI_U32 u32Satuature, HAL_CSC_MODE_E enCscMode, CscCoef_S* pstCstCoef)
{
    HI_S32 s32Luma     = 0;
    HI_S32 s32Contrast = 0;
    HI_S32 s32Hue      = 0;
    HI_S32 s32Satu     = 0;
    const CscCoef_S* pstCscTmp = NULL;

    s32Luma     = (HI_S32)u32Luma * 64 / 100 - 32;
    s32Contrast = ((HI_S32)u32Contrast - 50) * 2 + 100;
    s32Hue      = (HI_S32)u32Hue * 60 / 100;
    s32Satu     = ((HI_S32)u32Satuature - 50) * 2 + 100;

    switch (enCscMode)
    {
        case HAL_CSC_MODE_BT601_TO_BT601:
        case HAL_CSC_MODE_BT709_TO_BT709:
        case HAL_CSC_MODE_RGB_TO_RGB:
            pstCscTmp = &g_stCSC_Init;
            break;
        case HAL_CSC_MODE_BT709_TO_BT601:
            pstCscTmp = &g_stCSC_YUV2YUV_709_601;
            break;
        case HAL_CSC_MODE_BT601_TO_BT709:
            pstCscTmp = &g_stCSC_YUV2YUV_601_709;
            break;
        case HAL_CSC_MODE_BT601_TO_RGB_PC:
            pstCscTmp = &g_stCSC_YUV6012RGB_pc;
            break;
        case HAL_CSC_MODE_BT709_TO_RGB_PC:
            pstCscTmp = &g_stCSC_YUV7092RGB_pc;
            break;
        #if 0
        case HAL_CSC_MODE_BT2020_TO_RGB_PC:
            pstCscTmp = &g_stCSC_YUV20202RGB_pc;
            break;
        #endif
        case HAL_CSC_MODE_RGB_TO_BT601_PC:
            pstCscTmp = &g_stCSC_RGB2YUV601_pc;
            break;
        case HAL_CSC_MODE_RGB_TO_BT709_PC:
            pstCscTmp = &g_stCSC_RGB2YUV709_pc;
            break;
        #if 0
        case HAL_CSC_MODE_RGB_TO_BT2020_PC:
            pstCscTmp = &g_stCSC_RGB2YUV2020_pc;
            break;
        #endif
        case HAL_CSC_MODE_RGB_TO_BT601_TV:
            pstCscTmp = &g_stCSC_RGB2YUV601_tv;
            break;
        case HAL_CSC_MODE_RGB_TO_BT709_TV:
            pstCscTmp = &g_stCSC_RGB2YUV709_tv;
            break;
        default:
            return HI_FAILURE;
    }

    pstCstCoef->csc_in_dc0  = pstCscTmp->csc_in_dc0;
    pstCstCoef->csc_in_dc1  = pstCscTmp->csc_in_dc1;
    pstCstCoef->csc_in_dc2  = pstCscTmp->csc_in_dc2;
    pstCstCoef->csc_out_dc0 = pstCscTmp->csc_out_dc0;
    pstCstCoef->csc_out_dc1 = pstCscTmp->csc_out_dc1;
    pstCstCoef->csc_out_dc2 = pstCscTmp->csc_out_dc2;


    if ((HAL_CSC_MODE_BT601_TO_RGB_PC == enCscMode) || (HAL_CSC_MODE_BT709_TO_RGB_PC == enCscMode)
        || (HAL_CSC_MODE_BT601_TO_RGB_TV == enCscMode) || (HAL_CSC_MODE_BT709_TO_RGB_TV == enCscMode))
    {
        pstCstCoef->csc_coef00 = (s32Contrast * pstCscTmp->csc_coef00) / 100;
        pstCstCoef->csc_coef01 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef01 * COS_TABLE[s32Hue] - pstCscTmp->csc_coef02 * SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef02 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef01 * SIN_TABLE[s32Hue] + pstCscTmp->csc_coef02 * COS_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef10 = (s32Contrast * pstCscTmp->csc_coef10) / 100;
        pstCstCoef->csc_coef11 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef11 * COS_TABLE[s32Hue] - pstCscTmp->csc_coef12 * SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef12 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef11 * SIN_TABLE[s32Hue] + pstCscTmp->csc_coef12 * COS_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef20 = (s32Contrast * pstCscTmp->csc_coef20) / 100;
        pstCstCoef->csc_coef21 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef21 * COS_TABLE[s32Hue] - pstCscTmp->csc_coef22 * SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef22 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef21 * SIN_TABLE[s32Hue] + pstCscTmp->csc_coef22 * COS_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_in_dc0 += (0 != s32Contrast) ? (s32Luma * 100 / s32Contrast) : s32Luma * 100;
    }
    else
    {
        pstCstCoef->csc_coef00 = (s32Contrast * pstCscTmp->csc_coef00) / 100;
        pstCstCoef->csc_coef01 = (s32Contrast * pstCscTmp->csc_coef01) / 100;
        pstCstCoef->csc_coef02 = (s32Contrast * pstCscTmp->csc_coef02) / 100;
        pstCstCoef->csc_coef10 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef10 * COS_TABLE[s32Hue] + pstCscTmp->csc_coef20 * SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef11 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef11 * COS_TABLE[s32Hue] + pstCscTmp->csc_coef21 * SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef12 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef12 * COS_TABLE[s32Hue] + pstCscTmp->csc_coef22 * SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef20 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef20 * COS_TABLE[s32Hue] - pstCscTmp->csc_coef10 * SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef21 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef21 * COS_TABLE[s32Hue] - pstCscTmp->csc_coef11 * SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef22 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef22 * COS_TABLE[s32Hue] - pstCscTmp->csc_coef12 * SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_out_dc0 += s32Luma;
    }
    return HI_SUCCESS;
}

HI_S32 VOU_DRV_SetCscCoef(HAL_DISP_LAYER_E Layer, VO_CSC_S *pstCsc, CscCoefParam_S * pstCscCoefParam)
{
    CscCoef_S          stCscCoef;
    HAL_CSC_MODE_E     enCscMode = HAL_CSC_MODE_RGB_TO_BT601_TV;

    HI_U32 u32Pre = 8;
    HI_U32 u32DcPre = 4;

    switch (pstCsc->enCscMatrix)
    {
        case VO_CSC_MATRIX_IDENTITY :
            enCscMode = HAL_CSC_MODE_BT601_TO_BT601;
            break;

        case VO_CSC_MATRIX_BT601_TO_BT709 :
            enCscMode = HAL_CSC_MODE_BT601_TO_BT709;
            break;

        case VO_CSC_MATRIX_BT709_TO_BT601 :
            enCscMode = HAL_CSC_MODE_BT709_TO_BT601;
            break;

        case VO_CSC_MATRIX_BT601_TO_RGB_PC :
            enCscMode = HAL_CSC_MODE_BT601_TO_RGB_PC;
            break;

        case VO_CSC_MATRIX_BT709_TO_RGB_PC :
            enCscMode = HAL_CSC_MODE_BT709_TO_RGB_PC;
            break;

        case VO_CSC_MATRIX_BT2020_TO_RGB_PC :
            enCscMode = HAL_CSC_MODE_BT2020_TO_RGB_PC;
            break;

        case VO_CSC_MATRIX_RGB_TO_BT601_PC:
            enCscMode = HAL_CSC_MODE_RGB_TO_BT601_PC;
            break;

        case VO_CSC_MATRIX_RGB_TO_BT709_PC:
            enCscMode = HAL_CSC_MODE_RGB_TO_BT709_PC;
            break;

        case VO_CSC_MATRIX_RGB_TO_BT2020_PC:
            enCscMode = HAL_CSC_MODE_RGB_TO_BT2020_PC;
            break;

        default :
            enCscMode = HAL_CSC_MODE_BT601_TO_BT601;
            break;
    }

    VOU_DRV_CalcCscMatrix(pstCsc->u32Luma, pstCsc->u32Contrast,
        pstCsc->u32Hue, pstCsc->u32Satuature, enCscMode, &stCscCoef);

    stCscCoef.new_csc_clip_max = GFX_CSC_CLIP_MAX;
    stCscCoef.new_csc_clip_min = GFX_CSC_CLIP_MIN;
    stCscCoef.new_csc_scale2p  = GFX_CSC_SCALE;

    stCscCoef.csc_coef00 =(HI_S32)u32Pre*stCscCoef.csc_coef00 * 1024 / 1000;
    stCscCoef.csc_coef01 =(HI_S32)u32Pre*stCscCoef.csc_coef01 * 1024 / 1000;
    stCscCoef.csc_coef02 =(HI_S32)u32Pre*stCscCoef.csc_coef02 * 1024 / 1000;
    stCscCoef.csc_coef10 =(HI_S32)u32Pre*stCscCoef.csc_coef10 * 1024 / 1000;
    stCscCoef.csc_coef11 =(HI_S32)u32Pre*stCscCoef.csc_coef11 * 1024 / 1000;
    stCscCoef.csc_coef12 =(HI_S32)u32Pre*stCscCoef.csc_coef12 * 1024 / 1000;
    stCscCoef.csc_coef20 =(HI_S32)u32Pre*stCscCoef.csc_coef20 * 1024 / 1000;
    stCscCoef.csc_coef21 =(HI_S32)u32Pre*stCscCoef.csc_coef21 * 1024 / 1000;
    stCscCoef.csc_coef22 =(HI_S32)u32Pre*stCscCoef.csc_coef22 * 1024 / 1000;

    stCscCoef.csc_in_dc0  = (HI_S32)u32DcPre*stCscCoef.csc_in_dc0;
    stCscCoef.csc_in_dc1  = (HI_S32)u32DcPre*stCscCoef.csc_in_dc1;
    stCscCoef.csc_in_dc2  = (HI_S32)u32DcPre*stCscCoef.csc_in_dc2;

    stCscCoef.csc_out_dc0 = (HI_S32)u32DcPre*stCscCoef.csc_out_dc0;
    stCscCoef.csc_out_dc1 = (HI_S32)u32DcPre*stCscCoef.csc_out_dc1;
    stCscCoef.csc_out_dc2 = (HI_S32)u32DcPre*stCscCoef.csc_out_dc2;


    HAL_LAYER_SetCscCoef(Layer, &stCscCoef);


    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
 }
#endif
#endif /* End of #ifdef __cplusplus */

