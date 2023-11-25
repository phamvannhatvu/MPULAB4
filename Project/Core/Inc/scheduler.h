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

	// Delay (in ticks) until the function will (next) be run (from when the
	// preceding task will be executed)
	uint32_t delay;

	// Interval (in ticks) between subsequent runs
	uint32_t period;

	// Incremented (by scheduler) when task is due to execute
	uint8_t runMe;
} sTask;

void SCH_Init(void);
void SCH_Update(void);
void SCH_Delete_Task(uint8_t);
void SCH_Add_Task(void (*)(void), uint32_t, uint32_t);
uint8_t SCH_Find_Task(void (*)(void));
void SCH_Dispatch_Tasks(void);


#endif /* INC_SCHEDULER_H_ */
