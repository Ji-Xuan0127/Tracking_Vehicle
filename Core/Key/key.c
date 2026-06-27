/*
 * key.c
 *
 *  Created on: Apr 14, 2026
 *      Author: JiXuan
 */
#include "key.h"

uint8_t Key_Num;

uint8_t Key_GetState(void)
{
	if(HAL_GPIO_ReadPin(Motor_Start_GPIO_Port, Motor_Start_Pin) == GPIO_PIN_SET){
		return 1;
	}
	if(HAL_GPIO_ReadPin(Motor_up_GPIO_Port, Motor_up_Pin) == GPIO_PIN_SET){
		return 2;
	}
	if(HAL_GPIO_ReadPin(Motor_down_GPIO_Port, Motor_down_Pin) == GPIO_PIN_SET){
		return 3;
	}
	return 0;
}

void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrState, PrevState;

	Count++;
	if(Count >= 20){
		Count = 0;

		PrevState = CurrState;
		CurrState = Key_GetState();

		if(CurrState == 0 && PrevState != 0){
			Key_Num = PrevState;
		}
	}
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if(Key_Num){
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
}
