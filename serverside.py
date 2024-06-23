import psutil
import serial
import time

# Specify the Arduino port directly
ARDUINO_PORT = '/dev/ttyACM0'

# Configure the serial port
try:
    ser = serial.Serial(ARDUINO_PORT, 9600, timeout=1)
    print(f"Successfully opened serial port: {ARDUINO_PORT}")
except Exception as e:
    print(f"Error opening serial port {ARDUINO_PORT}: {e}")
    exit()

def get_cpu_usage():
    return psutil.cpu_percent(interval=1)

try:
    while True:
        cpu_usage = get_cpu_usage()
        message = f"{cpu_usage}\n".encode()
        ser.write(message)
        print(f"Sent CPU usage: {cpu_usage}%")

        # Optional: Read and print any response from Arduino
        response = ser.readline().decode().strip()
        if response:
            print(f"Received from Arduino: {response}")

        time.sleep(1)  # Send update every second

except KeyboardInterrupt:
    print("Script terminated by user")

except Exception as e:
    print(f"An error occurred: {e}")

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Serial port closed")
