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
#include <version.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
//#include <asm/sizes.h>
#include <config.h>

#include "vou_drv.h"
#include "vou_hal.h"

static HI_U32 g_au32BgColor[4] = {0x00ffff, 0xFF00, 0xFF00, 0xFF00};     /* default value is CYAN */
//static HI_BOOL g_abHdStart[2] = {0};
//static HI_U32 g_au32HdType[2] = {0};
static HI_BOOL g_bInited = HI_FALSE;
static RECT_S stMaxRect = {0};

#define VOU_ALAIN(u32Val, u32Align)    ((u32Val +u32Align -1)&(~(u32Align-1)))


static inline HI_VOID Rgb2Yc(HI_U16 r, HI_U16 g, HI_U16 b, HI_U16 * py, HI_U16 * pcb, HI_U16 * pcr)
{
    /* Y */
    *py = (HI_U16)((((r*66+g*129+b*25) >> 8) + 16) << 2);

    /* Cb */
    *pcb = (HI_U16)(((((b*112-r*38)-g*74) >> 8) + 128) << 2);

    /* Cr */
    *pcr = (HI_U16)(((((r*112-g*94)-b*18) >> 8) + 128) << 2);
}

static inline HI_U32 Rgb2Yuv(HI_U32 u32Rgb)
{
    HI_U16 r,g,b;
    HI_U16 y,u,v;

    r = RGB_RED(u32Rgb);
    g = RGB_GREEN(u32Rgb);
    b = RGB_BLUE(u32Rgb);

    Rgb2Yc(r, g, b, &y, &u, &v);

    return YUV(y,u,v);
}

static inline unsigned int GfxConvertLayer(unsigned int layer)
{
    unsigned int uGfxLayer;

    /* hard cursor is not supported. */
    switch (layer)
    {
        case 0:
            uGfxLayer = VOU_LAYER_G0;
            break;
        case 1:
            uGfxLayer = VOU_LAYER_G1;
            break;
        default :
            uGfxLayer = VOU_LAYER_G0;
            break;
    }
    return uGfxLayer;
}

static inline HAL_DISP_LAYER_E GfxConvertToHalLayer(unsigned int layer)
{
    HAL_DISP_LAYER_E enLayer;

    /* hard cursor is not supported. */
    switch (layer)
    {
        case 0:
            enLayer = HAL_DISP_LAYER_GFX0;
            break;
        case 1:
            enLayer = HAL_DISP_LAYER_GFX1;
            break;
        default :
            enLayer = HAL_DISP_LAYER_GFX0;
            break;
    }
    return enLayer;
}
static inline VOU_LAYER_E VideoLayerConvert(unsigned int layer)
{
    VOU_LAYER_E enLayer;

    /* hard cursor is not supported. */
    switch (layer)
    {
        case 0:
            enLayer = VOU_LAYER_VHD0;
            break;
        case 1:
            enLayer = VOU_LAYER_VHD1;
            break;
        default :
            enLayer = VOU_LAYER_VHD0;
            break;

    }
    return enLayer;
}

static inline HAL_DISP_LAYER_E VideoLayerConvertToHal(unsigned int layer)
{
    HAL_DISP_LAYER_E enLayer;

    /* hard cursor is not supported. */
    switch (layer)
    {
        case 0:
            enLayer = HAL_DISP_LAYER_VHD0;
            break;
        case 1:
            enLayer = HAL_DISP_LAYER_VHD1;
            break;
        default :
            enLayer = HAL_DISP_LAYER_VHD0;
            break;

    }
    return enLayer;
}


int set_vobg(unsigned int dev, unsigned int rgb)
{
	if (dev < 3)
    {
    	g_au32BgColor[dev] = rgb;
    }
	return 0;
}

HI_VOID VouSetDispMaxSize(VO_DEV VoDev, VO_INTF_SYNC_E enVoOutMode)
{
    HI_U32 u32MaxWidth = 720, u32MaxHeight = 576;

    switch (enVoOutMode)
    {
        case VO_OUTPUT_PAL :
        case VO_OUTPUT_576P50 :
            u32MaxWidth  = 720;
            u32MaxHeight = 576;
            break;
        case VO_OUTPUT_NTSC :
        case VO_OUTPUT_480P60:
            u32MaxWidth  = 720;
            u32MaxHeight = 480;
            break;
        case VO_OUTPUT_720P60 :
        case VO_OUTPUT_720P50 :
            u32MaxWidth  = 1280;
            u32MaxHeight = 720;
            break;
        case VO_OUTPUT_1080I50 :
        case VO_OUTPUT_1080I60 :
        case VO_OUTPUT_1080P24 :
        case VO_OUTPUT_1080P25 :
        case VO_OUTPUT_1080P30 :
        case VO_OUTPUT_1080P50 :
        case VO_OUTPUT_1080P60 :
            u32MaxWidth  = 1920;
            u32MaxHeight = 1080;
            break;
        case VO_OUTPUT_800x600_60 :
        case VO_OUTPUT_800x600_50:
            u32MaxWidth  = 800;
            u32MaxHeight = 600;
            break;
        case VO_OUTPUT_1024x768_60 :
            u32MaxWidth  = 1024;
            u32MaxHeight = 768;
            break;
        case VO_OUTPUT_1280x1024_60 :
            u32MaxWidth  = 1280;
            u32MaxHeight = 1024;
            break;
        case VO_OUTPUT_1366x768_60 :
            u32MaxWidth  = 1366;
            u32MaxHeight = 768;
            break;
        case VO_OUTPUT_1440x900_60 :
            u32MaxWidth  = 1440;
            u32MaxHeight = 900;
            break;
        case VO_OUTPUT_1280x800_60 :
            u32MaxWidth  = 1280;
            u32MaxHeight = 800;
            break;
        case VO_OUTPUT_1600x1200_60 :
            u32MaxWidth  = 1600;
            u32MaxHeight = 1200;
            break;
        case VO_OUTPUT_1680x1050_60 :
            u32MaxWidth  = 1680;
            u32MaxHeight = 1050;
            break;
        case VO_OUTPUT_1920x1200_60 :
            u32MaxWidth  = 1920;
            u32MaxHeight = 1200;
            break;
        case VO_OUTPUT_640x480_60 :
            u32MaxWidth  = 640;
            u32MaxHeight = 480;
            break;
    	case VO_OUTPUT_1920x2160_30:
            u32MaxWidth  = 1920;
            u32MaxHeight = 2160;
            break;
    	case VO_OUTPUT_2560x1440_30:
    	case VO_OUTPUT_2560x1440_60:
            u32MaxWidth  = 2560;
            u32MaxHeight = 1440;
            break;
    	case VO_OUTPUT_2560x1600_60:
            u32MaxWidth  = 2560;
            u32MaxHeight = 1600;
            break;
    	case VO_OUTPUT_3840x2160_24:
    	case VO_OUTPUT_3840x2160_25:
    	case VO_OUTPUT_3840x2160_30:
    	case VO_OUTPUT_3840x2160_50:
    	case VO_OUTPUT_3840x2160_60:
            u32MaxWidth  = 3840;
            u32MaxHeight = 2160;
            break;
    	case VO_OUTPUT_4096x2160_24:
    	case VO_OUTPUT_4096x2160_25:
    	case VO_OUTPUT_4096x2160_30:
    	case VO_OUTPUT_4096x2160_50:
    	case VO_OUTPUT_4096x2160_60:
            u32MaxWidth  = 4096;
            u32MaxHeight = 2160;
            break;
        case VO_OUTPUT_320x240_50:
        case VO_OUTPUT_320x240_60 :  /* just for hi3518ev200 hi3519*/
            u32MaxWidth  = 320;
            u32MaxHeight = 240;
            break;
        case VO_OUTPUT_240x320_50:  /* just for hi3518ev200 hi3519*/
        case VO_OUTPUT_240x320_60:
            u32MaxWidth  = 240;
            u32MaxHeight = 320;
            break;
        case VO_OUTPUT_720x1280_60:
            u32MaxWidth  = 720;
            u32MaxHeight = 1280;
            break;
        case VO_OUTPUT_1080x1920_60:
            u32MaxWidth  = 1080;
            u32MaxHeight = 1920;
            break;
        case VO_OUTPUT_7680x4320_30:
            u32MaxWidth  = 7680;
            u32MaxHeight = 4320;
            break;
        default:
            u32MaxWidth = 1920;
            u32MaxHeight = 1080;
    }


    stMaxRect.u32Width = u32MaxWidth;
    stMaxRect.u32Height = u32MaxHeight;

    printf("stMaxRect.u32Width:%d, stMaxRect.u32Height: %d. \n", stMaxRect.u32Width, stMaxRect.u32Height);
}


int start_vo(unsigned int dev, unsigned int type, unsigned int sync)
{
    if (g_bInited == HI_FALSE)
    {
        //SYS_HAL_DDRConifg();
        //SYS_HAL_SelVoVgaPinConifg();
        if (type & VO_INTF_BT1120)
        {
            SYS_HAL_SelVoBt1120PinConifg();
        }
        SYS_HAL_VouVDPLowPower(HI_TRUE);
        SYS_HAL_VouBusResetSel(HI_FALSE);
        SYS_HAL_VouBusClkEn(HI_TRUE);
        SYS_HAL_SelVoClk(0x0);
        //SYS_HAL_VouHdOutClkSel(HI_TRUE);
        /* open clk*/
        VO_DRV_SetAllCrgClk(HI_TRUE);
        VOU_DRV_BoardInit();
        HAL_SYS_Control();

        g_bInited = HI_TRUE;
    }

    if ((VOU_DEV_DHD0 == dev) || (VOU_DEV_DHD1 == dev))
    {
        SYS_HAL_VouDevClkEn(dev, HI_TRUE);
    }
    else
    {}

    VouSetDispMaxSize(dev, sync);
    VOU_DRV_SetDevIntfType(dev, type);
    VOU_DRV_SetDevOutSync(dev, sync);
    if (type & VO_INTF_HDMI)
    {
        VOU_DRV_SetDevBkGrd(dev, Rgb2Yuv(g_au32BgColor[dev]));
    }
    else if (type & VO_INTF_MIPI)
    {
        VOU_DRV_SetDevBkGrd(dev, g_au32BgColor[dev]);
    }
    VOU_DRV_SetDevClk(dev);
    VOU_DRV_Open(dev);

    return 0;
}

int stop_vo(unsigned int dev)
{
    VOU_DRV_Close(dev);
    SYS_HAL_VouDevClkEn(dev, HI_FALSE);

    return 0;
}

int start_gx(unsigned int layer, unsigned long addr, unsigned int strd,
    unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    RECT_S             stDispRect = {x, y, w, h};
    VOU_LAYER_E        enLayer;
    HAL_DISP_LAYER_E   enHalLayer;
    VO_INTF_TYPE_E     enVoIntfType;

    GF_G0_ZME_CFG_S    st_zme_cfg;

    enLayer       = GfxConvertLayer(layer);
    enHalLayer    = GfxConvertLayer(layer);
    enVoIntfType  = VO_DRV_GetDevIntfType(layer);

    HAL_GRAPHIC_SetGfxExt(enLayer, HAL_GFX_BITEXTEND_3RD);
    HAL_GRAPHIC_SetGfxPalpha(enLayer, HI_TRUE, HI_TRUE, 0xff, 0xff);
    HAL_LAYER_SetLayerGalpha(enLayer, 0xff);
    HAL_GRAPHIC_SetGfxPreMult(enLayer, HI_FALSE);
    // for mipi_tx, do not do this two line.
    if (VO_INTF_HDMI == enVoIntfType ||
        VO_INTF_BT1120 == enVoIntfType)
    {
        // do rgb to yuv.
        VOU_DRV_GraphicsSetCscCoef(enLayer);
        HAL_LAYER_SetCscEn(enLayer, HI_TRUE);
    }
    else if (VO_INTF_MIPI == enVoIntfType)
    {
        // do rgb to rgb or do nothing.
    }

    HAL_GRAPHIC_SetGfxAddr(enLayer, addr);
    HAL_GRAPHIC_SetGfxStride(enLayer, strd>>4);
    HAL_LAYER_SetLayerInRect(enLayer, &stDispRect);

    // G0:full screen display
    if (VOU_LAYER_G0 == enLayer)
    {
        // do zme: disprect to maxrect.
        st_zme_cfg.in_width   = stDispRect.u32Width;
        st_zme_cfg.in_height  = stDispRect.u32Height;
        st_zme_cfg.out_width  = stDispRect.u32Width;
        st_zme_cfg.out_height = stDispRect.u32Height;
        GRAPHIC_DRV_EnableZME(enLayer, &st_zme_cfg, HI_TRUE);

        //HAL_LAYER_SetLayerOutRect(enLayer, &stDispRect);
        HAL_VIDEO_SetLayerDispRect(enLayer, &stDispRect);
        HAL_VIDEO_SetLayerVideoRect(enLayer, &stDispRect);
    }else
    {
        HAL_VIDEO_SetLayerDispRect(enLayer, &stDispRect);
        HAL_VIDEO_SetLayerVideoRect(enLayer, &stDispRect);
    }

    HAL_GFX_SetSrcResolution(enLayer, &stDispRect);

    HAL_LAYER_SetLayerDataFmt(enLayer, HAL_INPUTFMT_ARGB_1555);

    HAL_LAYER_EnableLayer(enLayer, HI_TRUE);
    HAL_LAYER_SetRegUp(enHalLayer);

    return 0;
}

int stop_gx(unsigned int layer)
{
    VOU_LAYER_E enLayer;
    HAL_DISP_LAYER_E enHalLayer;

    enLayer = GfxConvertLayer(layer);
    enHalLayer = GfxConvertToHalLayer(layer);

    HAL_LAYER_SetRegUp(enLayer);
    HAL_LAYER_EnableLayer(enLayer, HI_FALSE);
    HAL_LAYER_SetRegUp(enHalLayer);

    return 0;
}

int start_videolayer(unsigned int layer, unsigned long addr, unsigned int strd,
    unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    int          i                    = 0;
    RECT_S       stDispRect           = {x, y, w, h};
    VOU_LAYER_E  enLayer;
    HAL_DISP_LAYER_E  enHalLayer;
    HI_S32       as32HfirCoef[2][8]   = {
        {0x3f9, 0xc, 0x3ef, 0x19, 0x3da, 0x3a, 0x397, 0x148},
        {0x3f5, 0xf, 0x3ec, 0x1c, 0x3d8, 0x3d, 0x395, 0x14a}
    };
    VO_INTF_TYPE_E     enVoIntfType;

    VDP_VID_IP_CFG_S     st_vid_cfg       = {0};

    enLayer = VideoLayerConvert(layer);
    enHalLayer = VideoLayerConvertToHal(layer);
    HAL_LAYER_SetLayerDataFmt(layer, VOU_LAYER_PIXERL_FORMAT_SPYCbCr_420);
    enVoIntfType  = VO_DRV_GetDevIntfType(layer);

    // for mipi_tx, do not do this two line.
    if (VO_INTF_HDMI == enVoIntfType ||
        VO_INTF_BT1120 == enVoIntfType)
    {
        // do rgb to rgb or do nothing.
    }
    else if (VO_INTF_MIPI == enVoIntfType)
    {
        // do yuv to rgb.
        VOU_DRV_VideoSetCscCoef(enLayer);
        HAL_LAYER_SetCscEn(enLayer, HI_TRUE);
    }

    for (i = 0; i <= HAL_DISP_LAYER_VHD1; i++)
    {
        HAL_VIDEO_SetHfirMode(i, HAL_HFIRMODE_COPY);
        HAL_VIDEO_SetHfirCoef(i, as32HfirCoef[i]);
    }

    HAL_VIDEO_SetLayerDispRect(layer, &stDispRect);
    HAL_VIDEO_SetLayerVideoRect(layer, &stDispRect);
    HAL_LAYER_SetLayerOutRect(layer, &stDispRect);
    HAL_LAYER_SetLayerInRect(layer, &stDispRect);
    HAL_LAYER_SetLayerGalpha(layer, 255);
    HAL_LAYER_SetSrcResolution(layer, &stDispRect);

    HAL_LAYER_SetZmeEnable(layer, HAL_DISP_ZMEMODE_ALL, HI_FALSE);
    HAL_LAYER_SetZmeMscEnable(layer, HAL_DISP_ZMEMODE_VERC, HI_TRUE);
    //HAL_LAYER_SetVerRatio(layer, 0x1000);

    st_vid_cfg.u32CscEn  =  0;
    st_vid_cfg.u32HfirEn =  1;
    st_vid_cfg.vid_iw = stDispRect.u32Width;
    st_vid_cfg.vid_ih = stDispRect.u32Height;

    st_vid_cfg.vid_ow = stDispRect.u32Width;
    st_vid_cfg.vid_oh = stDispRect.u32Height;
    st_vid_cfg.u32ZmeEn = HI_FALSE;
    VO_VID_SetZmeEnable(layer, &st_vid_cfg);

    HAL_LAYER_SetZmeInfo(layer,stDispRect.u32Width,stDispRect.u32Height,HAL_DISP_ZME_OUTFMT422);

    HAL_LAYER_SetZmeVerType(layer, 1);

    //HAL_LAYER_SetCscEn(layer, 0);

    HAL_VIDEO_SetMultiAreaRect(layer, 0, &stDispRect);
    HAL_VIDEO_SetMultiAreaReso(layer, 0, stDispRect.u32Width);
    HAL_VIDEO_SetMultiAreaLAddr(layer, 0, addr, strd);
    HAL_VIDEO_SetMultiAreaCAddr(layer, 0, addr+strd * VOU_ALAIN(h, 16), strd);
    HAL_VIDEO_SetMultiAreaEnable(layer, 0, 1);
    HAL_LAYER_EnableLayer(enLayer, HI_TRUE);
    HAL_LAYER_SetRegUp(enHalLayer);

    return 0;
}

int stop_videolayer(unsigned int layer)
{
    VOU_LAYER_E enLayer;
    HAL_DISP_LAYER_E enHalLayer;

    enLayer = VideoLayerConvert(layer);
    enHalLayer = VideoLayerConvertToHal(layer);

    HAL_VIDEO_SetMultiAreaEnable(layer, 0, 0);
    HAL_LAYER_EnableLayer(enLayer, HI_FALSE);
    HAL_LAYER_SetRegUp(enHalLayer);

    return 0;
}

