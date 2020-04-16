/*
* Copyright (c) 2018 HiSilicon Technologies Co., Ltd.
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/


#ifdef __cplusplus
#if __cplusplus
    extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */




#include <common.h>
#include <command.h>
#include <malloc.h>
#include <version.h>
#include <net.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
#include <config.h>
#include "jpegd.h"


const char hilogo_magic_str[] = "HISILICON LOGO MAGIC";

unsigned long hilogo = 0;
unsigned long jpeg_size=0;
unsigned long VIDEO_DATA_BASE=0;
unsigned long JPEGD_EMAR_BUF=0;
unsigned int output_format = 0;

extern void dcache_enable(void);
extern void dcache_disable(void);
extern int dcache_status(void);
extern void flush_dcache_range(unsigned long start, unsigned long stop);


int jpeg_decode(unsigned int format)
{
    JPEGD_HANDLE Handle;
    HI_S32 s32Ret;

    output_format = format;
    if(dcache_status())
    {
        flush_dcache_range(hilogo, hilogo+jpeg_size);
    }

    Handle = JPEGD_GetHandle();

    s32Ret = JPEGD_StartDecoding(Handle);
    if(HI_SUCCESS != s32Ret)
    {
        printf("decoding error!");
        return -1;
    }

    JPEGD_FinishDecoding(Handle);

    return 0;
}

int load_jpeg(void)
{
    char *str1=NULL;
    char *str2=NULL;
    char *str3=NULL;
    char *str4=NULL;
    long u=0;
    HI_U32 u32Len;
    HI_UL ulInterval;

    printf("jpeg decoding ...\n");

#if 0
    run_command("setenv vobuf 0xcf000000",0);
    run_command("setenv jpeg_size 0x20000",0);
    run_command("setenv jpeg_addr 0x81f00000",0);
#endif

    /* get vo buffer address */
    str1 = getenv("vobuf");
    if(str1)
    {
        VIDEO_DATA_BASE = simple_strtol(str1,NULL,0);
    }
    else
    {
        printf("Invalid vobuf address!\n");
        return 1;
    }
    if(VIDEO_DATA_BASE == 0)
    {
        printf("Invalid VOU buffer address 0x%08lX\n",VIDEO_DATA_BASE);
        return -1;
    }

    /* get emar buffer address for jpegd */
    str2 = getenv("jpeg_emar_buf");
    if(str2)
    {
        JPEGD_EMAR_BUF = simple_strtol(str2,NULL,0);
    }
    else
    {
        printf("Invalid jpeg_emar_buf address!\n");
        return 1;
    }
    if(JPEGD_EMAR_BUF == 0)
    {
        printf("Invalid jpeg_emar_buf address 0x%08lX\n",JPEGD_EMAR_BUF);
        return -1;
    }

    /* jpeg decoder need 2 emar buffer */
    u32Len = ONE_EMAR_BUF_SIZE*2;
    if(VIDEO_DATA_BASE > JPEGD_EMAR_BUF)
    {
        ulInterval = VIDEO_DATA_BASE - JPEGD_EMAR_BUF;
    }
    else
    {
        ulInterval = JPEGD_EMAR_BUF - VIDEO_DATA_BASE;
    }
    if(ulInterval < u32Len)
    {
        printf("vobuf and jpeg_emar_buf is overlapping! Their ulInterval len must larger than %u\n", u32Len);
        return -1;
    }

    /* get jpeg stream address*/
    str3 = getenv("jpeg_size");
    if(str3)
    {
        jpeg_size = simple_strtol(str3,NULL,0);
    }
    else
    {
        printf("you should set jpeg stream address!\n");
        return -1;
    }
    if(jpeg_size == 0)
    {
        printf("Invalid jpeg_size 0x%08lX\n",jpeg_size);
        return -1;
    }

    str4 = getenv("jpeg_addr");
    if(str4)
    {
        hilogo =  simple_strtol(str4,NULL,0);
    }
    else
    {
        printf("you should set jpeg picture's address!\n");
        return -1;
    }
    if(hilogo == 0)
    {
        printf("Invalid jpeg_addr 0x%08lX\n",hilogo);
        return -1;
    }

    printf("<<addr=%#lx, size=%#lx, jpeg_emar_buf=%#lx, vobuf=%#lx>>\n", hilogo, jpeg_size, JPEGD_EMAR_BUF, VIDEO_DATA_BASE);
    if(0 != (hilogo & 0x7f))
    {
        printf("jpeg_addr:%#lx should be align to 128bytes!\n",hilogo);
        return -1;
    }
    if (*(unsigned char *)hilogo != 0xFF || *(unsigned char *)(hilogo+1) != 0xD8)
    {
        printf("addr:%#lx,size:%ld,logoaddr:%#lx,:%2x,%2x\n",hilogo,jpeg_size,u,*(unsigned char *)hilogo,*(unsigned char *)(hilogo+1));
        return -1;
    }

    return 0;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */




