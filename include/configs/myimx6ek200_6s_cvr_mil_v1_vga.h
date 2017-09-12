
#ifndef __CONFIG_H
#define __CONFIG_H

#include <asm/arch/mx6.h>

 /* High Level Configuration Options */
#define CONFIG_MX6DL
#define CONFIG_MX6S_DDR3
#define CONFIG_DDR_32BIT /* for DDR 32bit */
#define CONFIG_BOARD_IS_MYIMXEK200
#define CONFIG_RAM_SIZE_IS_512M
#define MYIMX6EK_SPEC	"6s"
#define MYIMX6EK200_CVR_MIL_V1_VGA 1 /* for CVR-MIL-V1-VGA board */

#include "myimx6_common.h"

#endif				/* __CONFIG_H */
