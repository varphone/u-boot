/*
 * MYZR Technology Co.,Ltd
 * http://www.myzr.com.cn
 * Tang Bin <tangb@myzr.com.cn>
 */
 
#ifndef __MYIMX6EVK_CONFIG_H
#define __MYIMX6EVK_CONFIG_H

#include <asm/arch/imx-regs.h>
#include <linux/sizes.h>
#include "mx6_common.h"
#include <asm/imx-common/gpio.h>

#define CONFIG_MX6
#define CONFIG_ROM_UNIFIED_SECTIONS
#define CONFIG_SYS_GENERIC_BOARD
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#undef CONFIG_LDO_BYPASS_CHECK

#ifdef CONFIG_SECURE_BOOT
#ifndef CONFIG_CSF_SIZE
#define CONFIG_CSF_SIZE 0x4000
#endif
#endif

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_REVISION_TAG

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_MXC_GPIO

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX			1
#define CONFIG_BAUDRATE				115200

#define CONFIG_CMD_FUSE
#ifdef CONFIG_CMD_FUSE
#define CONFIG_MXC_OCOTP
#endif

/* MMC Configs */
#define CONFIG_FSL_USDHC
#ifdef CONFIG_FSL_USDHC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC2_BASE_ADDR
#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_MMC
#define CONFIG_CMD_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#define CONFIG_SUPPORT_EMMC_BOOT 	/* eMMC specific */
#endif

#undef CONFIG_BOOTM_NETBSD
#undef CONFIG_BOOTM_PLAN9
#undef CONFIG_BOOTM_RTEMS

#undef CONFIG_CMD_EXPORTENV
#undef CONFIG_CMD_IMPORTENV

#define CONFIG_CMD_NET
#ifdef CONFIG_CMD_NET
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define CONFIG_FEC_ENET_DEV 0

#if (CONFIG_FEC_ENET_DEV == 0)
#define IMX_FEC_BASE				ENET_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR		0
#define CONFIG_FEC_XCV_TYPE			RMII
#endif

#define CONFIG_ETHPRIME				"FEC"

#define CONFIG_PHYLIB
#define CONFIG_PHY_SMSC
#define CONFIG_FEC_DMA_MINALIGN		64

#define CONFIG_SERVERIP				192.168.18.18
#define CONFIG_IPADDR				192.168.18.81
#endif

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX			1

/* I2C configs */
#define CONFIG_CMD_I2C
#ifdef CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		100000
#endif

#define CONFIG_BOARD_NAME			"myimx6ek140"
#if (CONFIG_DRAM_SIZE == 512)
#define PHYS_SDRAM_SIZE				SZ_512M
#define EK_SPEC						"6g-512m"
#else
#define PHYS_SDRAM_SIZE				SZ_256M
#define EK_SPEC						"6g"
#endif
#define CONFIG_BOOTARGS_CMA_SIZE   "cma=96M "


#define CONFIG_VIDEO

/* Command definition */
#include <config_cmd_default.h>

#undef CONFIG_CMD_IMLS

#define CONFIG_BOOTDELAY			1

#define CONFIG_LOADADDR				0x80800000
#define CONFIG_SYS_TEXT_BASE		0x87800000

#define CONFIG_SYS_MMC_IMG_LOAD_PART	1
#define CONFIG_MFG_NAND_PARTITION	""
#endif

#ifdef CONFIG_VIDEO
#define CONFIG_VIDEO_MODE \
	"panel=TFT43AB\0"
#else
#define CONFIG_VIDEO_MODE ""
#endif

#define CONFIG_MFG_ENV_SETTINGS \
	"mfgtool_args=setenv bootargs console=${console},${baudrate} " \
	    CONFIG_BOOTARGS_CMA_SIZE \
		"rdinit=/linuxrc " \
		"g_mass_storage.stall=0 g_mass_storage.removable=1 " \
		"g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "\
		"g_mass_storage.iSerialNumber=\"\" "\
		CONFIG_MFG_NAND_PARTITION \
		"clk_ignore_unused "\
		"\0" \
	"initrd_addr=0x83800000\0" \
	"initrd_high=0xffffffff\0" \
	"bootcmd_mfg=run mfgtool_args;bootz ${loadaddr} ${initrd_addr} ${fdt_addr};\0" \

#define CONFIG_EXTRA_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS \
	CONFIG_VIDEO_MODE \
	"image=zImage-myimx6\0" \
	"console=ttymxc0\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"ubt_file=uboot-"CONFIG_BOARD_NAME"-"EK_SPEC".imx\0" \
	"fdt_file="CONFIG_BOARD_NAME"-"EK_SPEC".dtb\0" \
	"fdt_addr=0x83000000\0" \
	"bootmedia=mmc\0" \
	"ip_dyn=no\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcautodetect=yes\0" \
	"set_net_cmd=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi;\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
	    CONFIG_BOOTARGS_CMA_SIZE \
		"root=${mmcroot}\0" \
	"netargs=setenv bootargs console=${console},${baudrate} " \
	    CONFIG_BOOTARGS_CMA_SIZE \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"loadfdt=run set_net_cmd; " \
		"if test ${bootmedia} = tftp || test ${bootmedia} = net; then " \
			"${get_cmd} ${fdt_addr} ${fdt_file}; " \
		"else " \
			"mmc dev ${mmcdev}; fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}; " \
		"fi;\0" \
	"loadimage=run set_net_cmd; " \
		"if test ${bootmedia} = tftp || test ${bootmedia} = net; then " \
			"${get_cmd} ${loadaddr} ${image}; " \
		"else " \
			"mmc dev ${mmcdev}; fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}; " \
		"fi;\0" \
	"bootcmd_mmc=echo Booting from mmc ...; " \
		"setenv bootmedia mmc; run loadfdt; run loadimage; run mmcargs; " \
		"bootz ${loadaddr} - ${fdt_addr};\0 " \
	"bootcmd_tftp=echo Booting from tftp ...; " \
		"setenv bootmedia tftp; run loadfdt; run loadimage; run mmcargs; " \
		"bootz ${loadaddr} - ${fdt_addr};\0 " \
	"bootcmd_nfs=echo Booting from nfs ...; " \
		"setenv bootmedia nfs; mmc dev ${mmcdev}; run loadfdt; run loadimage; run netargs; " \
		"bootz ${loadaddr} - ${fdt_addr};\0 " \
	"bootcmd_net=echo Booting from net ...; " \
		"setenv bootmedia net; run loadfdt; run loadimage; run netargs; " \
		"bootz ${loadaddr} - ${fdt_addr};\0 " \
	"update_uboot=run set_net_cmd;" \
		"if ${get_cmd} ${loadaddr} ${ubt_file}; then " \
			"mmc dev ${mmcdev} 0; mmc write ${loadaddr} 0x2 0x3fe; " \
		"else " \
			"echo FAIL: Update u-boot fail ...; " \
		"fi;\0" \
	"update_fdt=run set_net_cmd;" \
		"if ${get_cmd} ${loadaddr} ${fdt_file}; then " \
			"fatwrite mmc ${mmcdev}:${mmcpart} ${loadaddr} ${fdt_file} 0x20000; " \
		"else " \
			"echo FAIL: Update device tree fail ...; " \
		"fi;\0" \
	"update_kern=run set_net_cmd;" \
		"if ${get_cmd} ${loadaddr} ${image}; then " \
			"fatwrite mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image} 0x800000; " \
		"else " \
			"echo FAIL: Update kernel fail ...; " \
		"fi;\0" \

#define CONFIG_BOOTCOMMAND \
	   "if test ${bootmedia} = tftp; then " \
			"run bootcmd_tftp; " \
		"else " \
			"if test ${bootmedia} = nfs; then " \
				"run bootcmd_nfs; " \
			"else " \
				"if test ${bootmedia} = net; then " \
					"run bootcmd_net; " \
				"else " \
					"run bootcmd_mmc; " \
				"fi; " \
		   "fi; " \
		"fi"

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT			"=> "
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE			1024

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE			(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS			256
#define CONFIG_SYS_BARGSIZE			CONFIG_SYS_CBSIZE

#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + PHYS_SDRAM_SIZE)

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ				1000

#define CONFIG_CMDLINE_EDITING
#define CONFIG_STACKSIZE			SZ_128K

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM					MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_FAT_WRITE
/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_SIZE				SZ_8K

#define CONFIG_ENV_IS_IN_MMC
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET			(8 * SZ_64K)
#endif

#define CONFIG_SYS_MMC_ENV_DEV		1   /* USDHC2 */
#define CONFIG_SYS_MMC_ENV_PART		0	/* user area */
#define CONFIG_MMCROOT				"/dev/mmcblk1p2"  /* USDHC2 */

#define CONFIG_OF_LIBFDT
#define CONFIG_CMD_BOOTZ

#ifndef CONFIG_SYS_DCACHE_OFF
#define CONFIG_CMD_CACHE
#endif

#ifdef CONFIG_VIDEO
#define	CONFIG_CFB_CONSOLE
#define	CONFIG_VIDEO_MXS
#define	CONFIG_VIDEO_LOGO
#define	CONFIG_VIDEO_SW_CURSOR
#define	CONFIG_VGA_AS_SINGLE_DEVICE
#define	CONFIG_SYS_CONSOLE_IS_IN_ENV
#define	CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define	CONFIG_CMD_BMP
#define	CONFIG_BMP_16BPP
#define	CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#endif

/* USB Configs */
#define CONFIG_CMD_USB
#ifdef CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC			(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS			0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	2
#endif
