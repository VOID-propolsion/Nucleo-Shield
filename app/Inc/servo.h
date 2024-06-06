#ifndef SERVO_H
#define SERVO_H

#include "main.h"
#include "tim.h"

#ifdef __cplusplus
extern "C" {
#endif

void Servo_Init(void);
void Set_Servo_Angle(uint16_t angle);

#ifdef __cplusplus
}
#endif

#endif // SERVO_H
