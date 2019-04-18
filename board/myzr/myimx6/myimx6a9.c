
#include <stdlib.h>
#include <common.h>
#include <asm/errno.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <asm/imx-common/mxc_i2c.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/boot_mode.h>
#include <asm/imx-common/video.h>
#include <asm/imx-common/spi.h>
#include <asm/arch/mx6-ddr.h>
#include <asm/arch/mxc_hdmi.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/sys_proto.h>
#include <i2c.h>
#include <fsl_esdhc.h>
#include <micrel.h>
#include <miiphy.h>
#include <mmc.h>
#include <netdev.h>
#include <usb.h>
#ifdef CONFIG_CMD_SATA
#include <asm/imx-common/sata.h>
#endif
#ifdef CONFIG_IMX_PWM
#include <asm/imx-common/imx_pwm.h>
#endif
#ifdef CONFIG_UBOOT_LOGO_ENABLE
#include <asm/imx-common/mxc_ipu.h>
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

#define I2C_PAD		MUX_PAD_CTRL(I2C_PAD_CTRL)

/* DRAM ********************************************************************* */
int dram_init(void)
{
#if defined(CONFIG_MY_DDR_DBG)
	gd->ram_size = imx_ddr_size() >> 1;
#else
	gd->ram_size = imx_ddr_size();
#endif
	return 0;
}

#if defined(CONFIG_MYIMX6CB336)
void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_0;
	gd->bd->bi_dram[0].size = (phys_size_t)CONFIG_DDR_MB * 1024 * 1024;
	gd->bd->bi_dram[1].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[1].size = (phys_size_t)CONFIG_DDR_MB * 1024 * 1024;
}
#endif

/* SPI ********************************************************************** */
#define ECSPI1_CS0	IMX_GPIO_NR(2, 30)
#define ECSPI1_CS1	IMX_GPIO_NR(3, 19)

static iomux_v3_cfg_t const ecspi1_pads[] = {
	MX6_PAD_EIM_D16__ECSPI1_SCLK	| MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_D17__ECSPI1_MISO	| MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_D18__ECSPI1_MOSI	| MUX_PAD_CTRL(SPI_PAD_CTRL),
	MX6_PAD_EIM_EB2__GPIO2_IO30		| MUX_PAD_CTRL(NO_PAD_CTRL),
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
	else if (bus == 0 && cs == 0)
		ret = ECSPI1_CS0;
	
	return ret;
}

/* I2C ********************************************************************** */
static struct i2c_pads_info i2c_pad_info1 = {
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
};

/* PMIC: RN5T568 ************************************************************ */
#define PMIC_PWR_CTRL	IMX_GPIO_NR(7, 12)
#define PMIC_INT_PMU	IMX_GPIO_NR(7, 13)

#define PMIC_I2C_BUS	1
#define PMIC_I2C_ADDR	0x36
#define PMIC_REG_REPCNT	0xf

#if defined(CONFIG_MYIMX6_RN5T568)
static iomux_v3_cfg_t const pmic_pads[] = {
	MX6_PAD_GPIO_17__GPIO7_IO12 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_GPIO_18__GPIO7_IO13 | MUX_PAD_CTRL(NO_PAD_CTRL),
};
#endif

int my_pmic_init(void)
{
#if defined(CONFIG_MYIMX6_RN5T568)
	unsigned char buf = 0;
	
	imx_iomux_v3_setup_multiple_pads(pmic_pads, ARRAY_SIZE(pmic_pads));
	
	gpio_direction_output(PMIC_PWR_CTRL, 1);
	gpio_direction_output(PMIC_INT_PMU, 1);

	I2C_SET_BUS(PMIC_I2C_BUS);

	if (i2c_read(PMIC_I2C_ADDR, PMIC_REG_REPCNT, 1, &buf, 1))
		return -1;
	
	buf |= 1;
	if (i2c_write(PMIC_I2C_ADDR, PMIC_REG_REPCNT, 1, &buf, 1))
		return -1;
#endif

	return 0;
}

/* UART ********************************************************************* */
static iomux_v3_cfg_t const dbg_uart[] = {
	MX6_PAD_CSI0_DAT10__UART1_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_CSI0_DAT11__UART1_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static void setup_iomux_uart(void)
{
	imx_iomux_v3_setup_multiple_pads(dbg_uart, ARRAY_SIZE(dbg_uart));
}

/* ENET ********************************************************************* */
static iomux_v3_cfg_t const smi_pads[] = {
	MX6_PAD_ENET_MDIO__ENET_MDIO	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_MDC__ENET_MDC		| MUX_PAD_CTRL(ENET_PAD_CTRL),
};

#if defined(CONFIG_MYIMX6CB200)
static iomux_v3_cfg_t const rmii_pads[] = {
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
};
#endif

#if (defined(CONFIG_MYIMX6CB314) || defined(CONFIG_MYIMX6CB336))
static iomux_v3_cfg_t const rgmii_pads[] = {
	MX6_PAD_RGMII_TXC__RGMII_TXC		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD0__RGMII_TD0		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD1__RGMII_TD1		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD2__RGMII_TD2		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD3__RGMII_TD3		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TX_CTL__RGMII_TX_CTL	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_REF_CLK__ENET_TX_CLK	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RXC__RGMII_RXC		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD0__GPIO6_IO25		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_RGMII_RD1__GPIO6_IO27		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_RGMII_RD2__GPIO6_IO28		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_RGMII_RD3__GPIO6_IO29		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_RGMII_RX_CTL__GPIO6_IO24	| MUX_PAD_CTRL(NO_PAD_CTRL),
	/* KSZ9031 Reset */
	MX6_PAD_ENET_CRS_DV__GPIO1_IO25		| MUX_PAD_CTRL(NO_PAD_CTRL),
};

static iomux_v3_cfg_t const rgmii_pads2[] = {
	MX6_PAD_RGMII_RD0__RGMII_RD0	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD1__RGMII_RD1	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD2__RGMII_RD2	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD3__RGMII_RD3	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RX_CTL__RGMII_RX_CTL	| MUX_PAD_CTRL(ENET_PAD_CTRL),
};
#endif

static void setup_iomux_enet(void)
{
	imx_iomux_v3_setup_multiple_pads(smi_pads, ARRAY_SIZE(smi_pads));

#if defined(CONFIG_MYIMX6CB200)
	imx_iomux_v3_setup_multiple_pads(rmii_pads, ARRAY_SIZE(rmii_pads));

	gpio_direction_output(IMX_GPIO_NR(1, 23) , 0);
	udelay(500);
	gpio_set_value(IMX_GPIO_NR(1, 23), 1);
	imx_iomux_set_gpr_register(1, 21, 1, 1);
#endif
#if (defined(CONFIG_MYIMX6CB314) || defined(CONFIG_MYIMX6CB336))
	imx_iomux_v3_setup_multiple_pads(rgmii_pads, ARRAY_SIZE(rgmii_pads));

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

	imx_iomux_v3_setup_multiple_pads(rgmii_pads2, ARRAY_SIZE(rgmii_pads2));
#endif
}

#if (defined(CONFIG_MYIMX6CB314) || defined(CONFIG_MYIMX6CB336))
int mx6_rgmii_rework(struct phy_device *phydev)
{
	phy_write(phydev, MDIO_DEVAD_NONE, MII_CTRL1000, 0x1c00);
	
	/* KSZ9031 的寄存器值需要确认 */
	if (ksz9031_phy_extended_write(phydev, 0x2,
				       MII_KSZ9031_EXT_RGMII_CTRL_SIG_SKEW,
				       MII_KSZ9031_MOD_DATA_NO_POST_INC,
				       0x0070))
		return -EIO;

	if (ksz9031_phy_extended_write(phydev, 0x2,
				       MII_KSZ9031_EXT_RGMII_RX_DATA_SKEW,
				       MII_KSZ9031_MOD_DATA_NO_POST_INC,
				       0x7777))
		return -EIO;

	if (ksz9031_phy_extended_write(phydev, 0x2,
				       MII_KSZ9031_EXT_RGMII_TX_DATA_SKEW,
				       MII_KSZ9031_MOD_DATA_NO_POST_INC,
				       0x0000))
		return -EIO;

	if (ksz9031_phy_extended_write(phydev, 0x2,
				       MII_KSZ9031_EXT_RGMII_CLOCK_SKEW,
				       MII_KSZ9031_MOD_DATA_NO_POST_INC,
				       0x03f4))
		return -EIO;

	return 0;
}
#endif

int board_phy_config(struct phy_device *phydev)
{
#if (defined(CONFIG_MYIMX6CB314) || defined(CONFIG_MYIMX6CB336))
	mx6_rgmii_rework(phydev);
#endif

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}

#if defined(CONFIG_MYIMX6CB200)
static void setup_fec(void)
{
	/* select ENET MAC0 TX clock from PLL */
#if defined(CONFIG_MX6QP)
	imx_iomux_set_gpr_register(5, 9, 1, 1);
#else
	imx_iomux_set_gpr_register(1, 21, 1, 1);
#endif

	if (enable_fec_anatop_clock(0, ENET_50MHZ))
		printf("Error fec anatop clock settings!\n");
}

int board_eth_init(bd_t *bis)
{
	setup_iomux_enet();

	return cpu_eth_init(bis);
}
#endif

#if (defined(CONFIG_MYIMX6CB314) || defined(CONFIG_MYIMX6CB336))
/* 此接口需要确定 */
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
#endif

/* uSDHC ******************************************************************** */
#define USDHC3_CD_GPIO	IMX_GPIO_NR(4, 10)

static iomux_v3_cfg_t const usdhc3_pads[] = {
	MX6_PAD_SD3_CLK__SD3_CLK		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_CMD__SD3_CMD		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT0__SD3_DATA0		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT1__SD3_DATA1		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT2__SD3_DATA2		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT3__SD3_DATA3		| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_KEY_COL2__GPIO4_IO10    | MUX_PAD_CTRL(NO_PAD_CTRL), /* CD */
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

int board_mmc_get_env_dev(int devno)
{
	return devno - 2;
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

/* USB ********************************************************************** */
static iomux_v3_cfg_t const usb_otg_pads[] = {
	MX6_PAD_GPIO_1__USB_OTG_ID | MUX_PAD_CTRL(OTG_ID_PAD_CTRL),
};

#define USB_OTHERREGS_OFFSET	0x800
#define UCTRL_PWR_POL			(1 << 9)
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

/* PCI-E ******************************************************************** */
iomux_v3_cfg_t const pcie_pads[] = {
	MX6_PAD_NANDF_ALE__GPIO6_IO08 | MUX_PAD_CTRL(NO_PAD_CTRL),	/* POWER */
#if defined(CONFIG_TARGET_MYIMX6EK200)
	MX6_PAD_NANDF_CLE__GPIO6_IO07 | MUX_PAD_CTRL(NO_PAD_CTRL),	/* RESET */
#elif (defined(CONFIG_TARGET_MYIMX6EK314) || defined(CONFIG_TARGET_MYIMX6EK336))
	MX6_PAD_NANDF_CS1__GPIO6_IO14 | MUX_PAD_CTRL(NO_PAD_CTRL),	/* RESET */
#endif
};

static void setup_pcie(void)
{
	imx_iomux_v3_setup_multiple_pads(pcie_pads, ARRAY_SIZE(pcie_pads));
}

/* DISPLAY ****************************************************************** */
static void enable_display_pwm(void)
{
	struct pwm_device pwm = {
		.pwm_id = 0,
		.pwmo_invert = 0,
	};
    
	imx_pwm_config(pwm, 25000, 50000);
	imx_pwm_enable(pwm);
    
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT3__PWM1_OUT | MUX_PAD_CTRL(NO_PAD_CTRL));
}

#if defined(CONFIG_VIDEO_IPUV3) || defined(IPU_OUTPUT_MODE_LCD)
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
};

static void enable_rgb(struct display_info_t const *dev)
{
	imx_iomux_v3_setup_multiple_pads(rgb_pads, ARRAY_SIZE(rgb_pads));
    enable_display_pwm();
}
#endif

#if defined(CONFIG_VIDEO_IPUV3)
iomux_v3_cfg_t const di0_pads[] = {
	MX6_PAD_DI0_DISP_CLK__IPU1_DI0_DISP_CLK,	/* DISP0_CLK */
	MX6_PAD_DI0_PIN2__IPU1_DI0_PIN02,		/* DISP0_HSYNC */
	MX6_PAD_DI0_PIN3__IPU1_DI0_PIN03,		/* DISP0_VSYNC */
};

static void disable_lvds(struct display_info_t const *dev)
{
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;

	int reg = readl(&iomux->gpr[2]);

	reg &= ~(IOMUXC_GPR2_LVDS_CH0_MODE_MASK |
		 IOMUXC_GPR2_LVDS_CH1_MODE_MASK);

	writel(reg, &iomux->gpr[2]);
}

static void enable_lvds(struct display_info_t const *dev)
{
	struct iomuxc *iomux = (struct iomuxc *)
				IOMUXC_BASE_ADDR;
	u32 reg = readl(&iomux->gpr[2]);
	reg |= IOMUXC_GPR2_DATA_WIDTH_CH0_18BIT |
	       IOMUXC_GPR2_DATA_WIDTH_CH1_18BIT;
	writel(reg, &iomux->gpr[2]);
}

static void do_enable_hdmi(struct display_info_t const *dev)
{
	disable_lvds(dev);
	imx_enable_hdmi_phy();
}

struct display_info_t const displays[] = {{
	.bus	= -1,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB666,
	.detect	= NULL,
	.enable	= enable_lvds,
	.mode	= {
		.name           = "MY-LVDS-WSVGA",
		.refresh        = 60,
		.xres           = 1024,
		.yres           = 600,
		.pixclock       = 15384,
		.left_margin    = 220,
		.right_margin   = 40,
		.upper_margin   = 21,
		.lower_margin   = 7,
		.hsync_len      = 60,
		.vsync_len      = 10,
		.sync           = FB_SYNC_EXT,
		.vmode          = FB_VMODE_NONINTERLACED
} }, {
	.bus	= -1,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB24,
	.detect	= NULL,
	.enable	= do_enable_hdmi,
	.mode	= {
		.name           = "MY-HDMI-FHD",
		.refresh        = 60,
		.xres           = 1920,
		.yres           = 1080,
		.pixclock       = 6734,
		.left_margin    = 148,
		.right_margin   = 88,
		.upper_margin   = 36,
		.lower_margin   = 4,
		.hsync_len      = 44,
		.vsync_len      = 5,
		.sync           = 0,
		.vmode          = FB_VMODE_NONINTERLACED
} }, {
	.bus	= 0,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB24,
	.detect	= NULL,
	.enable	= enable_rgb,
	.mode	= {
		.name           = "MY-LCD-WVGA",
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
} } };
size_t display_count = ARRAY_SIZE(displays);

static void setup_display(void)
{
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;
	int reg;

	/* Setup HSYNC, VSYNC, DISP_CLK for debugging purposes */
	imx_iomux_v3_setup_multiple_pads(di0_pads, ARRAY_SIZE(di0_pads));

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
	     | IOMUXC_GPR2_LVDS_CH0_MODE_DISABLED
	     | IOMUXC_GPR2_LVDS_CH1_MODE_ENABLED_DI0;
	writel(reg, &iomux->gpr[2]);

	reg = readl(&iomux->gpr[3]);
	reg = (reg & ~(IOMUXC_GPR3_LVDS1_MUX_CTL_MASK
			| IOMUXC_GPR3_HDMI_MUX_CTL_MASK))
	    | (IOMUXC_GPR3_MUX_SRC_IPU1_DI0
	       << IOMUXC_GPR3_LVDS1_MUX_CTL_OFFSET);
	writel(reg, &iomux->gpr[3]);
}
#endif /* CONFIG_VIDEO_IPUV3 */

/* Interface **************************************************************** */
#ifdef CONFIG_LDO_BYPASS_CHECK
void ldo_mode_set(int ldo_bypass)
{
	return;
}
#endif

int overwrite_console(void)
{
	return 1;
}

int checkboard(void)
{
	puts("Board: MYZR i.MX6 Evaluation Kit\nModel: ");
	
#if defined(CONFIG_TARGET_MYIMX6EK200)
	puts("MY-IMX6-EK200");
#elif defined(CONFIG_TARGET_MYIMX6EK314)
	puts("MY-IMX6-EK314");
#elif defined(CONFIG_TARGET_MYIMX6EK336)
	puts("MY-IMX6-EK336");
#endif

#if defined(CONFIG_MYIMX6QP)
	puts("-6QP");
#elif defined(CONFIG_MYIMX6Q)
	puts("-6Q");
#elif defined(CONFIG_MYIMX6U)
	puts("-6U");
#elif defined(CONFIG_MYIMX6S)
	puts("-6S");
#endif

#if (CONFIG_DDR_MB == 2048)
	puts("-2G");
#elif (CONFIG_DDR_MB == 1024)
	puts("-1G");
#elif (CONFIG_DDR_MB == 512)
	puts("-512M");
#elif (CONFIG_DDR_MB == 256)
	puts("-256M");
#endif

	puts("\n");

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

int board_init(void)
{
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	setup_spi();

	setup_i2c(1, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1);

#if defined(CONFIG_MYIMX6CB200)
	setup_fec();
#endif

	setup_usb();
	
	setup_sata();

	setup_pcie();

	return 0;
}

int power_init_board(void)
{
	return my_pmic_init();
}

int board_late_init(void)
{
#ifdef CONFIG_UBOOT_LOGO_ENABLE
	unsigned int size = DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8);
	unsigned char * pData;
	unsigned int start, count;
	int i, bmpReady = 0;
	int mmc_dev = CONFIG_SYS_MMC_DEV_INDEX;
	struct mmc *mmc = find_mmc_device(mmc_dev);

	pData = (unsigned char *)CONFIG_FB_BASE;

	if (mmc)	{
		if (mmc_init(mmc) == 0) {
			start = ALIGN(UBOOT_LOGO_BMP_ADDR, mmc->read_bl_len) / mmc->read_bl_len;
			count = ALIGN(size, mmc->read_bl_len) / mmc->read_bl_len;
			mmc->block_dev.block_read(&mmc->block_dev, start, count, pData);
			bmpReady = 1;
		}
	}

	if (bmpReady == 0) {
		// Fill RGB frame buffer
		// Red
		for (i = 0; i < (DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8) / 3); i += (DISPLAY_BPP / 8)) {
#if (DISPLAY_BPP == 16)
			pData[i + 0] = 0x00;
			pData[i + 1] = 0xF8;
#else
			pData[i + 0] = 0x00;
			pData[i + 1] = 0x00;
			pData[i + 2] = 0xFF;
#endif
		}

		// Green
		for (; i < (DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8) / 3) * 2; i += (DISPLAY_BPP / 8)) {
#if (DISPLAY_BPP == 16)
			pData[i + 0] = 0xE0;
			pData[i + 1] = 0x07;
#else
			pData[i + 0] = 0x00;
			pData[i + 1] = 0xFF;
			pData[i + 2] = 0x00;
#endif
		}

		// Blue
		for (; i < DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8); i += (DISPLAY_BPP / 8)) {
#if (DISPLAY_BPP == 16)
			pData[i + 0] = 0x1F;
			pData[i + 1] = 0x00;
#else
			pData[i + 0] = 0xFF;
			pData[i + 1] = 0x00;
			pData[i + 2] = 0x00;
#endif
		}
	}

#ifndef CONFIG_SYS_DCACHE_OFF
	flush_dcache_range((u32)pData, (u32)(pData + DISPLAY_WIDTH * DISPLAY_HEIGHT * (DISPLAY_BPP / 8)));
#endif

#ifdef IPU_OUTPUT_MODE_LVDS
    enable_display_pwm();
#endif

#ifdef IPU_OUTPUT_MODE_LCD
	enable_rgb(NULL);
#endif

#ifdef IPU_OUTPUT_MODE_HDMI
	imx_iomux_v3_setup_pad(MX6_PAD_KEY_COL3__HDMI_TX_DDC_SCL | MUX_PAD_CTRL(NO_PAD_CTRL));
	imx_iomux_v3_setup_pad(MX6_PAD_KEY_ROW3__HDMI_TX_DDC_SDA | MUX_PAD_CTRL(NO_PAD_CTRL));
#endif

	ipu_display_setup();
#endif

	return 0;
}

#if defined(MY_DISP_SELECT_FROM_KEYS)
iomux_v3_cfg_t const my_keys_pads[] = {
	MX6_PAD_GPIO_4__GPIO1_IO04			| MUX_PAD_CTRL(NO_PAD_CTRL),
#if defined(CONFIG_TARGET_MYIMX6EK200)
	MX6_PAD_EIM_BCLK__GPIO6_IO31		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_CSI0_DATA_EN__GPIO5_IO20	| MUX_PAD_CTRL(NO_PAD_CTRL),
#elif (defined(CONFIG_TARGET_MYIMX6EK314) || defined(CONFIG_TARGET_MYIMX6EK336))
	MX6_PAD_NANDF_CS2__GPIO6_IO15		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_NANDF_RB0__GPIO6_IO10		| MUX_PAD_CTRL(NO_PAD_CTRL),
#endif
};

#define MY_KEY_FOR_LCD_SELECT	IMX_GPIO_NR(1, 4)
#if defined(CONFIG_TARGET_MYIMX6EK200)
#define MY_KEY_FOR_LDB0_SELECT	IMX_GPIO_NR(6, 31)
#define MY_KEY_FOR_HDMI_SELECT	IMX_GPIO_NR(5, 20)
#elif (defined(CONFIG_TARGET_MYIMX6EK314) || defined(CONFIG_TARGET_MYIMX6EK336))
#define MY_KEY_FOR_LDB0_SELECT	IMX_GPIO_NR(6, 15)
#define MY_KEY_FOR_HDMI_SELECT	IMX_GPIO_NR(6, 10)
#endif

void my_panel_set(void)
{
	imx_iomux_v3_setup_multiple_pads(my_keys_pads, ARRAY_SIZE(my_keys_pads));
	gpio_direction_input(MY_KEY_FOR_LCD_SELECT);
	gpio_direction_input(MY_KEY_FOR_LDB0_SELECT);
	gpio_direction_input(MY_KEY_FOR_HDMI_SELECT);
	
    if (!gpio_get_value(MY_KEY_FOR_HDMI_SELECT)) {
		printf("Try: Panel(%s). Display(%s).\n", displays[1].mode.name, "HDMI");
		setenv("panel", displays[1].mode.name);
		setenv("display", MY_ENV_FB0_IS_HDMI);
    } else if (!gpio_get_value(MY_KEY_FOR_LDB0_SELECT)) {
		printf("Try: Panel(%s). Display(%s).\n", displays[0].mode.name, "LVDS0");
		setenv("panel", displays[0].mode.name);
		setenv("display", MY_ENV_FB0_IS_LVDS0);
	} else if (!gpio_get_value(MY_KEY_FOR_LCD_SELECT)) {
		printf("Try: Panel(%s). Display(%s).\n", displays[2].mode.name, "LCD");
		setenv("panel", displays[2].mode.name);
		setenv("display", MY_ENV_FB0_IS_LCD);
	}
}
#else
void my_panel_set(void)
{
	return;
}
#endif
