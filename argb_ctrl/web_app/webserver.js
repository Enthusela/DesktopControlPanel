var http = require('http').createServer(handler); // create HTTP server with function handler()
var fs = require('fs');
var path = require('path');

// Require socket.io and pass the http object
var io = require('socket.io')(http);

// GPIO setup with graceful fallback for non-Raspberry Pi systems
const GPIO_MODE = process.env.GPIO_MODE || 'auto';
let Gpio, LED, pushButton;
let gpioAvailable = false;

try {
    if (GPIO_MODE !== 'mock') {
        Gpio = require('onoff').Gpio;
        LED = new Gpio(21, 'out');
        pushButton = new Gpio(26, 'in', 'both');
        gpioAvailable = true;
        console.log('GPIO initialized successfully');
    } else {
        console.log('GPIO disabled (mock mode)');
    }
} catch (err) {
    console.warn('GPIO not available:', err.message);
    console.log('Running in mock GPIO mode - physical button will not work');
    // Mock GPIO objects
    LED = { writeSync: () => {}, unexport: () => {} };
    pushButton = { watch: () => {}, unexport: () => {} };
}

// Add serialport library to communicate with Arduino
const { SerialPort } = require('serialport');
const SERIAL_PORT = process.env.SERIAL_PORT || '/dev/ttyACM0';
const port = new SerialPort({
    path: SERIAL_PORT,
    baudRate: 9600
});

// Listen on port 8080

http.listen(8080);

// Create server
function handler (req, res) {
    // Serve index.html for root request
    var filePath = req.url === '/' ? '/public/index.html' : '/public' + req.url;
    var fullPath = __dirname + filePath;

    // Determine content type
    var extname = path.extname(fullPath);
    var contentType = 'text/html';
    switch (extname) {
        case '.js':
            contentType = 'text/javascript';
            break;
        case '.css':
            contentType = 'text/css';
            break;
    }

    fs.readFile(fullPath, function (err, data) {
        if (err) {
            res.writeHead(404, {'Content-Type': 'text/html'});
            return res.end("404 Not Found");
        }
        res.writeHead(200, {'Content-Type': contentType});
        res.write(data);
        return res.end();
    });
}

// WebSocket connection
io.sockets.on('connection', function (socket) {
    var lightsOn = 1; // static variable for current status
    var hue = 0;
    var saturation = 255;
    var brightness = 255;
    // Get light switch status from server (i.e. button)
    if (gpioAvailable) {
        pushButton.watch(function (err, value) {
            if (err) {
                throw err;
            }
            lightsOn = value;
            socket.emit('onOff', lightsOn);
        });
    }
    // Get light switch status from client (i.e. checkbox)
    socket.on('onOff', function(data) {
        lightsOn = data;
        writeHsvToPort(lightsOn, hue, saturation, brightness);
    });
    // Get hue value from client
    socket.on('hsv_hue', function(data) {
        hue = data;
        writeHsvToPort(lightsOn, hue, saturation, brightness);
    });
    // Get saturation value from client
    socket.on('hsv_saturation', function(data) {
        saturation = data;
        writeHsvToPort(lightsOn, hue, saturation, brightness);
    });
    // Get value value from client
    socket.on('hsv_brightness', function(data) {
        brightness = data;
        writeHsvToPort(lightsOn, hue, saturation, brightness);
    });
    // Function to log any errors writing to serial port
    port.on('error', function(err) {
        console.log('Error: ', err.message);
    });
});

process.on('SIGINT', function () {
    LED.writeSync(0);
    LED.unexport();
    pushButton.unexport();
    process.exit();
});

// function to write three bytes to the serial port
function writeHsvToPort(lightsOn, h, s, b) {
    // Convert h, s, and b to chars
    // Write bytes to serial port
    var hsb = Uint8Array.from([0, 0, 0]);
    if (lightsOn) {
        hsb = Uint8Array.from([h, s, b]);
    }
    console.log('Writing to port: _,' + hsb);
    port.write('_');
    port.write(hsb);
}

