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

#include "hi3519av100_vo.h"

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
#define CRG_REGS_ADDR   0x04510000
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
#define CRG_PERCTL59_ADDR (0x00ec + CRG_REGS_ADDR)


#define CRG_PERCTL64_ADDR (0x0100 + CRG_REGS_ADDR)   /* VDP low power CRG */
#define CRG_PERCTL72_ADDR (0x0120 + CRG_REGS_ADDR)
#define CRG_PERCTL73_ADDR (0x0124 + CRG_REGS_ADDR)   /* VDP CRG */
#define CRG_PERCTL15_ADDR (0x003C + CRG_REGS_ADDR)

#if 0
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
#endif

#define LCD_CRG_PERCTL_ADDR CRG_PERCTL72_ADDR
#define VOU_CRG_PERCTL_ADDR CRG_PERCTL73_ADDR



/****************************************************************************
 * EXTERN VARIABLES                                                         *
 ****************************************************************************/

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
volatile S_VDP_REGS_TYPE*   pVoReg     = NULL;

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
    pVoReg     = (volatile S_VDP_REGS_TYPE *)IO_ADDRESS(VO_BASE_ADDR);
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


HI_S32 SYS_HAL_SelVoBt1120PinConifg(HI_VOID)
{
#if 0
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
#endif
    return 0;
}

HI_BOOL HAL_INTF_BT_SetDfirEn(HI_U32 dfir_en)
{
    U_INTF_BT_CTRL INTF_BT_CTRL;

    INTF_BT_CTRL.u32 = HAL_ReadReg((HI_U32*)(HI_UL) & (pVoReg->INTF_BT_CTRL.u32));
    INTF_BT_CTRL.bits.dfir_en = dfir_en;
    HAL_WriteReg((HI_U32*)(HI_UL) & (pVoReg->INTF_BT_CTRL.u32), INTF_BT_CTRL.u32);

    return HI_TRUE;
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



HI_S32 SYS_HAL_VouBusResetSel(HI_BOOL bReset)
{
    HI_U32 u32Tmp = (bReset == HI_TRUE) ? 1 : 0;
    HI_RegSetBit(u32Tmp, 0, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouAPBClkEn(HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;
    HI_RegSetBit(u32Tmp, 1, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouBusClkEn(HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;
    HI_RegSetBit(u32Tmp, 2, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouCFGClkEn(HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;
    HI_RegSetBit(u32Tmp, 3, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouDevClkEn(HI_S32 s32VoDev, HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    if (0 == s32VoDev)
    {
        HI_RegSetBit(u32Tmp, 5, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));
        HI_RegSetBit(u32Tmp, 6, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));
    }
    else if (1 == s32VoDev)
    {
        HI_RegSetBit(u32Tmp, 4, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));
        HI_RegSetBit(u32Tmp, 7, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}


HI_S32 SYS_HAL_VouBtClkEn(HI_BOOL pbBtClkEn)
{
    HI_U32 u32Tmp = (pbBtClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 8, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));
    HI_RegSetBit(u32Tmp, 9, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouHdmiClkEn(HI_BOOL pbHdmiClkEn)
{
    HI_U32 u32Tmp = (pbHdmiClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 10, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));
    HI_RegSetBit(u32Tmp, 27, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouMipiClkEn(HI_BOOL pbMipiClkEn)
{
    HI_U32 u32Tmp = (pbMipiClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 11, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));
    HI_RegSetBit(u32Tmp, 28, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_BOOL HAL_DISP_SetIntvsyncTeMode(HAL_DISP_OUTPUTCHANNEL_E enChan, HI_BOOL bSlave)
{
    volatile U_DHD0_PRECHARGE_THD     DHD0_PRECHARGE_THD;
    volatile  HI_UL addr_REG;

    switch (enChan)
    {
        case HAL_DISP_CHANNEL_DHD0:
        case HAL_DISP_CHANNEL_DHD1:
        {
            addr_REG = Vou_GetChnAbsAddr(enChan, (HI_UL) & (pVoReg->DHD0_PRECHARGE_THD.u32));
            DHD0_PRECHARGE_THD.u32 = HAL_ReadReg((HI_U32*)addr_REG);

            DHD0_PRECHARGE_THD.bits.vsync_te_mode = bSlave;
            HAL_WriteReg((HI_U32*)addr_REG, DHD0_PRECHARGE_THD.u32);
            break;
        }

        default:
        {

            return HI_FALSE;
        }
    }

    return HI_TRUE;
}

HI_S32 SYS_HAL_VouHd0DivMode(HI_S32 s32Dev, HI_U32 u32Hd0DivMod)
{
    if (0 == s32Dev)
    {
        HI_RegWrite32(u32Hd0DivMod << 12, 0x3 << 12, IO_ADDRESS(CRG_PERCTL73_ADDR));
    }
    else
    {
        return -1;
    }

    return 0;
}

HI_S32 SYS_HAL_VouHd1DivMode(HI_U32 u32Hd1DivMod)
{
    HI_RegWrite32(u32Hd1DivMod << 14, 0x3 << 14, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));
    return 0;
}

HI_S32 SYS_HAL_VouHd0PPCSel(HI_U32 u32ClkSel)
{
    HI_RegWrite32(u32ClkSel << 16, 0x3 << 16, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}


HI_S32 SYS_HAL_VouOutClkSel(HI_U32 u32ClkSel)
{
    HI_RegWrite32(u32ClkSel << 18, 0x7 << 18, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouHd1PPCSel(HI_U32 u32ClkSel)
{
    HI_RegWrite32(u32ClkSel << 21, 0x3 << 21, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}


HI_S32 SYS_HAL_VouBtClkSel(HI_U32 pbBtClkChSel)
{
    HI_RegWrite32(pbBtClkChSel << 25, 0x1 << 25, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}


HI_S32 SYS_HAL_VouHdOutPctrl(HI_BOOL pbClkReverse)
{

    HI_U32 u32Tmp = (pbClkReverse == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 29, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouMipiChnSel(HI_U32 pbMipiChSel)
{
    HI_RegWrite32(pbMipiChSel << 30, 0x1 << 30, IO_ADDRESS(VOU_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_VouLcdClkEn(HI_BOOL pbClkEn)
{
    HI_U32 u32Tmp = (pbClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 27, IO_ADDRESS(LCD_CRG_PERCTL_ADDR));

    return 0;
}

HI_S32 SYS_HAL_LcdMclkDiv(HI_U32 u32MclkDiv)
{
    HI_RegWrite32(u32MclkDiv, 0x7ffffff, IO_ADDRESS(LCD_CRG_PERCTL_ADDR));

    return 0;
}


HI_S32 SYS_HAL_LcdDataMode(HI_U32 u32DataMode)
{
    HI_RegWrite32(u32DataMode << 26, 0x7 << 26, IO_ADDRESS(0x04528004));

    return 0;
}



HI_S32 SYS_HAL_SetVoPllFrac(HI_S32 s32Pll, HI_U32 u32BitsSet)
{
    if (0 == s32Pll)
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


HI_S32 SYS_HAL_SetVoLowPowerCtrl(HI_BOOL bLowPowerClkEn)
{
    HI_U32 u32Tmp = (bLowPowerClkEn == HI_TRUE) ? 1 : 0;

    HI_RegSetBit(u32Tmp, 1, IO_ADDRESS(CRG_PERCTL59_ADDR));

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

            return 0;
        }
    }

    return RegAbsAddr;
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

        return HI_FALSE;
    }


    return HI_TRUE;

}




HI_BOOL HAL_DISP_SetIntfSync(HAL_DISP_OUTPUTCHANNEL_E enChan,
                             HAL_DISP_SYNCINFO_S* pstSyncInfo, HAL_DISP_SYNCINV_S* pstInv)
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
            DHD0_CTRL.bits.iop      = pstSyncInfo->bIop;
            DHD0_CTRL.bits.intf_ihs = pstInv->u32hs_inv;
            DHD0_CTRL.bits.intf_ivs = pstInv->u32vs_inv;
            DHD0_CTRL.bits.intf_idv = pstInv->u32dv_inv;
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

            return HI_FALSE;
        }
    }
    return HI_TRUE;
}

HI_VOID HAL_VIDEO_HFIR_SetCkGtEn(HAL_DISP_LAYER_E enLayer, HI_U32 ck_gt_en)
{
    U_V0_HFIR_CTRL V0_HFIR_CTRL;
    volatile HI_U32  addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_HFIR_CTRL.u32));
        V0_HFIR_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HFIR_CTRL.bits.ck_gt_en = ck_gt_en;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HFIR_CTRL.u32);
    }

    return ;
}


HI_VOID HAL_VIDEO_CVFIR_SetOutHeight(HAL_DISP_LAYER_E enLayer, HI_U32 out_height)
{
    U_V0_CVFIR_VINFO V0_CVFIR_VINFO;
    volatile HI_U32  addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VINFO.u32));
        V0_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VINFO.bits.out_height = out_height - 1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VINFO.u32);
    }

    return ;
}

HI_VOID HAL_VIDEO_CVFIR_SetOutFmt(HAL_DISP_LAYER_E enLayer, HI_U32 out_fmt)
{
    U_V0_CVFIR_VINFO V0_CVFIR_VINFO;
    volatile HI_U32  addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VINFO.u32));
        V0_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VINFO.bits.out_fmt = out_fmt;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VINFO.u32);
    }

    return ;
}

HI_VOID HAL_VIDEO_CVFIR_SetOutPro(HAL_DISP_LAYER_E enLayer, HI_U32 out_pro)
{
    U_V0_CVFIR_VINFO V0_CVFIR_VINFO;
    volatile HI_U32  addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VINFO.u32));
        V0_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VINFO.bits.out_pro = out_pro;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VINFO.u32);
    }

    return ;
}

HI_VOID HAL_VIDEO_CVFIR_SetVzmeCkGtEn(HAL_DISP_LAYER_E enLayer, HI_BOOL vzme_ck_gt_en)
{
    U_V0_CVFIR_VINFO V0_CVFIR_VINFO;
    volatile HI_U32  addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VINFO.u32));
        V0_CVFIR_VINFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VINFO.bits.vzme_ck_gt_en = vzme_ck_gt_en;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VINFO.u32);
    }

    return ;
}

/* VSP */
HI_VOID HAL_VIDEO_CVFIR_SetCvfirEn(HAL_DISP_LAYER_E enLayer, HI_U32 cvfir_en)
{
    U_V0_CVFIR_VSP  V0_CVFIR_VSP;
    volatile HI_U32 addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VSP.u32));
        V0_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VSP.bits.cvfir_en = cvfir_en;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VSP.u32);
    }

    return ;
}

HI_VOID HAL_VIDEO_CVFIR_SetCvmidEn(HAL_DISP_LAYER_E enLayer, HI_U32 cvmid_en)
{
    U_V0_CVFIR_VSP  V0_CVFIR_VSP;
    volatile HI_U32 addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VSP.u32));
        V0_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VSP.bits.cvmid_en = cvmid_en;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VSP.u32);
    }

    return ;
}

HI_VOID HAL_VIDEO_CVFIR_SetCvfirMode(HAL_DISP_LAYER_E enLayer, HI_U32 cvfir_mode)
{
    U_V0_CVFIR_VSP  V0_CVFIR_VSP;
    volatile HI_U32 addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VSP.u32));
        V0_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VSP.bits.cvfir_mode = cvfir_mode;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VSP.u32);
    }

    return ;
}

HI_VOID HAL_VIDEO_CVFIR_SetVratio(HAL_DISP_LAYER_E enLayer, HI_U32 vratio)
{
    U_V0_CVFIR_VSP  V0_CVFIR_VSP;
    volatile HI_U32 addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VSP.u32));
        V0_CVFIR_VSP.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VSP.bits.vratio = vratio;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VSP.u32);
    }

    return ;
}

HI_VOID HAL_VIDEO_CVFIR_SetVChromaOffset(HAL_DISP_LAYER_E enLayer, HI_U32 vchroma_offset)
{
    U_V0_CVFIR_VOFFSET V0_CVFIR_VOFFSET;
    volatile HI_U32 addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VOFFSET.u32));
        V0_CVFIR_VOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VOFFSET.bits.vchroma_offset = vchroma_offset;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VOFFSET.u32);
    }

    return ;
}


/* VBOFFSET */
HI_VOID HAL_VIDEO_CVFIR_SetVbChromaOffset(HAL_DISP_LAYER_E enLayer, HI_U32 vbchroma_offset)
{
    U_V1_CVFIR_VBOFFSET V0_CVFIR_VBOFFSET;
    volatile HI_U32 addr_REG;

    if (enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer, (HI_U32) & (pVoReg->V0_CVFIR_VBOFFSET.u32));
        V0_CVFIR_VBOFFSET.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_CVFIR_VBOFFSET.bits.vbchroma_offset = vbchroma_offset;
        HAL_WriteReg((HI_U32*)addr_REG, V0_CVFIR_VBOFFSET.u32);
    }

    return ;
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

            return HI_FALSE;
        }
    }

    return HI_TRUE;
}


HI_BOOL HAL_VIDEO_SetLayerDispRect(HAL_DISP_LAYER_E enLayer, RECT_S* pstRect)
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
        case HAL_DISP_LAYER_VHD2:
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
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_DFPOS));
            G0_DFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_DFPOS.bits.disp_xfpos = pstRect->s32X;
            G0_DFPOS.bits.disp_yfpos = pstRect->s32Y;
            HAL_WriteReg((HI_U32*)addr_REG, G0_DFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_DLPOS));
            G0_DLPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_DLPOS.bits.disp_xlpos = pstRect->s32X + pstRect->u32Width - 1;
            G0_DLPOS.bits.disp_ylpos = pstRect->s32Y + pstRect->u32Height - 1;
            HAL_WriteReg((HI_U32*)addr_REG, G0_DLPOS.u32);
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
        case HAL_DISP_LAYER_VHD2:
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

            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_VFPOS));
            G0_VFPOS.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            G0_VFPOS.bits.video_xfpos = pstRect->s32X;
            G0_VFPOS.bits.video_yfpos = pstRect->s32Y;
            HAL_WriteReg((HI_U32*)addr_REG, G0_VFPOS.u32);

            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_VLPOS));
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

        return HI_FALSE;
    }

    return HI_TRUE;
}



HI_BOOL HAL_LAYER_EnableLayer(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)

{
    volatile U_V0_CTRL V0_CTRL;
    volatile U_G0_CTRL G0_CTRL;
    volatile  HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_CTRL.u32));
            //osal_printk("+++layer:%d V1_CTRL:%p,%s:%d+++\n", enLayer, (HI_U32*)addr_REG,__FUNCTION__,__LINE__);
            V0_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            V0_CTRL.bits.surface_en = bEnable;
            V0_CTRL.bits.nosec_flag = 0;
            HAL_WriteReg((HI_U32*)addr_REG, V0_CTRL.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_CTRL));
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
    if (HAL_DISP_LAYER_VHD0 == enLayer ||
        HAL_DISP_LAYER_VHD1 == enLayer ||
        HAL_DISP_LAYER_VHD2 == enLayer)
    {
        addr_REG = Vou_GetVidAbsAddr(enLayer, (HI_UL) & (pVoReg->VID_SRC_INFO.u32));
        VID_SRC_INFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        VID_SRC_INFO.bits.data_type = enDataFmt;
        HAL_WriteReg((HI_U32*)addr_REG, VID_SRC_INFO.u32);
    }
    else if (HAL_DISP_LAYER_GFX0 == enLayer ||
             HAL_DISP_LAYER_GFX1 == enLayer ||
             HAL_DISP_LAYER_GFX3 == enLayer)
    {
        addr_REG = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_SRC_INFO.u32));
        GFX_SRC_INFO.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        GFX_SRC_INFO.bits.ifmt = enDataFmt;
        HAL_WriteReg((HI_U32*)addr_REG, GFX_SRC_INFO.u32);
    }
    else
    {
        HAL_PRINT("Error layer id%d found in %s: L%d\n", enLayer, __FUNCTION__, __LINE__);
        return HI_FALSE;
    }

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

        return HI_FALSE;
    }
    return HI_TRUE;
}



HI_BOOL HAL_LAYER_SetLayerGalpha(HAL_DISP_LAYER_E enLayer,
                                     HI_U8 u8Alpha0)
{
    volatile U_V0_CTRL V0_CTRL;


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


        default:
        {

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


    else
    {

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

    else
    {

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

        return HI_FALSE;
    }

    return HI_TRUE;
}



HI_BOOL  HAL_LAYER_SetRegUp(HAL_DISP_LAYER_E enLayer)
{
    U_V0_UPD V0_UPD;
    U_G0_UPD G0_UPD;
    volatile HI_UL addr_REG;

    switch (enLayer)
    {
        case HAL_DISP_LAYER_VHD0:
        case HAL_DISP_LAYER_VHD1:
        case HAL_DISP_LAYER_VHD2:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->V0_UPD.u32));
            V0_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            /* video layer register update */
            V0_UPD.bits.regup = 0x1;
            HAL_WriteReg((HI_U32*)addr_REG, V0_UPD.u32);
            break;
        }

        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX3:
        {
            addr_REG = Vou_GetAbsAddr(enLayer, (HI_UL) & (pVoReg->G0_UPD));
            G0_UPD.u32 = HAL_ReadReg((HI_U32*)addr_REG);
            /* graphic layer register update */
            G0_UPD.bits.regup = 0x1;
            HAL_WriteReg((HI_U32*)addr_REG, G0_UPD.u32);
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

        return HI_FALSE;
    }

    return HI_TRUE;
}


HI_BOOL HAL_CBM_SetCbmMixerPrio(HAL_DISP_LAYER_E enLayer, HI_U8 u8Prio, HI_U8 u8MixerId)
{
    U_CBM_MIX1 CBM_MIX1;
    U_CBM_MIX2 CBM_MIX2;

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

                return HI_FALSE;
            }
        }

        HAL_WriteReg((HI_U32*) & (pVoReg->CBM_MIX2.u32), CBM_MIX2.u32);
    }

    else
    {

        return HI_FALSE;
    }


    return HI_TRUE;
}

HI_BOOL HAL_GRAPHIC_SetGfxExt(HAL_DISP_LAYER_E enLayer,
                              HAL_GFX_BITEXTEND_E enMode)
{
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile  HI_UL addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
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

HI_BOOL HAL_GRAPHIC_SetGfxPalpha(HAL_DISP_LAYER_E enLayer,
                                 HI_U32 bAlphaEn, HI_U32 bArange,
                                 HI_U8 u8Alpha0, HI_U8 u8Alpha1)
{
    //*
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    U_GFX_1555_ALPHA GFX_1555_ALPHA;

    volatile HI_UL addr_REG;

    if (HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
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


HI_BOOL HAL_GRAPHIC_SetGfxPreMult(HAL_DISP_LAYER_E enLayer, HI_U32 bEnable)
{
    //*
    U_GFX_OUT_CTRL GFX_OUT_CTRL;

    volatile  HI_UL addr_REG;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
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

HI_BOOL HAL_GRAPHIC_SetGfxAddr(HAL_DISP_LAYER_E enLayer, HI_U64 u64LAddr)
{
    volatile HI_UL ul_GFX_ADDR_H = 0;
    volatile HI_UL ul_GFX_ADDR_L = 0;

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
    {
        // Write low address to register.
        ul_GFX_ADDR_L   = Vou_GetGfxAbsAddr(enLayer, (HI_UL) & (pVoReg->GFX_ADDR_L));
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

    if( HAL_DISP_LAYER_GFX0 == enLayer ||
        HAL_DISP_LAYER_GFX1 == enLayer ||
        HAL_DISP_LAYER_GFX3 == enLayer)
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


HI_VOID HAL_LAYER_CSC_SetDcCoef(HAL_DISP_LAYER_E enLayer, VDP_CSC_DC_COEF_S *pstCscDcCoef)
{
    U_V0_HIPP_CSC_IDC0 V0_HIPP_CSC_IDC0;
    U_V0_HIPP_CSC_IDC1 V0_HIPP_CSC_IDC1;
    U_V0_HIPP_CSC_IDC2 V0_HIPP_CSC_IDC2;
    U_V0_HIPP_CSC_ODC0 V0_HIPP_CSC_ODC0;
    U_V0_HIPP_CSC_ODC1 V0_HIPP_CSC_ODC1;
    U_V0_HIPP_CSC_ODC2 V0_HIPP_CSC_ODC2;

    volatile HI_U32 addr_REG;

    if(enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_IDC0.u32));
        V0_HIPP_CSC_IDC0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_IDC0.bits.hipp_csc_idc0 = pstCscDcCoef->csc_in_dc0;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_IDC0.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_IDC1.u32));
        V0_HIPP_CSC_IDC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_IDC1.bits.hipp_csc_idc1 = pstCscDcCoef->csc_in_dc1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_IDC1.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_IDC2.u32));
        V0_HIPP_CSC_IDC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_IDC2.bits.hipp_csc_idc2 = pstCscDcCoef->csc_in_dc2;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_IDC2.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_ODC0.u32));
        V0_HIPP_CSC_ODC0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_ODC0.bits.hipp_csc_odc0 = pstCscDcCoef->csc_out_dc0;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_ODC0.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_ODC1.u32));
        V0_HIPP_CSC_ODC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_ODC1.bits.hipp_csc_odc1 = pstCscDcCoef->csc_out_dc1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_ODC1.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_ODC2.u32));
        V0_HIPP_CSC_ODC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_ODC2.bits.hipp_csc_odc2 = pstCscDcCoef->csc_out_dc2;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_ODC2.u32);
    }
    else if(enLayer >= HAL_DISP_LAYER_GFX0&& enLayer <= HAL_DISP_LAYER_GFX3)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_IDC0.u32));
        V0_HIPP_CSC_IDC0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_IDC0.bits.hipp_csc_idc0 = pstCscDcCoef->csc_in_dc0;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_IDC0.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_IDC1.u32));
        V0_HIPP_CSC_IDC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_IDC1.bits.hipp_csc_idc1 = pstCscDcCoef->csc_in_dc1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_IDC1.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_IDC2.u32));
        V0_HIPP_CSC_IDC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_IDC2.bits.hipp_csc_idc2 = pstCscDcCoef->csc_in_dc2;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_IDC2.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_ODC0.u32));
        V0_HIPP_CSC_ODC0.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_ODC0.bits.hipp_csc_odc0 = pstCscDcCoef->csc_out_dc0;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_ODC0.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_ODC1.u32));
        V0_HIPP_CSC_ODC1.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_ODC1.bits.hipp_csc_odc1 = pstCscDcCoef->csc_out_dc1;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_ODC1.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_ODC2.u32));
        V0_HIPP_CSC_ODC2.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_ODC2.bits.hipp_csc_odc2 = pstCscDcCoef->csc_out_dc2;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_ODC2.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s,%s,%d\n",__FILE__, __FUNCTION__, __LINE__);
    }

}

HI_VOID HAL_LAYER_CSC_SetParam(HAL_DISP_LAYER_E enLayer, CscCoefParam_S *pstCoefParam)
{
    U_V0_HIPP_CSC_SCALE    V0_HIPP_CSC_SCALE;
    U_V0_HIPP_CSC_MIN_Y    V0_HIPP_CSC_MIN_Y;
    U_V0_HIPP_CSC_MIN_C    V0_HIPP_CSC_MIN_C;
    U_V0_HIPP_CSC_MAX_Y    V0_HIPP_CSC_MAX_Y;
    U_V0_HIPP_CSC_MAX_C    V0_HIPP_CSC_MAX_C;

    volatile HI_U32 addr_REG;
    if(enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_SCALE.u32));
        V0_HIPP_CSC_SCALE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_SCALE.bits.hipp_csc_scale = pstCoefParam->csc_scale2p;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_SCALE.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_MIN_Y.u32));
        V0_HIPP_CSC_MIN_Y.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_MIN_Y.bits.hipp_csc_min_y = pstCoefParam->csc_clip_min;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_MIN_Y.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_MIN_C.u32));
        V0_HIPP_CSC_MIN_C.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_MIN_C.bits.hipp_csc_min_c = pstCoefParam->csc_clip_min;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_MIN_C.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_MAX_Y.u32));
        V0_HIPP_CSC_MAX_Y.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_MAX_Y.bits.hipp_csc_max_y = pstCoefParam->csc_clip_max;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_MAX_Y.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_MAX_C.u32));
        V0_HIPP_CSC_MAX_C.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_MAX_C.bits.hipp_csc_max_c = pstCoefParam->csc_clip_max;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_MAX_C.u32);
    }
    else if (enLayer >= LAYER_GFX_START && enLayer <= LAYER_GFX_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_SCALE.u32));
        V0_HIPP_CSC_SCALE.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_SCALE.bits.hipp_csc_scale = pstCoefParam->csc_scale2p;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_SCALE.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_MIN_Y.u32));
        V0_HIPP_CSC_MIN_Y.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_MIN_Y.bits.hipp_csc_min_y = pstCoefParam->csc_clip_min;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_MIN_Y.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_MIN_C.u32));
        V0_HIPP_CSC_MIN_C.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_MIN_C.bits.hipp_csc_min_c = pstCoefParam->csc_clip_min;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_MIN_C.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_MAX_Y.u32));
        V0_HIPP_CSC_MAX_Y.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_MAX_Y.bits.hipp_csc_max_y = pstCoefParam->csc_clip_max;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_MAX_Y.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_MAX_C.u32));
        V0_HIPP_CSC_MAX_C.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_MAX_C.bits.hipp_csc_max_c = pstCoefParam->csc_clip_max;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_MAX_C.u32);
    }
}

HI_VOID HAL_LAYER_CSC_SetCoef(HAL_DISP_LAYER_E enLayer, VDP_CSC_COEF_S *pstCscCoef)
{
    U_V0_HIPP_CSC_COEF00   V0_HIPP_CSC_COEF00;
    U_V0_HIPP_CSC_COEF01   V0_HIPP_CSC_COEF01;
    U_V0_HIPP_CSC_COEF02   V0_HIPP_CSC_COEF02;
    U_V0_HIPP_CSC_COEF10   V0_HIPP_CSC_COEF10;
    U_V0_HIPP_CSC_COEF11   V0_HIPP_CSC_COEF11;
    U_V0_HIPP_CSC_COEF12   V0_HIPP_CSC_COEF12;
    U_V0_HIPP_CSC_COEF20   V0_HIPP_CSC_COEF20;
    U_V0_HIPP_CSC_COEF21   V0_HIPP_CSC_COEF21;
    U_V0_HIPP_CSC_COEF22   V0_HIPP_CSC_COEF22;

    volatile HI_U32 addr_REG;

    if(enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_COEF00.u32));
        V0_HIPP_CSC_COEF00.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF00.bits.hipp_csc_coef00 = pstCscCoef->csc_coef00;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF00.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_COEF01.u32));
        V0_HIPP_CSC_COEF01.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF01.bits.hipp_csc_coef01 = pstCscCoef->csc_coef01;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF01.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_COEF02.u32));
        V0_HIPP_CSC_COEF02.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF02.bits.hipp_csc_coef02 = pstCscCoef->csc_coef02;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF02.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_COEF10.u32));
        V0_HIPP_CSC_COEF10.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF10.bits.hipp_csc_coef10 = pstCscCoef->csc_coef10;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF10.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_COEF11.u32));
        V0_HIPP_CSC_COEF11.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF11.bits.hipp_csc_coef11 = pstCscCoef->csc_coef11;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF11.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_COEF12.u32));
        V0_HIPP_CSC_COEF12.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF12.bits.hipp_csc_coef12 = pstCscCoef->csc_coef12;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF12.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_COEF20.u32));
        V0_HIPP_CSC_COEF20.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF20.bits.hipp_csc_coef20 = pstCscCoef->csc_coef20;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF20.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_COEF21.u32));
        V0_HIPP_CSC_COEF21.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF21.bits.hipp_csc_coef21 = pstCscCoef->csc_coef21;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF21.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_COEF22.u32));
        V0_HIPP_CSC_COEF22.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF22.bits.hipp_csc_coef22 = pstCscCoef->csc_coef22;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF22.u32);
    }
    else if(enLayer >= HAL_DISP_LAYER_GFX0 && enLayer <= HAL_DISP_LAYER_GFX3)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_COEF00.u32));
        V0_HIPP_CSC_COEF00.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF00.bits.hipp_csc_coef00 = pstCscCoef->csc_coef00;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF00.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_COEF01.u32));
        V0_HIPP_CSC_COEF01.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF01.bits.hipp_csc_coef01 = pstCscCoef->csc_coef01;
        printf("in uboot pstCscCoef->csc_coef01: %#x\n", pstCscCoef->csc_coef01);
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF01.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_COEF02.u32));
        V0_HIPP_CSC_COEF02.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF02.bits.hipp_csc_coef02 = pstCscCoef->csc_coef02;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF02.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_COEF10.u32));
        V0_HIPP_CSC_COEF10.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF10.bits.hipp_csc_coef10 = pstCscCoef->csc_coef10;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF10.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_COEF11.u32));
        V0_HIPP_CSC_COEF11.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF11.bits.hipp_csc_coef11 = pstCscCoef->csc_coef11;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF11.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_COEF12.u32));
        V0_HIPP_CSC_COEF12.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF12.bits.hipp_csc_coef12 = pstCscCoef->csc_coef12;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF12.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_COEF20.u32));
        V0_HIPP_CSC_COEF20.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF20.bits.hipp_csc_coef20 = pstCscCoef->csc_coef20;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF20.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_COEF21.u32));
        V0_HIPP_CSC_COEF21.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF21.bits.hipp_csc_coef21 = pstCscCoef->csc_coef21;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF21.u32);

        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_COEF22.u32));
        V0_HIPP_CSC_COEF22.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_COEF22.bits.hipp_csc_coef22 = pstCscCoef->csc_coef22;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_COEF22.u32);
    }
    else
    {
        HAL_PRINT("Error layer id found in %s,%s,%d\n",__FILE__, __FUNCTION__, __LINE__);
    }

    return ;
}

HI_BOOL HAL_LAYER_SetCscCoef(HAL_DISP_LAYER_E enLayer, CscCoef_S* pstCscCoef)
{

    HAL_LAYER_CSC_SetDcCoef(enLayer, (VDP_CSC_DC_COEF_S *)(&pstCscCoef->csc_in_dc0));
    HAL_LAYER_CSC_SetCoef(enLayer, (VDP_CSC_COEF_S *)(&pstCscCoef->csc_coef00));
    HAL_LAYER_CSC_SetParam(enLayer, (CscCoefParam_S *)(&pstCscCoef->new_csc_scale2p));

    return HI_TRUE;
}

HI_VOID HAL_LAYER_CSC_SetCkGtEn(HAL_DISP_LAYER_E enLayer, HI_BOOL ck_gt_en)
{
    U_V0_HIPP_CSC_CTRL     V0_HIPP_CSC_CTRL;

    volatile HI_U32 addr_REG;

    if(enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_CTRL.u32));
        V0_HIPP_CSC_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_CTRL.bits.hipp_csc_ck_gt_en = ck_gt_en;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_CTRL.u32);
    }
    else if (enLayer >= LAYER_GFX_START && enLayer <= LAYER_GFX_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_CTRL.u32));
        V0_HIPP_CSC_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_CTRL.bits.hipp_csc_ck_gt_en = ck_gt_en;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_CTRL.u32);
    }
}

HI_VOID HAL_LAYER_CSC_SetEnable(HAL_DISP_LAYER_E enLayer, HI_BOOL csc_en)
{
    U_V0_HIPP_CSC_CTRL     V0_HIPP_CSC_CTRL;

    volatile HI_U32 addr_REG;

    if(enLayer >= LAYER_VHD_START && enLayer <= LAYER_VHD_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->V0_HIPP_CSC_CTRL.u32));
        V0_HIPP_CSC_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_CTRL.bits.hipp_csc_en = csc_en;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_CTRL.u32);

    }
    else if (enLayer >= LAYER_GFX_START && enLayer <= LAYER_GFX_END)
    {
        addr_REG = Vou_GetAbsAddr(enLayer,(HI_U32)&(pVoReg->G0_HIPP_CSC_CTRL.u32));
        V0_HIPP_CSC_CTRL.u32 = HAL_ReadReg((HI_U32*)addr_REG);
        V0_HIPP_CSC_CTRL.bits.hipp_csc_en = csc_en;
        HAL_WriteReg((HI_U32*)addr_REG, V0_HIPP_CSC_CTRL.u32);

    }
}

HI_BOOL HAL_LAYER_SetCscEn(HAL_DISP_LAYER_E enLayer, HI_BOOL bCscEn)
{
    if(enLayer < HAL_DISP_LAYER_VHD0 || enLayer > HAL_DISP_LAYER_GFX3)
    {
        HAL_PRINT("Error, Wrong layer ID!%d\n",__LINE__);
        return HI_FALSE;
    }

    HAL_LAYER_CSC_SetCkGtEn(enLayer, bCscEn);
    HAL_LAYER_CSC_SetEnable(enLayer, bCscEn);

    return HI_TRUE;
}

#ifdef __cplusplus
#if __cplusplus
 }
#endif
#endif /* End of #ifdef __cplusplus */

