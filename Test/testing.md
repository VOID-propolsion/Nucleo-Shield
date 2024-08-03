# Testing

The release/debug version of the code is compiled separetly from the test version, albeit, to run the tests, you need to compile the code in the test configuration. This way, the MCU has all the tests uploaded, but none of the flight logic. You can see that in "main.c" there is a #if that determinds the setup call. Uploading the code, the MCU awaits a serial/uart message. This message is an ID of a test. When sending 1, the test with the 1 ID will run, returning some string, PASS or FAILED. How to send the ID message? scripts/test/tester.py is a custom pyscript that sends this number, and looks for a result message. This pyscript is also called by "CTest", which is integrated into CMake. 

## How the testing works:

1. Write some test with the hierchahy in mind
2. tests.cpp contains the switchcase that will determine which test will run. Add your test there
3. Add your test to the cmake list