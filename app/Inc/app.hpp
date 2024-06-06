#pragma once
#include "main.h"
#include "tim.h"
#include "rflink.h"
#include "LPS22HB.h"
#include "servo.h"

#define BOARD_BUTTON        GPIO_PIN_13 // b
#define BOARD_LED           GPIO_PIN_5 // a

#define SHIELD_LED_1        GPIO_PIN_15 // a
#define SHIELD_LED_2        GPIO_PIN_1 // a

#define SHIELD_SWITCH_1     GPIO_PIN_8 // C
#define SHIELD_SWITCH_2     GPIO_PIN_7 // C
#define SHIELD_SWITCH_3     GPIO_PIN_5 // C
#define SHIELD_SWITCH_4     GPIO_PIN_4 // C

#ifdef __cplusplus

extern "C"
{
#endif

    void setup();
    void loop();
    int handleUartInput(uint8_t* buffer);

#ifdef __cplusplus
}
#endif