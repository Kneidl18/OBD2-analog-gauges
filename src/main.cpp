/**
 * @file main.cpp
 * @brief Main entry point for ESP32 automotive display
 * @author Andreas Auer
 *
 * DISPLAY TEST MODE:
 * To enable display test mode (disables Bluetooth, simulates sensor data):
 * 1. Uncomment #define DISPLAY_TEST in include/config.h
 * 2. Compile and upload the firmware
 * 3. The display will show simulated data cycling through ranges:
 *    - Temperature: -20°C to 135°C in 5°C steps every 1 second
 *    - Turbo Pressure: 0.9 to 2.5 bar in 0.1 bar steps every 1.5 seconds
 *    - Battery Voltage: 11.5V to 16V in 0.25V steps every 2 seconds
 * 4. All test parameters are configurable in config.h
 */

#include <Arduino.h>
#include "AutomotiveDisplay.h"
#include "config.h"

// Check if Bluetooth is available (only when not in display test mode)
#ifndef DISPLAY_TEST
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#endif

// Main application instance
AutomotiveDisplay automotiveDisplay;

void setup() {
#ifdef DISPLAY_TEST
    DEBUG_PRINTLN("ESP32 Automotive Display - DISPLAY TEST MODE");
    DEBUG_PRINTLN("Bluetooth functionality disabled for display testing");
#else
    DEBUG_PRINTLN("ESP32 OBD2 Automotive Display Starting...");
#endif
    DEBUG_PRINTLN("Firmware: " FIRMWARE_VERSION);

    // Initialize the main application
    if (!automotiveDisplay.begin()) {
        DEBUG_PRINTLN("Failed to initialize automotive display!");
        while (true) {
            delay(1000); // Halt on critical error
        }
    }

#ifdef DISPLAY_TEST
    DEBUG_PRINTLN("Display test mode setup complete!");
    DEBUG_PRINTLN("Testing temperature range: " + String(TEST_TEMP_MIN) + "°C to " + String(TEST_TEMP_MAX) + "°C");
    DEBUG_PRINTLN("Testing pressure range: " + String(TEST_PRESSURE_MIN) + " to " + String(TEST_PRESSURE_MAX) + " bar");
    DEBUG_PRINTLN("Testing voltage range: " + String(TEST_VOLTAGE_MIN) + "V to " + String(TEST_VOLTAGE_MAX) + "V");
#else
    DEBUG_PRINTLN("Setup complete!");
#endif
}

void loop() {
    // Main application update loop
    automotiveDisplay.update();

    // Small delay to prevent excessive CPU usage
    delay(DISPLAY_UPDATE_INTERVAL);
}

// All functionality now handled by AutomotiveDisplay class