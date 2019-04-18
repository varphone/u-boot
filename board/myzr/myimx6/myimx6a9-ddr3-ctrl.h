/*
 * Tang Bin 	<tangb@myzr.com.cn>
 *
 * Homepage:	http://www.myzr.com.cn/
 * Document:	http://wiki.myzr.com.cn/
 * Forum:		http://bbs.myzr.com.cn/
 * E-mail:		service@myzr.com.cn
 */

#if (CONFIG_DDR_MB == 512)
#define CH0_CS0_END_VAL 	0x00000017
#elif (CONFIG_DDR_MB == 1024)
#define CH0_CS0_END_VAL 	0x00000027
#elif (CONFIG_DDR_MB == 2048)
#define CH0_CS0_END_VAL 	0x00000047
#elif (CONFIG_DDR_MB == 256)
#define CH0_CS0_END_VAL 	0x0000000F
#endif

#if defined(CONFIG_MYIMX6Q)
#if (CONFIG_DDR_MB == 1024)
#define MMDC0_MDCFG0_VAL	0x54597955
#define MMDC0_MDOR_VAL		0x00591023
#define MMDC0_MDCTL_VAL		0x831A0000
#elif (CONFIG_DDR_MB == 2048)
#define MMDC0_MDCFG0_VAL	0x898E7955
#define MMDC0_MDOR_VAL		0x008E1023
#define MMDC0_MDCTL_VAL		0x841A0000
#elif (CONFIG_DDR_MB == 512)
#define MMDC0_MDCFG0_VAL	0x54597955
#define MMDC0_MDOR_VAL		0x00591023
#define MMDC0_MDCTL_VAL		0x83190000
#else
#error MYZR: Memory size is not set
#endif	/* CONFIG_DDR_MB */

#elif defined(CONFIG_MYIMX6S)
#if (CONFIG_DDR_MB == 512)
#define MMDC0_MDCFG0_VAL	0x3F4352F3
#define MMDC0_MDOR_VAL		0x00431023
#define MMDC0_MDCTL_VAL		0x83190000
#elif (CONFIG_DDR_MB == 1024)
#define MMDC0_MDCFG0_VAL	0x676B52F3
#define MMDC0_MDOR_VAL		0x006B1023
#define MMDC0_MDCTL_VAL		0x84190000
#elif (CONFIG_DDR_MB == 256)
#define MMDC0_MDCFG0_VAL	0x2B2F52F3
#define MMDC0_MDOR_VAL		0x002F1023
#define MMDC0_MDCTL_VAL		0x82190000
#else
#error MYZR: Memory size is not set
#endif	/* CONFIG_DDR_MB */

#elif defined(CONFIG_MYIMX6QP)
#if (CONFIG_DDR_MB == 1024)
#define MMDC0_MDCTL_VAL			0x831A0000
#define GPV0_S_A_0_DDRCONF		0x00000000
#define GPV0_S_A_0_DDRTIMING	0x2891E41A
#elif (CONFIG_DDR_MB == 2048)
#define MMDC0_MDCTL_VAL			0x841A0000
#define GPV0_S_A_0_DDRCONF		0x00000000
#define GPV0_S_A_0_DDRTIMING	0x2891E41A
#elif (CONFIG_DDR_MB == 512)
#define MMDC0_MDCTL_VAL			0x83190000
#define GPV0_S_A_0_DDRCONF		0x00000003
#define GPV0_S_A_0_DDRTIMING	0xA891E41A
#else
#error MYZR: Memory size is not set
#endif	/* CONFIG_DDR_MB */

#elif defined(CONFIG_MYIMX6U)
#if (CONFIG_DDR_MB == 1024)
#define MMDC0_MDCFG0_VAL	0x3F4352F3
#define MMDC0_MDOR_VAL		0x00431023
#define MMDC0_MDCTL_VAL		0x831A0000
#elif (CONFIG_DDR_MB == 2048)
#define MMDC0_MDCFG0_VAL	0x676B52F3
#define MMDC0_MDOR_VAL		0x006B1023
#define MMDC0_MDCTL_VAL		0x841A0000
#elif (CONFIG_DDR_MB == 512)
#define MMDC0_MDCFG0_VAL	0x3F4352F3
#define MMDC0_MDOR_VAL		0x00431023
#define MMDC0_MDCTL_VAL		0x83190000
#else
#error MYZR: Memory size is not set
#endif	/* CONFIG_DDR_MB */

#else
#error MYZR: CPU Type is not set
#endif
