/*
   opencpu_base_func.c
   created by xgl,2018/4/2
*/

#include "m5311_opencpu.h"

#define GKI_LOG_NAME "emmi"
#define HSL_LOG_NAME "uls"
#define USER_GKI_LOG_PORT SERIAL_PORT_DEV_USB_COM1   //请在这里修改用户GKI log的输出串口，默认为USB COM1
#define USER_HSL_LOG_PORT SERIAL_PORT_DEV_USB_COM2  //请在这里修改用户HSL log的输出串口，默认为USB COM2
hal_uart_port_t  opencpu_exception_port = HAL_UART_0; //模组发生异常死机时，此变量决定输出死机信息的串口，默认为串口0
/********************************************************************************/
//此函数为opencpu产线模式相关的回调函数，返回1则版本下载到模组后，以AT命令方式启动，需要先执行AT+ATCLOSE命令，之后才会以opencpu方式启动。
//返回0则以opencpu方式启动
//请务必联系技术支持后再确定返回值
//请客户务必保留此函数中所有代码，仅根据需求调整log输出的串口号和返回值。否则可能造成功能紊乱
int get_factory_mode(void)
{
	opencpu_exception_port = HAL_UART_0;
	//以下代码是判断模组log口的输出串口号，确保生效的设置和用户的设置一致，防止log口设置错误而干扰用户
	serial_port_dev_t gki_port = -1;
	serial_port_dev_t hsl_port = -1;
	int uart3_log;
	opencpu_read_port_config(GKI_LOG_NAME,&gki_port);
	opencpu_read_port_config(HSL_LOG_NAME,&hsl_port);
	
	if( (gki_port != USER_GKI_LOG_PORT) || (hsl_port != USER_HSL_LOG_PORT)) //根据读的结果决定是否写
	{
		opencpu_write_port_config(GKI_LOG_NAME,USER_GKI_LOG_PORT);
		opencpu_write_port_config(HSL_LOG_NAME,USER_HSL_LOG_PORT);
        //以下代码用来支持UART3抓GKI log
         if((USER_GKI_LOG_PORT == SERIAL_PORT_DEV_UART_3)||(USER_HSL_LOG_PORT == SERIAL_PORT_DEV_UART_3))
        {
            uart3_log = 1;
            nvdm_modem_write_normal_data_item(
					 "cmiot",
					 "cmiot_log_set",
					 0x01,
					 (const uint32_t*)&uart3_log,
					 sizeof(uint32_t),
					 0);
        }
        else
        {
            uart3_log = 0;
            nvdm_modem_write_normal_data_item(
					 "cmiot",
					 "cmiot_log_set",
					 0x01,
					 (const uint32_t*)&uart3_log,
					 sizeof(uint32_t),
					 0);
        }
		opencpu_reboot();//因为设置是重启生效，所以设置完必须有reboot函数
	}
	
	
	return 1;
}

//OpenCPU在AT模式提供AT+SWVER命令查询当前OpenCPU软件版本号，此版本号可通过以下回调函数进行定制
//此版本号最大长度60个字节
//返回1代表定制版本号有效，返回0代表定制版本号无效
int opencpu_custom_swver_cb(unsigned char *p)
{
    unsigned char * temp = "OpenCPU VX.X.X";
    strcpy(p,temp);
    return 1;
    
}
/********************************************************************************/
//此函数为wakeup引脚中断回调函数，在wakein引脚拉低时触发执行
//睡眠状态下测试时，打印函数不会生效，因为系统刚刚恢复，还未初始化uart
void opencpu_wakeup_callback()
{
	//此处去掉打印函数，防止因为串口未初始化，造成opencpu_printf阻塞，使模组wakeupin唤醒后无法正常启动opencpu线程 20190906
	//opencpu_printf("opencpu wakeup\n");
}
/********************************************************************************/
static int char_to_int(unsigned char *s)
{
	int i;  
    int n = 0;  
    for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)  
    {  
        n = 10 * n + (s[i] - '0');  
    }  
    return n;  
}

//测试获取实时时间，以及计算time(null)返回值的示例
void test_get_time()
{
	unsigned char time_string[50];
	unsigned char *p1;
	unsigned char *p2;
	struct tm l_tm;
	memset(time_string,0,50);
	opencpu_rtc_get_time(time_string);
	opencpu_printf("TIME:%s\n",time_string);
	p1 = strchr(time_string,'/');
	*p1 = 0;
	l_tm.tm_year = atoi(time_string)-1900;
	p2 = strchr(p1+1,'/');
	*p2 = 0;
	l_tm.tm_mon = atoi(p1+1)-1;	
	p1 = strchr(p2+1,',');
	*p1 =0;
	l_tm.tm_mday = atoi(p2+1);	
	p2 = strchr(p1+1,':');
	*p2 = 0;
	l_tm.tm_hour = atoi(p1+1);
	p1 = strchr(p2+1,':');
	*p1 = 0;
	l_tm.tm_min = atoi(p2+1);
	p2  = strchr(p1+1,'G');
	*p2 = 0;
	l_tm.tm_sec = atoi(p1+1);
	opencpu_printf("secs:%d",mktime(&l_tm));
}


//测试设置RTC时间
void test_set_time()
{
	/*
<time>:  string type value; format is "yy/MM/dd,hh:mm:ss+-zz"
*                       where characters indicate the last two digits of
*                       year,month,day,hour,minute,second and time zone.
*                       The time zone is expressed in quarters of an hour
*                       between the local time and GMT; range (-47...+48)
*                       eg. 6th May 1994 22:10:00 GMT+2 hours equals
*                       "94/05/06,22:10:00+08"
*/
	opencpu_rtc_set_time("94/05/06,22:10:00+08");
	opencpu_printf("time set ok\n");
	
}
/********************************************************************************/
//测试获取IMEI
unsigned char local_imei[40] = {0};
void test_get_imei()
{
	memset(local_imei,0,40);
	opencpu_printf("result:%d\n",opencpu_get_imei(local_imei));
	opencpu_printf("IMEI:%s\n",local_imei);
	
}
/********************************************************************************/
//测试获取IMSI
unsigned char local_imsi[40] = {0};
void test_get_imsi()
{
	memset(local_imsi,0,40);
	opencpu_printf("result:%d\n",opencpu_get_imsi(local_imsi));
	opencpu_printf("IMSI:%s\n",local_imsi);
	
}
/********************************************************************************/
//测试获取ICCID
void test_get_iccid()
{
	unsigned char buf[30];
	int i = 0;
	memset(buf,0,30);
	while(opencpu_iccid(buf)!= 0)
	{
		i++;
		vTaskDelay(10);
		if(i>20)
		{
			opencpu_printf("iccid timeout\n");
			return;
		}
	}
	opencpu_printf("ICCID:%s\n",buf);
}
/********************************************************************************/
/*
   获取当前信号强度
*/
void get_signal_strength()
{
	int rssi,rxlevel;
	opencpu_csq(&rssi,&rxlevel);
	opencpu_printf( "%d,%d\n",rssi,rxlevel);
}
/********************************************************************************/
void test_dm_auto_mode()
{
	dm_config_t dm_config = {
		1, //测试
		1, //enable
		2, //心跳间隔2min
		20, //版本号，默认2.0
		"M100000052", //appkey
		"n525A97z0M7Vyh91b0508l7j0U5g2g9Y"};
	opencpu_dm_set_mode(1);
	opencpu_dm_start(dm_config);//自动模式下调用此函数之后即会定时上报DM信息
}
void test_dm_trigger_mode()
{	
	dm_config_t dm_config = {
		1, //测试
		1, //enable
		2, //心跳间隔2min
		20, //版本号，默认2.0
		"M100000052", //appkey
		"n525A97z0M7Vyh91b0508l7j0U5g2g9Y"};
	opencpu_dm_set_mode(0);
	opencpu_dm_start(dm_config);//手动模式下，调用此函数后只是初始化，要上报DM信息需要调用opencpu_dm_update();
	
	opencpu_dm_update();
	
}
/********************************************************************************/
//测试FLASH
void flash_test()
{
	unsigned char temp_read[10];
	unsigned char temp_write[] = "123123";
	memset(temp_read,0,10);
	opencpu_flash_erase(0,6);
	opencpu_flash_write(1,temp_write,strlen(temp_write));
	opencpu_flash_read(1,temp_read,6);
	opencpu_printf("read:%s\n",temp_read);
}
void cJSON_test()
{
    cJSON *root=NULL;
    cJSON *sub_js=NULL;
    char *out=NULL;
    
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "ver", "v3.3.0");
    cJSON_AddStringToObject(root, "imei", "1xxxxxxxxxxxxxxxx");
    cJSON_AddStringToObject(root, "hardware_ver", "V1.0.0");
    cJSON_AddStringToObject(root, "software_ver", "V3.3.0");
    cJSON_AddItemToObject(root, "data", sub_js = cJSON_CreateObject());
    cJSON_AddNumberToObject(sub_js, "status", 1);
    cJSON_AddTrueToObject(sub_js, "material");
    cJSON_AddTrueToObject(sub_js, "power_on");
    cJSON_AddNumberToObject(sub_js, "qty", 123);
    cJSON_AddStringToObject(root, "dt", "2017-11-04T05:15:52");
    
    out=cJSON_Print(root);
    cJSON_Delete(root);    
    opencpu_printf("%s\n",out);    
    cJSON_free(out);

}