#include "m5311_opencpu.h"

int is_andlink_ref = 0;//是否注册

void opencpu_andlink_event_cb(int event)
{
	opencpu_printf("event:%d\n",event);
	switch(event)
	{
		case CIS_EVENT_REG_SUCCESS: 
		{
			is_andlink_ref = 1;
			opencpu_printf("andlink register done!\n");
			break;
		}
		case CIS_EVENT_UNREG_DONE:
		{
			is_andlink_ref = 0;
			opencpu_printf("andlink register failed\n");
			break;
		}
		default:
     		break;
	}
}

void opencpu_andlink_notify_cb(int ack_id)
{
    //TODO
    //...
    opencpu_printf("ack_id:%d\n",ack_id);
}

void opencpu_andlink_read_cb_error(int mid, int objid, int insid, int resid)
{
    //TODO
    //...
    opencpu_andlink_result(mid, RESULT_400_BADREQUEST);//返回表示该操作结果错误
}

void opencpu_andlink_read_cb_ok(int mid, int objid, int insid, int resid)
{
    //TODO
    //...
    //根据传入的objid_insid_resid决定需上传的数据
	char strread[100] = "{\"n\":\"/19/1/0\",\"vs\":{\"gas\":2}}";

    if(objid == 19) 
    {
        opencpu_andlink_read(mid, strread);//返回表示该操作结果数据,示例资源类型为string型
    }
    else
    {
        opencpu_andlink_result(mid, RESULT_400_BADREQUEST);//返回表示该操作结果错误
    }
}

void opencpu_andlink_write_cb(int mid, int objid,int insid, int resid, int type, int flag, int len, char *data)
{
    //TODO
    //...
    opencpu_printf("write:%d %s\n",len,data);
    opencpu_andlink_result(mid, RESULT_204_CHANGED);//操作正确完成返回204
}

void opencpu_andlink_execute_cb(int mid, int objid, int insid, int resid, int len, char *data)
{
    //TODO
    //...
    opencpu_printf("write:%d %s\n",len,data);
    opencpu_andlink_result(mid, RESULT_204_CHANGED);//操作正确完成返回204
}

void opencpu_andlink_parameter_cb(int mid,int objid, int insid, int resid, int len, char *parameter)
{
    //TODO
    //...
    opencpu_andlink_result(mid, RESULT_204_CHANGED);//操作正确完成返回204
}

void test_andlink()
{
	cot_cb_t callback;
	char strnotify[100] = "{\"n\":\"/19/1/0\",\"vs\":{\"gas\":2}}";
	char device_id[10] = "500240";

	callback.onRead = opencpu_andlink_read_cb_ok;
	callback.onWrite = opencpu_andlink_write_cb;
	callback.onExec = opencpu_andlink_execute_cb;
	callback.onObserve = NULL;
	callback.onParams = opencpu_andlink_parameter_cb;
	callback.onEvent = opencpu_andlink_event_cb;
	callback.onNotify = opencpu_andlink_notify_cb;
	callback.onDiscover = NULL;
	
	opencpu_andlink_init();//初始化任务
	opencpu_andlink_create("lwm.home.komect.com", 13, NULL, &callback);//创建设备
	opencpu_andlink_config_set("DEVICETYPE", device_id);
   	opencpu_andlink_add_obj(19, 1, "1", 0, 0);
    cot_printf("open andlink");
	if(opencpu_andlink_open(86400, 30) == 0)
    {
        while(!is_andlink_ref)
        {
            vTaskDelay(100);//休眠
        }
        vTaskDelay(3000);//休眠
        cot_printf("cot_andlink_cm_notify_cmd start %s\n", strnotify);
        opencpu_andlink_notify(5678, strnotify);

        while(is_andlink_ref)
        {
            vTaskDelay(100);//休眠
        }
        opencpu_andlink_del_obj(19);
        cot_printf("close andlink");
        opencpu_andlink_close(4);//清理设备
    }
	
}