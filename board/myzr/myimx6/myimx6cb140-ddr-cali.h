/*
 * Tang Bin 	<tangb@myzr.com.cn>
 *
 * Homepage:	http://www.myzr.com.cn/
 * Document:	http://wiki.myzr.com.cn/
 * Forum:		http://bbs.myzr.com.cn/
 * E-mail:		service@myzr.com.cn
 */

#if defined(CONFIG_MYIMX6G)
#define MMDC_MPWLDECTRL0_CH0	0x00000000
#define MMDC_MPDGCTRL0_PHY0		0x40400140
#define MMDC_MPRDDLCTL_PHY0		0x40404040
#define MMDC_MPWRDLCTL_PHY0		0x40404040

#elif defined(CONFIG_MYIMX6Y)
#define MMDC_MPWLDECTRL0_CH0	0x00000004
#define MMDC_MPDGCTRL0_PHY0		0x40400140
#define MMDC_MPRDDLCTL_PHY0		0x40404040
#define MMDC_MPWRDLCTL_PHY0		0x40404040

#else
#error MYZR: CPU Type is not set
#endif
