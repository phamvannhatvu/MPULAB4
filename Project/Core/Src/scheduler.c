/*
 * scheduler.c
 *
 *  Created on: Nov 15, 2023
 *      Author: ADMIN
 */

#include "scheduler.h"

sTask SCH_tasks_G[SCH_MAX_TASKS];
uint8_t numOfTasks = 0;
uint32_t jitterTimes = 0;

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
	if (SCH_tasks_G[0].delay > 0)
	{
		--SCH_tasks_G[0].delay;
	}else
	{
		// number of time the tasks are "jitter" because of a long task
		++jitterTimes;
	}
}

void SCH_Delete_Task(uint8_t index)
{
	if (index < numOfTasks)
	{
		if (index < numOfTasks - 1)
		{
			SCH_tasks_G[index + 1].delay += SCH_tasks_G[index].delay;
		}
		for (uint8_t i = index; i < numOfTasks - 1; ++i)
		{
			SCH_tasks_G[i].delay = SCH_tasks_G[i + 1].delay;
			SCH_tasks_G[i].period = SCH_tasks_G[i + 1].period;
			SCH_tasks_G[i].pTask = SCH_tasks_G[i + 1].pTask;
		}
		--numOfTasks;
		SCH_tasks_G[numOfTasks].pTask = 0;
		SCH_tasks_G[numOfTasks].delay = 0;
		SCH_tasks_G[numOfTasks].period = 0;
	}
}

void SCH_Dispatch_Tasks(void)
{
	do
	{
		// Execute the first tasks whose delay is zero (ready to be run)
		while (numOfTasks > 0 && SCH_tasks_G[0].delay == 0)
		{
			// Run the task
			(*SCH_tasks_G[0].pTask)();
			if (SCH_tasks_G[0].period > 0)
			{
				SCH_Add_Task(SCH_tasks_G[0].pTask, SCH_tasks_G[0].period * TICK_DURATION,
						SCH_tasks_G[0].period * TICK_DURATION);
			}
			SCH_Delete_Task(0);
		}

		// Compensate the "jitter" times by reducing the "delay" attribute of
		// the remaining tasks
		if (numOfTasks > 0)
		{
			for (uint8_t i = 0; i < numOfTasks; ++i)
			{
				if (jitterTimes > SCH_tasks_G[i].delay)
				{
					jitterTimes -= SCH_tasks_G[i].delay;
					SCH_tasks_G[i].delay = 0;
				}else
				{
					SCH_tasks_G[i].delay -= jitterTimes;
					jitterTimes = 0;
					break;
				}
			}
		}
	}while (jitterTimes > 0 && numOfTasks > 0);

	SCH_Report_Status();
	SCH_Go_To_Sleep();
}

// delay and period parameter is in ms unit
void SCH_Add_Task(void (*pFunction)(void), uint32_t delay, uint32_t period)
{
	// execute immediately
	if (delay == 0)
	{
		(*pFunction)();
		if (period > 0)
		{
			SCH_Add_Task(pFunction, period, period);
		}
	}else
	{
		delay /= TICK_DURATION;
		period /= TICK_DURATION;

		if (numOfTasks < SCH_MAX_TASKS)
		{
			uint32_t curSumDelay = 0;
			uint8_t insertPosition = numOfTasks;

			for (uint8_t i = 0; i < numOfTasks; ++i)
			{
				curSumDelay += SCH_tasks_G[i].delay;
				if (curSumDelay > delay)
				{
					insertPosition = i;
					break;
				}
			}

			// The new task is not added at the end of the array
			if (insertPosition != numOfTasks)
			{
				curSumDelay -= SCH_tasks_G[insertPosition].delay;
				for (uint8_t i = numOfTasks; i > insertPosition; --i)
				{
					SCH_tasks_G[i].delay = SCH_tasks_G[i - 1].delay;
					SCH_tasks_G[i].period = SCH_tasks_G[i - 1].period;
					SCH_tasks_G[i].pTask = SCH_tasks_G[i - 1].pTask;
				}
				SCH_tasks_G[insertPosition + 1].delay -= (delay - curSumDelay);
			}

			SCH_tasks_G[insertPosition].delay = delay - curSumDelay;
			SCH_tasks_G[insertPosition].period = period;
			SCH_tasks_G[insertPosition].pTask = pFunction;
			++numOfTasks;
		}
	}
}

// Get index of a task based on the respective function pointer
uint8_t SCH_Find_Task(void (*pFunction)(void))
{
	for (uint8_t i = 0; i < SCH_MAX_TASKS; ++i)
	{
		if (SCH_tasks_G[i].pTask == pFunction)
		{
			return i;
		}
	}

	// SCH_MAX_TASKS return value means the given task hasn't been added yet
	return SCH_MAX_TASKS;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim2.Instance)
	{
		++display_counter;
		SCH_Update();
	}
}


