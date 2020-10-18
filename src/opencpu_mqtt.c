#include "m5311_opencpu.h"

/***************************************
以下定义适用于onenet mqtt套件
CLIENTID:对应设备名称
USER:对应产品ID
PWD:用onenet官网的token.exe工具生成。
CMDREQUEST：

***************************************/
#define HOST "183.230.40.16"
#define PORT 8883
#define CLIENTID "test"
#define KEEPALIVE 120
#define USER "247845"
#define PWD "version=2018-10-31&res=products%2F247845%2Fdevices%2Ftest&et=1694376057&method=md5&sign=32%2B6jCEhKLdgXwdvpAQM8g%3D%3D"
#define CMDREQUEST "$sys/247845/test/cmd/request/"
#define CMDRESPON  "$sys/247845/test/cmd/response/"
uint8 onenetFlag = 0;

/*****************************************************
ack 回调函数，publish msg 收到ack后会触发该类型函数,用户自行编写;
传入:qos>0 时在opencpu_mqttpub 中传入该函数，不关心ack 可传入null
/*****************************************************/
void opencpu_recvpuback_cb()
{
	opencpu_printf("OC get ack\n");
	vTaskDelay(2000/portTICK_RATE_MS);
}
void opencpu_recvpuback_cb1()
{
	opencpu_printf("last publish success,off\n");
	vTaskDelay(1000/portTICK_RATE_MS);
	opencpu_mqttdisc();
}

/*****************************************************
接收到某主题的回调函数:接收到平台下对topic的推送消息是会触发该类型函数。
传入:在opencpu_mqttsub 订阅某topic时传入该类型函数。

/*****************************************************/
void opencpu_defaultpublish_cb(uint32_t dup, uint32_t qos, uint32_t retained, uint32_t msgid, char *topicname, uint32_t msglen, char *msg)
{

	opencpu_printf("+OPENCPUCPUBLISH:%d,%d,%d,%d,%s,%d,%s\n",dup, qos, retained, msgid, topicname,msglen, msg);
	if(onenetFlag == 1)
	{
		
		char* res = strstr(topicname,CMDREQUEST);
		if(res != NULL)
		{
			char* cmdid = &topicname[strlen(CMDREQUEST)];
			char topic[96] = CMDRESPON;
			strcat(topic,cmdid);
			// 回复平台下发的指令，回复内容可自定义
			opencpu_mqttpub(topic,0, 0, 0, 0, "REC OK", NULL);	
		}
	}
	if(strcmp(msg,"close") == 0)
	{
		vTaskDelay(1000/portTICK_RATE_MS);
		opencpu_mqttdisc();
	}
	vTaskDelay(2000/portTICK_RATE_MS);
}

void opencpu_userpublish_cb(uint32_t dup, uint32_t qos, uint32_t retained, uint32_t msgid, char *topicname, uint32_t msglen, char *msg)
{

	opencpu_printf("+MYCB:%d,%d,%d,%d,%s,%d,%s\n",dup, qos, retained, msgid, topicname,msglen, msg);
	vTaskDelay(2000/portTICK_RATE_MS);
	//opencpu_mqttpub("temp1",1,0,0,0,"from_pub_usercb", NULL);
}
/**************************
opencpu_userpublish_cb1
opencpu_userpublish_cb2
....
opencpu_userpublish_cbn

**************************/
void opencpu_userpublish_cb1(uint32_t dup, uint32_t qos, uint32_t retained, uint32_t msgid, char *topicname, uint32_t msglen, char *msg)
{
	;
}

/*****************************
连接服务器成功的回调函数
*****************************/
void opencpu_open_cb(uint8_t result)
{
	if(result == 0 )
	{
		opencpu_printf("%s\n","MQTTCLIENT CONNECT OK");
	}
	
}

/***********************************
和服务器断开的回调函数
***********************************/
void opencpu_disc_cb()
{
	opencpu_printf("%s\n","MQTTCLIENT DISCONNECTED ");
}

/************************************************
测试1

/************************************************/

mqtt_test1()
{
	opencpu_printf("mqtt test1 start\n");
	opencpu_mqttcfg("a1baIzlRzFn.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883,"300001000100002320|securemode=3,signmethod=hmacsha1,timestamp=1562641624767|",
          60,"300001000100002320&a1baIzlRzFn","CFCA1B2E267EF1F6971722DD65F85D592B846CA6",1,0);
		  opencpu_mqttopen(1,1,0,0,0,0,0);
	while(opencpu_mqttstat() != 5)   
	{
		vTaskDelay(1000/portTICK_RATE_MS);
	}
	opencpu_mqttpub("a1baIzlRzFn/300001000100002320/user/pub/params",1,0,0,270,"7B22656F223A22333030303031303030313030303032333230222C2264617461223A7B2276616C7665223A5B7B225670223A302C2274223A313536373436363736373030307D5D2C2268656174223A5B7B227467223A32342E362C227468223A32342E382C2274223A313536373436363736373030302C2269735472616E73706172656E74223A307D5D2C226367223A5B7B223234223A3232302C223337223A33352C2274223A313536373436363736373030307D5D2C226661756C7473223A7B22697346616C7574223A312C226974656D73223A5B7B2274797065223A312C2274223A313536373436363736373030307D5D7D2C226368223A2D38372C2273657373696F6E4964223A22227D7D",opencpu_recvpuback_cb1);

	while(opencpu_mqttstat() == 5)
	{
		vTaskDelay(1000/portTICK_RATE_MS);
	}
	while(opencpu_mqttstat() != 1)
	{
		vTaskDelay(1000/portTICK_RATE_MS);
	}
	opencpu_mqttdel();
	opencpu_printf("mqtt test1 over\n");
}

/************************************************
onenet mqtt 套件非加密示例 
目前onenet 平台不支持自定义topic订阅
/************************************************/

mqtt_test2()
{
	opencpu_printf("mqtt test2 start\n");
	opencpu_mqttcfg("183.230.40.96",1883,"test",60,"247845","version=2018-10-31&res=products%2F247845%2Fdevices%2Ftest&et=1694376057&method=md5&sign=32%2B6jCEhKLdgXwdvpAQM8g%3D%3D",1);
	opencpu_mqttopen(1,1,0,0,0,0,0);
	while(opencpu_mqttstat() != 5)   
	{
		vTaskDelay(1000/portTICK_RATE_MS);
	}
	onenetFlag = 1;
	vTaskDelay(1000/portTICK_RATE_MS);
	opencpu_mqttsub("$sys/247845/test/dp/post/json/+",1,opencpu_userpublish_cb);
	vTaskDelay(1000/portTICK_RATE_MS);
	opencpu_mqttsub("$sys/247845/test/cmd/request/+",1,opencpu_userpublish_cb);
	vTaskDelay(1000/portTICK_RATE_MS);
	opencpu_mqttpub("$sys/247845/test/dp/post/json",0,0,0,66,"7b226964223a3132333435362c226470223a7b2274656d7065726174727565223a5b7b2276223a33302c7d5d2c22706f776572223a5b7b2276223a35352c7d5d7d7d",NULL);
	vTaskDelay(1000/portTICK_RATE_MS);
	opencpu_mqttpub("$sys/247845/test/dp/post/json",0,0,0,0,"{\"id\":123458,\"dp\":{\"temperatrue\":[{\"v\":31,}],\"power\":[{\"v\":58,}]}}",NULL);
	while(opencpu_mqttstat() == 5)
	{
		vTaskDelay(2000/portTICK_RATE_MS);
	}
	vTaskDelay(2000/portTICK_RATE_MS);
	opencpu_mqttdel();
	opencpu_printf("mqtt test2 over\n");
}
/*********************************************
onenet mqtt新套件加密连接示例
带$sys 为平台定义的特殊topic,订阅平台的特殊topic时，
将opencpu_mqttsub最后的回调函数置为NULL，默认调用 opencpu_defaultpublish_cb
"$sys/XXXX/XXXXt/dp/post/json"  数据在平台上显示
"$sys/247845/test/cmd/request/+": 订阅之后可接受平台下发的指令
**********************************************/
mqtt_test3()
{
	opencpu_printf("mqtt test3 start\n");
	opencpu_mqttcfg(HOST,PORT,CLIENTID,KEEPALIVE,USER,PWD,1);
	//设置为加密连接，在opencpu_mqttcfg之后设置
	opencpu_mqttencrypt(1);
	//写入平台给的证书
	opencpu_mqttca("MIIDOzCCAiOgAwIBAgIJAPCCNfxANtVEMA0GCSqGSIb3DQEBCwUAMDQxCzAJBgNVBAYTAkNOMQ4wDAYDVQQKDAVDTUlPVDEVMBMGA1UEAwwMT25lTkVUIE1RVFRTMB4XDTE5MDUyOTAxMDkyOFoXDTQ5MDUyMTAxMDkyOFowNDELMAkGA1UEBhMCQ04xDjAMBgNVBAoMBUNNSU9UMRUwEwYDVQQDDAxPbmVORVQgTVFUVFMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC/VvJ6lGWfy9PKdXKBdzY83OERB35AJhu+9jkx5d4SOtZScTe93Xw9TSVRKrFwu5muGgPusyAlbQnFlZoTJBZY/745MG6aeli6plpRr93G6qVN5VLoXAkvqKslLZlj6wXy70/e0GC0oMFzqSP0AY74icANk8dUFB2Q8usSUseRafNBcYfqACzF/Wa+Fu/upBGwtl7wDLYZdCm3KNjZZZstvVB5DWGnqNX9HkTlU9NBMS/7yph3XYU3mJqUZxryb8pHLVHazarNRppx1aoNroi+5/t3Fx/gEa6a5PoPouH35DbykmzvVE67GUGpAfZZtEFE1e0E/6IB84PE00llvy3pAgMBAAGjUDBOMB0GA1UdDgQWBBTTi/q1F2iabqlS7yEoX1rbOsz5GDAfBgNVHSMEGDAWgBTTi/q1F2iabqlS7yEoX1rbOsz5GDAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQALaqJ2FgcKLBBHJ8VeNSuGV2cxVYH1JIaHnzL6SlE5q7MYVg+Ofbs2PRlTiWGMazC7q5RKVj9zj0z/8i3ScWrWXFmyp85ZHfuo/DeK6HcbEXJEOfPDvyMPuhVBTzuBIRJb41M27NdIVCdxP6562n6Vp0gbE8kN10q+ksw8YBoLFP0D1da7D5WnSV+nwEIP+F4a3ZX80bNt6tRj9XY0gM68mI60WXrF/qYL+NUz+D3Lw9bgDSXxpSN8JGYBR85BxBvRNNAhsJJ3yoAvbPUQ4m8J/CoVKKgcWymS1pvEHmF47pgzbbjm5bdthlIx+swdiGFaWzdhzTYwVkxBaU+xf/2w");
	opencpu_mqttopen(1,1,0,0,0,0,0);
	while(opencpu_mqttstat() != 5)   
	{
		vTaskDelay(1000/portTICK_RATE_MS);
	}
	onenetFlag = 1;
	opencpu_mqttsub("$sys/247845/test/dp/post/json/+",1,NULL);
	vTaskDelay(500/portTICK_RATE_MS);
	opencpu_mqttsub("$sys/247845/test/cmd/request/+",1,NULL);
	vTaskDelay(500/portTICK_RATE_MS);
	opencpu_mqttsub("$sys/247845/test/cmd/response/+/+",1,NULL);
	//hex 形式向平台dp发送数据
	vTaskDelay(500/portTICK_RATE_MS);
	opencpu_mqttpub("$sys/247845/test/dp/post/json",0,0,0,66,"7b226964223a3132333435362c226470223a7b2274656d7065726174727565223a5b7b2276223a33302c7d5d2c22706f776572223a5b7b2276223a35352c7d5d7d7d",NULL);
	//json 形式向平台dp发送数据
	vTaskDelay(500/portTICK_RATE_MS);
	opencpu_mqttpub("$sys/247845/test/dp/post/json",1,0,0,0,"{\"id\":123458,\"dp\":{\"temperatrue\":[{\"v\":31,}],\"power\":[{\"v\":58,}]}}",NULL);
	vTaskDelay(500/portTICK_RATE_MS);
	while(opencpu_mqttstat() == 5)
	{
		vTaskDelay(2000/portTICK_RATE_MS);
	}
	vTaskDelay(2000/portTICK_RATE_MS);
	opencpu_mqttdel();
	opencpu_printf("mqtt test3 over\n");
}

