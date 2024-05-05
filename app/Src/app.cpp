#include "app.hpp"
#include "main.h"
#include "RadioLib.h"

int switches[4] = {SHIELD_SWITCH_1, SHIELD_SWITCH_2, SHIELD_SWITCH_3, SHIELD_SWITCH_4};

extern "C"
{
    void setup()
    {
    }

    void loop()
    {
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
