#include "app.hpp"
#include "main.h"

extern "C"
{

    void setup()
    {
    }

    void loop()
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
        HAL_Delay(1000);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
        HAL_Delay(100);
    }
}