
#ifndef __MYIMX6_COMMON_CONFIG_H
#define __MYIMX6_COMMON_CONFIG_H

 /* High Level Configuration Options */
#define CONFIG_ARMV7	/* This is armv7 Cortex-A9 CPU core */
#define CONFIG_MXC

#if defined(CONFIG_BOARD_IS_MYIMXEK200)
#if defined(CONFIG_MX6Q)
#define CONFIG_MYIMX6EK200_6Q
#else
#define CONFIG_MYIMX6EK200_6U
#endif
#elif defined(CONFIG_BOARD_IS_MYIMXEK314)
#if defined(CONFIG_MX6Q)
#define CONFIG_MYIMX6EK314_6Q
#else
#define CONFIG_MYIMX6EK314_6U
#endif
#endif

#define CONFIG_FLASH_HEADER
#define CONFIG_FLASH_HEADER_OFFSET 	0x400
#define CONFIG_MX6_CLK32	   		32768

#define CONFIG_SKIP_RELOCATE_UBOOT

#define CONFIG_ARCH_CPU_INIT
#define CONFIG_L2_OFF  /* disable L2 cache first*/

#define CONFIG_MX6_HCLK_FREQ		24000000

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_SYS_64BIT_VSPRINTF

#define BOARD_LATE_INIT

#define CONFIG_CMDLINE_TAG	/* enable passing of ATAGs */
#define CONFIG_SERIAL_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_MXC_GPIO

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(2 * 1024 * 1024)
/* size in bytes reserved for initial data */
#define CONFIG_SYS_GBL_DATA_SIZE	128

/*
 * Hardware drivers
 */
#define CONFIG_MXC_UART
#define CONFIG_UART_BASE_ADDR   	UART1_BASE_ADDR

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_CONS_INDEX			1
#define CONFIG_BAUDRATE				115200
#define CONFIG_SYS_BAUDRATE_TABLE	{9600, 19200, 38400, 57600, 115200}

/***********************************************************
 * Command definition
 ***********************************************************/

#include <config_cmd_default.h>

#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_CMD_NET
#define CONFIG_NET_RETRY_COUNT  	100
#define CONFIG_NET_MULTI 			1
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_DNS

#define CONFIG_CMD_SPI
#define CONFIG_CMD_I2C
#define CONFIG_CMD_IMXOTP

#define CONFIG_CMD_SF
#define CONFIG_CMD_MMC
#define CONFIG_CMD_ENV
#define CONFIG_CMD_REGUL

#define CONFIG_CMD_CLOCK
#define CONFIG_REF_CLK_FREQ CONFIG_MX6_HCLK_FREQ

#undef CONFIG_CMD_IMLS

#define CONFIG_PRIME			"FEC0"
#define CONFIG_LOADADDR			0x10800000	/* loadaddr env var */
#define CONFIG_RD_LOADADDR		(CONFIG_LOADADDR + 0x300000)

#if defined(CONFIG_DDR_32BIT)
#define BOOTARGS_BASE			"console=ttymxc0,115200 nosmp "
#define SPLASHIMAGE				"0x1D000000"
#else
#define BOOTARGS_BASE			"console=ttymxc0,115200 "
#define SPLASHIMAGE				"0x30000000"
#endif

#if defined(CONFIG_MFG)
/* ************************************************************************** */
#define CONFIG_BOOTDELAY 		0
#define BOOTARGS_MFG			"rdinit=/linuxrc enable_wait_mode=off"

#define BOOTARGS(str1,str2)		str1 str2
#define CONFIG_BOOTARGS         BOOTARGS(BOOTARGS_BASE, BOOTARGS_MFG)
#define CONFIG_BOOTCOMMAND      "bootm 0x10800000 0x10c00000"

#define	CONFIG_EXTRA_ENV_SETTINGS					\
		"netdev=eth0\0"						\
		"ethprime=FEC0\0"					\
		"uboot=u-boot.bin\0"			\
		"kernel=uImage\0"

#else
/* ************************************************************************** */
#if defined(CONFIG_BOARD_IS_MYIMXEK200)
#define EK_NAME					"myimx6ek200"
#elif defined(CONFIG_BOARD_IS_MYIMXEK314)
#define EK_NAME					"myimx6ek314"
#else
#define EK_NAME					"unknow"
#endif

#define CONFIG_BOOTDELAY 		3
#define CONFIG_CMD_IMX_DOWNLOAD_MODE
#define BOOTARGS_VIDEO			"video=mxcfb0:dev=ldb,LDB-1024X600,if=RGB666"

#define	CONFIG_EXTRA_ENV_SETTINGS					\
		"netdev=eth0\0"						\
		"ethprime=FEC0\0"					\
		"uboot=uboot-"EK_NAME"-"MYIMX6EK_SPEC".bin\0"			\
		"kernel=uImage-"EK_NAME"\0"				\
		"nfsroot=/home/myzr/srv/nfs/rootfs\0"				\
		"bootargs_base=setenv bootargs "BOOTARGS_BASE" "BOOTARGS_VIDEO"\0"\
		"bootargs_mmc=setenv bootargs ${bootargs} ip=none "     \
			"root=/dev/mmcblk0p1 rootwait\0"                \
		"bootcmd_tftp=run bootargs_base bootargs_mmc; "		\
			"tftpboot ${loadaddr} ${kernel}; bootm\0"	\
		"bootargs_nfs=setenv bootargs ${bootargs} root=/dev/nfs "\
			"ip=${ipaddr} nfsroot=${serverip}:${nfsroot},v3,tcp\0"\
		"bootcmd_net=run bootargs_base bootargs_nfs; "		\
			"tftpboot ${loadaddr} ${kernel}; bootm\0"	\
		"bootcmd_mmc=run bootargs_base bootargs_mmc; "   \
			"mmc dev 2; mmc read ${loadaddr} 0x800 0x2000; bootm\0"	\
		"bootcmd=run bootcmd_mmc\0"                             \
		"update_uboot=tftpboot ${loadaddr} ${uboot}; "SF_PROBE_CMD"; sf erase 0 0x200000; sf write ${loadaddr} 0 0x80000\0"	  \
		"update_uImage=tftpboot ${loadaddr} ${kernel}; mmc dev 2; mmc write ${loadaddr} 0x800 0x2000\0" \
		"splashimage="SPLASHIMAGE"\0"	\
		"splashpos=m,m\0"	\
		"lvds_num=1\0"
#endif
/* ************************************************************************** */

#define CONFIG_ARP_TIMEOUT	200UL

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP			/* undef to save memory */
#if defined(CONFIG_BOARD_IS_MYIMXEK200)
#define CONFIG_SYS_PROMPT			"MYIMX6EK200 > "
#elif defined(CONFIG_BOARD_IS_MYIMXEK314)
#define CONFIG_SYS_PROMPT			"MYIMX6EK314 > "
#else
#define CONFIG_SYS_PROMPT			"u-boot > "
#endif

#define CONFIG_AUTO_COMPLETE
#if defined(CONFIG_MFG)
#define CONFIG_SYS_CBSIZE			256		/* Console I/O Buffer Size */
#else
#define CONFIG_SYS_CBSIZE			1024	/* Console I/O Buffer Size */
#endif
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE 			(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 128)
#define CONFIG_SYS_MAXARGS			128		/* max number of command args */
#define CONFIG_SYS_BARGSIZE 		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size */

#define CONFIG_SYS_MEMTEST_START	0x10000000	/* memtest works on */
#define CONFIG_SYS_MEMTEST_END		0x10010000

#undef	CONFIG_SYS_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_SYS_HZ				1000

#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_HUSH_PARSER		1 	/* Use the HUSH parser */
#ifdef	CONFIG_SYS_HUSH_PARSER
#define	CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#endif

#if defined(CONFIG_BOARD_IS_MYIMXEK200)
#define CONFIG_ENET_RMII
#define CONFIG_FEC0_PHY_ADDR		0xFF
#elif defined(CONFIG_BOARD_IS_MYIMXEK314)
#define CONFIG_FEC0_PHY_ADDR		6
#define CONFIG_PHY_MICREL_KSZ9021
#else
#define CONFIG_FEC0_PHY_ADDR		-1
#endif

#define CONFIG_FEC0_IOBASE			ENET_BASE_ADDR
#define CONFIG_FEC0_PINMUX			-1
#define CONFIG_FEC0_MIIBASE			-1
#define CONFIG_GET_FEC_MAC_ADDR_FROM_IIM
#define CONFIG_MXC_FEC
// #define CONFIG_DISCOVER_PHY
#define CONFIG_ETH_PRIME
#define CONFIG_RMII
#define CONFIG_CMD_MII
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_IPADDR				192.168.18.81
#define CONFIG_SERVERIP				192.168.18.18
#define CONFIG_NETMASK				255.255.255.0

/*
 * OCOTP Configs
 */
#ifdef CONFIG_CMD_IMXOTP
	#define CONFIG_IMX_OTP
	#define IMX_OTP_BASE			OCOTP_BASE_ADDR
	#define IMX_OTP_ADDR_MAX		0x7F
	#define IMX_OTP_DATA_ERROR_VAL	0xBADABADA
#endif

/*
 * SPI Configs
 */
#ifdef CONFIG_CMD_SF
	#define CONFIG_FSL_SF			1
	#define CONFIG_SPI_FLASH_IMX_SST		1
#if defined(CONFIG_BOARD_IS_MYIMXEK200)
	#define CONFIG_SPI_FLASH_CS		1
	#define SF_PROBE_CMD			"sf probe 1"
#elif defined(CONFIG_BOARD_IS_MYIMXEK314)
	#define CONFIG_SPI_FLASH_CS 	0
	#define SF_PROBE_CMD			"sf probe 0"
#endif
	#define CONFIG_IMX_ECSPI
	#define IMX_CSPI_VER_2_3		1
	#define MAX_SPI_BYTES			(64 * 4)
#endif

/* Regulator Configs */
#ifdef CONFIG_CMD_REGUL
	#define CONFIG_ANATOP_REGULATOR
	#define CONFIG_CORE_REGULATOR_NAME 		"vdd1p1"
	#define CONFIG_PERIPH_REGULATOR_NAME 	"vdd1p1"
#endif

/*
 * MMC Configs
 */
#ifdef CONFIG_CMD_MMC
	#define CONFIG_MMC
	#define CONFIG_GENERIC_MMC
	#define CONFIG_IMX_MMC
	#define CONFIG_SYS_FSL_USDHC_NUM    3
	#define CONFIG_SYS_FSL_ESDHC_ADDR   0
	#define CONFIG_SYS_MMC_ENV_DEV  	2
	#define CONFIG_DOS_PARTITION		1
	#define CONFIG_CMD_FAT				1
	#define CONFIG_CMD_EXT2				1

	/* detect whether SD1, 2, 3, or 4 is boot device */
	#define CONFIG_DYNAMIC_MMC_DEVNO

	/* SD3 and SD4 are 8 bit */
	#define CONFIG_MMC_8BIT_PORTS   	0xC
	/* Setup target delay in DDR mode for each SD port */
	#define CONFIG_GET_DDR_TARGET_DELAY
#endif

/*
 * I2C Configs
 */
#ifdef CONFIG_CMD_I2C
	#define CONFIG_HARD_I2C         1
	#define CONFIG_I2C_MXC          1
	#define CONFIG_SYS_I2C_PORT             I2C3_BASE_ADDR
	#define CONFIG_SYS_I2C_SPEED            100000
	#define CONFIG_SYS_I2C_SLAVE            0x8
#endif

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE				(128 * 1024)	/* regular stack */

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM_1				CSD0_DDR_BASE_ADDR
#if defined(CONFIG_RAM_SIZE_IS_2G)
#define PHYS_SDRAM_1_SIZE			(2u * 1024 * 1024 * 1024)
#elif defined(CONFIG_RAM_SIZE_IS_512M)
#define PHYS_SDRAM_1_SIZE			(512 * 1024 * 1024)
#elif defined(CONFIG_RAM_SIZE_IS_1G)
#define PHYS_SDRAM_1_SIZE			(1u * 1024 * 1024 * 1024)
#endif
#define iomem_valid_addr(addr, size) \
	(addr >= PHYS_SDRAM_1 && addr <= (PHYS_SDRAM_1 + PHYS_SDRAM_1_SIZE))

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_NO_FLASH
/* Monitor at beginning of flash */
#if defined(CONFIG_MFG)
#define CONFIG_ENV_SECT_SIZE    (128 * 1024)
#else
#define CONFIG_ENV_SECT_SIZE    	(8 * 1024)
#define CONFIG_FSL_ENV_IN_SF
#endif

#define CONFIG_ENV_SIZE         	CONFIG_ENV_SECT_SIZE

#if defined(CONFIG_FSL_ENV_IN_SF)
	#define CONFIG_ENV_IS_IN_SPI_FLASH	1
#if defined(CONFIG_BOARD_IS_MYIMXEK200)
	#define CONFIG_ENV_SPI_CS		1
#elif defined(CONFIG_BOARD_IS_MYIMXEK314)
	#define CONFIG_ENV_SPI_CS 		0
#endif
	#define CONFIG_ENV_OFFSET       	(768 * 1024)
#else
	#define CONFIG_ENV_IS_NOWHERE		1
#endif

/*-----------------------------------------------------------------------
 * SPLASH SCREEN Configs
 */
#define CONFIG_SPLASH_SCREEN
#ifdef CONFIG_SPLASH_SCREEN
	/* Framebuffer and LCD */
	#define CONFIG_LCD
	#define CONFIG_FB_BASE				(TEXT_BASE + 0x300000)
	#define CONFIG_SYS_CONSOLE_IS_IN_ENV

	#define CONFIG_IPU_V3H
	#define CONFIG_VIDEO_MX5
	#define CONFIG_IPU_CLKRATE			260000000
	#define CONFIG_SYS_CONSOLE_ENV_OVERWRITE
	#define CONFIG_SYS_CONSOLE_OVERWRITE_ROUTINE
	#define LCD_BPP						LCD_COLOR16
	#define CONFIG_CMD_BMP
	#define CONFIG_BMP_8BPP
	#define CONFIG_SPLASH_SCREEN_ALIGN
	#define CONFIG_SYS_WHITE_ON_BLACK

	#define CONFIG_IMX_PWM
	#define IMX_PWM1_BASE    			PWM1_BASE_ADDR
	#define IMX_PWM2_BASE    			PWM2_BASE_ADDR
#endif /* CONFIG_SPLASH_SCREEN */
#endif                         /* __MYIMX6_COMMON_CONFIG_H */
