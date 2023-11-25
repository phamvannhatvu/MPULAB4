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

typedef struct
{
	// Pointer to the task
	void (*pTask)(void);

	// Delay (in ms) until the function will (next) be run
	uint32_t delay;

	// Interval (in ms) between subsequent runs
	uint32_t period;

	// Incremented (by scheduler) when task is due to execute
	uint8_t runMe;
} sTask;

void SCH_Init(void);
void SCH_Update(void);
void SCH_Delete_Task(uint8_t);
void SCH_Add_Task(void (*)(void), uint32_t, uint32_t);
void SCH_Dispatch_Tasks(void);


#endif /* INC_SCHEDULER_H_ */
