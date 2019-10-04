#include "user.h"
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
	PRINTF(pcTaskName);
}
void UserMain(void)
{
	//创建开始任务
	xTaskCreate(start_task, "start_task", START_TASK_STK_SIZE, NULL, START_TASK_PRIO, &start_task_handler);
	//开始调度
	vTaskStartScheduler();
}

void start_task(void* pvParameters)
{
	//进入临界区
	taskENTER_CRITICAL();
	xTaskCreate(print_world, "print_world", PRINT_WORLD_STK_SIZE,NULL, PRINT_WORLD_PRIO, &print_world_handler);
	xTaskCreate(print_num, "print_num", PRINT_NUM_STK_SIZE,NULL, PRINT_NUM_PRIO, &print_num_handler);
	queue0 = xQueueCreate(4, sizeof(char));
	xTaskCreate(queue_send, "queue_send", QUEUE_SEND_STK_SIZE,NULL, QUEUE_SEND_PRIO, &queue_send_handler);
	xTaskCreate(queue_receive, "queue_receive", QUEUE_RECEIVE_STK_SIZE,NULL, QUEUE_RECEIVE_PRIO, &queue_receive_handler);
	xTaskCreate(RunTimeInfo_get, "RunTimeInfo_get", RUNTIMEINFO_STK_SIZE,NULL, RUNTIMEINFO_PRIO, &RunTimeInfo_get_handler);
	//退出临界区
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
}


void print_world(void* pvParameters)
{
	while (1)
	{
		PRINTF("Hello World\n");
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

void print_num(void* pvParameters)
{
	float num_b = num;
	while (1)
	{
		arm_sqrt_f32(num_b, &num);
		PRINTF("%f = sqrt( %f )\n", num, num_b);
		num_b = num;
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void RunTimeInfo_get(void* pvParameters)
{
	uint8_t times = 3;
	memset(RunTimeInfo, 0, sizeof(RunTimeInfo));
	while (1)
	{
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		vTaskGetRunTimeStats(RunTimeInfo);
		taskENTER_CRITICAL();
		PRINTF("任务名\t\t\t运行时间us\t运行所占百分比\r\n");
		PRINTF("%s\r\n", RunTimeInfo);
		taskEXIT_CRITICAL();
		times--;
		if (times==0)
		{
			vTaskSuspend(RunTimeInfo_get_handler);
			times = 3;
		}
	}
}

void queue_send(void* pvParameters)
{
	PRINTF("0123 to on 4567 to off\n");
	char q;
	int result;
	while (1)
	{
		q=GETCHAR();
		result=xQueueSend(queue0, &q, portMAX_DELAY);//一直等待发送
	}
}

void queue_receive(void* pvParameters)
{
	char q;
	int result;
	while (1)
	{
		result=xQueueReceive(queue0, &q, portMAX_DELAY);//一直阻塞等待消息
		if (q=='0'){GPIO_PinWrite(LED1_GPIO, LED1_PIN, 0); PRINTF("LED1 on\n");}
		else if (q == '1') { GPIO_PinWrite(LED2_GPIO, LED2_PIN, 0); PRINTF("LED1 on\n"); }
		else if (q == '2') { GPIO_PinWrite(LED3_GPIO, LED3_PIN, 0); PRINTF("LED2 on\n"); }
		else if (q == '3') { GPIO_PinWrite(LED4_GPIO, LED4_PIN, 0); PRINTF("LED3 on\n"); }
		else if (q == '4') { GPIO_PinWrite(LED1_GPIO, LED1_PIN, 1); PRINTF("LED4 off\n"); }
		else if (q == '5') { GPIO_PinWrite(LED2_GPIO, LED2_PIN, 1); PRINTF("LED2 off\n"); }
		else if (q == '6') { GPIO_PinWrite(LED3_GPIO, LED3_PIN, 1); PRINTF("LED3 off\n"); }
		else if (q == '7') { GPIO_PinWrite(LED4_GPIO, LED4_PIN, 1); PRINTF("LED4 off\n"); }
	}
}