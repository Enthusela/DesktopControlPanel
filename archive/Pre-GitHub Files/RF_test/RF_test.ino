// Powertech Plus Mains Outlet Controller Remote
// 24-bit address code, CRC check

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI library
#include <SPI.h>

// Create Amplitude Shift Keying object
RH_ASK rf_driver;

void setup() {
    // Initialise ASK object
    rf_driver.init();
    rf_driver.waitAvailable
    // Set up Serial monitor
    Serial.begin(9600);
    Serial.println(F("Starting " __FILE__ " at " __TIMESTAMP__));
}

void loop () {
    // Set buffer to size of expected message
    // 24-bit address code, CRC check
    // Set to 8-byte for now
    uint8_t buffer[8];
    uint8_t buffer_length = sizeof(buffer);
    // Check if received packet is correct size
    // if (rf_driver.recv(buffer, &buffer_length)) {
    //     // Message received with valid checksum
    //     Serial.print("Messaged received: ");
    //     Serial.println((char*)buffer);
    // }
    if (rf_driver.available()) {
        Serial.println(F("Message available"));
    }
}