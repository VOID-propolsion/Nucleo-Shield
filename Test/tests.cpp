// #include <cstddef>
// #include <unity.h>

// static void TestTask(void *pv)
// {
//     int numberOfFailures;

//     UNITY_BEGIN();

//     uint32_t test_arg;
//     test_arg = serialgetargument(); /* get test arguments */
//     switch (test_arg)
//     {
//     case 1:
//         RUN_TEST();
//         break;
//     case 2:
//         RUN_TEST();
//         break;
//     default:
//         RUN_TEST(TestArgFailed);
//         break;
//     }
//     numberOfFailures = UNITY_END();

//     /* report failed or pass */
//     if (nofFailures == 0)
//     {
//         // McuShell_SendStr((unsigned char *)"*** PASSED ***\n", McuRTT_stdio.stdOut);
//     }
//     else
//     {
//         // McuShell_SendStr((unsigned char *)"*** FAILED ***\n", McuRTT_stdio.stdOut);
//     }

//     // McuShell_SendStr((unsigned char *)"*STOP*\n", McuRTT_stdio.stdOut); /* stop JRun */
//     // vTaskDelete(NULL);                                                  /* terminate task */
// }

// void Tests_Init(void)
// {
//     TestTask(NULL);
// }

// // void Tests_Init(void) {
// //   if (xTaskCreate(
// //       TestTask,  /* pointer to the task */
// //       "Test", /* task name for kernel awareness debugging */
// //       1500/sizeof(StackType_t), /* task stack size */
// //       (void*)NULL, /* optional task startup argument */
// //       tskIDLE_PRIORITY,  /* initial priority */
// //       (TaskHandle_t*)NULL /* optional task handle to create */
// //     ) != pdPASS)
// //   {
// //     McuLog_fatal("Failed creating task");
// //     for(;;){} /* error! probably out of memory */
// //   }
// // }