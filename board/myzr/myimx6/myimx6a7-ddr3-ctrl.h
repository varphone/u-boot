/*
 * Tang Bin 	<tangb@myzr.com.cn>
 *
 * Homepage:	http://www.myzr.com.cn/
 * Document:	http://wiki.myzr.com.cn/
 * Forum:		http://bbs.myzr.com.cn/
 * E-mail:		service@myzr.com.cn
 */

#if defined(CONFIG_MYIMX6G)
#if (CONFIG_DDR_MB == 256)
#define MMDC0_MDCFG0_VAL	0x3F4352F3
#define MMDC0_MDOR_VAL		0x00431023
#define CH0_CS0_END_VAL 	0x00000047
#define MMDC0_MDCTL_VAL		0x83180000
#elif (CONFIG_DDR_MB == 128)
#define MMDC0_MDCFG0_VAL	0x2B2F52F3
#define MMDC0_MDOR_VAL		0x002F1023
#define CH0_CS0_END_VAL 	0x00000043
#define MMDC0_MDCTL_VAL		0x82180000
#elif (CONFIG_DDR_MB == 512)
#define MMDC0_MDCFG0_VAL	0x676B52F3
#define MMDC0_MDOR_VAL		0x006B1023
#define CH0_CS0_END_VAL 	0x0000004F
#define MMDC0_MDCTL_VAL		0x84180000
#else
#error MYZR: Memory size is not set
#endif	/* CONFIG_DDR_MB */

#elif defined(CONFIG_MYIMX6Y)
#if (CONFIG_DDR_MB == 256)
#define MMDC0_MDCFG0_VAL	0x3F4352F3
#define MMDC0_MDOR_VAL		0x00431023
#define CH0_CS0_END_VAL 	0x00000047
#define MMDC0_MDCTL_VAL		0x83180000
#elif (CONFIG_DDR_MB == 128)
#define MMDC0_MDCFG0_VAL	0x2B2F52F3
#define MMDC0_MDOR_VAL		0x002F1023
#define CH0_CS0_END_VAL 	0x00000043
#define MMDC0_MDCTL_VAL		0x82180000
#elif (CONFIG_DDR_MB == 512)
#define MMDC0_MDCFG0_VAL	0x676B52F3
#define MMDC0_MDOR_VAL		0x006B1023
#define CH0_CS0_END_VAL 	0x0000004F
#define MMDC0_MDCTL_VAL		0x84180000
#else
#error MYZR: Memory size is not set
#endif	/* CONFIG_DDR_MB */

#else
#error MYZR: CPU Type is not set
#endif
