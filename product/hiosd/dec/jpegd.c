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
#endif /* __cplusplus */




#include "jpegd_image.h"
#include "jpegd_drv.h"


extern unsigned long hilogo;
extern unsigned long jpeg_size;
extern unsigned long VIDEO_DATA_BASE;
extern unsigned long JPEGD_EMAR_BUF;
extern unsigned int output_format;
static const HI_U8 zigzagforQtable[64] =
{
     0,  1,  8, 16,  9,  2,  3, 10, 17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63,
};


MotionJpegObj g_JpegdCtx;


/*****************************************************************************
 Prototype    : JPEGD_DecFrame
 Description  : decode jpeg picture
 Input        : JPEGD_HANDLE handle,HI_U32 uFlags
 Output       : HI_S32
 Return Value :
 Calls        :
*****************************************************************************/
HI_S32 JPEGD_DecFrame(JPEGD_HANDLE Handle,HI_U32 uFlags)
{
    MotionJpegObj *pJpegdHdlCtx = (MotionJpegObj *)Handle;
    HI_U32 i, jpeg_flag = 0;
    HI_S32 result = 0;
    HI_U32 type;
    HI_U8 *pStream;
    HI_U32 len;
    HI_U64 ullPTS;
    MJPEG_DEC_FRAME_S *pDecFrame, stDecFrame;
    HI_BOOL bQuantTableValid   = HI_FALSE;
    HI_BOOL bHuffmanTableValid = HI_FALSE;

    if( NULL == pJpegdHdlCtx)
    {
        HI_TRACE( "jpegd handle is null!\n");
        return HI_MJPEG_ERR_HANDLE;
    }

    pStream = pJpegdHdlCtx->stStream.pu8VirAddr;
    len     = pJpegdHdlCtx->stStream.u32Len;
    ullPTS  = pJpegdHdlCtx->stStream.u64PTS;
    pDecFrame = &stDecFrame;

    if(NULL == pStream)
    {
        HI_TRACE( "jpegd stream VirAddr is null!\n");
        return HI_MJPEG_ERR_HANDLE;
    }

    pDecFrame->ErrorCode = 0;
    pDecFrame->bError = 0;
    pJpegdHdlCtx->Frame.RestartInterval = 0;

    for(i=0; i<len; )
    {
        type = 0xFF;

        while( i<len && 0xff != *(pStream++) )
        {
            i++;
        }
        while( i<len && 0xff == (type = *(pStream++)) )
        {
            i++;
        }

        switch(type)
        {
            case BASELINE:  // sequential DCT
            {
                if( jpeg_flag == 1 )
                {
                    HI_S32 tmp_len = (pStream[0]<<8) + pStream[1];
                    tmp_len = MIN(tmp_len, len - i);
                    pJpegdHdlCtx->StreamBuffer = pStream;
                    i       += (tmp_len + 2);
                    pStream += (tmp_len );
                    result = DecodeSOF0(pJpegdHdlCtx, pJpegdHdlCtx->StreamBuffer, tmp_len);
                    if( HI_JPEG_DEC_OK == result )
                    {
                        jpeg_flag = 2;
                    }
                    else
                    {
                        pDecFrame->ErrorCode = result;
                        HI_TRACE( "jpeg decode DCT error!\n");
                        goto END;
                    }
                }
            }
            break;

            case DHT:  // DHT
            {
                HI_U32 tmp_len = (pStream[0]<<8) + pStream[1];
                tmp_len = MIN(tmp_len, len - i);
                pJpegdHdlCtx->StreamBuffer = pStream;
                i       += (tmp_len + 2);
                pStream += (tmp_len);

                if(DecodeDHT(pJpegdHdlCtx, pJpegdHdlCtx->StreamBuffer, tmp_len))
                {
                    pDecFrame->ErrorCode = HI_ERR_HUFFMAN_TABLE;

                    HI_TRACE( "jpeg decode DHT error!\n");
                    goto END;
                }
                bHuffmanTableValid = HI_TRUE;
            }
            break;

            case SOS:  // scan header
            {
                if( jpeg_flag == 2 )
                {
                    HI_S32 tmp_len = (pStream[0]<<8) + pStream[1];
                    tmp_len = MIN(tmp_len, len - i);
                    pJpegdHdlCtx->StreamBuffer = pStream;
                    i       += (tmp_len);
                    pStream += (tmp_len);
                    pJpegdHdlCtx->FirstMCU = 0;
                    result = DecodeSOS( pJpegdHdlCtx, pJpegdHdlCtx->StreamBuffer, tmp_len );

                    if( HI_JPEG_DEC_OK == result )
                    {
                        jpeg_flag = 3; // scan header decoding success
                        goto END;
                    }
                    else
                    {
                        HI_TRACE( "jpeg decode SOS error!\n");
                        goto END; // scan header decoding error
                    }
                }
            }
            break;

            case SOI:  // a new jpeg picture
            {
                i += 2;
                jpeg_flag = 1;
            }
            break;

            case EOI:  // end of jpeg picture
            {
                i += 2;
                goto END;
            }

            case DQT:
            {
                {
                    HI_S32 tmp_len = (pStream[0]<<8) + pStream[1];
                    tmp_len = MIN(tmp_len, len - i);
                    pJpegdHdlCtx->StreamBuffer = pStream;
                    i       += (tmp_len + 2);
                    pStream += (tmp_len);

                    if(DecodeDQT(pJpegdHdlCtx, pJpegdHdlCtx->StreamBuffer, tmp_len))
                    {
                        pDecFrame->ErrorCode = HI_ERR_QUANT_TABLE;

                        HI_TRACE( "jpeg decode DQT error!\n");
                        goto END;
                    }

                    bQuantTableValid = HI_TRUE;
                }
            }
            break;

            case DNL:
            {
                i += 6;
                pStream  += 4;
                jpeg_flag = 0;
            }
            break;

            case DRI:
            {
                pJpegdHdlCtx->StreamBuffer[0] = pStream[0];
                pJpegdHdlCtx->StreamBuffer[1] = pStream[1];
                pJpegdHdlCtx->StreamBuffer[2] = pStream[2];
                pJpegdHdlCtx->StreamBuffer[3] = pStream[3];
                if(DecodeDRI(pJpegdHdlCtx, pJpegdHdlCtx->StreamBuffer, 4))
                {
                    jpeg_flag = 0;
                    pDecFrame->ErrorCode = HI_ERR_RESTART_ERROR;
                    HI_TRACE( "jpeg decode DRI error!\n");
                    goto END;
                }
                i += 6;
                pStream += 4;
            }
            break;

            default:    // do not support
            {
                if( (type & 0xE0) == 0xE0 )
                {
                    int tmp_len = (pStream[0]<<8) + pStream[1];
                    i       += (tmp_len + 2);
                    pStream += (tmp_len);

                    if ((type >= 0xE0) && (type <= 0xEF))
                    {
                        //HI_TRACE( "Get an App%d, lenth: %d\n", type&0xf, tmp_len + 2);
                    }
                    else if ((type >= 0xE0) && (type <= 0xEF))
                    {
                        //HI_TRACE( "Get a JPG%d, length: %d\n", type&0xf, tmp_len + 2);
                    }
                    else
                    {
                        //HI_TRACE( "Get a COM, length: %d\n", tmp_len + 2);
                    }
                }
                else if( (type & 0xF0) == 0xC0 )
                {
                    jpeg_flag = 0;
                    pDecFrame->ErrorCode = HI_ERR_NOT_BASELINE;
                    HI_TRACE( "jpeg decode error, unsupport type!\n");
                    goto END;
                }
                else
                {
                    //HI_TRACE( "Unknown Mark! \n");
                    i += 2;
                }
            }
            break;
        }
    }

END:
    if( 3 == jpeg_flag )
    {
        pJpegdHdlCtx->u32StreamOffest = i+2;
        pDecFrame->uWidth         = pJpegdHdlCtx->Frame.yWidth;
        pDecFrame->uHeight        = pJpegdHdlCtx->Frame.yHeight;
        pDecFrame->uYStride       = pJpegdHdlCtx->yStride;
        pDecFrame->uCStride       = pJpegdHdlCtx->cStride;
        pDecFrame->uPictureFormat = pJpegdHdlCtx->enPictureFormat;
        pDecFrame->ullPTS         = ullPTS;
        pDecFrame->reserved       = 0;
        pDecFrame->pUserData      = NULL;
        pJpegdHdlCtx->ValidFrame  ^= 0x1;      //  change to another buffer

        if (bQuantTableValid == HI_FALSE)
        {
            InitDefaultQuantTable(pJpegdHdlCtx);
        }
        if (bHuffmanTableValid == HI_FALSE)
        {
            InitDefaultHuffmanTable(pJpegdHdlCtx);
        }
        return HI_MJPEG_DEC_OK;
    }
    else
    {
        pDecFrame->pY             = NULL;
        pDecFrame->pU             = NULL;
        pDecFrame->pV             = NULL;
        pDecFrame->uWidth         = 0;
        pDecFrame->uHeight        = 0;
        pDecFrame->uYStride       = 0;
        pDecFrame->uCStride       = 0;
        pDecFrame->uPictureFormat = PICTURE_FORMAT_BUTT;
        pDecFrame->ullPTS         = 0;
        pDecFrame->reserved       = 0;
        pDecFrame->pUserData      = NULL;

        return HI_MJPEG_NO_PICTURE;
    }
}




/*****************************************************************************
 Prototype    : JPEGD_GetHandle
 Description  : create a JPEG decoder handle
 Input        : JPEGD_HANDLE handle,HI_U32 uFlags
 Output       : HI_S32
 Return Value :
 Calls        :
 Called By    :
 Last Modified: 2016-10-15
*****************************************************************************/

JPEGD_HANDLE JPEGD_GetHandle(HI_VOID)
{
    MotionJpegObj *mjpeg = &g_JpegdCtx;
    HI_S32 PicSize;

    mjpeg->MaxWidth  = JPEGD_MAX_WIDTH;
    mjpeg->MaxHeight = JPEGD_MAX_HEIGHT;

    PicSize = mjpeg->MaxWidth * mjpeg->MaxHeight;
    mjpeg->BufSize = PicSize + 0x10000;
    mjpeg->StreamBuffer    = NULL;
    mjpeg->PicBuffer       = NULL;
    mjpeg->ValidFrame      = 0;
    mjpeg->WidthInMCU      = 1;
    mjpeg->HeightInMCU     = 0;
    mjpeg->yStride         = 0;
    mjpeg->cStride         = 0;
    mjpeg->enPictureFormat = PICTURE_FORMAT_BUTT;
    mjpeg->FirstMCU        = 0;
    mjpeg->Bits.BitLen     = 0;
    mjpeg->Bits.BitOffset  = 0;
    mjpeg->Bits.pBuffer    = NULL;
    mjpeg->Frame.Nf                    = 0;
    mjpeg->Frame.yHeight               = 0;
    mjpeg->Frame.yWidth                = 0;
    mjpeg->Frame.RestartInterval       = 0;
    mjpeg->Frame.RestartIntervalLogic  = 0;
    mjpeg->Frame.MaxMCUNumber          = 0;

    InitDefaultHuffmanTable( mjpeg );
    InitDefaultQuantTable( mjpeg );

    mjpeg->enState = STATE_IDLE;

    return (JPEGD_HANDLE)mjpeg;
}






/****************************************************************************
* Function Name : JPEGD_PrepareHardInfo
* Description   : Prepare the jpegd Hardware Info
* Parameters    : handle pRegBase
* Return Type   : HI_SUCCESS or error code
****************************************************************************/
HI_S32  JPEGD_PrepareHardwareInfo(MotionJpegObj *pJpegHdlCtx)
{
    HI_U32 i, u32Ysize;
    HI_U32 u32HuffmanTable[12] = {0};
    HI_U8 LumaAcMinTable[16] = {0};
    HI_U8 LumaAcBaseTable[16] = {0};
    HI_U8 ChromaAcMinTable[16] = {0};
    HI_U8 ChromaAcBaseTable[16] = {0};

    u32Ysize = ALIGN_UP(pJpegHdlCtx->Frame.yWidth,64)*ALIGN_UP(pJpegHdlCtx->Frame.yHeight,16);
    pJpegHdlCtx->stVpuConfig.u32Width         = pJpegHdlCtx->Frame.yWidth;
    pJpegHdlCtx->stVpuConfig.u32Height        = pJpegHdlCtx->Frame.yHeight;
    pJpegHdlCtx->stVpuConfig.u32WidthInMcu    = pJpegHdlCtx->WidthInMCU;
    pJpegHdlCtx->stVpuConfig.u32HeightInMcu   = pJpegHdlCtx->HeightInMCU;
    pJpegHdlCtx->stVpuConfig.u64CPhyAddr      = pJpegHdlCtx->stVpuConfig.u64YPhyAddr + u32Ysize;
    pJpegHdlCtx->stVpuConfig.u32YStride       = ALIGN_UP(pJpegHdlCtx->Frame.yWidth,64);
    pJpegHdlCtx->stVpuConfig.u32CStride       = ALIGN_UP(pJpegHdlCtx->Frame.yWidth,64);
    pJpegHdlCtx->stVpuConfig.u64PhyStrStart   = pJpegHdlCtx->stVpuConfig.u64PhyStrStart + pJpegHdlCtx->u32StreamOffest;
    pJpegHdlCtx->stVpuConfig.u8YFac           = pJpegHdlCtx->Frame.H[0];
    pJpegHdlCtx->stVpuConfig.u8UFac            = pJpegHdlCtx->Frame.H[1];
    pJpegHdlCtx->stVpuConfig.u8VFac           = pJpegHdlCtx->Frame.H[2];
    pJpegHdlCtx->stVpuConfig.u32Dri           = pJpegHdlCtx->Frame.RestartIntervalLogic;
    pJpegHdlCtx->stVpuConfig.enPictureFormat  = pJpegHdlCtx->enPictureFormat;
    switch(pJpegHdlCtx->stVpuConfig.enPictureFormat)
    {
        case PICTURE_FORMAT_YUV420:
        {
            pJpegHdlCtx->stVpuConfig.u32PicType = 3;
            break;
        }
        case PICTURE_FORMAT_YUV422:
        {
            pJpegHdlCtx->stVpuConfig.u32PicType = 4;
            break;
        }
        case PICTURE_FORMAT_YUV444:
        {
            pJpegHdlCtx->stVpuConfig.u32PicType = 6;
            break;
        }
        case PICTURE_FORMAT_YUV422v:
        {
            pJpegHdlCtx->stVpuConfig.u32PicType = 5;
            break;
        }
        case PICTURE_FORMAT_YUV400:
        {
            pJpegHdlCtx->stVpuConfig.u32PicType = 0;
            break;
        }
        default:
        {
            printf("Unkonwn picture format %d!", pJpegHdlCtx->stVpuConfig.enPictureFormat);
            return HI_FAILURE;
        }
    }

    if(PIXEL_FORMAT_YVU_SEMIPLANAR_420 == pJpegHdlCtx->stVpuConfig.enPixelFormat)
    {
        pJpegHdlCtx->stVpuConfig.bOutYUV = HI_TRUE;
    }
    else
    {
        pJpegHdlCtx->stVpuConfig.bOutYUV = HI_FALSE;
        if( (PIXEL_FORMAT_ARGB_8888 == pJpegHdlCtx->stVpuConfig.enPixelFormat) || (PIXEL_FORMAT_ABGR_8888 == pJpegHdlCtx->stVpuConfig.enPixelFormat))
        {
            pJpegHdlCtx->stVpuConfig.u32RGBStride = ALIGN_UP(pJpegHdlCtx->Frame.yWidth*4, 16);
        }
        else if( (PIXEL_FORMAT_ARGB_1555 == pJpegHdlCtx->stVpuConfig.enPixelFormat) || (PIXEL_FORMAT_ABGR_1555 == pJpegHdlCtx->stVpuConfig.enPixelFormat))
        {
            pJpegHdlCtx->stVpuConfig.u32RGBStride = ALIGN_UP(pJpegHdlCtx->Frame.yWidth*2, 16);
        }
        else if( (PIXEL_FORMAT_RGB_888 == pJpegHdlCtx->stVpuConfig.enPixelFormat) || (PIXEL_FORMAT_BGR_888 == pJpegHdlCtx->stVpuConfig.enPixelFormat))
        {
            pJpegHdlCtx->stVpuConfig.u32RGBStride = ALIGN_UP(pJpegHdlCtx->Frame.yWidth*3, 16);
        }
        else if( (PIXEL_FORMAT_RGB_565 == pJpegHdlCtx->stVpuConfig.enPixelFormat) || (PIXEL_FORMAT_BGR_565 == pJpegHdlCtx->stVpuConfig.enPixelFormat))
        {
            pJpegHdlCtx->stVpuConfig.u32RGBStride = ALIGN_UP(pJpegHdlCtx->Frame.yWidth*2, 16);
        }
    }

    /* config Quant table */
    {
        HI_U32 u32QTabYIndex = pJpegHdlCtx->Frame.Tq[0] & 3;
        HI_U32 u32QTabUIndex = pJpegHdlCtx->Frame.Tq[1] & 3;
        HI_U32 u32QTabVIndex = pJpegHdlCtx->Frame.Tq[2] & 3;

        HI_U8  *QCr = (HI_U8 *)&pJpegHdlCtx->qTab[u32QTabVIndex];
        HI_U8  *QCb = (HI_U8 *)&pJpegHdlCtx->qTab[u32QTabUIndex];
        HI_U8  *QY  = (HI_U8 *)&pJpegHdlCtx->qTab[u32QTabYIndex];
        HI_S32 pos;

        for(i=0; i<QUANT_TABLE_SIZE; i++)
        {
            pos = zigzagforQtable[i & 0x3f];
            pJpegHdlCtx->stVpuConfig.u32QuantTable[pos] = QY[i] + (QCb[i]<<8) + (QCr[i]<<16);
        }
    }

    {
        HuffmanTab *pHufTab = &pJpegHdlCtx->hTab[1];
        int i, num, j, index;

        for (index=0, i=0; i<16; i++)
        {
            num = pHufTab->Len[i];
            for (j=0; j<num && index < 12; j++, index++)
            {
                int pos = pHufTab->HuffmanVal[index];
                u32HuffmanTable[pos] |= ((i+1)<<20) + (((j+pHufTab->MinCode[i])<<12) & 0xFF000);
            }
        }
    }


    {
        HuffmanTab *pHufTab = &pJpegHdlCtx->hTab[0];
        int i, num, j, index;

        for (index=0, i=0; i<16; i++)
        {
            num = pHufTab->Len[i];
            for (j=0; j<num && index < 12; j++, index++)
            {
                int pos = pHufTab->HuffmanVal[index];
                u32HuffmanTable[pos] |= ((i+1)<<8) + ((j+pHufTab->MinCode[i]) & 0xFF);
            }
        }
    }

    {
        HuffmanTab *pHufTab = &pJpegHdlCtx->hTab[2];
        int i;

        for (i=0; i<16; i++)
        {
            int baseCode = 0;

            if (pHufTab->Len[i])
                baseCode = pHufTab->HuffmanValPtr[i] - pHufTab->MinCode[i];

            LumaAcMinTable[i] = (pHufTab->MinCode[i]) & 0xFF;
            LumaAcBaseTable[i] = baseCode & 0xFF;
        }
    }

    {
        HuffmanTab *pHufTab = &pJpegHdlCtx->hTab[3];
        int i;

        for (i=0; i<16; i++)
        {
            int baseCode = 0;

            if (pHufTab->Len[i])
                baseCode = pHufTab->HuffmanValPtr[i] - pHufTab->MinCode[i];

            ChromaAcMinTable[i] = (pHufTab->MinCode[i]) & 0xFF;
            ChromaAcBaseTable[i] = baseCode & 0xFF;
        }
    }

    /* config Huffman table */
    {
        for(i=0; i<HDC_TABLE_SIZE; i++)
        {
            pJpegHdlCtx->stVpuConfig.u32HuffmanTable[i] = u32HuffmanTable[i];
        }
    }

    /* config HuffmanMinTable table */
    {
        for(i=0; i<HAC_MIN_TABLE_SIZE; i++)
        {
            pJpegHdlCtx->stVpuConfig.u32HuffmanMinTable[i] =
                (ChromaAcMinTable[2*i+1]<<24) + (ChromaAcMinTable[2*i]<<16) + (LumaAcMinTable[2*i+1]<<8) + (LumaAcMinTable[2*i]);
        }
    }

    /* config HuffmanBaseTable table */
    {
        for(i=0; i<HAC_BASE_TABLE_SIZE; i++)
        {
            pJpegHdlCtx->stVpuConfig.u32HuffmanBaseTable[i] =
                (ChromaAcBaseTable[2*i+1]<<24) + (ChromaAcBaseTable[2*i]<<16) + (LumaAcBaseTable[2*i+1]<<8) + (LumaAcBaseTable[2*i]);
        }
    }

    /* config HuffmanSymbolTable table */
    {
        for(i=0; i<HAC_SYMBOL_TABLE_SIZE; i++)
        {
            pJpegHdlCtx->stVpuConfig.u32HuffmanSymbolTable[i] =
                (pJpegHdlCtx->hTab[3].HuffmanVal[i]<<8) + pJpegHdlCtx->hTab[2].HuffmanVal[i];
        }
    }

    return HI_SUCCESS;
}



HI_S32 JPEGD_StartOneFrame(JPEGD_HANDLE Handle,HI_U32 uFlags)
{
    HI_S32 s32Ret;
    MotionJpegObj *pJpegdHdlCtx = (MotionJpegObj *)Handle;


    s32Ret = JPEGD_DecFrame(Handle,uFlags);
    if(HI_SUCCESS != s32Ret)
    {
        HI_TRACE( "VDEC_StartOneFrame: decode stream fail for 0x%x\n", s32Ret);
        return s32Ret;
    }


    s32Ret = JPEGD_PrepareHardwareInfo(pJpegdHdlCtx);
    if(HI_SUCCESS != s32Ret)
    {
        HI_TRACE( "VDEC_StartOneFrame: decode stream fail for 0x%x\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}


/****************************************************************************
* Function Name : JPEGD_WriteRegs
* Description   : write the jpegd register
* Parameters    : handle pRegBase
* Return Type   : HI_SUCCESS or error code
****************************************************************************/
HI_S32  JPEGD_WriteRegs(JPEGD_HANDLE Handle, HI_VOID* pRegBase)
{
    MotionJpegObj *pJpegHdlCtx = (MotionJpegObj *)Handle;

    JPEGD_DRV_WriteRegs(pRegBase, &pJpegHdlCtx->stVpuConfig);
    return HI_SUCCESS;
}

/****************************************************************************
* Function Name : JPEGD_WriteRegs
* Description   : write the jpegd register
* Parameters    : handle pRegBase
* Return Type   : HI_SUCCESS or error code
****************************************************************************/
HI_S32  JPEGD_ReadRegs(JPEGD_HANDLE Handle, HI_VOID* pRegBase)
{
    MotionJpegObj *pJpegHdlCtx = (MotionJpegObj *)Handle;

    JPEGD_DRV_ReadRegs(pRegBase, &pJpegHdlCtx->stVpuStatus);
    return HI_SUCCESS;
}


HI_S32  JPEGD_StartDecoding(JPEGD_HANDLE Handle)
{
    MotionJpegObj *pJpegdHldCtx;
    HI_S32 s32Ret;

    pJpegdHldCtx = (MotionJpegObj *)Handle;
    pJpegdHldCtx->stVpuConfig.u64YPhyAddr       = VIDEO_DATA_BASE;
    pJpegdHldCtx->stVpuConfig.u64PhyStrBufStart = hilogo;
    pJpegdHldCtx->stVpuConfig.u64PhyStrBufEnd   = hilogo + ALIGN_UP(jpeg_size, 128);
    pJpegdHldCtx->stVpuConfig.u64PhyStrStart    = hilogo;
    pJpegdHldCtx->stVpuConfig.u64PhyStrEnd      = hilogo + jpeg_size;
    pJpegdHldCtx->stVpuConfig.u64PhyEmarBuffer0 = JPEGD_EMAR_BUF;
    pJpegdHldCtx->stVpuConfig.u64PhyEmarBuffer1 = JPEGD_EMAR_BUF + ONE_EMAR_BUF_SIZE;
    pJpegdHldCtx->stStream.pu8VirAddr              = (HI_U8 *)hilogo;
    pJpegdHldCtx->stStream.u32Len                  = jpeg_size;
    pJpegdHldCtx->stStream.u64PhyAddr              = hilogo;
    pJpegdHldCtx->stStream.u64PTS                  = 0;
    pJpegdHldCtx->stVpuConfig.s32ChnID             = 0;
    pJpegdHldCtx->stVpuConfig.u32Alpha             = 255;

    if(output_format == 0)
    {
        pJpegdHldCtx->stVpuConfig.enPixelFormat    = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    }
    else if(output_format == 1)
    {
        pJpegdHldCtx->stVpuConfig.enPixelFormat    = PIXEL_FORMAT_ARGB_1555;
    }
    else if(output_format == 2)
    {
        pJpegdHldCtx->stVpuConfig.enPixelFormat    = PIXEL_FORMAT_ARGB_8888;
    }
    else
    {
        pJpegdHldCtx->stVpuConfig.enPixelFormat    = PIXEL_FORMAT_YVU_SEMIPLANAR_420;
    }

    s32Ret = JPEGD_StartOneFrame(Handle, 0);
    if(HI_SUCCESS != s32Ret)
    {
        HI_TRACE( "JPEGD_StartDecoding: decode stream fail for 0x%x\n", s32Ret);
        return  s32Ret;
    }

    JPEGD_SetClockEn(0, HI_TRUE);
    JPEGD_Reset(0, HI_NULL);
    JPEGD_SetTimeOut(0, 0xFFFFFFFF);

    JPEGD_WriteRegs(Handle, (HI_VOID* )JPEGD_REGS_ADDR);

    JPEGD_StartVpu(0);

    return HI_SUCCESS;
}


HI_VOID JPEGD_FinishDecoding(JPEGD_HANDLE Handle)
{
    HI_U32 u32IntStatue;
    MotionJpegObj *pJpegdHldCtx = (MotionJpegObj *)Handle;

    while(1)
    {
        udelay(10);
        u32IntStatue = JPEGD_ReadInt(0);
        if(u32IntStatue & 0x1f)
        {
            break;
        }
    }

    JPEGD_ReadRegs(Handle, (HI_VOID* )JPEGD_REGS_ADDR);
    if(0 == pJpegdHldCtx->stVpuStatus.bIntDecFinish)
    {
        printf("Hardware decoding error!\n");
    }
    else
    {
        if(HI_TRUE != pJpegdHldCtx->stVpuConfig.bOutYUV)
        {
            printf("Hardware decoding success! %dx%d, stride %d.\n",
                pJpegdHldCtx->Frame.yWidth, pJpegdHldCtx->Frame.yHeight, pJpegdHldCtx->stVpuConfig.u32RGBStride);
        }
        else
        {
            printf("Hardware decoding success! %dx%d, stride %d.\n",
                pJpegdHldCtx->Frame.yWidth, pJpegdHldCtx->Frame.yHeight, pJpegdHldCtx->stVpuConfig.u32YStride);
        }
    }

    JPEGD_ClearInt(0);
    JPEGD_ResetSelect(0, HI_TRUE);

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

