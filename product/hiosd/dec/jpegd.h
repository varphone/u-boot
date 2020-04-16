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

#ifndef __JPEGD_H__
#define __JPEGD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


#include "hi_type.h"
#include "jpegd_drv.h"



#define ALIGN_UP(x, a)           ( ((x) + ((a) - 1) ) & ( ~((a) - 1) ) )

#define  HI_JPEG_DEC_OK             0
#define  HI_JPEG_DEC_FRAME_ERR      1
#define  HI_JPEG_DEC_SCAN_ERR       2
#define  HI_JPEG_DEC_MCU_ERR        3
#define  HI_JPEG_DEC_DRI_ERR        4
#define  HI_JPEG_DEC_DHT_ERR        5
#define  HI_JPEG_DEC_DQT_ERR        6


#define JPEGD_MAX_WIDTH           8192
#define JPEGD_MAX_HEIGHT          8192
#define JPEGD_MIN_WIDTH           8
#define JPEGD_MIN_HEIGHT          8

#define ONE_EMAR_BUF_SIZE (128 * ( (JPEGD_MAX_WIDTH + 7) >> 3 ))


#define BASELINE 	0xC0  // baseline frame header
#define PROGRESSIVE 0xC2  // progressive frame header
#define DHT      	0xC4
#define RST0     	0xD0
#define RST1     	0xD1
#define RST2     	0xD2
#define RST3     	0xD3
#define RST4     	0xD4
#define RST5     	0xD5
#define RST6     	0xD6
#define RST7     	0xD7
#define SOI      	0xD8
#define EOI      	0xD9
#define SOS      	0xDA
#define DQT      	0xDB
#define DNL      	0xDC
#define DRI      	0xDD

#define MIN(a,b)  (((a)<(b))? (a): (b))

#define  HI_MJPEG_DEC_OK          0
#define  HI_MJPEG_NO_PICTURE      -1
#define  HI_MJPEG_ERR_HANDLE      -2

#define  JPEGD_MAX_HANDLE_NUM 3

#define JPEGD_ALLOC_BUFF_CACHED


typedef void* JPEGD_HANDLE;


typedef enum hiJPEGD_STATE
{
    STATE_IDLE  = 0,
	STATE_BUSY	= 1,
    STATE_BUTT
}JPEGD_STATE_E;



typedef struct
{
    HI_S32 MaxCode[16];
    HI_S32 MaxCode16[16];
    HI_S32 MinCode[16];
    HI_S32 Len[16];
    HI_U16 HuffmanValPtr[16];
    HI_U8  HuffmanVal[256];

    HI_U8  FastHuffmanCode[256];
    HI_U8  FastHuffmanLen[256];
    HI_U32 uCode32;
} HuffmanTab;

typedef struct
{
    HI_U8 Quant[64];
} QuantTab;

typedef struct
{
    HI_U32 Nf;
    HI_U32 Colordepth;
    HI_U32 yWidth;
    HI_U32 yHeight;
    HI_U8  Ci[4];
    HI_U8  H[4];
    HI_U8  V[4];
    HI_U8  Tq[4];
    HI_U32 RestartInterval;
	HI_U32 RestartIntervalLogic;
    HI_U32 MaxMCUNumber;
} FrameHeader;

typedef struct
{
    HI_U32 Ns;
    HI_U8  Cs[4];
    HI_U8  Td[4];
    HI_U8  Ta[4];
    HI_U8  Ss;
    HI_U8  Se;
    HI_U8  Ah;
    HI_U8  Al;
} ScanHeader;

typedef struct
{
    HI_U8 *pBuffer;
    HI_U32 BitLen;
    HI_U32 BitOffset;
} Bitstream;

typedef struct
{
    HI_U8 *pu8VirAddr;
	HI_U64 u64PhyAddr;
    HI_U32 u32Len;
    HI_U32 u64PTS;
} JPEGD_STREAM_S;

typedef struct
{
    HI_U8 *StreamBuffer;
    HI_U8 *PicBuffer;
	HI_U32 u32StreamOffest;
	JPEGD_STREAM_S stStream;
    Bitstream   Bits;
    FrameHeader Frame;
    ScanHeader  Scan;
    HuffmanTab  hTab[4];
    QuantTab    qTab[4];

    HI_U8 *pY[2];
    HI_U8 *pU[2];
    HI_U8 *pV[2];
    HI_S32 ValidFrame;
    HI_U32 MaxWidth;
    HI_U32 MaxHeight;
    HI_U32 BufSize;
    HI_U32 FirstMCU;
    HI_S16 JpegDCPred[4]; // 0 for Y;  1 for U;   2 for V;  3 reserved

    HI_U32 HeightInMCU;
    HI_U32 WidthInMCU;
    HI_U32 yStride;
    HI_U32 cStride;
    PICTURE_FORMAT_E enPictureFormat;

    QuantTab   *qTabComponents[3];
    HuffmanTab *hTabComponentsDC[3];
    HuffmanTab *hTabComponentsAC[3];
	HuffmanTab HuffmanTmp;

    HI_U32  u32Profile;        /*0: baseline; 1:progressive; other:unsupport; */
	JPEGD_STATE_E enState;
	JPEGD_VPU_CONFIG_S stVpuConfig;
	JPEGD_VPU_STATUS_S stVpuStatus;
} MotionJpegObj;



typedef struct hiMJPEG_USERDATA_S
{
    HI_U32 uUserDataType;    //Type of userdata
    HI_U32 uUserDataSize;    //Length of userdata in byte
    HI_U8* pData;            //Buffer contains userdata stuff
    struct hiMJPEG_USERDATA_S* pNext;    //Pointer to next userdata
} MJPEG_USERDATA_S;

typedef struct hiMJPEG_DEC_FRAME_S
{
    HI_U8 *pY;
    HI_U8 *pU;
    HI_U8 *pV;
    HI_U32 uWidth;
    HI_U32 uHeight;
    HI_U32 uYStride;
    HI_U32 uCStride;
    HI_S32 uPictureFormat;       // 0: YUV420;
                                 // 1: YUV422;
                                 // 2: YUV444;
                                 // 3: YUV422V;
                                 // 4: YUV400;
                                 // >=5: reserved
    HI_S32 bError;               // 0: No error
                                 // 1: MCU error
    HI_U64 ullPTS;               // time stamp
    HI_U32 reserved;
	HI_S32 ErrorCode;
    MJPEG_USERDATA_S *pUserData; //Pointer to the first userdata
} MJPEG_DEC_FRAME_S;


JPEGD_HANDLE JPEGD_GetHandle(HI_VOID);
HI_S32  JPEGD_DecFrame( JPEGD_HANDLE  Handle, HI_U32 uFlags);
HI_S32 JPEGD_StartOneFrame(JPEGD_HANDLE Handle,HI_U32 uFlags);
HI_S32  JPEGD_WriteRegs(JPEGD_HANDLE Handle, HI_VOID* pRegBase);
HI_S32	JPEGD_ReadRegs(JPEGD_HANDLE Handle, HI_VOID* pRegBase);
HI_S32	JPEGD_StartDecoding(JPEGD_HANDLE Handle);
HI_VOID	JPEGD_FinishDecoding(JPEGD_HANDLE Handle);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* _JPEGD_H_ */

