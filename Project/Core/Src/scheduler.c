/*
 * scheduler.c
 *
 *  Created on: Nov 15, 2023
 *      Author: ADMIN
 */

#include "scheduler.h"

sTask SCH_tasks_G[SCH_MAX_TASKS];

void Timer_Init()
{
	HAL_TIM_Base_Start_IT(&htim2);
}

void SCH_Report_Status()
{

}

void SCH_Go_To_Sleep()
{

}

void SCH_Init(void)
{
	// Reset the value of non-empty tasks
	for (uint8_t i = 0; i < SCH_MAX_TASKS; ++i)
	{
		SCH_Delete_Task(i);
	}
	Timer_Init();
}

void SCH_Update(void)
{
	for (uint8_t i = 0; i < SCH_MAX_TASKS; ++i)
	{
		// If there is a task in this location
		if (SCH_tasks_G[i].pTask)
		{
			if (SCH_tasks_G[i].delay <= 0)
			{
				// The task is due to run
				++SCH_tasks_G[i].runMe;
				// If the task is run periodically
				if (SCH_tasks_G[i].period)
				{
					SCH_tasks_G[i].delay = SCH_tasks_G[i].period;
				}
			}else
			{
				--SCH_tasks_G[i].delay;
			}
		}
	}
}

void SCH_Delete_Task(uint8_t index)
{
	if (index < SCH_MAX_TASKS)
	{
		SCH_tasks_G[index].pTask = 0;
		SCH_tasks_G[index].delay = 0;
		SCH_tasks_G[index].period = 0;
		SCH_tasks_G[index].runMe = 0;
	}
}

void SCH_Dispatch_Tasks(void)
{
	for (uint8_t i = 0; i < SCH_MAX_TASKS; ++i)
	{
		// This task is ready to run
		if (SCH_tasks_G[i].runMe > 0)
		{
			// Run the task
			(*SCH_tasks_G[i].pTask)();
			--SCH_tasks_G[i].runMe;
			if (SCH_tasks_G[i].period == 0)
			{
				SCH_Delete_Task(i);
			}
		}
	}
	SCH_Report_Status();
	SCH_Go_To_Sleep();
}


void SCH_Add_Task(void (*pFunction)(void), uint32_t delay, uint32_t period)
{
	uint8_t index = 0;
	while ((index < SCH_MAX_TASKS) && (SCH_tasks_G[index].pTask != 0))
	{
		++index;
	}
	if (index < SCH_MAX_TASKS)
	{
		SCH_tasks_G[index].pTask = pFunction;
		SCH_tasks_G[index].delay = delay / TICK_DURATION;
		SCH_tasks_G[index].period = period / TICK_DURATION;
		SCH_tasks_G[index].runMe = 0;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim2.Instance)
	{
		SCH_Update();
	}
}


