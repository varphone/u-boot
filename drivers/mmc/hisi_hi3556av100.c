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

#define REG_IO_CFG_0_BASE		0x04058000
#define EMMC_MUXCTRL_REG_START  0x8c    /* iocfg_reg35 */
#define EMMC_MUXCTRL_REG_END    0xbc    /* iocfg_reg47 */

#define IO_CFG_REG46            0xb8
#define IO_CFG_REG47            0xbc

#define REG_BASE_MISC			0x04528000
#define DRV50_VAL				0x000
#define BIT_RG_EMMC_LHEN_IN		(0x3f << 17)
#define BIT_EMMC_ISO_EN			(0x1 << 16)
#define DRV50_NO_OD_val			0xfff

#define PERI_CRG_EMMC			0x1a8
#define MMC_CLK_SEL_SHIFT      	24
#define MMC_RST					(0x1 << 27)
#define MMC_CLK_EN				(0x1 << 28)
#define MMC_DRV_DLL_RST			(0x1 << 29)
#define MMC_SAM_DLL_RST			(0x1 << 30)
#define MMC_DRV_PHASE_MASK		(0x1f << 24)
#define SW_TUNING_EN			(0x1 << 4)
#define MMC_CLK_50M				0x2
#define SW_TUNING_45			0x4
#define SW_TUNING_0				0x0

#define PERI_EMMC_DRV_DLL		0x1c4

void mmc_muxctrl_config(void)
{
	unsigned int offt;
	unsigned int val;

	/* set emmc multiplexing */
	for (offt = EMMC_MUXCTRL_REG_START;
			offt <= EMMC_MUXCTRL_REG_END;
			offt += 4) {
		val = readl(REG_IO_CFG_0_BASE + offt);
		val &= ~0xf;
		writel(val, REG_IO_CFG_0_BASE + offt);
	}

	val = readl(REG_IO_CFG_0_BASE + IO_CFG_REG46);
	val |= (0x1 << 10);
	writel(val, REG_IO_CFG_0_BASE + IO_CFG_REG46);

	val = readl(REG_IO_CFG_0_BASE + IO_CFG_REG47);
	val |= (0x1 << 10);
	writel(val, REG_IO_CFG_0_BASE + IO_CFG_REG47);
}

static int emmc_hardware_init(void)
{
	unsigned int reg = 0;
	unsigned int timeout = 10;

	mmc_muxctrl_config();

	/* emmc phy config */
	reg = readl(REG_BASE_MISC + MISC_CTRL17);
	reg &= ~(BIT_RG_EMMC_LHEN_IN | BIT_EMMC_ISO_EN);
	writel(reg, REG_BASE_MISC + MISC_CTRL17);

	/*enable phy init*/
	reg = readl(EMMC_PHY_INIT_CTRL);
	reg |= (PHY_INIT_EN | PHY_ZCAL_EN);
	reg &= ~PHY_DLYMEAS_EN;
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
	writel(DRV50_VAL, EMMC_PHY_IOCTL_RONSEL_1_0);
	/* DRV50, OD = 0 */
	writel(DRV50_NO_OD_val, EMMC_PHY_IOCTL_OD_RONSEL_2);

	/* invert the TXCLK */
	reg = readl(EMMC_PHY_DLY_CTL1);
	reg &= ~PHY_INV_CLK;
	writel(reg, EMMC_PHY_DLY_CTL1);

	/*enable pull up*/
	reg = readl(REG_BASE_SCTL + REG_PERI_STAT);
	if (reg & EMMC_PULL_UP_DISABLE)
		/* no pull-up */
		writel(RG_EMMC_PULL_UP << 16, EMMC_PHY_IOCTL_PUPD);
	else
		/* pull-up */
		writel((RG_EMMC_PULL_UP << 16) | RG_EMMC_PUPD_EN, EMMC_PHY_IOCTL_PUPD);

	writel((DA_EMMC_IE << 16) | DA_EMMC_E, EMMC_PHY_IOCTL_IOE);

	/* enable ck_en */
	reg = readl(CRG_REG_BASE + PERI_CRG_EMMC);
	reg |= MMC_CLK_EN;
	writel(reg, CRG_REG_BASE + PERI_CRG_EMMC);

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
			writel(0x6ff, EMMC_PHY_IOCTL_RONSEL_1_0);
			writel(0x6ff, EMMC_PHY_IOCTL_OD_RONSEL_2);
		}
	}
}

static void hisi_set_phase(struct sdhci_host *host)
{
	unsigned int drv_phase, sample_phase;

	if (host->mmc->timing == MMC_TIMING_MMC_HS400) {
		drv_phase = 9;	/*101.25 degree*/
		sample_phase = host->tuning_phase;
	} else if (host->mmc->timing == MMC_TIMING_MMC_HS200) {
		drv_phase = 20;	/*225 degree*/
		sample_phase = host->tuning_phase;
	} else if (host->mmc->timing == MMC_TIMING_UHS_SDR104) {
		drv_phase = 20;	/*225 degree*/
		sample_phase = host->tuning_phase;

	} else if (host->mmc->timing == MMC_TIMING_SD_HS
			|| host->mmc->timing == MMC_TIMING_UHS_SDR25
			|| host->mmc->timing == MMC_TIMING_UHS_DDR50
			|| host->mmc->timing == MMC_TIMING_MMC_HS) {
		drv_phase = 16; /* 180 degree */
		sample_phase = 4; /* 45 degreee*/

	} else {
		drv_phase = 16; /* 180 degree */
		sample_phase = 0;
	}

	hisi_set_drv_phase(host, drv_phase);
	hisi_enable_sample(host);
	hisi_set_sampl_phase(host, sample_phase);

	udelay(25);
}
