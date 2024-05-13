#include "app.hpp"
#include "main.h"
#include "stm32f4xx_radio.h"

STM32Hal* hal = new STM32Hal(GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5);
SX1280 radio = new Module(hal, GPIO_PIN_4, 0, GPIO_PIN_10);

int switches[4] = {SHIELD_SWITCH_1, SHIELD_SWITCH_2, SHIELD_SWITCH_3, SHIELD_SWITCH_4};

extern "C"
{
    void setup()
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin     = BOARD_BUTTON;
        GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull    = GPIO_PULLUP;
        GPIO_InitStruct.Speed   = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        HAL_GPIO_WritePin(GPIOA, BOARD_LED, GPIO_PIN_SET);
    }

    void loop()
    {
        //DEBUG("this is a test\r\n");
        if (!HAL_GPIO_ReadPin(GPIOC, BOARD_BUTTON)) {
            for (int i = 0; i < 4; i++) {
                HAL_GPIO_WritePin(GPIOA, SHIELD_LED_2, GPIO_PIN_SET);
                if (!HAL_GPIO_ReadPin(GPIOC, switches[i])) {
                    HAL_GPIO_WritePin(GPIOA, SHIELD_LED_1, GPIO_PIN_SET);
                }
                HAL_Delay(250);
                HAL_GPIO_WritePin(GPIOA, SHIELD_LED_1, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOA, SHIELD_LED_2, GPIO_PIN_RESET);
                HAL_Delay(250);
            }
            while (!HAL_GPIO_ReadPin(GPIOC, BOARD_BUTTON)) {
                HAL_Delay(100);
            }
        }
        HAL_Delay(100);
    }
}
