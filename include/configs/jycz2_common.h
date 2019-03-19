/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 * Copyright 2018 NXP
 *
 * Configuration settings for the Freescale i.MX6Q SabreSD board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __JYCZ2_COMMON_H__
#define __JYCZ2_COMMON_H__

#include "mx6_common.h"
#include "imx_env.h"

#define CONFIG_IMX_THERMAL

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_MXC_UART

/* MMC Configs */
#define CONFIG_SYS_FSL_ESDHC_ADDR      0

/* Network */
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE		RMII
#define CONFIG_FEC_ENET_DEV             0
#define CONFIG_FEC_MXC_PHYADDR          0
#define CONFIG_ETHPRIME                 "FEC"

#define CONFIG_CMD_READ
#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV         0

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"fastboot_buffer=0x12000000\0" \

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	"image=zImage\0" \
	"loadaddr=0x10008000\0" \
	"fdt_file=" CONFIG_DEFAULT_DEVICE_TREE ".dtb\0" \
	"fdt_addr=0x14000000\0" \
	"fdt_high=0x140fffff\0"	  \
	"initrd_addr=0x14100000\0" \
	"initrd_high=0x15ffffff\0" \
	"mmcdev=" __stringify(CONFIG_SYS_MMC_BOOT_DEV) "\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_BOOT_PART) "\0" \
	"spi_bus=0\0" \
	"spi_uboot=0x400\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"if run loadimage && run loadfdt; then " \
			"bootz ${loadaddr} - ${fdt_addr}; " \
		"else " \
			"echo WARN: Cannot load the Kernl or DT; " \
		"fi;\0" \

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev}; " \
	"if mmc rescan; then " \
		"run mmcboot; " \
	"else " \
		"echo WARN: Cannot find the boot device!; " \
	"fi"

#define CONFIG_ARP_TIMEOUT             200UL

#define CONFIG_SYS_MEMTEST_START       0x10000000
#define CONFIG_SYS_MEMTEST_END         0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS           1
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR
#define PHYS_SDRAM_SIZE                (1u * 1024 * 1024 * 1024)

#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* Environment organization */
#define CONFIG_ENV_SIZE			(8 * 1024)

#ifdef CONFIG_CMD_SF
#define CONFIG_SF_DEFAULT_BUS           0
#define CONFIG_SF_DEFAULT_CS            0
#define CONFIG_SF_DEFAULT_SPEED         20000000
#define CONFIG_SF_DEFAULT_MODE          (SPI_MODE_0)
#endif /* CONFIG_CMD_SF */

#if defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET              (896 * 1024)
#define CONFIG_ENV_SECT_SIZE           (64 * 1024)
#define CONFIG_ENV_SPI_BUS             CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS              CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE            CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ          CONFIG_SF_DEFAULT_SPEED
#endif /* CONFIG_ENV_IS_IN_SPI_FLASH */

/* I2C Configs */
#ifndef CONFIG_DM_I2C
#define CONFIG_SYS_I2C
#endif /* CONFIG_DM_I2C */
#ifdef CONFIG_CMD_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */
#define CONFIG_SYS_I2C_SPEED		  100000
#endif /* CONFIG_CMD_I2C */

/* Framebuffer */
#define CONFIG_VIDEO_IPUV3
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_HDMI
#define CONFIG_IMX_VIDEO_SKIP

#endif /* __JYCZ2_COMMON_H__ */
