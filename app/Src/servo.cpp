#include "servo.h"
#include "spi.h"
#include "main.h"

void Servo_Init(void) {
    // Ensure TIM3 is initialized for PWM
    MX_TIM3_Init();

    // Start PWM on TIM3 Channel 1
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void Set_Servo_Angle(uint16_t angle) {
        if(angle > 180 || angle < 0){
            DEBUG("Error! Servo angle should be in range: [0,180]\n");
        }
        
        uint16_t pulseWidth = (int) (1000.00*(angle/180.00) + 250.00);
        DEBUG("angle %d : PWM: %d\n",angle, pulseWidth);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulseWidth);
}
