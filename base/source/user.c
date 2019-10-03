#include "user.h"

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
		PRINTF("任务名\t\t\t运行时间\t运行所占百分比\r\n");
		PRINTF("%s\r\n", RunTimeInfo);
		times--;
		if (times==0)
		{
			vTaskSuspend(RunTimeInfo_get_handler);
			times = 3;
		}
	}
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
	PRINTF(pcTaskName);
}
