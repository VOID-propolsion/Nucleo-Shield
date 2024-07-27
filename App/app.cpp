#include "app.hpp"
#include "LPS22HB.hpp"
#include "W25Q.hpp"
#include "lfs_adapter.hpp"
#include "lfs.h"

// #include "rflink.h"

// RfLink rfLink = RfLink(&htim3, true);
// Packet receivedPacket;
// LPS22HB baro = LPS22HB();

// int switches[4] = {SHIELD_SWITCH_1, SHIELD_SWITCH_2, SHIELD_SWITCH_3, SHIELD_SWITCH_4};

extern "C"
{
    void setup()
    {
        W25Q_Init();
        initLittleFs();

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    }

    void loop()
    {

        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
        // DEBUG("temperature: %f\n", baro.readTemperature());
        // DEBUG("pressure: %f\n", baro.readPressure());
        // if (!HAL_GPIO_ReadPin(GPIOC, BOARD_BUTTON)) {
        //     rfLink.sendPacket("hello world!");
        //     while (!HAL_GPIO_ReadPin(GPIOC, BOARD_BUTTON)) {
        //     }
        // } else {
        //     rfLink.enterRx();
        //     if (rfLink.receivePacket(&receivedPacket)) {
        //         DEBUG("received: %s\n", (char *) receivedPacket.payload);
        //     }
        //     HAL_Delay(1000);
        // }
        // HAL_Delay(500);
    }
}

// rfLink.init();

// baro.begin(&hspi2);
// rfLink.onTransmit = [](Packet &packet)
// {
//     // DEBUG("Sent data: ");
//     // for (int i = 0; i < sizeof(packet.payload); i++) {
//     //     DEBUG("%02X ", packet.payload[i]);
//     // }
//     // DEBUG("\n");
//     // DEBUG("message sent: %s\n", packet.payload);
// };

// rfLink.onReceive = [](Packet &packet)
// {
//     // DEBUG("message received: %s\n", packet.payload);
// };