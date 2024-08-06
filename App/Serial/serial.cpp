#include <serial.hpp>
#include <usart.h>
#include <cstdlib> // for atoi
#include <cstring> // for memset

extern UART_HandleTypeDef huart2;

#define UART_TIMEOUT 1000 // 1 second

HAL_StatusTypeDef Serial_Transmit_Receive(uint8_t *tx_buf, uint16_t tx_size, uint8_t *rx_buf, uint16_t rx_size)
{
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, tx_buf, tx_size, UART_TIMEOUT);
    if (status != 0)
    {
        return HAL_UART_Receive(&huart2, rx_buf, rx_size, UART_TIMEOUT);
    }
    {
        return status;
    }
}

HAL_StatusTypeDef Serial_transmit(uint8_t *tx_buf, uint16_t tx_size, uint16_t tx_timout = UART_TIMEOUT)
{
    return HAL_UART_Transmit(&huart2, tx_buf, tx_size, tx_timout);
}

HAL_StatusTypeDef Serial_receive(uint8_t *rx_buf, uint16_t rx_size, uint16_t rx_timout = UART_TIMEOUT)
{
    return HAL_UART_Receive(&huart2, rx_buf, rx_size, rx_timout);
}

uint8_t AsciiToInt(const uint8_t *asciiStr, size_t length)
{
    // Create a temporary buffer
    char tempBuffer[3] = {0}; // 3 digits

    // Copy the ASCII characters to the temporary buffer
    if (length > 3)
        length = 3; // Ensure we don't copy more than 3 characters
    memcpy(tempBuffer, asciiStr, length);

    // Convert the ASCII string to an integer
    int number = atoi(tempBuffer);

    // Ensure the number fits into a uint8_t
    if (number < 0)
        number = 0;
    if (number > 255)
        number = 255;

    return static_cast<uint8_t>(number);
}