/*
 * button_reading.c
 *
 *  Created on: Nov 25, 2023
 *      Author: ADMIN
 */

#include "button_reading.h"

//Previous button reading value
GPIO_PinState prevBtnReadVal[2] = {RELEASED, RELEASED};
GPIO_PinState buttonState = RELEASED;

uint32_t pressedCounter = 0;
uint8_t pToggleIndex = SCH_MAX_TASKS;

void LED_BTN_toggle(void)
{
	HAL_GPIO_TogglePin(LED_BTN_GPIO_Port, LED_BTN_Pin);
}

void read_button(void)
{
	GPIO_PinState curBtnReadVal = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
	if (prevBtnReadVal[0] == prevBtnReadVal[1] && prevBtnReadVal[1] == curBtnReadVal)
	{
		if (curBtnReadVal != buttonState)
		{
			buttonState = curBtnReadVal;
			if (buttonState == PRESSED)
			{
				HAL_GPIO_TogglePin(LED_BTN_GPIO_Port, LED_BTN_Pin);
			}else
			{
				if (pToggleIndex != SCH_MAX_TASKS)
				{
					SCH_Delete_Task(pToggleIndex);
				}
				pToggleIndex = SCH_MAX_TASKS;
				pressedCounter = 0;
			}
		}else
		{
			if (buttonState == PRESSED)
			{
				//check for press more than 1s
				++pressedCounter;
				if (pressedCounter * TICK_DURATION >= 1000)
				{
					if (pToggleIndex == SCH_MAX_TASKS)
					{
						pToggleIndex = SCH_Add_Task(LED_BTN_toggle, 0, 500);
					}
				}
			}
		}
	}
	prevBtnReadVal[0] = prevBtnReadVal[1];
	prevBtnReadVal[1] = curBtnReadVal;
}
