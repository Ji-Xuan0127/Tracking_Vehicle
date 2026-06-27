/*
 * key.h
 *
 *  Created on: Apr 14, 2026
 *      Author: JiXuan
 */

#ifndef INC_KEY_H_
#define INC_KEY_H_

#include "main.h"

uint8_t Key_GetState(void);
void Key_Tick(void);
uint8_t Key_GetNum(void);

#endif /* INC_KEY_H_ */
