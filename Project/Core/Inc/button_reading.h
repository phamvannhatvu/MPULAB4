/*
 * button_reading.h
 *
 *  Created on: Nov 25, 2023
 *      Author: ADMIN
 */

#ifndef INC_BUTTON_READING_H_
#define INC_BUTTON_READING_H_

#include "main.h"
#include "scheduler.h"

#define PRESSED GPIO_PIN_RESET
#define RELEASED GPIO_PIN_SET

void read_button(void);

#endif /* INC_BUTTON_READING_H_ */
