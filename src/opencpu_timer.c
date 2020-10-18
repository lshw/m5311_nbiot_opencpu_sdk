/*********************************************************
*  @file    opencpu_timer.c
*  @brief   M5311 OpenCPU timer example file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangliang 2019/11/26
********************************************************/
#include "m5311_opencpu.h"

#define gpt_port HAL_GPT_1


void gpt_cb(void *param)
{
    opencpu_printf("%s\n",param);
}

void gpt_us_timer_test()
{
    if(hal_gpt_init(gpt_port) != HAL_GPT_STATUS_OK)
    {
        opencpu_printf("init failed");
    }
    hal_gpt_register_callback(gpt_port,gpt_cb, "us timer\n");
    opencpu_printf("start timer:%d\n",hal_gpt_start_timer_us(gpt_port, 1000000, HAL_GPT_TIMER_TYPE_ONE_SHOT));
}

void  gpt_ms_timer_test()
{
    unsigned int handle;
    int remain;
    if(hal_gpt_sw_get_timer(&handle) != HAL_GPT_STATUS_OK)
    {
        opencpu_printf("init ms timer fail");
    }
    opencpu_printf("ms timer start:%d\n",hal_gpt_sw_start_timer_ms(handle,1000, gpt_cb, "ms_timer\n"));
    vTaskDelay(1);
    hal_gpt_sw_get_remaining_time_ms(handle, &remain);
    opencpu_printf("remain:%d\n",remain);
}