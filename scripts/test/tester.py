import sys
import serial
import time

arg1 = sys.argv[1]
port = sys.argv[2]
serialString = ""
isResultPass = False
ser = None

# main
if __name__ == "__main__":

    print("Running test: ", arg1)

    try:
        ser = serial.Serial("COM3", 115200)
        # ser.open()
    except IOError:
        sys.exit("Error: Could not open serial port COM3")

    # Format the test ID to 3 digits with leading zeros
    formatted_arg = f"{int(arg1):03}"

    # Send the test ID to the serial port with leading zeros
    ser.write(formatted_arg.encode())

    # Timeout in seconds
    timeout = 10
    start_time = time.time()

    # Wait for a response within the timeout period
    while time.time() - start_time < timeout:
        if ser.in_waiting > 0:
            serialString += ser.read().decode()
            break

    if serialString == "":
        ser.close()
        sys.exit("Timed out with no reply...")

    # There is a connection, wait for the test to finish
    while 1:
        if ser.in_waiting > 0:
            char = ser.read(ser.in_waiting)
            serialString += char.decode()

            # check regex for ***PASS or ***FAIL
            if "\033[42m***OK***\033[0m" in serialString:
                print("Test Passed...")
                isResultPass = True
                break
            elif "\033[41m***FAIL***\033[0m" in serialString:
                print("Test Failed...")
                isResultPass = False
                break
    print(serialString)

    ser.close()

    if isResultPass:
        sys.exit(0)
    else:
        sys.exit(1)
