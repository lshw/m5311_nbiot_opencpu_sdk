/*********************************************************
 *  @file    opencpu_backtrace.c
 *  @brief   OpenCPU backtrace功能相关内容
 *  Copyright (c) 2019 China Mobile IOT.
 *  All rights reserved.
 *  created by XieGangLiang @2019
 ********************************************************/
#include "m5311_opencpu.h"

/*用于选择死机信息处理通道，0：MTK原始通道 1：cm_backtrace通道*/
unsigned int OC_DEBUG_CHANNEL = 0;

