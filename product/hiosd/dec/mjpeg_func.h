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

#ifndef __MJPEG_FUNC__
#define  __MJPEG_FUNC__

#ifdef __cplusplus
extern "C" {
#endif

//#include "hi_config.h"
#include "jpegd.h"




HI_S32 DecodeMCU400(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U32 ystride);
HI_S32 DecodeMCU444(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U8 *dstu, HI_U8 *dstv, HI_U32 ystride);
HI_S32 DecodeMCU420(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U8 *dstu, HI_U8 *dstv, HI_U32 ystride);
HI_S32 DecodeMCU422h(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U8 *dstu, HI_U8 *dstv, HI_U32 ystride);
HI_S32 DecodeMCU422v(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U8 *dstu, HI_U8 *dstv, HI_U32 ystride);

HI_S32 DecodeSOF0( MotionJpegObj *mjpeg, const HI_U8* stream, HI_U32 len );
HI_S32 DecodeSOS( MotionJpegObj *mjpeg, HI_U8* stream, HI_U32 len );
HI_S32 DecodeRSTm(MJPEG_DEC_FRAME_S *pDecFrame, MotionJpegObj *mjpeg, HI_U8* stream, HI_U32 len );
HI_S32 DecodeDHT( MotionJpegObj *mjpeg, const HI_U8 *stream, HI_U32 len );
HI_S32 DecodeDQT( MotionJpegObj *mjpeg, const HI_U8 *stream, HI_U32 len );
//HI_S32 DecodeDNL( MotionJpegObj *mjpeg, const HI_U8* stream, HI_S32 len );
HI_S32 DecodeDRI( MotionJpegObj *mjpeg, const HI_U8* stream, HI_U32 len );
void InitDefaultHuffmanTable( MotionJpegObj *mjpeg );
void InitDefaultQuantTable( MotionJpegObj *mjpeg );

void jpeg_idct_fix_c( HI_S16 *src, HI_U8 *dst, HI_U32 stride);

#if SUPPORT_NEON
void jpeg_idct_fix_neon( HI_S16 *src, HI_U8 *dst, HI_U32 stride);
#endif

#if SUPPORT_SSE
void jpeg_idct_fix_sse( HI_S16 *src, HI_U8 *dst, HI_U32 stride);
#endif

#ifdef __cplusplus
}
#endif

#endif //__MJPEG_FUNC__
