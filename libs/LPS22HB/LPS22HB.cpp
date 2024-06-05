#include "LPS22HB.h"

// Constructor
LPS22HB::LPS22HB()
{
    _odr = P_10Hz;
}

bool LPS22HB::checkNewData()
{
    return (bool)readRegister(STATUS);   
}

float LPS22HB::readPressure()
{
    uint8_t rawData[3];  // 24-bit pressure register data stored here
    readRegisters((PRESS_OUT_XL | 0x80), 3, &rawData[0]); // bit 7 must be one to read multiple bytes
    return ((int32_t) ((int32_t) rawData[2] << 16 | (int32_t) rawData[1] << 8 | rawData[0])) / 4096.f * 100;
}

float LPS22HB::readTemperature()
{
    uint8_t rawData[2];  // 16-bit pressure register data stored here
    readRegisters((TEMP_OUT_L | 0x80), 2, &rawData[0]); // bit 7 must be one to read multiple bytes
    return ((int16_t)((int16_t) rawData[1] << 8 | rawData[0])) / 100.f;
}

LPS22HB::Error_t LPS22HB::begin(SPI_HandleTypeDef *hspi)
{
    // Set up SPI handle
    _hspi = hspi;

    if (readRegister(WHOAMI) != 0xB1) {
        return ERROR_ID;
    }

    // set sample rate by setting bits 6:4 
    // enable low-pass filter by setting bit 3 to one
    // bit 2 == 0 means bandwidth is odr/9, bit 2 == 1 means bandwidth is odr/20
    // make sure data not updated during read by setting block data update (bit 1) to 1
    writeRegister(CTRL_REG1, _odr << 4 | 0x08 | 0x02);  
    writeRegister(CTRL_REG3, 0x04);  // enable data ready as interrupt source

    return ERROR_NONE;
}

void LPS22HB::clearInterrupt(void)
{
    readPressure();
    readTemperature();
}

uint8_t LPS22HB::readRegister(uint8_t subAddress)
{
    uint8_t data;
    readRegisters(subAddress, 1, &data);
    return data;
}

void LPS22HB::writeRegister(uint8_t subAddress, uint8_t data)
{
    uint8_t buffer[2] = {subAddress & 0x7F, data}; 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_SPI_Transmit(_hspi, buffer, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

void LPS22HB::readRegisters(uint8_t subAddress, uint8_t count, uint8_t *dest)
{
    subAddress |= 0x80; // MSB must be 1 for read operation
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_SPI_Transmit(_hspi, &subAddress, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(_hspi, dest, count, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}
