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


//#include "hi_config.h"
//#include "mjpeg_type.h"
#include "mjpeg_func.h"
#include "jpegd_error.h"
//#include "jpegd.h"


static const HI_U8 zigzag[64] =
{
     0,  1,  8, 16,  9,  2,  3, 10, 17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63,
};

static HI_S32 HuffmanDecoder(Bitstream *bits, const HuffmanTab * pTab, HI_S32 *runlevel, HI_S32 *pResult)
{
    HI_S32 result, code, ErrorFlag = 0;
    HI_U8 *pBuffer = bits->pBuffer + (bits->BitOffset >> 3);
    HI_U32 offset = bits->BitOffset & 7;
    HI_U32 ncode;
    HI_S32 len, tmp;

    ncode = (pBuffer[0]<<24) + (pBuffer[1]<<16) + (pBuffer[2]<<8) + pBuffer[3];
    ncode <<= offset;

    /* decode huffman code length */
    if (ncode < pTab->uCode32)
    {
        tmp = ncode >> 24;
        len = pTab->FastHuffmanLen[tmp] & 0xf;
        result = pTab->FastHuffmanCode[tmp];
    }
    else
    {
        len = 0;
        ncode |= pBuffer[4] >> (8-offset);
        code = (HI_S32)(ncode >> 16);

        while( code > pTab->MaxCode16[len++]);

        tmp = pTab->HuffmanValPtr[len-1];
        tmp += (ncode>>(32-len)) - pTab->MinCode[len-1];
        result = pTab->HuffmanVal[tmp & 0xff];
        ErrorFlag |= ( (code>>(16 - len)) > pTab->MaxCode[len - 1]);
    }

    code = (HI_S32)(ncode << len);
    bits->BitOffset += (result & 0xf) + len;
    *runlevel = result;
    len = (result & 0xf);

    /* decode huffman code */
    code >>= 1;
    code ^= 0x80000000;
    code >>= (31 - len);
    code -= (code >> 16);
    *pResult = code;
    return ErrorFlag;
}

static HI_S32 DecodeDataUnit(MotionJpegObj *mjpeg, HI_U32 stride , HI_U8 *dst, HI_S32 com) //com == 0: Y;  com == 1: U,V;  com == 2: V;
{
    HI_S16 zz[64]= {0};
    HI_S32 ErrorFlag = 0, k = 0, pos, runlevel, code;
    //HI_S32 run;
    Bitstream *bits = &mjpeg->Bits;
    const QuantTab *pQuantTab = mjpeg->qTabComponents[com];
    const HuffmanTab *pDCTab  = mjpeg->hTabComponentsDC[com];
    const HuffmanTab *pACTab  = mjpeg->hTabComponentsAC[com];

#if SUPPORT_SSE
    __asm
    {
        push ESI
        lea ESI, zz
        pxor mm0,mm0
        movq qword ptr[ESI], mm0
        movq [ESI + 8], mm0
        movq [ESI + 16], mm0
        movq [ESI + 24], mm0
        movq [ESI + 32], mm0
        movq [ESI + 40], mm0
        movq [ESI + 48], mm0
        movq [ESI + 56], mm0

        movq [ESI + 64], mm0
        movq [ESI + 72], mm0
        movq [ESI + 80], mm0
        movq [ESI + 88], mm0
        movq [ESI + 96], mm0
        movq [ESI + 104], mm0
        movq [ESI + 112], mm0
        movq [ESI + 120], mm0
        pop ESI
        emms
    }
#endif

    // decode DC coeff
    ErrorFlag |= HuffmanDecoder(bits, pDCTab, &runlevel, &code);
    mjpeg->JpegDCPred[com] += code;
    zz[0] = mjpeg->JpegDCPred[com] * pQuantTab->Quant[0];

    // decode AC coeff
    while(k < 63)
    {
        ErrorFlag |= HuffmanDecoder(bits, pACTab, &runlevel, &code);
        k = (runlevel)? ( k + (runlevel >> 4) + 1 ) : 63;
        pos = zigzag[k & 0x3f];
        zz[pos] = code * pQuantTab->Quant[k & 0x3f];
    }

    // IDCT
    mjpeg->jpeg_idct_fix(zz,dst,stride);

    return ErrorFlag;
}

HI_S32 DecodeMCU400(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U32 yStride)
{
    HI_S32 ErrorFlag = 0;
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty, 0);        // Y
    return ErrorFlag;
}

HI_S32 DecodeMCU444(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U8 *dstu, HI_U8 *dstv, HI_U32 yStride)
{
    HI_S32 ErrorFlag = 0;
    HI_U32 cStride = yStride;
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty, 0);        // Y
    ErrorFlag |= DecodeDataUnit(mjpeg, cStride, dstu, 1);        // U
    ErrorFlag |= DecodeDataUnit(mjpeg, cStride, dstv, 2);        // V
    return ErrorFlag;
}

HI_S32 DecodeMCU420(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U8 *dstu, HI_U8 *dstv, HI_U32 yStride)
{
    HI_S32 ErrorFlag = 0;
    HI_U32 cStride = yStride >> 1;
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty, 0);                      // Y0
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty + 8, 0);                  // Y1
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty + yStride * 8, 0);        // Y2
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty + yStride * 8 + 8, 0);    // Y3
    ErrorFlag |= DecodeDataUnit(mjpeg, cStride, dstu, 1);                      // U
    ErrorFlag |= DecodeDataUnit(mjpeg, cStride, dstv, 2);                      // V
    return ErrorFlag;
}

HI_S32 DecodeMCU422h(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U8 *dstu, HI_U8 *dstv, HI_U32 yStride)
{
    HI_S32 ErrorFlag = 0;
    HI_U32 cStride = yStride >> 1;
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty, 0);     // Y0
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty + 8, 0); // Y1
    ErrorFlag |= DecodeDataUnit(mjpeg, cStride, dstu, 1);     // U
    ErrorFlag |= DecodeDataUnit(mjpeg, cStride, dstv, 2);     // V
    return ErrorFlag;
}

HI_S32 DecodeMCU422v(MotionJpegObj *mjpeg, HI_U8 *dsty, HI_U8 *dstu, HI_U8 *dstv, HI_U32 yStride)
{
    HI_S32 ErrorFlag = 0;
    HI_U32 cStride = yStride;
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty, 0);     // Y0
    ErrorFlag |= DecodeDataUnit(mjpeg, yStride, dsty + yStride * 8, 0); // Y1
    ErrorFlag |= DecodeDataUnit(mjpeg, cStride, dstu, 1);     // U
    ErrorFlag |= DecodeDataUnit(mjpeg, cStride, dstv, 2);     // V
    return ErrorFlag;
}

