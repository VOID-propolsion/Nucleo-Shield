#pragma once

#include <stdint.h>
#include "spi.h"

class LPS22HB
{
public:
    static const uint8_t ADDRESS = 0x5C;
    SPI_HandleTypeDef *_hspi;

    typedef enum
    {
        P_1shot,
        P_1Hz,
        P_10Hz,
        P_25Hz,
        P_50Hz,
        P_75Hz

    } Rate_t;

    typedef enum
    {

        ERROR_NONE,
        ERROR_CONNECT,
        ERROR_ID,
        ERROR_SELFTEST

    } Error_t;

    LPS22HB();

    Error_t begin(SPI_HandleTypeDef *hspi);

    void clearInterrupt(void);

    bool checkNewData();

    float readPressure();

    float readTemperature();

private:
    // See LPS22H "MEMS pressure sensor: 260-1260 hPa absolute digital output barometer" Data Sheet
    // http://www.st.com/content/ccc/resource/technical/document/datasheet/bf/c1/4f/23/61/17/44/8a/DM00140895.pdf/files/DM00140895.pdf/jcr:content/translations/en.DM00140895.pdf
    static const uint8_t INTERRUPT_CFG = 0x0B;
    static const uint8_t THS_P_L = 0x0C;
    static const uint8_t THS_P_H = 0x0D;
    static const uint8_t WHOAMI = 0x0F;
    static const uint8_t CTRL_REG1 = 0x10;
    static const uint8_t CTRL_REG2 = 0x11;
    static const uint8_t CTRL_REG3 = 0x12;
    static const uint8_t FIFO_CTRL = 0x14;
    static const uint8_t REF_P_XL = 0x15;
    static const uint8_t REF_P_L = 0x16;
    static const uint8_t REF_P_H = 0x17;
    static const uint8_t RPDS_L = 0x18;
    static const uint8_t RPDS_H = 0x19;
    static const uint8_t RES_CONF = 0x1A;
    static const uint8_t INT_SOURCE = 0x25;
    static const uint8_t FIFO_STATUS = 0x26;
    static const uint8_t STATUS = 0x27;
    static const uint8_t PRESS_OUT_XL = 0x28;
    static const uint8_t PRESS_OUT_L = 0x29;
    static const uint8_t PRESS_OUT_H = 0x2A;
    static const uint8_t TEMP_OUT_L = 0x2B;
    static const uint8_t TEMP_OUT_H = 0x2C;
    static const uint8_t LPFP_RES = 0x33;

    Rate_t _odr;

    // Cross-platform support
    uint8_t _i2c;

    uint8_t readRegister(uint8_t subAddress);

    void writeRegister(uint8_t subAddress, uint8_t data);
    void readRegisters(uint8_t subAddress, uint8_t count, uint8_t *dest);
};
