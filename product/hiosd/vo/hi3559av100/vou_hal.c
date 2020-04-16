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

#ifdef __LINUX_OS__
#include <linux/kernel.h>       /* printk */
#include <asm/hardware.h>       /* IO_ADDRESS */
#include <asm/io.h>             /* ioremap_nocache */
#include <linux/delay.h>        /* for ndelay(ns) */
#else
#include <common.h>
#include <command.h>
#include <malloc.h>
//#include <devices.h>
#include <version.h>
#include <net.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
//#include <asm/sizes.h>
#endif

#include "vou_hal.h"

#include "hi3559av100_vo.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define HAL_PRINT printf
#define IO_ADDRESS(x) (x)

#define HI_INVALID_LAYER (-1)

/***************************************/
#define CRG_REGS_ADDR   0x12010000
#define CRG_REGS_SIZE   0X10000

#define CRG_PERCTL2_ADDR (0x0008 + CRG_REGS_ADDR)
#define CRG_PERCTL3_ADDR (0x000C + CRG_REGS_ADDR)
#define CRG_PERCTL4_ADDR (0x0010 + CRG_REGS_ADDR)
#define CRG_PERCTL5_ADDR (0x0014 + CRG_REGS_ADDR)
#define CRG_PERCTL6_ADDR (0x0018 + CRG_REGS_ADDR)
#define CRG_PERCTL7_ADDR (0x001c + CRG_REGS_ADDR)

#define CRG_PERCTL10_ADDR (0x0028 + CRG_REGS_ADDR)
#define CRG_PERCTL11_ADDR (0x002c + CRG_REGS_ADDR)

#define CRG_PERCTL18_ADDR (0x0048 + CRG_REGS_ADDR)
#define CRG_PERCTL19_ADDR (0x004c + CRG_REGS_ADDR)

#define CRG_PERCTL64_ADDR (0x0100 + CRG_REGS_ADDR)   /* VDP low power CRG */
#define CRG_PERCTL73_ADDR (0x0124 + CRG_REGS_ADDR)   /* VDP CRG */
#define CRG_PERCTL15_ADDR (0x003C + CRG_REGS_ADDR)

#define HDMI_REGS_ADDR  0x201f0000L
#define HDMI_REGS_SIZE  0X10000
#define IO_CONFIG_ADDR 0x1F000000
#define IO_CONFIG_BT1120_CLK        (0x1084 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA0   (0x1088 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA1   (0x108C + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA2   (0x1090 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA3   (0x1094 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA4   (0x1098 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA5   (0x109C + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA6   (0x10A0 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA7   (0x10A4 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA8   (0x10A8 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA9   (0x10AC + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA10  (0x10B0+ IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA11  (0x10B4 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA12  (0x10B8 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA13  (0x10BC + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA14  (0x10C0 + IO_CONFIG_ADDR)
#define IO_CONFIG_BT1120_DATA15  (0x10C4 + IO_CONFIG_ADDR)

#if 0
#define HDMI_DE_DLY_ADDR        (0x32*4 + HDMI_REGS_ADDR)
#define HDMI_DE_CTRL_ADDR       (0x33*4 + HDMI_REGS_ADDR)
#define HDMI_DE_TOP_ADDR        (0x34*4 + HDMI_REGS_ADDR)
#define HDMI_DE_CNTH_ADDR       (0x37*4 + HDMI_REGS_ADDR)
#define HDMI_DE_CNTL_ADDR       (0x36*4 + HDMI_REGS_ADDR)
#define HDMI_DE_LINH_ADDR       (0x39*4 + HDMI_REGS_ADDR)
#define HDMI_DE_LINL_ADDR       (0x38*4 + HDMI_REGS_ADDR)
#define HDMI_HBIT_2HSYNC1_ADDR  (0x40*4 + HDMI_REGS_ADDR)
#define HDMI_HBIT_2HSYNC2_ADDR  (0x41*4 + HDMI_REGS_ADDR)
#define HDMI_FLD2_HS_OFSTH_ADDR (0x43*4 + HDMI_REGS_ADDR)
#define HDMI_FLD2_HS_OFSTL_ADDR (0x42*4 + HDMI_REGS_ADDR)
#define HDMI_HWIDTH1_ADDR       (0x44*4 + HDMI_REGS_ADDR)
#define HDMI_HWIDTH2_ADDR       (0x45*4 + HDMI_REGS_ADDR)
#define HDMI_VBIT_TO_VSYNC_ADDR (0x46*4 + HDMI_REGS_ADDR)
#define HDMI_VWIDTH_ADDR        (0x47*4 + HDMI_REGS_ADDR)
#define HDMI_VID_MODE_ADDR      (0x4a*4 + HDMI_REGS_ADDR)
#endif

/****************************************************************************
 * EXTERN VARIABLES                                                         *
 ****************************************************************************/

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
volatile S_VDP_REGS_TYPE*   pVoReg     = NULL;
volatile S_HIPP_REGS_TYPE*  pVoHippReg = NULL;

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


/*****************************************************************************
 Prototype         : Hal initial
 Description     :

*****************************************************************************/
HI_VOID HAL_VOU_Init(HI_VOID)
{
#if 1
    pVoReg     = (volatile S_VDP_REGS_TYPE *)IO_ADDRESS(VO_BASE_ADDR);
    pVoHippReg = (volatile S_HIPP_REGS_TYPE *)IO_ADDRESS(VO_BASE_ADDR);
#else
    pVoReg = (volatile VOU_REGS_S *)ioremap_nocache(VO_BASE_ADDR,sizeof(VOU_REGS_S));
#endif
}

HI_VOID HAL_VOU_Exit(HI_VOID)
{
#if 0
    iounmap(pVoReg);
#endif
}

static inline void HI_RegSetBit(unsigned long value, unsigned long offset,
    unsigned long addr)
{
	unsigned long t, mask;

	mask = 1 << offset;
	t = readl(addr);
	t &= ~mask;
	t |= (value << offset) & mask;
	writel(t, addr);
}

static inline void HI_RegWrite32(unsigned long value, unsigned long mask,
    unsigned long addr)
{
	unsigned long t;

	t = readl(addr);
	t &= ~mask;
	t |= value & mask;
	writel(t, addr);
}

HI_VOID HAL_WriteReg(HI_U32 *pAddress, HI_U32 Value)
{
    *(volatile HI_U32*)pAddress = Value;
    return;
}

HI_U32 HAL_ReadReg(HI_U32 *pAddress)
{
    return *(volatile HI_U32*)(pAddress);
}

HI_S32 SYS_HAL_DDRConifg(HI_VOID)
{
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120130), 0x53356633);//  ## JPGE-VPSS1-VDH1-TDE-MDU-JPGD-VPSS0-VDH0
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120134), 0x05251554);  //## VICAP-PGD-AVC-IVE-AIO-VGS0-GZIP
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120138), 0x56555006);  //## USB2-SDIO0-NFC-DMA1-DMA0-VDP1-VDP0-GPU
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x1212013c), 0x37444666);  //## A7-A17-USB3-PCIE-BPD-SHA-SDIO1-CIPHER
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120140), 0x33411116);  //## VPSS2-VDH2-SFC-XOR-SATA-  -TOE-VDMA

//######### media0 timeout ctrl #######
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120034), 0x8020801c);
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120038), 0x00000000);
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x1212003c), 0x00008370);
//######### media0 pri #######
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120040), 0x00050066);
//######### media1 timeout ctrl #######
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120044), 0x8020801c);
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120048), 0x00000000);
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x1212004c), 0x00008170);
//######### media1 pri #######
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120050), 0x00050066);
//######### media2 timeout ctrl #######
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120054), 0x00000000);
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120058), 0x00008032);
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x1212005c), 0x8020801c);
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120060), 0x00000000);
//######### media2 pri #######
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120064), 0x00560600);
//######### media3 timeout ctrl #######
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120144), 0x00000000);
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120148), 0x00000000);
//######### media3 pri #######
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x1212014c), 0x00000000);
//######### hp_peri timeout ctrl #######
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x12120068), 0x81708032);
    HAL_WriteReg((HI_U32*)IO_ADDRESS( 0x1212006c), 0x00000000);

//###############################
//## mddrc0 pri&timeout setting #
//###############################
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110200),  0x00370000);  //# ports0
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110210),  0x00370000);  //# ports1
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110220),  0x00370000);  //# ports2
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110230),  0x00370000);  //# ports3
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110240),  0x00370000);  //# ports4
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110250),  0x00370000);  //# ports5
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110260),  0x00370000);  //# ports6
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110270),  0x00370000);  //# ports7
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110280),  0x00370000);  //# ports8
//##DDRC AXI pri ports0 - 6
//############## WR pri ##############
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110204),  0x76543210);  //# ports0
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110214),  0x76543210);  //# ports1
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110224),  0x76543210);  //# ports2
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110234),  0x76543210);  //# ports3
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110244),  0x76543210);  //# ports4
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110254),  0x76543210);  //# ports5
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110264),  0x76543210);  //# ports6
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110274),  0x76543210);  //# ports7
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110284),  0x76543210);  //# ports8
//############## RD pri ##############
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110208),  0x76543210);  //# ports0
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110218),  0x76543210);  //# ports1
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110228),  0x76543210);  //# ports2
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110238),  0x76543210);  //# ports3
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110248),  0x76543210);  //# ports4
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110258),  0x76543210);  //# ports5
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110268),  0x76543210);  //# ports6
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110278),  0x76543210);  //# ports7
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12110288),  0x76543210);  //# ports8
//##############  qosbuf #############
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12114000),  0x00000002);   //#qosb_push_ctrl
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x1211410c),  0x0000000a);   //#qosb_dmc_lvl
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12114110),  0x0000000a);   //#qosb_dmc_lvl
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x1211408c),  0xb3032010);   //#qosb_wbuf_ctrl
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x12114090),  0xb3032010);   //#qosb_wbuf_ctrl
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140f4),  0x00000033);   //#row-hit enable
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140ec),  0x00000055);   //#row-hit
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140f0),  0x00003333);   //#row-hit
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121141f4),  0x00000000);   //#qosb_wbuf_pri_ctrl

    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121141f0),  0x00000001);   //#enable qosbuf timeout,through prilvl to remap timeout level
//############## WR timeout ###########
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x1211409c),  0x0000000a);  //# wr_tout3 ~wr_tout0
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140a0),  0x00000000);  //# wr_tout7 ~wr_tout4
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140a4),  0x00000000);  //# wr_tout11~wr_tout8
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140a8),  0x00000000);  //# wr_tout15~wr_tout12

//############## RD timeout ###########
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140ac),  0x0000000a);  //# rd_tout3 ~rd_tout0
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140b0),  0x00000000);  //# rd_tout7 ~rd_tout4
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140b4),  0x00000000);  //# rd_tout11~rd_tout8
    HAL_WriteReg((HI_U32*)IO_ADDRESS(  0x121140b8),  0x00000000);  //# rd_tout15~rd_tout12

    return 0;
}

HI_S32 SYS_HAL_SelVoVgaPinConifg(HI_VOID)
{
    HI_RegWrite32(0x0, 0xffffffff, IO_ADDRESS(0x120f01E4));
    HI_RegWrite32(0x0, 0xffffffff, IO_ADDRESS(0x120f01E8));
    return 0;
}

HI_S32 SYS_HAL_SelVoHdmiPinConifg(HI_VOID)
{

    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x120f0110));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x120f0114));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x120f0118));
    HI_RegWrite32(0x1, 0xffffffff, IO_ADDRESS(0x120f011C));
    return 0;
}

HI_S32 SYS_HAL_SetVoHdmiReset(HI_BOOL bReset)
{
    if (bReset)
    {
        HI_RegWrite32(0x01, 0xffffffff, IO_ADDRESS(CRG_PERCTL15_ADDR));
    }
    else
    {
        HI_RegWrite32(0x0c, 0xffffffff, IO_ADDRESS(CRG_PERCTL15_ADDR));
    }
    return 0;
}

HI_S32 SYS_HAL_VouHdDacClkEn(HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 6, IO_ADDRESS(CRG_PERCTL73_ADDR));


    return 0;
}
HI_S32 SYS_HAL_SelVoHdDacClk(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 18, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouBt1120ClkSel(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 19, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouBt1120ChnSel(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 25, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouBt1120ClkEn(HI_BOOL bClkEn)
{
    HI_U32 u32Tmp = (bClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 20, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_SelVoHDMIClk(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 19, IO_ADDRESS(CRG_PERCTL15_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouSdDacClkEn(HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 7, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_SelVoBt1120PinConifg(HI_VOID)
{
    HI_RegWrite32(0xf1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_CLK));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA0));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA1));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA2));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA3));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA4));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA5));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA6));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA7));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA8));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA9));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA10));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA11));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA12));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA13));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA14));
    HI_RegWrite32(0x4f1, 0xffffffff, IO_ADDRESS(IO_CONFIG_BT1120_DATA15));
    printf("BT1120 pin config end.\n");
    return 0;
}

HI_S32 SYS_HAL_VouBtBpClkEn(HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 8, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouBtClkEn(HI_BOOL pbClkEn)
{
    volatile HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 9, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_SelVoClk(HI_U32 u32ClkSel)
{
    HI_RegWrite32(u32ClkSel << 14, 0x3 << 14, IO_ADDRESS(CRG_PERCTL73_ADDR));
    return 0;
}

HI_S32 SYS_HAL_VouSdClkSel(HI_U32 u32ClkSel)
{
    HI_RegWrite32(u32ClkSel << 16, 0x3 << 16, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_SelVoSdClkDiv(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 13, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}
HI_S32 SYS_HAL_VouVDPLowPower(HI_BOOL bClkEn)
{
    volatile HI_U32 u32Tmp = (bClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 13, IO_ADDRESS(CRG_PERCTL64_ADDR));

    return 0;
}
HI_S32 SYS_HAL_VouBusResetSel(HI_BOOL bReset)
{
    volatile HI_U32 u32Tmp = (bReset == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 0, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouDevClkEn(HI_S32 s32VoDev, HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    if (0 == s32VoDev)
    {
        HI_RegSetBit(u32Tmp, 4, IO_ADDRESS(CRG_PERCTL73_ADDR));
        HI_RegSetBit(u32Tmp, 5, IO_ADDRESS(CRG_PERCTL73_ADDR));
        HI_RegSetBit(u32Tmp, 6, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else if (1 == s32VoDev)
    {
        HI_RegSetBit(u32Tmp, 4, IO_ADDRESS(CRG_PERCTL73_ADDR));
        HI_RegSetBit(u32Tmp, 5, IO_ADDRESS(CRG_PERCTL73_ADDR));
        HI_RegSetBit(u32Tmp, 7, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else
    {
        return -1;
    }


    return 0;
}

HI_S32 SYS_HAL_VouHdOutClkSel(HI_U32 u32ClkSel)
{
    HI_RegSetBit(u32ClkSel, 12, IO_ADDRESS(CRG_PERCTL73_ADDR));
    return 0;
}

/*hdmi  clk*/
HI_S32 SYS_HAL_VouHdmiClkEn(HI_S32 s32VoDev, HI_BOOL pbHdmiClkEn)
{
    HI_U32 u32Tmp = (pbHdmiClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 27, IO_ADDRESS(CRG_PERCTL73_ADDR));
    HI_RegSetBit(u32Tmp, 10, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

/*hdmi  clk*/
HI_S32 SYS_HAL_VouHdmiTxClkEn(HI_S32 s32VoDev, HI_BOOL pbHdmiClkEn)
{
    HI_U32 u32Tmp = (pbHdmiClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 27, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouMipiTxChnSel(HI_U32 u32ClkSel)
{

    HI_RegSetBit(u32ClkSel, 26, IO_ADDRESS(CRG_PERCTL73_ADDR));
    HI_RegSetBit(u32ClkSel, 30, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}


HI_S32 SYS_HAL_VouMipiClkEn(HI_S32 s32VoDev, HI_BOOL pbMipiClkEn)
{
    HI_U32 u32Tmp = (pbMipiClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 11, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}
HI_S32 SYS_HAL_VouHd0DivMode(HI_S32 s32Dev, HI_U32 u32Hd0DivMod)
{
    if (0 == s32Dev || 1 == s32Dev)
    {
        // set 2bit from 12th bit
        HI_RegWrite32(u32Hd0DivMod << 12, 0x3 << 12, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}

HI_S32 SYS_HAL_SetVoPllFrac(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        // set 24 bits at 0bit
        HI_RegWrite32(u32BitsSet << 0, 0xffffff << 0, IO_ADDRESS(CRG_PERCTL10_ADDR));

        if (0 == u32BitsSet)
        {
            HI_RegSetBit(1, 24, IO_ADDRESS(CRG_PERCTL11_ADDR));
        }
        else
        {
            HI_RegSetBit(0, 24, IO_ADDRESS(CRG_PERCTL11_ADDR));
        }
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 0, 0xffffff << 0, IO_ADDRESS(CRG_PERCTL10_ADDR));

        if (0 == u32BitsSet)
        {
            HI_RegSetBit(1, 24, IO_ADDRESS(CRG_PERCTL11_ADDR));
        }
        else
        {
            HI_RegSetBit(0, 24, IO_ADDRESS(CRG_PERCTL11_ADDR));
        }
    }
    else
    {
        return -1;
    }
    return 0;

}

HI_S32 SYS_HAL_SetVoPllPostdiv1(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        // set 3 bits at 24bit
        HI_RegWrite32(u32BitsSet << 24, 0x7 << 24, IO_ADDRESS(CRG_PERCTL10_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 24, 0x7 << 24, IO_ADDRESS(CRG_PERCTL10_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;

}

HI_S32 SYS_HAL_SetVoPllPostdiv2(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        //set 3 bits at 28bit
        HI_RegWrite32(u32BitsSet << 28, 0x7 << 28, IO_ADDRESS(CRG_PERCTL10_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 28, 0x7 << 28, IO_ADDRESS(CRG_PERCTL10_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;

}
HI_S32 SYS_HAL_SetVoPllRefdiv(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        //set 6 bits at 12bit.
        HI_RegWrite32(u32BitsSet << 12, 0x3f << 12, IO_ADDRESS(CRG_PERCTL11_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 12, 0x3f << 12, IO_ADDRESS(CRG_PERCTL11_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;

}
HI_S32 SYS_HAL_SetVoPllFbdiv(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        // set 11 bits at 0bit.
        HI_RegWrite32(u32BitsSet << 0, 0xfff << 0, IO_ADDRESS(CRG_PERCTL11_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 0, 0xfff << 0, IO_ADDRESS(CRG_PERCTL11_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;

}

HI_S32 SYS_HAL_SetVoSPllFrac(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        // set 24 bits at 0bit
        HI_RegWrite32(u32BitsSet << 0, 0xffffff << 0, IO_ADDRESS(CRG_PERCTL18_ADDR));

        if (0 == u32BitsSet)
        {
            HI_RegSetBit(1, 24, IO_ADDRESS(CRG_PERCTL19_ADDR));
        }
        else
        {
            HI_RegSetBit(0, 24, IO_ADDRESS(CRG_PERCTL19_ADDR));
        }
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 0, 0xffffff << 0, IO_ADDRESS(CRG_PERCTL18_ADDR));

        if (0 == u32BitsSet)
        {
            HI_RegSetBit(1, 24, IO_ADDRESS(CRG_PERCTL19_ADDR));
        }
        else
        {
            HI_RegSetBit(0, 24, IO_ADDRESS(CRG_PERCTL19_ADDR));
        }
    }
    else
    {
        return -1;
    }
    return 0;

}

HI_S32 SYS_HAL_SetVoSPllPostdiv1(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        // set 3 bits at 24bit
        HI_RegWrite32(u32BitsSet << 24, 0x7 << 24, IO_ADDRESS(CRG_PERCTL18_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 24, 0x7 << 24, IO_ADDRESS(CRG_PERCTL18_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;

}

HI_S32 SYS_HAL_SetVoSPllPostdiv2(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        //set 3 bits at 28bit
        HI_RegWrite32(u32BitsSet << 28, 0x7 << 28, IO_ADDRESS(CRG_PERCTL18_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 28, 0x7 << 28, IO_ADDRESS(CRG_PERCTL18_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;

}
HI_S32 SYS_HAL_SetVoSPllRefdiv(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        //set 6 bits at 12bit.
        HI_RegWrite32(u32BitsSet << 12, 0x3f << 12, IO_ADDRESS(CRG_PERCTL19_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 12, 0x3f << 12, IO_ADDRESS(CRG_PERCTL19_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;

}
HI_S32 SYS_HAL_SetVoSPllFbdiv(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
    {
        // set 11 bits at 0bit.
        HI_RegWrite32(u32BitsSet << 0, 0xfff << 0, IO_ADDRESS(CRG_PERCTL19_ADDR));
    }
    else if(1 == s32Pll)
    {
        HI_RegWrite32(u32BitsSet << 0, 0xfff << 0, IO_ADDRESS(CRG_PERCTL19_ADDR));
    }
    else
    {
        return -1;
    }
    return 0;

}


HI_S32 SYS_HAL_VouOutClkSel(HI_S32 s32Dev, HI_U32 u32ClkSel)
{
    if (0 == s32Dev || 1 == s32Dev)
    {
        // set 3bits at 18bit.
        HI_RegWrite32(u32ClkSel << 18, 0x7 << 18, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}

/* VO DHD clk sel*/
HI_S32 SYS_HAL_VouHdClkSel(HI_S32 s32Dev, HI_U32 u32ClkSel)
{
    if (0 == s32Dev)
    {
        // set 2bits at 16bit.
        HI_RegWrite32(u32ClkSel << 16, 0x3 << 16, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}

HI_S32 SYS_HAL_VouHd1DivMode(HI_S32 s32Dev, HI_U32 u32Hd1DivMod)
{
    if (0 == s32Dev || 1 == s32Dev)
    {
        // set 2 bits at 16bit.
        HI_RegWrite32(u32Hd1DivMod << 14, 0x3 << 14, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}

HI_S32 SYS_HAL_VouCFGClkEn(HI_S32 s32Dev, HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    if (0 == s32Dev )
    {
        HI_RegSetBit(u32Tmp, 3, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}

HI_S32 SYS_HAL_VouCoreClkEn(HI_S32 s32Dev, HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    if (0 == s32Dev)
    {
        HI_RegSetBit(u32Tmp, 5, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}


/* VO APB clk en*/
HI_S32 SYS_HAL_VouAPBClkEn(HI_S32 s32Dev, HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    if (0 == s32Dev)
    {
        HI_RegSetBit(u32Tmp, 1, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}

/* VO AXI BUS CLK */
HI_S32 SYS_HAL_VouBusClkEn(HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;
    HI_RegSetBit(u32Tmp, 2, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouMipiTxClkEn(HI_S32 s32VoDev, HI_BOOL pbMipiClkEn)
{
    HI_U32 u32Tmp = (pbMipiClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 28, IO_ADDRESS(CRG_PERCTL73_ADDR));

    return 0;
}

HI_VOID HAL_SYS_Control(HI_VOID)
{
    volatile U_VOCTRL VOCTRL;

    /* outstand */
    VOCTRL.u32 = pVoReg->VOCTRL.u32;
    VOCTRL.u32 = 0x80000000;
    pVoReg->VOCTRL.u32 = VOCTRL.u32;
}


HI_U32 inline HAL_GetXDC_BUMA(HI_S32 s32Value)
{
    HI_U32 u32AbsValue = s32Value << 2;

    if(s32Value >= 0)
    {
        return u32AbsValue & 0x7ff;
    }
    else
    {
        u32AbsValue = (~((-1)* u32AbsValue) +1);
        //return ( (((~u32AbsValue)+1)& 0xFF) | 0x100 );
        u32AbsValue = ((u32AbsValue & 0x7ff) | 0x400);
        return u32AbsValue;
    }
}

HI_U32 inline HAL_GetODC_BUMA_NEW(HI_S32 s32Value)
{
    HI_U32 u32AbsValue = s32Value << 2;

    if (s32Value >= 0)
    {
        return u32AbsValue & 0xfff;
    }
    else
    {
        u32AbsValue = (~((-1) * u32AbsValue) + 1);
        u32AbsValue = ((u32AbsValue & 0xfff) | 0x1000);
        return u32AbsValue;
    }
}

HI_U32 inline HAL_Conver_CscCoef(HI_S32 s32Value)
{
    HI_S32 s32Result = ((s32Value << 10)/ 1000 );

    if (s32Result < 0)
    //if (s32Value < 0)
    {
        s32Result = (~((-1)*s32Result) + 1);

        /* 5.8 format */
        s32Result = ((s32Result & 0x7fff) | 0x4000);
    }
    else
    {
        s32Result = s32Result & 0x7fff;
    }

    return s32Result;
}

HI_U32 inline HAL_Conver_CscCoef_NEW(HI_S32 s32Value)
{
    HI_S32 s32Result = ((s32Value << 10) / 1000 );

    if (s32Result < 0)
    {
        s32Result = (~((-1) * s32Result) + 1);

        s32Result = ((s32Result & 0xffff) | 0x8000);
    }
    else
    {
        s32Result = s32Result & 0xffff;
    }

    return (HI_U32)s32Result;
}

HI_UL Vou_GetAbsAddr(HAL_DISP_LAYER_E enLayer, HI_UL pReg)
{
    HI_UL RegAbsAddr;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0 :
        case HAL_DISP_LAYER_VHD1 :
        {
            RegAbsAddr = (pReg) + (enLayer - HAL_DISP_LAYER_VHD0) * VHD_REGS_LEN;
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        {
            RegAbsAddr = (pReg) + (enLayer - HAL_DISP_LAYER_GFX0) * GFX_REGS_LEN;
            break;
        }

        // one wbc dev
        case HAL_DISP_LAYER_WBC:
            RegAbsAddr = (pReg);
            break;

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}

HI_UL Vou_GetChnAbsAddr(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_UL pReg)
{
    volatile HI_UL RegAbsAddr;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            RegAbsAddr = pReg + (enChan - HAL_DISP_CHANNEL_DHD0) * DHD_REGS_LEN;
            break;
        }

        default:
        {
            printf("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}

HI_UL Vou_GetHdmiOrMipiIntfAbsAddr(HAL_DISP_INTF_E enIntf, HI_UL pReg)
{
    volatile HI_UL RegAbsAddr;

    switch (enIntf)
    {
        case HAL_DISP_INTF_HDMI:
        {
            RegAbsAddr = pReg ;
            break;
        }

        case HAL_DISP_INTF_MIPI:
        {
            RegAbsAddr = pReg + 0x80; // Reg INTF_MIPI_CTRL's offset ralative reg INTF_HDMI_CTRL
            break;
        }
        default:
        {
            HAL_PRINT("Error intf id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}


 //for get Intf abs addr
HI_UL Vou_GetIntfAbsAddr(HAL_DISP_INTF_E enIntf, HI_UL pReg)
{
     volatile HI_UL RegAbsAddr;

     switch (enIntf)
     {
         case HAL_DISP_INTF_HDMI:
         {
             RegAbsAddr = pReg ;
             break;
         }

         case HAL_DISP_INTF_MIPI:
         {
             RegAbsAddr = pReg + 0x80; // Reg INTF_MIPI_CTRL's offset ralative reg INTF_HDMI_CTRL
             break;
         }

         case HAL_DISP_INTF_BT656:
         case HAL_DISP_INTF_BT1120:
         {
             RegAbsAddr = pReg + 1 * INTF_REGS_LEN;
             break;
         }

         case HAL_DISP_INTF_LCD:
         case HAL_DISP_INTF_LCD_6BIT:
         case HAL_DISP_INTF_LCD_8BIT:
         case HAL_DISP_INTF_LCD_16BIT:
         case HAL_DISP_INTF_LCD_24BIT:
         {
             RegAbsAddr = pReg + 2 * INTF_REGS_LEN;
             break;
         }

         default:
         {
             printf("Error intf id found in %s: L%d\n", __FUNCTION__, __LINE__);
             return 0;
         }
     }
     return RegAbsAddr;
}

/*
* Name : Vou_GetVidAbsAddr
* Desc : VID means VIDeo,related to video's input resolution,
         video's luma address,video's chrom address,video's data fmt,
**/
HI_UL Vou_GetVidAbsAddr(HAL_DISP_LAYER_E enLayer, HI_UL pReg)
{
    volatile HI_UL RegAbsAddr;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0 :
        case HAL_DISP_LAYER_VHD1 :
        {
            RegAbsAddr = pReg + (enLayer - HAL_DISP_LAYER_VHD0) * VID_REGS_LEN;
            break;
        }

        case HAL_DISP_LAYER_WBC :
            RegAbsAddr = pReg;
            break;

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}


/*
* Name : Vou_GetGfxAbsAddr
* Desc : GFX means Graphics,related to graphics' input resolution,
         graphics' luma address,graphics' chrom address,graphics' data fmt,
**/
HI_UL Vou_GetGfxAbsAddr(HAL_DISP_LAYER_E enLayer, HI_UL pReg)
{
    volatile HI_UL RegAbsAddr;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_GFX0 :
        case HAL_DISP_LAYER_GFX1 :
        {
            RegAbsAddr = pReg + (enLayer - HAL_DISP_LAYER_GFX0) * GRF_REGS_LEN;
            break;
        }
        default:
        {
            HAL_PRINT("Error layer id found in FUNC:%s,LINE:%d\n", __FUNCTION__, __LINE__);
            return 0;
        }
    }

    return RegAbsAddr;
}


HI_U32 HAL_GetBkg_BUMA(HI_U8 u8Bkg_YUV)
{
    HI_U32 u32Value = 0;
    return (u32Value|u8Bkg_YUV) << 2;
}

HI_VOID HAL_SYS_SetOutstanding(HI_VOID)
{

    return ;
}

/* AXI Master select */
HI_VOID HAL_SYS_SetAxiMaster(HI_VOID)
{
    /*
    volatile U_VOAXISEL VOAXISEL;

    VOAXISEL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VOAXISEL.u32));
    VOAXISEL.bits.v0_axi_sel = 0;
    VOAXISEL.bits.v1_axi_sel = 0;
    VOAXISEL.bits.v3_axi_sel = 0;
    VOAXISEL.bits.v4_axi_sel = 0;
    VOAXISEL.bits.g0_axi_sel = 1;
    VOAXISEL.bits.g1_axi_sel = 1;
    VOAXISEL.bits.g2_axi_sel = 1;
    VOAXISEL.bits.g3_axi_sel = 1;
    VOAXISEL.bits.g4_axi_sel = 1;
    VOAXISEL.bits.wbc_dhd_axi_sel = 0;
    VOAXISEL.bits.wbc_g0_axi_sel = 1;
    VOAXISEL.bits.wbc_g4_axi_sel = 1;
    VOAXISEL.bits.para_axi_sel = 0;
    HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VOAXISEL.u32), VOAXISEL.u32);
    //*/

    return;
}

HI_VOID HAL_SYS_SetRdBusId(HI_U32 bMode)
{

    return ;
}

HI_BOOL HAL_DISP_SetIntfEnable(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL enIntf)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
    volatile HI_UL addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_CTRL.u32));
            //osal_printk("++++%p++++\n",(HI_U32*)addr_REG);
            DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_CTRL.bits.intf_en = enIntf;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_CTRL.u32);

            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetIntfCtrl(HAL_DISP_INTF_E enIntf, HI_U32* pu32CtrlInfo)
{
    U_INTF_HDMI_CTRL  INTF_HDMI_CTRL;
//    U_INTF_MIPI_CTRL  INTF_MIPI_CTRL;
    U_INTF_BT_CTRL    INTF_BT_CTRL;
    U_INTF_LCD_CTRL   INTF_LCD_CTRL;
    volatile HI_UL    addr_REG;
    U_INTF_HDMI_CTRL* pstHdmiCtrl;
//    U_INTF_MIPI_CTRL* pstMipiCtrl;
    U_INTF_BT_CTRL*   pstBtCtrl;
    U_INTF_LCD_CTRL*  pstLCDCtrl;

    if (VO_INTF_BT1120    == enIntf ||
        VO_INTF_BT656     == enIntf)
    {
        pstBtCtrl = (U_INTF_BT_CTRL*)pu32CtrlInfo;
        addr_REG = Vou_GetIntfAbsAddr(enIntf, (HI_UL) & (pVoReg->INTF_HDMI_CTRL.u32));
        INTF_BT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        INTF_BT_CTRL.bits.hdmi_mode          = pstBtCtrl->bits.hdmi_mode;
        INTF_BT_CTRL.bits.lcd_serial_mode    = pstBtCtrl->bits.lcd_serial_mode;
        INTF_BT_CTRL.bits.lcd_serial_perd    = pstBtCtrl->bits.lcd_serial_perd;
        INTF_BT_CTRL.bits.lcd_parallel_order = pstBtCtrl->bits.lcd_parallel_order;
        INTF_BT_CTRL.bits.lcd_data_inv       = pstBtCtrl->bits.lcd_data_inv;
        INTF_BT_CTRL.bits.lcd_parallel_mode  = pstBtCtrl->bits.lcd_parallel_mode;
        HAL_WriteReg((HI_U32*)addr_REG, INTF_BT_CTRL.u32);
    }
    else if(VO_INTF_LCD       == enIntf ||
            VO_INTF_LCD_6BIT  == enIntf ||
            VO_INTF_LCD_8BIT  == enIntf ||
            VO_INTF_LCD_16BIT == enIntf ||
            VO_INTF_LCD_18BIT == enIntf ||
            VO_INTF_LCD_24BIT == enIntf)
    {
        pstLCDCtrl = (U_INTF_LCD_CTRL*)pu32CtrlInfo;
        addr_REG   = Vou_GetIntfAbsAddr(enIntf, (HI_UL) & (pVoReg->INTF_HDMI_CTRL.u32));
        INTF_LCD_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        INTF_LCD_CTRL.bits.hdmi_mode          = pstLCDCtrl->bits.hdmi_mode;
        INTF_LCD_CTRL.bits.lcd_serial_mode    = pstLCDCtrl->bits.lcd_serial_mode;
        INTF_LCD_CTRL.bits.lcd_serial_perd    = pstLCDCtrl->bits.lcd_serial_perd;
        INTF_LCD_CTRL.bits.lcd_parallel_order = pstLCDCtrl->bits.lcd_parallel_order;
        INTF_LCD_CTRL.bits.lcd_data_inv       = pstLCDCtrl->bits.lcd_data_inv;
        INTF_LCD_CTRL.bits.lcd_parallel_mode  = pstLCDCtrl->bits.lcd_parallel_mode;
        HAL_WriteReg((HI_U32*)addr_REG, INTF_LCD_CTRL.u32);
    }
    else if(VO_INTF_HDMI      == enIntf ||
            VO_INTF_MIPI      == enIntf)
    {
        pstHdmiCtrl = (U_INTF_HDMI_CTRL*)pu32CtrlInfo;
        addr_REG    = Vou_GetHdmiOrMipiIntfAbsAddr(enIntf, (HI_UL) & (pVoReg->INTF_HDMI_CTRL.u32));
        INTF_HDMI_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        INTF_HDMI_CTRL.bits.hdmi_mode      = pstHdmiCtrl->bits.hdmi_mode;
        INTF_HDMI_CTRL.bits.intf_420_mode  = pstHdmiCtrl->bits.intf_420_mode;
        INTF_HDMI_CTRL.bits.intf_420_en    = pstHdmiCtrl->bits.intf_420_en;
        INTF_HDMI_CTRL.bits.intf_422_en    = pstHdmiCtrl->bits.intf_422_en;
        HAL_WriteReg((HI_U32*)addr_REG, INTF_HDMI_CTRL.u32);
    }
    else
    {
        HAL_PRINT("Error interface type found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }


    return HI_TRUE;

}


HI_BOOL HAL_DISP_SetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,
                                     HAL_DISP_SYNCINFO_S *pstSyncInfo)
{
    volatile U_DHD0_CTRL   DHD0_CTRL;
    volatile U_DHD0_VSYNC1 DHD0_VSYNC1;
    volatile U_DHD0_VSYNC2 DHD0_VSYNC2;
    volatile U_DHD0_HSYNC1 DHD0_HSYNC1;
    volatile U_DHD0_HSYNC2 DHD0_HSYNC2;
    volatile U_DHD0_VPLUS1 DHD0_VPLUS1;
    volatile U_DHD0_VPLUS2 DHD0_VPLUS2;
    volatile U_DHD0_PWR    DHD0_PWR;

    volatile HI_UL addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_CTRL.u32));
            DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_CTRL.bits.iop   = pstSyncInfo->bIop;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_CTRL.u32);

            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_HSYNC1.u32));
            DHD0_HSYNC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_HSYNC1.bits.hact = pstSyncInfo->u16Hact - 1;
            DHD0_HSYNC1.bits.hbb  = (pstSyncInfo->u16Hbb) - 1;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_HSYNC1.u32);

            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_HSYNC2.u32));
            DHD0_HSYNC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_HSYNC2.bits.hmid = (pstSyncInfo->u16Hmid == 0) ? 0 : pstSyncInfo->u16Hmid - 1;
            DHD0_HSYNC2.bits.hfb  = (pstSyncInfo->u16Hfb) - 1;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_HSYNC2.u32);


            //Config VHD interface veritical timming
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_VSYNC1.u32));
            DHD0_VSYNC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_VSYNC1.bits.vact = pstSyncInfo->u16Vact  - 1;
            DHD0_VSYNC1.bits.vbb = pstSyncInfo->u16Vbb - 1;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_VSYNC1.u32);

            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_VSYNC2.u32));
            DHD0_VSYNC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_VSYNC2.bits.vfb =  pstSyncInfo->u16Vfb - 1;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_VSYNC2.u32);

            //Config VHD interface veritical bottom timming,no use in progressive mode
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_VPLUS1.u32));
            DHD0_VPLUS1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_VPLUS1.bits.bvact = pstSyncInfo->u16Bvact - 1;
            DHD0_VPLUS1.bits.bvbb = pstSyncInfo->u16Bvbb - 1;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_VPLUS1.u32);

            // TODO
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_VPLUS2.u32));
            DHD0_VPLUS2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_VPLUS2.bits.bvfb =  pstSyncInfo->u16Bvfb - 1;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_VPLUS2.u32);

            //Config VHD interface veritical bottom timming,
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_PWR.u32));
            DHD0_PWR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_PWR.bits.hpw = pstSyncInfo->u16Hpw - 1;
            DHD0_PWR.bits.vpw = pstSyncInfo->u16Vpw - 1;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_PWR.u32);

            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDevMultiChnEn(HAL_DISP_OUTPUTCHANNEL_E enChn, HAL_MULTICHN_EN enMultiChnEn)
{
    volatile U_DHD0_PWR DHD0_PWR;
    volatile HI_UL addr_REG;
    addr_REG = Vou_GetChnAbsAddr(enChn, (HI_UL) & (pVoReg->DHD0_PWR.u32));
    DHD0_PWR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    DHD0_PWR.bits.multichn_en = enMultiChnEn;
    HAL_WriteReg((HI_U32*)addr_REG, DHD0_PWR.u32);

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetIntfSyncInv(HAL_DISP_INTF_E enIntf, HAL_DISP_SYNCINV_S *pstInv)
{
    volatile U_INTF_HDMI_SYNC_INV INTF_HDMI_SYNC_INV;
    volatile HI_UL addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf, (HI_UL) & (pVoReg->INTF_HDMI_SYNC_INV.u32));
    INTF_HDMI_SYNC_INV.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_HDMI_SYNC_INV.bits.f_inv = pstInv->u32f_inv;
    INTF_HDMI_SYNC_INV.bits.vs_inv = pstInv->u32vs_inv;
    INTF_HDMI_SYNC_INV.bits.hs_inv = pstInv->u32hs_inv;
    INTF_HDMI_SYNC_INV.bits.dv_inv = pstInv->u32dv_inv;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_HDMI_SYNC_INV.u32);

    return HI_TRUE;
}

HI_BOOL  HAL_DISP_SetIntfMuxDefaultSel(HI_VOID)
{
    return HI_TRUE;
}

HI_BOOL  HAL_DISP_SetIntfMuxSel(HAL_DISP_OUTPUTCHANNEL_E enChan,HAL_DISP_INTF_E enIntf)
{
    //*
    volatile U_VO_MUX VO_MUX;
    volatile  HI_UL addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        {
            addr_REG = (HI_UL) & (pVoReg->VO_MUX.u32);
            VO_MUX.u32 = HAL_ReadReg((HI_U32*)addr_REG);

            switch (enIntf)
            {
                case HAL_DISP_INTF_BT1120:
                {
                    VO_MUX.bits.digital_sel = 0;
                    VO_MUX.bits.bt_sel      = 0;
                    break;
                }

                case HAL_DISP_INTF_BT656:
                {
                    VO_MUX.bits.digital_sel = 1;
                    VO_MUX.bits.bt_sel      = 0;
                    break;
                }

                case HAL_DISP_INTF_MIPI:
                {
                    VO_MUX.bits.digital_sel = 0;
                    VO_MUX.bits.mipi_sel    = 0;
                    break;
                }

                default:
                {
                    printf("Error interface id found in %s: L%d \n", __FUNCTION__, __LINE__);
                    return HI_FALSE;
                }
            }

            break;
        }

        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = (HI_UL) & (pVoReg->VO_MUX.u32);
            VO_MUX.u32 = HAL_ReadReg((HI_U32*)addr_REG);

            switch (enIntf)
            {
                case HAL_DISP_INTF_BT1120:
                {
                    VO_MUX.bits.digital_sel = 0;
                    VO_MUX.bits.bt_sel      = 1;
                    break;
                }

                case HAL_DISP_INTF_BT656:
                {
                    VO_MUX.bits.digital_sel = 1;
                    VO_MUX.bits.bt_sel      = 1;
                    break;
                }

                case HAL_DISP_INTF_MIPI:
                {
                    VO_MUX.bits.digital_sel = 0;
                    VO_MUX.bits.mipi_sel    = 1;
                    break;
                }

                case HAL_DISP_INTF_LCD:
                case HAL_DISP_INTF_LCD_6BIT:
                case HAL_DISP_INTF_LCD_8BIT:
                case HAL_DISP_INTF_LCD_16BIT:
                case HAL_DISP_INTF_LCD_24BIT:
                {
                    VO_MUX.bits.digital_sel = 2;
                    VO_MUX.bits.lcd_sel =     1;
                    break;
                }

                default:
                {
                    printf("Error interface id found in %s: L%d \n", __FUNCTION__, __LINE__);
                    return HI_FALSE;
                }
            }

            break;
        }

        default:
        {
            printf("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    // TODO
    HAL_WriteReg((HI_U32*) & (pVoReg->VO_MUX.u32), VO_MUX.u32);
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetIntfDacEnable(HAL_DISP_INTF_E enIntf, HI_BOOL bDacEnable)
{
    /*
    volatile U_VO_DAC_CTRL VO_DAC_CTRL;
    volatile U_VO_DAC_C_CTRL VO_DAC_C_CTRL;
    volatile U_VO_DAC_R_CTRL VO_DAC_R_CTRL;
    volatile U_VO_DAC_G_CTRL VO_DAC_G_CTRL;
    volatile U_VO_DAC_B_CTRL VO_DAC_B_CTRL;

    switch(enIntf)
    {
        case(HAL_DISP_INTF_VGA):
            VO_DAC_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_CTRL.u32));
            VO_DAC_CTRL.bits.envbg = bDacEnable;
            HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_CTRL.u32), VO_DAC_CTRL.u32);

            VO_DAC_R_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_R_CTRL.u32));
            VO_DAC_R_CTRL.bits.dac_en = bDacEnable;
            HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_R_CTRL.u32), VO_DAC_R_CTRL.u32);

            VO_DAC_G_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_G_CTRL.u32));
            VO_DAC_G_CTRL.bits.dac_en = bDacEnable;
            HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_G_CTRL.u32), VO_DAC_G_CTRL.u32);

            VO_DAC_B_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_B_CTRL.u32));
            VO_DAC_B_CTRL.bits.dac_en = bDacEnable;
            HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_B_CTRL.u32), VO_DAC_B_CTRL.u32);

            break;
        case(HAL_DISP_INTF_CVBS):
            VO_DAC_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_CTRL.u32));
            VO_DAC_CTRL.bits.envbg = bDacEnable;
            HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_CTRL.u32), VO_DAC_CTRL.u32);

            VO_DAC_C_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_C_CTRL.u32));
            VO_DAC_C_CTRL.bits.dac_en = bDacEnable;
            HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_C_CTRL.u32), VO_DAC_C_CTRL.u32);

            break;
        default:
            break;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_VgaDacEn(HI_BOOL bEn)
{
    /*
    volatile U_VO_MUX_DAC VO_MUX_DAC;
    if(bEn)
    {
        VO_MUX_DAC.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_MUX_DAC.u32));
        VO_MUX_DAC.bits.dac2_sel = 9;
        VO_MUX_DAC.bits.dac1_sel = 8;
        VO_MUX_DAC.bits.dac0_sel = 7;
        HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_MUX_DAC.u32), VO_MUX_DAC.u32);
    }

    //*/
    return HI_TRUE;
}

HI_BOOL HAL_DISP_CvbsDacEn(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL bEn)
{
    /*
    volatile U_VO_MUX_DAC VO_MUX_DAC;

    VO_MUX_DAC.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_MUX_DAC.u32));
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        VO_MUX_DAC.bits.dac3_sel = 0;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_MUX_DAC.u32), VO_MUX_DAC.u32);

    //*/
    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetVgaGc(HI_U32 u32Value)
{
    /*
    volatile U_VO_DAC_R_CTRL VO_DAC_R_CTRL;
    volatile U_VO_DAC_G_CTRL VO_DAC_G_CTRL;
    volatile U_VO_DAC_B_CTRL VO_DAC_B_CTRL;

    VO_DAC_R_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_R_CTRL.u32));
    VO_DAC_R_CTRL.bits.dacgc = u32Value;
    HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_R_CTRL.u32), VO_DAC_R_CTRL.u32);

    VO_DAC_G_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_G_CTRL.u32));
    VO_DAC_G_CTRL.bits.dacgc = u32Value;
    HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_G_CTRL.u32), VO_DAC_G_CTRL.u32);

    VO_DAC_B_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_B_CTRL.u32));
    VO_DAC_B_CTRL.bits.dacgc = u32Value;
    HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_B_CTRL.u32), VO_DAC_B_CTRL.u32);

    //*/


    return HI_TRUE;
}


HI_BOOL HAL_DISP_SetCvbsGc(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Value)
{

    /*
    volatile U_VO_DAC_C_CTRL VO_DAC_C_CTRL;

    VO_DAC_C_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_C_CTRL.u32));
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        VO_DAC_C_CTRL.bits.dacgc = u32Value;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VO_DAC_C_CTRL.u32), VO_DAC_C_CTRL.u32);
    //*/
    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetIntfCSCEn(HAL_DISP_INTF_E enIntf,HI_BOOL bCscEn)
{
    /*
    U_INTF_CSC_IDC INTF_CSC_IDC;
    volatile  HI_UL addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_UL)&(pVoReg->INTF_CSC_IDC.u32));
    INTF_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_CSC_IDC.bits.csc_en = bCscEn;
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_IDC.u32);
    //*/
    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetIntfCscCoef(HAL_DISP_INTF_E enIntf,CscCoef_S *pstCoef)
{
    /*
    U_INTF_CSC_IDC INTF_CSC_IDC;
    U_INTF_CSC_ODC INTF_CSC_ODC;
    U_INTF_CSC_IODC INTF_CSC_IODC;
    U_INTF_CSC_P0  INTF_CSC_P0;
    U_INTF_CSC_P1  INTF_CSC_P1;
    U_INTF_CSC_P2  INTF_CSC_P2;
    U_INTF_CSC_P3  INTF_CSC_P3;
    U_INTF_CSC_P4  INTF_CSC_P4;
    volatile  HI_UL addr_REG;
    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_UL)&(pVoReg->INTF_CSC_IDC.u32));
    INTF_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    INTF_CSC_IDC.bits.cscidc1 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc1);
//    DHD0_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc0);
    INTF_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc2);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_IDC.u32);

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_UL)&(pVoReg->INTF_CSC_ODC.u32));
    INTF_CSC_ODC.bits.cscodc1 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc1);
    //DHD0_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc0);
    INTF_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc2);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_ODC.u32);

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_UL)&(pVoReg->INTF_CSC_IODC.u32));
//    DHD0_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc2);
//    DHD0_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc2);
    INTF_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCoef->csc_in_dc0);
    INTF_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCoef->csc_out_dc0);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_IODC.u32);

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_UL)&(pVoReg->INTF_CSC_P0.u32));
    INTF_CSC_P0.bits.cscp00 = HAL_Conver_CscCoef(pstCoef->csc_coef00);
    INTF_CSC_P0.bits.cscp01 = HAL_Conver_CscCoef(pstCoef->csc_coef01);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P0.u32);

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_UL)&(pVoReg->INTF_CSC_P1.u32));
    INTF_CSC_P1.bits.cscp02 = HAL_Conver_CscCoef(pstCoef->csc_coef02);
    INTF_CSC_P1.bits.cscp10 = HAL_Conver_CscCoef(pstCoef->csc_coef10);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P1.u32);

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_UL)&(pVoReg->INTF_CSC_P2.u32));
    INTF_CSC_P2.bits.cscp11 = HAL_Conver_CscCoef(pstCoef->csc_coef11);
    INTF_CSC_P2.bits.cscp12 = HAL_Conver_CscCoef(pstCoef->csc_coef12);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P2.u32);

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_UL)&(pVoReg->INTF_CSC_P3.u32));
    INTF_CSC_P3.bits.cscp20 = HAL_Conver_CscCoef(pstCoef->csc_coef20);
    INTF_CSC_P3.bits.cscp21 = HAL_Conver_CscCoef(pstCoef->csc_coef21);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P3.u32);

    addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_UL)&(pVoReg->INTF_CSC_P4.u32));
    INTF_CSC_P4.bits.cscp22 = HAL_Conver_CscCoef(pstCoef->csc_coef22);
    HAL_WriteReg((HI_U32*)addr_REG, INTF_CSC_P4.u32);

    //*/

    return HI_TRUE;
}

HI_BOOL  HAL_DISP_SetIntfClip(HAL_DISP_INTF_E enIntf,
                                        HI_BOOL bClip,
                                        HAL_DISP_CLIP_S *pstClipData)
{
    volatile U_BT_CLIP0_L    BT_CLIP0_L;
    volatile U_BT_CLIP0_H    BT_CLIP0_H;

    volatile  HI_UL addr_REG;



    //addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)(HI_UL)&(pVoReg->INTF_CLIP0_L.u32));
    addr_REG = (HI_UL) & (pVoReg->BT_CLIP0_L.u32);
    BT_CLIP0_L.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    // osal_printk("+++HAL_DISP_SetIntfClip:%d+++\n",__LINE__);
    BT_CLIP0_L.bits.clip_en = bClip;
    BT_CLIP0_L.bits.clip_cl2 = pstClipData->u16ClipLow_y;
    BT_CLIP0_L.bits.clip_cl1 = pstClipData->u16ClipLow_cb;
    BT_CLIP0_L.bits.clip_cl0 = pstClipData->u16ClipLow_cr;
    HAL_WriteReg((HI_U32*)addr_REG, BT_CLIP0_L.u32);

    //addr_REG = Vou_GetIntfAbsAddr(enIntf,(HI_U32)(HI_UL)&(pVoReg->INTF_CLIP0_H.u32));
    addr_REG = (HI_UL) & (pVoReg->BT_CLIP0_H.u32);
    BT_CLIP0_H.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    BT_CLIP0_H.bits.clip_ch2 = pstClipData->u16ClipHigh_y;
    BT_CLIP0_H.bits.clip_ch1 = pstClipData->u16ClipHigh_cb;
    BT_CLIP0_H.bits.clip_ch0 = pstClipData->u16ClipHigh_cr;
    HAL_WriteReg((HI_U32*)addr_REG, BT_CLIP0_H.u32);

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetVtThdMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 uFieldMode)
{
    volatile U_DHD0_VTTHD     DHD0_VTTHD;
    volatile U_WBC_DHD0_CTRL  WBC_DHD0_CTRL;
    volatile  HI_UL addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_VTTHD.u32));
            DHD0_VTTHD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            // TODO
            DHD0_VTTHD.bits.thd1_mode = uFieldMode;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_VTTHD.u32);
            break;
        }

        case HAL_DISP_CHANNEL_WBC:
        {
            addr_REG = (HI_UL) & (pVoReg->WBC_DHD0_CTRL.u32);
            WBC_DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            // TODO
            WBC_DHD0_CTRL.bits.mode_out = uFieldMode;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_CTRL.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }
    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetVtThd(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 vtthd)
{
    //*
    volatile U_DHD0_VTTHD  DHD0_VTTHD;
    volatile  HI_UL addr_REG;

    switch (enChan)
    {

        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_VTTHD.u32));
            DHD0_VTTHD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            DHD0_VTTHD.bits.vtmgthd1 = vtthd;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_VTTHD.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error channel id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    //*/
    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetIntMask(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK VOINTMSK;
    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->VOINTMSK.u32));
    VOINTMSK.u32 = VOINTMSK.u32 | u32MaskEn;
    HAL_WriteReg((HI_U32*) & (pVoReg->VOINTMSK.u32), VOINTMSK.u32);


    return HI_TRUE;
}

HI_BOOL  HAL_DISP_ClrIntMask(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK VOINTMSK;

    /* Dispaly interrupt mask enable */
    VOINTMSK.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->VOINTMSK.u32));
    VOINTMSK.u32 = VOINTMSK.u32 & (~u32MaskEn);
    HAL_WriteReg((HI_U32*) & (pVoReg->VOINTMSK.u32), VOINTMSK.u32);


    return HI_TRUE;
}

HI_BOOL HAL_DISP_ClearIntStatus(HI_U32 u32IntMsk)
{
    /* read interrupt status */
    HAL_WriteReg((HI_U32*)&(pVoReg->VOMSKINTSTA.u32), u32IntMsk);
    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetClkGateEnable(HI_U32 u32Data)
{
    volatile U_VOCTRL VOCTRL;

    VOCTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VOCTRL.u32));
    VOCTRL.bits.vo_ck_gt_en = u32Data;
    HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VOCTRL.u32), VOCTRL.u32);
    return HI_TRUE;
}

HI_VOID HAL_DISP_DATE_OutCtrl(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32OutCtrl)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        U_DATE_COEFF21 DateOutCtrl, tmp;

        tmp.u32 = u32OutCtrl;
        DateOutCtrl.u32 = pVoReg->DATE_COEFF21.u32;
        DateOutCtrl.bits.dac0_in_sel = tmp.bits.dac0_in_sel;
        DateOutCtrl.bits.dac1_in_sel = tmp.bits.dac1_in_sel;
        DateOutCtrl.bits.dac2_in_sel = tmp.bits.dac2_in_sel;
        DateOutCtrl.bits.dac3_in_sel = tmp.bits.dac3_in_sel;
        DateOutCtrl.bits.dac4_in_sel = tmp.bits.dac4_in_sel;
        DateOutCtrl.bits.dac5_in_sel = tmp.bits.dac5_in_sel;
        pVoReg->DATE_COEFF21.u32 = DateOutCtrl.u32;
    }
    //*/
    return;
}

HI_BOOL HAL_DISP_SetDateCoeff(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    U_DATE_COEFF0 DATE_COEFF0;

    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        DATE_COEFF0.u32 = pVoReg->DATE_COEFF0.u32;
        DATE_COEFF0.u32 = u32Data;
        pVoReg->DATE_COEFF0.u32 = DATE_COEFF0.u32;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff22(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    volatile U_DATE_COEFF22 DATE_COEFF22;

    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        DATE_COEFF22.u32 = pVoReg->DATE_COEFF22.u32;
        DATE_COEFF22.bits.video_phase_delta = u32Data;
        pVoReg->DATE_COEFF22.u32 = DATE_COEFF22.u32;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff24(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        pVoReg->DATE_COEFF24.u32 = u32Data;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff50(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        pVoReg->DATE_COEFF50.u32 = u32Data;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff51(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        pVoReg->DATE_COEFF51.u32 = u32Data;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff52(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        pVoReg->DATE_COEFF52.u32 = u32Data;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff53(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        pVoReg->DATE_COEFF53.u32 = u32Data;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff54(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        pVoReg->DATE_COEFF54.u32 = u32Data;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff55(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        pVoReg->DATE_COEFF55.u32 = u32Data;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff57(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        pVoReg->DATE_COEFF57.u32 = u32Data;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_DISP_SetDateCoeff61(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_U32 u32Data)
{
    /*
    if (HAL_DISP_CHANNEL_DSD0 == enChan)
    {
        pVoReg->DATE_COEFF61.u32 = u32Data;
    }
    else
    {
        HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_VOID HAL_DISP_SetDateCoeffByIdx(HI_U32 u32Idx, HI_U32 u32Data)
{
    /*
    switch (u32Idx)
    {
        case 60:
        {
            pVoReg->DATE_COEFF60.u32 = u32Data;
            break;
        }

        case 73:
        {
            pVoReg->DATE_COEFF73.u32 = u32Data;
            break;
        }

        default:
            break;
    }
    //*/
}

inline HI_U16 VOU_HAL_Conver_DateCoef(HI_S16 s16Value)
{
    HI_U32 temp = 0;

    if(s16Value >= 0)
    {
        return s16Value;
    }
    else
    {
        temp = (-1)*s16Value;
        return ( ((~temp)+1)& 0x3FF);
    }
}

HI_VOID VOU_HAL_DISP_SetDateNotchCoeff(const HI_S16 s16Coef[])
{
/*
    HI_U16 u16Coef[12];
    int i = 0;

    for (i = 0; i < 12; i++)
    {
        u16Coef[i] = VOU_HAL_Conver_DateCoef(s16Coef[i]);
    }

    pVoReg->DATE_COEFF74.bits.coef_notch_1 = u16Coef[0];
    pVoReg->DATE_COEFF74.bits.coef_notch_2 = u16Coef[1];

    pVoReg->DATE_COEFF75.bits.coef_notch_3 = u16Coef[2];
    pVoReg->DATE_COEFF75.bits.coef_notch_4 = u16Coef[3];

    pVoReg->DATE_COEFF76.bits.coef_notch_5 = u16Coef[4];
    pVoReg->DATE_COEFF76.bits.coef_notch_6 = u16Coef[5];

    pVoReg->DATE_COEFF77.bits.coef_notch_7 = u16Coef[6];
    pVoReg->DATE_COEFF77.bits.coef_notch_8 = u16Coef[7];

    pVoReg->DATE_COEFF78.bits.coef_notch_9 = u16Coef[8];
    pVoReg->DATE_COEFF78.bits.coef_notch_10 = u16Coef[9];

    pVoReg->DATE_COEFF79.bits.coef_notch_11 = u16Coef[10];
    pVoReg->DATE_COEFF79.bits.coef_notch_12 = u16Coef[11];
    //*/

}

HI_BOOL HAL_DISP_SetIntMask1(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK1 VOINTMSK1;
    /* Dispaly interrupt mask enable */
    VOINTMSK1.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->VOINTMSK1.u32));
    VOINTMSK1.u32 = VOINTMSK1.u32 | u32MaskEn;

    HAL_WriteReg((HI_U32*) & (pVoReg->VOINTMSK1.u32), VOINTMSK1.u32);
    VOINTMSK1.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->VOINTMSK1.u32));

    return HI_TRUE;
}

HI_BOOL  HAL_DISP_ClrIntMask1(HI_U32 u32MaskEn)
{
    volatile U_VOINTMSK1 VOINTMSK1;

    /* Dispaly interrupt mask enable */
    VOINTMSK1.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->VOINTMSK1.u32));
    VOINTMSK1.u32 = VOINTMSK1.u32 & (~u32MaskEn);
    HAL_WriteReg((HI_U32*) & (pVoReg->VOINTMSK1.u32), VOINTMSK1.u32);

    return HI_TRUE;
}


HI_BOOL HAL_INTF_BT_SetDfirEn(HI_U32 dfir_en)
{
    U_INTF_BT_CTRL INTF_BT_CTRL;

    INTF_BT_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->INTF_BT_CTRL.u32));
    INTF_BT_CTRL.bits.dfir_en = dfir_en;
    HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->INTF_BT_CTRL.u32),INTF_BT_CTRL.u32);

    return HI_TRUE;
}

HI_VOID HAL_DISP_SetRegUp (HAL_DISP_OUTPUTCHANNEL_E enChan)
{
    volatile U_DHD0_CTRL DHD0_CTRL;
    volatile  HI_UL addr_REG;

    if (enChan > HAL_DISP_CHANNEL_DHD1)
    {
        printf("Error,HAL_DISP_SetRegUp Select Wrong CHANNEL ID\n");
        return ;
    }

    // If wbc?
    // TODO
    // If DHD0 or DHD1
    addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_CTRL.u32));
    DHD0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
    DHD0_CTRL.bits.regup = 0x1;
    HAL_WriteReg((HI_U32*)addr_REG, DHD0_CTRL.u32);
    return;
}

HI_BOOL HAL_VIDEO_SetLayerUpMode(HAL_DISP_LAYER_E enLayer, HI_U32 bUpMode)
{
    U_V0_CTRL V0_CTRL;
    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_CTRL.u32));
            V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_CTRL.bits.rgup_mode = bUpMode;
            HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }
    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetIfirMode(HAL_DISP_LAYER_E enLayer, HAL_IFIRMODE_E enMode)
{
    /*
    volatile U_V0_CTRL V0_CTRL;
    volatile  HI_UL addr_REG;
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->V0_CTRL.u32));
        V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CTRL.bits.ifir_mode = enMode;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/
    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetIfirCoef(HAL_DISP_LAYER_E enLayer, HI_S32 * s32Coef)
{
    /*
    volatile U_V0_IFIRCOEF01 V0_IFIRCOEF01;
    volatile U_V0_IFIRCOEF23 V0_IFIRCOEF23;
    volatile U_V0_IFIRCOEF45 V0_IFIRCOEF45;
    volatile U_V0_IFIRCOEF67 V0_IFIRCOEF67;

    volatile  HI_UL addr_REG;
    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->V0_IFIRCOEF01.u32));
        V0_IFIRCOEF01.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_IFIRCOEF01.bits.coef0 = s32Coef[0];
        V0_IFIRCOEF01.bits.coef1 = s32Coef[1];
        HAL_WriteReg((HI_U32*)addr_REG, V0_IFIRCOEF01.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->V0_IFIRCOEF23.u32));
        V0_IFIRCOEF23.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_IFIRCOEF23.bits.coef2 = s32Coef[2];
        V0_IFIRCOEF23.bits.coef3 = s32Coef[3];
        HAL_WriteReg((HI_U32*)addr_REG, V0_IFIRCOEF23.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->V0_IFIRCOEF45.u32));
        V0_IFIRCOEF45.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_IFIRCOEF45.bits.coef4 = s32Coef[4];
        V0_IFIRCOEF45.bits.coef5 = s32Coef[5];
        HAL_WriteReg((HI_U32*)addr_REG, V0_IFIRCOEF45.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->V0_IFIRCOEF67.u32));
        V0_IFIRCOEF67.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_IFIRCOEF67.bits.coef6 = s32Coef[6];
        V0_IFIRCOEF67.bits.coef7 = s32Coef[7];
        HAL_WriteReg((HI_U32*)addr_REG, V0_IFIRCOEF67.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    //*/


    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetHfirMode(HAL_DISP_LAYER_E enLayer, HAL_HFIRMODE_E enMode)
{
    volatile U_V0_HFIR_CTRL  V0_HFIR_CTRL;
    volatile U_WD_HPZME_CTRL WD_HPZME_CTRL;
    volatile U_WD_HCDS_CTRL  WD_HCDS_CTRL;

    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_HFIR_CTRL.u32));
            V0_HFIR_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HFIR_CTRL.bits.hfir_mode = enMode;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HFIR_CTRL.u32);
            break;
        }

        case HAL_DISP_LAYER_WBC:
        {
            // HPZME
            // TODO
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_HPZME_CTRL.u32));
            WD_HPZME_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_HPZME_CTRL.bits.ck_gt_en  = 1;
            WD_HPZME_CTRL.bits.hfir_mode = enMode;
            HAL_WriteReg((HI_U32*)addr_REG, WD_HPZME_CTRL.u32);

            // HCDS
            // TODO
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_HCDS_CTRL.u32));
            WD_HCDS_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_HCDS_CTRL.bits.ck_gt_en  = 1;
            WD_HCDS_CTRL.bits.hfir_en   = 1;
            WD_HCDS_CTRL.bits.hfir_mode = 0x0;
            HAL_WriteReg((HI_U32*)addr_REG, WD_HCDS_CTRL.u32);

            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetHfirCoef(HAL_DISP_LAYER_E enLayer, HI_S32* s32Coef)
{
    volatile U_V0_HFIRCOEF01 V0_HFIRCOEF01;
    volatile U_V0_HFIRCOEF23 V0_HFIRCOEF23;
    volatile U_V0_HFIRCOEF45 V0_HFIRCOEF45;
    volatile U_V0_HFIRCOEF67 V0_HFIRCOEF67;
    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_HFIRCOEF01.u32));
            V0_HFIRCOEF01.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HFIRCOEF01.bits.coef0 = s32Coef[0];
            V0_HFIRCOEF01.bits.coef1 = s32Coef[1];
            HAL_WriteReg((HI_U32*)addr_REG, V0_HFIRCOEF01.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_HFIRCOEF23.u32));
            V0_HFIRCOEF23.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HFIRCOEF23.bits.coef2 = s32Coef[2];
            V0_HFIRCOEF23.bits.coef3 = s32Coef[3];
            HAL_WriteReg((HI_U32*)addr_REG, V0_HFIRCOEF23.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_HFIRCOEF45.u32));
            V0_HFIRCOEF45.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HFIRCOEF45.bits.coef4 = s32Coef[4];
            V0_HFIRCOEF45.bits.coef5 = s32Coef[5];
            HAL_WriteReg((HI_U32*)addr_REG, V0_HFIRCOEF45.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_HFIRCOEF67.u32));
            V0_HFIRCOEF67.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HFIRCOEF67.bits.coef6 = s32Coef[6];
            V0_HFIRCOEF67.bits.coef7 = s32Coef[7];
            HAL_WriteReg((HI_U32*)addr_REG, V0_HFIRCOEF67.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


HI_BOOL HAL_VIDEO_SetLayerDispRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect)
{
    volatile U_V0_DFPOS V0_DFPOS;
    volatile U_V0_DLPOS V0_DLPOS;

    volatile U_G0_DFPOS G0_DFPOS;
    volatile U_G0_DLPOS G0_DLPOS;
    volatile  HI_UL addr_REG;

    switch (enLayer)
    {

        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_DFPOS.u32));
            V0_DFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_DFPOS.bits.disp_xfpos = pstRect->s32X;
            V0_DFPOS.bits.disp_yfpos = pstRect->s32Y;
            HAL_WriteReg((HI_U32*)addr_REG, V0_DFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_DLPOS.u32));
            V0_DLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_DLPOS.bits.disp_xlpos = pstRect->s32X + pstRect->u32Width - 1;
            V0_DLPOS.bits.disp_ylpos = pstRect->s32Y + pstRect->u32Height - 1;
            HAL_WriteReg((HI_U32*)addr_REG, V0_DLPOS.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_DFPOS.u32));
            G0_DFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_DFPOS.bits.disp_xfpos = pstRect->s32X;
            G0_DFPOS.bits.disp_yfpos = pstRect->s32Y;
            HAL_WriteReg((HI_U32*)addr_REG, G0_DFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_DLPOS.u32));
            G0_DLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_DLPOS.bits.disp_xlpos = pstRect->s32X + pstRect->u32Width - 1;
            G0_DLPOS.bits.disp_ylpos = pstRect->s32Y + pstRect->u32Height - 1;
            HAL_WriteReg((HI_U32*)addr_REG, G0_DLPOS.u32);
            break;
        }

        default:
        {
            printf("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }


    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetLayerVideoRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect)
{
    //*
    volatile U_V0_VFPOS V0_VFPOS;
    volatile U_V0_VLPOS V0_VLPOS;
    // TODO
    //volatile U_V0_CPOS V0_CPOS;

    volatile U_G0_VFPOS G0_VFPOS;
    volatile U_G0_VLPOS G0_VLPOS;
    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_VFPOS.u32));
            V0_VFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VFPOS.bits.video_xfpos = pstRect->s32X;
            V0_VFPOS.bits.video_yfpos = pstRect->s32Y;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_VLPOS.u32));
            V0_VLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VLPOS.bits.video_xlpos = pstRect->s32X + pstRect->u32Width - 1;
            V0_VLPOS.bits.video_ylpos = pstRect->s32Y + pstRect->u32Height - 1;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VLPOS.u32);

            // TODO
            //addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->V0_CPOS.u32));
            //V0_CPOS.u32 = HAL_ReadReg((HI_U32*)(HI_UL)addr_REG);
            //V0_CPOS.bits.src_xfpos = pstRect->s32X;
            //V0_CPOS.bits.src_xlpos = pstRect->s32X + pstRect->u32Width - 1;
            //HAL_WriteReg((HI_U32*)(HI_UL)addr_REG, V0_CPOS.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_VFPOS.u32));
            G0_VFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_VFPOS.bits.video_xfpos = pstRect->s32X;
            G0_VFPOS.bits.video_yfpos = pstRect->s32Y;
            HAL_WriteReg((HI_U32*)addr_REG, G0_VFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_VLPOS.u32));
            G0_VLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_VLPOS.bits.video_xlpos = pstRect->s32X + pstRect->u32Width - 1;
            G0_VLPOS.bits.video_ylpos = pstRect->s32Y + pstRect->u32Height - 1;
            HAL_WriteReg((HI_U32*)addr_REG, G0_VLPOS.u32);
            break;

        }

        default:
        {
            HAL_PRINT("Error layer id %d# found in %s,%s: L%d\n", enLayer, __FILE__, __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    //*/

    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetMultiAreaLAddr  (HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,HI_UL ulLAddr, HI_U16 stride)
{
    HI_UL ul_pxaddr_addr = 0;
    HI_UL ul_pxstride_addr = 0;
    U_VID_STRIDE VID_STRIDE;

    if (HAL_DISP_LAYER_VHD0 == enLayer || HAL_DISP_LAYER_VHD1 == enLayer)
    {
        // low 32 bits to VID_ADDR_L
        ul_pxaddr_addr   = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_ADDR_L));
        HAL_WriteReg((HI_U32*)ul_pxaddr_addr, GetLowAddr(ulLAddr));

        // High 32 bits to  VID_ADDR_H
        ul_pxaddr_addr   = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_ADDR_H));
        HAL_WriteReg((HI_U32*)ul_pxaddr_addr, GetHighAddr(ulLAddr));

        ul_pxstride_addr = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_STRIDE.u32));
        VID_STRIDE.u32 = HAL_ReadReg((HI_U32*)ul_pxstride_addr);
        VID_STRIDE.bits.lm_stride = stride;
        HAL_WriteReg((HI_U32*)ul_pxstride_addr, VID_STRIDE.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetMultiAreaCAddr  (HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,HI_UL ulCAddr, HI_U16 stride)
{
    HI_UL uL_pxaddr_addr = 0;
    HI_UL uL_pxstride_addr = 0;
    U_VID_STRIDE VID_STRIDE;

    if (HAL_DISP_LAYER_VHD0 == enLayer || HAL_DISP_LAYER_VHD1 == enLayer)
    {
        // Low 32 bits to VID_CADDR_L
        uL_pxaddr_addr   = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_CADDR_L));
        HAL_WriteReg((HI_U32*)uL_pxaddr_addr, GetLowAddr(ulCAddr));

        // High 32 bits to VID_CADDR_H
        uL_pxaddr_addr   = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_CADDR_H));
        HAL_WriteReg((HI_U32*)uL_pxaddr_addr, GetHighAddr(ulCAddr));

        uL_pxstride_addr = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_STRIDE.u32)) ;
        VID_STRIDE.u32 = HAL_ReadReg((HI_U32*)uL_pxstride_addr);
        VID_STRIDE.bits.chm_stride = stride;
        HAL_WriteReg((HI_U32*)uL_pxstride_addr, VID_STRIDE.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetMultiAreaEnable(HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,HI_U32 bEnable)
{
    /*
    volatile  HI_UL addr_REG;
    volatile U_V0_P0CTRL V0_P0CTRL;
    HI_UL pstP0CTRL;
    HI_U32 u32MaxAreaNum;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        u32MaxAreaNum = HAL_LAYER_GetLayerMaxAreaNum(enLayer);
        if(u32AreaNum < u32MaxAreaNum)
        {
            pstP0CTRL = (HI_UL)&(pVoReg->V0_P0CTRL.u32) + u32AreaNum * MULTI_AREA_CFG_LEN;
            addr_REG = Vou_GetAbsAddr(enLayer, pstP0CTRL);

            V0_P0CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_P0CTRL.bits.p0_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_P0CTRL.u32);
        }
        else
        {
            HAL_PRINT("Error area num found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetMultiAreaReso(HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,
                                              HI_U32 u32Width)
{
    /*
    U_V0_P0RESO  V0_P0RESO;
    HI_UL pstReso;
    volatile  HI_UL addr_REG;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        pstReso  = (HI_UL)&(pVoReg->V0_P0RESO.u32)  + u32AreaNum * MULTI_AREA_CFG_LEN;
        //cfg resolution
        addr_REG = Vou_GetAbsAddr(enLayer,pstReso);
        V0_P0RESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_P0RESO.bits.w = u32Width -1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0RESO.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_VIDEO_SetMultiAreaRect(HAL_DISP_LAYER_E enLayer,HI_U32 u32AreaNum,RECT_S *pstVideoAreaRect)
{
    /*
    U_V0_P0VFPOS V0_P0VFPOS;
    U_V0_P0VLPOS V0_P0VLPOS;

    HI_UL pstVfpos;
    HI_UL pstVlpos;

    volatile  HI_UL addr_REG;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        pstVfpos = (HI_UL)&(pVoReg->V0_P0VFPOS.u32) + u32AreaNum * MULTI_AREA_CFG_LEN;
        pstVlpos = (HI_UL)&(pVoReg->V0_P0VLPOS.u32) + u32AreaNum * MULTI_AREA_CFG_LEN;

        //cfg fpos
        addr_REG = Vou_GetAbsAddr(enLayer,pstVfpos);
        V0_P0VFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_P0VFPOS.bits.video_xfpos = pstVideoAreaRect->s32X;
        V0_P0VFPOS.bits.video_yfpos = pstVideoAreaRect->s32Y;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0VFPOS.u32);

        //cfg lpos
        addr_REG = Vou_GetAbsAddr(enLayer,pstVlpos);
        V0_P0VLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_P0VLPOS.bits.video_xlpos = pstVideoAreaRect->s32X + pstVideoAreaRect->u32Width -1;
        V0_P0VLPOS.bits.video_ylpos = pstVideoAreaRect->s32Y + pstVideoAreaRect->u32Height -1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_P0VLPOS.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/


    return HI_TRUE;
}
HI_BOOL HAL_LAYER_EnableLayer(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)

{
    volatile  U_V0_CTRL  V0_CTRL;
    volatile  U_G0_CTRL  G0_CTRL;
    volatile  HI_UL      addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_CTRL.u32));
            //osal_printk("+++V1_CTRL:%p,%s:%d+++\n",(HI_U32*)addr_REG,__FUNCTION__,__LINE__);
            V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_CTRL.bits.surface_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32);
            break;
        }
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_CTRL.u32));
            G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_CTRL.bits.surface_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32);
            break;
        }
        default:
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetLayerDataFmt(HAL_DISP_LAYER_E enLayer,
                                            HAL_DISP_PIXEL_FORMAT_E  enDataFmt)
{
    volatile U_VID_SRC_INFO VID_SRC_INFO;
    volatile U_GFX_SRC_INFO GFX_SRC_INFO;
    volatile  HI_UL addr_REG;

    // only for v0 and v1
    if (HAL_DISP_LAYER_VHD0 == enLayer || HAL_DISP_LAYER_VHD1 == enLayer)
    {
        addr_REG = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_SRC_INFO.u32));
        VID_SRC_INFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        VID_SRC_INFO.bits.data_type = enDataFmt;
        HAL_WriteReg((HI_U32*)addr_REG, VID_SRC_INFO.u32);
    }
    else if (HAL_DISP_LAYER_GFX0 == enLayer || HAL_DISP_LAYER_GFX1 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_SRC_INFO.u32));
        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_SRC_INFO.bits.ifmt = enDataFmt;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_SRC_INFO.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in FILE:%s,FUNC:%s,LINE%d\n", __FILE__, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }


    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetCscCoef(HAL_DISP_LAYER_E enLayer, CscCoef_S *pstCscCoef)
{
    //*
    // for V0
    volatile U_HIHDR_V_YUV2RGB_COEF00 HIHDR_V_YUV2RGB_COEF00;
    volatile U_HIHDR_V_YUV2RGB_COEF01 HIHDR_V_YUV2RGB_COEF01;
    volatile U_HIHDR_V_YUV2RGB_COEF02 HIHDR_V_YUV2RGB_COEF02;
    volatile U_HIHDR_V_YUV2RGB_COEF03 HIHDR_V_YUV2RGB_COEF03;
    volatile U_HIHDR_V_YUV2RGB_COEF04 HIHDR_V_YUV2RGB_COEF04;
    volatile U_HIHDR_V_YUV2RGB_COEF05 HIHDR_V_YUV2RGB_COEF05;
    volatile U_HIHDR_V_YUV2RGB_COEF06 HIHDR_V_YUV2RGB_COEF06;
    volatile U_HIHDR_V_YUV2RGB_COEF07 HIHDR_V_YUV2RGB_COEF07;
    volatile U_HIHDR_V_YUV2RGB_COEF08 HIHDR_V_YUV2RGB_COEF08;
    //volatile U_HIHDR_V_YUV2RGB_COEF10 HIHDR_V_YUV2RGB_COEF10;
    //volatile U_HIHDR_V_YUV2RGB_COEF11 HIHDR_V_YUV2RGB_COEF11;
    //volatile U_HIHDR_V_YUV2RGB_COEF12 HIHDR_V_YUV2RGB_COEF12;
    //volatile U_HIHDR_V_YUV2RGB_COEF13 HIHDR_V_YUV2RGB_COEF13;
    //volatile U_HIHDR_V_YUV2RGB_COEF14 HIHDR_V_YUV2RGB_COEF14;
    //volatile U_HIHDR_V_YUV2RGB_COEF15 HIHDR_V_YUV2RGB_COEF15;
    //volatile U_HIHDR_V_YUV2RGB_COEF16 HIHDR_V_YUV2RGB_COEF16;
    //volatile U_HIHDR_V_YUV2RGB_COEF17 HIHDR_V_YUV2RGB_COEF17;
    //volatile U_HIHDR_V_YUV2RGB_COEF18 HIHDR_V_YUV2RGB_COEF18;

    volatile U_HIHDR_V_YUV2RGB_SCALE2P HIHDR_V_YUV2RGB_SCALE2P;

    volatile U_HIHDR_V_YUV2RGB_IN_DC00  HIHDR_V_YUV2RGB_IN_DC00;
    volatile U_HIHDR_V_YUV2RGB_IN_DC01  HIHDR_V_YUV2RGB_IN_DC01;
    volatile U_HIHDR_V_YUV2RGB_IN_DC02  HIHDR_V_YUV2RGB_IN_DC02;
    volatile U_HIHDR_V_YUV2RGB_OUT_DC00 HIHDR_V_YUV2RGB_OUT_DC00;
    volatile U_HIHDR_V_YUV2RGB_OUT_DC01 HIHDR_V_YUV2RGB_OUT_DC01;
    volatile U_HIHDR_V_YUV2RGB_OUT_DC02 HIHDR_V_YUV2RGB_OUT_DC02;

    //volatile U_HIHDR_V_YUV2RGB_IN_DC10  HIHDR_V_YUV2RGB_IN_DC10;
    //volatile U_HIHDR_V_YUV2RGB_IN_DC11  HIHDR_V_YUV2RGB_IN_DC11;
    //volatile U_HIHDR_V_YUV2RGB_IN_DC12  HIHDR_V_YUV2RGB_IN_DC12;
    //volatile U_HIHDR_V_YUV2RGB_OUT_DC10 HIHDR_V_YUV2RGB_OUT_DC10;
    //volatile U_HIHDR_V_YUV2RGB_OUT_DC11 HIHDR_V_YUV2RGB_OUT_DC11;
    //volatile U_HIHDR_V_YUV2RGB_OUT_DC12 HIHDR_V_YUV2RGB_OUT_DC12;

    volatile U_HIHDR_V_YUV2RGB_MIN HIHDR_V_YUV2RGB_MIN;
    volatile U_HIHDR_V_YUV2RGB_MAX HIHDR_V_YUV2RGB_MAX;

    // for V1
    volatile U_V1_CSC_IDC         V1_CSC_IDC;
    volatile U_V1_CSC_ODC         V1_CSC_ODC;
    volatile U_V1_CSC_IODC        V1_CSC_IODC;
    volatile U_V1_CSC_P0 V1_CSC_P0;
    volatile U_V1_CSC_P1 V1_CSC_P1;
    volatile U_V1_CSC_P2 V1_CSC_P2;
    volatile U_V1_CSC_P3 V1_CSC_P3;
    volatile U_V1_CSC_P4 V1_CSC_P4;

    // for G0
    volatile U_HIHDR_G_RGB2YUV_COEF0 HIHDR_G_RGB2YUV_COEF0;
    volatile U_HIHDR_G_RGB2YUV_COEF1 HIHDR_G_RGB2YUV_COEF1;
    volatile U_HIHDR_G_RGB2YUV_COEF2 HIHDR_G_RGB2YUV_COEF2;
    volatile U_HIHDR_G_RGB2YUV_COEF3 HIHDR_G_RGB2YUV_COEF3;
    volatile U_HIHDR_G_RGB2YUV_COEF4 HIHDR_G_RGB2YUV_COEF4;
    volatile U_HIHDR_G_RGB2YUV_COEF5 HIHDR_G_RGB2YUV_COEF5;
    volatile U_HIHDR_G_RGB2YUV_COEF6 HIHDR_G_RGB2YUV_COEF6;
    volatile U_HIHDR_G_RGB2YUV_COEF7 HIHDR_G_RGB2YUV_COEF7;
    volatile U_HIHDR_G_RGB2YUV_COEF8 HIHDR_G_RGB2YUV_COEF8;

    volatile U_HIHDR_G_RGB2YUV_IDC0 HIHDR_G_RGB2YUV_IDC0;
    volatile U_HIHDR_G_RGB2YUV_IDC1 HIHDR_G_RGB2YUV_IDC1;
    volatile U_HIHDR_G_RGB2YUV_IDC2 HIHDR_G_RGB2YUV_IDC2;
    volatile U_HIHDR_G_RGB2YUV_ODC0 HIHDR_G_RGB2YUV_ODC0;
    volatile U_HIHDR_G_RGB2YUV_ODC1 HIHDR_G_RGB2YUV_ODC1;
    volatile U_HIHDR_G_RGB2YUV_ODC2 HIHDR_G_RGB2YUV_ODC2;


    // for G1
    volatile U_G1_CSC_IDC         G1_CSC_IDC;
    volatile U_G1_CSC_ODC         G1_CSC_ODC;
    volatile U_G1_CSC_IODC        G1_CSC_IODC;

    volatile U_G1_CSC_P0 G1_CSC_P0;
    volatile U_G1_CSC_P1 G1_CSC_P1;
    volatile U_G1_CSC_P2 G1_CSC_P2;
    volatile U_G1_CSC_P3 G1_CSC_P3;
    volatile U_G1_CSC_P4 G1_CSC_P4;

    // for WD
#if 0
    volatile U_WDHDR_RGB2YUV_COEF0 WDHDR_RGB2YUV_COEF0;
    volatile U_WDHDR_RGB2YUV_COEF1 WDHDR_RGB2YUV_COEF1;
    volatile U_WDHDR_RGB2YUV_COEF2 WDHDR_RGB2YUV_COEF2;
    volatile U_WDHDR_RGB2YUV_COEF3 WDHDR_RGB2YUV_COEF3;
    volatile U_WDHDR_RGB2YUV_COEF4 WDHDR_RGB2YUV_COEF4;
    volatile U_WDHDR_RGB2YUV_COEF5 WDHDR_RGB2YUV_COEF5;
    volatile U_WDHDR_RGB2YUV_COEF6 WDHDR_RGB2YUV_COEF6;
    volatile U_WDHDR_RGB2YUV_COEF7 WDHDR_RGB2YUV_COEF7;
    volatile U_WDHDR_RGB2YUV_COEF8 WDHDR_RGB2YUV_COEF8;

    volatile U_WDHDR_RGB2YUV_SCALE2P WDHDR_RGB2YUV_SCALE2P;

    volatile U_WDHDR_RGB2YUV_IDC0 WDHDR_RGB2YUV_IDC0;
    volatile U_WDHDR_RGB2YUV_IDC1 WDHDR_RGB2YUV_IDC1;
    volatile U_WDHDR_RGB2YUV_IDC2 WDHDR_RGB2YUV_IDC2;
    volatile U_WDHDR_RGB2YUV_ODC0 WDHDR_RGB2YUV_ODC0;
    volatile U_WDHDR_RGB2YUV_ODC1 WDHDR_RGB2YUV_ODC1;
    volatile U_WDHDR_RGB2YUV_ODC2 WDHDR_RGB2YUV_ODC2;

    volatile U_WDHDR_RGB2YUV_MIN WDHDR_RGB2YUV_MIN;
    volatile U_WDHDR_RGB2YUV_MAX WDHDR_RGB2YUV_MAX;
#endif

    volatile  HI_UL addr_REG;

    if (HAL_DISP_LAYER_VHD0== enLayer)
    {
        //osal_printk("+++[Func:%s][Line:%d] Hal csc coef configed+++\n", __FUNCTION__, __LINE__);
        // coef 0x:00 01 02 03 04 05 06 07 08
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF00.u32);
        HIHDR_V_YUV2RGB_COEF00.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF00.bits.hihdr_y2r_coef00 = HAL_Conver_CscCoef_NEW(pstCscCoef->new_csc_coef00);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF00.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF01.u32);
        HIHDR_V_YUV2RGB_COEF01.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF01.bits.hihdr_y2r_coef01 = HAL_Conver_CscCoef_NEW(pstCscCoef->new_csc_coef01);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF01.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF02.u32);
        HIHDR_V_YUV2RGB_COEF02.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF02.bits.hihdr_y2r_coef02 = HAL_Conver_CscCoef_NEW(pstCscCoef->new_csc_coef02);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF02.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF03.u32);
        HIHDR_V_YUV2RGB_COEF03.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF03.bits.hihdr_y2r_coef03 = HAL_Conver_CscCoef_NEW(pstCscCoef->new_csc_coef03);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF03.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF04.u32);
        HIHDR_V_YUV2RGB_COEF04.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF04.bits.hihdr_y2r_coef04 = HAL_Conver_CscCoef_NEW(pstCscCoef->new_csc_coef04);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF04.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF05.u32);
        HIHDR_V_YUV2RGB_COEF05.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF05.bits.hihdr_y2r_coef05 = HAL_Conver_CscCoef_NEW(pstCscCoef->new_csc_coef05);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF05.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF06.u32);
        HIHDR_V_YUV2RGB_COEF06.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF06.bits.hihdr_y2r_coef06 = HAL_Conver_CscCoef_NEW(pstCscCoef->new_csc_coef06);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF06.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF07.u32);
        HIHDR_V_YUV2RGB_COEF07.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF07.bits.hihdr_y2r_coef07 = HAL_Conver_CscCoef_NEW(pstCscCoef->new_csc_coef07);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF07.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF08.u32);
        HIHDR_V_YUV2RGB_COEF08.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF08.bits.hihdr_y2r_coef08 = HAL_Conver_CscCoef_NEW(pstCscCoef->new_csc_coef08);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF08.u32);

        // coef 1x:10 11 12 13 14 15 16 17 18
    #if 0
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF10.u32);
        HIHDR_V_YUV2RGB_COEF10.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF10.bits.hihdr_y2r_coef10 = pstCscCoef->new_csc_coef10;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF10.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF11.u32);
        HIHDR_V_YUV2RGB_COEF11.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF11.bits.hihdr_y2r_coef11 = pstCscCoef->new_csc_coef11;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF11.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF12.u32);
        HIHDR_V_YUV2RGB_COEF12.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF12.bits.hihdr_y2r_coef12 = pstCscCoef->new_csc_coef12;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF12.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF13.u32);
        HIHDR_V_YUV2RGB_COEF13.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF13.bits.hihdr_y2r_coef13 = pstCscCoef->new_csc_coef13;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF13.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF14.u32);
        HIHDR_V_YUV2RGB_COEF14.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF14.bits.hihdr_y2r_coef14 = pstCscCoef->new_csc_coef14;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF14.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF15.u32);
        HIHDR_V_YUV2RGB_COEF15.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF15.bits.hihdr_y2r_coef15 = pstCscCoef->new_csc_coef15;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF15.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF16.u32);
        HIHDR_V_YUV2RGB_COEF16.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF16.bits.hihdr_y2r_coef16 = pstCscCoef->new_csc_coef16;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF16.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF17.u32);
        HIHDR_V_YUV2RGB_COEF17.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF17.bits.hihdr_y2r_coef17 = pstCscCoef->new_csc_coef17;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF17.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF18.u32);
        HIHDR_V_YUV2RGB_COEF18.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_COEF18.bits.hihdr_y2r_coef18 = pstCscCoef->new_csc_coef18;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_COEF18.u32);
    #endif

        // scale2p
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_SCALE2P.u32);
        HIHDR_V_YUV2RGB_SCALE2P.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_SCALE2P.bits.hihdr_y2r_v0_scale2p = pstCscCoef->new_csc_scale2p;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_SCALE2P.u32);


        // 0x:idc00 idc01 idc02
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_IN_DC00.u32);
        HIHDR_V_YUV2RGB_IN_DC00.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_IN_DC00.bits.hihdr_y2r_idc00 = HAL_GetXDC_BUMA(pstCscCoef->new_csc_in_dc00);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_IN_DC00.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_IN_DC01.u32);
        HIHDR_V_YUV2RGB_IN_DC01.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_IN_DC01.bits.hihdr_y2r_idc01 = HAL_GetXDC_BUMA(pstCscCoef->new_csc_in_dc01);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_IN_DC01.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_IN_DC02.u32);
        HIHDR_V_YUV2RGB_IN_DC02.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_IN_DC02.bits.hihdr_y2r_idc02 = HAL_GetXDC_BUMA(pstCscCoef->new_csc_in_dc02);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_IN_DC02.u32);

        // 0x:odc00 odc01 odc02
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_OUT_DC00.u32);
        HIHDR_V_YUV2RGB_OUT_DC00.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_OUT_DC00.bits.hihdr_y2r_odc00 = HAL_GetODC_BUMA_NEW(pstCscCoef->new_csc_out_dc00);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_OUT_DC00.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_OUT_DC01.u32);
        HIHDR_V_YUV2RGB_OUT_DC01.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_OUT_DC01.bits.hihdr_y2r_odc01 = HAL_GetODC_BUMA_NEW(pstCscCoef->new_csc_out_dc01);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_OUT_DC01.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_OUT_DC02.u32);
        HIHDR_V_YUV2RGB_OUT_DC02.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_OUT_DC02.bits.hihdr_y2r_odc02 = HAL_GetODC_BUMA_NEW(pstCscCoef->new_csc_out_dc02);
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_OUT_DC02.u32);


        // 1x:idc10 idc11 idc12
    #if 0
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_IN_DC10.u32);
        HIHDR_V_YUV2RGB_IN_DC10.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_IN_DC10.bits.hihdr_y2r_idc10 = pstCscCoef->new_csc_in_dc10;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_IN_DC10.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_IN_DC11.u32);
        HIHDR_V_YUV2RGB_IN_DC11.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_IN_DC11.bits.hihdr_y2r_idc11 = pstCscCoef->new_csc_in_dc11;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_IN_DC11.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_IN_DC12.u32);
        HIHDR_V_YUV2RGB_IN_DC12.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_IN_DC12.bits.hihdr_y2r_idc12 = pstCscCoef->new_csc_in_dc12;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_IN_DC12.u32);

        // 1x:odc10 odc11 odc12
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_OUT_DC10.u32);
        HIHDR_V_YUV2RGB_OUT_DC10.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_OUT_DC10.bits.hihdr_y2r_odc10 = pstCscCoef->new_csc_out_dc10;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_OUT_DC10.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_OUT_DC11.u32);
        HIHDR_V_YUV2RGB_OUT_DC11.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_OUT_DC11.bits.hihdr_y2r_odc11 = pstCscCoef->new_csc_out_dc11;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_OUT_DC11.u32);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_OUT_DC12.u32);
        HIHDR_V_YUV2RGB_OUT_DC12.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        HIHDR_V_YUV2RGB_OUT_DC12.bits.hihdr_y2r_odc12 = pstCscCoef->new_csc_out_dc12;
        HAL_WriteReg((HI_U32*)addr_REG, HIHDR_V_YUV2RGB_OUT_DC12.u32);
    #endif

        // clip min and max
        HIHDR_V_YUV2RGB_MIN.u32 = HAL_ReadReg((HI_U32*) & (pVoHippReg->HIHDR_V_YUV2RGB_MIN.u32));
        HIHDR_V_YUV2RGB_MIN.bits.hihdr_y2r_v0_clip_min = pstCscCoef->new_csc_clip_min;
        HAL_WriteReg((HI_U32*) & (pVoHippReg->HIHDR_V_YUV2RGB_MIN.u32), HIHDR_V_YUV2RGB_MIN.u32);

        HIHDR_V_YUV2RGB_MAX.u32 = HAL_ReadReg((HI_U32*) & (pVoHippReg->HIHDR_V_YUV2RGB_MAX.u32));
        HIHDR_V_YUV2RGB_MAX.bits.hihdr_y2r_v0_clip_max = pstCscCoef->new_csc_clip_max;
        HAL_WriteReg((HI_U32*) & (pVoHippReg->HIHDR_V_YUV2RGB_MAX.u32), HIHDR_V_YUV2RGB_MAX.u32);

        // print value of register
    #if 0
        // read coef00 01 02 03 04 05 06 07 08
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF00.u32);
        HIHDR_V_YUV2RGB_COEF00.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF01.u32);
        HIHDR_V_YUV2RGB_COEF01.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF02.u32);
        HIHDR_V_YUV2RGB_COEF02.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF03.u32);
        HIHDR_V_YUV2RGB_COEF03.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF04.u32);
        HIHDR_V_YUV2RGB_COEF04.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF05.u32);
        HIHDR_V_YUV2RGB_COEF05.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF06.u32);
        HIHDR_V_YUV2RGB_COEF06.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF07.u32);
        HIHDR_V_YUV2RGB_COEF07.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_COEF08.u32);
        HIHDR_V_YUV2RGB_COEF08.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        // read idc 0 1 2
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_IN_DC00.u32);
        HIHDR_V_YUV2RGB_IN_DC00.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_IN_DC01.u32);
        HIHDR_V_YUV2RGB_IN_DC01.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_IN_DC02.u32);
        HIHDR_V_YUV2RGB_IN_DC02.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        // read odc 0 1 2
        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_OUT_DC00.u32);
        HIHDR_V_YUV2RGB_OUT_DC00.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_OUT_DC01.u32);
        HIHDR_V_YUV2RGB_OUT_DC01.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        addr_REG = (HI_UL) & (pVoHippReg->HIHDR_V_YUV2RGB_OUT_DC02.u32);
        HIHDR_V_YUV2RGB_OUT_DC02.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        // print coef, idc, odc
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_COEF00.bits.hihdr_y2r_coef00 = %d \n",__FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_COEF00.bits.hihdr_y2r_coef00);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_COEF01.bits.hihdr_y2r_coef01 = %d \n",__FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_COEF01.bits.hihdr_y2r_coef01);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_COEF02.bits.hihdr_y2r_coef02 = %d \n",__FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_COEF02.bits.hihdr_y2r_coef02);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_COEF03.bits.hihdr_y2r_coef03 = %d \n",__FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_COEF03.bits.hihdr_y2r_coef03);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_COEF04.bits.hihdr_y2r_coef04 = %d \n",__FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_COEF04.bits.hihdr_y2r_coef04);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_COEF05.bits.hihdr_y2r_coef05 = %d \n",__FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_COEF05.bits.hihdr_y2r_coef05);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_COEF06.bits.hihdr_y2r_coef06 = %d \n",__FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_COEF06.bits.hihdr_y2r_coef06);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_COEF07.bits.hihdr_y2r_coef07 = %d \n",__FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_COEF07.bits.hihdr_y2r_coef07);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_COEF08.bits.hihdr_y2r_coef08 = %d \n",__FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_COEF08.bits.hihdr_y2r_coef08);

        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_IN_DC00.bits.hihdr_y2r_idc00 = %d \n", __FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_IN_DC00.bits.hihdr_y2r_idc00);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_IN_DC01.bits.hihdr_y2r_idc01 = %d \n", __FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_IN_DC01.bits.hihdr_y2r_idc01);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_IN_DC02.bits.hihdr_y2r_idc02 = %d \n", __FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_IN_DC02.bits.hihdr_y2r_idc02);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_OUT_DC00.bits.hihdr_y2r_odc00 = %d \n", __FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_OUT_DC00.bits.hihdr_y2r_odc00);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_OUT_DC01.bits.hihdr_y2r_odc01 = %d \n", __FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_OUT_DC01.bits.hihdr_y2r_odc01);
        osal_printk("[Func:%s][Line:%d] HIHDR_V_YUV2RGB_OUT_DC02.bits.hihdr_y2r_odc02 = %d \n", __FUNCTION__, __LINE__, HIHDR_V_YUV2RGB_OUT_DC02.bits.hihdr_y2r_odc02);
    #endif


    }
    else if (HAL_DISP_LAYER_VHD1 == enLayer)
    {
        // coef
        V1_CSC_P0.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P0.u32));
        V1_CSC_P0.bits.cscp00 = HAL_Conver_CscCoef(pstCscCoef->csc_coef00);
        V1_CSC_P0.bits.cscp01 = HAL_Conver_CscCoef(pstCscCoef->csc_coef01);
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC_P0.u32), V1_CSC_P0.u32);

        V1_CSC_P1.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P1.u32));
        V1_CSC_P1.bits.cscp02 = HAL_Conver_CscCoef(pstCscCoef->csc_coef02);
        V1_CSC_P1.bits.cscp10 = HAL_Conver_CscCoef(pstCscCoef->csc_coef10);
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC_P1.u32), V1_CSC_P1.u32);

        V1_CSC_P2.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P2.u32));
        V1_CSC_P2.bits.cscp11 = HAL_Conver_CscCoef(pstCscCoef->csc_coef11);
        V1_CSC_P2.bits.cscp12 = HAL_Conver_CscCoef(pstCscCoef->csc_coef12);
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC_P2.u32), V1_CSC_P2.u32);

        V1_CSC_P3.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P3.u32));
        V1_CSC_P3.bits.cscp20 = HAL_Conver_CscCoef(pstCscCoef->csc_coef20);
        V1_CSC_P3.bits.cscp21 = HAL_Conver_CscCoef(pstCscCoef->csc_coef21);
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC_P3.u32), V1_CSC_P3.u32);

        V1_CSC_P4.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P4.u32));
        V1_CSC_P4.bits.cscp22 = HAL_Conver_CscCoef(pstCscCoef->csc_coef22);
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC_P4.u32), V1_CSC_P4.u32);

        // xdc
        V1_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_IDC.u32));
        V1_CSC_IDC.bits.cscidc1  = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc1);
        V1_CSC_IDC.bits.cscidc0  = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc0);
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC_IDC.u32), V1_CSC_IDC.u32);

        V1_CSC_ODC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_ODC.u32));
        V1_CSC_ODC.bits.cscodc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc1);
        V1_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc0);
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC_ODC.u32), V1_CSC_ODC.u32);

        V1_CSC_IODC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_IODC.u32));
        V1_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc2);
        V1_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc2);
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC_IODC.u32), V1_CSC_IODC.u32);

    #if 0
        // coef
        V1_CSC1_P0.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC1_P0.u32));
        V1_CSC1_P0.bits.cscp00 = pstCscCoef->csc_coef00;
        V1_CSC1_P0.bits.cscp01 = pstCscCoef->csc_coef01;
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC1_P0.u32), V1_CSC1_P0.u32);

        V1_CSC1_P1.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC1_P1.u32));
        V1_CSC1_P1.bits.cscp02 = pstCscCoef->csc_coef02;
        V1_CSC1_P1.bits.cscp10 = pstCscCoef->csc_coef10;
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC1_P1.u32), V1_CSC1_P1.u32);

        V1_CSC1_P2.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC1_P2.u32));
        V1_CSC1_P2.bits.cscp11 = pstCscCoef->csc_coef11;
        V1_CSC1_P2.bits.cscp12 = pstCscCoef->csc_coef12;
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC1_P2.u32), V1_CSC1_P2.u32);

        V1_CSC1_P3.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC1_P3.u32));
        V1_CSC1_P3.bits.cscp20 = pstCscCoef->csc_coef20;
        V1_CSC1_P3.bits.cscp21 = pstCscCoef->csc_coef21;
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC1_P3.u32), V1_CSC1_P3.u32);

        V1_CSC1_P4.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC1_P4.u32));
        V1_CSC1_P4.bits.cscp22 = pstCscCoef->csc_coef22;
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC1_P4.u32), V1_CSC1_P4.u32);

        // xdc
        V1_CSC1_IDC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC1_IDC.u32));
        V1_CSC1_IDC.bits.cscidc1  = pstCscCoef->csc_in_dc1;
        V1_CSC1_IDC.bits.cscidc0  = pstCscCoef->csc_in_dc0;
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC1_IDC.u32), V1_CSC1_IDC.u32);

        V1_CSC1_ODC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC1_ODC.u32));
        V1_CSC1_ODC.bits.cscodc1 = pstCscCoef->csc_out_dc1;
        V1_CSC1_ODC.bits.cscodc0 = pstCscCoef->csc_out_dc0;
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC1_ODC.u32), V1_CSC1_ODC.u32);

        V1_CSC1_IODC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC1_IODC.u32));
        V1_CSC1_IODC.bits.cscodc2 = pstCscCoef->csc_out_dc2;
        V1_CSC1_IODC.bits.cscidc2 = pstCscCoef->csc_in_dc2;
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC1_IODC.u32), V1_CSC1_IODC.u32);

    #endif
    #if 0

        V1_CSC_P0.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P0.u32));
        V1_CSC_P1.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P1.u32));
        V1_CSC_P2.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P2.u32));
        V1_CSC_P3.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P3.u32));
        V1_CSC_P4.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_P4.u32));
        V1_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_IDC.u32));
        V1_CSC_ODC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_ODC.u32));
        V1_CSC_IODC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_IODC.u32));
        osal_printk("[Func:%s][Line:%d] V1_CSC_P0.bits.cscp00 = %d \n",__FUNCTION__, __LINE__, V1_CSC_P0.bits.cscp00);
        osal_printk("[Func:%s][Line:%d] V1_CSC_P0.bits.cscp01 = %d \n",__FUNCTION__, __LINE__, V1_CSC_P0.bits.cscp01);
        osal_printk("[Func:%s][Line:%d] V1_CSC_P1.bits.cscp02 = %d \n",__FUNCTION__, __LINE__, V1_CSC_P1.bits.cscp02);
        osal_printk("[Func:%s][Line:%d] V1_CSC_P1.bits.cscp10 = %d \n",__FUNCTION__, __LINE__, V1_CSC_P1.bits.cscp10);
        osal_printk("[Func:%s][Line:%d] V1_CSC_P2.bits.cscp11 = %d \n",__FUNCTION__, __LINE__, V1_CSC_P2.bits.cscp11);
        osal_printk("[Func:%s][Line:%d] V1_CSC_P2.bits.cscp12 = %d \n",__FUNCTION__, __LINE__, V1_CSC_P2.bits.cscp12);
        osal_printk("[Func:%s][Line:%d] V1_CSC_P3.bits.cscp10 = %d \n",__FUNCTION__, __LINE__, V1_CSC_P3.bits.cscp20);
        osal_printk("[Func:%s][Line:%d] V1_CSC_P3.bits.cscp11 = %d \n",__FUNCTION__, __LINE__, V1_CSC_P3.bits.cscp21);
        osal_printk("[Func:%s][Line:%d] V1_CSC_P4.bits.cscp12 = %d \n",__FUNCTION__, __LINE__, V1_CSC_P4.bits.cscp22);


        osal_printk("[Func:%s][Line:%d] V1_CSC_IDC.bits.cscidc0  = %d \n", __FUNCTION__, __LINE__, V1_CSC_IDC.bits.cscidc0 );
        osal_printk("[Func:%s][Line:%d] V1_CSC_IDC.bits.cscidc1  = %d \n", __FUNCTION__, __LINE__, V1_CSC_IDC.bits.cscidc1 );
        osal_printk("[Func:%s][Line:%d] V1_CSC_IODC.bits.cscidc2 = %d \n", __FUNCTION__, __LINE__, V1_CSC_IODC.bits.cscidc2);
        osal_printk("[Func:%s][Line:%d] V1_CSC_ODC.bits.cscodc0  = %d \n", __FUNCTION__, __LINE__, V1_CSC_ODC.bits.cscodc0 );
        osal_printk("[Func:%s][Line:%d] V1_CSC_ODC.bits.cscodc1  = %d \n", __FUNCTION__, __LINE__, V1_CSC_ODC.bits.cscodc1 );
        osal_printk("[Func:%s][Line:%d] V1_CSC_IODC.bits.cscodc2 = %d \n", __FUNCTION__, __LINE__, V1_CSC_IODC.bits.cscodc2);
    #endif

    }
    else if(HAL_DISP_LAYER_GFX0 == enLayer)
    {

        // coef
        HIHDR_G_RGB2YUV_COEF0.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF0.u32));
        HIHDR_G_RGB2YUV_COEF0.bits.hihdr_r2y_coef00 = (pstCscCoef->csc_coef00);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF0.u32), HIHDR_G_RGB2YUV_COEF0.u32);

        HIHDR_G_RGB2YUV_COEF1.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF1.u32));
        HIHDR_G_RGB2YUV_COEF1.bits.hihdr_r2y_coef01 = (pstCscCoef->csc_coef01);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF1.u32), HIHDR_G_RGB2YUV_COEF1.u32);

        HIHDR_G_RGB2YUV_COEF2.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF2.u32));
        HIHDR_G_RGB2YUV_COEF2.bits.hihdr_r2y_coef02 = (pstCscCoef->csc_coef02);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF2.u32), HIHDR_G_RGB2YUV_COEF2.u32);

        HIHDR_G_RGB2YUV_COEF3.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF3.u32));
        HIHDR_G_RGB2YUV_COEF3.bits.hihdr_r2y_coef10 = (pstCscCoef->csc_coef10);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF3.u32), HIHDR_G_RGB2YUV_COEF3.u32);

        HIHDR_G_RGB2YUV_COEF4.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF4.u32));
        HIHDR_G_RGB2YUV_COEF4.bits.hihdr_r2y_coef11 = (pstCscCoef->csc_coef11);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF4.u32), HIHDR_G_RGB2YUV_COEF4.u32);

        HIHDR_G_RGB2YUV_COEF5.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF5.u32));
        HIHDR_G_RGB2YUV_COEF5.bits.hihdr_r2y_coef12 = (pstCscCoef->csc_coef12);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF5.u32), HIHDR_G_RGB2YUV_COEF5.u32);

        HIHDR_G_RGB2YUV_COEF6.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF6.u32));
        HIHDR_G_RGB2YUV_COEF6.bits.hihdr_r2y_coef20 = (pstCscCoef->csc_coef20);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF6.u32), HIHDR_G_RGB2YUV_COEF6.u32);

        HIHDR_G_RGB2YUV_COEF7.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF7.u32));
        HIHDR_G_RGB2YUV_COEF7.bits.hihdr_r2y_coef21 = (pstCscCoef->csc_coef21);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF7.u32), HIHDR_G_RGB2YUV_COEF7.u32);

        HIHDR_G_RGB2YUV_COEF8.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF8.u32));
        HIHDR_G_RGB2YUV_COEF8.bits.hihdr_r2y_coef22 = (pstCscCoef->csc_coef22);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_COEF8.u32), HIHDR_G_RGB2YUV_COEF8.u32);

        // xdc
        // idc
        HIHDR_G_RGB2YUV_IDC0.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_IDC0.u32));
        HIHDR_G_RGB2YUV_IDC0.bits.hihdr_r2y_idc0 = (pstCscCoef->csc_in_dc0);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_IDC0.u32), HIHDR_G_RGB2YUV_IDC0.u32);

        HIHDR_G_RGB2YUV_IDC1.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_IDC1.u32));
        HIHDR_G_RGB2YUV_IDC1.bits.hihdr_r2y_idc1 = (pstCscCoef->csc_in_dc1);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_IDC1.u32), HIHDR_G_RGB2YUV_IDC1.u32);

        HIHDR_G_RGB2YUV_IDC2.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_IDC2.u32));
        HIHDR_G_RGB2YUV_IDC2.bits.hihdr_r2y_idc2 = (pstCscCoef->csc_in_dc2);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_IDC2.u32), HIHDR_G_RGB2YUV_IDC2.u32);

        // odc
        HIHDR_G_RGB2YUV_ODC0.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_ODC0.u32));
        HIHDR_G_RGB2YUV_ODC0.bits.hihdr_r2y_odc0 = (pstCscCoef->csc_out_dc0);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_ODC0.u32), HIHDR_G_RGB2YUV_ODC0.u32);

        HIHDR_G_RGB2YUV_ODC1.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_ODC1.u32));
        HIHDR_G_RGB2YUV_ODC1.bits.hihdr_r2y_odc1 = (pstCscCoef->csc_out_dc1);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_ODC1.u32), HIHDR_G_RGB2YUV_ODC1.u32);

        HIHDR_G_RGB2YUV_ODC2.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_ODC2.u32));
        HIHDR_G_RGB2YUV_ODC2.bits.hihdr_r2y_odc2 = (pstCscCoef->csc_out_dc2);
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_ODC2.u32), HIHDR_G_RGB2YUV_ODC2.u32);
    }
    else if(HAL_DISP_LAYER_GFX1 == enLayer)
    {
        //addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->G0_CSC_IDC.u32));
        addr_REG = (HI_UL)&(pVoReg->G1_CSC_IDC.u32);
        G1_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G1_CSC_IDC.bits.cscidc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc1);
        G1_CSC_IDC.bits.cscidc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc0);
        HAL_WriteReg((HI_U32*)addr_REG, G1_CSC_IDC.u32);

        //addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->G0_CSC_ODC.u32));
        addr_REG = (HI_UL)&(pVoReg->G1_CSC_ODC.u32);
        G1_CSC_ODC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G1_CSC_ODC.bits.cscodc1 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc1);
        G1_CSC_ODC.bits.cscodc0 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc0);
        HAL_WriteReg((HI_U32*)addr_REG, G1_CSC_ODC.u32);

        //addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->G0_CSC_IODC.u32));
        addr_REG = (HI_UL)&(pVoReg->G1_CSC_IODC.u32);
        G1_CSC_IODC.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G1_CSC_IODC.bits.cscodc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_out_dc2);
        G1_CSC_IODC.bits.cscidc2 = HAL_GetXDC_BUMA(pstCscCoef->csc_in_dc2);
        HAL_WriteReg((HI_U32*)addr_REG, G1_CSC_IODC.u32);

        //addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->G0_CSC_P0.u32));
        addr_REG = (HI_UL)&(pVoReg->G1_CSC_P0.u32);
        G1_CSC_P0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G1_CSC_P0.bits.cscp00 = HAL_Conver_CscCoef(pstCscCoef->csc_coef00);
        G1_CSC_P0.bits.cscp01 = HAL_Conver_CscCoef(pstCscCoef->csc_coef01);
        HAL_WriteReg((HI_U32*)addr_REG, G1_CSC_P0.u32);

        //addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->G0_CSC_P1.u32));
        addr_REG = (HI_UL)&(pVoReg->G1_CSC_P1.u32);
        G1_CSC_P1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G1_CSC_P1.bits.cscp02 = HAL_Conver_CscCoef(pstCscCoef->csc_coef02);
        G1_CSC_P1.bits.cscp10 = HAL_Conver_CscCoef(pstCscCoef->csc_coef10);
        HAL_WriteReg((HI_U32*)addr_REG, G1_CSC_P1.u32);

        //addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->G0_CSC_P2.u32));
        addr_REG = (HI_UL)&(pVoReg->G1_CSC_P2.u32);
        G1_CSC_P2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G1_CSC_P2.bits.cscp11 = HAL_Conver_CscCoef(pstCscCoef->csc_coef11);
        G1_CSC_P2.bits.cscp12 = HAL_Conver_CscCoef(pstCscCoef->csc_coef12);
        HAL_WriteReg((HI_U32*)addr_REG, G1_CSC_P2.u32);

        //addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->G0_CSC_P3.u32));
        addr_REG = (HI_UL)&(pVoReg->G1_CSC_P3.u32);
        G1_CSC_P3.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G1_CSC_P3.bits.cscp20 = HAL_Conver_CscCoef(pstCscCoef->csc_coef20);
        G1_CSC_P3.bits.cscp21 = HAL_Conver_CscCoef(pstCscCoef->csc_coef21);
        HAL_WriteReg((HI_U32*)addr_REG, G1_CSC_P3.u32);

        //addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->G)0_CSC_P4.u32));
        addr_REG = (HI_UL)&(pVoReg->G1_CSC_P4.u32);
        G1_CSC_P4.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G1_CSC_P4.bits.cscp22 = HAL_Conver_CscCoef(pstCscCoef->csc_coef22);
        HAL_WriteReg((HI_U32*)addr_REG, G1_CSC_P4.u32);
    }
#if 0
    else if (HAL_DISP_LAYER_WBC == enLayer)
    {
        // for coef 0 1 2 3 4 5 6 7 8
        WDHDR_RGB2YUV_COEF0.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF0.u32));
        WDHDR_RGB2YUV_COEF0.bits.vhdr_r2y_coef00  = pstCscCoef->csc_coef00;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF0.u32), WDHDR_RGB2YUV_COEF0.u32);

        WDHDR_RGB2YUV_COEF1.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF1.u32));
        WDHDR_RGB2YUV_COEF1.bits.vhdr_r2y_coef01  = pstCscCoef->csc_coef01;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF1.u32), WDHDR_RGB2YUV_COEF1.u32);

        WDHDR_RGB2YUV_COEF2.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF2.u32));
        WDHDR_RGB2YUV_COEF2.bits.vhdr_r2y_coef02  = pstCscCoef->csc_coef02;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF2.u32), WDHDR_RGB2YUV_COEF2.u32);

        WDHDR_RGB2YUV_COEF3.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF3.u32));
        WDHDR_RGB2YUV_COEF3.bits.vhdr_r2y_coef10  = pstCscCoef->csc_coef10;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF3.u32), WDHDR_RGB2YUV_COEF3.u32);

        WDHDR_RGB2YUV_COEF4.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF4.u32));
        WDHDR_RGB2YUV_COEF4.bits.vhdr_r2y_coef11  = pstCscCoef->csc_coef11;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF4.u32), WDHDR_RGB2YUV_COEF4.u32);

        WDHDR_RGB2YUV_COEF5.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF5.u32));
        WDHDR_RGB2YUV_COEF5.bits.vhdr_r2y_coef12  = pstCscCoef->csc_coef12;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF5.u32), WDHDR_RGB2YUV_COEF5.u32);

        WDHDR_RGB2YUV_COEF6.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF6.u32));
        WDHDR_RGB2YUV_COEF6.bits.vhdr_r2y_coef20  = pstCscCoef->csc_coef20;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF6.u32), WDHDR_RGB2YUV_COEF6.u32);

        WDHDR_RGB2YUV_COEF7.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF7.u32));
        WDHDR_RGB2YUV_COEF7.bits.vhdr_r2y_coef21  = pstCscCoef->csc_coef21;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF7.u32), WDHDR_RGB2YUV_COEF7.u32);

        WDHDR_RGB2YUV_COEF8.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF8.u32));
        WDHDR_RGB2YUV_COEF8.bits.vhdr_r2y_coef22  = pstCscCoef->csc_coef22;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_COEF8.u32), WDHDR_RGB2YUV_COEF8.u32);

        // for scale2p
        addr_REG = (HI_UL) & (pVoHippReg->WDHDR_RGB2YUV_SCALE2P.u32);
        WDHDR_RGB2YUV_SCALE2P.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WDHDR_RGB2YUV_SCALE2P.bits.vhdr_r2y_scale2p  = pstCscCoef->new_csc_scale2p;
        WDHDR_RGB2YUV_SCALE2P.bits.vhdr_cadj_scale2p = pstCscCoef->new_csc_scale2p;
        HAL_WriteReg((HI_U32*)addr_REG, WDHDR_RGB2YUV_SCALE2P.u32);

        // for idc 0 1 2
        addr_REG = (HI_UL) & (pVoHippReg->WDHDR_RGB2YUV_IDC0.u32);
        WDHDR_RGB2YUV_IDC0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WDHDR_RGB2YUV_IDC0.bits.vhdr_r2y_idc0 = pstCscCoef->new_csc_in_dc00;
        HAL_WriteReg((HI_U32*)addr_REG, WDHDR_RGB2YUV_IDC0.u32);

        addr_REG = (HI_UL) & (pVoHippReg->WDHDR_RGB2YUV_IDC1.u32);
        WDHDR_RGB2YUV_IDC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WDHDR_RGB2YUV_IDC1.bits.vhdr_r2y_idc1 = pstCscCoef->new_csc_in_dc01;
        HAL_WriteReg((HI_U32*)addr_REG, WDHDR_RGB2YUV_IDC1.u32);

        addr_REG = (HI_UL) & (pVoHippReg->WDHDR_RGB2YUV_IDC2.u32);
        WDHDR_RGB2YUV_IDC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WDHDR_RGB2YUV_IDC2.bits.vhdr_r2y_idc2 = pstCscCoef->new_csc_in_dc02;
        HAL_WriteReg((HI_U32*)addr_REG, WDHDR_RGB2YUV_IDC2.u32);

        // for odc 0 1 2
        addr_REG = (HI_UL) & (pVoHippReg->WDHDR_RGB2YUV_ODC0.u32);
        WDHDR_RGB2YUV_ODC0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WDHDR_RGB2YUV_ODC0.bits.vhdr_r2y_odc0 = pstCscCoef->new_csc_out_dc00;
        HAL_WriteReg((HI_U32*)addr_REG, WDHDR_RGB2YUV_ODC0.u32);

        addr_REG = (HI_UL) & (pVoHippReg->WDHDR_RGB2YUV_ODC1.u32);
        WDHDR_RGB2YUV_ODC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WDHDR_RGB2YUV_ODC1.bits.vhdr_r2y_odc1 = pstCscCoef->new_csc_out_dc01;
        HAL_WriteReg((HI_U32*)addr_REG, WDHDR_RGB2YUV_ODC1.u32);

        addr_REG = (HI_UL) & (pVoHippReg->WDHDR_RGB2YUV_ODC2.u32);
        WDHDR_RGB2YUV_ODC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WDHDR_RGB2YUV_ODC2.bits.vhdr_r2y_odc2 = pstCscCoef->new_csc_out_dc02;
        HAL_WriteReg((HI_U32*)addr_REG, WDHDR_RGB2YUV_ODC2.u32);

        // for clip min and max
        WDHDR_RGB2YUV_MIN.u32 = HAL_ReadReg((HI_U32*) & (pVoHippReg->WDHDR_RGB2YUV_MIN.u32));
        WDHDR_RGB2YUV_MIN.bits.vhdr_r2y_clip_y_min = pstCscCoef->new_csc_clip_min;
        WDHDR_RGB2YUV_MIN.bits.vhdr_r2y_clip_c_min = pstCscCoef->new_csc_clip_min;
        HAL_WriteReg((HI_U32*) & (pVoHippReg->WDHDR_RGB2YUV_MIN.u32), WDHDR_RGB2YUV_MIN.u32);

        WDHDR_RGB2YUV_MAX.u32 = HAL_ReadReg((HI_U32*) & (pVoHippReg->WDHDR_RGB2YUV_MAX.u32));
        WDHDR_RGB2YUV_MAX.bits.vhdr_r2y_clip_y_max = pstCscCoef->new_csc_clip_max;
        WDHDR_RGB2YUV_MAX.bits.vhdr_r2y_clip_c_max = pstCscCoef->new_csc_clip_max;
        HAL_WriteReg((HI_U32*) & (pVoHippReg->WDHDR_RGB2YUV_MAX.u32), WDHDR_RGB2YUV_MAX.u32);
    }
#endif
    else{
        HAL_PRINT("Error layer id found in %s,%s,%d\n",__FILE__, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
//*/

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn)
{
    //*
    U_HIHDR_V_YUV2RGB_CTRL HIHDR_V_YUV2RGB_CTRL;
    U_V1_CSC_IDC           V1_CSC_IDC;
    U_HIHDR_G_RGB2YUV_CTRL HIHDR_G_RGB2YUV_CTRL;
    U_G1_CSC_IDC           G1_CSC_IDC;
    //U_WDHDR_RGB2YUV_CTRL   WDHDR_RGB2YUV_CTRL;
    //volatile  HI_UL addr_REG;

    if(HAL_DISP_LAYER_VHD0 == enLayer)
    {
        //TODO
        //osal_printk("+++[Func:%s][Line:%d] enable v0's csc.+++\n",__FUNCTION__,__LINE__);
        HIHDR_V_YUV2RGB_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->HIHDR_V_YUV2RGB_CTRL.u32));
        HIHDR_V_YUV2RGB_CTRL.bits.hihdr_v0_y2r_en = 0x1;
        HIHDR_V_YUV2RGB_CTRL.bits.hihdr_y2r_ck_gt_en =0x1;
        HIHDR_V_YUV2RGB_CTRL.bits.hihdr_y2r_en = bCscEn;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->HIHDR_V_YUV2RGB_CTRL.u32), HIHDR_V_YUV2RGB_CTRL.u32);
    }
    else if(HAL_DISP_LAYER_VHD1 == enLayer)
    {
        V1_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CSC_IDC.u32));
        //osal_printk("+++[Func:%s][Line:%d] csc en = %d+++\n",__FUNCTION__,__LINE__,bCscEn);
        V1_CSC_IDC.bits.csc_en = bCscEn;
        //V1_CSC_IDC.bits.csc_mode = 0x5;  // 101
        //V1_CSC_IDC.bits.csc_ck_gt_en = 0x1;
        HAL_WriteReg((HI_U32*)&(pVoReg->V1_CSC_IDC.u32), V1_CSC_IDC.u32);
    }
    else if(HAL_DISP_LAYER_GFX0 == enLayer)
    {
        //osal_printk("+++[Func:%s][Line:%d] enable g0's csc = %d.+++\n",__FUNCTION__,__LINE__,bCscEn);
        HIHDR_G_RGB2YUV_CTRL.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_CTRL.u32));
        HIHDR_G_RGB2YUV_CTRL.bits.hihdr_r2y_en = bCscEn;
        HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_CTRL.u32), HIHDR_G_RGB2YUV_CTRL.u32);
    }
    else if(HAL_DISP_LAYER_GFX1 == enLayer)
    {
        // addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)(HI_UL)&(pVoReg->G0_CSC_IDC.u32));
        G1_CSC_IDC.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G1_CSC_IDC.u32));
        G1_CSC_IDC.bits.csc_en = bCscEn;
        HAL_WriteReg((HI_U32*)&(pVoReg->G1_CSC_IDC.u32), G1_CSC_IDC.u32);
    }
#if 0
    else if (HAL_DISP_LAYER_WBC == enLayer)
    {
        WDHDR_RGB2YUV_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_CTRL.u32));
        WDHDR_RGB2YUV_CTRL.bits.vhdr_cadj_en = bCscEn;
        WDHDR_RGB2YUV_CTRL.bits.vhdr_r2y_en = bCscEn;
        HAL_WriteReg((HI_U32*)&(pVoHippReg->WDHDR_RGB2YUV_CTRL.u32), WDHDR_RGB2YUV_CTRL.u32);
    }
#endif
    else
    {
        HAL_PRINT("Error:Wrong layer ID in HAL_LAYER_SetCscEn !\n");
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetLayerInRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect)
{
    U_VID_IN_RESO VID_IN_RESO;
    U_GFX_IRESO   GFX_IRESO;
    //U_G0SFPOS G0SFPOS;
    volatile  HI_UL addr_REG;

    if (HAL_DISP_LAYER_VHD0 == enLayer || HAL_DISP_LAYER_VHD1 == enLayer)
    {
        addr_REG = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_IN_RESO.u32));
        VID_IN_RESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        VID_IN_RESO.bits.ireso_w = pstRect->u32Width - 1;
        VID_IN_RESO.bits.ireso_h = pstRect->u32Height - 1;
        HAL_WriteReg((HI_U32*)addr_REG, VID_IN_RESO.u32);
    }
    else if (HAL_DISP_LAYER_GFX0 == enLayer || HAL_DISP_LAYER_GFX1 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_IRESO.u32));
        GFX_IRESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_IRESO.bits.ireso_w = pstRect->u32Width - 1;
        GFX_IRESO.bits.ireso_h = pstRect->u32Height - 1;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_IRESO.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s,%s,%d\n",__FILE__, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetLayerOutRect(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect)
{
/*
    U_V0_ORESO V0_ORESO;
    volatile  HI_UL addr_REG;

    if((LAYER_VHD_START <= enLayer)&&(enLayer <= LAYER_VSD_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->V0_ORESO.u32));
        V0_ORESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_ORESO.bits.ow = pstRect->u32Width- 1;
        V0_ORESO.bits.oh = pstRect->u32Height- 1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_ORESO.u32);
    }
    else if((LAYER_GFX_START <= enLayer)&&(enLayer <= LAYER_GFX_END))
    {

    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/
    return HI_FALSE;
}

HI_BOOL HAL_LAYER_SetLayerGalpha(HAL_DISP_LAYER_E enLayer,
                                     HI_U8 u8Alpha0)
{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;

    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_CTRL.u32));
            //osal_printk("+++++++HAL_LAYER_SetLayerGAlpha:%d, reg: %p++++++++\n", __LINE__, (HI_U32*)addr_REG);
            V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_CTRL.bits.galpha = u8Alpha0;
            HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_CTRL.u32));
            G0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_CTRL.bits.galpha = u8Alpha0;
            HAL_WriteReg((HI_U32*)addr_REG, G0_CTRL.u32);
            break;
        }

        default:
        {
            HAL_PRINT("Error layer id %d found in %s: L%d\n", enLayer, __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetZmeInfo(HAL_DISP_LAYER_E enLayer, HI_U32 u32Width, HI_U32 u32Height,
                             HAL_DISP_ZME_OUTFMT_E enZmeOutFmt)
{

    volatile U_V0_ZME_HINFO V0_ZME_HINFO;
    volatile U_V0_ZME_VINFO V0_ZME_VINFO;

    volatile U_V1_CVFIR_VINFO V1_CVFIR_VINFO;
    volatile U_V2_CVFIR_VINFO V2_CVFIR_VINFO;

    volatile U_G0_ZME_HINFO G0_ZME_HINFO;
    volatile U_G0_ZME_VINFO G0_ZME_VINFO;

    volatile U_WD_ZME_HINFO WD_ZME_HINFO;
    volatile U_WD_ZME_VINFO WD_ZME_VINFO;

    volatile  HI_UL addr_REG;

    if (HAL_DISP_LAYER_VHD0 == enLayer)
    {
        // Horiz info
        addr_REG = (HI_UL) & (pVoReg->V0_ZME_HINFO.u32);
        V0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_ZME_HINFO.bits.out_width = u32Width - 1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_HINFO.u32);

        // Vertical info
        addr_REG = (HI_UL) & (pVoReg->V0_ZME_VINFO.u32);
        V0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        // TODO
        V0_ZME_VINFO.bits.out_pro    = 1;
        V0_ZME_VINFO.bits.out_height = u32Height - 1;
        V0_ZME_VINFO.bits.out_fmt    = enZmeOutFmt;
        HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_VINFO.u32);
    }
    else if (HAL_DISP_LAYER_VHD1 == enLayer)
    {
        // Vertical info
        addr_REG = (HI_UL) & (pVoReg->V1_CVFIR_VINFO.u32);
        V1_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        // TODO
        V1_CVFIR_VINFO.bits.vzme_ck_gt_en = 1;
        V1_CVFIR_VINFO.bits.out_pro    = 1;
        V1_CVFIR_VINFO.bits.out_height = u32Height;
        V1_CVFIR_VINFO.bits.out_fmt    = enZmeOutFmt;
        HAL_WriteReg((HI_U32*)addr_REG, V1_CVFIR_VINFO.u32);
    }
    else if (HAL_DISP_LAYER_VHD2 == enLayer)
    {
        // Vertical info
        addr_REG = (HI_UL) & (pVoReg->V2_CVFIR_VINFO.u32);
        V2_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        // TODO
        V2_CVFIR_VINFO.bits.vzme_ck_gt_en = 1;
        V2_CVFIR_VINFO.bits.out_pro    = 1;
        V2_CVFIR_VINFO.bits.out_height = u32Height;
        V2_CVFIR_VINFO.bits.out_fmt    = enZmeOutFmt;
        HAL_WriteReg((HI_U32*)addr_REG, V2_CVFIR_VINFO.u32);
    }
    else if (HAL_DISP_LAYER_GFX0 == enLayer)
    {
        // Horiz info
        addr_REG = (HI_UL) & (pVoReg->G0_ZME_HINFO.u32);
        G0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_ZME_HINFO.bits.out_width = u32Width;
        HAL_WriteReg((HI_U32*)addr_REG, G0_ZME_HINFO.u32);

        // Vertical info
        addr_REG = (HI_UL) & (pVoReg->G0_ZME_VINFO.u32);
        G0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        // TODO
        G0_ZME_VINFO.bits.out_pro    = 1;
        G0_ZME_VINFO.bits.out_height = u32Height;
        // G0_ZME_VINFO.bits.out_fmt    = enDataFmt;
        HAL_WriteReg((HI_U32*)addr_REG, G0_ZME_VINFO.u32);
    }
    else if (HAL_DISP_LAYER_WBC == enLayer)
    {
        // osal_printk("+++%s,line:%d+++\n",__FUNCTION__,__LINE__);
        // Horizon info
        addr_REG = (HI_UL) & (pVoReg->WD_ZME_HINFO.u32);
        WD_ZME_HINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        //WD_ZME_HINFO.bits.hzme_ck_gt_en = 1;
        WD_ZME_HINFO.bits.out_width = u32Width - 1;
        HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_HINFO.u32);
        // Vertica info
        addr_REG = (HI_UL) & (pVoReg->WD_ZME_VINFO.u32);
        WD_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WD_ZME_VINFO.bits.out_pro    = 1;
        WD_ZME_VINFO.bits.out_height = u32Height - 1;
        //WD_ZME_VINFO.bits.out_fmt    = enZmeOutFmt;
        HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_VINFO.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }


    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetSrcResolution(HAL_DISP_LAYER_E enLayer,
                                   RECT_S*   pstRect)
{
    U_VID_SRC_RESO VID_SRC_RESO;

    volatile  HI_UL addr_REG;

    if ( HAL_DISP_LAYER_VHD0 == enLayer ||
         HAL_DISP_LAYER_VHD1 == enLayer ||
         HAL_DISP_LAYER_VHD2 == enLayer)
    {
        addr_REG = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_SRC_RESO.u32));
        VID_SRC_RESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        VID_SRC_RESO.bits.src_w = pstRect->u32Width - 1;
        VID_SRC_RESO.bits.src_h = pstRect->u32Height - 1;
        HAL_WriteReg((HI_U32*)addr_REG, VID_SRC_RESO.u32);
    }

    return HI_TRUE;
}


/* Vou zoom enable */
HI_BOOL HAL_LAYER_SetZmeEnable(HAL_DISP_LAYER_E enLayer,
                                    HAL_DISP_ZMEMODE_E enMode,
                                    HI_U32 bEnable)
{
    volatile U_V0_ZME_HSP V0_ZME_HSP;
    volatile U_V0_ZME_VSP V0_ZME_VSP;

    volatile U_V1_CVFIR_VSP V1_CVFIR_VSP;

    volatile U_G0_ZME_HSP G0_ZME_HSP;
    volatile U_G0_ZME_VSP G0_ZME_VSP;

    volatile U_WD_ZME_HSP WD_ZME_HSP;
    volatile U_WD_ZME_VSP WD_ZME_VSP;

    volatile  HI_UL addr_REG;

    if (enLayer == HAL_DISP_LAYER_VHD0)
    {
        // layer zoom enable
        if ((enMode == HAL_DISP_ZMEMODE_HORL) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_ZME_HSP.u32));
            V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            // TODO
            //V0_ZME_HSP.bits.hlmsc_en = bEnable;
            V0_ZME_HSP.bits.lhfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_HORC) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_ZME_HSP.u32));
            V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            // TODO
            //V0_ZME_HSP.bits.hchmsc_en = bEnable;
            V0_ZME_HSP.bits.chfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERL) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_ZME_VSP.u32));
            V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            // TODO
            //V0_ZME_VSP.bits.vlmsc_en = bEnable;
            V0_ZME_VSP.bits.lvfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_VSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERC) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_ZME_VSP.u32));
            V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            // TODO
            //V0_ZME_VSP.bits.vchmsc_en = bEnable;
            V0_ZME_VSP.bits.cvfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_VSP.u32);
        }
    }
    else if (enLayer == HAL_DISP_LAYER_VHD1)
    {
        if ((enMode == HAL_DISP_ZMEMODE_VERL) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = (HI_UL) & (pVoReg->V1_CVFIR_VSP.u32);
            V1_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            // TODO
            //V0_ZME_VSP.bits.vlmsc_en = bEnable;
            V1_CVFIR_VSP.bits.cvfir_en = bEnable;
            V1_CVFIR_VSP.bits.cvmid_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V1_CVFIR_VSP.u32);
        }
    }

    else if (HAL_DISP_LAYER_GFX0 == enLayer) // TODO
    {
        // VSD layer zoom enable
        if ((enMode == HAL_DISP_ZMEMODE_HORL) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_ZME_HSP.u32));
            G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            //G0_ZME_HSP.bits.hlmsc_en = bEnable;
            G0_ZME_HSP.bits.hfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, G0_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERL) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_ZME_VSP.u32));
            G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            //G0_ZME_VSP.bits.vlmsc_en = bEnable;
            G0_ZME_VSP.bits.vfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, G0_ZME_VSP.u32);
        }


    }
    else if (HAL_DISP_LAYER_WBC == enLayer)
    {
        // wbc layer zoom enable
        if ((enMode == HAL_DISP_ZMEMODE_HORL) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_ZME_HSP.u32));
            WD_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_ZME_HSP.bits.lhfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_HORC) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_ZME_HSP.u32));
            WD_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_ZME_HSP.bits.chfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERL) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_ZME_VSP.u32));
            WD_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_ZME_VSP.bits.lvfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_VSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERC) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_ZME_VSP.u32));
            WD_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_ZME_VSP.bits.cvfir_en = 1;
            HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_VSP.u32);
        }
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetZmeFirEnable(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 bEnable)
{
    U_V0_ZME_HSP V0_ZME_HSP;
    U_V0_ZME_VSP V0_ZME_VSP;

    U_V1_CVFIR_VSP V1_CVFIR_VSP;

    U_WD_ZME_HSP WD_ZME_HSP;
    U_WD_ZME_VSP WD_ZME_VSP;

    volatile  HI_UL addr_REG;


    if (HAL_DISP_LAYER_VHD0 == enLayer)
    {
        // layer zoom enable
        if ((enMode == HAL_DISP_ZMEMODE_HORL) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_ZME_HSP.u32));
            V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_ZME_HSP.bits.lhfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_HORC) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_ZME_HSP.u32));
            V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_ZME_HSP.bits.chfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERL) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_ZME_VSP.u32));
            V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_ZME_VSP.bits.lvfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_VSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERC) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_ZME_VSP.u32));
            V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_ZME_VSP.bits.cvfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_VSP.u32);
        }
    }
    else if (HAL_DISP_LAYER_VHD1 == enLayer)
    {
        if ((enMode == HAL_DISP_ZMEMODE_VERC) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = (HI_UL) & (pVoReg->V1_CVFIR_VSP.u32);
            V1_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V1_CVFIR_VSP.bits.cvfir_en = bEnable;
            V1_CVFIR_VSP.bits.cvmid_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V1_CVFIR_VSP.u32);
        }
    }
    else if (HAL_DISP_LAYER_WBC == enLayer)
    {
        // wbc layer zoom enable
        if ((enMode == HAL_DISP_ZMEMODE_HORL) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_ZME_HSP.u32));
            WD_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_ZME_HSP.bits.lhfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_HORC) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_ZME_HSP.u32));
            WD_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_ZME_HSP.bits.chfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERL) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_ZME_VSP.u32));
            WD_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_ZME_VSP.bits.lvfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_VSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERC) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WD_ZME_VSP.u32));
            WD_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WD_ZME_VSP.bits.cvfir_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WD_ZME_VSP.u32);
        }
    }

    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetZmeMscEnable(HAL_DISP_LAYER_E enLayer, HAL_DISP_ZMEMODE_E enMode, HI_U32 bEnable)
{
    //U_V0_ZME_VINFO V0_ZME_VINFO;
    //U_V0_VSP V0_VSP;
    U_WBC_ME_ZME_HSP WBC_ME_ZME_HSP;
    U_WBC_ME_ZME_VSP WBC_ME_ZME_VSP;

    volatile  HI_UL addr_REG;

    /*
    if(enLayer <= HAL_DISP_LAYER_VHD0)
    {
        // layer zoom enable
        if((enMode == HAL_DISP_ZMEMODE_HORL)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->V0_HSP.u32));
            V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HSP.bits.hlmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32);
        }

        if((enMode == HAL_DISP_ZMEMODE_HORC)||(enMode == HAL_DISP_ZMEMODE_HOR)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HSP.u32));
            V0_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_HSP.bits.hchmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_HSP.u32);
        }

        if((enMode == HAL_DISP_ZMEMODE_VERL)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));
            V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VSP.bits.vlmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32);
        }

        if((enMode == HAL_DISP_ZMEMODE_VERC)||(enMode == HAL_DISP_ZMEMODE_VER)||(enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_VSP.u32));
            V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_VSP.bits.vchmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32);
        }
    }
    //*/
    if (HAL_DISP_LAYER_VHD0 == enLayer || HAL_DISP_LAYER_VHD1 == enLayer)
    {
        // TODO
    }
    else if (HAL_DISP_LAYER_WBC == enLayer)
    {
        // WBC layer zoom enable
        if ((enMode == HAL_DISP_ZMEMODE_HORL) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WBC_ME_ZME_HSP.u32));
            WBC_ME_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_ME_ZME_HSP.bits.hlmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_ME_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_HORC) || (enMode == HAL_DISP_ZMEMODE_HOR) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WBC_ME_ZME_HSP.u32));
            WBC_ME_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_ME_ZME_HSP.bits.hchmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_ME_ZME_HSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERL) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WBC_ME_ZME_VSP.u32));
            WBC_ME_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_ME_ZME_VSP.bits.vlmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_ME_ZME_VSP.u32);
        }

        if ((enMode == HAL_DISP_ZMEMODE_VERC) || (enMode == HAL_DISP_ZMEMODE_VER) || (enMode == HAL_DISP_ZMEMODE_ALL))
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WBC_ME_ZME_VSP.u32));
            WBC_ME_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            WBC_ME_ZME_VSP.bits.vchmsc_en = bEnable;
            HAL_WriteReg((HI_U32*)addr_REG, WBC_ME_ZME_VSP.u32);
        }
    }

    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_LAYER_SetZmeVerType(HAL_DISP_LAYER_E enLayer, HI_U32 uVerType)
{
/*
    U_V0_VSP V0_VSP;
    U_WBC_DHD0_ZME_VSP WBC_DHD0_ZME_VSP;

    volatile  HI_UL addr_REG;
    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->V0_VSP.u32));
        V0_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_VSP.bits.zme_in_fmt = uVerType;
        HAL_WriteReg((HI_U32*)addr_REG, V0_VSP.u32);
    }
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->WBC_DHD0_ZME_VSP.u32));
        WBC_DHD0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_DHD0_ZME_VSP.bits.zme_in_fmt = uVerType;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_ZME_VSP.u32);
    }
    //*/

    return HI_TRUE;
}
HI_BOOL HAL_LAYER_SetVerRatio(HAL_DISP_LAYER_E enLayer, HI_U32 uRatio)
{
    volatile U_V0_ZME_VSP V0_ZME_VSP;
    volatile U_G0_ZME_VSP G0_ZME_VSP;
    volatile U_WBC_ME_ZME_VSR WBC_ME_ZME_VSR;

    volatile HI_UL addr_REG;


    if (HAL_DISP_LAYER_VHD0 == enLayer)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_ZME_VSP.u32));
        V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_ZME_VSP.bits.vratio = uRatio;
        HAL_WriteReg((HI_U32*)addr_REG, V0_ZME_VSP.u32);
    }
    else if (HAL_DISP_LAYER_GFX0 == enLayer)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_ZME_VSP.u32));
        G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        G0_ZME_VSP.bits.vratio = uRatio;
        HAL_WriteReg((HI_U32*)addr_REG, G0_ZME_VSP.u32);
    }

    else if (HAL_DISP_LAYER_WBC ==  enLayer)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->WBC_ME_ZME_VSR.u32));
        WBC_ME_ZME_VSR.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        WBC_ME_ZME_VSR.bits.vratio = uRatio;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_ME_ZME_VSR.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL  HAL_LAYER_SetRegUp(HAL_DISP_LAYER_E enLayer)
{
    U_V0_UPD V0_UPD;
    U_G0_UPD G0_UPD;
    U_WBC_DHD0_UPD WBC_DHD0_UPD;

    volatile HI_UL addr_REG;
    if(enLayer <= HAL_DISP_LAYER_VSD0)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->V0_UPD.u32));
        V0_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        /* layer register update */
        V0_UPD.bits.regup = 0x1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_UPD.u32);
    }
    else if((LAYER_GFX_START <= enLayer) && (enLayer <= LAYER_GFX_END))
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->G0_UPD.u32));
        G0_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        /* layer register update */
        G0_UPD.bits.regup = 0x1;
        HAL_WriteReg((HI_U32*)addr_REG, G0_UPD.u32);
    }
    else if(enLayer == HAL_DISP_LAYER_WBC_D)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_UL)&(pVoReg->WBC_DHD0_UPD.u32));
        WBC_DHD0_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        /* G1 layer register update */
        WBC_DHD0_UPD.bits.regup = 0x1;
        HAL_WriteReg((HI_U32*)addr_REG, WBC_DHD0_UPD.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 u64LAddr)
{
    volatile HI_UL ul_GFX_ADDR_H = 0;
    volatile HI_UL ul_GFX_ADDR_L = 0;

    if(HAL_DISP_LAYER_GFX0 == enLayer || HAL_DISP_LAYER_GFX1 == enLayer)
    {
        // Write low address to register.
        ul_GFX_ADDR_L   = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_ADDR_L));
        //osal_printk("+++%s,%d,u64LAddr=0x%llx+++,GetLowAddr(u64LAddr)=0x%x,GetHighAddr(u64LAddr)=0x%x\n",
        //    __FUNCTION__,__LINE__,
        //    u64LAddr,
        //    GetLowAddr(u64LAddr),
        //    GetHighAddr(u64LAddr));
        HAL_WriteReg((HI_U32*)ul_GFX_ADDR_L, GetLowAddr(u64LAddr));

        // Write high address to register.
        ul_GFX_ADDR_H   = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_ADDR_H));
        HAL_WriteReg((HI_U32*)ul_GFX_ADDR_H, GetHighAddr(u64LAddr));
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxStride(HAL_DISP_LAYER_E enLayer, HI_U16 u16pitch)
{
    volatile  U_GFX_STRIDE   GFX_STRIDE;
    volatile  HI_UL          addr_REG;

    if(HAL_DISP_LAYER_GFX0 == enLayer || HAL_DISP_LAYER_GFX1 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pVoReg->GFX_STRIDE.u32));
        GFX_STRIDE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_STRIDE.bits.surface_stride = u16pitch;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_STRIDE.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    return HI_TRUE;
}

HI_BOOL HAL_GFX_SetSrcResolution(HAL_DISP_LAYER_E enLayer, RECT_S *pstRect)
{
    U_GFX_SRC_RESO GFX_SRC_RESO;
    volatile  HI_UL addr_REG;

    if (HAL_DISP_LAYER_GFX0 == enLayer ||
             HAL_DISP_LAYER_GFX1 == enLayer ||
             HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pVoReg->GFX_SRC_RESO.u32));
        GFX_SRC_RESO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_SRC_RESO.bits.src_w = pstRect->u32Width - 1;
        GFX_SRC_RESO.bits.src_h = pstRect->u32Height - 1;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_SRC_RESO.u32);
    }
    else
    {
        HAL_PRINT("Error:layer id not found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_BOOL HAL_GRAPHIC_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                                HAL_GFX_BITEXTEND_E enMode)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile  HI_UL addr_REG;

    if(HAL_DISP_LAYER_GFX0 == enLayer || HAL_DISP_LAYER_GFX1 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pVoReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_OUT_CTRL.bits.bitext = enMode;
        HAL_WriteReg((HI_U32*)(HI_UL)addr_REG, GFX_OUT_CTRL.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    //*
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile  HI_UL addr_REG;

    if(HAL_DISP_LAYER_GFX0 == enLayer || HAL_DISP_LAYER_GFX1 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL)&(pVoReg->GFX_OUT_CTRL.u32));
        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_OUT_CTRL.bits.premulti_en = bEnable;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_OUT_CTRL.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                   HI_U32 bAlphaEn,HI_U32 bArange,
                                   HI_U8 u8Alpha0, HI_U8 u8Alpha1)
{
        //*
        U_GFX_OUT_CTRL GFX_OUT_CTRL;

        U_GFX_1555_ALPHA GFX_1555_ALPHA;

        volatile HI_UL addr_REG;

        if (HAL_DISP_LAYER_GFX0 == enLayer || HAL_DISP_LAYER_GFX1 == enLayer)
        {
            addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_OUT_CTRL.u32));
            GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            GFX_OUT_CTRL.bits.palpha_en = bAlphaEn;
            HAL_WriteReg((HI_U32*)addr_REG, GFX_OUT_CTRL.u32);

            if (HI_TRUE == bAlphaEn)
            {
                // TODO
                addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_1555_ALPHA.u32));
                GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
                GFX_1555_ALPHA.bits.alpha_1 = u8Alpha1;
                HAL_WriteReg((HI_U32*)addr_REG, GFX_1555_ALPHA.u32);


                addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_1555_ALPHA.u32));
                GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32*)addr_REG);
                GFX_1555_ALPHA.bits.alpha_0 = u8Alpha0;
                HAL_WriteReg((HI_U32*)addr_REG, GFX_1555_ALPHA.u32);
            }
            else
            {
                // TODO
                addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_1555_ALPHA.u32));
                GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32*)(HI_UL)addr_REG);
                GFX_1555_ALPHA.bits.alpha_1 = 0xff;
                HAL_WriteReg((HI_U32*)(HI_UL)addr_REG, GFX_1555_ALPHA.u32);

                addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_1555_ALPHA.u32));
                GFX_1555_ALPHA.u32 = HAL_ReadReg((HI_U32*)(HI_UL)addr_REG);
                GFX_1555_ALPHA.bits.alpha_0 = 0xff;
                HAL_WriteReg((HI_U32*)(HI_UL)addr_REG, GFX_1555_ALPHA.u32);
            }
        }
        else
        {
            HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
            return HI_FALSE;
        }
    //*/

    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxPalphaRange(HAL_DISP_LAYER_E enLayer, HI_U32 bArange)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;
    volatile  HI_UL addr_REG;

    if ((HAL_DISP_LAYER_GFX0 <= enLayer) && (enLayer <= HAL_DISP_LAYER_GFX1))
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer,(HI_UL) & (pVoReg->GFX_OUT_CTRL.u32));

        GFX_OUT_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);

        GFX_OUT_CTRL.bits.palpha_range = bArange;

        HAL_WriteReg((HI_U32*)addr_REG, GFX_OUT_CTRL.u32);

    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL HAL_CBM_SetCbmAttr(HAL_DISP_LAYER_E enLayer, HAL_DISP_OUTPUTCHANNEL_E enChan)
{
    /*
    U_VHD_CORRESP VHD_CORRESP;
    U_GDC_CORRESP GDC_CORRESP;

    if(enLayer == HAL_DISP_LAYER_VP)
    {
        VHD_CORRESP.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->VHD_CORRESP.u32));
        if(enChan == HAL_DISP_CHANNEL_DHD0)
        {
            VHD_CORRESP.bits.v1_corresp = 0x1;
        }
        else if(enChan == HAL_DISP_CHANNEL_DHD1)
        {
            VHD_CORRESP.bits.v1_corresp = 0x2;
        }
        else
        {
            HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }

        HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->VHD_CORRESP.u32), VHD_CORRESP.u32);
    }
    else if(enLayer == HAL_DISP_LAYER_HC0)
    {
        GDC_CORRESP.u32 = HAL_ReadReg((HI_U32*)(HI_UL)&(pVoReg->GDC_CORRESP.u32));
        if(enChan == HAL_DISP_CHANNEL_DHD0)
        {
            GDC_CORRESP.bits.g2_corresp = 0x2;
        }
        else if(enChan == HAL_DISP_CHANNEL_DHD1)
        {
            GDC_CORRESP.bits.g2_corresp = 0x2;
        }
        else if(enChan == HAL_DISP_CHANNEL_DSD0)
        {
            GDC_CORRESP.bits.g2_corresp = 0x4;
        }
        else
        {
            HAL_PRINT("Error channel id found in %s: L%d\n",__FUNCTION__, __LINE__);
            return HI_FALSE;
        }

        HAL_WriteReg((HI_U32*)(HI_UL)&(pVoReg->GDC_CORRESP.u32), GDC_CORRESP.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s: L%d\n",__FUNCTION__, __LINE__);
        return HI_FALSE;
    }
    //*/
    return HI_TRUE;
}

HI_BOOL HAL_CBM_SetCbmBkg(HI_U32 u32MixerId, HAL_DISP_BKCOLOR_S *pstBkg)
{
    U_CBM_BKG1 CBM_BKG1;
    U_CBM_BKG2 CBM_BKG2;

    if (HAL_CBMMIX1 == u32MixerId)
    {
        CBM_BKG1.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->CBM_BKG1.u32));
        CBM_BKG1.bits.cbm_bkgy1  = (pstBkg->u16Bkg_y);
        CBM_BKG1.bits.cbm_bkgcb1 = (pstBkg->u16Bkg_cb);
        CBM_BKG1.bits.cbm_bkgcr1 = (pstBkg->u16Bkg_cr);
        HAL_WriteReg((HI_U32*) & (pVoReg->CBM_BKG1.u32), CBM_BKG1.u32);
    }
    else if (HAL_CBMMIX2 == u32MixerId)
    {
        CBM_BKG2.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->CBM_BKG2.u32));
        CBM_BKG2.bits.cbm_bkgy2  = (pstBkg->u16Bkg_y);
        CBM_BKG2.bits.cbm_bkgcb2 = (pstBkg->u16Bkg_cb);
        CBM_BKG2.bits.cbm_bkgcr2 = (pstBkg->u16Bkg_cr);
        HAL_WriteReg((HI_U32*) & (pVoReg->CBM_BKG2.u32), CBM_BKG2.u32);
    }
    else
    {
        HAL_PRINT("Error mix id found in %s: L%d\n", __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_BOOL HAL_CBM_SetCbmMixerPrio(HAL_DISP_LAYER_E enLayer, HI_U8 u8Prio, HI_U8 u8MixerId)
{
    U_CBM_MIX1 CBM_MIX1;
    U_CBM_MIX2 CBM_MIX2;
    U_CBM_MIX3 CBM_MIX3;
    HI_U8  u8LayerId = 0;

    // Settings for v0 and g0
    if (HAL_CBMMIX1 == u8MixerId)
    {
        //check layer availability
        switch (enLayer)
        {
            case HAL_DISP_LAYER_VHD0:
            {
                // TODO
                u8LayerId = 0x1;
                break;
            }

            case HAL_DISP_LAYER_GFX0:
            {
                // TODO
                u8LayerId = 0x2;
                break;
            }

            default:
            {
                HAL_PRINT("Error layer id %d found in %s: L%d\n", enLayer, __FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }

        //set mixer prio
        CBM_MIX1.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->CBM_MIX1.u32));

        switch (u8Prio)
        {

            case 0:
            {
                CBM_MIX1.bits.mixer_prio0 = u8LayerId;
                break;
            }

            case 1:
            {
                CBM_MIX1.bits.mixer_prio1 = u8LayerId;
                break;
            }

            case 2:
            {
                CBM_MIX1.bits.mixer_prio2 = u8LayerId;
                break;
            }

            default:
            {
                HAL_PRINT("Error priority id found in %s: L%d\n", __FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }

        HAL_WriteReg((HI_U32*) & (pVoReg->CBM_MIX1.u32), CBM_MIX1.u32);
    }
    else if (HAL_CBMMIX2 == u8MixerId)
    {
        //check layer availability
        switch (enLayer)
        {
            case HAL_DISP_LAYER_VHD1:
            {
                // TODO
                u8LayerId = 0x1;
                break;
            }

            case HAL_DISP_LAYER_GFX1:
            {
                // TODO
                u8LayerId = 0x2;
                break;
            }

            default:
            {
                HAL_PRINT("Error layer id %d found in %s: L%d\n", enLayer, __FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }

        //set mixer prio
        CBM_MIX2.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->CBM_MIX2.u32));

        switch (u8Prio)
        {

            case 0:
            {
                CBM_MIX2.bits.mixer_prio0 = u8LayerId;
                break;
            }

            case 1:
            {
                CBM_MIX2.bits.mixer_prio1 = u8LayerId;
                break;
            }

            default:
            {
                HAL_PRINT("Error priority id found in %s: L%d\n", __FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }

        HAL_WriteReg((HI_U32*) & (pVoReg->CBM_MIX2.u32), CBM_MIX2.u32);
    }
    // Settings for v1 and g1
    else if (HAL_CBMMIX3 == u8MixerId)
    {
        // TODO
        //check layer availability
        switch (enLayer)
        {
            case HAL_DISP_LAYER_VHD1:
            {
                // TODO
                u8LayerId = 0x1;
                break;
            }

            case HAL_DISP_LAYER_GFX1:
            {
                // TODO
                u8LayerId = 0x3;
                break;
            }

            default:
            {
                HAL_PRINT("Error layer id %d found in %s: L%d\n", enLayer, __FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }

        //set mixer prio
        CBM_MIX3.u32 = HAL_ReadReg((HI_U32*) & (pVoReg->CBM_MIX3.u32));

        switch (u8Prio)
        {
            case 0:
            {
                CBM_MIX3.bits.mixer_prio0 = u8LayerId;
                break;
            }

            case 1:
            {
                CBM_MIX3.bits.mixer_prio1 = u8LayerId;
                break;
            }

            case 2:
            {
                CBM_MIX3.bits.mixer_prio2 = u8LayerId;
                break;
            }

            default:
            {
                HAL_PRINT("Error priority id found in %s: L%d\n", __FUNCTION__, __LINE__);
                return HI_FALSE;
            }
        }

        HAL_WriteReg((HI_U32*) & (pVoReg->CBM_MIX3.u32), CBM_MIX3.u32);
    }
    else
    {
        HAL_PRINT("Error, u8MixerId %d Vou_SetCbmMixerPrio() Set a wrong CBMMIX\n", u8MixerId);
        return HI_FALSE;
    }


    return HI_TRUE;
}

HI_VOID HAL_HIHDR_G_SetHihdrR2yScale2p(HI_U32 hihdr_r2y_scale2p)
{
    U_HIHDR_G_RGB2YUV_SCALE2P HIHDR_G_RGB2YUV_SCALE2P;

    HIHDR_G_RGB2YUV_SCALE2P.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_SCALE2P.u32));
    HIHDR_G_RGB2YUV_SCALE2P.bits.hihdr_r2y_scale2p = hihdr_r2y_scale2p;
    HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_SCALE2P.u32), HIHDR_G_RGB2YUV_SCALE2P.u32);

    return ;
}

HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMin(HI_U32 hihdr_r2y_clip_min)
{
    U_HIHDR_G_RGB2YUV_MIN HIHDR_G_RGB2YUV_MIN;

    HIHDR_G_RGB2YUV_MIN.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_MIN.u32));
    HIHDR_G_RGB2YUV_MIN.bits.hihdr_r2y_clip_min = hihdr_r2y_clip_min;
    HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_MIN.u32), HIHDR_G_RGB2YUV_MIN.u32);

    return ;
}


HI_VOID HAL_HIHDR_G_SetHihdrR2yClipMax(HI_U32 hihdr_r2y_clip_max)
{
    U_HIHDR_G_RGB2YUV_MAX HIHDR_G_RGB2YUV_MAX;

    HIHDR_G_RGB2YUV_MAX.u32 = HAL_ReadReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_MAX.u32));
    HIHDR_G_RGB2YUV_MAX.bits.hihdr_r2y_clip_max = hihdr_r2y_clip_max;
    HAL_WriteReg((HI_U32 *)&(pVoHippReg->HIHDR_G_RGB2YUV_MAX.u32), HIHDR_G_RGB2YUV_MAX.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHzmeCkGtEn(HI_U32 hzme_ck_gt_en)
{
    U_V0_ZME_HINFO V0_ZME_HINFO;

    V0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HINFO.u32));
    V0_ZME_HINFO.bits.hzme_ck_gt_en = hzme_ck_gt_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HINFO.u32),V0_ZME_HINFO.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetOutWidth(HI_U32 out_width)
{
    U_V0_ZME_HINFO V0_ZME_HINFO;

    V0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HINFO.u32));
    V0_ZME_HINFO.bits.out_width = out_width;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HINFO.u32),V0_ZME_HINFO.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetLhfirEn(HI_U32 lhfir_en)
{
    U_V0_ZME_HSP V0_ZME_HSP;

    V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32));
    V0_ZME_HSP.bits.lhfir_en = lhfir_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32),V0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetChfirEn(HI_U32 chfir_en)
{
    U_V0_ZME_HSP V0_ZME_HSP;

    V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32));
    V0_ZME_HSP.bits.chfir_en = chfir_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32),V0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetLhmidEn(HI_U32 lhmid_en)
{
    U_V0_ZME_HSP V0_ZME_HSP;

    V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32));
    V0_ZME_HSP.bits.lhmid_en = lhmid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32),V0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetChmidEn(HI_U32 chmid_en)
{
    U_V0_ZME_HSP V0_ZME_HSP;

    V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32));
    V0_ZME_HSP.bits.chmid_en = chmid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32),V0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetNonLnrEn(HI_U32 non_lnr_en)
{
    U_V0_ZME_HSP V0_ZME_HSP;

    V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32));
    V0_ZME_HSP.bits.non_lnr_en = non_lnr_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32),V0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetLhfirMode(HI_U32 lhfir_mode)
{
    U_V0_ZME_HSP V0_ZME_HSP;

    V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32));
    V0_ZME_HSP.bits.lhfir_mode = lhfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32),V0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetChfirMode(HI_U32 chfir_mode)
{
    U_V0_ZME_HSP V0_ZME_HSP;

    V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32));
    V0_ZME_HSP.bits.chfir_mode = chfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32),V0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHfirOrder(HI_U32 hfir_order)
{
    U_V0_ZME_HSP V0_ZME_HSP;

    V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32));
    V0_ZME_HSP.bits.hfir_order = hfir_order;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32),V0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHratio(HI_U32 hratio)
{
    U_V0_ZME_HSP V0_ZME_HSP;

    V0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32));
    V0_ZME_HSP.bits.hratio = hratio;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HSP.u32),V0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetLhfirOffset(HI_U32 lhfir_offset)
{
    U_V0_ZME_HLOFFSET V0_ZME_HLOFFSET;

    V0_ZME_HLOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HLOFFSET.u32));
    V0_ZME_HLOFFSET.bits.lhfir_offset = lhfir_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HLOFFSET.u32),V0_ZME_HLOFFSET.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetChfirOffset(HI_U32 chfir_offset)
{
    U_V0_ZME_HCOFFSET V0_ZME_HCOFFSET;

    V0_ZME_HCOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HCOFFSET.u32));
    V0_ZME_HCOFFSET.bits.chfir_offset = chfir_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HCOFFSET.u32),V0_ZME_HCOFFSET.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetZone0Delta(HI_U32 zone0_delta)
{
    U_V0_ZME_HZONE0DELTA V0_ZME_HZONE0DELTA;

    V0_ZME_HZONE0DELTA.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HZONE0DELTA.u32));
    V0_ZME_HZONE0DELTA.bits.zone0_delta = zone0_delta;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HZONE0DELTA.u32),V0_ZME_HZONE0DELTA.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetZone2Delta(HI_U32 zone2_delta)
{
    U_V0_ZME_HZONE2DELTA V0_ZME_HZONE2DELTA;

    V0_ZME_HZONE2DELTA.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HZONE2DELTA.u32));
    V0_ZME_HZONE2DELTA.bits.zone2_delta = zone2_delta;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HZONE2DELTA.u32),V0_ZME_HZONE2DELTA.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetZone1End(HI_U32 zone1_end)
{
    U_V0_ZME_HZONEEND V0_ZME_HZONEEND;

    V0_ZME_HZONEEND.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HZONEEND.u32));
    V0_ZME_HZONEEND.bits.zone1_end = zone1_end;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HZONEEND.u32),V0_ZME_HZONEEND.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetZone0End(HI_U32 zone0_end)
{
    U_V0_ZME_HZONEEND V0_ZME_HZONEEND;

    V0_ZME_HZONEEND.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HZONEEND.u32));
    V0_ZME_HZONEEND.bits.zone0_end = zone0_end;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HZONEEND.u32),V0_ZME_HZONEEND.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHlShootctrlEn(HI_U32 hl_shootctrl_en)
{
    U_V0_ZME_HL_SHOOTCTRL V0_ZME_HL_SHOOTCTRL;

    V0_ZME_HL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32));
    V0_ZME_HL_SHOOTCTRL.bits.hl_shootctrl_en = hl_shootctrl_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32),V0_ZME_HL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHlShootctrlMode(HI_U32 hl_shootctrl_mode)
{
    U_V0_ZME_HL_SHOOTCTRL V0_ZME_HL_SHOOTCTRL;

    V0_ZME_HL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32));
    V0_ZME_HL_SHOOTCTRL.bits.hl_shootctrl_mode = hl_shootctrl_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32),V0_ZME_HL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHlFlatdectMode(HI_U32 hl_flatdect_mode)
{
    U_V0_ZME_HL_SHOOTCTRL V0_ZME_HL_SHOOTCTRL;

    V0_ZME_HL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32));
    V0_ZME_HL_SHOOTCTRL.bits.hl_flatdect_mode = hl_flatdect_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32),V0_ZME_HL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHlCoringadjEn(HI_U32 hl_coringadj_en)
{
    U_V0_ZME_HL_SHOOTCTRL V0_ZME_HL_SHOOTCTRL;

    V0_ZME_HL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32));
    V0_ZME_HL_SHOOTCTRL.bits.hl_coringadj_en = hl_coringadj_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32),V0_ZME_HL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHlGain(HI_U32 hl_gain)
{
    U_V0_ZME_HL_SHOOTCTRL V0_ZME_HL_SHOOTCTRL;

    V0_ZME_HL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32));
    V0_ZME_HL_SHOOTCTRL.bits.hl_gain = hl_gain;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32),V0_ZME_HL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHlCoring(HI_U32 hl_coring)
{
    U_V0_ZME_HL_SHOOTCTRL V0_ZME_HL_SHOOTCTRL;

    V0_ZME_HL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32));
    V0_ZME_HL_SHOOTCTRL.bits.hl_coring = hl_coring;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HL_SHOOTCTRL.u32),V0_ZME_HL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHcShootctrlEn(HI_U32 hc_shootctrl_en)
{
    U_V0_ZME_HC_SHOOTCTRL V0_ZME_HC_SHOOTCTRL;

    V0_ZME_HC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32));
    V0_ZME_HC_SHOOTCTRL.bits.hc_shootctrl_en = hc_shootctrl_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32),V0_ZME_HC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHcShootctrlMode(HI_U32 hc_shootctrl_mode)
{
    U_V0_ZME_HC_SHOOTCTRL V0_ZME_HC_SHOOTCTRL;

    V0_ZME_HC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32));
    V0_ZME_HC_SHOOTCTRL.bits.hc_shootctrl_mode = hc_shootctrl_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32),V0_ZME_HC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHcFlatdectMode(HI_U32 hc_flatdect_mode)
{
    U_V0_ZME_HC_SHOOTCTRL V0_ZME_HC_SHOOTCTRL;

    V0_ZME_HC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32));
    V0_ZME_HC_SHOOTCTRL.bits.hc_flatdect_mode = hc_flatdect_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32),V0_ZME_HC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHcCoringadjEn(HI_U32 hc_coringadj_en)
{
    U_V0_ZME_HC_SHOOTCTRL V0_ZME_HC_SHOOTCTRL;

    V0_ZME_HC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32));
    V0_ZME_HC_SHOOTCTRL.bits.hc_coringadj_en = hc_coringadj_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32),V0_ZME_HC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHcGain(HI_U32 hc_gain)
{
    U_V0_ZME_HC_SHOOTCTRL V0_ZME_HC_SHOOTCTRL;

    V0_ZME_HC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32));
    V0_ZME_HC_SHOOTCTRL.bits.hc_gain = hc_gain;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32),V0_ZME_HC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetHcCoring(HI_U32 hc_coring)
{
    U_V0_ZME_HC_SHOOTCTRL V0_ZME_HC_SHOOTCTRL;

    V0_ZME_HC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32));
    V0_ZME_HC_SHOOTCTRL.bits.hc_coring = hc_coring;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HC_SHOOTCTRL.u32),V0_ZME_HC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetApbVhdHfLren(HI_U32 apb_vhd_hf_lren)
{
    U_V0_ZME_HCOEF_REN V0_ZME_HCOEF_REN;

    V0_ZME_HCOEF_REN.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HCOEF_REN.u32));
    V0_ZME_HCOEF_REN.bits.apb_vhd_hf_lren = apb_vhd_hf_lren;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HCOEF_REN.u32),V0_ZME_HCOEF_REN.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetApbVhdHfCren(HI_U32 apb_vhd_hf_cren)
{
    U_V0_ZME_HCOEF_REN V0_ZME_HCOEF_REN;

    V0_ZME_HCOEF_REN.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HCOEF_REN.u32));
    V0_ZME_HCOEF_REN.bits.apb_vhd_hf_cren = apb_vhd_hf_cren;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HCOEF_REN.u32),V0_ZME_HCOEF_REN.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetApbVhdHcoefRaddr(HI_U32 apb_vhd_hcoef_raddr)
{
    U_V0_ZME_HCOEF_RDATA V0_ZME_HCOEF_RDATA;

    V0_ZME_HCOEF_RDATA.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_HCOEF_RDATA.u32));
    V0_ZME_HCOEF_RDATA.bits.apb_vhd_hcoef_raddr = apb_vhd_hcoef_raddr;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_HCOEF_RDATA.u32),V0_ZME_HCOEF_RDATA.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVzmeCkGtEn(HI_U32 vzme_ck_gt_en)
{
    U_V0_ZME_VINFO V0_ZME_VINFO;

    V0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VINFO.u32));
    V0_ZME_VINFO.bits.vzme_ck_gt_en = vzme_ck_gt_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VINFO.u32),V0_ZME_VINFO.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetOutPro(HI_U32 out_pro)
{
    U_V0_ZME_VINFO V0_ZME_VINFO;

    V0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VINFO.u32));
    V0_ZME_VINFO.bits.out_pro = out_pro;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VINFO.u32),V0_ZME_VINFO.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetOutFmt(HI_U32 out_fmt)
{
    U_V0_ZME_VINFO V0_ZME_VINFO;

    V0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VINFO.u32));
    V0_ZME_VINFO.bits.out_fmt = out_fmt;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VINFO.u32),V0_ZME_VINFO.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetOutHeight(HI_U32 out_height)
{
    U_V0_ZME_VINFO V0_ZME_VINFO;

    V0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VINFO.u32));
    V0_ZME_VINFO.bits.out_height = out_height;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VINFO.u32),V0_ZME_VINFO.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetLvfirEn(HI_U32 lvfir_en)
{
    U_V0_ZME_VSP V0_ZME_VSP;

    V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32));
    V0_ZME_VSP.bits.lvfir_en = lvfir_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32),V0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetCvfirEn(HI_U32 cvfir_en)
{
    U_V0_ZME_VSP V0_ZME_VSP;

    V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32));
    V0_ZME_VSP.bits.cvfir_en = cvfir_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32),V0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetLvmidEn(HI_U32 lvmid_en)
{
    U_V0_ZME_VSP V0_ZME_VSP;

    V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32));
    V0_ZME_VSP.bits.lvmid_en = lvmid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32),V0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetCvmidEn(HI_U32 cvmid_en)
{
    U_V0_ZME_VSP V0_ZME_VSP;

    V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32));
    V0_ZME_VSP.bits.cvmid_en = cvmid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32),V0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVfir1tapEn(HI_U32 vfir_1tap_en)
{
    U_V0_ZME_VSP V0_ZME_VSP;

    V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32));
    V0_ZME_VSP.bits.vfir_1tap_en = vfir_1tap_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32),V0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetLvfirMode(HI_U32 lvfir_mode)
{
    U_V0_ZME_VSP V0_ZME_VSP;

    V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32));
    V0_ZME_VSP.bits.lvfir_mode = lvfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32),V0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetCvfirMode(HI_U32 cvfir_mode)
{
    U_V0_ZME_VSP V0_ZME_VSP;

    V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32));
    V0_ZME_VSP.bits.cvfir_mode = cvfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32),V0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetGraphdetEn(HI_U32 graphdet_en)
{
    U_V0_ZME_VSP V0_ZME_VSP;

    V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32));
    V0_ZME_VSP.bits.graphdet_en = graphdet_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32),V0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVratio(HI_U32 vratio)
{
    U_V0_ZME_VSP V0_ZME_VSP;

    V0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32));
    V0_ZME_VSP.bits.vratio = vratio;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VSP.u32),V0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVlumaOffset(HI_U32 vluma_offset)
{
    U_V0_ZME_VOFFSET V0_ZME_VOFFSET;

    V0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VOFFSET.u32));
    V0_ZME_VOFFSET.bits.vluma_offset = vluma_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VOFFSET.u32),V0_ZME_VOFFSET.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVchromaOffset(HI_U32 vchroma_offset)
{
    U_V0_ZME_VOFFSET V0_ZME_VOFFSET;

    V0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VOFFSET.u32));
    V0_ZME_VOFFSET.bits.vchroma_offset = vchroma_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VOFFSET.u32),V0_ZME_VOFFSET.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVblumaOffset(HI_U32 vbluma_offset)
{
    U_V0_ZME_VBOFFSET V0_ZME_VBOFFSET;

    V0_ZME_VBOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VBOFFSET.u32));
    V0_ZME_VBOFFSET.bits.vbluma_offset = vbluma_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VBOFFSET.u32),V0_ZME_VBOFFSET.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVbchromaOffset(HI_U32 vbchroma_offset)
{
    U_V0_ZME_VBOFFSET V0_ZME_VBOFFSET;

    V0_ZME_VBOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VBOFFSET.u32));
    V0_ZME_VBOFFSET.bits.vbchroma_offset = vbchroma_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VBOFFSET.u32),V0_ZME_VBOFFSET.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVlShootctrlEn(HI_U32 vl_shootctrl_en)
{
    U_V0_ZME_VL_SHOOTCTRL V0_ZME_VL_SHOOTCTRL;

    V0_ZME_VL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32));
    V0_ZME_VL_SHOOTCTRL.bits.vl_shootctrl_en = vl_shootctrl_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32),V0_ZME_VL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVlShootctrlMode(HI_U32 vl_shootctrl_mode)
{
    U_V0_ZME_VL_SHOOTCTRL V0_ZME_VL_SHOOTCTRL;

    V0_ZME_VL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32));
    V0_ZME_VL_SHOOTCTRL.bits.vl_shootctrl_mode = vl_shootctrl_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32),V0_ZME_VL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVlFlatdectMode(HI_U32 vl_flatdect_mode)
{
    U_V0_ZME_VL_SHOOTCTRL V0_ZME_VL_SHOOTCTRL;

    V0_ZME_VL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32));
    V0_ZME_VL_SHOOTCTRL.bits.vl_flatdect_mode = vl_flatdect_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32),V0_ZME_VL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVlCoringadjEn(HI_U32 vl_coringadj_en)
{
    U_V0_ZME_VL_SHOOTCTRL V0_ZME_VL_SHOOTCTRL;

    V0_ZME_VL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32));
    V0_ZME_VL_SHOOTCTRL.bits.vl_coringadj_en = vl_coringadj_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32),V0_ZME_VL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVlGain(HI_U32 vl_gain)
{
    U_V0_ZME_VL_SHOOTCTRL V0_ZME_VL_SHOOTCTRL;

    V0_ZME_VL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32));
    V0_ZME_VL_SHOOTCTRL.bits.vl_gain = vl_gain;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32),V0_ZME_VL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVlCoring(HI_U32 vl_coring)
{
    U_V0_ZME_VL_SHOOTCTRL V0_ZME_VL_SHOOTCTRL;

    V0_ZME_VL_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32));
    V0_ZME_VL_SHOOTCTRL.bits.vl_coring = vl_coring;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VL_SHOOTCTRL.u32),V0_ZME_VL_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVcShootctrlEn(HI_U32 vc_shootctrl_en)
{
    U_V0_ZME_VC_SHOOTCTRL V0_ZME_VC_SHOOTCTRL;

    V0_ZME_VC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32));
    V0_ZME_VC_SHOOTCTRL.bits.vc_shootctrl_en = vc_shootctrl_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32),V0_ZME_VC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVcShootctrlMode(HI_U32 vc_shootctrl_mode)
{
    U_V0_ZME_VC_SHOOTCTRL V0_ZME_VC_SHOOTCTRL;

    V0_ZME_VC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32));
    V0_ZME_VC_SHOOTCTRL.bits.vc_shootctrl_mode = vc_shootctrl_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32),V0_ZME_VC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVcFlatdectMode(HI_U32 vc_flatdect_mode)
{
    U_V0_ZME_VC_SHOOTCTRL V0_ZME_VC_SHOOTCTRL;

    V0_ZME_VC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32));
    V0_ZME_VC_SHOOTCTRL.bits.vc_flatdect_mode = vc_flatdect_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32),V0_ZME_VC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVcCoringadjEn(HI_U32 vc_coringadj_en)
{
    U_V0_ZME_VC_SHOOTCTRL V0_ZME_VC_SHOOTCTRL;

    V0_ZME_VC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32));
    V0_ZME_VC_SHOOTCTRL.bits.vc_coringadj_en = vc_coringadj_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32),V0_ZME_VC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVcGain(HI_U32 vc_gain)
{
    U_V0_ZME_VC_SHOOTCTRL V0_ZME_VC_SHOOTCTRL;

    V0_ZME_VC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32));
    V0_ZME_VC_SHOOTCTRL.bits.vc_gain = vc_gain;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32),V0_ZME_VC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetVcCoring(HI_U32 vc_coring)
{
    U_V0_ZME_VC_SHOOTCTRL V0_ZME_VC_SHOOTCTRL;

    V0_ZME_VC_SHOOTCTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32));
    V0_ZME_VC_SHOOTCTRL.bits.vc_coring = vc_coring;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VC_SHOOTCTRL.u32),V0_ZME_VC_SHOOTCTRL.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetApbVhdVfLren(HI_U32 apb_vhd_vf_lren)
{
    U_V0_ZME_VCOEF_REN V0_ZME_VCOEF_REN;

    V0_ZME_VCOEF_REN.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VCOEF_REN.u32));
    V0_ZME_VCOEF_REN.bits.apb_vhd_vf_lren = apb_vhd_vf_lren;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VCOEF_REN.u32),V0_ZME_VCOEF_REN.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetApbVhdVfCren(HI_U32 apb_vhd_vf_cren)
{
    U_V0_ZME_VCOEF_REN V0_ZME_VCOEF_REN;

    V0_ZME_VCOEF_REN.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VCOEF_REN.u32));
    V0_ZME_VCOEF_REN.bits.apb_vhd_vf_cren = apb_vhd_vf_cren;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VCOEF_REN.u32),V0_ZME_VCOEF_REN.u32);

    return ;
}


HI_VOID HAL_V0_ZME_SetApbVhdVcoefRaddr(HI_U32 apb_vhd_vcoef_raddr)
{
    U_V0_ZME_VCOEF_RDATA V0_ZME_VCOEF_RDATA;

    V0_ZME_VCOEF_RDATA.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_ZME_VCOEF_RDATA.u32));
    V0_ZME_VCOEF_RDATA.bits.apb_vhd_vcoef_raddr = apb_vhd_vcoef_raddr;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_ZME_VCOEF_RDATA.u32),V0_ZME_VCOEF_RDATA.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetCkGtEn(HI_U32 ck_gt_en)
{
    U_V0_HFIR_CTRL V0_HFIR_CTRL;

    V0_HFIR_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIR_CTRL.u32));
    V0_HFIR_CTRL.bits.ck_gt_en = ck_gt_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIR_CTRL.u32),V0_HFIR_CTRL.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetMidEn(HI_U32 mid_en)
{
    U_V0_HFIR_CTRL V0_HFIR_CTRL;

    V0_HFIR_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIR_CTRL.u32));
    V0_HFIR_CTRL.bits.mid_en = mid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIR_CTRL.u32),V0_HFIR_CTRL.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetHfirMode(HI_U32 hfir_mode)
{
    U_V0_HFIR_CTRL V0_HFIR_CTRL;

    V0_HFIR_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIR_CTRL.u32));
    V0_HFIR_CTRL.bits.hfir_mode = hfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIR_CTRL.u32),V0_HFIR_CTRL.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetCoef1(HI_U32 coef1)
{
    U_V0_HFIRCOEF01 V0_HFIRCOEF01;

    V0_HFIRCOEF01.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIRCOEF01.u32));
    V0_HFIRCOEF01.bits.coef1 = coef1;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIRCOEF01.u32),V0_HFIRCOEF01.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetCoef0(HI_U32 coef0)
{
    U_V0_HFIRCOEF01 V0_HFIRCOEF01;

    V0_HFIRCOEF01.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIRCOEF01.u32));
    V0_HFIRCOEF01.bits.coef0 = coef0;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIRCOEF01.u32),V0_HFIRCOEF01.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetCoef3(HI_U32 coef3)
{
    U_V0_HFIRCOEF23 V0_HFIRCOEF23;

    V0_HFIRCOEF23.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIRCOEF23.u32));
    V0_HFIRCOEF23.bits.coef3 = coef3;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIRCOEF23.u32),V0_HFIRCOEF23.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetCoef2(HI_U32 coef2)
{
    U_V0_HFIRCOEF23 V0_HFIRCOEF23;

    V0_HFIRCOEF23.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIRCOEF23.u32));
    V0_HFIRCOEF23.bits.coef2 = coef2;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIRCOEF23.u32),V0_HFIRCOEF23.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetCoef5(HI_U32 coef5)
{
    U_V0_HFIRCOEF45 V0_HFIRCOEF45;

    V0_HFIRCOEF45.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIRCOEF45.u32));
    V0_HFIRCOEF45.bits.coef5 = coef5;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIRCOEF45.u32),V0_HFIRCOEF45.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetCoef4(HI_U32 coef4)
{
    U_V0_HFIRCOEF45 V0_HFIRCOEF45;

    V0_HFIRCOEF45.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIRCOEF45.u32));
    V0_HFIRCOEF45.bits.coef4 = coef4;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIRCOEF45.u32),V0_HFIRCOEF45.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetCoef7(HI_U32 coef7)
{
    U_V0_HFIRCOEF67 V0_HFIRCOEF67;

    V0_HFIRCOEF67.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIRCOEF67.u32));
    V0_HFIRCOEF67.bits.coef7 = coef7;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIRCOEF67.u32),V0_HFIRCOEF67.u32);

    return ;
}


HI_VOID HAL_V0_HFIR_SetCoef6(HI_U32 coef6)
{
    U_V0_HFIRCOEF67 V0_HFIRCOEF67;

    V0_HFIRCOEF67.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V0_HFIRCOEF67.u32));
    V0_HFIRCOEF67.bits.coef6 = coef6;
    HAL_WriteReg((HI_U32*)&(pVoReg->V0_HFIRCOEF67.u32),V0_HFIRCOEF67.u32);

    return ;
}

HI_VOID HAL_V1_HFIR_SetCkGtEn(HI_U32 ck_gt_en)
{
    U_V1_HFIR_CTRL V1_HFIR_CTRL;

    V1_HFIR_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIR_CTRL.u32));
    V1_HFIR_CTRL.bits.ck_gt_en = ck_gt_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIR_CTRL.u32),V1_HFIR_CTRL.u32);

    return ;
}


HI_VOID HAL_V1_HFIR_SetMidEn(HI_U32 mid_en)
{
    U_V1_HFIR_CTRL V1_HFIR_CTRL;

    V1_HFIR_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIR_CTRL.u32));
    V1_HFIR_CTRL.bits.mid_en = mid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIR_CTRL.u32),V1_HFIR_CTRL.u32);

    return ;
}


HI_VOID HAL_V1_HFIR_SetHfirMode(HI_U32 hfir_mode)
{
    U_V1_HFIR_CTRL V1_HFIR_CTRL;

    V1_HFIR_CTRL.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIR_CTRL.u32));
    V1_HFIR_CTRL.bits.hfir_mode = hfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIR_CTRL.u32),V1_HFIR_CTRL.u32);

    return ;
}




HI_VOID HAL_V1_HFIR_SetCoef1(HI_U32 coef1)
{
    U_V1_HFIRCOEF01 V1_HFIRCOEF01;

    V1_HFIRCOEF01.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIRCOEF01.u32));
    V1_HFIRCOEF01.bits.coef1 = coef1;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIRCOEF01.u32),V1_HFIRCOEF01.u32);

    return ;
}


HI_VOID HAL_V1_HFIR_SetCoef0(HI_U32 coef0)
{
    U_V1_HFIRCOEF01 V1_HFIRCOEF01;

    V1_HFIRCOEF01.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIRCOEF01.u32));
    V1_HFIRCOEF01.bits.coef0 = coef0;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIRCOEF01.u32),V1_HFIRCOEF01.u32);

    return ;
}


HI_VOID HAL_V1_HFIR_SetCoef3(HI_U32 coef3)
{
    U_V1_HFIRCOEF23 V1_HFIRCOEF23;

    V1_HFIRCOEF23.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIRCOEF23.u32));
    V1_HFIRCOEF23.bits.coef3 = coef3;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIRCOEF23.u32),V1_HFIRCOEF23.u32);

    return ;
}


HI_VOID HAL_V1_HFIR_SetCoef2(HI_U32 coef2)
{
    U_V1_HFIRCOEF23 V1_HFIRCOEF23;

    V1_HFIRCOEF23.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIRCOEF23.u32));
    V1_HFIRCOEF23.bits.coef2 = coef2;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIRCOEF23.u32),V1_HFIRCOEF23.u32);

    return ;
}


HI_VOID HAL_V1_HFIR_SetCoef5(HI_U32 coef5)
{
    U_V1_HFIRCOEF45 V1_HFIRCOEF45;

    V1_HFIRCOEF45.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIRCOEF45.u32));
    V1_HFIRCOEF45.bits.coef5 = coef5;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIRCOEF45.u32),V1_HFIRCOEF45.u32);

    return ;
}


HI_VOID HAL_V1_HFIR_SetCoef4(HI_U32 coef4)
{
    U_V1_HFIRCOEF45 V1_HFIRCOEF45;

    V1_HFIRCOEF45.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIRCOEF45.u32));
    V1_HFIRCOEF45.bits.coef4 = coef4;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIRCOEF45.u32),V1_HFIRCOEF45.u32);

    return ;
}


HI_VOID HAL_V1_HFIR_SetCoef7(HI_U32 coef7)
{
    U_V1_HFIRCOEF67 V1_HFIRCOEF67;

    V1_HFIRCOEF67.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIRCOEF67.u32));
    V1_HFIRCOEF67.bits.coef7 = coef7;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIRCOEF67.u32),V1_HFIRCOEF67.u32);

    return ;
}


HI_VOID HAL_V1_HFIR_SetCoef6(HI_U32 coef6)
{
    U_V1_HFIRCOEF67 V1_HFIRCOEF67;

    V1_HFIRCOEF67.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_HFIRCOEF67.u32));
    V1_HFIRCOEF67.bits.coef6 = coef6;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_HFIRCOEF67.u32),V1_HFIRCOEF67.u32);

    return ;
}

HI_VOID HAL_V1_CVFIR_SetVzmeCkGtEn(HI_U32 vzme_ck_gt_en)
{
    U_V1_CVFIR_VINFO V1_CVFIR_VINFO;

    V1_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VINFO.u32));
    V1_CVFIR_VINFO.bits.vzme_ck_gt_en = vzme_ck_gt_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VINFO.u32),V1_CVFIR_VINFO.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetOutPro(HI_U32 out_pro)
{
    U_V1_CVFIR_VINFO V1_CVFIR_VINFO;

    V1_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VINFO.u32));
    V1_CVFIR_VINFO.bits.out_pro = out_pro;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VINFO.u32),V1_CVFIR_VINFO.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetOutFmt(HI_U32 out_fmt)
{
    U_V1_CVFIR_VINFO V1_CVFIR_VINFO;

    V1_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VINFO.u32));
    V1_CVFIR_VINFO.bits.out_fmt = out_fmt;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VINFO.u32),V1_CVFIR_VINFO.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetOutHeight(HI_U32 out_height)
{
    U_V1_CVFIR_VINFO V1_CVFIR_VINFO;

    V1_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VINFO.u32));
    V1_CVFIR_VINFO.bits.out_height = out_height;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VINFO.u32),V1_CVFIR_VINFO.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetCvfirEn(HI_U32 cvfir_en)
{
    U_V1_CVFIR_VSP V1_CVFIR_VSP;

    V1_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VSP.u32));
    V1_CVFIR_VSP.bits.cvfir_en = cvfir_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VSP.u32),V1_CVFIR_VSP.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetCvmidEn(HI_U32 cvmid_en)
{
    U_V1_CVFIR_VSP V1_CVFIR_VSP;

    V1_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VSP.u32));
    V1_CVFIR_VSP.bits.cvmid_en = cvmid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VSP.u32),V1_CVFIR_VSP.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetCvfirMode(HI_U32 cvfir_mode)
{
    U_V1_CVFIR_VSP V1_CVFIR_VSP;

    V1_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VSP.u32));
    V1_CVFIR_VSP.bits.cvfir_mode = cvfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VSP.u32),V1_CVFIR_VSP.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVratio(HI_U32 vratio)
{
    U_V1_CVFIR_VSP V1_CVFIR_VSP;

    V1_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VSP.u32));
    V1_CVFIR_VSP.bits.vratio = vratio;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VSP.u32),V1_CVFIR_VSP.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVchromaOffset(HI_U32 vchroma_offset)
{
    U_V1_CVFIR_VOFFSET V1_CVFIR_VOFFSET;

    V1_CVFIR_VOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VOFFSET.u32));
    V1_CVFIR_VOFFSET.bits.vchroma_offset = vchroma_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VOFFSET.u32),V1_CVFIR_VOFFSET.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVbchromaOffset(HI_U32 vbchroma_offset)
{
    U_V1_CVFIR_VBOFFSET V1_CVFIR_VBOFFSET;

    V1_CVFIR_VBOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VBOFFSET.u32));
    V1_CVFIR_VBOFFSET.bits.vbchroma_offset = vbchroma_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VBOFFSET.u32),V1_CVFIR_VBOFFSET.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVccoef00(HI_U32 vccoef00)
{
    U_V1_CVFIR_VCOEF0 V1_CVFIR_VCOEF0;

    V1_CVFIR_VCOEF0.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF0.u32));
    V1_CVFIR_VCOEF0.bits.vccoef00 = vccoef00;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF0.u32),V1_CVFIR_VCOEF0.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVccoef01(HI_U32 vccoef01)
{
    U_V1_CVFIR_VCOEF0 V1_CVFIR_VCOEF0;

    V1_CVFIR_VCOEF0.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF0.u32));
    V1_CVFIR_VCOEF0.bits.vccoef01 = vccoef01;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF0.u32),V1_CVFIR_VCOEF0.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVccoef02(HI_U32 vccoef02)
{
    U_V1_CVFIR_VCOEF0 V1_CVFIR_VCOEF0;

    V1_CVFIR_VCOEF0.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF0.u32));
    V1_CVFIR_VCOEF0.bits.vccoef02 = vccoef02;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF0.u32),V1_CVFIR_VCOEF0.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVccoef03(HI_U32 vccoef03)
{
    U_V1_CVFIR_VCOEF1 V1_CVFIR_VCOEF1;

    V1_CVFIR_VCOEF1.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF1.u32));
    V1_CVFIR_VCOEF1.bits.vccoef03 = vccoef03;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF1.u32),V1_CVFIR_VCOEF1.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVccoef10(HI_U32 vccoef10)
{
    U_V1_CVFIR_VCOEF1 V1_CVFIR_VCOEF1;

    V1_CVFIR_VCOEF1.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF1.u32));
    V1_CVFIR_VCOEF1.bits.vccoef10 = vccoef10;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF1.u32),V1_CVFIR_VCOEF1.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVccoef11(HI_U32 vccoef11)
{
    U_V1_CVFIR_VCOEF1 V1_CVFIR_VCOEF1;

    V1_CVFIR_VCOEF1.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF1.u32));
    V1_CVFIR_VCOEF1.bits.vccoef11 = vccoef11;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF1.u32),V1_CVFIR_VCOEF1.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVccoef12(HI_U32 vccoef12)
{
    U_V1_CVFIR_VCOEF2 V1_CVFIR_VCOEF2;

    V1_CVFIR_VCOEF2.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF2.u32));
    V1_CVFIR_VCOEF2.bits.vccoef12 = vccoef12;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF2.u32),V1_CVFIR_VCOEF2.u32);

    return ;
}


HI_VOID HAL_V1_CVFIR_SetVccoef13(HI_U32 vccoef13)
{
    U_V1_CVFIR_VCOEF2 V1_CVFIR_VCOEF2;

    V1_CVFIR_VCOEF2.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF2.u32));
    V1_CVFIR_VCOEF2.bits.vccoef13 = vccoef13;
    HAL_WriteReg((HI_U32*)&(pVoReg->V1_CVFIR_VCOEF2.u32),V1_CVFIR_VCOEF2.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetCkGtEn(HI_U32 ck_gt_en)
{
    U_G0_ZME_HINFO G0_ZME_HINFO;

    G0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HINFO.u32));
    G0_ZME_HINFO.bits.ck_gt_en = ck_gt_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HINFO.u32),G0_ZME_HINFO.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetOutWidth(HI_U32 out_width)
{
    U_G0_ZME_HINFO G0_ZME_HINFO;

    G0_ZME_HINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HINFO.u32));
    G0_ZME_HINFO.bits.out_width = out_width-1;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HINFO.u32),G0_ZME_HINFO.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetHfirEn(HI_U32 hfir_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.hfir_en = hfir_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetAhfirMidEn(HI_U32 ahfir_mid_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.ahfir_mid_en = ahfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLhfirMidEn(HI_U32 lhfir_mid_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.lhfir_mid_en = lhfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetChfirMidEn(HI_U32 chfir_mid_en)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.chfir_mid_en = chfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLhfirMode(HI_U32 lhfir_mode)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.lhfir_mode = lhfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetAhfirMode(HI_U32 ahfir_mode)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.ahfir_mode = ahfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetHfirOrder(HI_U32 hfir_order)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.hfir_order = hfir_order;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetHratio(HI_U32 hratio)
{
    U_G0_ZME_HSP G0_ZME_HSP;

    G0_ZME_HSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32));
    G0_ZME_HSP.bits.hratio = hratio;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HSP.u32),G0_ZME_HSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLhfirOffset(HI_U32 lhfir_offset)
{
    U_G0_ZME_HLOFFSET G0_ZME_HLOFFSET;

    G0_ZME_HLOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HLOFFSET.u32));
    G0_ZME_HLOFFSET.bits.lhfir_offset = lhfir_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HLOFFSET.u32),G0_ZME_HLOFFSET.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetChfirOffset(HI_U32 chfir_offset)
{
    U_G0_ZME_HCOFFSET G0_ZME_HCOFFSET;

    G0_ZME_HCOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_HCOFFSET.u32));
    G0_ZME_HCOFFSET.bits.chfir_offset = chfir_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_HCOFFSET.u32),G0_ZME_HCOFFSET.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetApbG0HfLren(HI_U32 apb_g0_hf_lren)
{
    U_G0_ZME_COEF_REN G0_ZME_COEF_REN;

    G0_ZME_COEF_REN.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_COEF_REN.u32));
    G0_ZME_COEF_REN.bits.apb_g0_hf_lren = apb_g0_hf_lren;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_COEF_REN.u32),G0_ZME_COEF_REN.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetApbG0VfLren(HI_U32 apb_g0_vf_lren)
{
    U_G0_ZME_COEF_REN G0_ZME_COEF_REN;

    G0_ZME_COEF_REN.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_COEF_REN.u32));
    G0_ZME_COEF_REN.bits.apb_g0_vf_lren = apb_g0_vf_lren;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_COEF_REN.u32),G0_ZME_COEF_REN.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetApbVhdCoefRaddr(HI_U32 apb_vhd_coef_raddr)
{
    U_G0_ZME_COEF_RDATA G0_ZME_COEF_RDATA;

    G0_ZME_COEF_RDATA.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_COEF_RDATA.u32));
    G0_ZME_COEF_RDATA.bits.apb_vhd_coef_raddr = apb_vhd_coef_raddr;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_COEF_RDATA.u32),G0_ZME_COEF_RDATA.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetOutPro(HI_U32 out_pro)
{
    U_G0_ZME_VINFO G0_ZME_VINFO;

    G0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VINFO.u32));
    G0_ZME_VINFO.bits.out_pro = out_pro;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VINFO.u32),G0_ZME_VINFO.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetOutHeight(HI_U32 out_height)
{
    U_G0_ZME_VINFO G0_ZME_VINFO;

    G0_ZME_VINFO.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VINFO.u32));
    G0_ZME_VINFO.bits.out_height = out_height-1;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VINFO.u32),G0_ZME_VINFO.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVfirEn(HI_U32 vfir_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.vfir_en = vfir_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetAvfirMidEn(HI_U32 avfir_mid_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.avfir_mid_en = avfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLvfirMidEn(HI_U32 lvfir_mid_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.lvfir_mid_en = lvfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetCvfirMidEn(HI_U32 cvfir_mid_en)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.cvfir_mid_en = cvfir_mid_en;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetLvfirMode(HI_U32 lvfir_mode)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.lvfir_mode = lvfir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVafirMode(HI_U32 vafir_mode)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.vafir_mode = vafir_mode;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVratio(HI_U32 vratio)
{
    U_G0_ZME_VSP G0_ZME_VSP;

    G0_ZME_VSP.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32));
    G0_ZME_VSP.bits.vratio = vratio;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VSP.u32),G0_ZME_VSP.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVtpOffset(HI_U32 vtp_offset)
{
    U_G0_ZME_VOFFSET G0_ZME_VOFFSET;

    G0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VOFFSET.u32));
    G0_ZME_VOFFSET.bits.vtp_offset = vtp_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VOFFSET.u32),G0_ZME_VOFFSET.u32);

    return ;
}


HI_VOID HAL_G0_ZME_SetVbtmOffset(HI_U32 vbtm_offset)
{
    U_G0_ZME_VOFFSET G0_ZME_VOFFSET;

    G0_ZME_VOFFSET.u32 = HAL_ReadReg((HI_U32*)&(pVoReg->G0_ZME_VOFFSET.u32));
    G0_ZME_VOFFSET.bits.vbtm_offset = vbtm_offset;
    HAL_WriteReg((HI_U32*)&(pVoReg->G0_ZME_VOFFSET.u32),G0_ZME_VOFFSET.u32);

    return ;
}


#ifdef __cplusplus
#if __cplusplus
 }
#endif
#endif /* End of #ifdef __cplusplus */

