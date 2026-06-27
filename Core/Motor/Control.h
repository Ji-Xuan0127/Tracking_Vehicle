#ifndef __CONTROL_H
#define __CONTROL_H

void Move_Forward(uint16_t speed);
void Move_Backward(uint16_t speed);
void Turn_Left(uint16_t speed);
void Turn_Right(uint16_t speed);
void Set_Speed(int16_t speed1, int16_t speed2);
void Stop_Motor(void);
void Start_Motor(void);

#endif /* __CONTROL_H */
