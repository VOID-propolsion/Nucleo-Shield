#include "app.hpp"

RfLink rfLink = RfLink(&htim3, true);
Packet receivedPacket;
LPS22HB baro = LPS22HB();

int switches[4] = {SHIELD_SWITCH_1, SHIELD_SWITCH_2, SHIELD_SWITCH_3, SHIELD_SWITCH_4};

extern "C"
{
    void setup()
    {   
        Servo_Init();
        Set_Servo_Angle(90);

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
        for (uint16_t i = 0; i <= 180; i += 18) {
            Set_Servo_Angle(i);
            
            HAL_Delay(500); // Wait for 0.5 seconds
        }
        HAL_Delay(1500);

        //DEBUG("reading: %f\n", baro.readPressure());
        
   
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
