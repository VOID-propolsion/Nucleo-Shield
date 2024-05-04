#include "stm32f4xx_hal.h"

#define LED_PIN                     GPIO_PIN_5
#define BOARD_LED_15                GPIO_PIN_15
#define BOARD_LED_1                 GPIO_PIN_1
#define LED_GPIO_PORT               GPIOA
#define LED_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()

int main(void) {
    HAL_Init();

    LED_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    // Initialize pin 5 (LED_PIN)
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLUP;
    GPIO_InitStruct.Speed   = GPIO_SPEED_HIGH;

    GPIO_InitStruct.Pin     = LED_PIN;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

    // Initialize pin 15 (BOARD_LED)
    GPIO_InitStruct.Pin     = BOARD_LED_15;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

    // Initialize pin 1 (BOARD_LED)
    GPIO_InitStruct.Pin     = BOARD_LED_1;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
    int count = 0;
    while (1) {
        // Toggle pin 5
        if (count % 2) HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        
        // Toggle pin 15
        if (count % 3 == 0) HAL_GPIO_TogglePin(LED_GPIO_PORT, BOARD_LED_15);

        // Toggle pin 1
        if (count % 5 == 0) HAL_GPIO_TogglePin(LED_GPIO_PORT, BOARD_LED_1);

        count++;
        count = count %5;
        HAL_Delay(250);
    }
}
