# Desktop Control Hub

Central coordination web application for the Desktop Control Panel system. This hub manages communication between various devices (Arduino, ESP32, Raspberry Pi) and provides a unified web interface for control and monitoring.

## Architecture

```
Browser (Web UI)
    ↕ Socket.IO
Hub (Node.js Server)
    ↕ Serial/GPIO/Network
Devices (Arduino, ESP32, etc.)
```

## Features

- **Real-time Communication**: Socket.IO for bidirectional updates
- **Device Coordination**: Manages multiple external devices
- **ARGB Control**: Controls addressable RGB LEDs via Arduino
- **GPIO Support**: Raspberry Pi GPIO integration (with mock mode for development)
- **Containerized**: Docker support for easy deployment

## Quick Start

### Development (Local Machine)

```bash
cd hub

# Start with Docker Compose
docker-compose up

# Or run directly with Node.js
npm install
node webserver.js
```

Access the web interface at: http://localhost:8080

### Production (Raspberry Pi)

```bash
cd hub

# Copy and configure environment
cp .env.example .env
# Edit .env with your settings

# Start production container
docker-compose -f docker-compose.prod.yml up -d
```

## Configuration

Environment variables (`.env`):

- `SERIAL_PORT`: Arduino serial port (default: `/dev/ttyACM0`)
- `GPIO_MODE`: `hardware` for Raspberry Pi, `mock` for development (default: `mock`)
- `NODE_ENV`: `development` or `production` (default: `development`)

## Device Integration

### Arduino (ARGB Control)

- **Protocol**: Serial @ 9600 baud
- **Format**: `_` + HSV bytes (Hue, Saturation, Value)
- **Device**: `/dev/ttyACM0`

### Raspberry Pi GPIO

- **LED**: GPIO 21 (output)
- **Button**: GPIO 26 (input)

## Development

### File Structure

```
hub/
├── public/              # Static web files
│   ├── index.html      # Web UI
│   └── index.js        # Client-side JavaScript
├── webserver.js        # Node.js server
├── package.json        # Dependencies
├── Dockerfile          # Container definition
└── docker-compose.yml  # Development orchestration
```

### Running without Docker

```bash
npm install
node webserver.js
```

### Hot Reload

The development Docker container uses nodemon for automatic restarts when code changes.

## API Events (Socket.IO)

### Client → Server

- `onOff` - Toggle power (0/1)
- `hsv_hue` - Set hue (0-255)
- `hsv_saturation` - Set saturation (0-255)
- `hsv_brightness` - Set brightness (0-255)

### Server → Client

- `onOff` - Power state update
- `connect` - Connection established
- `disconnect` - Connection lost

## Troubleshooting

### Arduino not detected

```bash
# List available serial ports
arduino-cli board list

# Update SERIAL_PORT in .env
```

### Permission denied on serial port

```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER
# Log out and back in
```

### GPIO not working

- Ensure `GPIO_MODE=hardware` on Raspberry Pi
- Check GPIO pins are not in use by other processes
- Verify user has GPIO permissions

## Related Components

- **argb_ctrl/ard_uno/**: Arduino firmware for ARGB control
- **argb_ctrl/raspi/**: Raspberry Pi integration scripts
- **argb_ctrl/esp_wifi/**: ESP32 WiFi controller

## Future Plans

- Multi-device dashboard
- E-ink display integration
- Cabinet lighting control
- Temperature monitoring
- System status display
