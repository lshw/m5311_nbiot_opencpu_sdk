/*
   M5311_opencpu.c
   created by xgl,2018/4/2
*/

#include "m5311_opencpu.h"


extern unsigned int OC_DEBUG_CHANNEL;
/*
  综合测试函数  
*/
void test_all_in_one()
{
	int i;
	unsigned char temp;
	unsigned  char iccid[40]={0};
	
	int iccid_len;
	unsigned char *p = 0;
	int rssi,rxlevel;
	unsigned char temp1,temp2;
    unsigned char spi_rx_buf[2]= {0};
    unsigned char spi_tx_buf[2]= {0};
	unsigned char base_version[30] = {0};
    hal_spi_master_config_t l_config;	
	hal_spi_master_send_and_receive_config_t spi_send_and_receive_config;
	custom_uart_init();
	opencpu_get_base_version(base_version);
	opencpu_printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");	
	opencpu_printf(opencpu_fota_version_cb());
	opencpu_printf("\nBASE_VERSION:%s\n",base_version);
	opencpu_printf("update status:%d\n",update_status);
	if(opencpu_is_boot_from_sleep()==1)
	{

		opencpu_printf("BOOT CAUSE:WAKE FROM SLEEP\n");
	}
	else
	{
		opencpu_printf("BOOT CAUSE:POWER_ON OR RESET\n");
	}
	opencpu_printf("M5311 opencpu ready!!\n");
	opencpu_printf("waiting for network...\n");
    opencpu_lock_light_sleep();
	//阻塞方式获取ICCID，必须要有SIM卡才能读到
	test_get_iccid();
	//获取网络注册状态，并阻塞等待网络注册成功
	opencpu_printf("network registering...\n");
	while(opencpu_cgact()!=1)
	{
		vTaskDelay(10);
	}
	opencpu_printf("network register success\n");
	opencpu_printf("network ready!!\n");	
	test_pwm_init();
	opencpu_printf("please input cmds");
	uart_cmd = 0;
	while(1)
	{
		//FOTA查询
		if(uart_cmd == 'f')
		{
			opencpu_fota_try_download();
			opencpu_printf("start qurey\n");
			uart_cmd = 0;
		}
		//FOTA升级
		if(uart_cmd == 'u')
		{
			opencpu_fota_update();
			opencpu_printf("start update!!\n");
			uart_cmd = 0;
		}
		if(uart_cmd == 'a')
		{
			test_dm_trigger_mode();
			opencpu_printf("test pm\n");
			uart_cmd = 0;
		}
	  //测试udp
	   if(uart_cmd=='U') 
	    {
		udp_test();
		uart_cmd = 0;
	    }
		//测试dns
	   if(uart_cmd == 'D')
	   {
		test_dns();
		uart_cmd = 0;
	   }
	   //测试获取信号质量
	   if(uart_cmd == 'X')
	   {
		   opencpu_csq(&rssi,&rxlevel);
		   opencpu_printf("%d,%d\n",rssi,rxlevel);
		   uart_cmd = 0;
	   }
	   //测试onenet
	   if(uart_cmd == 'o')
	    {
		   test_onenet();
		   uart_cmd = 0;
	    }
	   //测试TCP
	   if(uart_cmd == 'T')
	   {
		   tcp_test();
		   uart_cmd =0;
	   }
	   //测试ping
	   if(uart_cmd == 'P')
	   {
		   test_ping();
		   uart_cmd = 0;
	   }
	   //测试获取IMEI
	   if(uart_cmd == 'M')
	   {
		   test_get_imei();
		   uart_cmd = 0;
	   }
	   //测试获取IMSI
	   if(uart_cmd == 'N')
	   {
		   test_get_imsi();
		   uart_cmd = 0;
	   }
	   //测试ADC
	   if(uart_cmd == 'A')
	   {
		   test_adc();
		   uart_cmd = 0;
	   }
	   //获取RTC时间
	   if(uart_cmd == 'C')
	   {
		    test_get_time();
			uart_cmd = 0;
	   }
	   //设置RTC时间
	   if(uart_cmd == 'S')
	   {
		   test_set_time();
		   uart_cmd = 0;
		   
	   }
	   //测试flash
	   if(uart_cmd == 'F')
	   {
		   flash_test();
		   uart_cmd = 0;
	   }
	   //测试PWM打开
	   if(uart_cmd == 'p')
	   {
		   opencpu_pwm_start(3);	   
		   uart_cmd = 0;
	   }
	   //测试PWM关闭
	   if(uart_cmd == 'Z')
	   {
		    opencpu_pwm_stop(3);	   
		    uart_cmd = 0;
	   }
       if(uart_cmd == '1')
	   {
			opencpu_printf("%f,%f,%f,%f,%f,%f,%f\n",sin(2),sin(3),cos(2),tan(2),atan(1),acos(-1),asin(-1));
		    uart_cmd = 0;
	   }
	   if(uart_cmd == '0')
	   {
		   opencpu_printf("time:%d\n",time(0));
		   uart_cmd = 0;
	   }
	   if(uart_cmd == '2')
	   {
		    
			opencpu_printf("prod imei write:%d\n",opencpu_write_prod_imei("869975030003275"));
		    uart_cmd = 0;
	   }

	   //设置CEREG的读取模式，模式的具体值参考AT文档对应命令
	   if(uart_cmd == '4')
	   {
		   opencpu_cereg_excute(4);
		   uart_cmd = 0;
	   }
	   //读取网络注册信息
	   if(uart_cmd == '5')
	   {
		   ril_eps_network_registration_status_rsp_t param;
		   opencpu_cereg_read(&param);
		   opencpu_printf("+CEREG:%d,%d\n",param.active_time,param.periodic_tau);
		
		   uart_cmd = 0;
	   }
	  //设置psm
	   if(uart_cmd == '6')
	   {
	   	   ril_power_saving_mode_setting_req_t psm_req1;
	   	   psm_req1.mode=1;
	   	   psm_req1.req_prdc_rau=NULL;
	   	   psm_req1.req_gprs_rdy_tmr=NULL;
	   	   psm_req1.req_prdc_tau="00101011";
	   	   psm_req1.req_act_time="00100100";
	 
		   opencpu_set_psmparam(&psm_req1);
		   uart_cmd = 0;
		}
		//读取psm设置值
		if(uart_cmd == '7')
	   {
	   	   ril_power_saving_mode_setting_rsp_t psm_rsp1;
	   	   opencpu_get_psmparam(&psm_rsp1);
	   	   opencpu_printf("%d,%s,%s,%s,%s\n",psm_rsp1.mode,psm_rsp1.req_prdc_rau,
	   	                  psm_rsp1.req_gprs_rdy_tmr,psm_rsp1.req_prdc_tau,psm_rsp1.req_act_time);
	   	   
	 
		   uart_cmd = 0;
		}
		//测试IIC接口的BMP180传感器（需用户自己连接该外设）
          if(uart_cmd == '8')
		{
			
			bmp180_test();
			uart_cmd = 0;
		}
		//打开AT模式
	    if(uart_cmd == '9')
	    {
		   opencpu_at_open();
		   uart_cmd = 0;
	    }
		//测试EDRX参数读取
	     if(uart_cmd == 'E')
		 {
			 int temp_type;
			 unsigned char temp_value[10];
             opencpu_read_edrx(&temp_type,temp_value);		
             opencpu_printf("type:%d,value:%s\n",temp_type,temp_value);
             uart_cmd = 0;			 
		 }
		 //测试EDRX参数设置
		 if(uart_cmd == 'e')
		 {
			 opencpu_set_edrx(1,5,"0101");
			 opencpu_printf("edrx set ok\n");
			 uart_cmd = 0;	
		 }
		 
		 if(uart_cmd == '%')
		 {
			 opencpu_set_cmsysctrl(0, 2, 50, 300, 100, 800);
			 opencpu_printf("net state ok\n");
			 uart_cmd = 0;
		 }
		 if(uart_cmd == '&')
		 {
			 int i= 0;
			 while(i<5)
			 {
				 i++;
				 opencpu_delay_ms(1000);
				 opencpu_printf("i:%d\n",i);
			 }
			 uart_cmd = 0;
		 }
		 if(uart_cmd == '*')
		 {
			  int i= 0;
			 while(i<5)
			 {
				 i++;
				 opencpu_delay_us(1000000);
				 opencpu_printf("i:%d\n",i);
			 }
			 uart_cmd = 0;
		 }
		 if(uart_cmd == '.')
		 {
			 test_andlink();
			 uart_cmd = 0;
		 }
		 if(uart_cmd == '-')
        {
          
           int x = 0;
           void *address = NULL;
            OC_DEBUG_CHANNEL = 1;
            address = 0x0;
            x = *((uint32_t *)address);
            opencpu_printf("&&&&&&&&&&&&&&&&& %d &&&&&&&&&&&&&&&&!\r\n", x);
            uart_cmd = 0;
        }
        if(uart_cmd == '>')
        {
            http_test();
            uart_cmd = 0;
        }
        if(uart_cmd == '@')
        {
            gpt_us_timer_test();
            uart_cmd = 0;
        }
        if(uart_cmd == '#')
        {
            cJSON_test();
			uart_cmd = 0;
            
        }
        if(uart_cmd == '~')
        {
            mqtt_test3();
            uart_cmd = 0;
        }
		if(uart_cmd == 'm')
		{
			ril_low_power_related_info_rsp_t mlpinfo; 
            opencpu_mlpinfo(0,&mlpinfo);
			opencpu_unlock_light_sleep();
			opencpu_printf("sleep_duration:%d,rx_time:%d,tx_time:%d",mlpinfo.sleep_duration,mlpinfo.rx_time,mlpinfo.tx_time);
            uart_cmd = 0;

		}
        if(uart_cmd == 'b')
		{
			char a[128] = {0};
            cmband_cfg_t test = {0,0,0,0,0,0};
            //memset(&test, 0, sizeof(cmband_cfg_t));
            opencpu_cmband_read(&test);
            sprintf(a,"%d,%d,%d,%d,%d,%d", test.CFG_BAND_1, test.CFG_BAND_3, test.CFG_BAND_5, test.CFG_BAND_8, 
            test.CFG_BAND_20, test.CFG_BAND_28);
			opencpu_printf("band:%s\r\n",a);

		    test.CFG_BAND_5 = 0;
            opencpu_cmband_write(&test);

			cmband_cfg_t test2 = {0,0,0,0,0,0};
            opencpu_cmband_read(&test2);
            sprintf(a,"%d,%d,%d,%d,%d,%d", test2.CFG_BAND_1, test2.CFG_BAND_3, test2.CFG_BAND_5, test2.CFG_BAND_8, 
            test2.CFG_BAND_20, test2.CFG_BAND_28);
            opencpu_printf("band:%s\r\n",a);
            
            test.CFG_BAND_5 = 1;
            opencpu_cmband_write(&test);
			uart_cmd = 0;
		}
	vTaskDelay(10);
	}
}

//opencpu主任务函数
void opencpu_task_main()
{
  test_all_in_one();
  vTaskDelete(NULL);
}


/*
 新建opencpu任务，这个函数用户不可更改
*/
void test_opencpu_start()
{
	 xTaskCreate(opencpu_task_main,"opencpu",1024,NULL,TASK_PRIORITY_NORMAL,NULL);
}