/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute  it and/or modify it
* under  the terms of  the GNU General Public License as published by the
* Free Software Foundation;  either version 2 of the  License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#define EMMC_MUXCTRL_REG_START  (IO_CONFIG_REG_BASE + 0x2000)
#define EMMC_MUXCTRL_REG_END    (IO_CONFIG_REG_BASE + 0x201c)
#define EMMC_MUXCTRL_REG_RSTN   (IO_CONFIG_REG_BASE + 0x202c)
#define EMMC_MUXCTRL_REG_POWER_EN   (IO_CONFIG_REG_BASE + 0x2030)

void mmc_muxctrl_config(void)
{
	unsigned int reg_addr, val;

	/* set emmc multiplexing */
	for (reg_addr = EMMC_MUXCTRL_REG_START;
			reg_addr <= EMMC_MUXCTRL_REG_END;
			reg_addr += 4) {

		val = readl((long)reg_addr);
		val &= ~0xf;
		val |= PIN_SEL_EMMC_FUNC;
		writel(val, (long)reg_addr);
	}

	val = readl(EMMC_MUXCTRL_REG_RSTN);
	val &= ~0xf;
	val |= PIN_SEL_EMMC_FUNC;
	val |= EMMC_RSTN_PULL_UP_EN;
	writel(val, EMMC_MUXCTRL_REG_RSTN);

	val = readl(EMMC_MUXCTRL_REG_POWER_EN);
	val &= ~0xf;
	val |= PIN_SEL_EMMC_POWER_EN;
	writel(val, EMMC_MUXCTRL_REG_POWER_EN);
}

int mmc_phy_init(void)
{
	unsigned int reg = 0;
	unsigned int timeout = 10;

	mmc_muxctrl_config();

	/* emmc phy config */
	reg = readl(REG_MISC_CTRL17);
	reg &= ~UFS_EN;
	reg |= (DA_UFS_REFCLK_OEN | DA_UFS_RST_OEN);
	writel(reg, REG_MISC_CTRL17);

	reg = readl(REG_MISC_CTRL17);
	reg &= ~(EMMC_ISO_EN | RG_EMMC_LHEN_IN);
	writel(reg, REG_MISC_CTRL17);

	/*enable phy init*/
	reg = readl(EMMC_PHY_INIT_CTRL);
	reg |= (PHY_INIT_EN | PHY_ZCAL_EN);
	reg &= (~PHY_DLYMEAS_EN);
	writel(reg, EMMC_PHY_INIT_CTRL);

	/*wait phy init and zcal done*/
	for(;;) {
		reg = readl(EMMC_PHY_INIT_CTRL);
		if (!(reg & (PHY_INIT_EN | PHY_ZCAL_EN)))
			break;
		udelay(100);
		if (timeout-- == 0) {
			printf("wait mmc phy cal done time out\n");
			return -1;
		}
	}

	/* DRV50 */
	writel((RG_EMMC_RONSEL1 << 16) | RG_EMMC_RONSEL0,
			EMMC_PHY_IOCTL_RONSEL_1_0);
	/* DRV50, OD = 0 */
	writel(0, EMMC_PHY_IOCTL_OD_RONSEL_2);

	/* do not invert the TXCLK */
	reg = readl(EMMC_PHY_DLY_CTL1);
	reg &= ~PHY_INV_CLK;
	writel(reg, EMMC_PHY_DLY_CTL1);

	writel((DA_EMMC_IE << 16) | DA_EMMC_E, EMMC_PHY_IOCTL_IOE);

	/*enable pull up*/
	reg = readl(REG_PERI_STAT);
	if (!(reg & EMMC_PULL_UP_DISABLE))
		writel((RG_EMMC_PULL_UP << 16) | RG_EMMC_PUPD_EN,
				EMMC_PHY_IOCTL_PUPD);
	else
		writel((RG_EMMC_PULL_UP << 16) & (~RG_EMMC_PUPD_EN),
				EMMC_PHY_IOCTL_PUPD);

	return 0;
}

static int emmc_hardware_init(void)
{
	unsigned int reg = 0;
	unsigned int timeout = 10;

	mmc_muxctrl_config();

	/* emmc phy config */
	reg = readl(REG_MISC_CTRL17);
	reg &= ~UFS_EN;
	reg |= (DA_UFS_REFCLK_OEN | DA_UFS_RST_OEN);
	writel(reg, REG_MISC_CTRL17);

	reg = readl(REG_MISC_CTRL17);
	reg &= ~(EMMC_ISO_EN | RG_EMMC_LHEN_IN);
	writel(reg, REG_MISC_CTRL17);

	/*enable phy init*/
	reg = readl(EMMC_PHY_INIT_CTRL);
	reg |= (PHY_INIT_EN | PHY_ZCAL_EN);
	reg &= (~PHY_DLYMEAS_EN);
	writel(reg, EMMC_PHY_INIT_CTRL);

	/*wait phy init and zcal done*/
	for(;;) {
		reg = readl(EMMC_PHY_INIT_CTRL);
		if (!(reg & (PHY_INIT_EN | PHY_ZCAL_EN)))
			break;
		udelay(100);
		if (timeout-- == 0) {
			printf("wait mmc phy cal done time out\n");
			return -1;
		}
	}

	/* DRV50 */
	writel((RG_EMMC_RONSEL1 << 16) | RG_EMMC_RONSEL0,
			EMMC_PHY_IOCTL_RONSEL_1_0);
	/* DRV50, OD = 0 */
	writel(0, EMMC_PHY_IOCTL_OD_RONSEL_2);

	/* do not invert the TXCLK */
	reg = readl(EMMC_PHY_DLY_CTL1);
	reg &= ~PHY_INV_CLK;
	writel(reg, EMMC_PHY_DLY_CTL1);

	writel((DA_EMMC_IE << 16) | DA_EMMC_E, EMMC_PHY_IOCTL_IOE);

	/*enable pull up*/
	reg = readl(REG_PERI_STAT);
	if (!(reg & EMMC_PULL_UP_DISABLE))
		writel((RG_EMMC_PULL_UP << 16) | RG_EMMC_PUPD_EN,
				EMMC_PHY_IOCTL_PUPD);
	else
		writel((RG_EMMC_PULL_UP << 16) & (~RG_EMMC_PUPD_EN),
				EMMC_PHY_IOCTL_PUPD);

	/* eMMC clk enable */
	reg = readl(REG_EMMC_CRG);
	reg |= SDIO_CLK_EN;
	writel(reg, REG_EMMC_CRG);

	/* eMMC reset assert*/
	reg = readl(REG_EMMC_CRG);
	reg |= SDIO_SRST_REQ | SDIO_DRV_DLL_SRST_REQ
		| SDIO_SAMPL_DLL_SRST_REQ;
	writel(reg, REG_EMMC_CRG);
	udelay(25);

	/* eMMC reset deassert */
	reg &= ~SDIO_SRST_REQ;
	writel(reg, REG_EMMC_CRG);
	udelay(1);

	return 0;
}

static void hisi_mmc_set_ioconfig(struct sdhci_host *host)
{
	unsigned int reg;

	if (host->type == MMC_TYPE_MMC) {
		if (host->mmc->timing == MMC_TIMING_MMC_HS200
				|| host->mmc->timing == MMC_TIMING_MMC_HS400
				|| host->mmc->timing == MMC_TIMING_MMC_HS) {
			reg = sdhci_readw(host, SDHCI_EMMC_CTRL);
			reg |= SDHCI_CARD_IS_EMMC;
			sdhci_writew(host, reg, SDHCI_EMMC_CTRL);

			/* set drv strength to 50ohm */
			writel(0x0, EMMC_PHY_IOCTL_RONSEL_1_0);
			writel(0x6ff, EMMC_PHY_IOCTL_OD_RONSEL_2);
		}
	}
}

static void hisi_set_phase(struct sdhci_host *host)
{
	unsigned int drv_phase, sample_phase;

	if (host->mmc->timing == MMC_TIMING_MMC_HS400) {
		drv_phase = 8;	/*90 degree*/
		sample_phase = host->tuning_phase;
	} else if (host->mmc->timing == MMC_TIMING_MMC_HS200) {
		drv_phase = 20;	/*225 degree*/
		sample_phase = host->tuning_phase;
	} else {
		drv_phase = 16; /* 180 degree */
		sample_phase = 0;
	}

	hisi_set_drv_phase(host, drv_phase);
	hisi_enable_sample(host);
	hisi_set_sampl_phase(host, sample_phase);

	udelay(25);
}
