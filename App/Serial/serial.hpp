#pragma once

#include <usart.h>

#ifdef __cplusplus
extern "C"
{
#endif

    HAL_StatusTypeDef Serial_Transmit_Receive(uint8_t *tx_buf, uint16_t tx_size, uint8_t *rx_buf, uint16_t rx_size);
    HAL_StatusTypeDef Serial_transmit(uint8_t *tx_buf, uint16_t tx_size, uint16_t tx_timout);
    HAL_StatusTypeDef Serial_receive(uint8_t *rx_buf, uint16_t rx_size, uint16_t rx_timout);
    uint8_t AsciiToInt(const uint8_t *asciiStr, size_t length);

#ifdef __cplusplus
}
#endif