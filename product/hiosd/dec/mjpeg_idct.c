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
#include "jpegd_error.h"
#include "jpegd.h"

#define DCTSIZE   8

#define CONST_BITS 13



#define FIX_0_298631336 2446
#define FIX_0_390180644 3196
#define FIX_0_541196100 4433
#define FIX_0_765366865 6270
#define FIX_0_899976223 7373
#define FIX_1_175875602 9633
#define FIX_1_501321110 12299
#define FIX_1_847759065 15137
#define FIX_1_961570560 16069
#define FIX_2_053119869 16819
#define FIX_2_562915447 20995
#define FIX_3_072711026 25172

#if SUPPORT_NEON
#include"arm_neon.h"

void jpeg_add128_neon(short *in, unsigned char *out, int stride)
{
    int i;
    int16x8_t rfac = vdupq_n_s16 (128);
    uint8x8_t dst;

    for (i=0; i<8; i++)//n=3
    {
        int16x8_t src0=vld1q_s16(in);
        src0 = vaddq_s16(src0, rfac);
        dst = vqmovun_s16(src0);
        vst1_u8(out, dst);
        in += 8;
        out += stride;
    }
    return;
}
#endif

#if SUPPORT_SSE

/* add 128 and clip to unsigned 8bit int   */
static _inline void jpeg_add128(short *in, unsigned char *out, int stride)
{
    static const __int64 constant80 = 0x0080008000800080;
	REG_SIZE Stride = stride;

    _asm{
        push     ESI
        push     EDI
        push     EDX
        mov      ESI,in
        mov      EDI,out
        mov      EDX,Stride
        mov      ECX,4
        movq     mm7,constant80


        movq     mm0,[ESI]
        movq     mm1,[ESI+8]
        movq     mm2,[ESI+16]
        movq     mm3,[ESI+24]
        paddw    mm0,mm7
        paddw    mm1,mm7
        paddw    mm2,mm7
        paddw    mm3,mm7
        packuswb mm0,mm1
        packuswb mm2,mm3
        movq     [EDI],mm0
        movq     [EDI+EDX],mm2
        add      ESI,32
        add      EDI,EDX
        add      EDI,EDX

        movq     mm0,[ESI]
        movq     mm1,[ESI+8]
        movq     mm2,[ESI+16]
        movq     mm3,[ESI+24]
        paddw    mm0,mm7
        paddw    mm1,mm7
        paddw    mm2,mm7
        paddw    mm3,mm7
        packuswb mm0,mm1
        packuswb mm2,mm3
        movq     [EDI],mm0
        movq     [EDI+EDX],mm2
        add      ESI,32
        add      EDI,EDX
        add      EDI,EDX

        movq     mm0,[ESI]
        movq     mm1,[ESI+8]
        movq     mm2,[ESI+16]
        movq     mm3,[ESI+24]
        paddw    mm0,mm7
        paddw    mm1,mm7
        paddw    mm2,mm7
        paddw    mm3,mm7
        packuswb mm0,mm1
        packuswb mm2,mm3
        movq     [EDI],mm0
        movq     [EDI+EDX],mm2
        add      ESI,32
        add      EDI,EDX
        add      EDI,EDX

        movq     mm0,[ESI]
        movq     mm1,[ESI+8]
        movq     mm2,[ESI+16]
        movq     mm3,[ESI+24]
        paddw    mm0,mm7
        paddw    mm1,mm7
        paddw    mm2,mm7
        paddw    mm3,mm7
        packuswb mm0,mm1
        packuswb mm2,mm3
        movq     [EDI],mm0
        movq     [EDI+EDX],mm2
        emms

        pop      EDX
        pop      EDI
        pop      ESI
    }
    return;
}
#endif

/******** fast idct ****************/
#define BITS_INV_ACC  4
#define SHIFT_INV_ROW  16 - BITS_INV_ACC
#define SHIFT_INV_COL 1 + BITS_INV_ACC
#define RND_INV_ROW  1024 * (6 - BITS_INV_ACC)    /* 1 << (SHIFT_INV_ROW-1)        */


#define c_inv_corr_0 -1024 * (6 - BITS_INV_ACC) + 65536        /* -0.5 + (16.0 or 32.0)    */
#define c_inv_corr_1 1877 * (6 - BITS_INV_ACC)                /* 0.9167    */
#define c_inv_corr_2 1236 * (6 - BITS_INV_ACC)                /* 0.6035    */
#define c_inv_corr_3 680  * (6 - BITS_INV_ACC)                /* 0.3322    */
#define c_inv_corr_4 0    * (6 - BITS_INV_ACC)                /* 0.0        */
#define c_inv_corr_5 -569  * (6 - BITS_INV_ACC)                /* -0.278    */
#define c_inv_corr_6 -512  * (6 - BITS_INV_ACC)                /* -0.25    */
#define c_inv_corr_7 -651  * (6 - BITS_INV_ACC)                /* -0.3176    */

#define RND_INV_ROW_0 RND_INV_ROW + c_inv_corr_0
#define RND_INV_ROW_1 RND_INV_ROW + c_inv_corr_1
#define RND_INV_ROW_2 RND_INV_ROW + c_inv_corr_2
#define RND_INV_ROW_3 RND_INV_ROW + c_inv_corr_3
#define RND_INV_ROW_4 RND_INV_ROW + c_inv_corr_4
#define RND_INV_ROW_5 RND_INV_ROW + c_inv_corr_5
#define RND_INV_ROW_6 RND_INV_ROW + c_inv_corr_6
#define RND_INV_ROW_7 RND_INV_ROW + c_inv_corr_7

/* Table for rows 0,4 - constants are multiplied on cos_4_16 */
#if SUPPORT_SSE
__declspec(align(16)) short tab_i_04[] = {
    16384, 21407, 16384, 8867,
    -16384, 21407, 16384, -8867,
    16384,  -8867,  16384, -21407,
    16384,   8867, -16384, -21407,
    22725,  19266,  19266,  -4520,
    4520,  19266,  19266, -22725,
    12873, -22725,   4520, -12873,
    12873,   4520, -22725, -12873};

/* Table for rows 1,7 - constants are multiplied on cos_1_16 */

__declspec(align(16)) short tab_i_17[] = {
    22725,  29692,  22725,  12299,
    -22725,  29692,  22725, -12299,
    22725, -12299,  22725, -29692,
    22725,  12299, -22725, -29692,
    31521,  26722,  26722,  -6270,
    6270,  26722,  26722, -31521,
    17855, -31521,   6270, -17855,
    17855,   6270, -31521, -17855};

/* Table for rows 2,6 - constants are multiplied on cos_2_16 */

__declspec(align(16)) short tab_i_26[] = {
    21407,  27969,  21407,  11585,
    -21407,  27969,  21407, -11585,
    21407, -11585,  21407, -27969,
    21407,  11585, -21407, -27969,
    29692,  25172,  25172,  -5906,
    5906,  25172,  25172, -29692,
    16819, -29692,   5906, -16819,
    16819,   5906, -29692, -16819};

/* Table for rows 3,5 - constants are multiplied on cos_3_16 */

__declspec(align(16)) short tab_i_35[] = {
    19266,  25172,  19266,  10426,
    -19266,  25172,  19266, -10426,
    19266, -10426,  19266, -25172,
    19266,  10426, -19266, -25172,
    26722,  22654,  22654,  -5315,
    5315,  22654,  22654, -26722,
    15137, -26722,   5315, -15137,
    15137,   5315, -26722, -15137};

__declspec(align(16)) long round_i_0[] = {RND_INV_ROW_0,RND_INV_ROW_0,
    RND_INV_ROW_0,RND_INV_ROW_0};
__declspec(align(16)) long round_i_1[] = {RND_INV_ROW_1,RND_INV_ROW_1,
    RND_INV_ROW_1,RND_INV_ROW_1};
__declspec(align(16)) long round_i_2[] = {RND_INV_ROW_2,RND_INV_ROW_2,
    RND_INV_ROW_2,RND_INV_ROW_2};
__declspec(align(16)) long round_i_3[] = {RND_INV_ROW_3,RND_INV_ROW_3,
    RND_INV_ROW_3,RND_INV_ROW_3};
__declspec(align(16)) long round_i_4[] = {RND_INV_ROW_4,RND_INV_ROW_4,
    RND_INV_ROW_4,RND_INV_ROW_4};
__declspec(align(16)) long round_i_5[] = {RND_INV_ROW_5,RND_INV_ROW_5,
    RND_INV_ROW_5,RND_INV_ROW_5};
__declspec(align(16)) long round_i_6[] = {RND_INV_ROW_6,RND_INV_ROW_6,
    RND_INV_ROW_6,RND_INV_ROW_6};
__declspec(align(16)) long round_i_7[] = {RND_INV_ROW_7,RND_INV_ROW_7,
    RND_INV_ROW_7,RND_INV_ROW_7};

__declspec(align(16)) short tg_1_16[] = {
    13036,  13036,  13036,  13036,    /* tg * (2<<16) + 0.5 */
    13036,  13036,  13036,  13036};
__declspec(align(16)) short tg_2_16[] = {
    27146,  27146,  27146,  27146,    /* tg * (2<<16) + 0.5 */
    27146,  27146,  27146,  27146};
__declspec(align(16)) short tg_3_16[] = {
    -21746, -21746, -21746, -21746,    /* tg * (2<<16) + 0.5 */
    -21746, -21746, -21746, -21746};
__declspec(align(16)) short cos_4_16[] = {
    -19195, -19195, -19195, -19195,    /* cos * (2<<16) + 0.5 */
    -19195, -19195, -19195, -19195};

/*
* In this implementation the outputs of the iDCT-1D are multiplied
*    for rows 0,4 - on cos_4_16,
*    for rows 1,7 - on cos_1_16,
*    for rows 2,6 - on cos_2_16,
*    for rows 3,5 - on cos_3_16
* and are shifted to the left for rise of accuracy
*
* For used constants
*    FIX(float_const) = (short) (float_const * (1<<15) + 0.5)
*
*-----------------------------------------------------------------------------
*
* On the first stage the calculation is executed at once for two rows.
* The permutation for each output row is done on second stage
*    t7 t6 t5 t4 t3 t2 t1 t0 -> t4 t5 t6 t7 t3 t2 t1 t0
*
*-----------------------------------------------------------------------------
*/



#define DCT_8_INV_ROW_2R(TABLE, ROUND1, ROUND2) __asm {    \
    __asm pshuflw  xmm1, xmm0, 10001000b                \
    __asm pshuflw  xmm0, xmm0, 11011101b                \
    __asm pshufhw  xmm1, xmm1, 10001000b                \
    __asm pshufhw  xmm0, xmm0, 11011101b                \
    __asm movdqa   xmm2, XMMWORD PTR [TABLE]            \
    __asm pmaddwd  xmm2, xmm1                            \
    __asm movdqa   xmm3, XMMWORD PTR [TABLE + 32]        \
    __asm pmaddwd  xmm3, xmm0                           \
    __asm pmaddwd  xmm1, XMMWORD PTR [TABLE + 16]        \
    __asm pmaddwd  xmm0, XMMWORD PTR [TABLE + 48]        \
    __asm pshuflw  xmm5, xmm4, 10001000b                \
    __asm pshuflw  xmm4, xmm4, 11011101b                \
    __asm pshufhw  xmm5, xmm5, 10001000b                \
    __asm pshufhw  xmm4, xmm4, 11011101b                \
    __asm movdqa   xmm6, XMMWORD PTR [TABLE]            \
    __asm pmaddwd  xmm6, xmm5                           \
    __asm movdqa   xmm7, XMMWORD PTR [TABLE + 32]        \
    __asm pmaddwd  xmm7, xmm4                           \
    __asm pmaddwd  xmm5, XMMWORD PTR [TABLE + 16]        \
    __asm pmaddwd  xmm4, XMMWORD PTR [TABLE + 48]        \
    __asm pshufd   xmm1, xmm1, 01001110b                \
    __asm pshufd   xmm0, xmm0, 01001110b                \
    __asm paddd    xmm2, XMMWORD PTR [ROUND1]            \
    __asm paddd    xmm3, xmm0                            \
    __asm paddd    xmm1, xmm2                            \
    __asm pshufd   xmm5, xmm5, 01001110b                \
    __asm pshufd   xmm4, xmm4, 01001110b                \
    __asm movdqa   xmm2, xmm1                             \
    __asm psubd    xmm2, xmm3                             \
    __asm psrad    xmm2, SHIFT_INV_ROW                    \
    __asm paddd    xmm1, xmm3                            \
    __asm psrad    xmm1, SHIFT_INV_ROW                  \
    __asm packssdw xmm1, xmm2                            \
    __asm paddd    xmm6, XMMWORD PTR [ROUND2]            \
    __asm paddd    xmm7, xmm4                            \
    __asm paddd    xmm5, xmm6                            \
    __asm movdqa   xmm6, xmm5                            \
    __asm psubd    xmm6, xmm7                           \
    __asm psrad    xmm6, SHIFT_INV_ROW                  \
    __asm paddd    xmm5, xmm7                            \
    __asm psrad    xmm5, SHIFT_INV_ROW                  \
    __asm packssdw xmm5, xmm6                            \
    }

/*
*
* The second stage - inverse DCTs of columns
*
* The inputs are multiplied
*    for rows 0,4 - on cos_4_16,
*    for rows 1,7 - on cos_1_16,
*    for rows 2,6 - on cos_2_16,
*    for rows 3,5 - on cos_3_16
* and are shifted to the left for rise of accuracy
*/

#define DCT_8_INV_COL_8R(INP, OUTP) __asm {        \
    __asm movdqa   xmm0, [INP + 5*16]            \
    __asm movdqa   xmm1, XMMWORD PTR tg_3_16    \
    __asm movdqa   xmm2, xmm0                    \
    __asm movdqa   xmm3, [INP + 3*16]           \
    __asm pmulhw   xmm0, xmm1                   \
    __asm movdqa   xmm4, [INP + 7*16]           \
    __asm pmulhw   xmm1, xmm3                   \
    __asm movdqa   xmm5, XMMWORD PTR tg_1_16       \
    __asm movdqa   xmm6, xmm4                    \
    __asm pmulhw   xmm4, xmm5                   \
    __asm paddsw   xmm0, xmm2                   \
    __asm pmulhw   xmm5, [INP + 1*16]           \
    __asm paddsw   xmm1, xmm3                   \
    __asm movdqa   xmm7, [INP + 6*16]            \
    __asm paddsw   xmm0, xmm3                    \
    __asm movdqa   xmm3, XMMWORD PTR tg_2_16    \
    __asm psubsw   xmm2, xmm1                    \
    __asm pmulhw   xmm7, xmm3                    \
    __asm movdqa   xmm1, xmm0                    \
    __asm pmulhw   xmm3, [INP + 2*16]           \
    __asm psubsw   xmm5, xmm6                    \
    __asm paddsw   xmm4, [INP + 1*16]            \
    __asm paddsw   xmm0, xmm4                    \
    __asm psubsw   xmm4, xmm1                    \
    __asm pshufhw  xmm0, xmm0, 00011011b        \
    __asm paddsw   xmm7, [INP + 2*16]            \
    __asm movdqa   xmm6, xmm5                    \
    __asm psubsw   xmm3, [INP + 6*16]            \
    __asm psubsw   xmm5, xmm2                    \
    __asm paddsw   xmm6, xmm2                    \
    __asm movdqa   [OUTP + 7*16], xmm0            \
    __asm movdqa   xmm1, xmm4                    \
    __asm movdqa   xmm2, XMMWORD PTR cos_4_16      \
    __asm paddsw   xmm4, xmm5                    \
    __asm movdqa   xmm0, XMMWORD PTR cos_4_16      \
    __asm pmulhw   xmm2, xmm4                    \
    __asm pshufhw  xmm6, xmm6, 00011011b        \
    __asm movdqa   [OUTP + 3*16], xmm6            \
    __asm psubsw   xmm1, xmm5                    \
    __asm movdqa   xmm6, [INP + 0*16]           \
    __asm pmulhw   xmm0, xmm1                    \
    __asm movdqa   xmm5, [INP + 4*16]            \
    __asm paddsw   xmm4, xmm2                    \
    __asm paddsw   xmm5, xmm6                   \
    __asm psubsw   xmm6, [INP + 4*16]           \
    __asm paddsw   xmm0, xmm1                    \
    __asm pshufhw  xmm4, xmm4, 00011011b        \
    __asm movdqa   xmm2, xmm5                    \
    __asm paddsw   xmm5, xmm7                    \
    __asm movdqa   xmm1, xmm6                    \
    __asm psubsw   xmm2, xmm7                    \
    __asm movdqa   xmm7, [OUTP + 7*16]            \
    __asm paddsw   xmm6, xmm3                    \
    __asm pshufhw  xmm5, xmm5, 00011011b        \
    __asm paddsw   xmm7, xmm5                    \
    __asm psubsw   xmm1, xmm3                    \
    __asm pshufhw  xmm6, xmm6, 00011011b        \
    __asm movdqa   xmm3, xmm6                    \
    __asm paddsw   xmm6, xmm4                    \
    __asm pshufhw  xmm2, xmm2, 00011011b        \
    __asm psraw    xmm7, SHIFT_INV_COL           \
    __asm movdqa   [OUTP + 0*16], xmm7            \
    __asm movdqa   xmm7, xmm1                    \
    __asm paddsw   xmm1, xmm0                    \
    __asm psraw    xmm6, SHIFT_INV_COL            \
    __asm movdqa   [OUTP + 1*16], xmm6            \
    __asm pshufhw  xmm1, xmm1, 00011011b        \
    __asm movdqa   xmm6, [OUTP + 3*16]            \
    __asm psubsw   xmm7, xmm0                    \
    __asm psraw    xmm1, SHIFT_INV_COL           \
    __asm movdqa   [OUTP + 2*16], xmm1            \
    __asm psubsw   xmm5, [OUTP + 7*16]            \
    __asm paddsw   xmm6, xmm2                    \
    __asm psubsw   xmm2, [OUTP + 3*16]            \
    __asm psubsw   xmm3, xmm4                    \
    __asm psraw    xmm7, SHIFT_INV_COL          \
    __asm pshufhw  xmm7, xmm7, 00011011b        \
    __asm movdqa   [OUTP + 5*16], xmm7            \
    __asm psraw    xmm5, SHIFT_INV_COL            \
    __asm movdqa   [OUTP + 7*16], xmm5            \
    __asm psraw    xmm6, SHIFT_INV_COL            \
    __asm movdqa   [OUTP + 3*16], xmm6            \
    __asm psraw    xmm2, SHIFT_INV_COL            \
    __asm movdqa   [OUTP + 4*16], xmm2            \
    __asm psraw    xmm3, SHIFT_INV_COL            \
    __asm movdqa   [OUTP + 6*16], xmm3            \
    }

/*
*
*  Name:      dct_8x8_inv_16s
*  Purpose:   Inverse Discrete Cosine Transform 8x8 with
*             2D buffer of short int data
*  Context:
*      void dct_8x8_inv_16s ( short *src, short *dst )
*  Parameters:
*      src  - Pointer to the source buffer
*      dst  - Pointer to the destination buffer
*
*/

void dct_8x8_inv_16s ( short *src, short *dst ) {

    __asm {

        mov     ECX,  src
        mov     EDX,  dst

        movdqa  xmm0, [ECX+0*16]
        movdqa  xmm4, [ECX+4*16]
        DCT_8_INV_ROW_2R(tab_i_04, round_i_0, round_i_4)
        movdqa     [EDX+0*16], xmm1
        movdqa     [EDX+4*16], xmm5

        movdqa  xmm0, [ECX+1*16]
        movdqa  xmm4, [ECX+7*16]
        DCT_8_INV_ROW_2R(tab_i_17, round_i_1, round_i_7)
        movdqa     [EDX+1*16], xmm1
        movdqa     [EDX+7*16], xmm5

        movdqa  xmm0, [ECX+3*16]
        movdqa  xmm4, [ECX+5*16]
        DCT_8_INV_ROW_2R(tab_i_35, round_i_3, round_i_5);
        movdqa     [EDX+3*16], xmm1
        movdqa     [EDX+5*16], xmm5

        movdqa  xmm0, [ECX+2*16]
        movdqa  xmm4, [ECX+6*16]
        DCT_8_INV_ROW_2R(tab_i_26, round_i_2, round_i_6);
        movdqa     [EDX+2*16], xmm1
        movdqa     [EDX+6*16], xmm5

        DCT_8_INV_COL_8R(EDX+0, EDX+0);
    }
}




/*
*  Name:
*    Add128_8x8_16s8u
*
*  Purpose:
*    signed to unsigned conversion (level shift)
*    for 8x8 block of DCT coefficients
*/

__declspec(align(16)) HI_U32 const_128[]= {0x00800080, 0x00800080, 0x00800080, 0x00800080};

void Add128_8x8_16s8u(const HI_S16 *pSrc, HI_U8 *pDst, HI_U32 DstStep)
{
	REG_SIZE step = DstStep;
    __asm {
        push        EAX
        push        EBX
        push        ECX
        push        EDX
        push        ESI
        push        EDI

        mov         ESI, pSrc
        mov         EDI, pDst
        mov         EDX, step
        mov         ECX, 2
        mov         EBX, EDX
        mov         EAX, EDX
        sal         EBX, 1
        add         EAX, EBX
        movdqa      xmm7, XMMWORD PTR const_128

    again:

        movdqa      xmm0, XMMWORD PTR [ESI+0]  ; line 0
        movdqa      xmm1, XMMWORD PTR [ESI+16] ; line 1
        movdqa      xmm2, XMMWORD PTR [ESI+32] ; line 2
        movdqa      xmm3, XMMWORD PTR [ESI+48] ; line 3

        paddw     xmm0, xmm7
        paddw     xmm1, xmm7
        paddw     xmm2, xmm7
        paddw     xmm3, xmm7

        packuswb  xmm0, xmm1
        packuswb  xmm2, xmm3

        movq      QWORD PTR [EDI], xmm0      ;0*DstStep
        movq      QWORD PTR [EDI+EBX], xmm2  ;2*DstStep

        psrldq      xmm0, 8
        psrldq      xmm2, 8

        movq      QWORD PTR [EDI+EDX], xmm0  ;1*DstStep
        movq      QWORD PTR [EDI+EAX], xmm2  ;3*DstStep

        add         EDI, EBX
        add         ESI, 64
        add         EDI, EBX
        dec         ECX
        jnz         again

        pop         EDI
        pop         ESI
        pop         EDX
        pop         ECX
        pop         EBX
        pop         EAX
    }
}

void jpeg_idct_fix_sse(HI_S16 *pSrc, HI_U8 *pDst, HI_U32 stride)
{
    __declspec(align(16)) HI_S16 buf[DCTSIZE * DCTSIZE];
    HI_S16 *workbuf = buf;
    dct_8x8_inv_16s(pSrc,workbuf);
    Add128_8x8_16s8u(workbuf,pDst,stride);
}
#endif

short int idct_mmx_perm[64];
void idct_mmx_init(void)
{
    int i;
    /* the mmx/mmxext idct uses a reordered input, so we patch scan tables */
    for (i = 0; i < 64; i++)
    {
        idct_mmx_perm[i] = (i & 0x38) | ((i & 6) >> 1) | ((i & 1) << 2);
    }
}

#if SUPPORT_NEON
static void transpose8x8(HI_S16 *in)
{
    register short *dataptr;
    dataptr = in;
    int16x8x2_t temp0;
    int16x8x2_t temp1;
    int16x8x2_t temp2;
    int16x8x2_t temp3;

    int16x8x2_t temp4;
    int16x8x2_t temp5;
    int16x8x2_t temp6;
    int16x8x2_t temp7;

    temp0 = vld2q_s16(dataptr);
    temp1 = vld2q_s16(dataptr+16);
    temp2 = vld2q_s16(dataptr+32);
    temp3 = vld2q_s16(dataptr+48);

    temp4 = vuzpq_s16(temp0.val[0], temp1.val[0]);  // 0  4  0  4
    temp5 = vuzpq_s16(temp0.val[1], temp1.val[1]);  // 1  5  1  5
    temp6 = vuzpq_s16(temp2.val[0], temp3.val[0]);  // 2  6  2  6
    temp7 = vuzpq_s16(temp2.val[1], temp3.val[1]);  // 3  7  3  7

    temp0 = vuzpq_s16(temp4.val[0], temp6.val[0]);  // 0  2  4  6
    temp1 = vuzpq_s16(temp4.val[1], temp6.val[1]);  // 0  2  4  6
    temp2 = vuzpq_s16(temp5.val[0], temp7.val[0]);  // 1  3  5  7
    temp3 = vuzpq_s16(temp5.val[1], temp7.val[1]);  // 1  3  5  7

    temp4 = vuzpq_s16(temp0.val[0], temp2.val[0]);  // 0  4  0  4
    temp5 = vuzpq_s16(temp1.val[0], temp3.val[0]);  // 2  6  2  6
    temp6 = vuzpq_s16(temp0.val[1], temp2.val[1]);  // 1  5  1  5
    temp7 = vuzpq_s16(temp1.val[1], temp3.val[1]);  // 3  7  3  7

    vst2q_s16(dataptr, temp4);
    vst2q_s16(dataptr+16, temp5);
    vst2q_s16(dataptr+32, temp6);
    vst2q_s16(dataptr+48, temp7);
}

// the same as line, only >>17 change to >> 12
static void transform_4x8_col(HI_S16 *in)
{
    register short *dataptr = in;

    int16x4_t src1 = vld1_s16(dataptr+8);
    int16x4_t src3 = vld1_s16(dataptr+24);
    int16x4_t src5 = vld1_s16(dataptr+40);
    int16x4_t src7 = vld1_s16(dataptr+56);

    int16x4_t vz1 = vadd_s16(src7, src1);
    int16x4_t vz2 = vadd_s16(src5, src3);
    int16x4_t vz3 = vadd_s16(src7, src3);
    int16x4_t vz4 = vadd_s16(src5, src1);
    int16x4_t vz5 = vadd_s16(vz3, vz4);

    int16x4_t con0 = vdup_n_s16 (FIX_0_899976223);
    int16x4_t con1 = vdup_n_s16 (FIX_2_562915447);
    int16x4_t con2 = vdup_n_s16 (FIX_1_175875602);
    int32x4_t vlz5 = vmull_s16(vz5, con2);
    int32x4_t vlz1 = vmull_s16(vz1, con0);
    int32x4_t vlz2 = vmull_s16(vz2, con1);

    int16x4_t con3 = vdup_n_s16 (FIX_1_961570560);
    int16x4_t con4 = vdup_n_s16 (FIX_0_390180644);
    int32x4_t vlz3 = vmlsl_s16(vlz5, vz3, con3);
    int32x4_t vlz4 = vmlsl_s16(vlz5, vz4, con4);

    con0 = vdup_n_s16 (FIX_0_298631336);
    con1 = vdup_n_s16 (FIX_2_053119869);
    con2 = vdup_n_s16 (FIX_3_072711026);
    con3 = vdup_n_s16 (FIX_1_501321110);

    int32x4_t vtmp0 = vsubq_s32(vlz3, vlz1);
    int32x4_t vtmp1 = vsubq_s32(vlz4, vlz2);
    int32x4_t vtmp2 = vsubq_s32(vlz3, vlz2);
    int32x4_t vtmp3 = vsubq_s32(vlz4, vlz1);

    vtmp0 = vmlal_s16(vtmp0, src7, con0);
    vtmp1 = vmlal_s16(vtmp1, src5, con1);
    vtmp2 = vmlal_s16(vtmp2, src3, con2);
    vtmp3 = vmlal_s16(vtmp3, src1, con3);

    con0 = vdup_n_s16 (FIX_0_541196100);
    src1 = vld1_s16(dataptr);
    src5 = vld1_s16(dataptr+32);
    src3 = vld1_s16(dataptr+16);
    src7 = vld1_s16(dataptr+48);
    vz1  = vadd_s16(src3, src7);
    vlz1 = vmull_s16(vz1, con0);                              //int32x4_t  vmull_s16(int16x4_t a, int16x4_t b);   y = a * b

    //int32x4_t  vaddl_s16(int16x4_t a, int16x4_t b);
    int32x4_t vtmp10 = vaddl_s16(src1, src5);
    int32x4_t vtmp11 = vsubl_s16(src1, src5);

// int32x4_t  vmlal_s16(int32x4_t a, int16x4_t b, int16x4_t c);   y = a + b * c
// int32x4_t  vmlsl_s16(int32x4_t a, int16x4_t b, int16x4_t c);   y = a - b * c
    con1 = vdup_n_s16 (FIX_1_847759065);
    con2 = vdup_n_s16 (FIX_0_765366865);
    vlz5 =  vdupq_n_s32 (2048);
    int32x4_t vtmp12 = vmlsl_s16(vlz1, src7, con1);
    int32x4_t vtmp13 = vmlal_s16(vlz1, src3, con2);

    vtmp10 = vshlq_n_s32(vtmp10, 13);
    vtmp11 = vshlq_n_s32(vtmp11, 13);

    vtmp10 = vsubq_s32(vtmp10, vtmp13);   // tmp13
    vtmp13 = vaddq_s32(vtmp13, vtmp13);
    vtmp13 = vaddq_s32(vtmp13, vtmp10);   // tmp10

    vtmp11 = vsubq_s32(vtmp11, vtmp12);   // tmp12
    vtmp12 = vaddq_s32(vtmp12, vtmp12);
    vtmp12 = vaddq_s32(vtmp12, vtmp11);   // tmp11

//    vtmp10 = vaddq_s32(vtmp10, vlz5);
//    vtmp11 = vaddq_s32(vtmp11, vlz5);
//    vtmp12 = vaddq_s32(vtmp12, vlz5);
//    vtmp13 = vaddq_s32(vtmp13, vlz5);


    vlz1 = vhaddq_s32(vtmp13, vtmp3);   // tmp10 + tmp3
    vlz2 = vhsubq_s32(vtmp13, vtmp3);   // tmp10 - tmp3
    vlz3 = vhaddq_s32(vtmp12, vtmp2);   // tmp11 + tmp2
    vlz4 = vhsubq_s32(vtmp12, vtmp2);   // tmp11 - tmp2
    vz1 = vrshrn_n_s32(vlz1, 11);
    vz2 = vrshrn_n_s32(vlz2, 11);
    vz3 = vrshrn_n_s32(vlz3, 11);
    vz4 = vrshrn_n_s32(vlz4, 11);

    vst1_s16(dataptr, vz1);
    vst1_s16(dataptr+56, vz2);
    vst1_s16(dataptr+8, vz3);
    vst1_s16(dataptr+48, vz4);

    vlz1 = vhaddq_s32(vtmp11, vtmp1);   // tmp12 + tmp1
    vlz2 = vhsubq_s32(vtmp11, vtmp1);   // tmp12 - tmp1
    vlz3 = vhaddq_s32(vtmp10, vtmp0);   // tmp13 + tmp0
    vlz4 = vhsubq_s32(vtmp10, vtmp0);   // tmp13 - tmp0
    vz1 = vrshrn_n_s32(vlz1, 11);
    vz2 = vrshrn_n_s32(vlz2, 11);
    vz3 = vrshrn_n_s32(vlz3, 11);
    vz4 = vrshrn_n_s32(vlz4, 11);
    vst1_s16(dataptr+16, vz1);
    vst1_s16(dataptr+40, vz2);
    vst1_s16(dataptr+24, vz3);
    vst1_s16(dataptr+32, vz4);
}

// the same as line, only >>17 change to >> 12
static void transform_8x8_col(HI_S16 *in)
{
    register short *dataptr = in;

    int16x4_t src1 = vld1_s16(dataptr+8);
    int16x4_t src3 = vld1_s16(dataptr+24);
    int16x4_t src5 = vld1_s16(dataptr+40);
    int16x4_t src7 = vld1_s16(dataptr+56);
    int16x4_t src11 = vld1_s16(dataptr+12);
    int16x4_t src31 = vld1_s16(dataptr+28);
    int16x4_t src51 = vld1_s16(dataptr+44);
    int16x4_t src71 = vld1_s16(dataptr+60);


    int16x4_t vz1 = vadd_s16(src7, src1);
    int16x4_t vz2 = vadd_s16(src5, src3);
    int16x4_t vz3 = vadd_s16(src7, src3);
    int16x4_t vz4 = vadd_s16(src5, src1);
    int16x4_t vz5 = vadd_s16(vz3, vz4);
    int16x4_t vz11 = vadd_s16(src71, src11);
    int16x4_t vz21 = vadd_s16(src51, src31);
    int16x4_t vz31 = vadd_s16(src71, src31);
    int16x4_t vz41 = vadd_s16(src51, src11);
    int16x4_t vz51 = vadd_s16(vz31, vz41);


    int16x4_t con0 = vdup_n_s16 (FIX_0_899976223);
    int16x4_t con1 = vdup_n_s16 (FIX_2_562915447);
    int16x4_t con2 = vdup_n_s16 (FIX_1_175875602);
    int32x4_t vlz5 = vmull_s16(vz5, con2);
    int32x4_t vlz1 = vmull_s16(vz1, con0);
    int32x4_t vlz2 = vmull_s16(vz2, con1);
    int32x4_t vlz51 = vmull_s16(vz51, con2);
    int32x4_t vlz11 = vmull_s16(vz11, con0);
    int32x4_t vlz21 = vmull_s16(vz21, con1);


    int16x4_t con3 = vdup_n_s16 (FIX_1_961570560);
    int16x4_t con4 = vdup_n_s16 (FIX_0_390180644);
    int32x4_t vlz3 = vmlsl_s16(vlz5, vz3, con3);
    int32x4_t vlz4 = vmlsl_s16(vlz5, vz4, con4);
    int32x4_t vlz31 = vmlsl_s16(vlz51, vz31, con3);
    int32x4_t vlz41 = vmlsl_s16(vlz51, vz41, con4);

    con0 = vdup_n_s16 (FIX_0_298631336);
    con1 = vdup_n_s16 (FIX_2_053119869);
    con2 = vdup_n_s16 (FIX_3_072711026);
    con3 = vdup_n_s16 (FIX_1_501321110);

    int32x4_t vtmp0 = vsubq_s32(vlz3, vlz1);
    int32x4_t vtmp1 = vsubq_s32(vlz4, vlz2);
    int32x4_t vtmp2 = vsubq_s32(vlz3, vlz2);
    int32x4_t vtmp3 = vsubq_s32(vlz4, vlz1);
    int32x4_t vtmp01 = vsubq_s32(vlz31, vlz11);
    int32x4_t vtmp11 = vsubq_s32(vlz41, vlz21);
    int32x4_t vtmp21 = vsubq_s32(vlz31, vlz21);
    int32x4_t vtmp31 = vsubq_s32(vlz41, vlz11);

    vtmp0 = vmlal_s16(vtmp0, src7, con0);
    vtmp1 = vmlal_s16(vtmp1, src5, con1);
    vtmp2 = vmlal_s16(vtmp2, src3, con2);
    vtmp3 = vmlal_s16(vtmp3, src1, con3);
    vtmp01 = vmlal_s16(vtmp01, src71, con0);
    vtmp11 = vmlal_s16(vtmp11, src51, con1);
    vtmp21 = vmlal_s16(vtmp21, src31, con2);
    vtmp31 = vmlal_s16(vtmp31, src11, con3);

    con0 = vdup_n_s16 (FIX_0_541196100);
    src1 = vld1_s16(dataptr);
    src5 = vld1_s16(dataptr+32);
    src3 = vld1_s16(dataptr+16);
    src7 = vld1_s16(dataptr+48);
    vz1  = vadd_s16(src3, src7);
    vlz1 = vmull_s16(vz1, con0);  //int32x4_t  vmull_s16(int16x4_t a, int16x4_t b);   y = a * b

    src11 = vld1_s16(dataptr+4);
    src51 = vld1_s16(dataptr+36);
    src31 = vld1_s16(dataptr+20);
    src71 = vld1_s16(dataptr+52);
    vz11  = vadd_s16(src31, src71);
    vlz11 = vmull_s16(vz11, con0);

    //int32x4_t  vaddl_s16(int16x4_t a, int16x4_t b);
    int32x4_t vtmp4 = vaddl_s16(src1, src5);
    int32x4_t vtmp5 = vsubl_s16(src1, src5);
    int32x4_t vtmp101 = vaddl_s16(src11, src51);
    int32x4_t vtmp111 = vsubl_s16(src11, src51);

// int32x4_t  vmlal_s16(int32x4_t a, int16x4_t b, int16x4_t c);   y = a + b * c
// int32x4_t  vmlsl_s16(int32x4_t a, int16x4_t b, int16x4_t c);   y = a - b * c
    con1 = vdup_n_s16 (FIX_1_847759065);
    con2 = vdup_n_s16 (FIX_0_765366865);
    vlz5 =  vdupq_n_s32 (2048);
    int32x4_t vtmp6 = vmlsl_s16(vlz1, src7, con1);
    int32x4_t vtmp7 = vmlal_s16(vlz1, src3, con2);
    vtmp4 = vshlq_n_s32(vtmp4, 13);
    vtmp5 = vshlq_n_s32(vtmp5, 13);

    int32x4_t vtmp121 = vmlsl_s16(vlz11, src71, con1);
    int32x4_t vtmp131 = vmlal_s16(vlz11, src31, con2);
    vtmp101 = vshlq_n_s32(vtmp101, 13);
    vtmp111 = vshlq_n_s32(vtmp111, 13);


    vtmp4 = vsubq_s32(vtmp4, vtmp7);   // tmp13
    vtmp7 = vaddq_s32(vtmp7, vtmp7);
    vtmp7 = vaddq_s32(vtmp7, vtmp4);   // tmp10
    vtmp5 = vsubq_s32(vtmp5, vtmp6);   // tmp12
    vtmp6 = vaddq_s32(vtmp6, vtmp6);
    vtmp6 = vaddq_s32(vtmp6, vtmp5);   // tmp11

    vtmp101 = vsubq_s32(vtmp101, vtmp131);   // tmp13
    vtmp131 = vaddq_s32(vtmp131, vtmp131);
    vtmp131 = vaddq_s32(vtmp131, vtmp101);   // tmp10
    vtmp111 = vsubq_s32(vtmp111, vtmp121);   // tmp12
    vtmp121 = vaddq_s32(vtmp121, vtmp121);
    vtmp121 = vaddq_s32(vtmp121, vtmp111);   // tmp11


    vlz1 = vhaddq_s32(vtmp7, vtmp3);   // tmp10 + tmp3
    vlz2 = vhsubq_s32(vtmp7, vtmp3);   // tmp10 - tmp3
    vlz3 = vhaddq_s32(vtmp6, vtmp2);   // tmp11 + tmp2
    vlz4 = vhsubq_s32(vtmp6, vtmp2);   // tmp11 - tmp2
    vlz11 = vhaddq_s32(vtmp131, vtmp31);   // tmp10 + tmp3
    vlz21 = vhsubq_s32(vtmp131, vtmp31);   // tmp10 - tmp3
    vlz31 = vhaddq_s32(vtmp121, vtmp21);   // tmp11 + tmp2
    vlz41 = vhsubq_s32(vtmp121, vtmp21);   // tmp11 - tmp2

    vz1 = vrshrn_n_s32(vlz1, 11);
    vz2 = vrshrn_n_s32(vlz2, 11);
    vz3 = vrshrn_n_s32(vlz3, 11);
    vz4 = vrshrn_n_s32(vlz4, 11);
    vz11 = vrshrn_n_s32(vlz11, 11);
    vz21 = vrshrn_n_s32(vlz21, 11);
    vz31 = vrshrn_n_s32(vlz31, 11);
    vz41 = vrshrn_n_s32(vlz41, 11);

    vst1_s16(dataptr, vz1);
    vst1_s16(dataptr+56, vz2);
    vst1_s16(dataptr+8, vz3);
    vst1_s16(dataptr+48, vz4);
    vst1_s16(dataptr+4, vz11);
    vst1_s16(dataptr+60, vz21);
    vst1_s16(dataptr+12, vz31);
    vst1_s16(dataptr+52, vz41);

    vlz1 = vhaddq_s32(vtmp5, vtmp1);   // tmp12 + tmp1
    vlz2 = vhsubq_s32(vtmp5, vtmp1);   // tmp12 - tmp1
    vlz3 = vhaddq_s32(vtmp4, vtmp0);   // tmp13 + tmp0
    vlz4 = vhsubq_s32(vtmp4, vtmp0);   // tmp13 - tmp0
    vlz11 = vhaddq_s32(vtmp111, vtmp11);   // tmp12 + tmp1
    vlz21 = vhsubq_s32(vtmp111, vtmp11);   // tmp12 - tmp1
    vlz31 = vhaddq_s32(vtmp101, vtmp01);   // tmp13 + tmp0
    vlz41 = vhsubq_s32(vtmp101, vtmp01);   // tmp13 - tmp0

    vz1 = vrshrn_n_s32(vlz1, 11);
    vz2 = vrshrn_n_s32(vlz2, 11);
    vz3 = vrshrn_n_s32(vlz3, 11);
    vz4 = vrshrn_n_s32(vlz4, 11);
    vz11 = vrshrn_n_s32(vlz11, 11);
    vz21 = vrshrn_n_s32(vlz21, 11);
    vz31 = vrshrn_n_s32(vlz31, 11);
    vz41 = vrshrn_n_s32(vlz41, 11);

    vst1_s16(dataptr+16, vz1);
    vst1_s16(dataptr+40, vz2);
    vst1_s16(dataptr+24, vz3);
    vst1_s16(dataptr+32, vz4);
    vst1_s16(dataptr+20, vz11);
    vst1_s16(dataptr+44, vz21);
    vst1_s16(dataptr+28, vz31);
    vst1_s16(dataptr+36, vz41);


}

static void transform_8x8_line(HI_S16 *in, HI_U8 *out, HI_U32 stride)
{
    register short *dataptr = in;

    int16x4_t src1 = vld1_s16(dataptr+8);
    int16x4_t src3 = vld1_s16(dataptr+24);
    int16x4_t src5 = vld1_s16(dataptr+40);
    int16x4_t src7 = vld1_s16(dataptr+56);
    int16x4_t src11 = vld1_s16(dataptr+12);
    int16x4_t src31 = vld1_s16(dataptr+28);
    int16x4_t src51 = vld1_s16(dataptr+44);
    int16x4_t src71 = vld1_s16(dataptr+60);

    int16x4_t vz1 = vadd_s16(src7, src1);
    int16x4_t vz2 = vadd_s16(src5, src3);
    int16x4_t vz3 = vadd_s16(src7, src3);
    int16x4_t vz4 = vadd_s16(src5, src1);
    int16x4_t vz5 = vadd_s16(vz3, vz4);
    int16x4_t vz11 = vadd_s16(src71, src11);
    int16x4_t vz21 = vadd_s16(src51, src31);
    int16x4_t vz31 = vadd_s16(src71, src31);
    int16x4_t vz41 = vadd_s16(src51, src11);
    int16x4_t vz51 = vadd_s16(vz31, vz41);


    int16x4_t con0 = vdup_n_s16 (FIX_0_899976223);
    int16x4_t con1 = vdup_n_s16 (FIX_2_562915447);
    int16x4_t con2 = vdup_n_s16 (FIX_1_175875602);
    int32x4_t vlz5 = vmull_s16(vz5, con2);
    int32x4_t vlz1 = vmull_s16(vz1, con0);
    int32x4_t vlz2 = vmull_s16(vz2, con1);
    int32x4_t vlz51 = vmull_s16(vz51, con2);
    int32x4_t vlz11 = vmull_s16(vz11, con0);
    int32x4_t vlz21 = vmull_s16(vz21, con1);

    int16x4_t con3 = vdup_n_s16 (FIX_1_961570560);
    int16x4_t con4 = vdup_n_s16 (FIX_0_390180644);
    int32x4_t vlz3 = vmlsl_s16(vlz5, vz3, con3);
    int32x4_t vlz4 = vmlsl_s16(vlz5, vz4, con4);
    int32x4_t vlz31 = vmlsl_s16(vlz51, vz31, con3);
    int32x4_t vlz41 = vmlsl_s16(vlz51, vz41, con4);

    con0 = vdup_n_s16 (FIX_0_298631336);
    con1 = vdup_n_s16 (FIX_2_053119869);
    con2 = vdup_n_s16 (FIX_3_072711026);
    con3 = vdup_n_s16 (FIX_1_501321110);

    int32x4_t vtmp0 = vsubq_s32(vlz3, vlz1);
    int32x4_t vtmp1 = vsubq_s32(vlz4, vlz2);
    int32x4_t vtmp2 = vsubq_s32(vlz3, vlz2);
    int32x4_t vtmp3 = vsubq_s32(vlz4, vlz1);
    int32x4_t vtmp01 = vsubq_s32(vlz31, vlz11);
    int32x4_t vtmp11 = vsubq_s32(vlz41, vlz21);
    int32x4_t vtmp21 = vsubq_s32(vlz31, vlz21);
    int32x4_t vtmp31 = vsubq_s32(vlz41, vlz11);

    vtmp0 = vmlal_s16(vtmp0, src7, con0);
    vtmp1 = vmlal_s16(vtmp1, src5, con1);
    vtmp2 = vmlal_s16(vtmp2, src3, con2);
    vtmp3 = vmlal_s16(vtmp3, src1, con3);
    vtmp01 = vmlal_s16(vtmp01, src71, con0);
    vtmp11 = vmlal_s16(vtmp11, src51, con1);
    vtmp21 = vmlal_s16(vtmp21, src31, con2);
    vtmp31 = vmlal_s16(vtmp31, src11, con3);

    con0 = vdup_n_s16 (FIX_0_541196100);
    src1 = vld1_s16(dataptr);
    src5 = vld1_s16(dataptr+32);
    src3 = vld1_s16(dataptr+16);
    src7 = vld1_s16(dataptr+48);
    vz1  = vadd_s16(src3, src7);
    vlz1 = vmull_s16(vz1, con0);                              //int32x4_t  vmull_s16(int16x4_t a, int16x4_t b);   y = a * b

    src11 = vld1_s16(dataptr+4);
    src51 = vld1_s16(dataptr+36);
    src31 = vld1_s16(dataptr+20);
    src71 = vld1_s16(dataptr+52);
    vz11  = vadd_s16(src31, src71);
    vlz11 = vmull_s16(vz11, con0);

    //int32x4_t  vaddl_s16(int16x4_t a, int16x4_t b);
    int32x4_t vtmp4 = vaddl_s16(src1, src5);
    int32x4_t vtmp5 = vsubl_s16(src1, src5);
    int32x4_t vtmp41 = vaddl_s16(src11, src51);
    int32x4_t vtmp51 = vsubl_s16(src11, src51);

// int32x4_t  vmlal_s16(int32x4_t a, int16x4_t b, int16x4_t c);   y = a + b * c
// int32x4_t  vmlsl_s16(int32x4_t a, int16x4_t b, int16x4_t c);   y = a - b * c
    con1 = vdup_n_s16 (FIX_1_847759065);
    con2 = vdup_n_s16 (FIX_0_765366865);
    vlz5 =  vdupq_n_s32 (65536);
    int32x4_t vtmp6 = vmlsl_s16(vlz1, src7, con1);
    int32x4_t vtmp7 = vmlal_s16(vlz1, src3, con2);
    int32x4_t vtmp61 = vmlsl_s16(vlz11, src71, con1);
    int32x4_t vtmp71 = vmlal_s16(vlz11, src31, con2);

    vtmp4 = vshlq_n_s32(vtmp4, 13);
    vtmp5 = vshlq_n_s32(vtmp5, 13);
    vtmp41 = vshlq_n_s32(vtmp41, 13);
    vtmp51 = vshlq_n_s32(vtmp51, 13);


    vtmp4 = vsubq_s32(vtmp4, vtmp7);   // tmp13
    vtmp7 = vaddq_s32(vtmp7, vtmp7);
    vtmp7 = vaddq_s32(vtmp7, vtmp4);   // tmp10
    vtmp5 = vsubq_s32(vtmp5, vtmp6);   // tmp12
    vtmp6 = vaddq_s32(vtmp6, vtmp6);
    vtmp6 = vaddq_s32(vtmp6, vtmp5);   // tmp11

    vtmp41 = vsubq_s32(vtmp41, vtmp71);   // tmp13
    vtmp71 = vaddq_s32(vtmp71, vtmp71);
    vtmp71 = vaddq_s32(vtmp71, vtmp41);   // tmp10
    vtmp51 = vsubq_s32(vtmp51, vtmp61);   // tmp12
    vtmp61 = vaddq_s32(vtmp61, vtmp61);
    vtmp61 = vaddq_s32(vtmp61, vtmp51);   // tmp11

    vlz1 = vhaddq_s32(vtmp7, vtmp3);   // tmp10 + tmp3
    vlz2 = vhsubq_s32(vtmp7, vtmp3);   // tmp10 - tmp3
    vlz3 = vhaddq_s32(vtmp6, vtmp2);   // tmp11 + tmp2
    vlz4 = vhsubq_s32(vtmp6, vtmp2);   // tmp11 - tmp2
    vlz11 = vhaddq_s32(vtmp71, vtmp31);   // tmp10 + tmp3
    vlz21 = vhsubq_s32(vtmp71, vtmp31);   // tmp10 - tmp3
    vlz31 = vhaddq_s32(vtmp61, vtmp21);   // tmp11 + tmp2
    vlz41 = vhsubq_s32(vtmp61, vtmp21);   // tmp11 - tmp2


    vz1 = vrshrn_n_s32(vlz1, 16);
    vz2 = vrshrn_n_s32(vlz2, 16);
    vz3 = vrshrn_n_s32(vlz3, 16);
    vz4 = vrshrn_n_s32(vlz4, 16);
    vz11 = vrshrn_n_s32(vlz11, 16);
    vz21 = vrshrn_n_s32(vlz21, 16);
    vz31 = vrshrn_n_s32(vlz31, 16);
    vz41 = vrshrn_n_s32(vlz41, 16);

//int16x8_t   vcombine_s16(int16x4_t low, int16x4_t high);     // VMOV d0,d0
    int16x8_t  con51     =  vdupq_n_s16 (128);
    int16x8_t  vcom0 = vcombine_s16(vz1, vz11);
    int16x8_t  vcom1 = vcombine_s16(vz3, vz31);
    int16x8_t  vcom2 = vcombine_s16(vz4, vz41);
    int16x8_t  vcom3 = vcombine_s16(vz2, vz21);

    vcom0 = vaddq_s16(vcom0, con51);
    vcom1 = vaddq_s16(vcom1, con51);
    vcom2 = vaddq_s16(vcom2, con51);
    vcom3 = vaddq_s16(vcom3, con51);

    uint8x8_t dst0 = vqmovun_s16(vcom0);
    uint8x8_t dst1 = vqmovun_s16(vcom1);
    vst1_u8(out, dst0);
    vst1_u8(out+stride, dst1);
    out += stride << 1;

    //vst1q_s16(dataptr,    vcom0);
    //vst1q_s16(dataptr+8, vcom1);
    //vst1q_s16(dataptr+48, vcom2);
    //vst1q_s16(dataptr+56, vcom3);


    vlz1 = vhaddq_s32(vtmp5, vtmp1);   // tmp12 + tmp1
    vlz2 = vhsubq_s32(vtmp5, vtmp1);   // tmp12 - tmp1
    vlz3 = vhaddq_s32(vtmp4, vtmp0);   // tmp13 + tmp0
    vlz4 = vhsubq_s32(vtmp4, vtmp0);   // tmp13 - tmp0
    vlz11 = vhaddq_s32(vtmp51, vtmp11);   // tmp12 + tmp1
    vlz21 = vhsubq_s32(vtmp51, vtmp11);   // tmp12 - tmp1
    vlz31 = vhaddq_s32(vtmp41, vtmp01);   // tmp13 + tmp0
    vlz41 = vhsubq_s32(vtmp41, vtmp01);   // tmp13 - tmp0
    vz1 = vrshrn_n_s32(vlz1, 16);
    vz2 = vrshrn_n_s32(vlz2, 16);
    vz3 = vrshrn_n_s32(vlz3, 16);
    vz4 = vrshrn_n_s32(vlz4, 16);
    vz11 = vrshrn_n_s32(vlz11, 16);
    vz21 = vrshrn_n_s32(vlz21, 16);
    vz31 = vrshrn_n_s32(vlz31, 16);
    vz41 = vrshrn_n_s32(vlz41, 16);

    vcom0 = vcombine_s16(vz1, vz11);
    vcom1 = vcombine_s16(vz3, vz31);
    vcom0 = vaddq_s16(vcom0, con51);
    vcom1 = vaddq_s16(vcom1, con51);
    dst0 = vqmovun_s16(vcom0);
    dst1 = vqmovun_s16(vcom1);
    vst1_u8(out, dst0);
    vst1_u8(out+stride, dst1);
    out += stride << 1;

    vcom0 = vcombine_s16(vz4, vz41);
    vcom1 = vcombine_s16(vz2, vz21);
    vcom0 = vaddq_s16(vcom0, con51);
    vcom1 = vaddq_s16(vcom1, con51);
    dst0 = vqmovun_s16(vcom0);
    dst1 = vqmovun_s16(vcom1);
    vst1_u8(out, dst0);
    vst1_u8(out+stride, dst1);
    out += stride << 1;

    dst0 = vqmovun_s16(vcom2);
    dst1 = vqmovun_s16(vcom3);
    vst1_u8(out, dst0);
    vst1_u8(out+stride, dst1);
    //vst1q_s16(dataptr+16, vcom0);
    //vst1q_s16(dataptr+24, vcom1);
    //vst1q_s16(dataptr+32, vcom2);
    //vst1q_s16(dataptr+40, vcom3);



}
#endif

void jpeg_idct_fix_c(HI_S16 *in, HI_U8 *out, HI_U32 stride)
{
  int tmp0, tmp1, tmp2, tmp3;
  int tmp10, tmp11, tmp12, tmp13;
  int z1, z2, z3, z4, z5;
  int d0, d1, d2, d3, d4, d5, d6, d7;
  register short *dataptr;
  int i, k;
  dataptr = in;


    //transpose8x8(dataptr);
    //transform_8x8_col(dataptr);
    //transpose8x8(dataptr);


  for (k=0; k<DCTSIZE; k++)
  {
    d0 = dataptr[0];
    d1 = dataptr[1];
    d2 = dataptr[2];
    d3 = dataptr[3];
    d4 = dataptr[4];
    d5 = dataptr[5];
    d6 = dataptr[6];
    d7 = dataptr[7];


    {
        z1   = (d2 + d6) * FIX_0_541196100;
    	tmp2 = z1 - d6 * FIX_1_847759065;
        tmp3 = z1 + d2 * FIX_0_765366865;
        tmp0 = (d0 + d4) << CONST_BITS;
        tmp1 = (d0 - d4) << CONST_BITS;

        tmp10 = tmp0 + tmp3;
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

        z1 = (d7 + d1) * FIX_0_899976223;
        z2 = (d5 + d3) * FIX_2_562915447;
        z3 = d7 + d3;
        z4 = d5 + d1;
        z5 = (z3 + z4) * FIX_1_175875602;

        tmp0 = d7 * FIX_0_298631336;
        tmp1 = d5 * FIX_2_053119869;
        tmp2 = d3 * FIX_3_072711026;
        tmp3 = d1 * FIX_1_501321110;

        z3 = z5 - z3 * FIX_1_961570560;
        z4 = z5 - z4 * FIX_0_390180644;

        tmp0 += z3 - z1;
        tmp1 += z4 - z2;
        tmp2 += z3 - z2;
        tmp3 += z4 - z1;
    }
if (1)
{
	dataptr[0] = (short) ((tmp10 + tmp3 + 2048) >> 12);
	dataptr[7] = (short) ((tmp10 - tmp3 + 2048) >> 12);
	dataptr[1] = (short) ((tmp11 + tmp2 + 2048) >> 12);
	dataptr[6] = (short) ((tmp11 - tmp2 + 2048) >> 12);
	dataptr[2] = (short) ((tmp12 + tmp1 + 2048) >> 12);
	dataptr[5] = (short) ((tmp12 - tmp1 + 2048) >> 12);
	dataptr[3] = (short) ((tmp13 + tmp0 + 2048) >> 12);
	dataptr[4] = (short) ((tmp13 - tmp0 + 2048) >> 12);
}
    dataptr += DCTSIZE;         /* advance pointer to next row */
  }

  dataptr = in;

  for (k=0; k<DCTSIZE; k++)
  {
    d0 = dataptr[DCTSIZE*0];
    d1 = dataptr[DCTSIZE*1];
    d2 = dataptr[DCTSIZE*2];
    d3 = dataptr[DCTSIZE*3];
    d4 = dataptr[DCTSIZE*4];
    d5 = dataptr[DCTSIZE*5];
    d6 = dataptr[DCTSIZE*6];
    d7 = dataptr[DCTSIZE*7];

    z1   = (d2 + d6) * FIX_0_541196100;
    tmp2 = z1 - d6 * FIX_1_847759065;
    tmp3 = z1 + d2 * FIX_0_765366865;
    tmp0 = (d0 + d4) << CONST_BITS;
    tmp1 = (d0 - d4) << CONST_BITS;

    tmp10 = tmp0 + tmp3;
    tmp13 = tmp0 - tmp3;
    tmp11 = tmp1 + tmp2;
    tmp12 = tmp1 - tmp2;


    z1 = (d7 + d1) * FIX_0_899976223;
    z2 = (d5 + d3) * FIX_2_562915447;
    z3 = d7 + d3;
    z4 = d5 + d1;
    z5 = (z3 + z4) * FIX_1_175875602;

    tmp0 = d7 * FIX_0_298631336;
    tmp1 = d5 * FIX_2_053119869;
    tmp2 = d3 * FIX_3_072711026;
    tmp3 = d1 * FIX_1_501321110;

    z3 = z5 - z3 * FIX_1_961570560;
    z4 = z5 - z4 * FIX_0_390180644;

    tmp0 += z3 - z1;
    tmp1 += z4 - z2;
    tmp2 += z3 - z2;
    tmp3 += z4 - z1;
if(1)
{
	dataptr[DCTSIZE*0] = (short) ((tmp10 + tmp3 + 65536) >> 17);
	dataptr[DCTSIZE*7] = (short) ((tmp10 - tmp3 + 65536) >> 17);
	dataptr[DCTSIZE*1] = (short) ((tmp11 + tmp2 + 65536) >> 17);
	dataptr[DCTSIZE*6] = (short) ((tmp11 - tmp2 + 65536) >> 17);
	dataptr[DCTSIZE*2] = (short) ((tmp12 + tmp1 + 65536) >> 17);
	dataptr[DCTSIZE*5] = (short) ((tmp12 - tmp1 + 65536) >> 17);
	dataptr[DCTSIZE*3] = (short) ((tmp13 + tmp0 + 65536) >> 17);
	dataptr[DCTSIZE*4] = (short) ((tmp13 - tmp0 + 65536) >> 17);
}
//if(k==0 && (dataptr[DCTSIZE*0] != qqq[0] || dataptr[DCTSIZE*7] != qqq[4]))
//    printf("%d %d---> %d, %d\n", dataptr[DCTSIZE*0], dataptr[DCTSIZE*7], qqq[0], qqq[4]);

    dataptr++;
  }

    //jpeg_add128_neon(in, out, stride);

#if 1
    dataptr = in;
    for (i=0; i<8; i++)
    {
        tmp0 = dataptr[i*8+ 0] + 128; if (tmp0 > 255) tmp0 =  255;  if(tmp0 < 0) tmp0 = 0;  out[0] = tmp0;
        tmp0 = dataptr[i*8+ 1] + 128; if (tmp0 > 255) tmp0 =  255;  if(tmp0 < 0) tmp0 = 0;  out[1] = tmp0;
        tmp0 = dataptr[i*8+ 2] + 128; if (tmp0 > 255) tmp0 =  255;  if(tmp0 < 0) tmp0 = 0;  out[2] = tmp0;
        tmp0 = dataptr[i*8+ 3] + 128; if (tmp0 > 255) tmp0 =  255;  if(tmp0 < 0) tmp0 = 0;  out[3] = tmp0;
        tmp0 = dataptr[i*8+ 4] + 128; if (tmp0 > 255) tmp0 =  255;  if(tmp0 < 0) tmp0 = 0;  out[4] = tmp0;
        tmp0 = dataptr[i*8+ 5] + 128; if (tmp0 > 255) tmp0 =  255;  if(tmp0 < 0) tmp0 = 0;  out[5] = tmp0;
        tmp0 = dataptr[i*8+ 6] + 128; if (tmp0 > 255) tmp0 =  255;  if(tmp0 < 0) tmp0 = 0;  out[6] = tmp0;
        tmp0 = dataptr[i*8+ 7] + 128; if (tmp0 > 255) tmp0 =  255;  if(tmp0 < 0) tmp0 = 0;  out[7] = tmp0;

        out += stride;
    }
#endif
}

#if SUPPORT_NEON
void jpeg_idct_fix_neon(HI_S16 *in, HI_U8 *out, HI_U32 stride)
{
    register short *dataptr = in;

    transpose8x8(dataptr);

    transform_8x8_col(dataptr);
//    transform_4x8_col(dataptr);
//    transform_4x8_col(dataptr+4);

    transpose8x8(dataptr);

    transform_8x8_line(dataptr, out, stride);
//    transform_4x8_line(dataptr+4);

    //jpeg_add128_neon(in, out, stride);
}
#endif




