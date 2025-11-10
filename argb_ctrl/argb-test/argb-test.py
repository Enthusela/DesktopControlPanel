#!/usr/bin/env python3
"""
Test script to verify Arduino serial communication.
Sends HSV color values to the Arduino and monitors the connection.

Usage:
    python3 argb-test.py /dev/ttyACM0
"""

import serial
import time
import sys

def send_hsv_color(ser, hue, saturation, value):
    """
    Send HSV color to Arduino.

    Args:
        ser: Serial connection object
        hue: 0-255 (color)
        saturation: 0-255 (color intensity)
        value: 0-170 (brightness, max limited by Arduino)
    """
    # Protocol: start byte '_' followed by H, S, V
    data = bytes([ord('_'), hue, saturation, value])
    ser.write(data)
    print(f"Sent: H={hue}, S={saturation}, V={value}")

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 test_serial.py <port>")
        print("Example: python3 test_serial.py /dev/ttyACM0")
        sys.exit(1)

    port = sys.argv[1]
    baud_rate = 9600

    try:
        # Open serial connection
        print(f"Connecting to {port} at {baud_rate} baud...")
        ser = serial.Serial(port, baud_rate, timeout=1)
        time.sleep(2)  # Wait for Arduino to reset after serial connection
        print("Connected!\n")

        # Test sequence
        print("Starting test sequence...")
        print("=" * 50)

        # Test 1: Red (H=0, full saturation, medium brightness)
        print("\nTest 1: Red")
        send_hsv_color(ser, 0, 255, 100)
        time.sleep(2)

        # Test 2: Green (H=96 in FastLED)
        print("\nTest 2: Green")
        send_hsv_color(ser, 96, 255, 100)
        time.sleep(2)

        # Test 3: Blue (H=160 in FastLED)
        print("\nTest 3: Blue")
        send_hsv_color(ser, 160, 255, 100)
        time.sleep(2)

        # Test 4: White (zero saturation)
        print("\nTest 4: White")
        send_hsv_color(ser, 0, 0, 100)
        time.sleep(2)

        # Test 5: Rainbow cycle
        print("\nTest 5: Rainbow cycle (10 colors)")
        for i in range(10):
            hue = int((i / 10) * 255)
            send_hsv_color(ser, hue, 255, 100)
            time.sleep(0.5)

        print("\n" + "=" * 50)
        print("Test sequence complete!")
        print("\nYou can now send custom colors:")
        print("Enter HSV values (e.g., '128 255 100') or 'q' to quit")

        # Interactive mode
        while True:
            try:
                user_input = input("\nHSV> ").strip()
                if user_input.lower() in ['q', 'quit', 'exit']:
                    break

                parts = user_input.split()
                if len(parts) != 3:
                    print("Please enter 3 values: Hue Saturation Value")
                    continue

                h, s, v = map(int, parts)

                # Validate ranges
                if not (0 <= h <= 255):
                    print("Hue must be 0-255")
                    continue
                if not (0 <= s <= 255):
                    print("Saturation must be 0-255")
                    continue
                if not (0 <= v <= 170):
                    print("Value must be 0-170")
                    continue

                send_hsv_color(ser, h, s, v)

            except ValueError:
                print("Invalid input. Please enter three numbers.")
            except KeyboardInterrupt:
                print("\nExiting...")
                break

        ser.close()
        print("Serial connection closed.")

    except serial.SerialException as e:
        print(f"Error: Could not open serial port {port}")
        print(f"Details: {e}")
        print("\nTroubleshooting:")
        print("1. Check if the Arduino is connected: arduino-cli board list")
        print("2. Ensure you have permissions: sudo usermod -a -G dialout $USER")
        print("   (then log out and back in)")
        sys.exit(1)

if __name__ == "__main__":
    main()
