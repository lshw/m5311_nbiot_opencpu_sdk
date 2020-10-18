#include "m5311_opencpu.h"

void onNmi_cb(int data_length)
{
    //TODO
    //...
    opencpu_printf("onNmi_cb:%d\n",data_length);
    return;
}
void onRead_cb(int read_actual_length, int remain_length, char *data)
{
    //TODO
    //...
    opencpu_printf("onRead_cb:%d,%d,%s", read_actual_length, remain_length, data);
    return;
}
void onEvtind_cb(int type)
{
    //TODO
    //...
    opencpu_printf("onEvtind_cb:%d\n",type);
    return;
}
void onEvtind_mid_cb(int type, int mid)
{
    //TODO
    //...
    opencpu_printf("onEvtind_cb:%d,%d\n",type, mid);
    return;
}
void onStr_cb(int seq, int status)
{
    //TODO
    //...
    opencpu_printf("onStr_cb:%d, %d\n", seq, status);
    return;
}
void onDrop_cb(int length)
{
    //TODO
    //...
    opencpu_printf("onDrop_cb:%d\n",length);
    return;
}

void test_ct_init()
{
	ct_cb_t callback;
    callback.onNmi = onNmi_cb;
    callback.onRead = onRead_cb;
    callback.onEvtind = onEvtind_cb;
    callback.onEvtind_mid = onEvtind_mid_cb;
    callback.onStr = onStr_cb;
    callback.onDrop = onDrop_cb;

    opencpu_ct_create(&callback);//初始化任务

    if (APB_PROXY_RESULT_OK == opencpu_ct_new("180.101.147.115", "5683"))
    {
        opencpu_printf("ct_new OK\n");
    }
    else
    {
        opencpu_printf("ct_new fail\n");
        return;
    }
    vTaskDelay(1000 / portTICK_RATE_MS); 

    if(APB_PROXY_RESULT_OK == opencpu_ct_open(1, 90))
    {
        opencpu_printf("ct_open OK\n");
    }
    else
    {
        opencpu_printf("ct_open fail\n");
        return;
    }
  
    vTaskDelay(1000 / portTICK_RATE_MS);

    if(APB_PROXY_RESULT_OK == opencpu_ct_setcfg(1,1))
    {
        opencpu_printf("ct_setcfg OK\n");
    }
    else
    {
        opencpu_printf("ct_setcfg fail\n");
        return;
    }

    return;
}

void test_ct_send()
{
    int mid = 0;
    if(APB_PROXY_RESULT_OK == opencpu_ct_send_ex_mid(10, "mid4567898", 9, 33, &mid))
    {
        opencpu_printf("opencpu_ct_send_ex_mid OK, mid:%d\n", mid);
    }
    else
    {
        opencpu_printf("ct_send1 fail\n");
        return;
    }
    vTaskDelay(1000 / portTICK_RATE_MS);
}

void test_ct_read()
{  
    int read_actual_length = 0;
    int remain_length = 0;
    char *readdata = (char*)malloc(50);
    if (readdata == NULL)
    {
        opencpu_printf("malloc fail\n");
        return;
    }
    memset(readdata, 0 , sizeof(readdata));

    if(APB_PROXY_RESULT_OK == opencpu_ct_read(10, &read_actual_length, &remain_length, readdata))
    {
        opencpu_printf("ct_read OK read_actual_length:%d remain_length:%d readdata:%s\n" ,read_actual_length, remain_length, readdata);
    }
    else
    {
        opencpu_printf("ct_read fail\n");
        return;
    }
}

void test_ct_deinit()
{
    if(APB_PROXY_RESULT_OK == opencpu_ct_close())
    {
        opencpu_printf("ct_close OK\n");
    }
    else
    {
        opencpu_printf("ct_close fail\n");
        return;
    }
    vTaskDelay(3000 / portTICK_RATE_MS);
    if(APB_PROXY_RESULT_OK == opencpu_ct_client_delte())
    {
        opencpu_printf("ct_delte OK\n");
    }
    else
    {
        opencpu_printf("ct_delte fail\n");
        return;
    }

    return;
}