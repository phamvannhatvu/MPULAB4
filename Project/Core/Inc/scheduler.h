/*
 * scheduler.h
 *
 *  Created on: Nov 15, 2023
 *      Author: ADMIN
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include "global.h"

#define SCH_MAX_TASKS 10
#define TICK_DURATION 10
#define RUN_QUEUE_MAX_SIZE 10

typedef struct
{
	// Pointer to the task
	void (*pTask)(void);

	// Delay (in ticks) until the function will (next) be run (after the preceding task or from the current point in time if it is the first task)
	uint32_t delay;

	// Interval (in ticks) between subsequent runs
	uint32_t period;
} sTask;

extern uint32_t jitterTimes;

void SCH_Init(void);
void SCH_Update(void);
// 0 means delete successfully
uint8_t SCH_Delete_Task(uint32_t);
uint32_t SCH_Add_Task(void (*)(void), uint32_t, uint32_t);
uint8_t SCH_Find_Task(void (*)(void));
void SCH_Dispatch_Tasks(void);


#endif /* INC_SCHEDULER_H_ */
