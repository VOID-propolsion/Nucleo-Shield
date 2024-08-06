#ifndef UNITY_INCLUDE_CONFIG_H
#define UNITY_INCLUDE_CONFIG_H

#include <serial.hpp>

#define UNITY_OUTPUT_CHAR(a)                        \
    do                                              \
    {                                               \
        char temp = (a);                            \
        Serial_transmit((uint8_t *)&temp, 1, 1000); \
    } while (0)

#define UNITY_OUTPUT_COLOR

#endif // UNITY_INCLUDE_CONFIG_H