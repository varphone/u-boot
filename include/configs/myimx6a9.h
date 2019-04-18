/*
 * MYZR Technology Co.,Ltd
 * http://www.myzr.com.cn
 * Tang Bin <tangb@myzr.com.cn>
 */

#ifndef __MYIMX6A9_CONFIG_H
#define __MYIMX6A9_CONFIG_H

/* ENV: CPU Type */
#if defined(CONFIG_MYIMX6QP)
#define CONFIG_MX6QP
#define CONFIG_BOARD_CPU		"-6qp"
#elif defined(CONFIG_MYIMX6Q)
#define CONFIG_MX6Q
#define CONFIG_BOARD_CPU		"-6q"
#elif defined(CONFIG_MYIMX6S)
#define CONFIG_MX6SOLO
#define CONFIG_BOARD_CPU		"-6s"
#elif defined(CONFIG_MYIMX6U)
#define CONFIG_MX6DL
#define CONFIG_BOARD_CPU		"-6u"
#else
#error Please configure the type of CPU
#endif

/* ENV: Board Name */
#if defined(CONFIG_TARGET_MYIMX6EK200)
#define CONFIG_MYIMX6CB200
#define CONFIG_MYIMX6MB200
#define CONFIG_BOARD_NAME			"myimx6ek200"
#define CONFIG_MACH_TYPE			0x1408          /* 5128 */
#elif defined(CONFIG_TARGET_MYIMX6EK314)
#define CONFIG_MYIMX6CB314
#define CONFIG_MYIMX6MB314
#define CONFIG_BOARD_NAME			"myimx6ek314"
#define CONFIG_MACH_TYPE			0x1409          /* 5129 */
#elif defined(CONFIG_TARGET_MYIMX6EK336)
#define CONFIG_MYIMX6CB336
#define CONFIG_MYIMX6MB314
#define CONFIG_BOARD_NAME			"myimx6ek336"
#define CONFIG_MACH_TYPE			0x140a          /* 5130 */

#define PHYS_SDRAM_0				MMDC0_ARB_BASE_ADDR
#define PHYS_SDRAM_1				MMDC1_ARB_BASE_ADDR
#endif

/* ENV: MEM Size */
#if (CONFIG_DDR_MB == 2048)
#define PHYS_SDRAM_SIZE		(2u * 1024 * 1024 * 1024)
#define CONFIG_BOARD_MEM	"-2g"
#define BOOTARGS_CMA_SIZE	"cma=512M "
#elif (CONFIG_DDR_MB == 1024)
#define PHYS_SDRAM_SIZE		(1u * 1024 * 1024 * 1024)
#define CONFIG_BOARD_MEM	"-1g"
#define BOOTARGS_CMA_SIZE	"cma=256M "
#elif (CONFIG_DDR_MB == 512)
#define PHYS_SDRAM_SIZE		(512u * 1024 * 1024)
#define CONFIG_BOARD_MEM	"-512m"
#define BOOTARGS_CMA_SIZE	"cma=128M "
#elif (CONFIG_DDR_MB == 256)
#define PHYS_SDRAM_SIZE		(256u * 1024 * 1024)
#define CONFIG_BOARD_MEM	"-256m"
#define BOOTARGS_CMA_SIZE	"cma=64M "
#endif

#include "mx6_common.h"

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_IMX_THERMAL
#define CONFIG_FAT_WRITE

#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_STACKSIZE			(128 * 1024)

#ifndef CONFIG_SYS_NOSMP
#define CONFIG_SYS_NOSMP
#endif

/* PMIC ********************************************************************* */
#define CONFIG_POWER
#define CONFIG_POWER_I2C

#undef CONFIG_LDO_BYPASS_CHECK

/* Physical memory map ****************************************************** */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM					MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET 	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR 	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* SPI nor flash ************************************************************ */
#define CONFIG_SYS_BOOT_SPINOR
#if defined(CONFIG_SYS_BOOT_SPINOR)
#define CONFIG_SYS_USE_SPINOR
#define CONFIG_ENV_IS_IN_SPI_FLASH
#endif

#if defined(CONFIG_SYS_USE_SPINOR)
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_SST
#define CONFIG_MXC_SPI
#define CONFIG_SF_DEFAULT_BUS		0
#if defined(CONFIG_MYIMX6CB200)
#define CONFIG_SF_DEFAULT_CS		1
#elif (defined(CONFIG_MYIMX6CB314) || defined(CONFIG_MYIMX6CB336))
#define CONFIG_SF_DEFAULT_CS		0
#endif
#define CONFIG_SF_DEFAULT_SPEED		20000000
#define CONFIG_SF_DEFAULT_MODE		(SPI_MODE_0)
#endif

#if defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET			(768 * 1024)
#define CONFIG_ENV_SECT_SIZE		(64 * 1024)
#define CONFIG_ENV_SPI_BUS			CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS			CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE			CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_SF_DEFAULT_SPEED
#endif

#define CONFIG_ENV_SIZE					(8 * 1024)

/* UART ********************************************************************* */
#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE
#define CONFIG_CONSOLE_DEV			"ttymxc0"
#define CONFIG_BAUDRATE				115200

/* MMC Configs ************************************************************** */
#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_MMC_DEV_INDEX	1
#define CONFIG_SYS_FSL_ESDHC_ADDR	0

/* ENET ********************************************************************* */
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define CONFIG_PHYLIB
#define CONFIG_ETHPRIME				"FEC"
#define CONFIG_ARP_TIMEOUT     		200UL
#define IMX_FEC_BASE				ENET_BASE_ADDR

#if defined(CONFIG_TARGET_MYIMX6EK200)
#define CONFIG_FEC_XCV_TYPE			RMII
#define CONFIG_FEC_MXC_PHYADDR		0		/* MY-IMX6-MB200 */
#define CONFIG_PHY_SMSC
#elif (defined(CONFIG_TARGET_MYIMX6EK314) || defined(CONFIG_TARGET_MYIMX6EK336))
#define CONFIG_FEC_XCV_TYPE			RGMII
#define CONFIG_FEC_MXC_PHYADDR		5		/* MY-IMX6-MB314 */
#define CONFIG_PHY_MICREL
#define CONFIG_PHY_MICREL_KSZ9031
#endif

#define CONFIG_SERVERIP				192.168.137.99
#define CONFIG_IPADDR				192.168.137.81

#define CONFIG_IMX_PWM
#define IMX_PWM1_BASE		PWM1_BASE_ADDR
#define IMX_PWM2_BASE		PWM2_BASE_ADDR

#define CONFIG_UBOOT_LOGO_ENABLE

/* Framebuffer ************************************************************** */
#ifndef CONFIG_UBOOT_LOGO_ENABLE
#define CONFIG_VIDEO
#define CONFIG_VIDEO_IPUV3
#define CONFIG_CFB_CONSOLE
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#if defined(CONFIG_MX6DL)
#define CONFIG_IPUV3_CLK 198000000
#else
#define CONFIG_IPUV3_CLK 264000000
#endif
#define CONFIG_IMX_HDMI
#define CONFIG_IMX_VIDEO_SKIP
#endif  /* CONFIG_UBOOT_LOGO_ENABLE */

#ifdef CONFIG_UBOOT_LOGO_ENABLE
/* Select one of the output mode */
/*#define IPU_OUTPUT_MODE_HDMI*/
/*#define IPU_OUTPUT_MODE_LVDS*/
#define IPU_OUTPUT_MODE_LCD

#define CONFIG_FB_BASE		(CONFIG_SYS_TEXT_BASE + 0x1000000)
#define UBOOT_LOGO_BMP_ADDR	0x00100000

#ifdef CONFIG_MX6DL
#define CONFIG_IPUV3_CLK	270000000
#else
#define CONFIG_IPUV3_CLK	264000000
#endif

#ifdef IPU_OUTPUT_MODE_HDMI
/* For HDMI, 1920*1080 resolution */
#define DISPLAY_WIDTH			1920
#define DISPLAY_HEIGHT			1080
#define DISPLAY_BPP				24
#define DISPLAY_IF_BPP			24  /* RGB24 interface */

#define DISPLAY_HSYNC_START		148
#define DISPLAY_HSYNC_END		88
#define DISPLAY_HSYNC_WIDTH		44

#define DISPLAY_VSYNC_START		36
#define DISPLAY_VSYNC_END		4
#define DISPLAY_VSYNC_WIDTH		5

#define DISPLAY_PIX_CLOCK	148500000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz) */

#define DISPLAY_VSYNC_POLARITY			1
#define DISPLAY_HSYNC_POLARITY			1
#define DISPLAY_CLOCK_POLARITY			0
#define DISPLAY_DATA_POLARITY			0
#define DISPLAY_DATA_ENABLE_POLARITY	1

#define IPU_NUM			1  /* 1 for IPU1, 2 for IPU2. */
#define DI_NUM			0  /* 0 for DI0, 1 for DI1. */
#define DI_CLOCK_EXTERNAL_MODE  /* When clock external mode was defined, the DI clock root will be PLL5, without this macro, the DI root clock is IPU internal clock. */
#define CONFIG_IMX_HDMI
#endif

#ifdef IPU_OUTPUT_MODE_LVDS
	/* For LVDS, 1024*768 resolution */
#define DISPLAY_WIDTH			1024
#define DISPLAY_HEIGHT			600
#define DISPLAY_BPP				24
#define DISPLAY_IF_BPP			18  /* RGB666 interface */

#define DISPLAY_HSYNC_START		220
#define DISPLAY_HSYNC_END		40
#define DISPLAY_HSYNC_WIDTH		60

#define DISPLAY_VSYNC_START		21
#define DISPLAY_VSYNC_END		7
#define DISPLAY_VSYNC_WIDTH		10

#define DISPLAY_PIX_CLOCK		64000000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz) */

#define DISPLAY_VSYNC_POLARITY			0
#define DISPLAY_HSYNC_POLARITY			0
#define DISPLAY_CLOCK_POLARITY			0
#define DISPLAY_DATA_POLARITY			0
#define DISPLAY_DATA_ENABLE_POLARITY	1

#define IPU_NUM			1  /* 1 for IPU1, 2 for IPU2. */
#define DI_NUM			1  /* 0 for DI0, 1 for DI1. */
#define LVDS_PORT		1  /* 0 for LVDS0, 1 for LVDS1. Note: LVDS and DI should be same number. */
#define DI_CLOCK_EXTERNAL_MODE  /* When clock external mode was defined, the DI clock root will be PLL3 PFD1, without this macro, the DI root clock is IPU internal clock. */
	/*#define LVDS_CLOCK_SRC_PLL5*/
#endif

#ifdef IPU_OUTPUT_MODE_LCD
	/* For LCD, 800*480 resolution */
#define DISPLAY_WIDTH		800
#define DISPLAY_HEIGHT		480
#define DISPLAY_BPP			24
#define DISPLAY_IF_BPP		24  /* RGB24 interface */

#define DISPLAY_HSYNC_START		40
#define DISPLAY_HSYNC_END		60
#define DISPLAY_HSYNC_WIDTH		20

#define DISPLAY_VSYNC_START		10
#define DISPLAY_VSYNC_END		10
#define DISPLAY_VSYNC_WIDTH		10

#define DISPLAY_PIX_CLOCK		27000000  /*(DISPLAY_HSYNC_START + DISPLAY_HSYNC_END + DISPLAY_HSYNC_WIDTH + DISPLAY_WIDTH) * (DISPLAY_VSYNC_START + DISPLAY_VSYNC_END + DISPLAY_VSYNC_WIDTH + DISPLAY_HEIGHT) * refresh rate (60Hz) */

#define DISPLAY_VSYNC_POLARITY			0
#define DISPLAY_HSYNC_POLARITY			0
#define DISPLAY_CLOCK_POLARITY			1
#define DISPLAY_DATA_POLARITY			0
#define DISPLAY_DATA_ENABLE_POLARITY	1

#define IPU_NUM			1  /* 1 for IPU1, 2 for IPU2. */
#define DI_NUM			0  /* 0 for DI0, 1 for DI1. */
#define DI_CLOCK_EXTERNAL_MODE  /* When clock external mode was defined, the DI clock root will be PLL5, without this macro, the DI root clock is IPU internal clock. */
#endif
#endif  /* CONFIG_UBOOT_LOGO_ENABLE */

/* Command definition ******************************************************* */
#define CONFIG_CMD_BMODE

/* Command: ENET */
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII

/* Command: Memory test */
#define CONFIG_CMD_MEMTEST
#if defined(CONFIG_CMD_MEMTEST)
#define CONFIG_SYS_MEMTEST_START	0x10000000
#define CONFIG_SYS_MEMTEST_END		0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH	0x10800000
#endif

/* Command: I2C Configs */
#define CONFIG_CMD_I2C
#if defined(CONFIG_CMD_I2C)
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */
#define CONFIG_SYS_I2C_SPEED		100000
#endif

/* Command: USB Configs */
#define CONFIG_CMD_USB
#if defined(CONFIG_CMD_USB)
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_USB_MAX_CONTROLLER_COUNT		1
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET	/* For OTG port */
#define CONFIG_MXC_USB_PORTSC			(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS			0
#endif

/* Command: SATA */
#define CONFIG_CMD_SATA
#if defined(CONFIG_CMD_SATA)
#define CONFIG_DWC_AHSATA
#define CONFIG_SYS_SATA_MAX_DEVICE		1
#define CONFIG_DWC_AHSATA_PORT_ID		0
#define CONFIG_DWC_AHSATA_BASE_ADDR		SATA_ARB_BASE_ADDR
#define CONFIG_LBA48
#define CONFIG_LIBATA
#endif

/* Environment definition *************************************************** */

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

/* ENV: MFG */
#define CONFIG_MFG_ENV_SETTINGS		\
    "bootargs_mfg=setenv bootargs console=${console},${baudrate} "		\
        BOOTARGS_CMA_SIZE           \
		"rdinit=/linuxrc "			\
		"g_mass_storage.stall=0 g_mass_storage.removable=1 "	\
		"g_mass_storage.file=/fat g_mass_storage.ro=1 "			\
		"g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "	\
		"g_mass_storage.iSerialNumber=\"\" "	\
		"enable_wait_mode=off "		\
		"\0"	\
	"bootcmd_mfg=run bootargs_mfg; "    \
    "bootz ${kernel_addr_r} ${ramdisk_addr_r} ${fdt_addr_r}; "     \
    "bootz ${kernel_addr_r} ${ramdisk_addr_r};\0"

/* COMMON ENV *************************************************************** */
#define CONFIG_EXTRA_ENV_SETTINGS	\
    CONFIG_MFG_ENV_SETTINGS		\
	"bootdelay=1\0"		\
	"console="CONFIG_CONSOLE_DEV"\0"	\
	"baudrate="__stringify(CONFIG_BAUDRATE)"\0"		\
	"loadaddr=0x12000000\0"		\
	"fdt_addr_r=0x18000000\0"		\
	"kernel_addr_r=0x12000000\0"	\
	"ramdisk_addr_r=0x12C00000\0"	\
	"fdt_high=0xffffffff\0"	  \
	"initrd_high=0xffffffff\0" \
	"mmcdev=1\0" \
	"mmcpart=1\0"		\
	"file_scr=my_environment.scr\0"	\
	"file_ubot=uboot-"CONFIG_BOARD_NAME""CONFIG_BOARD_CPU""CONFIG_BOARD_MEM".imx\0"	\
	"fdt_file="CONFIG_BOARD_NAME""CONFIG_BOARD_CPU""CONFIG_BOARD_MEM".dtb\0"		\
	"mmcautodetect=yes\0"	\
    "machid="__stringify(CONFIG_MACH_TYPE)"\0"   \
	"smp="CONFIG_SYS_NOSMP"\0"		\
    "bootargs_cma="BOOTARGS_CMA_SIZE"\0" \
    "my_env_flag=unset\0"   \
    "load_scr=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${file_scr};\0"    

#define CONFIG_BOOTCOMMAND		\
    "if test ${my_env_flag} = unset; then "   \
		"run load_scr; source ${loadaddr}; saveenv; "   \
    "fi; "      \
    "run bootcmd_my; "

#endif                         /* __MYIMX6A9_CONFIG_H */
