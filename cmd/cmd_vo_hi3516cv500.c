/*
 * (C) Copyright 2009-2010
 * Kevin Hsu, Hisilicon inc, xujilin@huawei.com
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Video output Support
 */
#include <common.h>
#include <command.h>
#include <hi3519av100_vo.h>

extern int set_vobg(unsigned int dev, unsigned int rgb);
extern int start_vo(unsigned int dev, unsigned int type, unsigned int sync);
extern int stop_vo(unsigned int dev);
extern int start_videolayer(unsigned int layer, unsigned long addr, unsigned int strd,
        unsigned int x, unsigned int y, unsigned int w, unsigned int h);
extern int stop_videolayer(unsigned int layer);

extern int hdmi_display(unsigned int vosync, unsigned int input, unsigned int output);
extern void hdmi_stop(void);

extern int mipi_tx_display(void);
extern int mipi_tx_stop(void);


#define VOU_DEV_MAX_NUM         1
static int gs_aInterfaceType[VOU_DEV_MAX_NUM] = {0};

int check_vo_support(unsigned int dev, unsigned int type, unsigned int sync)
{
    /* check interface type, ONLY VGA & HDMI interface is supported. */
    if (VOU_DEV_DHD0 == dev)
    {
        if((type & ~(VO_INTF_HDMI | VO_INTF_BT1120 | VO_INTF_BT656 | VO_INTF_MIPI | VO_INTF_MIPI_SLAVE)) || (0 == type))
        {
            printf("Hd%d only supports HDMI,BT656,BT1120,MIPI_Tx intftype, intf %d is illegal!\n",dev,type);
            return -1;
        }
        // just one interface at the the time for a dev.
        if (type & ~(VO_INTF_HDMI | VO_INTF_BT1120)
            && (type & ~VO_INTF_BT656 ) && (type & ~VO_INTF_MIPI)
            && (type & ~VO_INTF_MIPI_SLAVE))
        {
            printf("For VO %d#, Only HDMI and BT1120 can be used at the same time!\n",
                     dev);
            return -1;
        }
    }
    else
    {
        printf("Unknow dev(%d)!\n", dev);
        return -1;
    }

    if (VO_INTF_HDMI & type)
    {
       if (!((sync >= VO_OUTPUT_1080P24 && sync <= VO_OUTPUT_640x480_60)  ||
                      (sync >= VO_OUTPUT_1920x2160_30 && sync <= VO_OUTPUT_4096x2160_60)))
        {
            printf("Vo%d's intfsync %d illegal!\n", dev, sync);
            return -1;
        }
    }

    if(VO_INTF_BT1120 & type)
    {
        if (sync < VO_OUTPUT_1080P24 ||
            sync > VO_OUTPUT_7680x4320_30)
        {
            printf("Vo%d's intfsync %d illegal!\n", dev, sync);
            return -1;
        }
    }
    return 0;
}

int do_vobg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned int dev, rgb;

    if (argc < 3)
    {
        printf("Insufficient parameter!\n");
        printf ("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    dev = (unsigned int)simple_strtoul(argv[1], NULL, 10);
    rgb = (unsigned int)simple_strtoul(argv[2], NULL, 10);
    if (dev >= VOU_DEV_BUTT)
    {
        printf("Invalid parameter!\n");
        return -1;
    }

    set_vobg(dev, rgb);

    printf("dev %d set background color!\n", dev);

    return 0;
}

int do_startvo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned int dev, intftype, sync;

    if (argc < 4)
    {
        printf("Insufficient parameter!\n");
        printf ("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    dev      = (unsigned int)simple_strtoul(argv[1], NULL, 10);
    intftype = (unsigned int)simple_strtoul(argv[2], NULL, 10);
    sync     = (unsigned int)simple_strtoul(argv[3], NULL, 10);
    if (dev >= VOU_DEV_BUTT || sync >= VO_OUTPUT_BUTT)
    {
        printf("Invalid parameter!\n");
        return -1;
    }

    if (check_vo_support(dev, intftype, sync))
    {
        printf("Unsupport parameter!\n");
        return -1;
    }

    start_vo(dev, intftype, sync);

    if (intftype & VO_INTF_HDMI)
    {
        gs_aInterfaceType[dev] =  VO_INTF_HDMI;
        hdmi_display(sync, 2, 2);
    }

    if (intftype & VO_INTF_MIPI)
    {
        // to call mipi_display.
        mipi_tx_display();
    }


    printf("Dev %d opened!\n", dev);

    return 0;
}

int do_stopvo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned int dev;
    if (argc < 2)
    {
        printf("Insufficient parameter!\n");
        printf ("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    dev = (unsigned int)simple_strtoul(argv[1], NULL, 10);
    if (dev >= VOU_DEV_BUTT)
    {
        printf("Invalid parameter!\n");
        return -1;
    }

    if (gs_aInterfaceType[dev] & VO_INTF_HDMI)
    {
        gs_aInterfaceType[dev] = 0;
        hdmi_stop();
    }

    if (gs_aInterfaceType[dev] & VO_INTF_MIPI)
    {
        // To call mipi_tx_stop().
        //gs_aInterfaceType[dev] = 0;
        mipi_tx_stop();
    }

    stop_vo(dev);

    printf("Dev %d closed!\n", dev);

    return 0;
}


int do_startvl(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned int layer, strd, x, y, w, h;
    unsigned long addr;

    if (argc < 8)
    {
        printf("Insufficient parameter!\n");
        printf ("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    layer = (unsigned int)simple_strtoul(argv[1], NULL, 10);
    addr  = (unsigned long)simple_strtoul(argv[2], NULL, 16);
    strd  = (unsigned int)simple_strtoul(argv[3], NULL, 10);
    x     = (unsigned int)simple_strtoul(argv[4], NULL, 10);
    y     = (unsigned int)simple_strtoul(argv[5], NULL, 10);
    w     = (unsigned int)simple_strtoul(argv[6], NULL, 10);
    h     = (unsigned int)simple_strtoul(argv[7], NULL, 10);

    if (layer > VOU_LAYER_VSD0 || layer == VOU_LAYER_VP
        || strd > (PIC_MAX_WIDTH*2)
        || x > PIC_MAX_WIDTH   || (x & 0x1)
        || y > PIC_MAX_HEIGHT  || (y & 0x1)
        || w > PIC_MAX_WIDTH   || (w & 0x1) || w < PIC_MIN_LENTH
        || h > PIC_MAX_HEIGHT  || (h & 0x1) || h < PIC_MIN_LENTH
        )
    {
        printf("Invalid parameter!\n");
        return -1;
    }

    start_videolayer(layer, addr, strd, x, y, w, h);

    printf("video layer %d opened!\n", layer);

    return 0;
}

int do_stopvl(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    unsigned int layer;

    if (argc < 2)
    {
        printf("Insufficient parameter!\n");
        printf ("Usage:\n%s\n", cmdtp->usage);
        return -1;
    }

    layer = (unsigned int)simple_strtoul(argv[1], NULL, 10);

    if (layer > VOU_LAYER_VSD0 || layer == VOU_LAYER_VP)
    {
        printf("Invalid parameter!\n");
        return -1;
    }

    stop_videolayer(layer);

    printf("video layer %d closed!\n", layer);

    return 0;
}

U_BOOT_CMD(
	startvo,    CFG_MAXARGS,	1,  do_startvo,
    "startvo   - open vo device with a certain output interface.\n"
    "\t- startvo [dev intftype sync]",
    "\nargs: [dev, intftype, sync]\n"
    "\t-<dev> : 0: DHD0,1: DHD1\n"
    "\t-<intftype>: 8(BT656),16(BT1120),32(HDMI),16384(MIPI_Tx)\n"
    "\t-<sync>: typical value:\n"
    "\t\t0(PAL),          1(NTSC),         4(1080P30),      6(720P60)\n"
    "\t\t10(1080P60),     21(1920x1200),   26(2560x1440_30),31(3840x2160_30)\n"
    "\t\t33(3840x2160_60),45(1080x1920_60)\n"

    );

U_BOOT_CMD(
	stopvo,    CFG_MAXARGS,	1,  do_stopvo,
    "stopvo   - close interface of vo device.\n"
    "\t- stopvo [dev]",
    "\nargs: [dev]\n"
    "\t-<dev> : 0~1(HD0~HD1)\n"
    );


U_BOOT_CMD(
	startvl,    CFG_MAXARGS,	1,  do_startvl,
    "startvl   - open video layer.\n"
    "\t- startvl [layer addr stride x y w h]\n",
    "\nargs: [layer, addr, stride, x, y, w, h]\n"
    "\t-<layer>   : 0(V0), 1(V1)\n"
    "\t-<addr>    : picture address\n"
    "\t-<stride>  : picture stride\n"
    "\t-<x,y,w,h> : display area\n"
    );

U_BOOT_CMD(
	stopvl,    CFG_MAXARGS,	1,  do_stopvl,
    "stopvl   - close video layer.\n"
    "\t- stopvl [layer]",
    "\nargs: [layer]\n"
    "\t-<layer> : 0(V0), 1(V1)\n"
    );


U_BOOT_CMD(
	setvobg,    CFG_MAXARGS,	1,  do_vobg,
    "setvobg   - set vo backgroud color.\n"
    "\t- setvobg [dev color]",
    "\nargs: [dev, color]\n"
    "\t-<dev> : 0~1(HD0~1)\n"
    "\t-<color>: rgb color space\n"
    );


