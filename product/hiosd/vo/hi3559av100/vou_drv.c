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
#define VOU_LAYER_MAX_NUM           17

#define VOU_SD_VTTH_WATERLINE       100
#define VOU_HD_VTTH_WATERLINE       240

#define VOU_BACKGROUD_BLACK         0x8080
#define VOU_BACKGROUD_GREEN         0x804D3A42
#define VOU_BACKGROUD_WHITE         0x3fffffff
#define VOU_BACKGROUD_DEFAULT       VOU_BACKGROUD_WHITE

/* vou interrupt mask type */
typedef enum tagVO_INT_MASK_E
{
    VO_INTMSK_NONE      = 0,
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

static HAL_LAYER_CONFIG_S g_stHalLayerCfg[VOU_LAYER_MAX_NUM] =
{
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    },
    {
        .u32BkGrd      = VOU_BACKGROUD_DEFAULT,
    }
};

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
    {1,   1,   2,  1200,  32,  3,  1920, 112,  48,   1,    1,   1,  1,  32,  6,  0,  1,  0},    /* 1920*1200@60Hz CVT (Reduced Blanking) */
    {1,   1,   2,   480,  35, 10,   640, 144,  16,   1,    1,   1,  1,  96,  2,  0,  1,  1},    /* 640*480@60Hz CVT */

    {0,   0,   0,   288,  22,  2,   960, 176,  16,   1,  288,  23,  2, 168,  3,  0,  0,  0},    /* 960H(PAL) */
    {0,   0,   0,   240,  18,  4,   960, 163,  21,   1,  240,  19,  4, 168,  3,  0,  0,  0},    /* 960H(NTSC) */

    {0,   1,   1,  2160,  72,  8,  1920, 192,  88,   1,    1,   1,  1,  44,  5,  0,  0,  0},    /* 1920*2160@30Hz*/
    {0,   1,   1,  1440,  37,  3,  2560, 80,   48,   1,    1,   1,  1,  32,  6,  0,  0,  0},    /* 2560*1440@30Hz*/
    {1,   1,   2,  1440,  39,  2,  2560, 112,  48,   1,    1,   1,  1,  32,  5,  0,  0,  0},    /* 2560*1440@60Hz*/
    {0,   1,   2,  1600,  43,  3,  2560, 112,  48,   1,    1,   1,  1,  32,  6,  0,  0,  0},    /* 2560*1600@60Hz CVT (Reduced Blanking)*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,  1276, 1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 3840*2160@24Hz*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,  1056, 1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 3840*2160@25Hz*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,  176,  1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 3840*2160@30Hz*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,  1056, 1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 3840*2160@50Hz*/
    {0,   1,   1,  2160,  82,  8,  3840, 384,  176,  1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 3840*2160@60Hz*/

    {0,   1,   1,  2160,  82,  8,  4096, 384, 1020,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@24 */
    {0,   1,   1,  2160,  82,  8,  4096, 216,  968,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@25 */
    {0,   1,   1,  2160,  82,  8,  4096, 216,   88,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@30 */
    {0,   1,   1,  2160,  82,  8,  4096, 216,  968,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@50 */
    {0,   1,   1,  2160,  82,  8,  4096, 216,   88,   1,    1,   1,  1,  88,  10, 0,  0,  0},    /* 4096x2160@60 */


    /*Synm,  Iop,   Itf, Vact,  Vbb,    Vfb,     Hact,   Hbb,    Hfb,  Hmid,    Bvact,    Bvbb,      Bvfb, Hpw,  Vpw, Idv,    Ihs, Ivs */
    {    0,   1,  1, 240,   15,    9,   320,   65,    7,    1,    240,    14,      9,   1,   1,  0,   0,  0},   /* 320X240@30 8bit LCD*/
    {    0,   1,  1, 240,    2,    2,   320,    5,   10,    1,      1,     1,      1,  10,   1,  0,   0,  0},   /* 320X240@50 6bit LCD */
    {    0,   1,  1, 320,    2,    2,   240,   20,   10,    1,      1,     1,      1,   2,   1,  0,   0,  0},   /* 240X320 6bit LCD */
    {    0,   1,  1, 320,    2,    2,   240,   20,   10,    1,      1,     1,      1,   2,   1,  0,   0,  0},   /* 240X320 16bit LCD */
    {    0,   1,  1, 600,   23,   12,   800,  210,   46,    1,      1,     1,      1,   2,   1,  0,   0,  0},   /* 800*600@60Hz*/

    {    0 ,  1,  1,1280,   24,    8,   720,  123,   99,    1,      1,     1,      1,  24,   4,  0,   0,  0},   /* For MIPI DSI Tx 720 x1280 at 60 Hz */
    {    0 ,  1,  1,1920,   36,   16,   1080,  28,  130,    1,      1,     1,      1,  24,   4,  0,   0,  0},   /* For MIPI DSI Tx 1080 x1920 at 60 Hz */

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

HI_VOID VOU_DRV_WriteReg(HI_U64 u64Addr, HI_U32 u32Value)
{
    HAL_WriteReg((HI_U32*)u64Addr, u32Value);
}

HI_VOID VOU_DRV_ReadReg(HI_U64 u64Addr, HI_U32 *pu32Value)
{
    *pu32Value = HAL_ReadReg((HI_U32*)u64Addr);
}

HI_VOID VOU_DRV_DacSwitch(HI_U32 u32DacId, HI_BOOL bOpen)
{
    /* todo */
    return;
}

HI_VOID VOU_DRV_BoardInit(HI_VOID)
{
    HAL_VOU_Init();
    VOU_DRV_DefaultSetting();

    return;

}

static inline HI_BOOL VouDrvIsMultiIntf(VO_INTF_TYPE_E enMuxIntf)
{
    HI_U32 u32Num, u32Tmp;

    u32Num = 0;
    u32Tmp = (HI_U32)enMuxIntf;
    while (0 != u32Tmp)
    {
        if (u32Tmp & 0x1)
        {
            u32Num++;
        }
        u32Tmp = u32Tmp >> 1;
    }

    return ((u32Num <= 1) ? HI_FALSE : HI_TRUE);
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

HI_VOID VOU_DRV_DateSetting(VO_DEV VoDev, VO_INTF_SYNC_E enOutSync)
{
    HI_U32 u32Data;
    HI_U32 u32DacGc;
    HI_U32 u32Coeff24;
    HI_U32 u32Coeff50;
    HI_U32 u32Coeff51;
    HI_U32 u32Coeff52;
    HI_U32 u32Coeff53;
    HI_U32 u32Coeff54;
    HI_U32 u32Coeff55;
    HI_U32 u32Coeff57;
    HI_U32 u32Coeff61;

    switch (enOutSync)
    {
        case VO_OUTPUT_PAL :
            //u32Data = 0x628412dc;
            u32Data = 0x628412dc;
            u32DacGc = 0x3a;
            u32Coeff57 = 0x80808282;
            u32Coeff24 = 0x12c99;
            u32Coeff61 = 0x0;
            u32Coeff50 = 0x07FF07FF;
            u32Coeff51 = 0x07FF0204;
            u32Coeff52 = 0x000007FF;
            u32Coeff53 = 0x07BF000C;
            u32Coeff54 = 0x01350135;
            u32Coeff55 = 0x000C07BF;
            /* disable 960H */
            HAL_DISP_SetDateCoeffByIdx(60, 0);
            break;
        case VO_OUTPUT_NTSC :
            //u32Data = 0x108414fc;
            u32Data = 0x000412dc;
            u32DacGc = 0x38;
            u32Coeff57 = 0x80808282;
            u32Coeff24 = 0x12099;
            u32Coeff61 = 0x1;
            u32Coeff50 = 0x07FF07FF;
            u32Coeff51 = 0x07FF0204;
            u32Coeff52 = 0x000007FF;
            u32Coeff53 = 0x07BF000C;
            u32Coeff54 = 0x01350135;
            u32Coeff55 = 0x000C07BF;
            /* disable 960H */
            HAL_DISP_SetDateCoeffByIdx(60, 0);
            break;
        case VO_OUTPUT_960H_PAL :
        case VO_OUTPUT_960H_NTSC :
        {
            if(VO_OUTPUT_960H_PAL == enOutSync)
            {
                u32Data = 0x628412dc;
                u32DacGc = 0x3a;
                u32Coeff57 = 0x80808080;
                u32Coeff24 = 0x12c99;
                u32Coeff61 = 0x0;
                u32Coeff50 = 0x7f9;
                u32Coeff51 = 0x20e;
                u32Coeff52 = 0x7f9;
                u32Coeff53 = 0x79f0014;
                u32Coeff54 = 0x14d014d;
                u32Coeff55 = 0x14079f;
            }
            else if(VO_OUTPUT_960H_NTSC == enOutSync)
            {
                u32Data = 0x000412dc;
                u32DacGc = 0x38;
                u32Coeff57 = 0x80808383;
                u32Coeff24 = 0x180cb;
                u32Coeff61 = 0x1;
                u32Coeff50 = 0x7f9;
                u32Coeff51 = 0x20e;
                u32Coeff52 = 0x7f9;
                u32Coeff53 = 0x79f0014;
                u32Coeff54 = 0x14d014d;
                u32Coeff55 = 0x14079f;
            }
            /* enable 960H date */
            HAL_DISP_SetDateCoeffByIdx(60, 1);
            /* enable notch coef ?? */
            HAL_DISP_SetDateCoeffByIdx(73, 1);
            /* default notch coef */
            //VOU_HAL_DISP_SetDateNotchCoeff(g_stNotchCoef_960h_lowpass);
            break;
        }
        default :
            return;
    }
    HAL_DISP_SetDateCoeff(VoDev, u32Data);
    HAL_DISP_DATE_OutCtrl(VoDev, 0x111111);
    HAL_DISP_SetCvbsGc(VoDev, u32DacGc);
    HAL_DISP_SetDateCoeff22(VoDev, 0xa8);
    HAL_DISP_SetDateCoeff24(VoDev, u32Coeff24);
    HAL_DISP_SetDateCoeff50(VoDev, u32Coeff50);
    HAL_DISP_SetDateCoeff51(VoDev, u32Coeff51);
    HAL_DISP_SetDateCoeff52(VoDev, u32Coeff52);
    HAL_DISP_SetDateCoeff53(VoDev, u32Coeff53);
    HAL_DISP_SetDateCoeff54(VoDev, u32Coeff54);
    HAL_DISP_SetDateCoeff55(VoDev, u32Coeff55);
    HAL_DISP_SetDateCoeff57(VoDev, u32Coeff57);
    HAL_DISP_SetDateCoeff61(VoDev, u32Coeff61);
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

HI_VOID VOU_DRV_SetLayerBkGrd(VO_LAYER VoLayer, HI_U32 u32BkGrd)
{
    g_stHalLayerCfg[VoLayer].u32BkGrd = u32BkGrd;
    return;
}

HI_VOID VOU_DRV_SetLayerDataFmt(VO_LAYER VoLayer, VOU_LAYER_PIXERL_FORMAT_E enDataFmt)
{
    HAL_DISP_LAYER_E         EnVoLayer = VoLayer;
    HAL_DISP_PIXEL_FORMAT_E  enHalFmt  = HAL_DISP_PIXELFORMAT_BUTT;
    switch(enDataFmt)
    {
        case VOU_LAYER_PIXERL_FORMAT_SPYCbCr_400:
            enHalFmt = HAL_INPUTFMT_YCbCr_SEMIPLANAR_400;
            break;
        case VOU_LAYER_PIXERL_FORMAT_SPYCbCr_420:
            enHalFmt = HAL_INPUTFMT_YCbCr_SEMIPLANAR_420;
            break;
        case VOU_LAYER_PIXERL_FORMAT_SPYCbCr_422:
            enHalFmt = HAL_INPUTFMT_YCbCr_SEMIPLANAR_422;
            break;
        default:
            enHalFmt = HAL_INPUTFMT_YCbCr_SEMIPLANAR_420;
            break;
    }
    HAL_LAYER_SetLayerDataFmt(EnVoLayer, enHalFmt);
    return;
}

HI_S32 VOU_DRV_CalcCscMatrix(HI_U32 u32Luma, HI_U32 u32Contrast,
    HI_U32 u32Hue, HI_U32 u32Satuature, HAL_CSC_MODE_E enCscMode, CscCoef_S *pstCstCoef)
{
    HI_S32 s32Luma     = 0;
    HI_S32 s32Contrast = 0;
    HI_S32 s32Hue      = 0;
    HI_S32 s32Satu     = 0;
    const CscCoef_S *pstCscTmp = NULL;

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
        case HAL_CSC_MODE_RGB_TO_BT601_PC:
            pstCscTmp = &g_stCSC_RGB2YUV601_pc;
            break;
        case HAL_CSC_MODE_RGB_TO_BT709_PC:
            pstCscTmp = &g_stCSC_RGB2YUV709_pc;
            break;
        default:
            return HI_FAILURE;
    }

    pstCstCoef->csc_in_dc0 = pstCscTmp->csc_in_dc0;
    pstCstCoef->csc_in_dc1 = pstCscTmp->csc_in_dc1;
    pstCstCoef->csc_in_dc2 = pstCscTmp->csc_in_dc2;
    pstCstCoef->csc_out_dc0 = pstCscTmp->csc_out_dc0;
    pstCstCoef->csc_out_dc1 = pstCscTmp->csc_out_dc1;
    pstCstCoef->csc_out_dc2 = pstCscTmp->csc_out_dc2;


    if ((HAL_CSC_MODE_BT601_TO_RGB_PC == enCscMode) || (HAL_CSC_MODE_BT709_TO_RGB_PC == enCscMode)
        || (HAL_CSC_MODE_BT601_TO_RGB_TV == enCscMode) || (HAL_CSC_MODE_BT709_TO_RGB_TV == enCscMode))
    {
        pstCstCoef->csc_coef00 = (s32Contrast * pstCscTmp->csc_coef00) / 100;
        pstCstCoef->csc_coef01 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef01*COS_TABLE[s32Hue] - pstCscTmp->csc_coef02*SIN_TABLE[s32Hue]) /1000)) / 10000;
        pstCstCoef->csc_coef02 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef01*SIN_TABLE[s32Hue] + pstCscTmp->csc_coef02*COS_TABLE[s32Hue]) /1000)) / 10000;
        pstCstCoef->csc_coef10 = (s32Contrast * pstCscTmp->csc_coef10) / 100;
        pstCstCoef->csc_coef11 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef11*COS_TABLE[s32Hue] - pstCscTmp->csc_coef12*SIN_TABLE[s32Hue]) /1000)) / 10000;
        pstCstCoef->csc_coef12 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef11*SIN_TABLE[s32Hue] + pstCscTmp->csc_coef12*COS_TABLE[s32Hue]) /1000)) / 10000;
        pstCstCoef->csc_coef20 = (s32Contrast * pstCscTmp->csc_coef20) / 100;
        pstCstCoef->csc_coef21 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef21*COS_TABLE[s32Hue] - pstCscTmp->csc_coef22*SIN_TABLE[s32Hue]) /1000)) / 10000;
        pstCstCoef->csc_coef22 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef21*SIN_TABLE[s32Hue] + pstCscTmp->csc_coef22*COS_TABLE[s32Hue]) /1000)) / 10000;
        pstCstCoef->csc_in_dc0 += (0 != s32Contrast) ? (s32Luma * 100 / s32Contrast) : s32Luma * 100;
    }
    else
    {
        pstCstCoef->csc_coef00 = (s32Contrast * pstCscTmp->csc_coef00) / 100;
        pstCstCoef->csc_coef01 = (s32Contrast * pstCscTmp->csc_coef01) / 100;
        pstCstCoef->csc_coef02 = (s32Contrast * pstCscTmp->csc_coef02) / 100;
        pstCstCoef->csc_coef10 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef10*COS_TABLE[s32Hue] + pstCscTmp->csc_coef20*SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef11 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef11*COS_TABLE[s32Hue] + pstCscTmp->csc_coef21*SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef12 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef12*COS_TABLE[s32Hue] + pstCscTmp->csc_coef22*SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef20 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef20*COS_TABLE[s32Hue] - pstCscTmp->csc_coef10*SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef21 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef21*COS_TABLE[s32Hue] - pstCscTmp->csc_coef11*SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_coef22 = (s32Contrast * s32Satu * ((pstCscTmp->csc_coef22*COS_TABLE[s32Hue] - pstCscTmp->csc_coef12*SIN_TABLE[s32Hue]) / 1000)) / 10000;
        pstCstCoef->csc_out_dc0 += s32Luma;
    }
    return HI_SUCCESS;
}

HI_S32 VOU_DRV_GraphicsSetCscCoef(VO_LAYER VoLayer)
{
    CscCoef_S          stCscCoef;
    HAL_CSC_MODE_E     enCscMode;
    HI_U32             hihdr_r2y_scale2p  = 0;
	HI_U32             hihdr_r2y_clip_min = 0;
	HI_U32             hihdr_r2y_clip_max = 0;

    if((VoLayer < LAYER_GFX_START) || (VoLayer > LAYER_GFX_END))
    {
        printf("[%s][%d] VoLayer:%d Not supported\n", __FUNCTION__, __LINE__, VoLayer);
        return HI_FAILURE;
    }

    if (VoLayer == HAL_DISP_LAYER_GFX0 || (VoLayer == HAL_DISP_LAYER_GFX1))
    {
        enCscMode = HAL_CSC_MODE_RGB_TO_BT709_PC;
    }
    else
    {
        printf("[%s][%d] VoLayer:%d Not supported\n", __FUNCTION__, __LINE__, VoLayer);
        return HI_FAILURE;
    }
    if (HAL_DISP_LAYER_GFX0 == VoLayer)
    {

        hihdr_r2y_scale2p  = 0xd;
        hihdr_r2y_clip_min = 0x0;
        hihdr_r2y_clip_max = 0x3ff;
        VOU_DRV_CalcCscMatrix(50,50,50,50,enCscMode, &stCscCoef);
        stCscCoef.csc_coef00 *=8;
        stCscCoef.csc_coef01 *=8;
        stCscCoef.csc_coef02 *=8;
        stCscCoef.csc_coef10 *=8;
        stCscCoef.csc_coef11 *=8;
        stCscCoef.csc_coef12 *=8;
        stCscCoef.csc_coef20 *=8;
        stCscCoef.csc_coef21 *=8;
        stCscCoef.csc_coef22 *=8;

        stCscCoef.csc_out_dc0 *= 4;
        stCscCoef.csc_out_dc1 *= 4;
        stCscCoef.csc_out_dc2 *= 4;


        // CSC dc coef
        HAL_LAYER_SetCscCoef(VoLayer, &stCscCoef);
        // Set scale
        HAL_HIHDR_G_SetHihdrR2yScale2p(hihdr_r2y_scale2p);
        // Set clip Min and Max
        HAL_HIHDR_G_SetHihdrR2yClipMin(hihdr_r2y_clip_min);
        HAL_HIHDR_G_SetHihdrR2yClipMax(hihdr_r2y_clip_max);
    }
    else if(HAL_DISP_LAYER_GFX1 == VoLayer)
    {
    	VOU_DRV_CalcCscMatrix(50,50,50,50,
            enCscMode, &stCscCoef);
        HAL_LAYER_SetCscCoef(VoLayer, &stCscCoef);
    }

    return HI_SUCCESS;
}

HI_S32 VOU_DRV_VideoSetCscCoef(VO_LAYER VoLayer)
{
    CscCoef_S          stCscCoef;
    HAL_CSC_MODE_E     enCscMode;

    if((VoLayer < LAYER_VHD_START) || (VoLayer > LAYER_VHD_END))
    {
        printf("[%s][%d] VoLayer:%d Not supported\n", __FUNCTION__, __LINE__, VoLayer);
        return HI_FAILURE;
    }

    if (VoLayer == HAL_DISP_LAYER_VHD0 || (VoLayer == HAL_DISP_LAYER_VHD1))
    {
        enCscMode = HAL_CSC_MODE_BT709_TO_RGB_PC;
    }
    else
    {
        printf("[%s][%d] VoLayer:%d Not supported\n", __FUNCTION__, __LINE__, VoLayer);
        return HI_FAILURE;
    }
    if (HAL_DISP_LAYER_VHD0 == VoLayer)
    {

        stCscCoef.new_csc_clip_max = 0xeb0; /* 0xeb0 */
        stCscCoef.new_csc_clip_min = 0x100; /* 0x100 */
        stCscCoef.new_csc_scale2p  = 0xb;   /*0xb*/
        VOU_DRV_CalcCscMatrix(50,50,50,50,enCscMode, &stCscCoef);
        stCscCoef.new_csc_coef00 = 8*stCscCoef.csc_coef00;
        stCscCoef.new_csc_coef01 = 8*stCscCoef.csc_coef01;
        stCscCoef.new_csc_coef02 = 8*stCscCoef.csc_coef02;
        stCscCoef.new_csc_coef03 = 8*stCscCoef.csc_coef10;
        stCscCoef.new_csc_coef04 = 8*stCscCoef.csc_coef11;
        stCscCoef.new_csc_coef05 = 8*stCscCoef.csc_coef12;
        stCscCoef.new_csc_coef06 = 8*stCscCoef.csc_coef20;
        stCscCoef.new_csc_coef07 = 8*stCscCoef.csc_coef21;
        stCscCoef.new_csc_coef08 = 8*stCscCoef.csc_coef22;

        stCscCoef.new_csc_in_dc00 = stCscCoef.csc_in_dc0;
        stCscCoef.new_csc_in_dc01 = stCscCoef.csc_in_dc1;
        stCscCoef.new_csc_in_dc02 = stCscCoef.csc_in_dc2;

        stCscCoef.new_csc_out_dc00 = 4*stCscCoef.csc_out_dc0;
        stCscCoef.new_csc_out_dc01 = 4*stCscCoef.csc_out_dc1;
        stCscCoef.new_csc_out_dc02 = 4*stCscCoef.csc_out_dc2;


        // CSC dc coef
        HAL_LAYER_SetCscCoef(VoLayer, &stCscCoef);
    }
    else if(HAL_DISP_LAYER_VHD1 == VoLayer)
    {
    	VOU_DRV_CalcCscMatrix(50,50,50,50,
            enCscMode, &stCscCoef);
        HAL_LAYER_SetCscCoef(VoLayer, &stCscCoef);
    }

    return HI_SUCCESS;
}

HI_VOID VOU_DRV_VGACscConfig(VO_CSC_S *pstVgaCSC)
{
    HAL_CSC_MODE_E enCscMode;
    CscCoef_S stCscCoef;

    switch (pstVgaCSC->enCscMatrix)
    {
        case VO_CSC_MATRIX_BT601_TO_RGB_PC:
            enCscMode = HAL_CSC_MODE_BT601_TO_RGB_PC;
            break;
        case VO_CSC_MATRIX_BT709_TO_RGB_PC :
            enCscMode = HAL_CSC_MODE_BT709_TO_RGB_PC;
            break;
        default :
            enCscMode = HAL_CSC_MODE_BT601_TO_RGB_PC;
            break;
    }

    VOU_DRV_CalcCscMatrix(pstVgaCSC->u32Luma, pstVgaCSC->u32Contrast, pstVgaCSC->u32Hue, pstVgaCSC->u32Satuature,
        enCscMode, &stCscCoef);

    HAL_DISP_SetIntfCscCoef(VO_INTF_VGA,&stCscCoef);
    return;
}

HI_VOID VOU_DRV_DefLayerBindDev(HI_VOID)
{
    HAL_CBM_SetCbmMixerPrio(VOU_LAYER_VHD0, VOU_MIX_PRIO0, HAL_CBMMIX1);
    HAL_CBM_SetCbmMixerPrio(VOU_LAYER_G0,   VOU_MIX_PRIO1, HAL_CBMMIX1);

    HAL_CBM_SetCbmMixerPrio(VOU_LAYER_VHD1, VOU_MIX_PRIO0, HAL_CBMMIX2);
    HAL_CBM_SetCbmMixerPrio(VOU_LAYER_G1,   VOU_MIX_PRIO1, HAL_CBMMIX2);

    return;
}

HI_VOID VOU_DRV_SetDevClk(VO_DEV VoDev)
{
    HI_S32 s32Pll = VoDev;
    /* set vpll */
    if (VoDev == 0)
    {
        HI_U32 u32Frac = 0, u32Postdiv1=0, u32Postdiv2=0, u32Fbdiv=0, u32Refdiv=0;
        //HI_U32 u32VoHdClkDiv;

        //HI_U32 u32VdpOutClkSel = 0x0;
        HI_U32 u32VdpHdClkSel  = 0x0;
        //HI_U32 u32LcdMclkDiv   = 0x015E4C3;

        s32Pll = VoDev;
        switch (g_stHalDevCfg[VoDev].enOutSync)
        {
            case VO_OUTPUT_PAL :
            case VO_OUTPUT_NTSC :
            {
                //27MHz
                //1 TODO
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
                u32Fbdiv = 99;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 4;
                u32Postdiv2 = 4;
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_BT1120)
                {
                    //u32VdpOutClkSel = 0x1;
                }
                break;
            }

            case VO_OUTPUT_1080P50 :
            case VO_OUTPUT_1080P60 :
            case VO_OUTPUT_1920x2160_30:
            case VO_OUTPUT_1080x1920_60:   // For MIPI Tx interface.
            {
                //148.5MHz
                u32Fbdiv = 99;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 4;
                u32Postdiv2 = 2;
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_BT1120)
                {
                    //u32VdpOutClkSel = 0x0;
                }

                u32VdpHdClkSel = 0x2;    // 0x2 for <=FHD@60
                break;
            }

            case VO_OUTPUT_576P50 :
            case VO_OUTPUT_480P60 :
            {
                //27MHz
                u32Fbdiv = 81;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 6;
                u32Postdiv2 = 6;
                break;
            }
            case VO_OUTPUT_800x600_60:
            {
                //40MHz
                u32Fbdiv = 80;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 6;
                u32Postdiv2 = 4;
                break;
            }
            case VO_OUTPUT_1024x768_60:
            {
                //65MHz
                u32Fbdiv = 65;
                u32Frac = 0;
                u32Refdiv = 1;
                u32Postdiv1 = 6;
                u32Postdiv2 = 4;
                break;
            }

            case VO_OUTPUT_1280x1024_60:
            {
                //108MHz
                u32Fbdiv = 72;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 4;
                u32Postdiv2 = 2;
                break;
            }
            case VO_OUTPUT_1366x768_60:
            {
                //85.5MHz
                u32Fbdiv = 171;
                u32Frac = 0;
                u32Refdiv = 4;
                u32Postdiv1 = 6;
                u32Postdiv2 = 2;
                break;
            }
            case VO_OUTPUT_1440x900_60:
            {
                //106.5MHz
                u32Fbdiv = 213;
                u32Frac = 0;
                u32Refdiv = 4;
                u32Postdiv1 = 6;
                u32Postdiv2 = 2;
                break;
            }
            case VO_OUTPUT_1280x800_60:
            {
                //83.5MHz
                u32Fbdiv = 167;
                u32Frac = 0;
                u32Refdiv = 4;
                u32Postdiv1 = 6;
                u32Postdiv2 = 2;
                break;
            }
            case VO_OUTPUT_1600x1200_60:
            {
                //162MHz
                u32Fbdiv = 81;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 3;
                u32Postdiv2 = 2;
                break;
            }
            case VO_OUTPUT_1680x1050_60:
            {
                //146.25MHz
                u32Fbdiv = 585;
                u32Frac = 0;
                u32Refdiv = 8;
                u32Postdiv1 = 6;
                u32Postdiv2 = 2;
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
                u32Fbdiv = 154;
                u32Frac = 0;
                u32Refdiv = 4;
                u32Postdiv1 = 3;
                u32Postdiv2 = 2;
    #endif
                break;
            }
            case VO_OUTPUT_640x480_60:
            {
                //25.175MHz,2^24=16777216
                u32Fbdiv = 113;
                u32Frac = 4823450;
                u32Refdiv = 3;
                u32Postdiv1 = 6;
                u32Postdiv2 = 6;
                break;
            }

            case VO_OUTPUT_320x240_60:
            {
                //u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x3;
                //u32LcdMclkDiv = 0x2ad11d;
                // LCD not supported in boot animation.
                //CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_320x240_50:
            {
                //u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x2;
                //u32LcdMclkDiv = 0x152306;
                // LCD not supported in boot animation.
                // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_240x320_50:
            {
                //u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x2;
                //u32LcdMclkDiv = 0x169f00;
                // LCD not supported in boot animation.
                // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_240x320_60:
            {
                //u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x0;
                //u32LcdMclkDiv = 0x90c54;
                // LCD not supported in boot animation.
                // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_800x600_50:
            {
                //u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x0;
                //u32LcdMclkDiv = 0x39cc92;
                // LCD not supported in boot animation.
                // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }


            case VO_OUTPUT_2560x1440_30:
            {
                //1 TODO
                //148.5MHz?
                u32Fbdiv = 159;
                u32Frac = 2796202;
                u32Refdiv = 2;
                u32Postdiv1 = 4;
                u32Postdiv2 = 4;
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
                //268.5MHz
                /* to calculate again.*/
                u32Fbdiv = 179;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 4;
                u32Postdiv2 = 2;
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
                /* to calculate again. */
                u32Fbdiv = 99;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 4;
                u32Postdiv2 = 1;
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_BT1120)
                {
                    // TODO
                }
                u32VdpHdClkSel = 0x1;
                break;
            }

            case VO_OUTPUT_3840x2160_50:
            case VO_OUTPUT_3840x2160_60:
            case VO_OUTPUT_4096x2160_50:
            case VO_OUTPUT_4096x2160_60:
            {
                //594MHz
                /* to calculate again.*/
                u32Fbdiv = 99;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 2;
                u32Postdiv2 = 1;

                u32VdpHdClkSel = 0x1;

                break;
            }
            case VO_OUTPUT_7680x4320_30:
            {
                //594MHz
                /* to calculate again.*/
                u32Fbdiv = 99;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 2;
                u32Postdiv2 = 1;
                u32VdpHdClkSel = 0x0;
                break;
            }


            default :
            {
                /* user set the pll reg.*/
                return;
            }
        }

        //SYS_HAL_VouOutClkSel(s32Pll,u32VdpOutClkSel);
        SYS_HAL_VouHdClkSel(s32Pll, u32VdpHdClkSel);

        SYS_HAL_SetVoPllFbdiv(s32Pll, u32Fbdiv);
        SYS_HAL_SetVoPllFrac(s32Pll, u32Frac);
        SYS_HAL_SetVoPllRefdiv(s32Pll, u32Refdiv);
        SYS_HAL_SetVoPllPostdiv1(s32Pll, u32Postdiv1);
        SYS_HAL_SetVoPllPostdiv2(s32Pll, u32Postdiv2);
    }
    else
    {
        HI_U32 u32Frac, u32Postdiv1, u32Postdiv2, u32Fbdiv, u32Refdiv;
        HI_U32 u32SPLLFrac = 0, u32SPLLPostdiv1 = 0, u32SPLLPostdiv2 = 0, u32SPLLFbdiv = 0, u32SPLLRefdiv = 0;
        HI_U32 u32VdpOutClkSel = 0x0;
        HI_U32 u32VdpHdClkSel  = 0x0;
        //HI_U32 u32LcdMclkDiv   = 0x015E4C3;

        switch (g_stHalDevCfg[VoDev].enOutSync)
        {
            case VO_OUTPUT_PAL :
            case VO_OUTPUT_NTSC :
            {
                //27MHz
                //1 TODO
                break;
            }

            case VO_OUTPUT_1080P24 :
            case VO_OUTPUT_1080P25 :
            case VO_OUTPUT_1080P30 :
            case VO_OUTPUT_720P50 :
            case VO_OUTPUT_720P60 :
            case VO_OUTPUT_1080I50 :
            case VO_OUTPUT_1080I60 :
            {
                //74.25MHz
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_BT1120)
                {
                    u32VdpOutClkSel = 0x1;
                }
                break;
            }
            case VO_OUTPUT_720x1280_60:   // For MIPI Tx interface.
            {
                //74.25.5MHz
                u32Fbdiv = 99;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 4;
                u32Postdiv2 = 4;
                //74.25MHz
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x1;
                }

                SYS_HAL_SetVoPllFbdiv(s32Pll, u32Fbdiv);
                SYS_HAL_SetVoPllFrac(s32Pll, u32Frac);
                SYS_HAL_SetVoPllRefdiv(s32Pll, u32Refdiv);
                SYS_HAL_SetVoPllPostdiv1(s32Pll, u32Postdiv1);
                SYS_HAL_SetVoPllPostdiv2(s32Pll, u32Postdiv2);

                break;
            }

            case VO_OUTPUT_1080P50 :
            case VO_OUTPUT_1080P60 :
            case VO_OUTPUT_1920x2160_30:
            {
                //148.5MHz
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_BT1120 ||
                    g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x0;
                }
                break;
            }
            case VO_OUTPUT_1080x1920_60:  // For MIPI Tx interface.
            {
                //148.5MHz
                u32Fbdiv = 99;
                u32Frac = 0;
                u32Refdiv = 2;
                u32Postdiv1 = 4;
                u32Postdiv2 = 2;
                //148.5MHz
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x0;
                }
                SYS_HAL_SetVoPllFbdiv(s32Pll, u32Fbdiv);
                SYS_HAL_SetVoPllFrac(s32Pll, u32Frac);
                SYS_HAL_SetVoPllRefdiv(s32Pll, u32Refdiv);
                SYS_HAL_SetVoPllPostdiv1(s32Pll, u32Postdiv1);
                SYS_HAL_SetVoPllPostdiv2(s32Pll, u32Postdiv2);
                break;
            }

            case VO_OUTPUT_576P50 :
            case VO_OUTPUT_480P60 :
            {
                //27MHz
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_BT1120 ||
                    g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x5;
                }
                break;
            }

            case VO_OUTPUT_800x600_60:
            {
                //40MHz
                u32SPLLFbdiv = 80;
                u32SPLLFrac = 0;
                u32SPLLRefdiv = 2;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 4;
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
                break;
            }
            case VO_OUTPUT_1024x768_60:
            {
                // 65MHz
                u32SPLLFbdiv = 65;
                u32SPLLFrac = 0;
                u32SPLLRefdiv = 1;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 4;
                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
                break;
            }
            case VO_OUTPUT_1280x1024_60:
            {
                //108MHz
                u32SPLLFbdiv = 72;
                u32SPLLFrac = 0;
                u32SPLLRefdiv = 2;
                u32SPLLPostdiv1 = 4;
                u32SPLLPostdiv2 = 2;

                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
                break;
            }
            case VO_OUTPUT_1366x768_60:
            {
                //85.5MHz
                u32SPLLFbdiv = 171;
                u32SPLLFrac = 0;
                u32SPLLRefdiv = 4;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 2;

                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
                break;
            }
            case VO_OUTPUT_1440x900_60:
            {
                //106.5MHz
                u32SPLLFbdiv = 213;
                u32SPLLFrac = 0;
                u32SPLLRefdiv = 4;
                u32SPLLPostdiv1 = 6;
                u32SPLLPostdiv2 = 2;

                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
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

                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
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

                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
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

                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
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

                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
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

                if(g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI)
                {
                    u32VdpOutClkSel = 0x7;
                }
                break;
            }


            case VO_OUTPUT_320x240_60:
            {
                u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x3;
                //u32LcdMclkDiv = 0x82aE4C3;
                // Not supported in boot animation.
                // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_320x240_50:
            {
                u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x2;
                //u32LcdMclkDiv = 0x152306;
                // Not supported in boot animation.
                // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_240x320_50:
            {
                u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x2;
                //u32LcdMclkDiv = 0x169f00;
                // Not supported in boot animation.
                // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_240x320_60:
            {
                u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x0;
                //u32LcdMclkDiv = 0x90c54;
                // Not supported in boot animation.
                // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            case VO_OUTPUT_800x600_50:
            {
                u32VdpOutClkSel = 0x6;
                u32VdpHdClkSel = 0x0;
                //u32LcdMclkDiv = 0x39cc92;
                // Not supported in boot animation.
                // CALL_SYS_DrvIoCtrl(&stMppChn, SYS_LCD_MCLK_DIV, &u32LcdMclkDiv);
                break;
            }

            default :
            {
                /* user set the pll reg. */
                return;
            }
        }

        SYS_HAL_VouOutClkSel(VoDev, u32VdpOutClkSel);
        SYS_HAL_VouHd1DivMode(VoDev, u32VdpHdClkSel);
        if((g_stHalDevCfg[VoDev].enIntfType & VO_INTF_MIPI) &&
            (g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_800x600_60   ||
             g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_1024x768_60  ||
             g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_1280x1024_60 ||
             g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_1366x768_60  ||
             g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_1440x900_60  ||
             g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_1280x800_60  ||
             g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_1600x1200_60 ||
             g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_1680x1050_60 ||
             g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_1920x1200_60 ||
             g_stHalDevCfg[VoDev].enOutSync == VO_OUTPUT_640x480_60))
        {
            SYS_HAL_SetVoSPllFbdiv(s32Pll, u32SPLLFbdiv);
            SYS_HAL_SetVoSPllFrac(s32Pll, u32SPLLFrac);
            SYS_HAL_SetVoSPllRefdiv(s32Pll, u32SPLLRefdiv);
            SYS_HAL_SetVoSPllPostdiv1(s32Pll, u32SPLLPostdiv1);
            SYS_HAL_SetVoSPllPostdiv2(s32Pll, u32SPLLPostdiv2);
        }
    }

    return;
}

HI_VOID VOU_DRV_SetVgaGc(HI_U32 u32Value)
{
    HAL_DISP_SetVgaGc(u32Value);
    return;
}

HI_U32 VouDrvGetDevVtth(VO_DEV VoDev)
{
    HI_U32 u32DevVtth;

    if ((VOU_DEV_DHD0 == (VoDev)) || (VOU_DEV_DHD1 == (VoDev)))
    {
        switch ( g_stHalDevCfg[VoDev].enOutSync )
        {
            case VO_OUTPUT_3840x2160_60 :
                u32DevVtth = 4 * VOU_HD_VTTH_WATERLINE;
                break;
            case VO_OUTPUT_2560x1600_60 :
            case VO_OUTPUT_3840x2160_30 :
                u32DevVtth = 2 * VOU_HD_VTTH_WATERLINE;
                break;
            default:
                u32DevVtth = VOU_HD_VTTH_WATERLINE;
        }
    }
    else
    {
        u32DevVtth = VOU_SD_VTTH_WATERLINE;
    }

    return u32DevVtth;
}

/* turn on clk*/
HI_VOID VO_DRV_SetAllCrgClk(HI_BOOL bClkEn)
{
    int       i        = 0;
    HI_BOOL   bVoClkEn = bClkEn;

    for (i=0;i<VOU_DEV_MAX_NUM;i++)
    {
        SYS_HAL_VouDevClkEn(i, bVoClkEn); // dev 0 :[4][5][6] sd ppc,hd ppc,hd0 clk; dev 1 :[4][5][7] sd ppc, hd ppc, hd1 clk
    }
    i = 0;
    SYS_HAL_VouCFGClkEn(i, bVoClkEn);    // [3] CFG clk
    SYS_HAL_VouAPBClkEn(i, bVoClkEn);    // [1] APB clk
    SYS_HAL_VouCoreClkEn(i, bVoClkEn);   // [5]
    SYS_HAL_VouBusClkEn(bVoClkEn);       // [2] AXI bus

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
    VO_CSC_S                 stVgaCSC;
    HAL_DISP_SYNCINV_S       stInv         = {0};
    VOU_INT_MODE_E           IntMode;
    HI_BOOL                  bHdmiClkEn  = 0x1;
    HI_BOOL                  bMipiClkEn  = 0x1;
    HAL_DEV_CONFIG_S*        pstHalDevCfg  = &g_stHalDevCfg[VoDev];
    HAL_MULTICHN_EN          MultiChnEn  = 0x1;
    HI_U32                   u32Hd0DivMod= 0x1;
    HI_U32                   u32Hd1DivMod= 0x0;

    VO_DRV_SetAllCrgClk(HI_TRUE);

    HAL_DISP_SetIntfEnable(VoDev, HI_FALSE);

    if (VO_INTF_HDMI & pstHalDevCfg->enIntfType)
    {
        SYS_HAL_VouHdmiClkEn(VoDev, bHdmiClkEn);       // [27] VDP hdmiclk en.

        SYS_HAL_VouHd0DivMode(VoDev, u32Hd0DivMod);   // [12:13] VDP HD0 div mode
    }
    if (VO_INTF_MIPI & pstHalDevCfg->enIntfType)
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

    if (VO_INTF_BT1120 & pstHalDevCfg->enIntfType)
    {
        HAL_DISP_SetIntfMuxSel(VoDev,VO_INTF_BT1120);
        HAL_DISP_SetIntfSyncInv(VO_INTF_BT1120,&stInv);
        // 1: filter, 0:ignore points
        HAL_INTF_BT_SetDfirEn(0x1);
        SYS_HAL_VouBt1120ChnSel(VoDev);
        SYS_HAL_VouBtBpClkEn(HI_TRUE);
        SYS_HAL_VouBtClkEn(HI_TRUE);

    }
    if (VO_INTF_VGA & pstHalDevCfg->enIntfType)
    {
        stVgaCSC.enCscMatrix = VO_CSC_MATRIX_BT709_TO_RGB_PC;
        stVgaCSC.u32Contrast = 50;
        stVgaCSC.u32Hue = 50;
        stVgaCSC.u32Luma = 50;
        stVgaCSC.u32Satuature = 59;
        VOU_DRV_VGACscConfig(&stVgaCSC);
        VOU_DRV_SetVgaGc(0xa);
        HAL_DISP_SetIntfMuxSel(VoDev,VO_INTF_VGA);
        HAL_DISP_SetIntfCSCEn(VO_INTF_VGA, HI_TRUE);
        HAL_DISP_VgaDacEn(HI_TRUE);
        HAL_DISP_SetIntfDacEnable(VO_INTF_VGA, HI_TRUE);/*enable dac*/
        HAL_DISP_SetIntfSyncInv(VO_INTF_VGA,&stInv);
    }
    if (VO_INTF_HDMI & pstHalDevCfg->enIntfType)
    {
        // For hdmi, do not do this.
        // HAL_DISP_SetIntfMuxSel(VoDev,VO_INTF_HDMI);

        if ((VO_OUTPUT_576P50 == pstHalDevCfg->enOutSync)
            || (VO_OUTPUT_480P60 == pstHalDevCfg->enOutSync))
        {
            stInv.u32hs_inv = 1 - stInv.u32hs_inv;
            stInv.u32vs_inv = 1 - stInv.u32vs_inv;
            HAL_DISP_SetIntfSyncInv(VO_INTF_HDMI,&stInv);
        }
        else
        {
            HAL_DISP_SetIntfSyncInv(VO_INTF_HDMI,&stInv);
        }

        if(VO_OUTPUT_7680x4320_30 == g_stHalDevCfg[VoDev].enOutSync)
        {
            u32Hd0DivMod = 0;
            SYS_HAL_VouHd0DivMode(VoDev, u32Hd0DivMod);   // [12:13] VDP HD0 div mode
        }
    }


    if (VO_INTF_MIPI & pstHalDevCfg->enIntfType)
    {
        SYS_HAL_VouMipiClkEn(VoDev, bMipiClkEn);    // [11] VDP mipi clk en.
        SYS_HAL_VouMipiTxClkEn(VoDev, bMipiClkEn);  // [28] VDP mipi tx clk en.
        SYS_HAL_VouMipiTxChnSel(VoDev);             // [28] VDP mipi tx chn sel.
        if (VOU_DEV_DHD0 == VoDev)
        {
            u32Hd0DivMod = 0x1;
            SYS_HAL_VouHd0DivMode(VoDev, u32Hd0DivMod);
        }
        else if(VOU_DEV_DHD1 == VoDev)
        {
            u32Hd1DivMod = 0x0;
            SYS_HAL_VouHd1DivMode(VoDev, u32Hd1DivMod);
        }
        HAL_DISP_SetIntfMuxSel(VoDev, VO_INTF_MIPI);
    }

    if (HI_TRUE == VouDrvIsMultiIntf(g_stHalDevCfg[VoDev].enIntfType))
    {
        if (VO_INTF_BT656 & g_stHalDevCfg[VoDev].enIntfType)
        {
            stSyncInfo.bSynm   = HAL_DISP_SYNC_MODE_TIMING;
            stSyncInfo.u8Intfb = HAL_DISP_BIT_WIDTH_1;
        }
        else
        {
            stSyncInfo.u8Intfb = HAL_DISP_BIT_WIDTH_2;
        }
    }
    else
    {
        switch(g_stHalDevCfg[VoDev].enIntfType)
        {
            case VO_INTF_BT1120:
                stSyncInfo.bSynm   = HAL_DISP_SYNC_MODE_TIMING;
                stSyncInfo.u8Intfb = HAL_DISP_BIT_WIDTH_2;
                break;
            case VO_INTF_BT656:
                stSyncInfo.bSynm   = HAL_DISP_SYNC_MODE_TIMING;
                stSyncInfo.u8Intfb = HAL_DISP_BIT_WIDTH_1;
                break;
            case VO_INTF_BT656_H:
                stSyncInfo.bSynm   = HAL_DISP_SYNC_MODE_TIMING;
                stSyncInfo.u8Intfb = HAL_DISP_BIT_WIDTH_1;
                break;
            case VO_INTF_BT656_L:
                stSyncInfo.bSynm   = HAL_DISP_SYNC_MODE_TIMING;
                stSyncInfo.u8Intfb = HAL_DISP_BIT_WIDTH_1;
                break;
            case VO_INTF_VGA:
                break;
            case VO_INTF_YPBPR:
                break;
            default:
                break;
        }
    }

    HAL_DISP_SetIntfSync(VoDev, &stSyncInfo);

    MultiChnEn = HAL_MULTICHN_EN_1P1C;
    HAL_DISP_SetDevMultiChnEn(VoDev,MultiChnEn);

    /* set clip */
    if (VO_INTF_HDMI & g_stHalDevCfg[VoDev].enIntfType)
    {
        HAL_DISP_CLIP_S stClipData = {0x10, 0x10, 0x10, 0x3ff, 0x3ff, 0x3ff};
        HAL_DISP_SetIntfClip(VO_INTF_HDMI, HI_TRUE, &stClipData);

        MultiChnEn = HAL_MULTICHN_EN_2P1C;
        HAL_DISP_SetDevMultiChnEn(VoDev,MultiChnEn);
    }

    if (VO_INTF_MIPI & g_stHalDevCfg[VoDev].enIntfType)
    {
        if (VOU_DEV_DHD0 == VoDev)
        {
            MultiChnEn = HAL_MULTICHN_EN_2P1C;
        }
        else if(VOU_DEV_DHD1 == VoDev)
        {
            MultiChnEn = HAL_MULTICHN_EN_1P1C;
        }
        HAL_DISP_SetDevMultiChnEn(VoDev,MultiChnEn);
    }

    if (VO_INTF_BT1120 & g_stHalDevCfg[VoDev].enIntfType)
    {
        HAL_DISP_CLIP_S stClipData = {0x10, 0x10, 0x10, 0x3ff, 0x3ff, 0x3ff};
        HAL_DISP_SetIntfClip(VO_INTF_BT1120, HI_TRUE, &stClipData);
    }

    #if 0
    if ((0 == stSyncInfo.bIop) && VOU_IS_HD_HW_DEV(VoDev)
        && !(VO_INTF_BT656 & g_stHalDevCfg[VoDev].enIntfType))
    #else
    if (0 == stSyncInfo.bIop)
    #endif
    {
        IntMode = VOU_INT_MODE_FIELD;
    }
    else
    {
        IntMode = VOU_INT_MODE_FRAME;
    }

    u16VtthLine = stSyncInfo.u16Vact - VouDrvGetDevVtth(VoDev);
    VOU_DRV_IntSetMode(VoDev, IntMode);
    VOU_DRV_IntRegUpMode(VoDev, IntMode);
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
    HI_BOOL           bHdmiClkEn  = 0x0;

    HAL_DISP_SetIntfEnable(VoDev, HI_FALSE);
    HAL_DISP_SetRegUp(VoDev);
    /* wait for finishing refresh */
    udelay(25 * 1000);

    if (VO_INTF_HDMI & g_stHalDevCfg[VoDev].enIntfType)
    {
        SYS_HAL_VouHdmiClkEn(VoDev,bHdmiClkEn);
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

    HAL_SYS_SetRdBusId(1);

    //set each layer global alpha
    for (i=HAL_DISP_LAYER_VHD0; i<=HAL_DISP_LAYER_VHD1; i++)
    {
        HAL_LAYER_SetLayerGalpha(i, 255);
    }
    for (i=HAL_DISP_LAYER_GFX0; i<=HAL_DISP_LAYER_GFX1; i++)
    {
        HAL_GRAPHIC_SetGfxPalphaRange(i, 1);
    }
    //set each cross bar default PRI
    VOU_DRV_DefLayerBindDev();
    //Vou Clock gate ctrl enable
    HAL_DISP_SetClkGateEnable(1);
    HAL_SYS_SetAxiMaster();
    /* outstanding */
    HAL_SYS_SetOutstanding();
    HAL_DISP_ClearIntStatus(VO_INTREPORT_ALL);
    for (i = HAL_DISP_LAYER_VHD0; i <= HAL_DISP_LAYER_VHD1; i++)
    {
        VOU_DRV_LayerEnable(i, HI_FALSE);
        VOU_DRV_SetLayerDataFmt(i, VOU_LAYER_PIXERL_FORMAT_SPYCbCr_422);
        VOU_DRV_SetLayerBkGrd(i, VOU_BACKGROUD_DEFAULT);
    }

    HAL_DISP_SetIntfMuxDefaultSel();

    return;
}

HI_VOID VO_FUNC_GetZmeCommPqCfg(VO_ZME_DS_INFO_S * pstDsInfo, VO_ZME_MODE_E enZmeMode, VO_ZME_COMM_PQ_CFG_S * pstCommPqCfg)
{
    //HI_S64 h_phase_rand      = 0;
    //HI_S64 h_phase           = 0;
    //HI_S64 h_phase_rand_c    = 0;
    //HI_S64 h_phase_c         = 0;
    //HI_S64 v_phase_rand      = 0;
    //HI_S64 v_phase           = 0;
    //HI_S64 v_phase_rand_c    = 0;
    //HI_S64 v_phase_c         = 0;
    //HI_S64 b_v_phase_rand      = 0;
    //HI_S64 b_v_phase           = 0;
    //HI_S64 b_v_phase_rand_c    = 0;
    //HI_S64 b_v_phase_c         = 0;

    HI_U32 zme_vprec = pstDsInfo->zme_vprec;
    HI_U32 zme_hprec = pstDsInfo->zme_hprec;


    if(enZmeMode == VO_ZME_TYP)
    {
        pstCommPqCfg->vluma_offset        = 0                         ; //  v_phase_rand   ;
        pstCommPqCfg->vchroma_offset      = 0                         ; //  v_phase_rand_c ;
        pstCommPqCfg->vbluma_offset       = -1*zme_vprec/2 ;
        pstCommPqCfg->vbchroma_offset     = -1*zme_vprec/2 ;
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
    else if(enZmeMode == VO_ZME_MAX)
    {
        pstCommPqCfg->vluma_offset        =  MAX_OFFSET*zme_vprec ;
        pstCommPqCfg->vchroma_offset      =  MAX_OFFSET*zme_vprec ;
        pstCommPqCfg->vbluma_offset       =  MAX_OFFSET*zme_vprec ;
        pstCommPqCfg->vbchroma_offset     =  MAX_OFFSET*zme_vprec ;
        pstCommPqCfg->vl_flatdect_mode    =  1  ;
        pstCommPqCfg->vl_coringadj_en     =  1  ;
        pstCommPqCfg->vl_gain             =  63 ;
        pstCommPqCfg->vl_coring           =  255;
        pstCommPqCfg->vc_flatdect_mode    =  1  ;
        pstCommPqCfg->vc_coringadj_en     =  1  ;
        pstCommPqCfg->vc_gain             =  63 ;
        pstCommPqCfg->vc_coring           =  255;
        pstCommPqCfg->lhfir_offset        =  MAX_OFFSET*zme_hprec;
        pstCommPqCfg->chfir_offset        =  MAX_OFFSET*zme_hprec;
        pstCommPqCfg->hl_flatdect_mode    =  1    ;
        pstCommPqCfg->hl_coringadj_en     =  1    ;
        pstCommPqCfg->hl_gain             =  63   ;
        pstCommPqCfg->hl_coring           =  255  ;
        pstCommPqCfg->hc_flatdect_mode    =  1    ;
        pstCommPqCfg->hc_coringadj_en     =  1    ;
        pstCommPqCfg->hc_gain             =  63   ;
        pstCommPqCfg->hc_coring           =  255  ;
    }
    else if(enZmeMode == VO_ZME_MIN)
    {
        pstCommPqCfg->vluma_offset        =  MIN_OFFSET*zme_vprec ;
        pstCommPqCfg->vchroma_offset      =  MIN_OFFSET*zme_vprec ;
        pstCommPqCfg->vbluma_offset       =  MIN_OFFSET*zme_vprec ;
        pstCommPqCfg->vbchroma_offset     =  MIN_OFFSET*zme_vprec ;
        pstCommPqCfg->vl_flatdect_mode    =  0;
        pstCommPqCfg->vl_coringadj_en     =  0;
        pstCommPqCfg->vl_gain             =  0;
        pstCommPqCfg->vl_coring           =  0;
        pstCommPqCfg->vc_flatdect_mode    =  0;
        pstCommPqCfg->vc_coringadj_en     =  0;
        pstCommPqCfg->vc_gain             =  0;
        pstCommPqCfg->vc_coring           =  0;
        pstCommPqCfg->lhfir_offset        =  MIN_OFFSET*zme_hprec;
        pstCommPqCfg->chfir_offset        =  MIN_OFFSET*zme_hprec;
        pstCommPqCfg->hl_flatdect_mode    =  0  ;
        pstCommPqCfg->hl_coringadj_en     =  0  ;
        pstCommPqCfg->hl_gain             =  0  ;
        pstCommPqCfg->hl_coring           =  0  ;
        pstCommPqCfg->hc_flatdect_mode    =  0  ;
        pstCommPqCfg->hc_coringadj_en     =  0  ;
        pstCommPqCfg->hc_gain             =  0  ;
        pstCommPqCfg->hc_coring           =  0  ;
    }
    else if(enZmeMode == VO_ZME_RAND)
    {
        // May be supported until sGetRandEx() is supported.
        /*
        v_phase      = sGetRandEx(0,32);
        v_phase_rand = v_phase*sGetRandEx(MIN_OFFSET,MAX_OFFSET)*zme_vprec/32;
        VDP_PRINT("v_phase      = %d \n",v_phase);
        VDP_PRINT("v_phase_rand = %d \n",v_phase_rand);

        v_phase_c      = sGetRandEx(0,32);
        v_phase_rand_c = v_phase_c*sGetRandEx(MIN_OFFSET,MAX_OFFSET)*zme_vprec/32;
        VDP_PRINT("v_phase_c      = %d \n",v_phase_c);
        VDP_PRINT("v_phase_rand_c = %d \n",v_phase_rand_c);

        b_v_phase      = sGetRandEx(0,32);
        b_v_phase_rand = b_v_phase*sGetRandEx(MIN_OFFSET,MAX_OFFSET)*zme_vprec/32;
        VDP_PRINT("b_v_phase      = %d \n",b_v_phase);
        VDP_PRINT("b_v_phase_rand = %d \n",b_v_phase_rand);

        b_v_phase_c      = sGetRandEx(0,32);
        b_v_phase_rand_c = b_v_phase_c*sGetRandEx(MIN_OFFSET,MAX_OFFSET)*zme_vprec/32;
        VDP_PRINT("b_v_phase_c      = %d \n",b_v_phase_c);
        VDP_PRINT("b_v_phase_rand_c = %d \n",b_v_phase_rand_c);

        h_phase      = sGetRandEx(0,32);
        h_phase_rand = h_phase*sGetRandEx(MIN_OFFSET,MAX_OFFSET)*zme_hprec/32;
        VDP_PRINT("h_phase      = %d \n",h_phase);
        VDP_PRINT("h_phase_rand = %d \n",h_phase_rand);

        h_phase_c      = sGetRandEx(0,32);
        h_phase_rand_c = h_phase_c*sGetRandEx(MIN_OFFSET,MAX_OFFSET)*zme_hprec/32;
        VDP_PRINT("h_phase_c      = %d \n",h_phase_c);
        VDP_PRINT("h_phase_rand_c = %d \n",h_phase_rand_c);

        pstCommPqCfg->vluma_offset        =  v_phase_rand ;
        pstCommPqCfg->vchroma_offset      =  v_phase_rand_c ;
        pstCommPqCfg->vbluma_offset       =  b_v_phase_rand ;
        pstCommPqCfg->vbchroma_offset     =  b_v_phase_rand_c ;
        pstCommPqCfg->vl_flatdect_mode    =  sGetRandEx( 0  , 1      ) ;
        pstCommPqCfg->vl_coringadj_en     =  sGetRandEx( 0  , 1      ) ;
        pstCommPqCfg->vl_gain             =  sGetRandEx( 0  , 63     ) ;
        pstCommPqCfg->vl_coring           =  sGetRandEx( 0  , 255    ) ;
        pstCommPqCfg->vc_flatdect_mode    =  sGetRandEx( 0  , 1      ) ;
        pstCommPqCfg->vc_coringadj_en     =  sGetRandEx( 0  , 1      ) ;
        pstCommPqCfg->vc_gain             =  sGetRandEx( 0  , 63     ) ;
        pstCommPqCfg->vc_coring           =  sGetRandEx( 0  , 255    ) ;
        pstCommPqCfg->lhfir_offset        =  h_phase_rand;
        pstCommPqCfg->chfir_offset        =  h_phase_rand_c;
        pstCommPqCfg->hl_flatdect_mode    =  sGetRandEx( 0  , 1      )  ;
        pstCommPqCfg->hl_coringadj_en     =  sGetRandEx( 0  , 1      )  ;
        pstCommPqCfg->hl_gain             =  sGetRandEx( 0  , 63     )  ;
        pstCommPqCfg->hl_coring           =  sGetRandEx( 0  , 255    )  ;
        pstCommPqCfg->hc_flatdect_mode    =  sGetRandEx( 0  , 1      )  ;
        pstCommPqCfg->hc_coringadj_en     =  sGetRandEx( 0  , 1      )  ;
        pstCommPqCfg->hc_gain             =  sGetRandEx( 0  , 63     )  ;
        pstCommPqCfg->hc_coring           =  sGetRandEx( 0  , 255    )  ;
        //*/
    }
    else
    {
//         MyAssert(0);
    }

}

HI_VOID VO_FUNC_SetV0ZmeMode(HI_U32 enLayer , VO_ZME_MODE_E enZmeMode, VO_V0_ZME_CFG_S * pstCfg)
{
    //filed declare
    HI_U32 hzme_ck_gt_en     = 0;
    HI_U32 out_width         = 0;
    HI_U32 lhfir_en          = 0;
    HI_U32 chfir_en          = 0;
    HI_U32 lhmid_en          = 0;
    HI_U32 chmid_en          = 0;
    HI_U32 non_lnr_en        = 0;
    HI_U32 lhfir_mode        = 0;
    HI_U32 chfir_mode        = 0;
    HI_U32 hfir_order        = 0;
    HI_U32 hratio            = 0;
    HI_U32 lhfir_offset      = 0;
    HI_U32 chfir_offset      = 0;
    HI_U32 zone0_delta       = 0;
    HI_U32 zone2_delta       = 0;
    HI_U32 zone1_end         = 0;
    HI_U32 zone0_end         = 0;
    HI_U32 hl_shootctrl_en   = 0;
    HI_U32 hl_shootctrl_mode = 0;
    HI_U32 hl_flatdect_mode  = 0;
    HI_U32 hl_coringadj_en   = 0;
    HI_U32 hl_gain           = 0;
    HI_U32 hl_coring         = 0;
    HI_U32 hc_shootctrl_en   = 0;
    HI_U32 hc_shootctrl_mode = 0;
    HI_U32 hc_flatdect_mode  = 0;
    HI_U32 hc_coringadj_en   = 0;
    HI_U32 hc_gain           = 0;
    HI_U32 hc_coring         = 0;
    HI_U32 vzme_ck_gt_en     = 0;
    HI_U32 out_pro           = 0;
    HI_U32 out_fmt           = 0;
    HI_U32 out_height        = 0;
    HI_U32 lvfir_en          = 0;
    HI_U32 cvfir_en          = 0;
    HI_U32 lvmid_en          = 0;
    HI_U32 cvmid_en          = 0;
    HI_U32 vfir_1tap_en      = 0;
    HI_U32 lvfir_mode        = 0;
    HI_U32 cvfir_mode        = 0;
    HI_U32 graphdet_en       = 0;
    HI_U32 vratio            = 0;
    HI_U32 vluma_offset      = 0;
    HI_U32 vchroma_offset    = 0;
    HI_U32 vbluma_offset     = 0;
    HI_U32 vbchroma_offset   = 0;
    HI_U32 vl_shootctrl_en   = 0;
    HI_U32 vl_shootctrl_mode = 0;
    HI_U32 vl_flatdect_mode  = 0;
    HI_U32 vl_coringadj_en   = 0;
    HI_U32 vl_gain           = 0;
    HI_U32 vl_coring         = 0;
    HI_U32 vc_shootctrl_en   = 0;
    HI_U32 vc_shootctrl_mode = 0;
    HI_U32 vc_flatdect_mode  = 0;
    HI_U32 vc_coringadj_en   = 0;
    HI_U32 vc_gain           = 0;
    HI_U32 vc_coring         = 0;
    /*
    HI_S32 h_phase_rand      = 0;
    HI_S32 h_phase           = 0;
    HI_S32 h_phase_rand_c    = 0;
    HI_S32 h_phase_c         = 0;
    HI_S32 v_phase_rand      = 0;
    HI_S32 v_phase           = 0;
    HI_S32 v_phase_rand_c    = 0;
    HI_S32 v_phase_c         = 0;
    HI_S32 b_v_phase_rand    = 0;
    HI_S32 b_v_phase         = 0;
    HI_S32 b_v_phase_rand_c  = 0;
    HI_S32 b_v_phase_c       = 0;
    //*/

    VO_ZME_DS_INFO_S        stDsInfo    = {0};
    VO_ZME_COMM_PQ_CFG_S    stCommPqCfg = {0};

    stDsInfo.zme_vprec       = ZME_VPREC  ;
    stDsInfo.zme_hprec       = ZME_HPREC  ;

    hfir_order               = pstCfg->hfir_order       ;
    vzme_ck_gt_en            = pstCfg->ck_gt_en         ;
    lvfir_en                 = pstCfg->lvfir_en         ;
    cvfir_en                 = pstCfg->cvfir_en         ;
    lvfir_mode               = pstCfg->lvfir_mode       ;
    cvfir_mode               = pstCfg->cvfir_mode       ;
    lvmid_en                 = 0;//pstCfg->lvmid_en         ;
    cvmid_en                 = 0;//pstCfg->cvmid_en         ;

    vl_shootctrl_en          = pstCfg->vl_shootctrl_en  ;
    vl_shootctrl_mode        = pstCfg->vl_shootctrl_mode;
    vc_shootctrl_en          = pstCfg->vc_shootctrl_en  ;
    vc_shootctrl_mode        = pstCfg->vc_shootctrl_mode;
    out_pro                  = pstCfg->out_pro          ;
    out_fmt                  = pstCfg->out_fmt          ;
    out_height               = pstCfg->out_height-1     ;
    vratio                   = (pstCfg->in_height * stDsInfo.zme_vprec)/pstCfg->out_height;

    hzme_ck_gt_en            = pstCfg->ck_gt_en         ;
    lhfir_en                 = pstCfg->lhfir_en         ;
    chfir_en                 = pstCfg->chfir_en         ;
    lhfir_mode               = pstCfg->lhfir_mode       ;
    chfir_mode               = pstCfg->chfir_mode       ;
    lhmid_en                 = 0;//pstCfg->lhmid_en         ;
    chmid_en                 = 0;//pstCfg->chmid_en         ;

    hl_shootctrl_en          = pstCfg->hl_shootctrl_en  ;
    hl_shootctrl_mode        = pstCfg->hl_shootctrl_mode;
    hc_shootctrl_en          = pstCfg->hc_shootctrl_en  ;
    hc_shootctrl_mode        = pstCfg->hc_shootctrl_mode;
    out_width                = pstCfg->out_width-1      ;
    hratio                   = (pstCfg->in_width * stDsInfo.zme_hprec)/pstCfg->out_width;


    VO_FUNC_GetZmeCommPqCfg(&stDsInfo, enZmeMode, &stCommPqCfg);

    vluma_offset             =  stCommPqCfg.vluma_offset    ; //  v_phase_rand   ;
    vchroma_offset           =  stCommPqCfg.vchroma_offset  ; //  v_phase_rand_c ;
    vbluma_offset            =  stCommPqCfg.vbluma_offset   ;
    vbchroma_offset          =  stCommPqCfg.vbchroma_offset ;
    vl_coringadj_en          =  stCommPqCfg.vl_coringadj_en ;
    vl_gain                  =  stCommPqCfg.vl_gain         ;
    vl_coring                =  stCommPqCfg.vl_coring       ;
    vc_flatdect_mode         =  stCommPqCfg.vc_flatdect_mode;
    vc_coringadj_en          =  stCommPqCfg.vc_coringadj_en ;
    vc_gain                  =  stCommPqCfg.vc_gain         ;
    vc_coring                =  stCommPqCfg.vc_coring       ;
    lhfir_offset             =  stCommPqCfg.lhfir_offset    ;
    chfir_offset             =  stCommPqCfg.chfir_offset    ;
    hl_flatdect_mode         =  stCommPqCfg.hl_flatdect_mode;
    hl_coringadj_en          =  stCommPqCfg.hl_coringadj_en ;
    hl_gain                  =  stCommPqCfg.hl_gain         ;
    hl_coring                =  stCommPqCfg.hl_coring       ;
    hc_flatdect_mode         =  stCommPqCfg.hc_flatdect_mode;
    hc_coringadj_en          =  stCommPqCfg.hc_coringadj_en ;
    hc_gain                  =  stCommPqCfg.hc_gain         ;
    hc_coring                =  stCommPqCfg.hc_coring       ;


    //drv transfer
    HAL_V0_ZME_SetHzmeCkGtEn( hzme_ck_gt_en);
    HAL_V0_ZME_SetOutWidth( out_width);
    HAL_V0_ZME_SetLhfirEn( lhfir_en);
    HAL_V0_ZME_SetChfirEn( chfir_en);
    HAL_V0_ZME_SetLhmidEn( lhmid_en);
    HAL_V0_ZME_SetChmidEn( chmid_en);
    HAL_V0_ZME_SetNonLnrEn( non_lnr_en);
    HAL_V0_ZME_SetLhfirMode( lhfir_mode);
    HAL_V0_ZME_SetChfirMode( chfir_mode);
    HAL_V0_ZME_SetHfirOrder( hfir_order);
    HAL_V0_ZME_SetHratio( hratio);
    HAL_V0_ZME_SetLhfirOffset( lhfir_offset);
    HAL_V0_ZME_SetChfirOffset( chfir_offset);
    HAL_V0_ZME_SetZone0Delta( zone0_delta);
    HAL_V0_ZME_SetZone2Delta( zone2_delta);
    HAL_V0_ZME_SetZone1End( zone1_end);
    HAL_V0_ZME_SetZone0End( zone0_end);
    HAL_V0_ZME_SetHlShootctrlEn( hl_shootctrl_en);
    HAL_V0_ZME_SetHlShootctrlMode( hl_shootctrl_mode);
    HAL_V0_ZME_SetHlFlatdectMode( hl_flatdect_mode);
    HAL_V0_ZME_SetHlCoringadjEn( hl_coringadj_en);
    HAL_V0_ZME_SetHlGain( hl_gain);
    HAL_V0_ZME_SetHlCoring( hl_coring);
    HAL_V0_ZME_SetHcShootctrlEn( hc_shootctrl_en);
    HAL_V0_ZME_SetHcShootctrlMode( hc_shootctrl_mode);
    HAL_V0_ZME_SetHcFlatdectMode( hc_flatdect_mode);
    HAL_V0_ZME_SetHcCoringadjEn( hc_coringadj_en);
    HAL_V0_ZME_SetHcGain( hc_gain);
    HAL_V0_ZME_SetHcCoring( hc_coring);
    HAL_V0_ZME_SetVzmeCkGtEn( vzme_ck_gt_en);
    HAL_V0_ZME_SetOutPro( out_pro);
    HAL_V0_ZME_SetOutFmt( out_fmt);
    HAL_V0_ZME_SetOutHeight( out_height);
    HAL_V0_ZME_SetLvfirEn( lvfir_en);
    HAL_V0_ZME_SetCvfirEn( cvfir_en);
    HAL_V0_ZME_SetLvmidEn( lvmid_en);
    HAL_V0_ZME_SetCvmidEn( cvmid_en);
    HAL_V0_ZME_SetVfir1tapEn( vfir_1tap_en);
    HAL_V0_ZME_SetLvfirMode( lvfir_mode);
    HAL_V0_ZME_SetCvfirMode( cvfir_mode);
    HAL_V0_ZME_SetGraphdetEn( graphdet_en);
    HAL_V0_ZME_SetVratio( vratio);
    HAL_V0_ZME_SetVlumaOffset( vluma_offset);
    HAL_V0_ZME_SetVchromaOffset( vchroma_offset);
    HAL_V0_ZME_SetVblumaOffset( vbluma_offset);
    HAL_V0_ZME_SetVbchromaOffset( vbchroma_offset);
    HAL_V0_ZME_SetVlShootctrlEn( vl_shootctrl_en);
    HAL_V0_ZME_SetVlShootctrlMode( vl_shootctrl_mode);
    HAL_V0_ZME_SetVlFlatdectMode( vl_flatdect_mode);
    HAL_V0_ZME_SetVlCoringadjEn( vl_coringadj_en);
    HAL_V0_ZME_SetVlGain( vl_gain);
    HAL_V0_ZME_SetVlCoring( vl_coring);
    HAL_V0_ZME_SetVcShootctrlEn( vc_shootctrl_en);
    HAL_V0_ZME_SetVcShootctrlMode( vc_shootctrl_mode);
    HAL_V0_ZME_SetVcFlatdectMode( vc_flatdect_mode);
    HAL_V0_ZME_SetVcCoringadjEn( vc_coringadj_en);
    HAL_V0_ZME_SetVcGain( vc_gain);
    HAL_V0_ZME_SetVcCoring( vc_coring);

}

HI_VOID VDP_FUNC_SetV0HfirMode(HI_U32 enLayer , VO_V0_HFIR_MODE_E V0HfirMode, VO_V0_HFIR_CFG_S * pstCfg)
{
    //filed declare
    HI_U32 ck_gt_en    =  0;
    HI_U32 mid_en    =  0;
    HI_U32 hfir_mode = 0;
    HI_S32 coef0=0;
    HI_S32 coef1=0;
    HI_S32 coef2=0;
    HI_S32 coef3=0;
    HI_S32 coef4=0;
    HI_S32 coef5=0;
    HI_S32 coef6=0;
    HI_S32 coef7=0;

    ck_gt_en  = pstCfg->ck_gt_en;
    hfir_mode = pstCfg->hfir_mode;
    mid_en    = pstCfg->mid_en;


    if(V0HfirMode == VDP_V0_HFIR_TYP)
    {
        //osal_printk("[Follow me] %s,%d enter into typ mode\n", __FUNCTION__,__LINE__);
        coef0 = -7;
        coef1 = 12;
        coef2 = -17;
        coef3 = 25;
        coef4 = -38;
        coef5 = 58;
        coef6 = -105;
        coef7 = 328;
    }
    else if(V0HfirMode == VDP_V0_HFIR_RAND)
    {
        /*
        osal_printk("VDP_FUNC_SetV0HfirMode enter into rand mode\n");
        coef0 = sGetRandEx(-512,511);
        coef1 = sGetRandEx(-512,511);
        coef2 = sGetRandEx(-512,511);
        coef3 = sGetRandEx(-512,511);
        coef4 = sGetRandEx(-512,511);
        coef5 = sGetRandEx(-512,511);
        coef6 = sGetRandEx(-512,511);
        coef7 = sGetRandEx(-512,511);
        //*/

    }
    else if(V0HfirMode == VDP_V0_HFIR_MIN)
    {
        //osal_printk("[Follow me] %s, %d enter into min mode\n", __FUNCTION__, __LINE__);
        coef0 = -512;
        coef1 = -512;
        coef2 = -512;
        coef3 = -512;
        coef4 = -512;
        coef5 = -512;
        coef6 = -512;
        coef7 = -512;
    }
    else if(V0HfirMode == VDP_V0_HFIR_MAX)
    {
        //osal_printk("[Follow me] %s, %d enter into max mode\n", __FUNCTION__, __LINE__);
        coef0 = 511;
        coef1 = 511;
        coef2 = 511;
        coef3 = 511;
        coef4 = 511;
        coef5 = 511;
        coef6 = 511;
        coef7 = 511;
    }

    //drv transfer
    HAL_V0_HFIR_SetCkGtEn( ck_gt_en);
    HAL_V0_HFIR_SetMidEn( mid_en);
    HAL_V0_HFIR_SetHfirMode( hfir_mode);
    HAL_V0_HFIR_SetCoef0(coef0);
    HAL_V0_HFIR_SetCoef1(coef1);
    HAL_V0_HFIR_SetCoef2(coef2);
    HAL_V0_HFIR_SetCoef3(coef3);
    HAL_V0_HFIR_SetCoef4(coef4);
    HAL_V0_HFIR_SetCoef5(coef5);
    HAL_V0_HFIR_SetCoef6(coef6);
    HAL_V0_HFIR_SetCoef7(coef7);

}

HI_VOID VDP_FUNC_GetCvfirPqCfg(VO_ZME_DS_INFO_S * pstDsInfo, VO_ZME_MODE_E enZmeMode, VO_ZME_COMM_PQ_CFG_S * pstCommPqCfg)
{
#if 0
    HI_S64 h_phase_rand      = 0;
    HI_S64 h_phase           = 0;
    HI_S64 h_phase_rand_c    = 0;
    HI_S64 h_phase_c         = 0;
    HI_S64 v_phase_rand      = 0;
    HI_S64 v_phase           = 0;
#endif
//    HI_S64 v_phase_rand_c    = 0;
//    HI_S64 v_phase_c         = 0;
//    HI_S64 b_v_phase_rand      = 0;
//    HI_S64 b_v_phase           = 0;
//    HI_S64 b_v_phase_rand_c    = 0;
//    HI_S64 b_v_phase_c         = 0;

    HI_U32 zme_vprec = pstDsInfo->zme_vprec;
    HI_U32 zme_hprec = pstDsInfo->zme_hprec;


    if(enZmeMode == VO_ZME_TYP)
    {
        pstCommPqCfg->vluma_offset        = 0                         ; //  v_phase_rand   ;
        pstCommPqCfg->vchroma_offset      = 0                         ; //  v_phase_rand_c ;
        pstCommPqCfg->vbluma_offset       = -1*zme_vprec/2 ;
        pstCommPqCfg->vbchroma_offset     = -1*zme_vprec/2 ;
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
    else if(enZmeMode == VO_ZME_MAX)
    {
        pstCommPqCfg->vluma_offset        =  MAX_OFFSET*zme_vprec ;
        pstCommPqCfg->vchroma_offset      =  MAX_OFFSET*zme_vprec ;
        pstCommPqCfg->vbluma_offset       =  MAX_OFFSET*zme_vprec ;
        pstCommPqCfg->vbchroma_offset     =  MAX_OFFSET*zme_vprec ;
        pstCommPqCfg->vl_flatdect_mode    =  1  ;
        pstCommPqCfg->vl_coringadj_en     =  1  ;
        pstCommPqCfg->vl_gain             =  63 ;
        pstCommPqCfg->vl_coring           =  255;
        pstCommPqCfg->vc_flatdect_mode    =  1  ;
        pstCommPqCfg->vc_coringadj_en     =  1  ;
        pstCommPqCfg->vc_gain             =  63 ;
        pstCommPqCfg->vc_coring           =  255;
        pstCommPqCfg->lhfir_offset        =  MAX_OFFSET*zme_hprec;
        pstCommPqCfg->chfir_offset        =  MAX_OFFSET*zme_hprec;
        pstCommPqCfg->hl_flatdect_mode    =  1    ;
        pstCommPqCfg->hl_coringadj_en     =  1    ;
        pstCommPqCfg->hl_gain             =  63   ;
        pstCommPqCfg->hl_coring           =  255  ;
        pstCommPqCfg->hc_flatdect_mode    =  1    ;
        pstCommPqCfg->hc_coringadj_en     =  1    ;
        pstCommPqCfg->hc_gain             =  63   ;
        pstCommPqCfg->hc_coring           =  255  ;
    }
    else if(enZmeMode == VO_ZME_MIN)
    {
        pstCommPqCfg->vluma_offset        =  MIN_OFFSET*zme_vprec ;
        pstCommPqCfg->vchroma_offset      =  MIN_OFFSET*zme_vprec ;
        pstCommPqCfg->vbluma_offset       =  MIN_OFFSET*zme_vprec ;
        pstCommPqCfg->vbchroma_offset     =  MIN_OFFSET*zme_vprec ;
        pstCommPqCfg->vl_flatdect_mode    =  0;
        pstCommPqCfg->vl_coringadj_en     =  0;
        pstCommPqCfg->vl_gain             =  0;
        pstCommPqCfg->vl_coring           =  0;
        pstCommPqCfg->vc_flatdect_mode    =  0;
        pstCommPqCfg->vc_coringadj_en     =  0;
        pstCommPqCfg->vc_gain             =  0;
        pstCommPqCfg->vc_coring           =  0;
        pstCommPqCfg->lhfir_offset        =  MIN_OFFSET*zme_hprec;
        pstCommPqCfg->chfir_offset        =  MIN_OFFSET*zme_hprec;
        pstCommPqCfg->hl_flatdect_mode    =  0  ;
        pstCommPqCfg->hl_coringadj_en     =  0  ;
        pstCommPqCfg->hl_gain             =  0  ;
        pstCommPqCfg->hl_coring           =  0  ;
        pstCommPqCfg->hc_flatdect_mode    =  0  ;
        pstCommPqCfg->hc_coringadj_en     =  0  ;
        pstCommPqCfg->hc_gain             =  0  ;
        pstCommPqCfg->hc_coring           =  0  ;
    }
    else if(enZmeMode == VO_ZME_RAND)
    {
        /*
        v_phase_c      = sGetRandEx(-1,1);
        v_phase_rand_c = v_phase_c*16*zme_vprec/32;
        //VDP_PRINT("v_phase_c      = %d \n",v_phase_c);
       // VDP_PRINT("v_phase_rand_c = %d \n",v_phase_rand_c);

        b_v_phase_c      = sGetRandEx(-1,1);
        b_v_phase_rand_c = b_v_phase_c*16*zme_vprec/32;
        //VDP_PRINT("b_v_phase_c      = %d \n",b_v_phase_c);
        //VDP_PRINT("b_v_phase_rand_c = %d \n",b_v_phase_rand_c);


        pstCommPqCfg->vluma_offset        =  0;//v_phase_rand ;
        pstCommPqCfg->vchroma_offset      =  v_phase_rand_c ;
        pstCommPqCfg->vbluma_offset       =  0;//b_v_phase_rand ;
        pstCommPqCfg->vbchroma_offset     =  b_v_phase_rand_c ;
        pstCommPqCfg->vl_flatdect_mode    =  sGetRandEx( 0  , 1      ) ;
        pstCommPqCfg->vl_coringadj_en     =  sGetRandEx( 0  , 1      ) ;
        pstCommPqCfg->vl_gain             =  sGetRandEx( 0  , 63     ) ;
        pstCommPqCfg->vl_coring           =  sGetRandEx( 0  , 255    ) ;
        pstCommPqCfg->vc_flatdect_mode    =  sGetRandEx( 0  , 1      ) ;
        pstCommPqCfg->vc_coringadj_en     =  sGetRandEx( 0  , 1      ) ;
        pstCommPqCfg->vc_gain             =  sGetRandEx( 0  , 63     ) ;
        pstCommPqCfg->vc_coring           =  sGetRandEx( 0  , 255    ) ;
        pstCommPqCfg->lhfir_offset        =  0;//h_phase_rand;
        pstCommPqCfg->chfir_offset        =  0;//h_phase_rand_c;
        pstCommPqCfg->hl_flatdect_mode    =  sGetRandEx( 0  , 1      )  ;
        pstCommPqCfg->hl_coringadj_en     =  sGetRandEx( 0  , 1      )  ;
        pstCommPqCfg->hl_gain             =  sGetRandEx( 0  , 63     )  ;
        pstCommPqCfg->hl_coring           =  sGetRandEx( 0  , 255    )  ;
        pstCommPqCfg->hc_flatdect_mode    =  sGetRandEx( 0  , 1      )  ;
        pstCommPqCfg->hc_coringadj_en     =  sGetRandEx( 0  , 1      )  ;
        pstCommPqCfg->hc_gain             =  sGetRandEx( 0  , 63     )  ;
        pstCommPqCfg->hc_coring           =  sGetRandEx( 0  , 255    )  ;
        //*/
    }
    else
    {
//         MyAssert(0);
    }

}

HI_VOID VDP_FUNC_SetV1CvfirMode(HI_U32 enLayer , VO_ZME_MODE_E enZmeMode, VDP_V1_CVFIR_CFG_S * pstCfg)
{
    //filed declare

    HI_U32 vzme_ck_gt_en     = 0;
    HI_U32 out_pro           = 0;
    HI_U32 out_fmt           = 0;
    HI_U32 out_height        = 0;
//    HI_U32 lvfir_en          = 0;
    HI_U32 cvfir_en          = 0;
//    HI_U32 lvmid_en          = 0;
    HI_U32 cvmid_en          = 0;
//  HI_U32 vfir_1tap_en      = 0;
//    HI_U32 lvfir_mode        = 0;
    HI_U32 cvfir_mode        = 0;
//  HI_U32 graphdet_en       = 0;
    HI_U32 vratio            = 0;
//    HI_U32 vluma_offset      = 0;
    HI_U32 vchroma_offset    = 0;
//    HI_U32 vbluma_offset     = 0;
    HI_U32 vbchroma_offset   = 0;
    VO_ZME_DS_INFO_S        stDsInfo    = {0};
    VO_ZME_COMM_PQ_CFG_S    stCommPqCfg = {0};

    stDsInfo.zme_vprec       = ZME_VPREC  ;
    stDsInfo.zme_hprec       = ZME_HPREC  ;

    //hfir_order               = pstCfg->hfir_order       ;
    vzme_ck_gt_en            = pstCfg->ck_gt_en         ;
    //lvfir_en                 = pstCfg->lvfir_en         ;
    cvfir_en                 = pstCfg->cvfir_en         ;
    //lvfir_mode               = pstCfg->lvfir_mode       ;
    cvfir_mode               = pstCfg->cvfir_mode       ;
    //lvmid_en                 = pstCfg->lvmid_en         ;
    cvmid_en                 = pstCfg->cvmid_en         ;

    out_pro                  = pstCfg->out_pro          ;
    out_fmt                  = pstCfg->out_fmt          ;
    out_height               = pstCfg->in_height-1      ;
    vratio                   = (pstCfg->in_height * stDsInfo.zme_vprec)/pstCfg->in_height;



    VDP_FUNC_GetCvfirPqCfg(&stDsInfo, enZmeMode, &stCommPqCfg);

    //vluma_offset             =  stCommPqCfg.vluma_offset    ; //  v_phase_rand   ;
    vchroma_offset           =  stCommPqCfg.vchroma_offset  ; //  v_phase_rand_c ;
    //vbluma_offset            =  stCommPqCfg.vbluma_offset   ;
    vbchroma_offset          =  stCommPqCfg.vbchroma_offset ;

    //drv transfer
    HAL_V1_CVFIR_SetVzmeCkGtEn( vzme_ck_gt_en);
    HAL_V1_CVFIR_SetOutPro( out_pro);
    HAL_V1_CVFIR_SetOutFmt( out_fmt);
    HAL_V1_CVFIR_SetOutHeight( out_height);
    HAL_V1_CVFIR_SetCvfirEn( cvfir_en);
    HAL_V1_CVFIR_SetCvmidEn( cvmid_en);
    HAL_V1_CVFIR_SetCvfirMode( cvfir_mode);
    HAL_V1_CVFIR_SetVratio( vratio);
    HAL_V1_CVFIR_SetVchromaOffset( vchroma_offset);
    HAL_V1_CVFIR_SetVbchromaOffset( vbchroma_offset);

}

HI_VOID VDP_FUNC_SetV1HfirMode(HI_U32 enLayer , VDP_V1_HFIR_MODE_E V1HfirMode, VDP_V1_HFIR_CFG_S * pstCfg)
{
    //filed declare
    HI_U32 ck_gt_en    =  0;
    HI_U32 mid_en    =  0;
    HI_U32 hfir_mode    =  0;

    HI_S32 coef0=0  ;
    HI_S32 coef1=0  ;
    HI_S32 coef2=0  ;
    HI_S32 coef3=0  ;
    HI_S32 coef4=0  ;
    HI_S32 coef5=0  ;
    HI_S32 coef6=0  ;
    HI_S32 coef7=0  ;

    ck_gt_en = pstCfg->ck_gt_en;
    mid_en  = pstCfg->mid_en;
    hfir_mode = pstCfg->hfir_mode;


    if(V1HfirMode == VDP_V1_HFIR_TYP)
    {
        //osal_printk("VDP_FUNC_SetV1HfirMode enter into typ mode\n");
        //typ mode
       // ck_gt_en  =      0 ;
        coef0 = -11;
        coef1 = 15;
        coef2 = -20;
        coef3 = 28;
        coef4 = -40;
        coef5 = 61;
        coef6 = -107;
        coef7 = 330;

    }
    else if(V1HfirMode == VDP_V1_HFIR_RAND)
    {
        /*
        osal_printk("VDP_FUNC_SetV1HfirMode enter into rand mode\n");
        //rand mode
      //  ck_gt_en  =   sGetRandEx( 0  , 1 )    ;
        coef0 = sGetRandEx(-512,511);
        coef1 = sGetRandEx(-512,511);
        coef2 = sGetRandEx(-512,511);
        coef3 = sGetRandEx(-512,511);
        coef4 = sGetRandEx(-512,511);
        coef5 = sGetRandEx(-512,511);
        coef6 = sGetRandEx(-512,511);
        coef7 = sGetRandEx(-512,511);
        //*/
    }
    else if(V1HfirMode == VDP_V1_HFIR_MIN)
    {
        //osal_printk("VDP_FUNC_SetV1HfirMode enter into min mode\n");
        //min mode
       // ck_gt_en  =      0 ;
        coef0 = -512;
        coef1 = -512;
        coef2 = -512;
        coef3 = -512;
        coef4 = -512;
        coef5 = -512;
        coef6 = -512;
        coef7 = -512;
    }
    else if(V1HfirMode == VDP_V1_HFIR_MAX)
    {
        //osal_printk("VDP_FUNC_SetV1HfirMode enter into max mode\n");
        //max mode
       // ck_gt_en  =      1 ;
        coef0 = 511;
        coef1 = 511;
        coef2 = 511;
        coef3 = 511;
        coef4 = 511;
        coef5 = 511;
        coef6 = 511;
        coef7 = 511;
    }

    //drv transfer
    HAL_V1_HFIR_SetCkGtEn( ck_gt_en);
    HAL_V1_HFIR_SetMidEn( mid_en);
    HAL_V1_HFIR_SetHfirMode( hfir_mode);
    HAL_V1_HFIR_SetCoef0(coef0);
    HAL_V1_HFIR_SetCoef1(coef1);
    HAL_V1_HFIR_SetCoef2(coef2);
    HAL_V1_HFIR_SetCoef3(coef3);
    HAL_V1_HFIR_SetCoef4(coef4);
    HAL_V1_HFIR_SetCoef5(coef5);
    HAL_V1_HFIR_SetCoef6(coef6);
    HAL_V1_HFIR_SetCoef7(coef7);


}

HI_VOID VO_VID_SetZmeEnable(HI_U32 enLayer, VDP_VID_IP_CFG_S* pst_vid_cfg)
{
    VO_V0_ZME_CFG_S            stZmeCfg    ;
    VDP_V1_CVFIR_CFG_S          stV1CvfirCfg;
    VO_V0_HFIR_CFG_S           stV0HfirCfg ;
    VDP_V1_HFIR_CFG_S           stV1HfirCfg ;
//    VO_CSC_V0_CFG_S            stYuv2RgbCfg;
//    VO_HIHDR_V_CFG_S           stVHihdrCfg;
//    VDP_CSC_V1_CFG_S            stV1CscCfg ;

//     stZmeCfg     = pst_vid_cfg->stZmeCfg    ;
//     stHfirCfg    = pst_vid_cfg->stHfirCfg   ;
//     stYuv2RgbCfg = pst_vid_cfg->stYuv2RgbCfg;
//     stVHihdrCfg  = pst_vid_cfg->stVHihdrCfg ;

    //HI_ASSERT( 0 != pst_vid_cfg->vid_iw );
    //HI_ASSERT( 0 != pst_vid_cfg->vid_ih );
    //HI_ASSERT( 0 != pst_vid_cfg->vid_ow );
    //HI_ASSERT( 0 != pst_vid_cfg->vid_oh );

    if(enLayer == HAL_DISP_LAYER_VHD0)
    {

        stZmeCfg.ck_gt_en     = 0/*rand()%2*/;
        stZmeCfg.in_height    = pst_vid_cfg->vid_ih;
        stZmeCfg.out_height   = pst_vid_cfg->vid_oh;
        stZmeCfg.out_fmt      = VDP_PROC_FMT_SP_422;
        stZmeCfg.out_pro      = 1;
        //stZmeCfg.lvfir_en     = 1;
        //stZmeCfg.cvfir_en     = 1;
        stZmeCfg.lvfir_mode   = 1;
        stZmeCfg.cvfir_mode   = 1;
        stZmeCfg.lvmid_en     = 0/*rand()%2*/;
        stZmeCfg.cvmid_en     = 0/*rand()%2*/;
        stZmeCfg.vfir_1tap_en = 0;
        stZmeCfg.vl_shootctrl_en   = 0/*rand()%2*/;
        stZmeCfg.vl_shootctrl_mode = 0/*rand()%2*/;
        stZmeCfg.vc_shootctrl_en   = 0/*rand()%2*/;
        stZmeCfg.vc_shootctrl_mode = 0/*rand()%2*/;

        stZmeCfg.in_width     = pst_vid_cfg->vid_iw;
        stZmeCfg.out_width    = pst_vid_cfg->vid_ow;
        //stZmeCfg.lhfir_en     = 1;
        //stZmeCfg.chfir_en     = 1;
        stZmeCfg.lhfir_mode   = 1;
        stZmeCfg.chfir_mode   = 1;
        stZmeCfg.lhmid_en     = 0/*rand()%2*/;
        stZmeCfg.chmid_en     = 0/*rand()%2*/;
        stZmeCfg.non_lnr_en   = 0;
        stZmeCfg.hl_shootctrl_en   = 0/*rand()%2*/;
        stZmeCfg.hl_shootctrl_mode = 0/*rand()%2*/;
        stZmeCfg.hc_shootctrl_en   = 0/*rand()%2*/;
        stZmeCfg.hc_shootctrl_mode = 0/*rand()%2*/;

        stZmeCfg.hfir_order   = 0/*rand()%2*/;


        if(pst_vid_cfg->u32ZmeEn == 1)
        {
            stZmeCfg.lhfir_en     = 1;
            stZmeCfg.chfir_en     = 1;
            stZmeCfg.lvfir_en     = 1;
            stZmeCfg.cvfir_en     = 1;
            //VO_FUNC_SetV0ZmeMode(enLayer, VO_ZME_TYP, &stZmeCfg);

            //vSetV0ZmeCoef(VO_RM_COEF_MODE_TYP);
            //HAL_PARA_SetParaUpVhdChn    (VO_COEF_BUF_V0_HZME);
            //HAL_PARA_SetParaUpVhdChn    (VO_COEF_BUF_V0_VZME);

            stZmeCfg.lhfir_mode   = 0;
            stZmeCfg.chfir_mode   = 0;
            stZmeCfg.lvfir_mode   = 0;
            stZmeCfg.cvfir_mode   = 0;

            VO_FUNC_SetV0ZmeMode(enLayer, VO_ZME_TYP, &stZmeCfg);
        }
        else
        {
            stZmeCfg.lvfir_en     = 1;
            stZmeCfg.cvfir_en     = 1;
            stZmeCfg.lhfir_en     = 1;
            stZmeCfg.chfir_en     = 1;

            stZmeCfg.lhfir_mode   = 0;
            stZmeCfg.chfir_mode   = 0;
            stZmeCfg.lvfir_mode   = 0;
            stZmeCfg.cvfir_mode   = 0;

            VO_FUNC_SetV0ZmeMode(enLayer, VO_ZME_TYP, &stZmeCfg);

        }

        if(pst_vid_cfg->u32HfirEn == 1)
        {
            stV0HfirCfg.ck_gt_en   = 0/*rand()%2*/;
            stV0HfirCfg.mid_en     = 0/*sGetRandEx(0,1)*/;
            stV0HfirCfg.hfir_mode  = XDP_V0_HFIR_FILT;

            VDP_FUNC_SetV0HfirMode(enLayer, VDP_V0_HFIR_TYP, &stV0HfirCfg);
        }
    }
    else if(enLayer == HAL_DISP_LAYER_VHD1)
    {
        stV1CvfirCfg.hfir_order   =    0;
        stV1CvfirCfg.lhfir_en     =    0;
        stV1CvfirCfg.chfir_en     =    0;
        stV1CvfirCfg.lhmid_en     =    0;
        stV1CvfirCfg.chmid_en     =    0;
        stV1CvfirCfg.lhfir_mode   =    0;
        stV1CvfirCfg.chfir_mode   =    0;
        stV1CvfirCfg.hl_shootctrl_en   =    0;
        stV1CvfirCfg.hl_shootctrl_mode =    0;
        stV1CvfirCfg.hc_shootctrl_en   =    0;
        stV1CvfirCfg.hc_shootctrl_mode =    0;
        stV1CvfirCfg.lvfir_en     =    0;
        stV1CvfirCfg.lvmid_en     =    0;
        stV1CvfirCfg.lvfir_mode   =    0;
        stV1CvfirCfg.vl_shootctrl_en   =    0;
        stV1CvfirCfg.vl_shootctrl_mode =    0;
        stV1CvfirCfg.vc_shootctrl_en   =    0;
        stV1CvfirCfg.vc_shootctrl_mode =    0;

        //v1 CVFIR
        stV1CvfirCfg.ck_gt_en     =    0/*rand()%2*/;
        stV1CvfirCfg.cvfir_en     =    1;
        stV1CvfirCfg.cvmid_en     =    0/*rand()%2*/;
        stV1CvfirCfg.cvfir_mode   =    0/*rand()%2*/;
        stV1CvfirCfg.out_pro      =    VDP_RMODE_PROGRESSIVE;
        stV1CvfirCfg.out_fmt      =    VDP_PROC_FMT_SP_422;
        stV1CvfirCfg.in_width     =    pst_vid_cfg->vid_iw;
        stV1CvfirCfg.in_height    =    pst_vid_cfg->vid_ih;
        stV1CvfirCfg.out_width    =    pst_vid_cfg->vid_ow;
        stV1CvfirCfg.out_height   =    pst_vid_cfg->vid_oh;
        VDP_FUNC_SetV1CvfirMode(enLayer, VO_ZME_TYP, &stV1CvfirCfg);
        //vSetV1CvfirCoef(VO_RM_COEF_MODE_TYP);

        if(pst_vid_cfg->u32HfirEn == 1)
        {
            stV1HfirCfg.ck_gt_en   = 0/*rand()%2*/;
            stV1HfirCfg.mid_en     = 0/*sGetRandEx(0,1)*/;
            stV1HfirCfg.hfir_mode  = XDP_HFIR_FILT;

            VDP_FUNC_SetV1HfirMode(enLayer, VDP_V1_HFIR_TYP, &stV1HfirCfg);
        }

        //stV1CscCfg.csc_en = 1;
        //stV1CscCfg.csc_ck_gt_en = 1/*rand()%2*/;
        //VO_FUNC_SetCscV1Mode(VDP_LAYER_VID1, VDP_CSC_YUV2YUV, &stV1CscCfg);
    }

}


HI_VOID GF_FUNC_SetG0ZmeMode(HI_U32 enLayer , GF_G0_ZME_MODE_E G0ZmeMode, GF_G0_ZME_CFG_S * pstCfg)
{
    //filed declare
    HI_U32 ck_gt_en      =  pstCfg->ck_gt_en;

    HI_U32 out_pro       =  pstCfg->out_pro;
    HI_U32 hfir_order    =  1;

    HI_U32 hfir_en          =  pstCfg->hfir_en   ;
    HI_U32 vfir_en          =  pstCfg->vfir_en   ;
//    HI_U32 in_width         =  pstCfg->in_width  ;
//    HI_U32 in_height        =  pstCfg->in_height ;
    HI_U32 out_width        =  pstCfg->out_width ;
    HI_U32 out_height       =  pstCfg->out_height;

    HI_U32 ahfir_mid_en     =  pstCfg->ahmid_en  ;
    HI_U32 lhfir_mid_en     =  pstCfg->lhmid_en  ;
    HI_U32 chfir_mid_en     =  pstCfg->lhmid_en  ;
    HI_U32 lhfir_mode       =  pstCfg->lhfir_mode;
    HI_U32 ahfir_mode       =  pstCfg->ahfir_mode;
    HI_S32 lhfir_offset     =  0;
    HI_S32 chfir_offset     =  0;
    HI_U32 avfir_mid_en     =  pstCfg->avmid_en  ;
    HI_U32 lvfir_mid_en     =  pstCfg->lvmid_en  ;
    HI_U32 cvfir_mid_en     =  pstCfg->lvmid_en  ;
    HI_U32 lvfir_mode       =  pstCfg->lvfir_mode;
    HI_U32 avfir_mode       =  pstCfg->avfir_mode;
    HI_S32 vtp_offset       =  0;
    HI_S32 vbtm_offset      =  0;

    HI_U64 zme_hprec        =  (1<<20);
    HI_U64 zme_vprec        =  (1<<12);

    HI_U32 hratio           =  (pstCfg->in_width *zme_hprec)/pstCfg->out_width;
    HI_U32 vratio           =  (pstCfg->in_height*zme_vprec)/pstCfg->out_height;

//    HI_S32 h_phase_rand      = 0;
//    HI_S32 v_phase_rand      = 0;
//    HI_S32 v_phase_rand_b    = 0;

    if(G0ZmeMode == VDP_G0_ZME_TYP)
    {
        //typ mode
        lhfir_offset = 0;
        chfir_offset = 0;
        vtp_offset   = 0;
        vbtm_offset  = (-1) * (HI_S64)zme_vprec/2;
    }
    else if(G0ZmeMode == VDP_G0_ZME_TYP1)
    {
        //typ1 mode
        lhfir_offset = (-1) * (HI_S64)zme_hprec/2;
        chfir_offset = (-1) * (HI_S64)zme_hprec/2;
        vtp_offset   =  0;
        vbtm_offset  = (-1) * (HI_S64)zme_vprec/2;
    }
    else if(G0ZmeMode == VDP_G0_ZME_RAND)
    {
        // Support this mode until sGetRandEx() is supported.
        /*
        //*/
    }
    else if(G0ZmeMode == VDP_G0_ZME_MIN)
    {

        lhfir_offset = (-2) * (HI_S64)zme_hprec;
        chfir_offset = (-2) * (HI_S64)zme_hprec;
        vtp_offset   = (-2) * (HI_S64)zme_vprec;
        vbtm_offset  = (-2) * (HI_S64)zme_vprec;
        //min mode
    }
    else if(G0ZmeMode == VDP_G0_ZME_MAX)
    {

        lhfir_offset = 3 * zme_hprec;
        chfir_offset = 3 * zme_hprec;
        vtp_offset   = 3 * zme_vprec;
        vbtm_offset  = 3 * zme_vprec;
        //max mode
    }

    //drv transfer
    HAL_G0_ZME_SetCkGtEn( ck_gt_en);
    HAL_G0_ZME_SetOutWidth( out_width);
    HAL_G0_ZME_SetHfirEn( hfir_en);
    HAL_G0_ZME_SetAhfirMidEn( ahfir_mid_en);
    HAL_G0_ZME_SetLhfirMidEn( lhfir_mid_en);
    HAL_G0_ZME_SetChfirMidEn( chfir_mid_en);
    HAL_G0_ZME_SetLhfirMode( lhfir_mode);
    HAL_G0_ZME_SetAhfirMode( ahfir_mode);
    HAL_G0_ZME_SetHfirOrder( hfir_order);
    HAL_G0_ZME_SetHratio( hratio);
    HAL_G0_ZME_SetLhfirOffset( lhfir_offset);
    HAL_G0_ZME_SetChfirOffset( chfir_offset);
    HAL_G0_ZME_SetOutPro( out_pro);
    HAL_G0_ZME_SetOutHeight( out_height);
    HAL_G0_ZME_SetVfirEn( vfir_en);
    HAL_G0_ZME_SetAvfirMidEn( avfir_mid_en);
    HAL_G0_ZME_SetLvfirMidEn( lvfir_mid_en);
    HAL_G0_ZME_SetCvfirMidEn( cvfir_mid_en);
    HAL_G0_ZME_SetLvfirMode( lvfir_mode);
    HAL_G0_ZME_SetVafirMode( avfir_mode);
    HAL_G0_ZME_SetVratio( vratio);
    HAL_G0_ZME_SetVtpOffset( vtp_offset);
    HAL_G0_ZME_SetVbtmOffset( vbtm_offset);

}

HI_BOOL GRAPHIC_DRV_EnableZME(HI_U32 enLayer, GF_G0_ZME_CFG_S* pst_zme_cfg, HI_BOOL bEnableZme)
{
    GF_G0_ZME_CFG_S            stZmeCfg    ;

    if(( 0 == pst_zme_cfg->in_width)  ||
       ( 0 == pst_zme_cfg->in_height) ||
       ( 0 == pst_zme_cfg->out_width) ||
       ( 0 == pst_zme_cfg->out_height)
    )
    {
        printf("%s,%d,error: args invalid\n",__FUNCTION__,__LINE__);
        return HI_FAILURE;
    }

    stZmeCfg.ck_gt_en         =  0/*rand()%2*/;
    stZmeCfg.out_pro          =  VDP_RMODE_PROGRESSIVE;

    stZmeCfg.in_width         =  pst_zme_cfg->in_width;
    stZmeCfg.in_height        =  pst_zme_cfg->in_height;
    stZmeCfg.out_width        =  pst_zme_cfg->out_width;
    stZmeCfg.out_height       =  pst_zme_cfg->out_height;
    stZmeCfg.lhmid_en         =  1;
    stZmeCfg.ahmid_en         =  1;
    stZmeCfg.lhfir_mode       =  1;
    stZmeCfg.ahfir_mode       =  1;
    stZmeCfg.lvmid_en         =  1;
    stZmeCfg.avmid_en         =  1;
    stZmeCfg.lvfir_mode       =  1;
    stZmeCfg.avfir_mode       =  1;


    if(bEnableZme)
    {
        stZmeCfg.hfir_en          =  1;
        stZmeCfg.vfir_en          =  1;
        // use cpy mode
        stZmeCfg.lhfir_mode       =  0;
        stZmeCfg.ahfir_mode       =  0;

        // use filter mode , not supported now.
        // stZmeCfg.lhfir_mode       =  1;
        // stZmeCfg.ahfir_mode       =  1;


        GF_FUNC_SetG0ZmeMode(HAL_DISP_LAYER_GFX0, VDP_G0_ZME_TYP, &stZmeCfg);

        //vSetG0zmeCoef(VDP_RM_COEF_MODE_TYP);
        //VDP_PARA_SetParaUpVhdChn    (VDP_COEF_BUF_G0ZME);
    }
    else
    {
        stZmeCfg.hfir_en     = 0;
        stZmeCfg.vfir_en     = 0;

        GF_FUNC_SetG0ZmeMode(HAL_DISP_LAYER_GFX0, VDP_G0_ZME_TYP, &stZmeCfg);
    }

    return HI_TRUE;
}




#ifdef __cplusplus
#if __cplusplus
 }
#endif
#endif /* End of #ifdef __cplusplus */

