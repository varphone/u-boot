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

#ifndef __HI_MIPI_TX__
#define __HI_MIPI_TX__

#define CMD_MAX_NUM    4
#define LANE_MAX_NUM   4

#define HI_ERR(x...) \
    do { \
        printf("%s(%d): ", __FUNCTION__, __LINE__); \
        printf(x); \
    } while (0)

#define MIPITX_CHECK_NULL_PTR(ptr)\
    do{\
        if(NULL == ptr)\
        {\
            HI_ERR("NULL point \r\n");\
            return -1;\
        }\
    } while(0)

typedef enum
{
    OUTPUT_MODE_CSI            = 0x0,              /* csi mode */
    OUTPUT_MODE_DSI_VIDEO      = 0x1,              /* dsi video mode */
    OUTPUT_MODE_DSI_CMD        = 0x2,              /* dsi command mode */

    OUTPUT_MODE_BUTT
} output_mode_t;

typedef enum
{
    BRUST_MODE                      = 0x0,
    NON_BRUST_MODE_SYNC_PULSES      = 0x1,
    NON_BRUST_MODE_SYNC_EVENTS      = 0x2,
} video_mode_t;

typedef enum
{
    OUT_FORMAT_RGB_16_BIT          = 0x0,
    OUT_FORMAT_RGB_18_BIT          = 0x1,
    OUT_FORMAT_RGB_24_BIT          = 0x2,
    OUT_FORMAT_YUV420_8_BIT_NORMAL = 0x3,
    OUT_FORMAT_YUV420_8_BIT_LEGACY = 0x4,
    OUT_FORMAT_YUV422_8_BIT        = 0x5,

    OUT_FORMAT_BUTT
} output_format_t;

typedef struct
{
    unsigned short  vid_pkt_size;
    unsigned short  vid_hsa_pixels;
    unsigned short  vid_hbp_pixels;
    unsigned short  vid_hline_pixels;
    unsigned short  vid_vsa_lines;
    unsigned short  vid_vbp_lines;
    unsigned short  vid_vfp_lines;
    unsigned short  vid_active_lines;
    unsigned short  edpi_cmd_size;
} sync_info_t;

typedef struct
{
    unsigned int        devno;                   /* device number*/
    short               lane_id[LANE_MAX_NUM];   /* lane_id: -1 - disable */
    output_mode_t       output_mode;             /* output mode: CSI/DSI_VIDEO/DSI_CMD */
    video_mode_t        video_mode;
    output_format_t     output_format;
    sync_info_t         sync_info;
    unsigned int        phy_data_rate;           /*mbps*/
    unsigned int        pixel_clk;               /*KHz*/
} combo_dev_cfg_t;

typedef struct
{
    unsigned int        devno;                   /* device number */
    unsigned short      data_type;
    unsigned short      cmd_size;
    unsigned char       *cmd;
} cmd_info_t;


typedef enum hiMIPI_TX_VO_INTF_SYNC_E
{
    VO_OUTPUT_PAL = 0,               /* PAL standard*/
    VO_OUTPUT_NTSC,                  /* NTSC standard */

    VO_OUTPUT_1080P24,               /* 1920 x 1080 at 24 Hz. */
    VO_OUTPUT_1080P25,               /* 1920 x 1080 at 25 Hz. */
    VO_OUTPUT_1080P30,               /* 1920 x 1080 at 30 Hz. */

    VO_OUTPUT_720P50,                /* 1280 x  720 at 50 Hz. */
    VO_OUTPUT_720P60,                /* 1280 x  720 at 60 Hz. */
    VO_OUTPUT_1080I50,               /* 1920 x 1080 at 50 Hz, interlace. */
    VO_OUTPUT_1080I60,               /* 1920 x 1080 at 60 Hz, interlace. */
    VO_OUTPUT_1080P50,               /* 1920 x 1080 at 50 Hz. */
    VO_OUTPUT_1080P60,               /* 1920 x 1080 at 60 Hz. */

    VO_OUTPUT_576P50,                /* 720  x  576 at 50 Hz. */
    VO_OUTPUT_480P60,                /* 720  x  480 at 60 Hz. */

    VO_OUTPUT_800x600_60,            /* VESA 800 x 600 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1024x768_60,           /* VESA 1024 x 768 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1280x1024_60,          /* VESA 1280 x 1024 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1366x768_60,           /* VESA 1366 x 768 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1440x900_60,           /* VESA 1440 x 900 at 60 Hz (non-interlaced) CVT Compliant */
    VO_OUTPUT_1280x800_60,           /* 1280*800@60Hz VGA@60Hz*/
    VO_OUTPUT_1600x1200_60,          /* VESA 1600 x 1200 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1680x1050_60,          /* VESA 1680 x 1050 at 60 Hz (non-interlaced) */
    VO_OUTPUT_1920x1200_60,          /* VESA 1920 x 1600 at 60 Hz (non-interlaced) CVT (Reduced Blanking)*/
    VO_OUTPUT_640x480_60,            /* VESA 640 x 480 at 60 Hz (non-interlaced) CVT */
    VO_OUTPUT_960H_PAL,              /* ITU-R BT.1302 960 x 576 at 50 Hz (interlaced)*/
    VO_OUTPUT_960H_NTSC,             /* ITU-R BT.1302 960 x 480 at 60 Hz (interlaced)*/
    VO_OUTPUT_1920x2160_30,          /* 1920x2160_30 */
    VO_OUTPUT_2560x1440_30,          /* 2560x1440_30 */
    VO_OUTPUT_2560x1600_60,          /* 2560x1600_60 */
    VO_OUTPUT_3840x2160_24,          /* 3840x2160_24 */
    VO_OUTPUT_3840x2160_25,          /* 3840x2160_25 */
    VO_OUTPUT_3840x2160_30,          /* 3840x2160_30 */
    VO_OUTPUT_3840x2160_50,          /* 3840x2160_50 */
    VO_OUTPUT_3840x2160_60,          /* 3840x2160_60 */
    VO_OUTPUT_320x240_60,            /* For ota5182 at 60 Hz (8bit)  */
    VO_OUTPUT_320x240_50,            /* For ili9342 at 50 Hz (6bit)  */
    VO_OUTPUT_240x320_50,            /* For ili9341 at 50 Hz (6bit)  */
    VO_OUTPUT_240x320_60,            /* For ili9341 at 60 Hz (16bit) */
    VO_OUTPUT_800x600_50,            /* For LCD     at 50 Hz (24bit) */
    VO_OUTPUT_720x1280_60,           /* For MIPI DSI Tx 720 x1280 at 60 Hz */
    VO_OUTPUT_1080x1920_60,          /* For MIPI DSI Tx 1080x1920 at 60 Hz */
    VO_OUTPUT_7680x4320_30,          /* For HDMI2.0 at 30 Hz         */
    VO_OUTPUT_USER,                  /* User timing. */
    VO_OUTPUT_BUTT


} MIPI_TX_VO_INTF_SYNC_E;


#define HI_MIPI_TX_SET_DEV_CFG               (0x01)
#define HI_MIPI_TX_SET_CMD                   (0x02)
#define HI_MIPI_TX_ENABLE                    (0x03)

int mipi_tx_display(unsigned int vosync);
int mipi_tx_stop(void);


#endif
