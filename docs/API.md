# API Documentation

## Class Overview

### AutomotiveDisplay
Main application controller that coordinates all components.

#### Public Methods
```cpp
bool begin()                    // Initialize the system
void update()                   // Main update loop (call in loop())
AppState getCurrentState()      // Get current application state
```

#### States
```cpp
enum class AppState {
    INITIALIZING,     // System startup
    WAITING_BT,       // Waiting for Bluetooth connection
    CONNECTING_OBD2,  // Connecting to OBD2 interface
    RUNNING,          // Normal gauge operation
    DIAGNOSTIC_MODE,  // Diagnostic scanner active
    ERROR             // Error state
};
```

### OBD2Manager
Handles all OBD2 communication and data parsing.

#### Public Methods
```cpp
bool begin()                                    // Initialize Bluetooth
void update()                                   // Update connection state
SensorData requestSensorData()                  // Get current sensor readings
bool isReady()                                  // Check if ready for requests
bool isConnected()                              // Check Bluetooth connection
std::vector<String> discoverSupportedPIDs()    // Find supported PIDs
String getDiagnosticData(const String& pid)    // Get specific PID data
std::vector<String> getAllDiagnosticData()     // Get all diagnostic data
float requestBatteryVoltage()                   // Get battery voltage
```

#### Supported PIDs
```cpp
#define PID_ENGINE_RPM              "010C"  // Engine RPM
#define PID_VEHICLE_SPEED           "010D"  // Vehicle Speed (km/h)
#define PID_THROTTLE_POSITION       "0111"  // Throttle Position (%)
#define PID_ENGINE_LOAD             "0104"  // Engine Load (%)
#define PID_FUEL_LEVEL              "012F"  // Fuel Level (%)
#define PID_INTAKE_TEMP             "010F"  // Intake Air Temperature (°C)
#define PID_MAF_FLOW_RATE           "0110"  // Mass Air Flow (g/s)
#define PID_TIMING_ADVANCE          "010E"  // Timing Advance (degrees)
#define PID_FUEL_PRESSURE           "010A"  // Fuel Pressure (kPa)
#define TEMP_PID                    "0105"  // Coolant Temperature (°C)
#define BOOST_PID                   "010B"  // Intake Manifold Pressure (kPa)
```

### GaugeDisplay
Manages TFT display and rendering of gauges and diagnostic data.

#### Public Methods
```cpp
bool begin()                                    // Initialize display
void updateGauges(const SensorData& data)      // Update analog gauges
void showStatus(const String& message)         // Display status message
void clear()                                    // Clear display
void showDiagnosticData(const std::vector<String>& data, int index)  // Show diagnostic text
void showDiagnosticHeader()                    // Show diagnostic mode header
```

#### Gauge Configuration
```cpp
struct GaugeConfig {
    int centerX, centerY;       // Gauge center position
    int radius;                 // Gauge radius
    float minValue, maxValue;   // Scale range
    float blueThreshold;        // Cold zone limit
    float greenThreshold;       // Normal zone limit
    float yellowThreshold;      // Warning zone limit
    String title, unit;         // Display text
    uint16_t needleColor;       // Needle color
};
```

### SensorData
Data structure for sensor readings.

#### Structure
```cpp
struct SensorData {
    float coolantTemperature;   // Engine coolant temperature (°C)
    float boostPressure;        // Turbo/intake pressure (bar)
    float batteryVoltage;       // Vehicle battery voltage (V)
    bool batteryVoltageValid;   // Battery reading validity
    bool isValid;               // Overall data validity
    unsigned long timestamp;    // Reading timestamp
    
    void updateBatteryVoltage(float voltage);  // Update battery data
};
```

## Configuration Constants

### Hardware Pins
```cpp
#define TFT_MISO_PIN            19      // Display MISO
#define TFT_MOSI_PIN            23      // Display MOSI
#define TFT_SCLK_PIN            18      // Display clock
#define TFT_CS_PIN              15      // Display chip select
#define TFT_DC_PIN              2       // Display data/command
#define TFT_RST_PIN             4       // Display reset
#define DIAGNOSTIC_BUTTON_PIN   21      // Diagnostic button
```

### Sensor Thresholds
```cpp
// Temperature (°C)
#define TEMP_BLUE_THRESHOLD     80.0f   // Cold zone
#define TEMP_GREEN_THRESHOLD    100.0f  // Normal zone
#define TEMP_YELLOW_THRESHOLD   115.0f  // Warning zone
#define TEMP_CRITICAL_VALUE     115.0f  // Critical alert

// Boost Pressure (bar)
#define BOOST_GREEN_THRESHOLD   1.8f    // Normal zone
#define BOOST_YELLOW_THRESHOLD  2.2f    // Warning zone
#define BOOST_CRITICAL_VALUE    2.4f    // Critical alert

// Battery Voltage (V)
#define VOLTAGE_LOW_THRESHOLD   12.5f   // Low voltage warning
#define VOLTAGE_HIGH_THRESHOLD  14.5f   // High voltage warning
```

### Timing Configuration
```cpp
#define UPDATE_INTERVAL         2000    // Sensor update interval (ms)
#define BUTTON_DEBOUNCE_MS      50      // Button debounce time (ms)
#define BUTTON_LONG_PRESS_MS    1000    // Long press duration (ms)
#define DIAGNOSTIC_SCROLL_INTERVAL 3000 // Auto-scroll time (ms)
```

## Usage Examples

### Basic Initialization
```cpp
#include "AutomotiveDisplay.h"

AutomotiveDisplay display;

void setup() {
    Serial.begin(115200);
    if (!display.begin()) {
        Serial.println("Failed to initialize!");
        while(1);
    }
}

void loop() {
    display.update();
    delay(10);
}
```

### Custom Gauge Thresholds
```cpp
// Modify in config.h before compilation
#define TEMP_GREEN_THRESHOLD    95.0f   // Custom normal temp
#define BOOST_YELLOW_THRESHOLD  2.0f    // Custom boost warning
```

### Debug Mode
```cpp
// Enable in config.h
#define DEBUG_ENABLED true
#define DEBUG_OBD2_COMMANDS true
#define DEBUG_DISPLAY_UPDATES true
```

### Display Test Mode
```cpp
// Enable in config.h for testing without OBD2
#define DISPLAY_TEST
```

## Error Handling

### Return Values
- `bool` methods return `true` on success, `false` on failure
- `SensorData` has `isValid` flag for data validity
- Check connection state before requesting data

### Error States
- `AppState::ERROR` indicates system error
- Monitor serial output for detailed error messages
- Use debug mode for troubleshooting

### Recovery Methods
- System automatically attempts reconnection
- Manual reset via hardware reset button
- Factory reset by re-uploading firmware

## Memory Considerations

### RAM Usage
- Base system: ~25KB
- Diagnostic mode: +5KB for PID storage
- Display buffers: ~15KB
- Total: ~45KB (ESP32 has 320KB available)

### Flash Usage
- Core firmware: ~800KB
- Libraries: ~300KB
- Total: ~1.1MB (ESP32 has 4MB available)

### Optimization Tips
- Disable unused debug output
- Reduce diagnostic PID count if needed
- Use PROGMEM for constant strings
- Optimize display update frequency
