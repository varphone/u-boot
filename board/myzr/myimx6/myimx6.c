
#include <common.h>
#include <asm/io.h>
#include <asm/arch/mx6.h>
#include <asm/arch/mx6_pins.h>
#if defined(CONFIG_SECURE_BOOT)
#include <asm/arch/mx6_secure.h>
#endif
#include <asm/arch/mx6dl_pins.h>
#include <asm/arch/iomux-v3.h>
#include <asm/arch/regs-anadig.h>
#include <asm/errno.h>
#ifdef CONFIG_MXC_FEC
#include <miiphy.h>
#endif

#if defined(CONFIG_VIDEO_MX5)
#include <asm/imx_pwm.h>
#include <linux/list.h>
#include <linux/fb.h>
#include <linux/mxcfb.h>
#include <ipu.h>
#endif
#if defined(CONFIG_VIDEO_MX5) || defined(CONFIG_MXC_EPDC)
#include <lcd.h>
#endif

#include "../../../drivers/video/mxc_epdc_fb.h"

#ifdef CONFIG_IMX_ECSPI
#include <imx_spi.h>
#endif

#if CONFIG_I2C_MXC
#include <i2c.h>
#endif

#ifdef CONFIG_CMD_MMC
#include <mmc.h>
#include <fsl_esdhc.h>
#endif

#ifdef CONFIG_CMD_CLOCK
#include <asm/clock.h>
#endif

#ifdef CONFIG_CMD_IMXOTP
#include <imx_otp.h>
#endif

#ifdef CONFIG_MXC_GPIO
#include <asm/gpio.h>
#include <asm/arch/gpio.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

static enum boot_device boot_dev;

#if defined(CONFIG_ENET_RMII)
#define FEC_PHY_RESET IMX_GPIO_NR(1, 23)
#else
#define FEC_PHY_RESET IMX_GPIO_NR(1, 25)
#endif

#ifdef CONFIG_VIDEO_MX5
extern unsigned char myzr_bmp_logo[];
extern int myzr_bmp_logo_size;
extern int g_ipu_hw_rev;

#if defined(CONFIG_BMP_8BPP)
unsigned short colormap[256];
#elif defined(CONFIG_BMP_16BPP)
unsigned short colormap[65536];
#else
unsigned short colormap[16777216];
#endif

static struct pwm_device pwm0 = {
	.pwm_id = 0,
	.pwmo_invert = 0,
};

static int di = 1;

extern int ipuv3_fb_init(struct fb_videomode *mode, int di,
		int interface_pix_fmt,
		ipu_di_clk_parent_t di_clk_parent,
		int di_clk_val);

static struct fb_videomode lvds_xga = {
	"XGA", 60, 1024, 600, 15385, 220, 40, 21, 7, 60, 10,
	FB_SYNC_EXT,
	FB_VMODE_NONINTERLACED,
	0,
};

vidinfo_t panel_info;
#endif

static inline void setup_boot_device(void)
{
	uint soc_sbmr = readl(SRC_BASE_ADDR + 0x4);
	uint bt_mem_ctl = (soc_sbmr & 0x000000FF) >> 4 ;
	uint bt_mem_type = (soc_sbmr & 0x00000008) >> 3;

	switch (bt_mem_ctl) {
		case 0x0:
			if (bt_mem_type)
				boot_dev = ONE_NAND_BOOT;
			else
				boot_dev = WEIM_NOR_BOOT;
			break;
		case 0x2:
			boot_dev = SATA_BOOT;
			break;
		case 0x3:
			if (bt_mem_type)
				boot_dev = I2C_BOOT;
			else
				boot_dev = SPI_NOR_BOOT;
			break;
		case 0x4:
		case 0x5:
			boot_dev = SD_BOOT;
			break;
		case 0x6:
		case 0x7:
			boot_dev = MMC_BOOT;
			break;
		case 0x8 ... 0xf:
			boot_dev = NAND_BOOT;
			break;
		default:
			boot_dev = UNKNOWN_BOOT;
			break;
	}
}

enum boot_device get_boot_device(void)
{
	return boot_dev;
}

u32 get_board_rev(void)
{
	return fsl_system_rev;
}

#if defined(CONFIG_UDELAY)
/* Note: udelay() is not accurate for i2c timing */
static void __udelay(int time)
{
	int i, j;

	for (i = 0; i < time; i++) {
		for (j = 0; j < 200; j++) {
			asm("nop");
			asm("nop");
		}
	}
}
#endif

#if defined(CONFIG_ENET_RMII)
#define ANATOP_PLL_LOCK                 0x80000000
#define ANATOP_PLL_BYPASS_MASK          0x00010000
#define ANATOP_PLL_PWDN_MASK            0x00001000
#define ANATOP_FEC_PLL_ENABLE_MASK      0x00002000
static int setup_fec(void)
{
	u32 reg = 0;
	s32 timeout = 100000;

	/* Enable PLLs */
	reg = readl(ANATOP_BASE_ADDR + 0xe0); /* ENET PLL */
	if ((reg & ANATOP_PLL_PWDN_MASK) || (!(reg & ANATOP_PLL_LOCK))) {
		reg &= ~ANATOP_PLL_PWDN_MASK;
		writel(reg, ANATOP_BASE_ADDR + 0xe0);
		while (timeout--) {
			if (readl(ANATOP_BASE_ADDR + 0xe0) & ANATOP_PLL_LOCK)
				break;
		}
		if (timeout <= 0)
			return -1;
	}

	/* Enable FEC clock */
	reg |= ANATOP_FEC_PLL_ENABLE_MASK;  //reg &= ~ANATOP_FEC_PLL_ENABLE_MASK;
	reg &= ~ANATOP_PLL_BYPASS_MASK;
	writel(reg, ANATOP_BASE_ADDR + 0xe0);

	return 0;
}
#endif

int dram_init(void)
{
	/*
	 * Switch PL301_FAST2 to DDR Dual-channel mapping
	 * however this block the boot up, temperory redraw
	 */
	/*
	 * u32 reg = 1;
	 * writel(reg, GPV0_BASE_ADDR);
	 */

	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return 0;
}

static void setup_uart(void)
{
#if defined CONFIG_MX6Q
	/* UART1 TXD */
	mxc_iomux_v3_setup_pad(MX6Q_PAD_CSI0_DAT10__UART1_TXD);
	/* UART1 RXD */
	mxc_iomux_v3_setup_pad(MX6Q_PAD_CSI0_DAT11__UART1_RXD);
#elif defined CONFIG_MX6DL
	/* UART1 TXD */
	mxc_iomux_v3_setup_pad(MX6DL_PAD_CSI0_DAT10__UART1_TXD);
	/* UART1 RXD */
	mxc_iomux_v3_setup_pad(MX6DL_PAD_CSI0_DAT11__UART1_RXD);
#endif
}

#ifdef CONFIG_I2C_MXC
static void setup_i2c(unsigned int module_base)
{
	unsigned int reg;

	switch (module_base) {
		case I2C1_BASE_ADDR:
#if defined CONFIG_MX6Q
			/* i2c1 SDA */
			mxc_iomux_v3_setup_pad(MX6Q_PAD_CSI0_DAT8__I2C1_SDA);
			/* i2c1 SCL */
			mxc_iomux_v3_setup_pad(MX6Q_PAD_CSI0_DAT9__I2C1_SCL);
#elif defined CONFIG_MX6DL
			/* i2c1 SDA */
			mxc_iomux_v3_setup_pad(MX6DL_PAD_CSI0_DAT8__I2C1_SDA);
			/* i2c1 SCL */
			mxc_iomux_v3_setup_pad(MX6DL_PAD_CSI0_DAT9__I2C1_SCL);
#endif

			/* Enable i2c clock */
			reg = readl(CCM_BASE_ADDR + CLKCTL_CCGR2);
			reg |= 0xC0;
			writel(reg, CCM_BASE_ADDR + CLKCTL_CCGR2);

			break;
		case I2C2_BASE_ADDR:
#if defined CONFIG_MX6Q
			/* i2c2 SDA */
			mxc_iomux_v3_setup_pad(MX6Q_PAD_KEY_ROW3__I2C2_SDA);
			/* i2c2 SCL */
			mxc_iomux_v3_setup_pad(MX6Q_PAD_KEY_COL3__I2C2_SCL);
#elif defined CONFIG_MX6DL
			/* i2c2 SDA */
			mxc_iomux_v3_setup_pad(MX6DL_PAD_KEY_ROW3__I2C2_SDA);
			/* i2c2 SCL */
			mxc_iomux_v3_setup_pad(MX6DL_PAD_KEY_COL3__I2C2_SCL);
#endif

			/* Enable i2c clock */
			reg = readl(CCM_BASE_ADDR + CLKCTL_CCGR2);
			reg |= 0x300;
			writel(reg, CCM_BASE_ADDR + CLKCTL_CCGR2);

			break;
		case I2C3_BASE_ADDR:
#if defined CONFIG_MX6Q
			/* GPIO_3 for I2C3_SCL */
			mxc_iomux_v3_setup_pad(MX6Q_PAD_GPIO_3__I2C3_SCL);
			/* GPIO_6 for I2C3_SDA */
			mxc_iomux_v3_setup_pad(MX6Q_PAD_GPIO_6__I2C3_SDA);

#elif defined CONFIG_MX6DL
			/* GPIO_3 for I2C3_SCL */
			mxc_iomux_v3_setup_pad(MX6DL_PAD_GPIO_3__I2C3_SCL);
			/* GPIO_6 for I2C3_SDA */
			mxc_iomux_v3_setup_pad(MX6DL_PAD_GPIO_6__I2C3_SDA);
#endif
			/* Enable i2c clock */
			reg = readl(CCM_BASE_ADDR + CLKCTL_CCGR2);
			reg |= 0xC00;
			writel(reg, CCM_BASE_ADDR + CLKCTL_CCGR2);

			break;
		default:
			printf("Invalid I2C base: 0x%x\n", module_base);
			break;
	}
}
#endif

#ifdef CONFIG_IMX_ECSPI
s32 spi_get_cfg(struct imx_spi_dev_t *dev)
{
	switch (dev->slave.cs) {
		case 0:
			/* SPI-NOR */
			dev->base = ECSPI1_BASE_ADDR;
			dev->freq = 25000000;
			dev->ss_pol = IMX_SPI_ACTIVE_LOW;
			dev->ss = 0;
			dev->fifo_sz = 64 * 4;
			dev->us_delay = 0;
			break;
		case 1:
			/* SPI-NOR */
			dev->base = ECSPI1_BASE_ADDR;
			dev->freq = 25000000;
			dev->ss_pol = IMX_SPI_ACTIVE_LOW;
			dev->ss = 1;
			dev->fifo_sz = 64 * 4;
			dev->us_delay = 0;
			break;
		default:
			printf("Invalid Bus ID!\n");
	}

	return 0;
}

void spi_io_init(struct imx_spi_dev_t *dev)
{
	u32 reg;

	switch (dev->base) {
		case ECSPI1_BASE_ADDR:
			/* Enable clock */
			reg = readl(CCM_BASE_ADDR + CLKCTL_CCGR1);
			reg |= 0x3;
			writel(reg, CCM_BASE_ADDR + CLKCTL_CCGR1);

#if defined CONFIG_MX6Q
			/* SCLK */
			mxc_iomux_v3_setup_pad(MX6Q_PAD_EIM_D16__ECSPI1_SCLK);
			/* MISO */
			mxc_iomux_v3_setup_pad(MX6Q_PAD_EIM_D17__ECSPI1_MISO);
			/* MOSI */
			mxc_iomux_v3_setup_pad(MX6Q_PAD_EIM_D18__ECSPI1_MOSI);

			if (dev->ss == 1)
				mxc_iomux_v3_setup_pad(MX6Q_PAD_EIM_D19__ECSPI1_SS1);
			else
				mxc_iomux_v3_setup_pad(MX6Q_PAD_EIM_EB2__ECSPI1_SS0);
#elif defined CONFIG_MX6DL
			/* SCLK */
			mxc_iomux_v3_setup_pad(MX6DL_PAD_EIM_D16__ECSPI1_SCLK);
			/* MISO */
			mxc_iomux_v3_setup_pad(MX6DL_PAD_EIM_D17__ECSPI1_MISO);
			/* MOSI */
			mxc_iomux_v3_setup_pad(MX6DL_PAD_EIM_D18__ECSPI1_MOSI);

			if (dev->ss == 1)
				mxc_iomux_v3_setup_pad(MX6DL_PAD_EIM_D19__ECSPI1_SS1);
			else
				mxc_iomux_v3_setup_pad(MX6DL_PAD_EIM_EB2__ECSPI1_SS0);
#endif
			break;
		case ECSPI2_BASE_ADDR:
		case ECSPI3_BASE_ADDR:
			/* ecspi2-3 fall through */
			break;
		default:
			break;
	}
}
#endif

#ifdef CONFIG_NET_MULTI
int board_eth_init(bd_t *bis)
{
	int rc = -ENODEV;

	return rc;
}
#endif

#ifdef CONFIG_CMD_MMC

/* On this board, only SD3 can support 1.8V signalling
 * that is required for UHS-I mode of operation.
 * Last element in struct is used to indicate 1.8V support.
 */
struct fsl_esdhc_cfg usdhc_cfg[3] = {
	{USDHC2_BASE_ADDR, 1, 1, 1, 0},
	{USDHC3_BASE_ADDR, 1, 1, 1, 0},
	{USDHC4_BASE_ADDR, 1, 1, 1, 0},
};

#if defined CONFIG_MX6Q
iomux_v3_cfg_t usdhc2_pads[] = {
	MX6Q_PAD_SD2_CLK__USDHC2_CLK,
	MX6Q_PAD_SD2_CMD__USDHC2_CMD,
	MX6Q_PAD_SD2_DAT0__USDHC2_DAT0,
	MX6Q_PAD_SD2_DAT1__USDHC2_DAT1,
	MX6Q_PAD_SD2_DAT2__USDHC2_DAT2,
	MX6Q_PAD_SD2_DAT3__USDHC2_DAT3,
};

iomux_v3_cfg_t usdhc3_pads[] = {
	MX6Q_PAD_SD3_CLK__USDHC3_CLK,
	MX6Q_PAD_SD3_CMD__USDHC3_CMD,
	MX6Q_PAD_SD3_DAT0__USDHC3_DAT0,
	MX6Q_PAD_SD3_DAT1__USDHC3_DAT1,
	MX6Q_PAD_SD3_DAT2__USDHC3_DAT2,
	MX6Q_PAD_SD3_DAT3__USDHC3_DAT3,
};

iomux_v3_cfg_t usdhc4_pads[] = {
	MX6Q_PAD_SD4_CLK__USDHC4_CLK,
	MX6Q_PAD_SD4_CMD__USDHC4_CMD,
	MX6Q_PAD_SD4_DAT0__USDHC4_DAT0,
	MX6Q_PAD_SD4_DAT1__USDHC4_DAT1,
	MX6Q_PAD_SD4_DAT2__USDHC4_DAT2,
	MX6Q_PAD_SD4_DAT3__USDHC4_DAT3,
	MX6Q_PAD_SD4_DAT4__USDHC4_DAT4,
	MX6Q_PAD_SD4_DAT5__USDHC4_DAT5,
	MX6Q_PAD_SD4_DAT6__USDHC4_DAT6,
	MX6Q_PAD_SD4_DAT7__USDHC4_DAT7,
};

#elif defined CONFIG_MX6DL
iomux_v3_cfg_t usdhc2_pads[] = {
	MX6DL_PAD_SD2_CLK__USDHC2_CLK,
	MX6DL_PAD_SD2_CMD__USDHC2_CMD,
	MX6DL_PAD_SD2_DAT0__USDHC2_DAT0,
	MX6DL_PAD_SD2_DAT1__USDHC2_DAT1,
	MX6DL_PAD_SD2_DAT2__USDHC2_DAT2,
	MX6DL_PAD_SD2_DAT3__USDHC2_DAT3,
};

iomux_v3_cfg_t usdhc3_pads[] = {
	MX6DL_PAD_SD3_CLK__USDHC3_CLK,
	MX6DL_PAD_SD3_CMD__USDHC3_CMD,
	MX6DL_PAD_SD3_DAT0__USDHC3_DAT0,
	MX6DL_PAD_SD3_DAT1__USDHC3_DAT1,
	MX6DL_PAD_SD3_DAT2__USDHC3_DAT2,
	MX6DL_PAD_SD3_DAT3__USDHC3_DAT3,
};

iomux_v3_cfg_t usdhc4_pads[] = {
	MX6DL_PAD_SD4_CLK__USDHC4_CLK,
	MX6DL_PAD_SD4_CMD__USDHC4_CMD,
	MX6DL_PAD_SD4_DAT0__USDHC4_DAT0,
	MX6DL_PAD_SD4_DAT1__USDHC4_DAT1,
	MX6DL_PAD_SD4_DAT2__USDHC4_DAT2,
	MX6DL_PAD_SD4_DAT3__USDHC4_DAT3,
	MX6DL_PAD_SD4_DAT4__USDHC4_DAT4,
	MX6DL_PAD_SD4_DAT5__USDHC4_DAT5,
	MX6DL_PAD_SD4_DAT6__USDHC4_DAT6,
	MX6DL_PAD_SD4_DAT7__USDHC4_DAT7,
};
#endif

int usdhc_gpio_init(bd_t *bis)
{
	s32 status = 0;
	u32 index = 0;

	for (index = 0; index < CONFIG_SYS_FSL_USDHC_NUM;
			++index) {
		switch (index) {
			case 0:
				mxc_iomux_v3_setup_multiple_pads(usdhc2_pads,
						sizeof(usdhc2_pads) /
						sizeof(usdhc2_pads[0]));
				break;
			case 1:
				mxc_iomux_v3_setup_multiple_pads(usdhc3_pads,
						sizeof(usdhc3_pads) /
						sizeof(usdhc3_pads[0]));
				break;
			case 2:
				mxc_iomux_v3_setup_multiple_pads(usdhc4_pads,
						sizeof(usdhc4_pads) /
						sizeof(usdhc4_pads[0]));
				break;
			default:
				printf("Warning: you configured more USDHC controllers"
						"(%d) then supported by the board (%d)\n",
						index+1, CONFIG_SYS_FSL_USDHC_NUM);
				return status;
		}
		status |= fsl_esdhc_initialize(bis, &usdhc_cfg[index]);
	}

	return status;
}

int board_mmc_init(bd_t *bis)
{
	if (!usdhc_gpio_init(bis))
		return 0;
	else
		return -1;
}

/* For DDR mode operation, provide target delay parameter for each SD port.
 * Use cfg->esdhc_base to distinguish the SD port #. The delay for each port
 * is dependent on signal layout for that particular port.  If the following
 * CONFIG is not defined, then the default target delay value will be used.
 */
#ifdef CONFIG_GET_DDR_TARGET_DELAY
u32 get_ddr_delay(struct fsl_esdhc_cfg *cfg)
{
	/* No delay required */
	return 0;
}
#endif

#endif

#ifndef CONFIG_MXC_EPDC
#ifdef CONFIG_LCD
void lcd_enable(void)
{
	char *s;
	int ret;
	unsigned int reg;

	s = getenv("lvds_num");
	di = simple_strtol(s, NULL, 10);

	/*
	 * hw_rev 2: IPUV3DEX
	 * hw_rev 3: IPUV3M
	 * hw_rev 4: IPUV3H
	 */
	g_ipu_hw_rev = IPUV3_HW_REV_IPUV3H;

	imx_pwm_config(pwm0, 25000, 50000);
	imx_pwm_enable(pwm0);

#if defined CONFIG_MX6Q
	/* PWM backlight */
	mxc_iomux_v3_setup_pad(MX6Q_PAD_SD1_DAT3__PWM1_PWMO);
#elif defined CONFIG_MX6DL
	/* PWM backlight */
	mxc_iomux_v3_setup_pad(MX6DL_PAD_SD1_DAT3__PWM1_PWMO);
#endif
	/* Disable ipu1_clk/ipu1_di_clk_x/ldb_dix_clk. */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CCGR3);
	reg &= ~0xC033;
	writel(reg, CCM_BASE_ADDR + CLKCTL_CCGR3);

#if defined CONFIG_MX6Q
	/*
	 * Align IPU1 HSP clock and IPU1 DIx pixel clock
	 * with kernel setting to avoid screen flick when
	 * booting into kernel. Developer should change
	 * the relevant setting if kernel setting changes.
	 * IPU1 HSP clock tree:
	 * osc_clk(24M)->pll2_528_bus_main_clk(528M)->
	 * periph_clk(528M)->mmdc_ch0_axi_clk(528M)->
	 * ipu1_clk(264M)
	 */
	/* pll2_528_bus_main_clk */
	/* divider */
	writel(0x1, ANATOP_BASE_ADDR + 0x34);

	/* periph_clk */
	/* source */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CBCMR);
	reg &= ~(0x3 << 18);
	writel(reg, CCM_BASE_ADDR + CLKCTL_CBCMR);

	reg = readl(CCM_BASE_ADDR + CLKCTL_CBCDR);
	reg &= ~(0x1 << 25);
	writel(reg, CCM_BASE_ADDR + CLKCTL_CBCDR);

	/*
	 * Check PERIPH_CLK_SEL_BUSY in
	 * MXC_CCM_CDHIPR register.
	 */
	do {
		udelay(5);
		reg = readl(CCM_BASE_ADDR + CLKCTL_CDHIPR);
	} while (reg & (0x1 << 5));

	/* mmdc_ch0_axi_clk */
	/* divider */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CBCDR);
	reg &= ~(0x7 << 19);
	writel(reg, CCM_BASE_ADDR + CLKCTL_CBCDR);

	/*
	 * Check MMDC_CH0PODF_BUSY in
	 * MXC_CCM_CDHIPR register.
	 */
	do {
		udelay(5);
		reg = readl(CCM_BASE_ADDR + CLKCTL_CDHIPR);
	} while (reg & (0x1 << 4));

	/* ipu1_clk */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CSCDR3);
	/* source */
	reg &= ~(0x3 << 9);
	/* divider */
	reg &= ~(0x7 << 11);
	reg |= (0x1 << 11);
	writel(reg, CCM_BASE_ADDR + CLKCTL_CSCDR3);

	/*
	 * ipu1_pixel_clk_x clock tree:
	 * osc_clk(24M)->pll2_528_bus_main_clk(528M)->
	 * pll2_pfd_352M(452.57M)->ldb_dix_clk(64.65M)->
	 * ipu1_di_clk_x(64.65M)->ipu1_pixel_clk_x(64.65M)
	 */
	/* pll2_pfd_352M */
	/* disable */
	writel(0x1 << 7, ANATOP_BASE_ADDR + 0x104);
	/* divider */
	writel(0x3F, ANATOP_BASE_ADDR + 0x108);
	writel(0x15, ANATOP_BASE_ADDR + 0x104);

	/* ldb_dix_clk */
	/* source */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CS2CDR);
	reg &= ~(0x3F << 9);
	reg |= (0x9 << 9);
	writel(reg, CCM_BASE_ADDR + CLKCTL_CS2CDR);
	/* divider */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CSCMR2);
	reg |= (0x3 << 10);
	writel(reg, CCM_BASE_ADDR + CLKCTL_CSCMR2);

	/* pll2_pfd_352M */
	/* enable after ldb_dix_clk source is set */
	writel(0x1 << 7, ANATOP_BASE_ADDR + 0x108);

	/* ipu1_di_clk_x */
	/* source */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CHSCCDR);
	reg &= ~0xE07;
	reg |= 0x803;
	writel(reg, CCM_BASE_ADDR + CLKCTL_CHSCCDR);
#elif defined CONFIG_MX6DL /* CONFIG_MX6Q */
	/*
	 * IPU1 HSP clock tree:
	 * osc_clk(24M)->pll3_usb_otg_main_clk(480M)->
	 * pll3_pfd_540M(540M)->ipu1_clk(270M)
	 */
	/* pll3_usb_otg_main_clk */
	/* divider */
	writel(0x3, ANATOP_BASE_ADDR + 0x18);

	/* pll3_pfd_540M */
	/* divider */
	writel(0x3F << 8, ANATOP_BASE_ADDR + 0xF8);
	writel(0x10 << 8, ANATOP_BASE_ADDR + 0xF4);
	/* enable */
	writel(0x1 << 15, ANATOP_BASE_ADDR + 0xF8);

	/* ipu1_clk */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CSCDR3);
	/* source */
	reg |= (0x3 << 9);
	/* divider */
	reg &= ~(0x7 << 11);
	reg |= (0x1 << 11);
	writel(reg, CCM_BASE_ADDR + CLKCTL_CSCDR3);

	/*
	 * ipu1_pixel_clk_x clock tree:
	 * osc_clk(24M)->pll2_528_bus_main_clk(528M)->
	 * pll2_pfd_352M(452.57M)->ldb_dix_clk(64.65M)->
	 * ipu1_di_clk_x(64.65M)->ipu1_pixel_clk_x(64.65M)
	 */
	/* pll2_528_bus_main_clk */
	/* divider */
	writel(0x1, ANATOP_BASE_ADDR + 0x34);

	/* pll2_pfd_352M */
	/* disable */
	writel(0x1 << 7, ANATOP_BASE_ADDR + 0x104);
	/* divider */
	writel(0x3F, ANATOP_BASE_ADDR + 0x108);
	writel(0x15, ANATOP_BASE_ADDR + 0x104);

	/* ldb_dix_clk */
	/* source */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CS2CDR);
	reg &= ~(0x3F << 9);
	reg |= (0x9 << 9);
	writel(reg, CCM_BASE_ADDR + CLKCTL_CS2CDR);
	/* divider */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CSCMR2);
	reg |= (0x3 << 10);
	writel(reg, CCM_BASE_ADDR + CLKCTL_CSCMR2);

	/* pll2_pfd_352M */
	/* enable after ldb_dix_clk source is set */
	writel(0x1 << 7, ANATOP_BASE_ADDR + 0x108);

	/* ipu1_di_clk_x */
	/* source */
	reg = readl(CCM_BASE_ADDR + CLKCTL_CHSCCDR);
	reg &= ~0xE07;
	reg |= 0x803;
	writel(reg, CCM_BASE_ADDR + CLKCTL_CHSCCDR);
#endif	/* CONFIG_MX6DL */

	/* Enable ipu1/ipu1_dix/ldb_dix clocks. */
	if (di == 1) {
		reg = readl(CCM_BASE_ADDR + CLKCTL_CCGR3);
		reg |= 0xC033;
		writel(reg, CCM_BASE_ADDR + CLKCTL_CCGR3);
	} else {
		reg = readl(CCM_BASE_ADDR + CLKCTL_CCGR3);
		reg |= 0x300F;
		writel(reg, CCM_BASE_ADDR + CLKCTL_CCGR3);
	}

	ret = ipuv3_fb_init(&lvds_xga, di, IPU_PIX_FMT_RGB666,
			DI_PCLK_LDB, 65000000);
	if (ret)
		puts("LCD cannot be configured\n");

	/*
	 * LVDS0 mux to IPU1 DI0.
	 * LVDS1 mux to IPU1 DI1.
	 */
	reg = readl(IOMUXC_BASE_ADDR + 0xC);
	reg &= ~(0x000003C0);
	reg |= 0x00000100;
	writel(reg, IOMUXC_BASE_ADDR + 0xC);

	if (di == 1)
		writel(0x40C, IOMUXC_BASE_ADDR + 0x8);
	else
		writel(0x201, IOMUXC_BASE_ADDR + 0x8);
}
#endif

#ifdef CONFIG_VIDEO_MX5
void panel_info_init(void)
{
	panel_info.vl_bpix = LCD_BPP;
	panel_info.vl_col = lvds_xga.xres;
	panel_info.vl_row = lvds_xga.yres;
	panel_info.cmap = colormap;
}
#endif

#ifdef CONFIG_SPLASH_SCREEN
void setup_splash_image(void)
{
	char *s;
	ulong addr;

	s = getenv("splashimage");

	if (s != NULL) {
		addr = simple_strtoul(s, NULL, 16);

		memcpy((char *)addr, (char *)myzr_bmp_logo,
				myzr_bmp_logo_size);
	}
}
#endif
#endif /* !CONFIG_MXC_EPDC */

int board_init(void)
{
	/* need set Power Supply Glitch to 0x41736166
	 *and need clear Power supply Glitch Detect bit
	 * when POR or reboot or power on Otherwise system
	 *could not be power off anymore*/
	u32 reg;
	writel(0x41736166, SNVS_BASE_ADDR + 0x64);/*set LPPGDR*/
	udelay(10);
	reg = readl(SNVS_BASE_ADDR + 0x4c);
	reg |= (1 << 3);
	writel(reg, SNVS_BASE_ADDR + 0x4c);/*clear LPSR*/

	mxc_iomux_v3_init((void *)IOMUXC_BASE_ADDR);
	setup_boot_device();
	fsl_set_system_rev();

	/* board id for linux */
#if defined(CONFIG_MYIMX6EK200_6Q) || defined(CONFIG_MYIMX6EK200_6U)
	gd->bd->bi_arch_number = MACH_TYPE_MYIMX6EK200_6Q;
#else
	gd->bd->bi_arch_number = MACH_TYPE_MYIMX6EK314_6Q;
#endif

	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	setup_uart();

#ifdef CONFIG_VIDEO_MX5
	panel_info_init();

	gd->fb_base = CONFIG_FB_BASE;
#endif

#if defined(CONFIG_ENET_RMII)
	setup_fec();
#endif

	return 0;
}

int board_late_init(void)
{
#ifdef CONFIG_I2C_MXC
	setup_i2c(CONFIG_SYS_I2C_PORT);
#endif
	return 0;
}

#ifdef CONFIG_MXC_FEC
static int phy_read(char *devname, unsigned char addr, unsigned char reg,
		unsigned short *pdata)
{
	int ret = miiphy_read(devname, addr, reg, pdata);
	if (ret)
		printf("Error reading from %s PHY addr=%02x reg=%02x\n",
				devname, addr, reg);
	return ret;
}

static int phy_write(char *devname, unsigned char addr, unsigned char reg,
		unsigned short value)
{
	int ret = miiphy_write(devname, addr, reg, value);
	if (ret)
		printf("Error writing to %s PHY addr=%02x reg=%02x\n", devname,
				addr, reg);
	return ret;
}

#if !defined(CONFIG_ENET_RMII)
int mx6_rgmii_rework(char *devname, int phy_addr)
{
	/* enable master mode, force phy to 100Mbps */
	phy_write(devname, phy_addr, 0x9, 0x1c00);

	/* min rx data delay */
	phy_write(devname, phy_addr, 0x0b, 0x8105);
	phy_write(devname, phy_addr, 0x0c, 0x0000);

	/* max rx/tx clock delay, min rx/tx control delay */
	phy_write(devname, phy_addr, 0x0b, 0x8104);
	phy_write(devname, phy_addr, 0x0c, 0xf0f0);
	phy_write(devname, phy_addr, 0x0b, 0x104);
	return 0;
}
#endif

#ifdef CONFIG_MX6Q
iomux_v3_cfg_t enet_pads[] = {
	MX6Q_PAD_ENET_MDIO__ENET_MDIO,
	MX6Q_PAD_ENET_MDC__ENET_MDC,
#if defined(CONFIG_ENET_RMII)
	/* ENET (10/100Mbps enet interface)*/
	MX6Q_PAD_ENET_CRS_DV__ENET_RX_EN,
	MX6Q_PAD_ENET_RX_ER__ENET_RX_ER,
	MX6Q_PAD_ENET_TX_EN__ENET_TX_EN,
	MX6Q_PAD_ENET_RXD0__ENET_RDATA_0,
	MX6Q_PAD_ENET_RXD1__ENET_RDATA_1,
	MX6Q_PAD_ENET_TXD0__ENET_TDATA_0,
	MX6Q_PAD_ENET_TXD1__ENET_TDATA_1,	
	MX6Q_PAD_GPIO_16__ENET_ANATOP_ETHERNET_REF_OUT,
	MX6Q_PAD_ENET_REF_CLK__GPIO_1_23, /* reset phy */
#else
	MX6Q_PAD_RGMII_TXC__ENET_RGMII_TXC,
	MX6Q_PAD_RGMII_TD0__ENET_RGMII_TD0,
	MX6Q_PAD_RGMII_TD1__ENET_RGMII_TD1,
	MX6Q_PAD_RGMII_TD2__ENET_RGMII_TD2,
	MX6Q_PAD_RGMII_TD3__ENET_RGMII_TD3,
	MX6Q_PAD_RGMII_TX_CTL__ENET_RGMII_TX_CTL,
	/* pin 35 - 1 (PHY_AD2) on reset */
	MX6Q_PAD_RGMII_RXC__GPIO_6_30,
	/* pin 32 - 1 - (MODE0) all */
	MX6Q_PAD_RGMII_RD0__GPIO_6_25,
	/* pin 31 - 1 - (MODE1) all */
	MX6Q_PAD_RGMII_RD1__GPIO_6_27,
	/* pin 28 - 1 - (MODE2) all */
	MX6Q_PAD_RGMII_RD2__GPIO_6_28,
	/* pin 27 - 1 - (MODE3) all */
	MX6Q_PAD_RGMII_RD3__GPIO_6_29,
	/* pin 33 - 1 - (CLK125_EN) 125Mhz clockout enabled */
	MX6Q_PAD_RGMII_RX_CTL__GPIO_6_24,
	MX6Q_PAD_GPIO_0__CCM_CLKO,
	MX6Q_PAD_GPIO_3__CCM_CLKO2,
	MX6Q_PAD_ENET_REF_CLK__ENET_TX_CLK,
#endif
};

#if !defined(CONFIG_ENET_RMII)
iomux_v3_cfg_t enet_pads_final[] = {
	MX6Q_PAD_RGMII_RXC__ENET_RGMII_RXC,
	MX6Q_PAD_RGMII_RD0__ENET_RGMII_RD0,
	MX6Q_PAD_RGMII_RD1__ENET_RGMII_RD1,
	MX6Q_PAD_RGMII_RD2__ENET_RGMII_RD2,
	MX6Q_PAD_RGMII_RD3__ENET_RGMII_RD3,
	MX6Q_PAD_RGMII_RX_CTL__ENET_RGMII_RX_CTL,
};
#endif
#elif defined CONFIG_MX6DL
iomux_v3_cfg_t enet_pads[] = {
	MX6DL_PAD_ENET_MDIO__ENET_MDIO,
	MX6DL_PAD_ENET_MDC__ENET_MDC,
#ifdef CONFIG_ENET_RMII
	/* ENET (10/100Mbps enet interface)*/
	MX6DL_PAD_ENET_CRS_DV__ENET_RX_EN,
	MX6DL_PAD_ENET_RX_ER__ENET_RX_ER,
	MX6DL_PAD_ENET_TX_EN__ENET_TX_EN,
	MX6DL_PAD_ENET_RXD0__ENET_RDATA_0,
	MX6DL_PAD_ENET_RXD1__ENET_RDATA_1,
	MX6DL_PAD_ENET_TXD0__ENET_TDATA_0,
	MX6DL_PAD_ENET_TXD1__ENET_TDATA_1,	
	MX6DL_PAD_GPIO_16__ENET_ANATOP_ETHERNET_REF_OUT,
	MX6DL_PAD_ENET_REF_CLK__GPIO_1_23, /* reset phy */
#else
	MX6DL_PAD_RGMII_TXC__ENET_RGMII_TXC,
	MX6DL_PAD_RGMII_TD0__ENET_RGMII_TD0,
	MX6DL_PAD_RGMII_TD1__ENET_RGMII_TD1,
	MX6DL_PAD_RGMII_TD2__ENET_RGMII_TD2,
	MX6DL_PAD_RGMII_TD3__ENET_RGMII_TD3,
	MX6DL_PAD_RGMII_TX_CTL__ENET_RGMII_TX_CTL,
	/* pin 35 - 1 (PHY_AD2) on reset */
	MX6DL_PAD_RGMII_RXC__GPIO_6_30,
	/* pin 32 - 1 - (MODE0) all */
	MX6DL_PAD_RGMII_RD0__GPIO_6_25,
	/* pin 31 - 1 - (MODE1) all */
	MX6DL_PAD_RGMII_RD1__GPIO_6_27,
	/* pin 28 - 1 - (MODE2) all */
	MX6DL_PAD_RGMII_RD2__GPIO_6_28,
	/* pin 27 - 1 - (MODE3) all */
	MX6DL_PAD_RGMII_RD3__GPIO_6_29,
	/* pin 33 - 1 - (CLK125_EN) 125Mhz clockout enabled */
	MX6DL_PAD_RGMII_RX_CTL__GPIO_6_24,
	MX6DL_PAD_GPIO_0__CCM_CLKO,
	MX6DL_PAD_GPIO_3__CCM_CLKO2,
	MX6DL_PAD_ENET_REF_CLK__ENET_TX_CLK,
#endif
};

#if !defined(CONFIG_ENET_RMII)
iomux_v3_cfg_t enet_pads_final[] = {
	MX6DL_PAD_RGMII_RXC__ENET_RGMII_RXC,
	MX6DL_PAD_RGMII_RD0__ENET_RGMII_RD0,
	MX6DL_PAD_RGMII_RD1__ENET_RGMII_RD1,
	MX6DL_PAD_RGMII_RD2__ENET_RGMII_RD2,
	MX6DL_PAD_RGMII_RD3__ENET_RGMII_RD3,
	MX6DL_PAD_RGMII_RX_CTL__ENET_RGMII_RX_CTL,
};
#endif
#endif

void enet_board_init(void)
{
	mxc_iomux_v3_setup_multiple_pads(enet_pads,
			ARRAY_SIZE(enet_pads));

#ifdef CONFIG_ENET_RMII
	gpio_direction_output(FEC_PHY_RESET, 0);
	udelay(5000);
	gpio_set_value(FEC_PHY_RESET, 1);
	/* Set GPIO_16 output for RMII reference clock
	 * For MX6 GPR1 bit21 meaning:
	 * Bit21:       1 - GPIO_16 pad output
	 *              0 - GPIO_16 pad input
	 * Use interal clock, set GPIO_16 output, and
	 * then force input(set SION)
	 * Use exteral clock, set GPIO_16 input, and
	 * don't set SION bit.
	 */
	mxc_iomux_set_gpr_register(1, 21, 1, 1);
#else
	mxc_iomux_v3_setup_pad(MX6X_IOMUX(PAD_ENET_CRS_DV__GPIO_1_25));

	gpio_direction_output(FEC_PHY_RESET, 0);
	udelay(500);
	gpio_direction_output(FEC_PHY_RESET, 1);

	mxc_iomux_v3_setup_multiple_pads(enet_pads_final,
			ARRAY_SIZE(enet_pads_final));
#endif
}
#endif

int checkboard(void)
{
#if defined(CONFIG_MYIMX6EK200_6Q) || defined(CONFIG_MYIMX6EK200_6U)
	printf("Board: MYIMX6EK200, %s.\n",	mx6_chip_name());
#else
	printf("Board: MYIMX6EK314, %s.\n",	mx6_chip_name());
#endif

	printf("Boot Device: ");
	switch (get_boot_device()) {
		case WEIM_NOR_BOOT:
			printf("NOR\n");
			break;
		case ONE_NAND_BOOT:
			printf("ONE NAND\n");
			break;
		case PATA_BOOT:
			printf("PATA\n");
			break;
		case SATA_BOOT:
			printf("SATA\n");
			break;
		case I2C_BOOT:
			printf("I2C\n");
			break;
		case SPI_NOR_BOOT:
			printf("SPI NOR\n");
			break;
		case SD_BOOT:
			printf("SD\n");
			break;
		case MMC_BOOT:
			printf("MMC\n");
			break;
		case NAND_BOOT:
			printf("NAND\n");
			break;
		case UNKNOWN_BOOT:
		default:
			printf("UNKNOWN\n");
			break;
	}

	return 0;
}
