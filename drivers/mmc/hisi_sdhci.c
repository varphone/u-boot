/*
 * (C) Copyright 2015 Linaro
 * peter.griffin <peter.griffin@linaro.org>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <sdhci.h>

#if defined(CONFIG_TARGET_HI3556AV100) || defined(CONFIG_TARGET_HI3519AV100)
#include <asm/arch-hi3556av100/platform.h>
#endif

#ifdef CONFIG_TARGET_HI3559AV100
#include <asm/arch-hi3559av100/platform.h>
#endif

#include "mmc_private.h"

#define MIN_FREQ 400000
#define NOT_FOUND -1
#define PHASE_SCALE 32

#define EMMC_PHY_INIT_CTRL      (EMMC_PHY_BASE + 0x4)
#define PHY_INIT_EN             BIT(0)
#define PHY_DLYMEAS_EN          BIT(2)
#define PHY_ZCAL_EN             BIT(3)

#define EMMC_PHY_DLY_CTL1       (EMMC_PHY_BASE + 0x254)
#define PHY_INV_CLK             BIT(31)

#define EMMC_PHY_IOCTL_PUPD     (EMMC_PHY_BASE + 0x260)
#define RG_EMMC_PUPD_EN         0x7ff
#define RG_EMMC_PULL_UP         0x2ff

#define EMMC_PHY_IOCTL_RONSEL_1_0   (EMMC_PHY_BASE + 0x264)
#define RG_EMMC_RONSEL1             0x7ff
#define RG_EMMC_RONSEL0             0x7ff
#define EMMC_PHY_IOCTL_OD_RONSEL_2  (EMMC_PHY_BASE + 0x268)

#define EMMC_PHY_IOCTL_IOE      (EMMC_PHY_BASE + 0x26c)
#define DA_EMMC_IE              0x7ff
#define DA_EMMC_E               0x6ff

#define EMMC_RSTN_PULL_UP_EN	BIT(8)
#define PIN_SEL_EMMC_FUNC	0x1
#define PIN_SEL_EMMC_POWER_EN	0x2

#define REG_PERI_STAT	(SYS_CTRL_REG_BASE + 0x30)
#define EMMC_PULL_UP_DISABLE    BIT(13)

#define REG_MISC_CTRL17         (MISC_REG_BASE + MISC_CTRL17)
#define REG_MISC_CTRL18         (MISC_REG_BASE + MISC_CTRL18)
#define SDIO0_PWRSW_SEL_1V8		BIT(5)
#define SDIO0_PWR_EN			BIT(4)
#define SDIO0_IO_MODE_SEL_1V8	BIT(1)
#define SDIO0_PWR_CTRL_BY_MISC  BIT(0)

#define SDIO_SAMPL_DLL_SRST_REQ	(0x1 << 30)
#define SDIO_DRV_DLL_SRST_REQ	(0x1 << 29)
#define SDIO_CLK_EN		(0x1 << 28)
#define SDIO_SRST_REQ	(0x1 << 27)

#define REG_EMMC_DRV_DLL_CTRL	(CRG_REG_BASE + 0x1b0)
#define REG_SDIO0_DRV_DLL_CTRL  (CRG_REG_BASE + 0x1d4)
#define SDIO_DRV_PHASE_SEL_MASK (0x1f << 24)
#define SDIO_DRV_SEL(phase)     ((phase) << 24)

#define REG_EMMC_DS_DLL_CTRL		(CRG_REG_BASE + 0x1b4)
#define EMMC_DS_DLL_MODE_SSEL       BIT(13)
#define EMMC_DS_DLL_SSEL_MASK       (0x1fff)
#define REG_EMMC_DS180_DLL_CTRL     (CRG_REG_BASE + 0x1b8)
#define EMMC_DS180_DLL_BYPASS       BIT(15)
#define REG_EMMC_DS_DLL_STATUS      (CRG_REG_BASE + 0x1c8)
#define EMMC_DS_DLL_LOCK            BIT(15)
#define EMMC_DS_DLL_MDLY_TAP_MASK   (0x1fff)

#define REG_EMMC_DRV_DLL_STATUS     (CRG_REG_BASE + 0x1c4)
#define REG_SDIO0_DRV_DLL_STATUS    (CRG_REG_BASE + 0x1e8)
#define SDIO_DRV_DLL_LOCK           BIT(15)

#define REG_EMMC_SAMPL_DLL_CTRL     (CRG_REG_BASE + 0x1a8)
#define REG_SDIO0_SAMPL_DLL_CTRL    (CRG_REG_BASE + 0x1ec)
#define SDIO_SAMPL_DLL_SLAVE_EN     BIT(16)

#define REG_EMMC_SAMPL_DLL_STATUS   (CRG_REG_BASE + 0x1bc)
#define REG_SDIO0_SAMPL_DLL_STATUS  (CRG_REG_BASE + 0x1e0)
#define SDIO_SAMPL_DLL_SLAVE_READY  BIT(14)

#define REG_EMMC_SAMPLB_DLL_CTRL    0x1ac
#define SDIO_SAMPLB_DLL_CLK_MASK    (0x1f << 24)
#define SDIO_SAMPLB_SEL(phase)      ((phase) << 24)

static void hisi_enable_sample(struct sdhci_host *host);
static void hisi_set_drv_phase(struct sdhci_host *host,	unsigned int phase);
static void hisi_set_sampl_phase(struct sdhci_host *host, unsigned int phase);

#ifdef CONFIG_TARGET_HI3559AV100
#include "hisi_hi3559av100.c"
#endif

#if defined(CONFIG_TARGET_HI3556AV100) || defined(CONFIG_TARGET_HI3519AV100)
#include "hisi_hi3556av100.c"
#endif

static void sd_hardware_init(void)
{
	unsigned int reg, ctrl;

	/* clk enable */
	reg = readl(REG_SDIO0_CRG);
	reg |= SDIO_CLK_EN;
	writel(reg, REG_SDIO0_CRG);

	/* reset assert*/
	reg = readl(REG_SDIO0_CRG);
	reg |= SDIO_SRST_REQ | SDIO_DRV_DLL_SRST_REQ
		| SDIO_SAMPL_DLL_SRST_REQ;
	writel(reg, REG_SDIO0_CRG);
	udelay(25);

	/* reset deassert */
	reg &= ~SDIO_SRST_REQ;
	writel(reg, REG_SDIO0_CRG);
	udelay(1);

	ctrl = readl(REG_MISC_CTRL18);
	ctrl |= SDIO0_PWR_EN | SDIO0_PWR_CTRL_BY_MISC;
	ctrl &= ~(SDIO0_IO_MODE_SEL_1V8 | SDIO0_PWRSW_SEL_1V8);
	writel(ctrl, REG_MISC_CTRL18);

	udelay(5000);
}

static void hisi_dll_reset_assert(struct sdhci_host *host)
{
	unsigned long crg_addr;
	unsigned int reg;

	crg_addr = host->type == MMC_TYPE_MMC ?
		REG_EMMC_CRG : REG_SDIO0_CRG;
	reg = readl(crg_addr);
	reg |= SDIO_DRV_DLL_SRST_REQ | SDIO_SAMPL_DLL_SRST_REQ;
	writel(reg, crg_addr);
}

static void hisi_dll_reset_deassert(struct sdhci_host *host)
{
	unsigned long crg_addr;
	unsigned int reg;

	crg_addr = host->type == MMC_TYPE_MMC ?
		REG_EMMC_CRG : REG_SDIO0_CRG;
	reg = readl(crg_addr);
	reg &= ~(SDIO_DRV_DLL_SRST_REQ | SDIO_SAMPL_DLL_SRST_REQ);
	writel(reg, crg_addr);
}

static void hisi_set_crg(struct sdhci_host *host, unsigned int clk)
{
	unsigned long crg_addr;
	unsigned int sel, reg;
	unsigned int clk_mux[8] = {100000, 25000000, 49500000,
		99000000, 187500000, 150000000, 198000000, 400000};

	crg_addr = host->type == MMC_TYPE_MMC ?
		REG_EMMC_CRG : REG_SDIO0_CRG;
	reg = readl(crg_addr);
	reg &= ~MMC_CLK_SEL_MASK;

	if (clk <= MIN_FREQ)
		sel = 7;
	else {
		for (sel = 6; sel > 0; sel--) {
			if (clk >= clk_mux[sel])
				break;
		}
	}

	reg |= MMC_CLK_SEL(sel);
	writel(reg, crg_addr);
}

static void hisi_enable_sample(struct sdhci_host *host)
{
	unsigned int reg;

	reg = sdhci_readl(host, SDHCI_AT_CTRL);
	reg |= SDHCI_SAMPLE_EN;
	sdhci_writel(host, reg, SDHCI_AT_CTRL);
}

static void hisi_set_drv_phase(struct sdhci_host *host,
		unsigned int phase)
{
	unsigned long crg_addr;
	unsigned int reg;

	crg_addr = host->type == MMC_TYPE_MMC ?
		REG_EMMC_DRV_DLL_CTRL : REG_SDIO0_DRV_DLL_CTRL;
	reg = readl(crg_addr);
	reg &= ~SDIO_DRV_PHASE_SEL_MASK;
	reg |= SDIO_DRV_SEL(phase);
	writel(reg, crg_addr);
}

static void hisi_set_sampl_phase(struct sdhci_host *host,
		unsigned int phase)
{
	unsigned int reg;

	reg = sdhci_readl(host, SDHCI_AT_STAT);
	reg &= ~SDHCI_PHASE_SEL_MASK;
	reg |= phase;
	sdhci_writel(host, reg, SDHCI_AT_STAT);
}

static void hisi_wait_ds_dll_lock(void)
{
	unsigned int reg, timeout = 20;

	do {
		reg = readl(REG_EMMC_DS_DLL_STATUS);
		if (reg & EMMC_DS_DLL_LOCK)
			return;

		udelay(1000);
		timeout--;
	} while (timeout > 0);

	printf("DS DLL master not locked.\n");
}

static void hisi_set_ds_dll_delay(void)
{
#if !defined(CONFIG_TARGET_HI3519AV100) && !defined(CONFIG_TARGET_HI3556AV100)
	unsigned int reg, mdly_tap;

	reg = readl(REG_EMMC_DS_DLL_STATUS);
	mdly_tap = reg & EMMC_DS_DLL_MDLY_TAP_MASK;

	reg = readl(REG_EMMC_DS_DLL_CTRL);
	reg &= ~(EMMC_DS_DLL_SSEL_MASK);
	reg |= (mdly_tap / 4 + 12) | EMMC_DS_DLL_MODE_SSEL;
	writel(reg, REG_EMMC_DS_DLL_CTRL);

	reg = readl(REG_EMMC_DS180_DLL_CTRL);
	reg |= EMMC_DS180_DLL_BYPASS;
	writel(reg, REG_EMMC_DS180_DLL_CTRL);
#endif
}

static void hisi_wait_drv_dll_lock(struct sdhci_host *host)
{
	unsigned int reg, timeout = 20;
	unsigned long reg_addr;

	reg_addr = host->type == MMC_TYPE_MMC ?
		REG_EMMC_DRV_DLL_STATUS:	REG_SDIO0_DRV_DLL_STATUS;
	do {
		reg = readl(reg_addr);
		if (reg & SDIO_DRV_DLL_LOCK)
			return;

		udelay(1000);
		timeout--;
	} while (timeout > 0);

	printf("DRV DLL master not locked.\n");
}

static void hisi_enable_sampl_dll_slave(struct sdhci_host *host)
{
	unsigned int reg;
	unsigned long reg_addr;

	reg_addr = host->type == MMC_TYPE_MMC ?
		REG_EMMC_SAMPL_DLL_CTRL:	REG_SDIO0_SAMPL_DLL_CTRL;
	reg = readl(reg_addr);
	reg |= SDIO_SAMPL_DLL_SLAVE_EN;
	writel(reg, reg_addr);
}

static void hisi_wait_sampl_dll_slave_ready(struct sdhci_host *host)
{
	unsigned int reg, timeout = 20;
	unsigned long reg_addr;

	reg_addr = host->type == MMC_TYPE_MMC ?
		REG_EMMC_SAMPL_DLL_STATUS: REG_SDIO0_SAMPL_DLL_STATUS;
	do {
		reg = readl(reg_addr);
		if (reg & SDIO_SAMPL_DLL_SLAVE_READY)
			return;

		udelay(1000);
		timeout--;
	} while (timeout > 0);

	printf("SAMPL DLL slave not ready.\n");
}

static int hisi_mmc_set_clk(struct sdhci_host *host, unsigned int clk)
{
	unsigned short ctrl, timeout;

	hisi_dll_reset_assert(host);
	udelay(25);
	hisi_set_crg(host, clk);
	hisi_set_phase(host);
	udelay(25);

	if (clk > MMC_HIGH_52_MAX_DTR) {
		hisi_enable_sampl_dll_slave(host);
		hisi_dll_reset_deassert(host);
	}

	ctrl = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	ctrl |= SDHCI_CLOCK_INT_EN | SDHCI_CLOCK_PLL_EN;
	sdhci_writew(host, ctrl, SDHCI_CLOCK_CONTROL);
	/* Wait max 20 ms */
	timeout = 20;
	while (!((ctrl = sdhci_readw(host, SDHCI_CLOCK_CONTROL))
				& SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
			printf("%s: Internal clock never stabilised.\n",
					__func__);
			return -EBUSY;;
		}
		timeout--;
		udelay(1000);
	}

	if (clk > MMC_HIGH_52_MAX_DTR) {
		hisi_wait_drv_dll_lock(host);
		hisi_wait_sampl_dll_slave_ready(host);
	}

	ctrl |= SDHCI_CLOCK_CARD_EN;
	sdhci_writew(host, ctrl, SDHCI_CLOCK_CONTROL);
	udelay(100);

	if (host->mmc->timing == MMC_TIMING_MMC_HS400 &&
			clk > MMC_HIGH_52_MAX_DTR) {
		hisi_wait_ds_dll_lock();
		hisi_set_ds_dll_delay();
	}

	return 0;
}

static void hisi_mmc_priv_init(struct sdhci_host *host)
{
	unsigned short ctrl;
	unsigned int reg;

	ctrl = sdhci_readw(host, SDHCI_MSHC_CTRL);
	ctrl &= ~SDHCI_CMD_CONFLIT_CHECK;
	sdhci_writew(host, ctrl, SDHCI_MSHC_CTRL);

	reg = sdhci_readl(host, SDHCI_AXI_MBIIU_CTRL);
	reg |= SDHCI_GM_WR_OSRC_LMT | SDHCI_GM_RD_OSRC_LMT;
	sdhci_writel(host, reg, SDHCI_AXI_MBIIU_CTRL);

	reg  = sdhci_readl(host, SDHCI_MULTI_CYCLE);
#if defined(CONFIG_TARGET_HI3519AV100) || defined(CONFIG_TARGET_HI3556AV100)
	reg |= SDHCI_EDGE_DETECT_EN | SDHCI_DOUT_EN_F_EDGE;
#else
	reg &= ~(SDHCI_DATA_DLY_EN | SDHCI_CMD_DLY_EN);
#endif
	sdhci_writel(host, reg, SDHCI_MULTI_CYCLE);
}

static void hisi_enable_edge_tuning(struct sdhci_host *host)
{
	unsigned int reg;

	reg = readl(REG_EMMC_SAMPLB_DLL_CTRL);
	reg &= ~SDIO_SAMPLB_DLL_CLK_MASK;
	reg |= SDIO_SAMPLB_SEL(8);
	writel(reg, REG_EMMC_SAMPLB_DLL_CTRL);

	reg = sdhci_readl(host, SDHCI_MULTI_CYCLE);
	reg |= SDHCI_EDGE_DETECT_EN;
	sdhci_writel(host, reg, SDHCI_MULTI_CYCLE);
}

static void hisi_disable_edge_tuning(struct sdhci_host *host)
{
	unsigned int reg;

	reg = sdhci_readl(host, SDHCI_MULTI_CYCLE);
	reg &= ~SDHCI_EDGE_DETECT_EN;
	sdhci_writel(host, reg, SDHCI_MULTI_CYCLE);
}

static void hisi_disable_card_clk(struct sdhci_host *host)
{
	u16 clk;

	clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	clk &= ~SDHCI_CLOCK_CARD_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);
}

static void hisi_enable_card_clk(struct sdhci_host *host)
{
	u16 clk;

	clk = sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	clk |= SDHCI_CLOCK_CARD_EN;
	sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);
}

static void hisi_select_sampl_phase(struct sdhci_host *host,
		unsigned int phase)
{
	hisi_disable_card_clk(host);
	hisi_set_sampl_phase(host, phase);
	hisi_wait_sampl_dll_slave_ready(host);
	hisi_enable_card_clk(host);
	udelay(1);
}

static int hisi_send_tuning(struct sdhci_host *host, u32 opcode)
{
	int count, err, tuning_num = 1;

	count = 0;
	do {
		err = mmc_send_tuning(host->mmc, opcode);
		if (err)
			break;

		count++;
	} while (count < tuning_num);

	return err;
}

static int hisi_mmc_exec_tuning(struct sdhci_host *host,
		unsigned int opcode)
{
	unsigned int index, val;
	unsigned int found = 0, prev_found = 0;
	unsigned int edge_p2f, edge_f2p, start, end;
	unsigned int phase, fall = NOT_FOUND, rise = NOT_FOUND;
	int err, prev_err = 0;
	unsigned short ctrl;

	hisi_wait_drv_dll_lock(host);
	hisi_enable_sampl_dll_slave(host);
	hisi_enable_sample(host);
	hisi_enable_edge_tuning(host);
	host->is_tuning = 1;

	start = 0;
	end = PHASE_SCALE / 4;

	edge_p2f = start;
	edge_f2p = end;
	for (index = 0; index < end; index++) {
		hisi_select_sampl_phase(host, index * 4);

		err = mmc_send_tuning(host->mmc, opcode);
		if (!err) {
			val = sdhci_readl(host, SDHCI_MULTI_CYCLE);
			found = val & SDHCI_FOUND_EDGE;
		} else
			found = 1;

		if (prev_found && !found)
			edge_f2p = index;
		else if (!prev_found && found)
			edge_p2f = index;

		if ((edge_p2f != start) && (edge_f2p != end))
			break;

		prev_found = found;
		found = 0;
	}

	if ((edge_p2f == start) && (edge_f2p == end)) {
		printf("tuning failed! can not found edge!\n");
		return -1;
	}

	hisi_disable_edge_tuning(host);

	start = edge_p2f * 4;
	end = edge_f2p * 4;
	if (end <= start)
		end += PHASE_SCALE;

	fall = start;
	rise = end;
	for (index = start; index < end; index++) {
		hisi_select_sampl_phase(host, index % PHASE_SCALE);

		err = hisi_send_tuning(host, opcode);
		if (err)
			debug("send tuning CMD%u fail! phase:%d err:%d\n",
					opcode, index, err);

		if (!prev_err && err)
			fall = index;

		if (prev_err && !err)
			rise = index;

		prev_err = err;
	}

	phase = ((fall + rise) / 2 + 16) % PHASE_SCALE;

	printf("tuning done! valid phase shift [%d, %d] Final Phase:%d\n",
			rise % PHASE_SCALE, fall % PHASE_SCALE, phase);

	host->tuning_phase = phase;
	hisi_select_sampl_phase(host, phase);

	ctrl = sdhci_readw(host, SDHCI_HOST_CONTROL2);
	ctrl |= SDHCI_CTRL_TUNED_CLK;
	sdhci_writew(host, ctrl, SDHCI_HOST_CONTROL2);
	host->is_tuning = 0;

	return 0;
}

int hisi_sdhci_add_port(int index, u32 regbase, u32 type)
{
	struct sdhci_host *host = NULL;


	if (type == MMC_TYPE_MMC)
		emmc_hardware_init();
	else
		sd_hardware_init();

	host = calloc(1, sizeof(struct sdhci_host));
	if (!host) {
		puts("sdhci_host malloc fail!\n");
		return -ENOMEM;
	}

	host->name = "hisi-sdhci";
	host->index = index;
	host->type = type;
	host->ioaddr = (void *)(unsigned long)regbase;
	host->quirks = 0;
	if (type == MMC_TYPE_MMC)
		host->host_caps = MMC_MODE_HS400ES |
			MMC_MODE_HS400_1_8V | MMC_MODE_HS200_1_8V | MMC_MODE_HS;
	host->set_clock = hisi_mmc_set_clk;
	host->priv_init = hisi_mmc_priv_init;
	host->set_control_reg = hisi_mmc_set_ioconfig;
	host->execute_tuning = hisi_mmc_exec_tuning;

#ifdef CONFIG_SDHCI_ADMA
	host->max_segs = 512;
	host->max_seg_size = 65536;
#ifdef CONFIG_TARGET_HI3559AV100
	host->desc_sz = sizeof(struct sdhci_adma2_64_desc);
#else
	host->desc_sz = sizeof(struct sdhci_adma2_32_desc);
#endif
	host->adma_table_sz = (host->max_segs + 2) * host->desc_sz;
	host->adma_table = malloc(ALIGN(host->adma_table_sz, CONFIG_SYS_CACHELINE_SIZE));
	host->adma_table += CONFIG_SYS_CACHELINE_SIZE - ((unsigned long)host->adma_table & (CONFIG_SYS_CACHELINE_SIZE - 1));
	if (!host->adma_table) {
		puts("adma table malloc fail!\n");
		free(host);
		return -ENOMEM;
	}
#endif

	add_sdhci(host, CONFIG_HISI_SDHCI_MAX_FREQ, MIN_FREQ);

	return 0;
}

static void print_mmcinfo(struct mmc *mmc)
{
	printf("MMC/SD Card:\n");
	printf("    MID:         0x%x\n", mmc->cid[0] >> 24);
	printf("    Read Block:  %d Bytes\n", mmc->read_bl_len);
	printf("    Write Block: %d Bytes\n", mmc->write_bl_len);
	printf("    Chip Size:   %s Bytes (%s)\n",
			ultohstr(mmc->capacity),
			mmc->high_capacity ? "High Capacity" : "Low Capacity");
	printf("    Name:        \"%c%c%c%c%c\"\n",
			mmc->cid[0] & 0xff,
			(mmc->cid[1] >> 24),
			(mmc->cid[1] >> 16) & 0xff,
			(mmc->cid[1] >> 8) & 0xff,
			mmc->cid[1] & 0xff);

	printf("    Chip Type:   %s\n"
			"    Version:     %d.%d\n",
			IS_SD(mmc) ? "SD" : "MMC",
			EXTRACT_SDMMC_MAJOR_VERSION(mmc->version),
			EXTRACT_SDMMC_MINOR_VERSION(mmc->version));

	printf("    Speed:       %sHz\n", ultohstr(mmc->tran_speed));
	printf("    Bus Width:   %dbit\n", mmc->bus_width);
	printf("    Mode:        %s\n", mmc->strobe_enhanced ? "HS400ES" :
			mmc->timing == MMC_TIMING_MMC_HS400 ? "HS400" :
			mmc->timing == MMC_TIMING_MMC_HS200 ? "HS200" : "HS");
}

int hisi_mmc_init(int dev_num)
{
	struct mmc *mmc = find_mmc_device(dev_num);
	int ret;

	if (!mmc) {
		printf("mmc device not found!!\n");
		return -EINVAL;
	}

	ret = mmc_init(mmc);
	if (ret)
		return ret;

	//print_mmcinfo(mmc);

	if (!IS_SD(mmc))
		return mmc_set_boot_config(mmc);

	return 0;
}

void printf_mmc(int dev_num)
{
	struct mmc *mmc = find_mmc_device(dev_num);

	if (mmc)
		print_mmcinfo(mmc);
}
