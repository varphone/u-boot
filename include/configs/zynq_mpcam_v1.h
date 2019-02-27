/*
 * (C) Copyright 2013 Xilinx, Inc.
 *
 * Configuration settings for the Xilinx Zynq ZC702 and ZC706 boards
 * See zynq-common.h for Zynq common configs
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_ZYNQ_MPCAM_V1_H
#define __CONFIG_ZYNQ_MPCAM_V1_H

/* CPU clock */
#ifndef CONFIG_CPU_FREQ_HZ
#define CONFIG_CPU_FREQ_HZ		666666667
#endif

#include <configs/zynq-common.h>

/* Override env offset on spi flash */
#ifdef CONFIG_ZYNQ_QSPI
#undef CONFIG_SYS_SPI_ARGS_OFFS
#define CONFIG_SYS_SPI_ARGS_OFFS	0x5E0000
#endif

/* Override the env settings */
#undef BOOTENV
#undef CONFIG_BOOTCOMMAND
#undef CONFIG_ENV_OFFSET
#undef CONFIG_ENV_SIZE
#undef CONFIG_EXTRA_ENV_SETTINGS

#define CONFIG_ENV_OFFSET		0x5C0000
#define CONFIG_ENV_SIZE			(128 << 10)

#define CONFIG_EXTRA_ENV_SETTINGS \
	"ethaddr=00:0a:35:00:01:22\0" \
	"ipaddr=192.168.1.128\0" \
	"netmask=255.255.0.0\0" \
	"serverip=192.168.0.7\0" \
	"fdt_high=0x02000000\0"	\
	"initrd_high=0x02000000\0" \
	"image=image.itb\0" \
	"image_size=0xA00000\0" \
	"image_load_addr=0x10000000\0" \
	"bootconf=#noramdisk\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv_addr=0x02000000\0" \
	"loadbootenv=load mmc 0 ${loadbootenv_addr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from SD ...; " \
		"env import -t ${loadbootenv_addr} ${filesize}\0" \
	"sd_uEnvtxt_existence_test=test -e mmc 0 ${bootenv}\0" \
	"preboot=if test $modeboot = sdboot && env run sd_uEnvtxt_existence_test; " \
			"then if env run loadbootenv; " \
				"then env run importbootenv; " \
			"fi; " \
		"fi; \0" \
	"uenvboot=" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv}; " \
			"run importbootenv; " \
		"fi; " \
		"if test -n ${uenvcmd}; then " \
			"echo Running uenvcmd ...; " \
			"run uenvcmd; " \
		"fi\0" \
	"qspiboot=echo Copying FIT Image from QSPI flash to RAM ... && " \
		"sf probe 0 0 0 && " \
		"sf read ${image_load_addr} 0x600000 ${image_size} && " \
		"bootm ${image_load_addr}${bootconf}\0" \
	"qspiupdate=echo Update QSPI flash ... && sf probe 0; " \
		"if fatload mmc 0 ${image_load_addr} fsbl.bin; then " \
			"sf erase 0 0x40000; " \
			"sf write ${image_load_addr} 0 0x40000; " \
		"fi; " \
		"if fatload mmc 0 ${image_load_addr} fpga.bin; then " \
			"sf erase 0x40000 0x500000; " \
			"sf write ${image_load_addr} 0x40000 0x500000; " \
		"fi; " \
		"if fatload mmc 0 ${image_load_addr} u-boot.bin; then " \
			"sf erase 0x540000 0x80000; " \
			"sf write ${image_load_addr} 0x540000 0x80000; " \
		"fi; " \
		"if fatload mmc 0 ${image_load_addr} u-boot-env.bin; then " \
			"sf erase 0x5C0000 0x40000; " \
			"sf write ${image_load_addr} 0x5C0000 0x40000; " \
		"fi; " \
		"if fatload mmc 0 ${image_load_addr} zynq-mpcam-v1.dtb; then " \
			"sf erase 0x5E0000 0x20000; " \
			"sf write ${image_load_addr} 0x5E0000 0x20000; " \
		"fi; " \
		"if fatload mmc 0 ${image_load_addr} image.itb; then " \
			"sf erase 0x600000 0xA00000; " \
			"sf write ${image_load_addr} 0x600000 0xA00000; " \
			"setenv image_size ${filesize}; " \
		"fi\0" \
	"force_sdboot=yes\0" \
	"sdboot=if mmcinfo; then " \
			"run uenvboot; " \
			"echo Copying FIT Image from SD to RAM ... && " \
			"load mmc 0 ${image_load_addr} ${image} && " \
			"bootm ${image_load_addr}${bootconf}; " \
		"fi\0" \
	"usbupdate=echo Update eMMC flash from USB ... && usb start; " \
		"if fatload usb 0:4 ${image_load_addr} sdcard.img; then " \
			"mmc write ${image_load_addr} 0 0x40000; " \
		"fi; " \
		"usb stop\0" \
	"modeboot=qspiboot\0" \
	"bootcmd=if test \"${force_sdboot}\" = \"yes\"; " \
			"then run sdboot; " \
			"else run ${modeboot}; " \
		"fi\0"

#endif /* __CONFIG_ZYNQ_MPCAM_V1_H */

