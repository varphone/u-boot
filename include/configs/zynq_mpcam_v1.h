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

#define CONFIG_ZYNQ_I2C0
#define CONFIG_ZYNQ_EEPROM

#include <configs/zynq-common.h>

/* Override env offset on spi flash */
#ifdef CONFIG_ZYNQ_QSPI
#undef CONFIG_SYS_SPI_ARGS_OFFS
#define CONFIG_SYS_SPI_ARGS_OFFS	0x80000
#endif

/* Override the env settings */
#undef BOOTENV
#undef CONFIG_BOOTCOMMAND
#undef CONFIG_EXTRA_ENV_SETTINGS

#define CONFIG_EXTRA_ENV_SETTINGS \
	"ethaddr=00:0a:35:00:01:22\0" \
	"ipaddr=192.168.1.128\0" \
	"netmask=255.255.0.0\0" \
	"serverip=192.168.0.7\0" \
	"fdt_high=0x02000000\0"	\
	"initrd_high=0x02000000\0" \
	"image=image.itb\0" \
	"image_size=0xC00000\0" \
	"image_load_addr=0x10000000\0" \
	"bootconf=#ramdisk\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv_addr=0x2000000\0" \
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
	"qspiboot=echo Copying FIT Image from QSPI flash to RAM... && " \
		"sf probe 0 0 0 && " \
		"sf read ${image_load_addr} 0x200000 ${image_size} && " \
		"bootm ${image_load_addr}${bootconf}\0" \
	"qspiupdate=echo Update QSPI flash ... && sf probe 0; " \
		"if fatload mmc 0 ${image_load_addr} boot.bin; then " \
			"sf erase 0 0x80000; " \
			"sf write ${image_load_addr} 0 0x80000; " \
		"fi; " \
		"if fatload mmc 0 ${image_load_addr} u-boot.img; then " \
			"sf erase 0x100000 0x100000; " \
			"sf write ${image_load_addr} 0x100000 0x100000; " \
		"fi; " \
		"if fatload mmc 0 ${image_load_addr} image.itb; then " \
			"sf erase 0x200000 0xC00000; " \
			"sf write ${image_load_addr} 0x200000 0xC00000; " \
			"setenv image_size ${filesize}; " \
		"fi\0" \
	"sdboot=if mmcinfo; then " \
			"run uenvboot; " \
			"echo Copying FIT Image from SD to RAM... && " \
			"load mmc 0 ${image_load_addr} ${image} && " \
			"bootm ${image_load_addr}${bootconf}; " \
		"fi\0" \
	"modeboot=sdboot\0" \
	"bootcmd=run ${modeboot}\0"

#endif /* __CONFIG_ZYNQ_MPCAM_V1_H */
