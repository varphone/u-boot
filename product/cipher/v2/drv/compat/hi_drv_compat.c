/******************************************************************************

    Copyright (C), 2017, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : ext_aead.c
  Version       : Initial Draft
  Created       : 2017
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include "cipher_adapt.h"

HI_S32 hi_drv_compat_init(void)
{
    HI_S32 ret = HI_FAILURE;

    ret = DRV_KLAD_Init();
    if (HI_SUCCESS != ret)
    {
         return ret;
    }

    return HI_SUCCESS;
}

HI_S32 hi_drv_compat_deinit(void)
{
    DRV_KLAD_DeInit();

    return HI_SUCCESS;
}

