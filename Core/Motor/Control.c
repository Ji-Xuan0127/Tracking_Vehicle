#include "main.h"
#include "tim.h"

void Move_Forward(int16_t speed)
{
    HAL_GPIO_WritePin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin, GPIO_PIN_RESET);

  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, speed);
  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, speed);
}

void Move_Backward(int16_t speed)
{
    HAL_GPIO_WritePin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin, GPIO_PIN_SET);

  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, speed);
  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, speed);
}

void Turn_Left(int16_t speed)
{
    HAL_GPIO_WritePin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin, GPIO_PIN_RESET);

  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, -speed);
  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, speed);
}

void Turn_Right(int16_t speed)
{
    HAL_GPIO_WritePin(Motor1_Con1_GPIO_Port, Motor1_Con1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Motor1_Con2_GPIO_Port, Motor1_Con2_Pin, GPIO_PIN_RESET);

  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, speed);
  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, -speed);
}

void Set_Speed(int16_t speed1, int16_t speed2)
{
  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, speed1);
  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, speed2);
}

void Stop_Motor(void)
{
  __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, 0);
}

void Start_Motor(void)
{
  uint8_t Start_Flag = 0;
 if(HAL_GPIO_ReadPin(Start_in_GPIO_Port, Start_in_Pin) == GPIO_PIN_SET)
 {
   Start_Flag = !Start_Flag;
 }
 if(Start_Flag)
 {
   HAL_GPIO_WritePin(Start_out_GPIO_Port, Start_out_Pin, GPIO_PIN_SET);
 }
 else
 {
   HAL_GPIO_WritePin(Start_out_GPIO_Port, Start_out_Pin, GPIO_PIN_RESET);
 }
}
