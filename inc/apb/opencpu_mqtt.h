/*******************************************************************************
 
 *******************************************************************************/
#ifndef __CMMQTT_OC_C_
#define __CMMQTT_OC_C_

/*****************************************************
参数配置:
server : 服务器地址
port: 端口号
client_id : 客户id
keepAlive : 发送ping包间隔
user: 用户名
passwd:密码
clean : 连接时是否清除之前的seesion
返回 :配置成功返回0，配置失败返回其他值。
notice :处于UNINITIALED 状态时才可进行配置
******************************************************/
int opencpu_mqttcfg(char* server, uint32_t port, char* client_id,  uint32_t keepAlive, char* user, char* passwd, uint32_t clean);

/*****************************************************
建立连接:
user_flag : 用户标志(0-1)
pwd_flag:   密码标志(0-1)
will_flag : willmsg标志(0-1)
retain_flag : 持久化标志(0-1)
will_qos:  qos(0-2)
will_topic: willmsg topic
will_msg : willmsg
返回 :发起成功返回0，失败返回其他值。异步连接模式，最终连接成功需判断根据是否收到MQTTOPEN:OK
notice: 处于INITIALED 转态势可发起连接，连接成功后状态会变为CONNECTED
******************************************************/
int opencpu_mqttopen(uint32_t user_flag, uint32_t pwd_flag, uint32_t will_flag, uint32_t retain_flag, uint32_t will_qos, char* will_topic, char* will_msg);

/*****************************************************
查询当前mqtt client 状态:
state : 
	UNINITIALED	 :0
	INITIALED	 :1
	DISCONNECTED :2
	CONNECTING   :3
	RECONNECTING :4
	CONNECTED    :5
	NET_CONNECTING: 6
	NET_CONNECTED :7
返回 :state 状态值
notice: 可根据当前转态进行合理的操作
******************************************************/
int opencpu_mqttstat();

/*****************************************************
订阅topic:
topic : 订阅的topic名字
qos:  (0-2)
user_recvpubcb: 留给客户完成的回调函数,处理接收到推送消息，不使用时输入NULL
返回 :配置成功返回0，配置失败返回其他值。
notice: 处于CONNNECTED 状态时可以发送
******************************************************/
int opencpu_mqttsub(char *topic, uint32_t qos, void(*user_recvpubcb));

/*****************************************************
取消订阅topic:
topic : 取消订阅的topic名字
返回 :配置成功返回0，配置失败返回其他值。
notice: notice: 处于CONNNECTED 状态时可以发送
******************************************************/
int opencpu_mqttunsub(char *topic, uint32_t qos);

/*****************************************************
推送信息:
topic: topic名字
qos:  (0-2) 
retain: (0-1)
dup: 	(0-1)
msg_len: 0: 以字符串形式发送；非0 :以hex形式发送，注意发送长度与msg对应
msg: 待发送的信息。
user_ackcb: 留给客户完成的回调函数，推送信息收到ack后会触发该函数,不使用时输入NULL
返回 :发起成功返回0，失败返回其他值。
notice: 处于CONNNECTED 状态时可以发送
******************************************************/
int opencpu_mqttpub(char *topic, uint32_t qos, uint32_t retain, uint32_t dup, uint32_t msg_len, char *msg, void(*user_ackcb));

/*****************************************************
断开连接:
返回 :成功返回0。
notice: 处于CONNNECTED 状态时可以进行断开操作，断开成功后变为DISCONNECTED状态
******************************************************/
int opencpu_mqttdisc();

/*****************************************************
释放资源:
返回 :成功返回0。
notice: 处于CONNNECTED 状态时可以进行断开操作
******************************************************/
int opencpu_mqttdel();


/******************************************************
以下是回调函数的编写说明: 对于异步接收数据，目前开放两种回调函数:
1: publish ack 的回调函数: 对于qos>1的推送信息，收到ack后会触发该
类型的回调函数，用户可根据自己的需要对每一个推送包编写所需的回调函数，
可以一次编写若干的回调函数，函数地址传入opencpu_mqttpub 的最后一个参数
即可。例如
void opencpu_recvpuback_cb()
{
	opencpu_printf("OC get excepted ack\n");
}
/*********************************************************************
 2，接收到某一topic推送信息的回调函数，有一个默认的回调函数，用户可仿造
 该函数(opencpu_defaultpublish_cb)编写其它所需的回调函数,
/******************************************************************
opencpu_defaultpublish_cb : 默认的一个异步回调函数，接收到服务器下发的数据时会触发该函数，
dup: 0 非重复包，1，重复包 
qos:  (0-2) 
retain: (0-1)
topicname: topic名字
msgid:服务器下发的包序号
msg_len: 接收到的数据长度 
msg: 服务器下发的数据
notice: 客户在订阅主题(opencpu_mqttsub)时，若最后一个参数为NULL时，
收到数据默认调用该函数；若用户针对某topic有自己的处理，参考该函数
重新写新的回调函数传入opencpu_mqttsub即可。
/*****************************************************/
void opencpu_defaultpublish_cb(uint32_t dup, uint32_t qos, uint32_t retained, uint32_t msgid, char *topicname, uint32_t msglen, char *msg);




#endif
