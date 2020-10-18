/*********************************************************
 *  @file    opencpu_http.c
 *  @brief   M5311 OpenCPU HTTP示例
 *  Copyright (c) 2019 China Mobile IOT.
 *  All rights reserved.
 *  created by XieGangLiang 2019/09/06
 ********************************************************/
#include "m5311_opencpu.h"
 
void http_test()
{    
    char* url = "https://www.baidu.com/";
    httpclient_t client = {0};
    httpclient_data_t client_data = {0};
    char *buf = NULL;
    buf = pvPortMalloc(600);
    if (buf == NULL) 
    {
        opencpu_printf("Malloc failed.\r\n");
        return;
    }
    memset(buf, 0, sizeof(buf));
    client_data.response_buf = buf;  
    client.timeout_in_sec = 20;
    client_data.response_buf_len = 600;  
    opencpu_printf("Data received: %d,\n%s\r\n", oc_httpclient_get(&client, url, &client_data),client_data.response_buf);
}
void http_test2()
{
    char* url = "https://www.baidu.com/";
    httpclient_t client = {0};
    httpclient_data_t client_data = {0};
    HTTPCLIENT_RESULT ret = HTTPCLIENT_ERROR_CONN;
    char *buf = NULL;
    buf = pvPortMalloc(600);
    if (buf == NULL) 
    {
        opencpu_printf("Malloc failed.\r\n");
        return;
    }
    memset(buf, 0, sizeof(buf));
    client_data.response_buf = buf;  //Sets a buffer to store the result.
    opencpu_printf("start http test\n");
    client_data.response_buf_len = 600;  //Sets the buffer size.
    opencpu_printf("start connect\n");
    ret = oc_httpclient_connect(&client, url);
    if (!ret) {
        opencpu_printf("start send request\n");
        ret = oc_httpclient_send_request(&client, url, HTTPCLIENT_GET, &client_data);

        if (!ret) {
            opencpu_printf("start recv\n");
            ret = oc_httpclient_recv_response(&client, &client_data);
            opencpu_printf("Data received: %s\r\n", client_data.response_buf);
        }
    }
    opencpu_printf("close\n");
    oc_httpclient_close(&client);
}
