/*
 * Tang Bin 	<tangb@myzr.com.cn>
 *
 * Homepage:	http://www.myzr.com.cn/
 * Document:	http://wiki.myzr.com.cn/
 * Forum:		http://bbs.myzr.com.cn/
 * E-mail:		service@myzr.com.cn
 */

#if defined(CONFIG_MYIMX6Q)
#define MMDC_CA_ABS_DELAY0	0x00400000
#define MMDC_CA_ABS_DELAY1	0x00400000
#define MMDC_MPRDDLCTL_PHY0	0x40404040
#define MMDC_MPRDDLCTL_PHY1	0x40404040
#define MMDC_MPWRDLCTL_PHY0	0x40404040
#define MMDC_MPWRDLCTL_PHY1	0x40404040

#else
#error MYZR: CPU Type is not set
#endif
