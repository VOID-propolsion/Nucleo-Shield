#include "pins.h"
#include "main.h"

void pinMode(uint32_t pin, uint32_t mode) {
    GPIO_InitTypeDef GPIO_InitStruct = {};
    GPIO_InitStruct.Pin = DECODE_GPIO_PIN(pin); 
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(DECODE_GPIO_PORT(pin), &GPIO_InitStruct);
}

void digitalWrite(uint32_t pin, uint32_t value) {
    HAL_GPIO_WritePin(DECODE_GPIO_PORT(pin), DECODE_GPIO_PIN(pin), (GPIO_PinState)value);
}

uint32_t digitalRead(uint32_t pin) {
    return (uint32_t)HAL_GPIO_ReadPin(DECODE_GPIO_PORT(pin), DECODE_GPIO_PIN(pin));
}

void delay(unsigned long ms) {
    HAL_Delay(ms);
}

void delayMicroseconds(unsigned long us) {
    uint32_t start = micros();
    while (micros() - start < us);
}

unsigned long millis() {
    return HAL_GetTick();
}

unsigned long micros() {
    return (HAL_GetTick() * 1000) + (SysTick->LOAD - SysTick->VAL) / (SystemCoreClock / 1000000);
}