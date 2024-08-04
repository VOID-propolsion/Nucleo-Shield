#include <tests.hpp>
#include <cstddef>
#include <unity_config.h>
#include <unity.h>
#include <serial.hpp>

#include <LPS22HB_test.hpp>
#include <littleFS_test.hpp>

void runTest(uint8_t testNum);

void testsLoop()
{
    uint8_t tx_buffer[3];

    uint8_t status = Serial_receive(tx_buffer, 3, 5000); // get status
    if (status == 0)                                     // if 0 then we have the buffer filled
    {
        Serial_transmit(tx_buffer, 3, 1000); // echo back the status

        uint8_t testID = AsciiToInt(tx_buffer, 3); // convert the first character to an integer

        runTest(testID);
    }
}

void testsSetup()
{
    UNITY_BEGIN();
}

void runTest(uint8_t testNum)
{
    uint8_t numberOfFailures = 0;

    switch (testNum)
    {
    case 1:
        RUN_TEST(test_LPS22HB);
        break;
    case 2:
        RUN_TEST(test_ASD);
        break;
    default:
        numberOfFailures = 1;
        break;
    }

    UNITY_END();

    UNITY_BEGIN();
}

void setUp(void)
{
}
void tearDown(void)
{
}
