#!/bin/bash

echo "ARGB Controller - Setup Checker"
echo "================================"
echo ""

# Check Docker
echo -n "Checking Docker... "
if command -v docker &> /dev/null; then
    echo "✓ Found ($(docker --version | cut -d' ' -f3 | tr -d ','))"
else
    echo "✗ Not found"
    echo "  Please install Docker: https://docs.docker.com/get-docker/"
fi

# Check Docker Compose
echo -n "Checking Docker Compose... "
if command -v docker-compose &> /dev/null || docker compose version &> /dev/null; then
    if command -v docker-compose &> /dev/null; then
        echo "✓ Found ($(docker-compose --version | cut -d' ' -f4 | tr -d ','))"
    else
        echo "✓ Found ($(docker compose version --short))"
    fi
else
    echo "✗ Not found"
    echo "  Please install Docker Compose: https://docs.docker.com/compose/install/"
fi

# Check for Arduino devices
echo -n "Checking for Arduino devices... "
DEVICES=$(ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null)
if [ -n "$DEVICES" ]; then
    echo "✓ Found:"
    for device in $DEVICES; do
        echo "  - $device"
    done
else
    echo "✗ No devices found"
    echo "  Connect your Arduino via USB and try again"
fi

# Check USB permissions
if [ -n "$DEVICES" ]; then
    echo -n "Checking USB permissions... "
    FIRST_DEVICE=$(echo $DEVICES | awk '{print $1}')
    if [ -r "$FIRST_DEVICE" ] && [ -w "$FIRST_DEVICE" ]; then
        echo "✓ Can read/write to $FIRST_DEVICE"
    else
        echo "✗ Cannot access $FIRST_DEVICE"
        echo "  Run: sudo usermod -aG dialout $USER"
        echo "  Then log out and back in"
    fi
fi

echo ""
echo "Setup check complete!"
