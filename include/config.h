/**
 * @file config.h
 * @brief System configuration and settings for ESP32 Automotive Display
 * @author Andreas Auer
 * @version 1.0.0
 * @date 2025-01-28
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// PROJECT METADATA
// ============================================================================
#define PROJECT_NAME            "ESP32 OBD2 Automotive Display"
#define PROJECT_VERSION         "1.0.0"
#define PROJECT_AUTHOR          "Andreas Auer"
#define PROJECT_EMAIL           "andreas.auer03@gmail.com"
#define PROJECT_DESCRIPTION     "Real-time automotive sensor display with analog gauges"
#define BUILD_DATE              __DATE__
#define BUILD_TIME              __TIME__
#define FIRMWARE_VERSION        "v1.0.0-" __DATE__

// ============================================================================
// HARDWARE CONFIGURATION
// ============================================================================

// --- ESP32 Board Configuration ---
#define ESP32_BOARD_TYPE        "ESP32 DevKit v1"
#define CPU_FREQUENCY_MHZ       240
#define FLASH_SIZE_MB           4
#define PSRAM_ENABLED           false

// --- TFT Display Configuration ---
// Note: TFT_DRIVER (ILI9341) is defined in platformio.ini build_flags
#define TFT_WIDTH               240
#define TFT_HEIGHT              320
#define TFT_ROTATION            0           // 0=Portrait, 1=Landscape, 2=Portrait flipped, 3=Landscape flipped

// TFT SPI Pin Configuration
#define TFT_MISO_PIN            19
#define TFT_MOSI_PIN            23
#define TFT_SCLK_PIN            18
#define TFT_CS_PIN              15
#define TFT_DC_PIN              2
#define TFT_RST_PIN             4
#define TFT_BL_PIN              (-1)          // Backlight pin (-1 if not used)

// TFT SPI Settings
#define TFT_SPI_FREQUENCY       27000000    // 27 MHz
#define TFT_SPI_READ_FREQUENCY  20000000    // 20 MHz for reading

// --- Bluetooth Configuration ---
#define BT_DEVICE_NAME          "ESP32_OBD2_Display"
#define BT_PIN_CODE             "1234"      // Optional PIN for pairing
#define BT_ENABLE_SSP           true        // Secure Simple Pairing
#define BT_DISCOVERABLE         true        // Make device discoverable

// --- OBD2 Configuration ---
#define OBD2_BAUD_RATE          38400       // Standard OBD2 baud rate
#define OBD2_TIMEOUT_MS         3000        // Response timeout
#define OBD2_RETRY_COUNT        3           // Number of retries for failed commands
#define OBD2_INIT_DELAY_MS      500         // Delay between initialization commands

// --- Diagnostic Button Configuration ---
#define DIAGNOSTIC_BUTTON_PIN   21          // GPIO pin for diagnostic mode button (connect to GND, uses internal pull-up)
#define BUTTON_DEBOUNCE_MS      50          // Button debounce time in milliseconds (prevents false triggers)
#define BUTTON_LONG_PRESS_MS    1000        // Long press duration for mode exit (1 second hold to exit diagnostic mode)

// ============================================================================
// SENSOR CONFIGURATION
// ============================================================================

// --- Temperature Sensor (PID 0105) ---
#define TEMP_PID                "0105"
#define TEMP_GAUGE_MIN_VALUE    40.0f       // Gauge minimum for needle positioning (°C)
#define TEMP_MAX_VALUE          130.0f      // Maximum temperature (°C)
#define TEMP_BLUE_THRESHOLD     80.0f       // Blue zone upper limit (°C) - cold engine
#define TEMP_GREEN_THRESHOLD    100.0f      // Green zone upper limit (°C) - normal operating
#define TEMP_YELLOW_THRESHOLD   115.0f      // Yellow zone upper limit (°C) - warning hot
#define TEMP_CRITICAL_VALUE     115.0f      // Critical temperature warning (°C) - overheating
#define TEMP_UNIT               "°C"
#define TEMP_TITLE              "Coolant"
#define TEMP_NEEDLE_MIN_TEMP    40.0f       // Temperature below which needle stays at 0%

// --- Boost Pressure Sensor (PID 010B) ---
#define BOOST_PID               "010B"
#define BOOST_MIN_VALUE         0.9f        // Minimum pressure (bar)
#define BOOST_MAX_VALUE         2.5f        // Maximum pressure (bar)
#define BOOST_GREEN_THRESHOLD   1.8f        // Green zone upper limit (bar)
#define BOOST_YELLOW_THRESHOLD  2.2f        // Yellow zone upper limit (bar)
#define BOOST_CRITICAL_VALUE    2.4f        // Critical pressure warning (bar)
#define BOOST_UNIT              "bar"
#define BOOST_TITLE             "Boost"

// ============================================================================
// BATTERY VOLTAGE MONITORING CONFIGURATION
// ============================================================================

// --- Battery Voltage Settings ---
#define BATTERY_VOLTAGE_ENABLED     true        // Enable battery voltage monitoring
#define BATTERY_UPDATE_INTERVAL     3000        // Update interval in milliseconds
#define VOLTAGE_LOW_THRESHOLD       12.5f       // Critical low voltage (red zone)
#define VOLTAGE_MID_THRESHOLD       13.0f       // Warning/normal boundary (yellow/green)
#define VOLTAGE_HIGH_THRESHOLD      14.5f       // Upper normal range limit
#define VOLTAGE_DISPLAY_DECIMALS    1           // Decimal places to display
#define VOLTAGE_DISPLAY_X           200         // X position for voltage display
#define VOLTAGE_DISPLAY_Y           10          // Y position for voltage display
#define VOLTAGE_FONT_SIZE           2           // Font size (1-4)

// --- Data Validation Ranges ---
#define TEMP_VALID_MIN          (-40.0f)      // Absolute minimum valid temperature
#define TEMP_VALID_MAX          150.0f      // Absolute maximum valid temperature
#define BOOST_VALID_MIN         0.0f        // Absolute minimum valid pressure (bar)
#define BOOST_VALID_MAX         3.0f        // Absolute maximum valid pressure (bar)

// ============================================================================
// DISPLAY CONFIGURATION
// ============================================================================

// --- Screen Layout ---
#define SCREEN_WIDTH            240
#define SCREEN_HEIGHT           320
#define STATUS_BAR_HEIGHT       30
#define GAUGE_AREA_HEIGHT       (SCREEN_HEIGHT - STATUS_BAR_HEIGHT)

// --- Gauge Configuration ---
#define GAUGE_RADIUS            80
#define GAUGE_NEEDLE_LENGTH     65          // Length of gauge needle
#define GAUGE_TICK_COUNT        8           // Number of major tick marks
#define GAUGE_MINOR_TICKS       true        // Enable minor tick marks
#define GAUGE_CENTER_DOT_SIZE   3           // Radius of center dot

// Gauge Positions
#define TEMP_GAUGE_X            (SCREEN_WIDTH / 2)
#define TEMP_GAUGE_Y            100
#define BOOST_GAUGE_X           (SCREEN_WIDTH / 2)
#define BOOST_GAUGE_Y           220

// --- Color Scheme ---
#define COLOR_BACKGROUND        0x0000      // Black
#define COLOR_TEXT              0xFFFF      // White
#define COLOR_GAUGE_OUTLINE     0xFFFF      // White
#define COLOR_NEEDLE            0xF800      // Red
#define COLOR_BLUE_ZONE         0x001F      // Blue (cold engine)
#define COLOR_GREEN_ZONE        0x07E0      // Green (normal operating)
#define COLOR_YELLOW_ZONE       0xFFE0      // Yellow (warning hot)
#define COLOR_RED_ZONE          0xF800      // Red (overheating)

// --- Battery Voltage Status Colors ---
#define COLOR_STATUS_OK         0x07E0      // Green (normal voltage)
#define COLOR_STATUS_WARNING    0xFFE0      // Yellow (low voltage warning)
#define COLOR_STATUS_ERROR      0xF800      // Red (critical voltage)
#define COLOR_TICK_MARKS        0xC618      // Light Grey
#define COLOR_STATUS_OK         0x07E0      // Green
#define COLOR_STATUS_WARNING    0xFFE0      // Yellow
#define COLOR_STATUS_ERROR      0xF800      // Red

// --- Text Configuration ---
#define FONT_SIZE_SMALL         1
#define FONT_SIZE_MEDIUM        2
#define FONT_SIZE_LARGE         3
#define TEXT_PADDING            5

// ============================================================================
// TIMING CONFIGURATION
// ============================================================================

// --- Update Intervals ---
#define SENSOR_UPDATE_INTERVAL  2000        // Sensor data update interval (ms)
#define DISPLAY_UPDATE_INTERVAL 100         // Display refresh interval (ms)
#define STATUS_UPDATE_INTERVAL  1000        // Status check interval (ms)
#define HEARTBEAT_INTERVAL      5000        // Heartbeat/keepalive interval (ms)

// --- Timeout Values ---
#define BT_CONNECTION_TIMEOUT   30000       // Bluetooth connection timeout (ms)
#define OBD2_INIT_TIMEOUT       10000       // OBD2 initialization timeout (ms)
#define SENSOR_DATA_TIMEOUT     5000        // Sensor data validity timeout (ms)
#define ERROR_RETRY_INTERVAL    5000        // Error recovery retry interval (ms)

// ============================================================================
// SYSTEM BEHAVIOR
// ============================================================================

// --- Debug Configuration (Production) ---
#define DEBUG_ENABLED           false       // Disable debug output for production
#define DEBUG_SERIAL_BAUD       115200      // Serial debug baud rate (keep for emergency)
#define DEBUG_OBD2_COMMANDS     false       // Disable OBD2 command logging
#define DEBUG_SENSOR_DATA       false       // Disable sensor data logging
#define DEBUG_DISPLAY_UPDATES   false       // Disable display update logging

// --- Error Handling ---
#define AUTO_RETRY_ENABLED      true        // Automatically retry failed operations
#define MAX_CONSECUTIVE_ERRORS  5           // Max errors before entering error state
#define WATCHDOG_ENABLED        true        // Enable software watchdog
#define WATCHDOG_TIMEOUT_MS     30000       // Watchdog timeout

// --- Power Management ---
#define SLEEP_MODE_ENABLED      false       // Enable sleep mode when idle
#define SLEEP_TIMEOUT_MS        300000      // Sleep after 5 minutes of inactivity
#define WAKE_ON_BT_ENABLED      true        // Wake up on Bluetooth activity

// ============================================================================
// PRODUCTION HARDWARE FLAGS
// ============================================================================

// --- Production Mode (Simulation Disabled) ---
#define SIMULATION_MODE         false       // Disable simulation mode for production
#define MOCK_OBD2_DATA          false       // Use real Bluetooth OBD2 communication
#define MOCK_BLUETOOTH          false       // Use real Bluetooth hardware
#define DISPLAY_DEBUG_INFO      false       // Disable debug info for production
#define SERIAL_SIMULATION_LOG   false       // Disable simulation logging

// --- Optional Features ---
#define ENABLE_DATA_LOGGING     false       // Log sensor data to SPIFFS
#define ENABLE_WIFI_CONFIG      false       // Enable WiFi configuration portal
#define ENABLE_OTA_UPDATES      false       // Enable Over-The-Air updates
#define ENABLE_WEB_INTERFACE    false       // Enable web-based configuration
#define ENABLE_SOUND_ALERTS     false       // Enable buzzer/speaker alerts
#define ENABLE_LED_INDICATORS   false       // Enable status LED indicators

// --- Gauge Features ---
#define ENABLE_GAUGE_ANIMATION  true        // Smooth needle animation
#define ENABLE_VALUE_SMOOTHING  true        // Smooth sensor value changes
#define ENABLE_PEAK_HOLD        true        // Show peak values
#define ENABLE_MIN_MAX_DISPLAY  false       // Show min/max values on screen

// ============================================================================
// VALIDATION MACROS
// ============================================================================

// Compile-time validation (integer values only)
#if GAUGE_RADIUS > (SCREEN_WIDTH / 2)
#error "Gauge radius too large for screen width"
#endif

#if SENSOR_UPDATE_INTERVAL < 1000
#warning "Sensor update interval less than 1 second may overload OBD2 interface"
#endif

// Runtime validation functions (for floating point values)
// These will be checked at runtime in the code

// ============================================================================
// UTILITY MACROS
// ============================================================================

// Convert milliseconds to seconds
#define MS_TO_SEC(ms)           ((ms) / 1000.0f)

// Convert seconds to milliseconds
#define SEC_TO_MS(sec)          ((sec) * 1000)

// Constrain value to range
#define CONSTRAIN(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

// Map value from one range to another
#define MAP(x, in_min, in_max, out_min, out_max) \
    (((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min))

// Unit conversion macros
#define KPA_TO_BAR(kpa)         ((kpa) / 100.0f)
#define BAR_TO_KPA(bar)         ((bar) * 100.0f)

// Temperature gauge needle position calculation
#define TEMP_NEEDLE_POSITION(temp) \
    ((temp) < TEMP_NEEDLE_MIN_TEMP ? TEMP_GAUGE_MIN_VALUE : (temp))

// Check if value is in valid range
#define IS_VALID_TEMP(val)      ((val) >= TEMP_VALID_MIN && (val) <= TEMP_VALID_MAX)
#define IS_VALID_BOOST(val)     ((val) >= BOOST_VALID_MIN && (val) <= BOOST_VALID_MAX)

// Debug print macros
#if DEBUG_ENABLED
    #define DEBUG_PRINT(x)      Serial.print(x)
    #define DEBUG_PRINTLN(x)    Serial.println(x)
    #define DEBUG_PRINTF(...)   Serial.printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTF(...)
#endif

// ============================================================================
// DISPLAY TEST CONFIGURATION
// ============================================================================

// --- Display Test Mode ---
// Uncomment the following line to enable display test mode
// This disables Bluetooth functionality and simulates sensor data for display testing
// #define DISPLAY_TEST

#ifdef DISPLAY_TEST
    // --- Temperature Test Parameters ---
    #define TEST_TEMP_MIN           (-20.0f)      // Starting temperature (°C)
    #define TEST_TEMP_MAX           135.0f      // Maximum temperature (°C)
    #define TEST_TEMP_STEP          5.0f        // Temperature increment per step (°C)
    #define TEST_TEMP_INTERVAL      1000        // Update interval in milliseconds

    // --- Turbo Pressure Test Parameters ---
    #define TEST_PRESSURE_MIN       0.9f        // Minimum pressure (bar)
    #define TEST_PRESSURE_MAX       2.5f        // Maximum pressure (bar)
    #define TEST_PRESSURE_STEP      0.1f        // Pressure increment per step (bar)
    #define TEST_PRESSURE_INTERVAL  1500        // Update interval in milliseconds

    // --- Battery Voltage Test Parameters ---
    #define TEST_VOLTAGE_MIN        11.5f       // Starting voltage (V)
    #define TEST_VOLTAGE_MAX        16.0f       // Maximum voltage (V)
    #define TEST_VOLTAGE_STEP       0.25f       // Voltage increment per step (V)
    #define TEST_VOLTAGE_INTERVAL   2000        // Update interval in milliseconds

    // --- Test Display Settings ---
    #define TEST_STATUS_INTERVAL    5000        // Status message update interval (ms)
    #define TEST_CONNECTION_MSG     "DISPLAY TEST MODE"
    #define TEST_ENABLE_DEBUG       true        // Enable debug output for test mode
#endif

// ============================================================================
// OBD2 DIAGNOSTIC SCANNER CONFIGURATION
// ============================================================================

// --- Diagnostic Mode Settings ---
#define DIAGNOSTIC_SCROLL_INTERVAL  3000    // Time to display each PID (ms)
#define DIAGNOSTIC_MAX_PIDS         50      // Maximum number of PIDs to scan
#define DIAGNOSTIC_LINES_PER_SCREEN 8       // Number of text lines per screen
#define DIAGNOSTIC_TEXT_SIZE        2       // Text size for diagnostic display

// --- Common OBD2 PIDs for Diagnostic Scanning ---
// Engine Data PIDs
#define PID_ENGINE_RPM              "010C"  // Engine RPM
#define PID_VEHICLE_SPEED           "010D"  // Vehicle Speed
#define PID_THROTTLE_POSITION       "0111"  // Throttle Position
#define PID_ENGINE_LOAD             "0104"  // Calculated Engine Load
#define PID_FUEL_LEVEL              "012F"  // Fuel Tank Level Input
#define PID_FUEL_PRESSURE           "010A"  // Fuel Pressure
#define PID_INTAKE_TEMP             "010F"  // Intake Air Temperature
#define PID_MAF_FLOW_RATE           "0110"  // Mass Air Flow Rate
#define PID_TIMING_ADVANCE          "010E"  // Timing Advance
#define PID_BAROMETRIC_PRESSURE     "0133"  // Barometric Pressure

// Emissions PIDs
#define PID_O2_SENSOR_1             "0114"  // Oxygen Sensor 1
#define PID_O2_SENSOR_2             "0115"  // Oxygen Sensor 2
#define PID_SHORT_FUEL_TRIM_1       "0106"  // Short Term Fuel Trim Bank 1
#define PID_LONG_FUEL_TRIM_1        "0107"  // Long Term Fuel Trim Bank 1
#define PID_SHORT_FUEL_TRIM_2       "0108"  // Short Term Fuel Trim Bank 2
#define PID_LONG_FUEL_TRIM_2        "0109"  // Long Term Fuel Trim Bank 2

// System PIDs
#define PID_RUNTIME_SINCE_START     "011F"  // Runtime Since Engine Start
#define PID_DISTANCE_WITH_MIL       "0121"  // Distance Traveled with MIL On
#define PID_FUEL_RAIL_PRESSURE      "0123"  // Fuel Rail Pressure
#define PID_COMMANDED_EGR           "012C"  // Commanded EGR
#define PID_EGR_ERROR               "012D"  // EGR Error

// PID Support Detection (used for discovery)
#define PID_SUPPORTED_01_20         "0100"  // PIDs supported 01-20
#define PID_SUPPORTED_21_40         "0120"  // PIDs supported 21-40
#define PID_SUPPORTED_41_60         "0140"  // PIDs supported 41-60

#endif // CONFIG_H
