
#ifndef __MYIMX6A7_CONFIG_H
#define __MYIMX6A7_CONFIG_H

#include "mx6_common.h"

#if defined(CONFIG_SECURE_BOOT)
#ifndef CONFIG_CSF_SIZE
#define CONFIG_CSF_SIZE 0x4000
#endif
#endif

#undef CONFIG_LDO_BYPASS_CHECK

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE
#define CONFIG_CONSOLE_DEV			"ttymxc0"

/* FLASH and environment organization */
#define CONFIG_CMD_BMODE
#define CONFIG_IMX_THERMAL

#if defined(CONFIG_MYIMX6Y)
#define CONFIG_IOMUX_LPSR
#endif

#define CONFIG_MODULE_FUSE
#define CONFIG_OF_SYSTEM_SETUP

/* Miscellaneous configurable options *************************************** */
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 0x10000000)

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ				1000

#define CONFIG_STACKSIZE			SZ_128K

/* Physical Memory Map ****************************************************** */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM					MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* MMC Configs ************************************************************** */
#if defined(CONFIG_FSL_USDHC)
#define CONFIG_SYS_FSL_ESDHC_ADDR		USDHC2_BASE_ADDR

#if defined(CONFIG_SYS_BOOT_NAND)
#define CONFIG_SYS_FSL_USDHC_NUM		1
#else
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_FSL_USDHC_NUM		2
#endif	/* CONFIG_SYS_BOOT_NAND */

#define CONFIG_SYS_MMC_ENV_DEV			1   				/* USDHC2 */
#define CONFIG_SYS_MMC_ENV_PART			0					/* user area */

#define	CONFIG_CMD_FAT
#define CONFIG_FAT_WRITE

#if defined(CONFIG_ENV_IS_IN_NAND)
#define CONFIG_ENV_OFFSET               (3 << 20)		/* 3M */
#define CONFIG_ENV_SECT_SIZE            (128 << 10)		/* 128K */
#define CONFIG_ENV_SIZE                 CONFIG_ENV_SECT_SIZE
#else
#define CONFIG_ENV_OFFSET				(12 * SZ_64K)
#define CONFIG_ENV_SIZE					SZ_8K
#endif	/* CONFIG_ENV_IS_IN_NAND */

#endif	/* CONFIG_FSL_USDHC */

/* Nand Configs ************************************************************* */
#if defined(CONFIG_SYS_BOOT_NAND)
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_CMD_MTDPARTS
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#endif	/* CONFIG_SYS_BOOT_NAND */

#if defined(CONFIG_SYS_BOOT_NAND)
#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_USE_NAND
#define CONFIG_ENV_IS_IN_NAND
#endif	/* CONFIG_SYS_BOOT_NAND */

/* NAND stuff */
#if defined(CONFIG_SYS_USE_NAND)
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_TRIMFFS

#define CONFIG_NAND_MXS
#define CONFIG_SYS_MAX_NAND_DEVICE      1
#define CONFIG_SYS_NAND_BASE            0x40000000
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_ONFI_DETECTION

/* DMA stuff, needed for GPMI/MXS NAND support */
#define CONFIG_APBH_DMA
#define CONFIG_APBH_DMA_BURST
#define CONFIG_APBH_DMA_BURST8
#endif	/* CONFIG_SYS_USE_NAND */

/* NET Configs ************************************************************** */
#if defined(CONFIG_CMD_NET)
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_CMD_MII
#define CONFIG_FEC_MXC
#define CONFIG_MII
/*#define CONFIG_ARP_TIMEOUT              5000UL*/

#define CONFIG_FEC_ENET_DEV			0
#if (CONFIG_FEC_ENET_DEV == 0)
#define IMX_FEC_BASE				ENET_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR		0x0		/* MY-IMX6-MB140P */
#define CONFIG_FEC_XCV_TYPE			RMII
#define CONFIG_ETHPRIME				"FEC"
#endif	/* CONFIG_FEC_ENET_DEV */

#define CONFIG_PHYLIB
#define CONFIG_PHY_SMSC
#define CONFIG_FEC_DMA_MINALIGN		64

#define CONFIG_SERVERIP				192.168.137.99
#define CONFIG_IPADDR				192.168.137.81
#endif	/* CONFIG_CMD_NET */

/* I2C configs ************************************************************** */
#define CONFIG_CMD_I2C
#if defined(CONFIG_CMD_I2C)
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_SPEED		100000
#endif	/* CONFIG_CMD_I2C */

/* USB Configs ************************************************************** */
#define CONFIG_CMD_USB
#if defined(CONFIG_CMD_USB)
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS   0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 1
#endif	/* CONFIG_CMD_USB */

/* Video ******************************************************************** */
#define CONFIG_VIDEO
#if defined(CONFIG_VIDEO)
#define CONFIG_CFB_CONSOLE
#define CONFIG_VIDEO_MXS
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_SHOW_MY_LOGO
#define CONFIG_VIDEO_SW_CURSOR
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_VIDEO_SKIP
#endif	/* CONFIG_VIDEO */

/* MY ENV ******************************************************************* */
#if (CONFIG_DDR_MB == 256)
#define PHYS_SDRAM_SIZE				SZ_256M
#define CONFIG_BOARD_MEM			"-256m"
#elif (CONFIG_DDR_MB == 128)
#define PHYS_SDRAM_SIZE				SZ_128M
#define CONFIG_BOARD_MEM			"-128m"
#elif (CONFIG_DDR_MB == 512)
#define PHYS_SDRAM_SIZE				SZ_512M
#define CONFIG_BOARD_MEM			"-512m"
#else
#error CONFIG_DDR_MB
#endif

#if defined(CONFIG_MYIMX6G)
#define CONFIG_BOARD_CPU		"-6g"
#elif defined(CONFIG_MYIMX6Y)
#define CONFIG_BOARD_CPU		"-6y"
#else
#error CONFIG_BOARD_CPU
#endif

#if defined(CONFIG_SYS_USE_NAND)
#define CONFIG_BOARD_STORAGE	"-nand"
#define CONFIG_MY_BOOTSCRIPT	"my_environment_nand.scr"
#else
#define CONFIG_BOARD_STORAGE	"-emmc"
#define CONFIG_MY_BOOTSCRIPT	"my_environment_emmc.scr"
#endif

#if defined(CONFIG_TARGET_MYIMX6EK140)
#define CONFIG_MYIMX6CB140
#define CONFIG_BOARD_NAME	"myimx6ek140"
#elif defined(CONFIG_TARGET_MYIMX6EK140P)
#define CONFIG_MYIMX6CB140
#define CONFIG_BOARD_NAME	"myimx6ek140p"
#endif

#define MTDIDS_DEFAULT		"nand0=gpmi-nand"
#define MTDPARTS_DEFAULT	"mtdparts=gpmi-nand:4m(uboot),10m(kernel),1m(dtb),-(rootfs)" 

/* MFG ENV ****************************************************************** */
#define CONFIG_MFG_ENV_SETTINGS \
	"bootargs_mfg=setenv bootargs console=${console},${baudrate} "	\
		"rdinit=/linuxrc "		\
		"g_mass_storage.stall=0 g_mass_storage.removable=1 "	\
		"g_mass_storage.file=/fat g_mass_storage.ro=1 "	\
		"g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "	\
		"g_mass_storage.iSerialNumber=\"\" "	\
		"${mtdparts} "			\
		"clk_ignore_unused\0"	\
	"bootcmd_mfg=run bootargs_mfg; bootz ${kernel_addr_r} ${ramdisk_addr_r} ${fdt_addr_r};\0"

/* COMMON ENV *************************************************************** */
#define CONFIG_EXTRA_ENV_SETTINGS	\
	CONFIG_MFG_ENV_SETTINGS	\
	"bootdelay=1\0"		\
	"console="CONFIG_CONSOLE_DEV"\0"	\
	"baudrate="__stringify(CONFIG_BAUDRATE)"\0"	\
	"loadaddr=0x80800000\0"		\
	"fdt_addr_r=0x83000000\0"		\
	"kernel_addr_r=0x80800000\0"	\
	"ramdisk_addr_r=0x83800000\0"	\
	"fdt_high=0xffffffff\0"	  \
	"initrd_high=0xffffffff\0" \
	"mtdids="MTDIDS_DEFAULT"\0"		\
	"mmcautodetect=yes\0"	\
    "panel=MY-WVGA\0"   \
	"mtdparts="MTDPARTS_DEFAULT"\0"	\
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0"	\
	"mmcpart=1\0"	\
	"file_scr="CONFIG_MY_BOOTSCRIPT"\0"	\
	"file_ubot=uboot-"CONFIG_BOARD_NAME""CONFIG_BOARD_CPU""CONFIG_BOARD_MEM""CONFIG_BOARD_STORAGE".imx\0"	\
	"fdt_file="CONFIG_BOARD_NAME""CONFIG_BOARD_CPU""CONFIG_BOARD_MEM""CONFIG_BOARD_STORAGE".dtb\0" \
    "my_env_flag=unset\0"   \
    "load_scr_emmc=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${file_scr};\0" \
    "load_scr_nand=nand read ${loadaddr} 0x3c0000 0x20000;\0"

#if defined(CONFIG_SYS_USE_NAND)
#define CONFIG_BOOTCOMMAND		\
    "if test ${my_env_flag} = unset; then "   \
		"run load_scr_nand; source ${loadaddr};"   \
    "fi; "      \
    "run bootcmd_my; "
#else
#define CONFIG_BOOTCOMMAND		\
    "if test ${my_env_flag} = unset; then "   \
		"run load_scr_emmc; source ${loadaddr}; saveenv; "   \
    "fi; "      \
    "run bootcmd_my; "
#endif

#endif		/* __MYIMX6A7_CONFIG_H */
