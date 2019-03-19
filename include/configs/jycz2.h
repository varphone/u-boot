/*
 * Copyright (C) 2019 Technology Development Co.,LTD
 * Copyright (C) 2019 Varphone Wong <varphone@qq.com>
 *
 * Configuration settings for the TDC JYCZ-2 board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __JYCZ2_H__
#define __JYCZ2_H__

#ifdef CONFIG_SPL
#include "imx6_spl.h"
#endif

#define CONFIG_MACH_TYPE        	3980
#define CONFIG_MXC_UART_BASE            UART1_BASE
#define CONSOLE_DEV                     "ttymxc0"

#if defined(CONFIG_MX6Q)
#define PHYS_SDRAM_SIZE                 (1u * 1024 * 1024 * 1024)
#elif defined(CONFIG_MX6DL)
#define PHYS_SDRAM_SIZE                 (1u * 1024 * 1024 * 1024)
#elif defined(CONFIG_MX6S)
#define PHYS_SDRAM_SIZE                 (512u * 1024 * 1024)
#endif

#include "jycz2_common.h"

/* Falcon Mode */
#define CONFIG_SPL_FS_LOAD_ARGS_NAME    "args"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME  "uImage"
#define CONFIG_SYS_SPL_ARGS_ADDR        0x18000000

/* Falcon Mode - MMC support: args@1MB kernel@2MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR          0x800   /* 1MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS         (CONFIG_CMD_SPL_WRITE_SIZE / 512)
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR        0x1000  /* 2MB */

#define CONFIG_SYS_FSL_USDHC_NUM	2

/* USB Configs */
#ifdef CONFIG_CMD_USB
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	1 /* Enabled USB controller number */
#endif

#endif /* __JYCZ2_H__ */
