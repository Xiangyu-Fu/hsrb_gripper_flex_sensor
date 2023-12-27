import serial
import time
from collections import defaultdict

# Configuration
# port = "/dev/ttyUSB0"       # Linux
port = "COM3"               # Windows
baud_rate = 115200          # Set the baud rate

# Initialize serial connection
ser = serial.Serial(port, baud_rate, timeout=1)

# Dictionary to store spike counts
spike_counts = defaultdict(int)

try:
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').rstrip()
            print("Received data:", data)

            # Parse the data to find neuron number
            if 'Neuron' in data:
                try:
                    neuron_number = int(data.split()[1])
                    spike_counts[neuron_number] += 1
                except ValueError:
                    print("Error: Unable to parse neuron number")

        # Optional: Display spike counts every few seconds
        # time.sleep(1)
        print("Spike counts:", dict(spike_counts))
except KeyboardInterrupt:
    print("Exiting program")

# Close the serial connection
ser.close()
