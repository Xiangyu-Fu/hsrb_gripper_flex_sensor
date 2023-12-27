import serial
import time

# Configuration
port = "/dev/ttyUSB0"  # Replace with your UART port name
baud_rate = 9600       # Set the baud rate

# Initialize serial connection
ser = serial.Serial(port, baud_rate, timeout=1)

# Read data from UART
try:
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').rstrip()
            print("Received data:", data)
        time.sleep(0.1)
except KeyboardInterrupt:
    print("Exiting program")

# Close the serial connection
ser.close()
