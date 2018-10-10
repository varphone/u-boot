/*
 * Tang Bin <tangb@myzr.com.cn>
 */

#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <asm/errno.h>
#include <asm/gpio.h>
#include <asm/imx-common/mxc_i2c.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/video.h>
#include <malloc.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <miiphy.h>
#include <netdev.h>
#include <micrel.h>

#include <asm/arch/mxc_hdmi.h>
#include <asm/arch/crm_regs.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <i2c.h>
#include <ipu_pixfmt.h>
#include <linux/fb.h>
#include <asm/arch/mx6-ddr.h>
#include <usb.h>

#ifdef CONFIG_CMD_SATA
#include <asm/imx-common/sata.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL  (PAD_CTL_PUS_100K_UP |			\
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm |			\
	PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define USDHC_PAD_CTRL (PAD_CTL_PUS_47K_UP |			\
	PAD_CTL_SPEED_LOW | PAD_CTL_DSE_80ohm |			\
	PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define ENET_PAD_CTRL  (PAD_CTL_PUS_100K_UP |			\
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS)

#define SPI_PAD_CTRL (PAD_CTL_HYS | PAD_CTL_SPEED_MED | \
		      PAD_CTL_DSE_40ohm | PAD_CTL_SRE_FAST)

#define I2C_PAD_CTRL  (PAD_CTL_PUS_100K_UP |			\
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS |	\
	PAD_CTL_ODE | PAD_CTL_SRE_FAST)

#define OTG_ID_PAD_CTRL (PAD_CTL_PKE | PAD_CTL_PUE |		\
	PAD_CTL_PUS_47K_UP  | PAD_CTL_SPEED_LOW |		\
	PAD_CTL_DSE_80ohm   | PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define I2C_PAD MUX_PAD_CTRL(I2C_PAD_CTRL)

int dram_init(void)
{
#if ((defined(CONFIG_MYIMX6CB336) || defined(CONFIG_TARGET_MYIMX6LEF)) \
	&& !defined(CONFIG_DBG_DDR))
	gd->ram_size = imx_ddr_size() << 1;
#else
	gd->ram_size = imx_ddr_size();
#endif
	return 0;
}

#if defined(CONFIG_MYIMX6CB336)
#define PMU_PWR_CTRL	IMX_GPIO_NR(7, 12)
#define PMU_INT_PMU		IMX_GPIO_NR(7, 13)

static iomux_v3_cfg_t const pmu_io[] = {
	MX6_PAD_GPIO_17__GPIO7_IO12 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_GPIO_18__GPIO7_IO13 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

int pmu_init(void)
{
	imx_iomux_v3_setup_multiple_pads(pmu_io, ARRAY_SIZE(pmu_io));
	
	gpio_direction_output(PMU_PWR_CTRL, 1);
	gpio_direction_output(PMU_INT_PMU, 1);

	return 0;
}
#endif

/* UART ********************************************************************* */
static iomux_v3_cfg_t const dbg_uart[] = {
#if (defined(CONFIG_TARGET_MYIMX6EK200) || defined(CONFIG_TARGET_MYIMX6EK314) \
	|| defined(CONFIG_TARGET_MYIMX6EK336) \
	|| defined(CONFIG_TARGET_MYIMX6ACO) || defined(CONFIG_TARGET_MYIMX6QJH))
	MX6_PAD_CSI0_DAT10__UART1_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_CSI0_DAT11__UART1_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
#elif defined(CONFIG_TARGET_MYIMX6LEF)
	MX6_PAD_SD3_DAT6__UART1_RX_DATA	| MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_SD3_DAT7__UART1_TX_DATA	| MUX_PAD_CTRL(UART_PAD_CTRL),
#endif
};

static void setup_iomux_uart(void)
{
	imx_iomux_v3_setup_multiple_pads(dbg_uart, ARRAY_SIZE(dbg_uart));
}

/* SPI ********************************************************************** */
#define ECSPI1_CS0	IMX_GPIO_NR(2, 30)
#define ECSPI1_CS1	IMX_GPIO_NR(3, 19)

static iomux_v3_cfg_t const ecspi1_pads[] = {
	MX6_PAD_EIM_D16__ECSPI1_SCLK	| MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_D17__ECSPI1_MISO	| MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_D18__ECSPI1_MOSI	| MUX_PAD_CTRL(SPI_PAD_CTRL),
#if !defined(CONFIG_TARGET_MYIMX6LEF)
	MX6_PAD_EIM_EB2__GPIO2_IO30		| MUX_PAD_CTRL(NO_PAD_CTRL),
#endif
	MX6_PAD_EIM_D19__GPIO3_IO19		| MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_spi(void)
{
	imx_iomux_v3_setup_multiple_pads(ecspi1_pads, ARRAY_SIZE(ecspi1_pads));
}

int board_spi_cs_gpio(unsigned bus, unsigned cs)
{
	int ret = -1;
	
	if (bus == 0 && cs == 1)
		ret = ECSPI1_CS1;
#if !defined(CONFIG_TARGET_MYIMX6LEF)
	else if (bus == 0 && cs == 0)
		ret = ECSPI1_CS0;
#endif
	
	return ret;
}

/* ENET ********************************************************************* */
static iomux_v3_cfg_t const enet_pads[] = {
#if defined(CONFIG_TARGET_MYIMX6EK200)	/* CONFIG_TARGET_MYIMX6EK200 */
	MX6_PAD_ENET_MDIO__ENET_MDIO		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_MDC__ENET_MDC		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_GPIO_16__ENET_REF_CLK		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_TXD0__ENET_TX_DATA0	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_TXD1__ENET_TX_DATA1	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_TX_EN__ENET_TX_EN		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_RXD0__ENET_RX_DATA0	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_RXD1__ENET_RX_DATA1	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_CRS_DV__ENET_RX_EN		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_RX_ER__ENET_RX_ER		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_GPIO_18__GPIO7_IO13			| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_ENET_REF_CLK__GPIO1_IO23 	| MUX_PAD_CTRL(NO_PAD_CTRL),
#elif (defined(CONFIG_TARGET_MYIMX6EK314) || defined(CONFIG_TARGET_MYIMX6EK336) \
	|| defined(CONFIG_TARGET_MYIMX6ACO) || defined(CONFIG_TARGET_MYIMX6QJH))
	MX6_PAD_ENET_MDIO__ENET_MDIO		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_MDC__ENET_MDC		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TXC__RGMII_TXC	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD0__RGMII_TD0	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD1__RGMII_TD1	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD2__RGMII_TD2	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD3__RGMII_TD3	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TX_CTL__RGMII_TX_CTL	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_REF_CLK__ENET_TX_CLK	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RXC__RGMII_RXC	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD0__GPIO6_IO25		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_RGMII_RD1__GPIO6_IO27		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_RGMII_RD2__GPIO6_IO28		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_RGMII_RD3__GPIO6_IO29		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_RGMII_RX_CTL__GPIO6_IO24	| MUX_PAD_CTRL(NO_PAD_CTRL),
	/* KSZ9031 Reset */
	MX6_PAD_ENET_CRS_DV__GPIO1_IO25		| MUX_PAD_CTRL(NO_PAD_CTRL),
#endif	/* End: CONFIG_TARGET_MYIMX6EK200 */
};

#if (defined(CONFIG_TARGET_MYIMX6EK314) || defined(CONFIG_TARGET_MYIMX6EK336) \
	|| defined(CONFIG_TARGET_MYIMX6ACO) || defined(CONFIG_TARGET_MYIMX6QJH))
static iomux_v3_cfg_t const enet_pads2[] = {
	MX6_PAD_RGMII_RD0__RGMII_RD0	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD1__RGMII_RD1	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD2__RGMII_RD2	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD3__RGMII_RD3	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RX_CTL__RGMII_RX_CTL	| MUX_PAD_CTRL(ENET_PAD_CTRL),
};
#endif

static void setup_iomux_enet(void)
{
	imx_iomux_v3_setup_multiple_pads(enet_pads, ARRAY_SIZE(enet_pads));

#if defined(CONFIG_TARGET_MYIMX6EK200)
	gpio_direction_output(IMX_GPIO_NR(1, 23) , 0);
	udelay(500);
	gpio_set_value(IMX_GPIO_NR(1, 23), 1);
	imx_iomux_set_gpr_register(1, 21, 1, 1);
#elif (defined(CONFIG_TARGET_MYIMX6EK314) || defined(CONFIG_TARGET_MYIMX6EK336) \
	|| defined(CONFIG_TARGET_MYIMX6ACO) || defined(CONFIG_TARGET_MYIMX6QJH))
	gpio_direction_output(IMX_GPIO_NR(1, 25) , 0);
	gpio_direction_output(IMX_GPIO_NR(6, 24), 1);
	gpio_direction_output(IMX_GPIO_NR(6, 25), 1);
	gpio_direction_output(IMX_GPIO_NR(6, 27), 1);
	gpio_direction_output(IMX_GPIO_NR(6, 28), 1);
	gpio_direction_output(IMX_GPIO_NR(6, 29), 1);
	udelay(1000);
	gpio_set_value(IMX_GPIO_NR(1, 25), 1);
	
	udelay(1000 * 100);
	gpio_free(IMX_GPIO_NR(6, 24));
	gpio_free(IMX_GPIO_NR(6, 25));
	gpio_free(IMX_GPIO_NR(6, 27));
	gpio_free(IMX_GPIO_NR(6, 28));
	gpio_free(IMX_GPIO_NR(6, 29));

	imx_iomux_v3_setup_multiple_pads(enet_pads2, ARRAY_SIZE(enet_pads2));
#endif
}

#if (CONFIG_FEC_XCV_TYPE == RGMII)	/* CONFIG_FEC_XCV_TYPE */
int mx6_rgmii_rework(struct phy_device *phydev)
{
	phy_write(phydev, MDIO_DEVAD_NONE, MII_CTRL1000, 0x1c00);
	
#if defined(CONFIG_PHY_MICREL_KSZ9031)
	ksz9031_phy_extended_write(phydev, 0x02,
				   MII_KSZ9031_EXT_RGMII_CTRL_SIG_SKEW,
				   MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x0000);
	ksz9031_phy_extended_write(phydev, 0x02,
				   MII_KSZ9031_EXT_RGMII_RX_DATA_SKEW,
				   MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x0000);
	ksz9031_phy_extended_write(phydev, 0x02,
				   MII_KSZ9031_EXT_RGMII_TX_DATA_SKEW,
				   MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x0000);
	ksz9031_phy_extended_write(phydev, 0x02,
				   MII_KSZ9031_EXT_RGMII_CLOCK_SKEW,
				   MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x03FF);
#endif

	return 0;
}
#endif	/* End: CONFIG_FEC_XCV_TYPE */

int board_phy_config(struct phy_device *phydev)
{
#if (CONFIG_FEC_XCV_TYPE == RGMII)
	mx6_rgmii_rework(phydev);
#endif

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}

#if (defined(CONFIG_TARGET_MYIMX6EK314) || defined(CONFIG_TARGET_MYIMX6EK336) \
	|| defined(CONFIG_TARGET_MYIMX6ACO) || defined(CONFIG_TARGET_MYIMX6QJH))
int board_eth_init(bd_t *bis)
{
	uint32_t base = IMX_FEC_BASE;
	struct mii_dev *bus = NULL;
	struct phy_device *phydev = NULL;
	int ret;

	setup_iomux_enet();

#ifdef CONFIG_FEC_MXC
	bus = fec_get_miibus(base, -1);
	if (!bus)
		return 0;

	phydev = phy_find_by_mask(bus, (0xf << 4), PHY_INTERFACE_MODE_RGMII);

	if (!phydev) {
		free(bus);
		return 0;
	}
	
	printf("using phy at %d\n", phydev->addr);
	ret  = fec_probe(bis, -1, base, bus, phydev);
	if (ret) {
		printf("FEC MXC: %s:failed\n", __func__);
		free(phydev);
		free(bus);
	}
#endif

	return 0;
}
#elif defined(CONFIG_TARGET_MYIMX6EK200)
int board_eth_init(bd_t *bis)
{
	int ret;

	setup_iomux_enet();
	
#ifdef CONFIG_MX6QP
	imx_iomux_set_gpr_register(5, 9, 1, 1);
#else
	imx_iomux_set_gpr_register(1, 21, 1, 1);
#endif

	ret = enable_fec_anatop_clock(CONFIG_FEC_ENET_DEV, ENET_50MHZ);
	if (ret)
		return ret;

	return cpu_eth_init(bis);
}
#endif

/* DISPLAY ****************************************************************** */
#if !defined(CONFIG_TARGET_MYIMX6QJH)
#define DISP0_PWM	IMX_GPIO_NR(1, 21)
#if defined(CONFIG_TARGET_MYIMX6LEF)
#define DISP0_RST	IMX_GPIO_NR(4, 20)
#endif

static iomux_v3_cfg_t const rgb_pads[] = {
	MX6_PAD_DI0_DISP_CLK__IPU1_DI0_DISP_CLK | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DI0_PIN15__IPU1_DI0_PIN15 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DI0_PIN2__IPU1_DI0_PIN02 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DI0_PIN3__IPU1_DI0_PIN03 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT0__IPU1_DISP0_DATA00 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT1__IPU1_DISP0_DATA01 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT2__IPU1_DISP0_DATA02 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT3__IPU1_DISP0_DATA03 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT4__IPU1_DISP0_DATA04 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT5__IPU1_DISP0_DATA05 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT6__IPU1_DISP0_DATA06 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT7__IPU1_DISP0_DATA07 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT8__IPU1_DISP0_DATA08 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT9__IPU1_DISP0_DATA09 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT10__IPU1_DISP0_DATA10 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT11__IPU1_DISP0_DATA11 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT12__IPU1_DISP0_DATA12 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT13__IPU1_DISP0_DATA13 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT14__IPU1_DISP0_DATA14 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT15__IPU1_DISP0_DATA15 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT16__IPU1_DISP0_DATA16 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT17__IPU1_DISP0_DATA17 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT18__IPU1_DISP0_DATA18 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT19__IPU1_DISP0_DATA19 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT20__IPU1_DISP0_DATA20 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT21__IPU1_DISP0_DATA21 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT22__IPU1_DISP0_DATA22 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_DISP0_DAT23__IPU1_DISP0_DATA23 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD1_DAT3__GPIO1_IO21 | MUX_PAD_CTRL(NO_PAD_CTRL),
#if defined(CONFIG_TARGET_MYIMX6LEF)
	/* Reset */
	MX6_PAD_DI0_PIN4__GPIO4_IO20 | MUX_PAD_CTRL(NO_PAD_CTRL),
#endif
};

iomux_v3_cfg_t const di0_pads[] = {
	MX6_PAD_DI0_DISP_CLK__IPU1_DI0_DISP_CLK,	/* DISP0_CLK */
	MX6_PAD_DI0_PIN2__IPU1_DI0_PIN02,		/* DISP0_HSYNC */
	MX6_PAD_DI0_PIN3__IPU1_DI0_PIN03,		/* DISP0_VSYNC */
};

static void enable_rgb(struct display_info_t const *dev)
{
	imx_iomux_v3_setup_multiple_pads(rgb_pads, ARRAY_SIZE(rgb_pads));
#if defined(CONFIG_TARGET_MYIMX6LEF)
	gpio_direction_output(DISP0_RST, 1);
#endif
	gpio_direction_output(DISP0_PWM, 1);
}
#endif		/* CONFIG_TARGET_MYIMX6QJH */

#if defined(CONFIG_VIDEO_IPUV3)
#if !defined(CONFIG_TARGET_MYIMX6QJH)
static void disable_lvds(struct display_info_t const *dev)
{
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;

	int reg = readl(&iomux->gpr[2]);

	reg &= ~(IOMUXC_GPR2_LVDS_CH0_MODE_MASK |
		 IOMUXC_GPR2_LVDS_CH1_MODE_MASK);

	writel(reg, &iomux->gpr[2]);
}
#endif

static void do_enable_hdmi(struct display_info_t const *dev)
{
#if !defined(CONFIG_TARGET_MYIMX6QJH)
	disable_lvds(dev);
#endif
	imx_enable_hdmi_phy();
}

#if !defined(CONFIG_TARGET_MYIMX6QJH)
static void enable_lvds(struct display_info_t const *dev)
{
	struct iomuxc *iomux = (struct iomuxc *)
				IOMUXC_BASE_ADDR;
	u32 reg = readl(&iomux->gpr[2]);
	reg |= IOMUXC_GPR2_DATA_WIDTH_CH0_18BIT |
	       IOMUXC_GPR2_DATA_WIDTH_CH1_18BIT;
	writel(reg, &iomux->gpr[2]);
}
#endif

#define MTY065_I2C_BUS	0
#define MTY065_I2C_ADDR	0x1b
static void enable_mty065(struct display_info_t const* dev)
{
	uint8_t buf[64];
	unsigned int old_bus;

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(dev->bus);

	/* Freeze */
	buf[0] = 1;
	i2c_write(dev->addr, 0x1a, 1, buf, 1);

	/* Set image curtain */
	buf[0] = 0x00;
	i2c_write(dev->addr, 0x16, 1, buf, 1);

	/* Set image crop */
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;
	buf[4] = dev->mode.xres & 0xff;
	buf[5] = (dev->mode.xres & 0xff00) >> 8;
	buf[6] = dev->mode.yres & 0xff;
	buf[7] = (dev->mode.yres & 0xff00) >> 8;
	i2c_write(dev->addr, 0x10, 1, buf, 8);

	/* Set input size */
	buf[0] = dev->mode.xres & 0xff;
	buf[1] = (dev->mode.xres & 0xff00) >> 8;
	buf[2] = dev->mode.yres & 0xff;
	buf[3] = (dev->mode.yres & 0xff00) >> 8;
	i2c_write(dev->addr, 0x2e, 1, buf, 4);

	/*
	 * Set display size
	 * must be after the image crop
	 */
	buf[0] = 854 & 0xff;
	buf[1] = (854 & 0xff00) >> 8;
	buf[2] = 480 & 0xff;
	buf[3] = (480 & 0xff00) >> 8;
	buf[4] = dev->mode.xres & 0xff;
	buf[5] = (dev->mode.xres & 0xff00) >> 8;
	buf[6] = dev->mode.yres & 0xff;
	buf[7] = (dev->mode.yres & 0xff00) >> 8;
	i2c_write(dev->addr, 0x12, 1, buf, 8);

	/* Set external video source format */
	buf[0] = 0x41; /* RGB666, 18 bits */
	i2c_write(dev->addr, 0x07, 1, buf, 1);

	/*
	 * Set input source select
	 * must be last to apply above settings
	 */
	buf[0] = 0x00; /* external */
	i2c_write(dev->addr, 0x05, 1, buf, 1);

	/* Clear test pattern */
	buf[0] = 0x00;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0x00;
	buf[5] = 0x00;
	i2c_write(dev->addr, 0x0b, 1, buf, 6);

	/* Unfreeze */
	buf[0] = 0x00;
	i2c_write(dev->addr, 0x1a, 1, buf, 1);

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);

	enable_lvds(dev);
}

/*
 * Detect MTY065X Military Projector
 * return 1 if found else 0
 */
static int detect_mty065(struct display_info_t const *dev)
{
	int ret;
	unsigned int old_bus;

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(dev->bus);

	ret = i2c_probe(dev->addr) == 0 ? 1 : 0;

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);

	return ret;
}

#define ATM88PA_I2C_BUS		0
#define ATM88PA_I2C_ADDR	0x7a
static void enable_nlb084sv01l(struct display_info_t const* dev)
{
	uint8_t buf[64];
	unsigned int old_bus;

	/* Enable video output first */
	enable_lvds(dev);

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(dev->bus);

	/* LCD_PWR */
	buf[0] = 1; /* Power on */
	i2c_write(dev->addr, 0x0b, 1, buf, 1);

	/* LCD_LIGHT */
	buf[0] = 255; /* Max brightness */
	i2c_write(dev->addr, 0x0e, 1, buf, 1);

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);
}

#define ADV739X_I2C_BUS     0
#define ADV739X_I2C_ADDR    0x2b
static int detect_adv739x_tvout(struct display_info_t const *dev)
{
	int ret;
	unsigned int old_bus;

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(dev->bus);

	ret = i2c_probe(dev->addr) == 0 ? 1 : 0;

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);

	return ret;
}

static void adv739x_write(uint8_t sa, uint8_t reg, uint8_t val)
{
	i2c_write(sa, reg, 1, &val, 1);
}

static void enable_adv739x_tvout(struct display_info_t const* dev)
{
	unsigned int old_bus;

	/* Enable video output first */
	enable_rgb(dev);

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(dev->bus);

	// Reg 0x17: reset
	adv739x_write(dev->addr, 0x17, 0x02);
	mdelay(20);
	// Reg 0x00: DAC1~3 power on
	adv739x_write(dev->addr, 0x00, 0x1C);
	// Reg 0x01: SD input
	adv739x_write(dev->addr, 0x01, 0x00);
	// Reg 0x80: SD, PAL
	adv739x_write(dev->addr, 0x80, 0x11);
	// Reg 0x82: SD, CVBS
	adv739x_write(dev->addr, 0x82, 0xC3);
	adv739x_write(dev->addr, 0x8C, 0xCB);
	adv739x_write(dev->addr, 0x8D, 0x8A);
	adv739x_write(dev->addr, 0x8E, 0x09);
	adv739x_write(dev->addr, 0x8F, 0x2A);
	// Reg 0x84: Color Bar
	adv739x_write(dev->addr, 0x84, 0x40);

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);
}

struct display_info_t const displays[] = {{
#if !defined(CONFIG_TARGET_MYIMX6QJH)
	.bus	= -1,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB666,
	.detect	= NULL,
	.enable	= enable_lvds,
	.mode	= {
		.name           = "Hannstar-XGA",
		.refresh        = 60,
		.xres           = 1024,
		.yres           = 600,
		.pixclock       = 15385,
		.left_margin    = 220,
		.right_margin   = 40,
		.upper_margin   = 21,
		.lower_margin   = 7,
		.hsync_len      = 60,
		.vsync_len      = 10,
		.sync           = FB_SYNC_EXT,
		.vmode          = FB_VMODE_NONINTERLACED
} }, {
#endif
	.bus	= -1,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB24,
	.detect	= NULL,
	.enable	= do_enable_hdmi,
	.mode	= {
		.name           = "HDMI",
		.refresh        = 60,
		.xres           = 640,
		.yres           = 480,
		.pixclock       = 39721,
		.left_margin    = 48,
		.right_margin   = 16,
		.upper_margin   = 33,
		.lower_margin   = 10,
		.hsync_len      = 96,
		.vsync_len      = 2,
		.sync           = 0,
		.vmode          = FB_VMODE_NONINTERLACED
#if !defined(CONFIG_TARGET_MYIMX6QJH)
} }, {
	.bus	= 0,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB24,
	.detect	= NULL,
	.enable	= enable_rgb,
	.mode	= {
		.name           = "SEIKO-WVGA",
		.refresh        = 60,
		.xres           = 800,
		.yres           = 480,
		.pixclock       = 29850,
		.left_margin    = 89,
		.right_margin   = 164,
		.upper_margin   = 23,
		.lower_margin   = 10,
		.hsync_len      = 10,
		.vsync_len      = 10,
		.sync           = 0,
		.vmode          = FB_VMODE_NONINTERLACED
#endif
} }, {
	.bus	= MTY065_I2C_BUS,
	.addr	= MTY065_I2C_ADDR,
	.pixfmt	= IPU_PIX_FMT_RGB666,
	.detect	= detect_mty065,
	.enable	= enable_mty065,
	.mode	= {
		.name           = "MTY065-720P",
		.refresh        = 60,
		.xres           = 1280,
		.yres           = 720,
		.pixclock       = 15619,
		.left_margin    = 80,
		.right_margin   = 48,
		.upper_margin   = 13,
		.lower_margin   = 3,
		.hsync_len      = 32,
		.vsync_len      = 5,
		.sync           = FB_SYNC_EXT,
		.vmode          = FB_VMODE_NONINTERLACED
} }, {
	.bus	= -1,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB24,
	.detect	= NULL,
	.enable	= do_enable_hdmi,
	.mode	= {
		.name           = "HDMI-S720P",
		.refresh        = 60,
		.xres           = 1920,
		.yres           = 720,
		.pixclock       = 39721,
		.left_margin    = 48,
		.right_margin   = 16,
		.upper_margin   = 33,
		.lower_margin   = 10,
		.hsync_len      = 96,
		.vsync_len      = 2,
		.sync           = 0,
		.vmode          = FB_VMODE_NONINTERLACED
} }, {
	.bus	= ATM88PA_I2C_BUS,
	.addr	= ATM88PA_I2C_ADDR,
	.pixfmt	= IPU_PIX_FMT_RGB666,
	.detect	= NULL,
	.enable	= enable_nlb084sv01l,
	.mode	= {
		.name           = "NLB084SV01L",
		.refresh        = 60,
		.xres           = 800,
		.yres           = 600,
		.pixclock       = 28093,
		.left_margin    = 80, /* hback_porch */
		.right_margin   = 48, /* hfront_porch */
		.upper_margin   = 11,  /* vback_porch */
		.lower_margin   = 3, /* vfront_porch */
		.hsync_len      = 32,
		.vsync_len      = 4,
		.sync           = FB_SYNC_EXT,
		.vmode          = FB_VMODE_NONINTERLACED
#if 0
} }, {
	.bus	= ADV739X_I2C_BUS,
	.addr	= ADV739X_I2C_ADDR,
	.pixfmt	= IPU_PIX_FMT_UYVY,
	.detect	= detect_adv739x_tvout,
	.enable	= enable_adv739x_tvout,
	.mode	= {
		.name           = "CVBS-PAL",
		.refresh        = 50,
		.xres           = 720,
		.yres           = 576,
		.pixclock       = 37037,
		.left_margin    = 22, /* hback_porch */
		.right_margin   = 2,  /* hfront_porch */
		.upper_margin   = 23, /* vback_porch */
		.lower_margin   = 2,  /* vfront_porch */
		.hsync_len      = 288,
		.vsync_len      = 1,
		.sync           = FB_SYNC_EXT,
		.vmode          = FB_VMODE_INTERLACED
#endif

} } };
size_t display_count = ARRAY_SIZE(displays);

static void setup_display(void)
{
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;
	int reg;

#if !defined(CONFIG_TARGET_MYIMX6QJH)
	/* Setup HSYNC, VSYNC, DISP_CLK for debugging purposes */
	imx_iomux_v3_setup_multiple_pads(di0_pads, ARRAY_SIZE(di0_pads));
#endif

	enable_ipu_clock();
	imx_setup_hdmi();

	/* Turn on LDB0, LDB1, IPU,IPU DI0 clocks */
	reg = readl(&mxc_ccm->CCGR3);
	reg |=  MXC_CCM_CCGR3_LDB_DI0_MASK | MXC_CCM_CCGR3_LDB_DI1_MASK;
	writel(reg, &mxc_ccm->CCGR3);

	/* set LDB0, LDB1 clk select to 011/011 */
	reg = readl(&mxc_ccm->cs2cdr);
	reg &= ~(MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_MASK
		 | MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_MASK);
	reg |= (3 << MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_OFFSET)
	      | (3 << MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_OFFSET);
	writel(reg, &mxc_ccm->cs2cdr);

	reg = readl(&mxc_ccm->cscmr2);
	reg |= MXC_CCM_CSCMR2_LDB_DI0_IPU_DIV | MXC_CCM_CSCMR2_LDB_DI1_IPU_DIV;
	writel(reg, &mxc_ccm->cscmr2);

	reg = readl(&mxc_ccm->chsccdr);
	reg |= (CHSCCDR_CLK_SEL_LDB_DI0
		<< MXC_CCM_CHSCCDR_IPU1_DI0_CLK_SEL_OFFSET);
	reg |= (CHSCCDR_CLK_SEL_LDB_DI0
		<< MXC_CCM_CHSCCDR_IPU1_DI1_CLK_SEL_OFFSET);
	writel(reg, &mxc_ccm->chsccdr);

	reg = IOMUXC_GPR2_BGREF_RRMODE_EXTERNAL_RES
	     | IOMUXC_GPR2_DI1_VS_POLARITY_ACTIVE_LOW
	     | IOMUXC_GPR2_DI0_VS_POLARITY_ACTIVE_LOW
	     | IOMUXC_GPR2_BIT_MAPPING_CH1_SPWG
	     | IOMUXC_GPR2_DATA_WIDTH_CH1_18BIT
	     | IOMUXC_GPR2_BIT_MAPPING_CH0_SPWG
	     | IOMUXC_GPR2_DATA_WIDTH_CH0_18BIT
#if defined(CONFIG_VIDEO_LVDS1)
	     | IOMUXC_GPR2_LVDS_CH0_MODE_DISABLED
	     | IOMUXC_GPR2_LVDS_CH1_MODE_ENABLED_DI0;
#else
	     | IOMUXC_GPR2_LVDS_CH0_MODE_ENABLED_DI0
	     | IOMUXC_GPR2_LVDS_CH1_MODE_DISABLED;
#endif
	writel(reg, &iomux->gpr[2]);

	reg = readl(&iomux->gpr[3]);
#if defined(CONFIG_VIDEO_LVDS1)
	reg = (reg & ~(IOMUXC_GPR3_LVDS1_MUX_CTL_MASK
#else
	reg = (reg & ~(IOMUXC_GPR3_LVDS0_MUX_CTL_MASK
#endif
			| IOMUXC_GPR3_HDMI_MUX_CTL_MASK))
	    | (IOMUXC_GPR3_MUX_SRC_IPU1_DI0
#if defined(CONFIG_VIDEO_LVDS1)
	       << IOMUXC_GPR3_LVDS1_MUX_CTL_OFFSET);
#else
	       << IOMUXC_GPR3_LVDS0_MUX_CTL_OFFSET);
#endif
	writel(reg, &iomux->gpr[3]);
}
#endif /* CONFIG_VIDEO_IPUV3 */

/* USB ********************************************************************** */
static iomux_v3_cfg_t const usb_otg_pads[] = {
	MX6_PAD_GPIO_1__USB_OTG_ID | MUX_PAD_CTRL(OTG_ID_PAD_CTRL),
};

#define USB_OTHERREGS_OFFSET	0x800
#define UCTRL_PWR_POL		(1 << 9)
static void setup_usb(void)
{
	imx_iomux_v3_setup_multiple_pads(usb_otg_pads,
					 ARRAY_SIZE(usb_otg_pads));

	/*
	 * set daisy chain for otg_pin_id on 6q.
	 * for 6dl, this bit is reserved
	 */
	imx_iomux_set_gpr_register(1, 13, 1, 0);
}

int board_ehci_hcd_init(int port)
{
	u32 *usbnc_usb_ctrl;

	if (port > 1)
		return -EINVAL;

	usbnc_usb_ctrl = (u32 *)(USB_BASE_ADDR + USB_OTHERREGS_OFFSET +
				 port * 4);

	setbits_le32(usbnc_usb_ctrl, UCTRL_PWR_POL);

	return 0;
}

int board_ehci_power(int port, int on)
{
	return 0;
}

/* uSDHC ******************************************************************** */
#if defined(CONFIG_TARGET_MYIMX6LEF) || defined(CONFIG_TARGET_MYIMX6QJH)
#define USDHC3_CD_GPIO	IMX_GPIO_NR(7, 1)
#define USDHC3_RST_GPIO	IMX_GPIO_NR(7, 8)
#else
#define USDHC3_CD_GPIO	IMX_GPIO_NR(4, 10)
#endif

static iomux_v3_cfg_t const usdhc3_pads[] = {
	MX6_PAD_SD3_CLK__SD3_CLK		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_CMD__SD3_CMD		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT0__SD3_DATA0		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT1__SD3_DATA1		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT2__SD3_DATA2		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT3__SD3_DATA3		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
#if (defined(CONFIG_TARGET_MYIMX6LEF) || defined(CONFIG_TARGET_MYIMX6QJH))
	MX6_PAD_SD3_DAT4__GPIO7_IO01	| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD3_RST__GPIO7_IO08	| MUX_PAD_CTRL(NO_PAD_CTRL),
#else
	MX6_PAD_KEY_COL2__GPIO4_IO10    | MUX_PAD_CTRL(NO_PAD_CTRL), /* CD */
#endif
};

static iomux_v3_cfg_t const usdhc4_pads[] = {
	MX6_PAD_SD4_CLK__SD4_CLK   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_CMD__SD4_CMD   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT0__SD4_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT1__SD4_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT2__SD4_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT3__SD4_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT4__SD4_DATA4 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT5__SD4_DATA5 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT6__SD4_DATA6 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT7__SD4_DATA7 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

struct fsl_esdhc_cfg usdhc_cfg[2] = {
	{USDHC3_BASE_ADDR},
	{USDHC4_BASE_ADDR},
};

int mmc_get_env_devno(void)
{
	return CONFIG_SYS_MMC_ENV_DEV;
}

int mmc_map_to_kernel_blk(int dev_no)
{
	return dev_no + 2;
}

int board_mmc_getcd(struct mmc *mmc)
{
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;

	switch (cfg->esdhc_base) {
	case USDHC3_BASE_ADDR:
		ret = !gpio_get_value(USDHC3_CD_GPIO);
		break;
	case USDHC4_BASE_ADDR:
		ret = 1; /* eMMC/uSDHC4 is always present */
		break;
	}

	return ret;
}

int board_mmc_init(bd_t *bis)
{
	int ret;
	int i;

	for (i = 0; i < CONFIG_SYS_FSL_USDHC_NUM; i++) {
		switch (i) {
		case 0:
			imx_iomux_v3_setup_multiple_pads(
				usdhc3_pads, ARRAY_SIZE(usdhc3_pads));
			gpio_direction_input(USDHC3_CD_GPIO);
			usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC3_CLK);
			break;
		case 1:
			imx_iomux_v3_setup_multiple_pads(
				usdhc4_pads, ARRAY_SIZE(usdhc4_pads));
			usdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC4_CLK);
			break;
		default:
			printf("Warning: you configured more USDHC controllers"
			       "(%d) then supported by the board (%d)\n",
			       i + 1, CONFIG_SYS_FSL_USDHC_NUM);
			return -EINVAL;
		}

		ret = fsl_esdhc_initialize(bis, &usdhc_cfg[i]);
		if (ret)
			return ret;
	}

	return 0;
}

/* PCI-E ******************************************************************** */
#if !(defined(CONFIG_TARGET_MYIMX6LEF) || defined(CONFIG_TARGET_MYIMX6QJH))
iomux_v3_cfg_t const pcie_pads[] = {
	MX6_PAD_NANDF_ALE__GPIO6_IO08 | MUX_PAD_CTRL(NO_PAD_CTRL),	/* POWER */
#if defined(CONFIG_TARGET_MYIMX6EK200)
	MX6_PAD_NANDF_CLE__GPIO6_IO07 | MUX_PAD_CTRL(NO_PAD_CTRL),	/* RESET */
#elif (defined(CONFIG_TARGET_MYIMX6EK314) || defined(CONFIG_TARGET_MYIMX6EK336) \
	|| defined(CONFIG_TARGET_MYIMX6ACO))
	MX6_PAD_NANDF_CS1__GPIO6_IO14 | MUX_PAD_CTRL(NO_PAD_CTRL),	/* RESET */
#endif
};

static void setup_pcie(void)
{
	imx_iomux_v3_setup_multiple_pads(pcie_pads, ARRAY_SIZE(pcie_pads));
}
#endif	/* CONFIG_TARGET_MYIMX6LEF, CONFIG_TARGET_MYIMX6QJH */

/* I2C ********************************************************************** */
static struct i2c_pads_info i2c_pad_info0 = {
	.scl = {
		.i2c_mode = MX6_PAD_CSI0_DAT9__I2C1_SCL | I2C_PAD,
		.gpio_mode = MX6_PAD_CSI0_DAT9__GPIO5_IO27 | I2C_PAD,
		.gp = IMX_GPIO_NR(5, 27)
	},
	.sda = {
		.i2c_mode = MX6_PAD_CSI0_DAT8__I2C1_SDA | I2C_PAD,
		.gpio_mode = MX6_PAD_CSI0_DAT8__GPIO5_IO26 | I2C_PAD,
		.gp = IMX_GPIO_NR(5, 26)
	}
};

static struct i2c_pads_info i2c_pad_info1 = {
#if (defined(CONFIG_MYIMX6CB200) || defined(CONFIG_MYIMX6CB314) \
	|| defined(CONFIG_MYIMX6CB336))
	.scl = {
		.i2c_mode = MX6_PAD_KEY_COL3__I2C2_SCL | I2C_PAD,
		.gpio_mode = MX6_PAD_KEY_COL3__GPIO4_IO12 | I2C_PAD,
		.gp = IMX_GPIO_NR(4, 12)
	},
	.sda = {
		.i2c_mode = MX6_PAD_KEY_ROW3__I2C2_SDA | I2C_PAD,
		.gpio_mode = MX6_PAD_KEY_ROW3__GPIO4_IO13 | I2C_PAD,
		.gp = IMX_GPIO_NR(4, 13)
	}
#endif
};

static struct i2c_pads_info i2c_pad_info2 = {
	.scl = {
		.i2c_mode = MX6_PAD_GPIO_3__I2C3_SCL | I2C_PAD,
		.gpio_mode = MX6_PAD_GPIO_3__GPIO1_IO03 | I2C_PAD,
		.gp = IMX_GPIO_NR(1, 3)
	},
	.sda = {
		.i2c_mode = MX6_PAD_GPIO_6__I2C3_SDA | I2C_PAD,
		.gpio_mode = MX6_PAD_GPIO_6__GPIO1_IO06 | I2C_PAD,
		.gp = IMX_GPIO_NR(1, 6)
	}
};

/* Interface **************************************************************** */
int overwrite_console(void)
{
	return 1;
}

int power_init_board(void)
{
	return 0;
}

void ldo_mode_set(int ldo_bypass)
{
	return;
}

/* Board ******************************************************************** */
int checkboard(void)
{
	puts("Board: MYZR i.MX6 Evaluation Kit\nModel: ");
	
#if defined(CONFIG_TARGET_MYIMX6EK200)
	puts("MY-IMX6-EK200");
#elif defined(CONFIG_TARGET_MYIMX6EK314)
	puts("MY-IMX6-EK314");
#elif defined(CONFIG_TARGET_MYIMX6EK336)
	puts("MY-IMX6-EK336");
#elif defined(CONFIG_TARGET_MYIMX6ACO)
	puts("MY-IMX6-ACO");
#elif defined(CONFIG_TARGET_MYIMX6LEF)
	puts("MY-IMX6-LEF");
#elif defined(CONFIG_TARGET_MYIMX6QJH)
	puts("MY-IMX6-QJH");
#endif

#if defined(CONFIG_MX6QP)
	puts("-6QP");
#elif defined(CONFIG_MX6Q)
	puts("-6Q");
#elif defined(CONFIG_MX6SOLO)
	puts("-6S");
#elif defined(CONFIG_MX6DL)
	puts("-6U");
#endif

#if (CONFIG_DDR_MB == 2048)
	puts("-2G");
#elif (CONFIG_DDR_MB == 1024)
	puts("-1G");
#elif (CONFIG_DDR_MB == 512)
	puts("-512M");
#endif

	puts("\n");

	return 0;
}

int board_late_init(void)
{
	return 0;
}

int board_early_init_f(void)
{
	setup_iomux_uart();
#if defined(CONFIG_VIDEO_IPUV3)
	setup_display();
#endif

	return 0;
}

#if defined(CONFIG_MYIMX6CB336)
#define PMIC_I2C_BUS	1
#define PMIC_I2C_ADDR	0x36
#define PMIC_REG_REPCNT	0xf

int pmic_init(void)
{
	unsigned char buf = 0;

	I2C_SET_BUS(PMIC_I2C_BUS);

	if (i2c_read(PMIC_I2C_ADDR, PMIC_REG_REPCNT, 1, &buf, 1))
		return -1;
	
	buf |= 1;
	if (i2c_write(PMIC_I2C_ADDR, PMIC_REG_REPCNT, 1, &buf, 1))
		return -1;

	return 0;
}
#endif

#define ATM88PA_BC_PWR_CTRL_REG	 0x0D /* R/W */
#define ATM88PA_FAULT_REG        0x0f

static void atm88pa_ctrl_bcpwr(u8 ctrl)
{
	unsigned int old_bus;

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(ATM88PA_I2C_BUS);

	i2c_write(ATM88PA_I2C_ADDR, ATM88PA_BC_PWR_CTRL_REG, 1, &ctrl, 1);

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);
}

static int atm88pa_get_power_led_state(void)
{
	int ret;
	u8 state = 0;
	unsigned int old_bus;

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(ATM88PA_I2C_BUS);

	ret  = i2c_read(ATM88PA_I2C_ADDR, ATM88PA_FAULT_REG, 1, &state, 1);
	if (ret < 0)
		state = -1;

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);

	return state;
}

static void atm88pa_set_power_led_state(u8 state)
{
	unsigned int old_bus;

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(ATM88PA_I2C_BUS);

	i2c_write(ATM88PA_I2C_ADDR, ATM88PA_FAULT_REG, 1, &state, 1);

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);
}

static void atm88pa_toggle_power_led_state(void)
{
	int state = 0;

	state = atm88pa_get_power_led_state();
	if (state >= 0)
		atm88pa_set_power_led_state((~state) & 0x01);
}

#define MTY065_HEATER_I2C_ADDR   0x33
#define MTY065_HEATER_STATUS_REG 0x01
#define MTY065_HEATER_TEMPER_REG 0x02

/* Detect the heater module of the mty065x
 * return true or false
 */
static int detect_mty065_heater(int bus)
{
	int ret;
	unsigned int old_bus;

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(bus);

	ret = i2c_probe(MTY065_HEATER_I2C_ADDR) == 0 ? 1 : 0;

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);

	return ret;
}

static void wait_for_mty065_ready(int bus)
{
	int ret;
	u8 status;
	s8 temper;
	unsigned int old_bus;

	/* Save old bus num */
	old_bus = i2c_get_bus_num();

	i2c_set_bus_num(bus);

	while (1) {
		ret  = i2c_read(MTY065_HEATER_I2C_ADDR,
		                MTY065_HEATER_STATUS_REG, 1, &status, 1);
		ret |= i2c_read(MTY065_HEATER_I2C_ADDR,
		                MTY065_HEATER_TEMPER_REG, 1, (u8*)&temper, 1);
		if (ret != 0) {
			printf("Error: Read MTY065 Heater registers failed, ret = %d\n", ret);
			break;
		}

		if (status & 0x01) {
			printf("The MTY065 marked \"Working\", status = %x\n", status);
			break;
		}

		/* Blink the power led */
		atm88pa_toggle_power_led_state();

		printf("The MTY065 is Warming-Up, Current: %d ℃\n", temper);

		mdelay(1000);
	}

	/* Reset the power led */
	atm88pa_set_power_led_state(0);

	/* Restore old bus num */
	i2c_set_bus_num(old_bus);

	printf("The MTY065 heating was complete!\n");
}

/* Hooking before the board_video_skip() */
void board_video_pre_skip(void)
{
	/* To detect the heater on mty065 and
	 * Wait for the temperature is ready for working */
	if (detect_mty065_heater(MTY065_I2C_BUS)) {
		printf("The MTY065 heater detected.\n");
		/* The heater need the power of the back camera */
		atm88pa_ctrl_bcpwr(1);

		/* Wait for warm up */
		wait_for_mty065_ready(MTY065_I2C_BUS);

		/* Power off the back camera after done */
		atm88pa_ctrl_bcpwr(0);

		/* Delay 1s to wait for mty065 ready */
		mdelay(1000);
	}
}

int board_init(void)
{
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;
	
#if defined(CONFIG_MYIMX6CB336)
	pmu_init();
#endif

	setup_spi();

	setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info0);
	setup_i2c(1, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1);
	setup_i2c(2, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info2);
	
#if defined(CONFIG_MYIMX6CB336)
	pmic_init();
#endif

	setup_usb();
	
#if !(defined(CONFIG_TARGET_MYIMX6LEF) || defined(CONFIG_TARGET_MYIMX6QJH))
	setup_pcie();
#endif

	return 0;
}
