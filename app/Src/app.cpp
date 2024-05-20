#include "app.hpp"
// #include "stm32f4xx_radio.h"

// STM32Hal* hal = new STM32Hal(
//     PB_3, // SCK
//     PB_4, // MISO
//     PB_5 // MOSI
// );
// SX1280 radio = new Module(
//     hal, 
//     PA_4, // Chip select
//     PB_12, // interupt 
//     PA_10 // reset
// );

RfLink rfLink = RfLink(&htim3, true);
Packet receivedPacket;

int switches[4] = {SHIELD_SWITCH_1, SHIELD_SWITCH_2, SHIELD_SWITCH_3, SHIELD_SWITCH_4};

extern "C"
{
    void setup()
    {
        rfLink.init();
        rfLink.onTransmit = [](Packet &packet) {
            // DEBUG("Sent data: ");
            // for (int i = 0; i < sizeof(packet.payload); i++) {
            //     DEBUG("%02X ", packet.payload[i]);
            // }
            // DEBUG("\n");
            // DEBUG("message sent: %s\n", packet.payload);
        };

        rfLink.onReceive = [](Packet &packet) {
            DEBUG("message received: %s\n", packet.payload);
        };

        // setting up some settings
        HAL_GPIO_WritePin(GPIOA, SHIELD_LED_1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, SHIELD_LED_2, GPIO_PIN_RESET);
    }

    void loop()
    {
        if (!HAL_GPIO_ReadPin(GPIOC, BOARD_BUTTON)) {
            rfLink.sendPacket("hello world!");
            while (!HAL_GPIO_ReadPin(GPIOC, BOARD_BUTTON)) {
                rfLink.enterRx();
                if (rfLink.receivePacket(&receivedPacket)) {
                    // Process received data
                    // DEBUG("Rough data: ");
                    // for (int i = 0; i < sizeof(receivedPacket.payload); i++) {
                    //     DEBUG("%02X ", receivedPacket.payload[i]);
                    // }
                    // DEBUG("\n");
                    // DEBUG("packet number: %d\n\n", receivedPacket.status.packetNumber);
                } else {
                    DEBUG("No data received or error occurred\n");
                }
                HAL_Delay(1000);
            }
        } 
        HAL_Delay(500);
    }

    int handleInput(uint8_t* buffer) {
        DEBUG("\n");
        rfLink.sendPacket((char *) buffer);
        return 0;
    }
}
