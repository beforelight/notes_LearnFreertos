/*
 * user.h
 *
 *  Created on: 2019年10月3日
 *      Author: 17616
 */

#ifndef USER_H_
#define USER_H_
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_debug_console.h"
#include "arm_math.h"
#include "math.h"
#include "queue.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "timers.h"
void UserMain(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName);

#define START_TASK_PRIO 1
#define START_TASK_STK_SIZE 128
static TaskHandle_t start_task_handler;
void start_task(void* pvParameters);

#define PRINT_WORLD_PRIO 2
#define PRINT_WORLD_STK_SIZE 300
static  TaskHandle_t print_world_handler;
void print_world(void* pvParameters);

#define PRINT_NUM_PRIO 2
#define PRINT_NUM_STK_SIZE 500
static TaskHandle_t print_num_handler;
static float num = 128.0f;
void print_num(void* pvParameters);

#define RUNTIMEINFO_PRIO 0
#define RUNTIMEINFO_STK_SIZE 500
static TaskHandle_t RunTimeInfo_get_handler;
static char RunTimeInfo[256];
void RunTimeInfo_get(void* pvParameters);

#define QUEUE_SEND_PRIO 0
#define QUEUE_SEND_STK_SIZE 400
static TaskHandle_t queue_send_handler;
void queue_send(void* pvParameters);
static QueueHandle_t queue0;
#define QUEUE_RECEIVE_PRIO QUEUE_SEND_PRIO+1
#define QUEUE_RECEIVE_STK_SIZE 400
static TaskHandle_t queue_receive_handler;
void queue_receive(void* pvParameters);
#endif /* USER_H_ */
