#include "app.hpp"
#include "main.h"
#include "stm32f4xx_radio.h"

STM32Hal* hal = new STM32Hal(
    ENCODE_GPIO_PIN(GPIOB, GPIO_PIN_3), // SCK
    ENCODE_GPIO_PIN(GPIOB, GPIO_PIN_4), // MISO
    ENCODE_GPIO_PIN(GPIOB, GPIO_PIN_5) // MOSI
);
SX1280 radio = new Module(hal, 
    ENCODE_GPIO_PIN(GPIOA, GPIO_PIN_4), // Chip select
    0, // interupt 
    ENCODE_GPIO_PIN(GPIOA, GPIO_PIN_10) // reset
);

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

        DEBUG("[SX1280] Initializing ... \n");
        int state = radio.begin();
        while (state != RADIOLIB_ERR_NONE) {
            DEBUG("initializing failed: %d\n", state);
            HAL_Delay(5000);
            DEBUG("attempting to connect again...\n");
            state = radio.begin();
        }
        DEBUG("success!\n");
    }

    void loop()
    {
        DEBUG("[SX1276] Transmitting packet ... ");
        int state = radio.transmit("Hello World!");
        if(state == RADIOLIB_ERR_NONE) {
            // the packet was successfully transmitted
            DEBUG("success!");
        } else {
            DEBUG("failed");
        }
            // send a packet
        //DEBUG("this is a test\r\n");
        // if (!HAL_GPIO_ReadPin(GPIOC, BOARD_BUTTON)) {
        //     for (int i = 0; i < 4; i++) {
        //         HAL_GPIO_WritePin(GPIOA, SHIELD_LED_2, GPIO_PIN_SET);
        //         if (!HAL_GPIO_ReadPin(GPIOC, switches[i])) {
        //             HAL_GPIO_WritePin(GPIOA, SHIELD_LED_1, GPIO_PIN_SET);
        //         }
        //         HAL_Delay(250);
        //         HAL_GPIO_WritePin(GPIOA, SHIELD_LED_1, GPIO_PIN_RESET);
        //         HAL_GPIO_WritePin(GPIOA, SHIELD_LED_2, GPIO_PIN_RESET);
        //         HAL_Delay(250);
        //     }
        //     while (!HAL_GPIO_ReadPin(GPIOC, BOARD_BUTTON)) {
        //         HAL_Delay(100);
        //     }
        // }
        HAL_Delay(100);
        
    }
}
