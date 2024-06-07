#include "app.hpp"

RfLink rfLink = RfLink(&htim3, true);
Packet receivedPacket;
LPS22HB baro = LPS22HB();

int switches[4] = {SHIELD_SWITCH_1, SHIELD_SWITCH_2, SHIELD_SWITCH_3, SHIELD_SWITCH_4};

extern "C"
{
    void setup()
    {
        rfLink.init();
        baro.begin(&hspi2);
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
        // DEBUG("temperature: %f\n", baro.readTemperature());
        DEBUG("pressure: %f\n", baro.readPressure());
        HAL_Delay(1000);
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

    int handleUartInput(uint8_t* buffer) {
        DEBUG("\n");
        rfLink.sendPacket((char *) buffer);
        return 0;
    }
}
