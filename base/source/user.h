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
void UserMain(void);

#define START_TASK_PRIO 1
#define START_TASK_STK_SIZE 128
static TaskHandle_t start_task_handler;
void start_task(void* pvParameters);

#define PRINT_WORLD_PRIO 2
#define PRINT_WORLD_STK_SIZE 1024
static  TaskHandle_t print_world_handler;
void print_world(void* pvParameters);

#define PRINT_NUM_PRIO 3
#define PRINT_NUM_STK_SIZE 1024
static TaskHandle_t print_num_handler;
static float num = 128.0f;
void print_num(void* pvParameters);

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName);
#endif /* USER_H_ */
