#ifndef STM32_HAL_H
#define STM32_HAL_H

#include "RadioLib.h"
#include "stm32f4xx_hal.h"  // Make sure this include is correct for your setup

class STM32Hal : public RadioLibHal {
public:
    STM32Hal(int8_t sck, int8_t miso, int8_t mosi)
    : RadioLibHal(GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_MODE_IT_RISING, GPIO_MODE_IT_FALLING),
      spiSCK(sck), spiMISO(miso), spiMOSI(mosi)  {
    }

    void init() override {
        spiBegin();
    }

    void term() override {
        spiEnd();
    }

    void pinMode(uint32_t pin, uint32_t mode) override {
        GPIO_InitTypeDef GPIO_InitStruct = {};
        GPIO_InitStruct.Pin = (1 << pin);  // Assuming pin is just the pin number
        GPIO_InitStruct.Mode = mode;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  // Adjust speed as necessary
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  // Change GPIOB to the correct GPIO port
    }

    void digitalWrite(uint32_t pin, uint32_t value) override {
        HAL_GPIO_WritePin(GPIOB, (1 << pin), (GPIO_PinState)value);
    }

    uint32_t digitalRead(uint32_t pin) override {
        return (uint32_t)HAL_GPIO_ReadPin(GPIOB, (1 << pin));
    }

    void delay(unsigned long ms) override {
        HAL_Delay(ms);
    }

    void delayMicroseconds(unsigned long us) override {
        // Accurate implementation may vary, this is a basic busy-wait
        uint32_t start = micros();
        while (micros() - start < us);
    }

    unsigned long millis() override {
        return HAL_GetTick();
    }

    unsigned long micros() override {
        return (HAL_GetTick() * 1000) + (SysTick->LOAD - SysTick->VAL) / (SystemCoreClock / 1000000);
    }

    void spiBegin() {
        __HAL_RCC_SPI1_CLK_ENABLE();  // Enable the clock for SPI1

        SPI_HandleTypeDef SpiHandle = {};
        SpiHandle.Instance               = SPI1;
        SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;  // Adjust as needed
        SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
        SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
        SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
        SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
        SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
        SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
        SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
        SpiHandle.Init.CRCPolynomial     = 7;
        SpiHandle.Init.NSS               = SPI_NSS_SOFT;
        SpiHandle.Init.Mode              = SPI_MODE_MASTER;

        HAL_SPI_Init(&SpiHandle);

        // Configure the GPIO pins for SPI
        pinMode(spiSCK, GPIO_MODE_AF_PP);
        pinMode(spiMISO, GPIO_MODE_AF_PP);
        pinMode(spiMOSI, GPIO_MODE_AF_PP);
        HAL_GPIO_WritePin(GPIOA, spiSCK | spiMISO | spiMOSI, GPIO_PIN_RESET); //I think it's GPIOA?
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
        GPIO_InitStruct.Pin = interruptNum;
        GPIO_InitStruct.Mode = mode;  // GPIO_MODE_IT_RISING or GPIO_MODE_IT_FALLING
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
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
        // example message:
        // uint8_t message[] = "Hello world\n";
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
    int8_t spiSCK;
    int8_t spiMISO;
    int8_t spiMOSI;
    SPI_HandleTypeDef hspi1;  // Handle for SPI, adjust for your specific SPI instance
};

#endif
