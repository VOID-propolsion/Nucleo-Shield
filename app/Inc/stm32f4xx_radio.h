#ifndef STM32_HAL_H
#define STM32_HAL_H

#include "RadioLib.h"
#include "stm32f4xx_hal.h"
#include "pins.h"
#include "main.h"

class STM32Hal : public RadioLibHal {
public:
    STM32Hal(uint32_t sck, uint32_t miso, uint32_t mosi)
    : RadioLibHal(GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_MODE_IT_RISING, GPIO_MODE_IT_FALLING),
    spiSCK(sck), spiMISO(miso), spiMOSI(mosi) {
    }

    void init() override {
        spiBegin();
    }

    void term() override {
        spiEnd();
    }

    void pinMode(uint32_t pin, uint32_t mode) override {
        GPIO_InitTypeDef GPIO_InitStruct = {};
        GPIO_InitStruct.Pin = DECODE_GPIO_PIN(pin); 
        GPIO_InitStruct.Mode = mode;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        HAL_GPIO_Init(DECODE_GPIO_PORT(pin), &GPIO_InitStruct);
    }

    void digitalWrite(uint32_t pin, uint32_t value) override {
        HAL_GPIO_WritePin(DECODE_GPIO_PORT(pin), DECODE_GPIO_PIN(pin), (GPIO_PinState)value);
    }

    uint32_t digitalRead(uint32_t pin) override {
        return (uint32_t)HAL_GPIO_ReadPin(DECODE_GPIO_PORT(pin), DECODE_GPIO_PIN(pin));
    }

    void delay(unsigned long ms) override {
        HAL_Delay(ms);
    }

    void delayMicroseconds(unsigned long us) override {
        uint32_t start = micros();
        while (micros() - start < us);
    }

    unsigned long millis() override {
        return HAL_GetTick();
    }

    unsigned long micros() override {
        return (HAL_GetTick() * 1000) + (SysTick->LOAD - SysTick->VAL) / (SystemCoreClock / 1000000);
    }

    // needs adjustments
    void spiBegin() {
        hspi1 = getSpi();
        digitalWrite(spiSCK, GPIO_PIN_RESET);
        digitalWrite(spiMISO, GPIO_PIN_RESET);
        digitalWrite(spiMOSI, GPIO_PIN_RESET);
        // note to self, the chip select pin should be high?
    }

    void spiEnd() {
        __HAL_RCC_SPI1_FORCE_RESET();
        __HAL_RCC_SPI1_RELEASE_RESET();
    }

    uint8_t spiTransferByte(uint8_t b) {
        uint8_t received_byte = 0;
        HAL_SPI_TransmitReceive(&hspi1, &b, &received_byte, 1, HAL_MAX_DELAY);
        return received_byte;
    }

    void attachInterrupt(uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) {
        // Example: Attaching an external interrupt
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = DECODE_GPIO_PIN(interruptNum);
        GPIO_InitStruct.Mode = mode;  // GPIO_MODE_IT_RISING or GPIO_MODE_IT_FALLING
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(DECODE_GPIO_PORT(interruptNum), &GPIO_InitStruct);
        HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
        // You must define the actual ISR somewhere in your code
    }

    void detachInterrupt(uint32_t interruptNum) override {
        // Implementation of detachInterrupt
    }

    long pulseIn(uint32_t pin, uint32_t state, RadioLibTime_t timeout) override {
        // Implementation of pulseIn
        return 0;  // Modify as necessary
    }

    void spiTransfer(uint8_t* out, size_t len, uint8_t* in) override {
        // Example message:
        HAL_SPI_TransmitReceive(&hspi1, out, in, len, HAL_MAX_DELAY);
    }

// Note, these two functions probably don't do what I need them to do...
    void spiBeginTransaction() override {
        // Begin SPI transaction if needed (configure settings)
    }
    void spiEndTransaction() override {
        // End SPI transaction if needed
        HAL_SPI_Abort(&hspi1);
    }

private:
    uint32_t spiSCK;
    uint32_t spiMISO;
    uint32_t spiMOSI;
    SPI_HandleTypeDef hspi1;
};

#endif
