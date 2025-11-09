# ARGB Controller - Containerized Setup

A containerized RGB LED controller that runs on both development machines and Raspberry Pi, communicating with Arduino via USB serial.

## Architecture

- **Web Interface**: Node.js server with Socket.IO for real-time control
- **Arduino Communication**: Serial connection via USB (default: `/dev/ttyACM0`)
- **GPIO Control**: Physical button support on Raspberry Pi (gracefully disabled on other systems)
- **Container**: Docker-based deployment for portability

## Prerequisites

### Development Machine
- Docker and Docker Compose installed
- Arduino connected via USB
- Linux host (for USB device passthrough)

### Raspberry Pi
- Docker and Docker Compose installed
- Arduino connected via USB
- GPIO pins 21 (LED) and 26 (button) wired (optional)

## Quick Start

### Development Mode (Your Local Machine)

1. **Find your Arduino device path:**
   ```bash
   ls -l /dev/ttyACM* /dev/ttyUSB*
   ```

2. **Update device path if needed:**
   Edit `docker-compose.yml` and update the device path:
   ```yaml
   devices:
     - /dev/ttyACM0:/dev/ttyACM0  # Change if your Arduino is on a different port
   ```

3. **Start the container:**
   ```bash
   cd argb_ctrl
   docker compose up --build
   ```

4. **Access the web interface:**
   Open your browser to `http://localhost:8080`

### Production Mode (Raspberry Pi)

1. **Build and start:**
   ```bash
   cd argb_ctrl
   docker compose -f docker-compose.prod.yml up --build -d
   ```

2. **Access the web interface:**
   From any device on the network: `http://<raspberry-pi-ip>:8080`

## Configuration

### Environment Variables

You can customize behavior in two ways:

**Option 1: Using a .env file (recommended)**
```bash
# Copy the example file and edit it
cp .env.example .env
# Edit .env with your preferred settings
```

**Option 2: Direct environment variables**
```bash
# Set before running docker compose
export SERIAL_PORT=/dev/ttyUSB0
export GPIO_MODE=mock
docker compose up
```

Available variables:
- `GPIO_MODE`: 'mock' (disable GPIO) or 'hardware' (enable GPIO)
- `SERIAL_PORT`: Arduino serial port path (default: /dev/ttyACM0)
- `NODE_ENV`: 'development' or 'production'

### USB Device Permissions

If you encounter permission errors accessing the Arduino:

```bash
# Add your user to the dialout group
sudo usermod -aG dialout $USER

# Or set device permissions (temporary, resets on reboot)
sudo chmod 666 /dev/ttyACM0
```

## Development Features

### Hot Reload
In development mode, the container uses `nodemon` to automatically restart when you edit:
- `webserver.js`
- `public/index.html`
- `public/index.js`

Changes are reflected immediately without rebuilding the container.

### Logs
```bash
# View logs
docker compose logs -f

# View only errors
docker compose logs -f | grep -i error
```

## Troubleshooting

### Arduino Not Found
```
Error: Error: Error Resource temporarily unavailable Cannot lock port
```
**Solution**:
- Check the device path: `ls -l /dev/ttyACM*`
- Update `docker-compose.yml` with correct device path
- Ensure the device isn't already open by another program
- Check permissions (see USB Device Permissions above)

### GPIO Errors on Non-Raspberry Pi
This is normal. The application will log:
```
GPIO not available: ...
Running in mock GPIO mode - physical button will not work
```
This is expected behavior on development machines. The web interface will still work.

### Port 8080 Already in Use
```bash
# Find and kill the process using port 8080
sudo lsof -i :8080
sudo kill -9 <PID>
```

### Container Won't Start
```bash
# Clean rebuild
docker compose down
docker compose up --build
```

## Project Structure

```
argb_ctrl/
├── docker-compose.yml          # Development configuration
├── docker-compose.prod.yml     # Production configuration (Raspberry Pi)
├── web_app/
│   ├── Dockerfile              # Container definition
│   ├── .dockerignore          # Files to exclude from container
│   ├── package.json           # Node.js dependencies
│   ├── webserver.js           # Main server application
│   └── public/
│       ├── index.html         # Web interface
│       └── index.js           # Client-side JavaScript
├── raspi/
│   └── ard.py                 # Python control script (alternative)
└── ard_uno/
    └── ARGBController.ino     # Arduino sketch
```

## Arduino Setup

1. Flash the Arduino with `ard_uno/ARGBController.ino`
2. Connect the Arduino via USB
3. The Arduino should appear as `/dev/ttyACM0` (or similar)
4. The web server will communicate using the protocol:
   - Send: `_` + 3 bytes (H, S, V values 0-255)
   - When off: `_` + 0, 0, 0

## Stopping the Application

### Development
```bash
docker compose down
```

### Production
```bash
docker compose -f docker-compose.prod.yml down
```

## Building for Different Platforms

### ARM64 (Raspberry Pi 4)
```bash
docker buildx build --platform linux/arm64 -t argb-web-app ./web_app
```

### AMD64 (Standard PC)
```bash
docker buildx build --platform linux/amd64 -t argb-web-app ./web_app
```

## Next Steps

1. Test the setup on your development machine
2. Customize the Arduino sketch for your RGB LED setup
3. Deploy to Raspberry Pi when ready
4. Configure autostart on Raspberry Pi boot (optional):
   ```bash
   # Add to /etc/rc.local (before 'exit 0')
   cd /path/to/argb_ctrl && docker compose -f docker-compose.prod.yml up -d
   ```
