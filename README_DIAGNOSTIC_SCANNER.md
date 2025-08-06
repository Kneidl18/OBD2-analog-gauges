# OBD2 Diagnostic Scanner Feature

## Overview

The OBD2 Diagnostic Scanner is an advanced feature that allows comprehensive vehicle diagnostics beyond the standard gauge display. This feature provides access to all available OBD2 PIDs (Parameter IDs) supported by the connected vehicle, displaying real-time diagnostic data in a scrollable text format.

## Hardware Requirements

### Physical Button Connection
- **GPIO Pin**: 21 (configurable in `config.h`)
- **Connection**: Connect button between GPIO 21 and GND
- **Pull-up**: Internal pull-up resistor is enabled (button is active LOW)
- **Debouncing**: Software debouncing implemented (50ms default)

### Wiring Diagram
```
ESP32 GPIO 21 ----[Button]---- GND
```

## Features

### 1. Button Controls
- **Short Press**: 
  - In normal mode: Enter diagnostic scanner mode
  - In diagnostic mode: Advance to next PID
- **Long Press** (1 second): Exit diagnostic scanner mode
- **Debouncing**: 50ms debounce time prevents false triggers

### 2. PID Discovery
- Automatically discovers supported PIDs from the vehicle
- Queries PID support ranges: 01-20, 21-40, 41-60
- Only displays PIDs that the vehicle actually supports
- Handles vehicles with limited PID support gracefully

### 3. Comprehensive Diagnostic Data
The scanner displays the following parameters (when supported by vehicle):

#### Engine Parameters
- **Engine RPM**: Real-time engine speed
- **Vehicle Speed**: Current vehicle speed in km/h
- **Throttle Position**: Throttle valve position percentage
- **Engine Load**: Calculated engine load percentage
- **Timing Advance**: Ignition timing advance
- **MAF Flow Rate**: Mass Air Flow sensor reading

#### Fuel System
- **Fuel Level**: Fuel tank level percentage
- **Fuel Pressure**: Fuel rail pressure
- **Short Term Fuel Trim**: Bank 1 fuel trim
- **Long Term Fuel Trim**: Bank 1 fuel trim

#### Temperature Sensors
- **Coolant Temperature**: Engine coolant temperature
- **Intake Air Temperature**: Intake manifold temperature

#### Pressure Sensors
- **Intake Manifold Pressure**: Boost/vacuum pressure
- **Barometric Pressure**: Atmospheric pressure

#### Emissions
- **O2 Sensor 1**: Oxygen sensor voltage/status
- **O2 Sensor 2**: Secondary oxygen sensor

#### System Information
- **Runtime Since Start**: Engine runtime since last start
- **Battery Voltage**: Vehicle electrical system voltage

### 4. Display Interface

#### Diagnostic Mode Display
- **Header**: "OBD2 DIAGNOSTICS" with cyan highlighting
- **Data Display**: Up to 8 lines of diagnostic data per screen
- **Current Item Highlighting**: Yellow background for selected item
- **Navigation Info**: Shows current item number and total count
- **Auto-scroll**: Automatically advances every 3 seconds
- **Text Truncation**: Long parameter names are truncated with "..."

#### Status Messages
- Connection status indicators
- "OBD2 not ready" when system is not connected
- "No diagnostic data available" when no PIDs are supported
- Real-time scanning status

## Configuration

### Button Settings (`config.h`)
```cpp
#define DIAGNOSTIC_BUTTON_PIN   21          // GPIO pin for button
#define BUTTON_DEBOUNCE_MS      50          // Debounce time
#define BUTTON_LONG_PRESS_MS    1000        // Long press duration
```

### Display Settings (`config.h`)
```cpp
#define DIAGNOSTIC_SCROLL_INTERVAL  3000    // Auto-scroll time (ms)
#define DIAGNOSTIC_MAX_PIDS         50      // Maximum PIDs to scan
#define DIAGNOSTIC_LINES_PER_SCREEN 8       // Lines per screen
#define DIAGNOSTIC_TEXT_SIZE        2       // Text size
```

### Supported PIDs (`config.h`)
All common OBD2 PIDs are pre-defined with human-readable names:
```cpp
#define PID_ENGINE_RPM              "010C"  // Engine RPM
#define PID_VEHICLE_SPEED           "010D"  // Vehicle Speed
#define PID_THROTTLE_POSITION       "0111"  // Throttle Position
// ... and many more
```

## Usage Instructions

### Entering Diagnostic Mode
1. Ensure OBD2 connection is established (normal gauge display working)
2. Press the diagnostic button (short press)
3. Display will clear and show "OBD2 DIAGNOSTICS" header
4. System will begin scanning for supported PIDs

### Navigating Diagnostic Data
- **Automatic**: Data automatically scrolls every 3 seconds
- **Manual**: Short press button to advance to next parameter
- **Highlighting**: Current parameter is highlighted in yellow
- **Pagination**: Display shows 8 parameters at a time

### Exiting Diagnostic Mode
1. Hold the diagnostic button for 1 second (long press)
2. System returns to normal gauge display mode
3. Previous display state is restored

## Technical Implementation

### State Management
- New `DIAGNOSTIC_MODE` state added to `AppState` enum
- `diagnosticModeActive` flag tracks current mode
- Button state tracking with debouncing logic

### PID Discovery Algorithm
1. Query PID support bitmaps (0100, 0120, 0140)
2. Parse 32-bit response to identify supported PIDs
3. Build vector of supported PID strings
4. Only query PIDs that vehicle supports

### Data Parsing
- PID-specific parsing for different data types
- Automatic unit conversion (RPM, %, °C, kPa, etc.)
- Error handling for invalid responses
- Fallback to raw hex data for unknown PIDs

### Memory Management
- Efficient vector usage for PID storage
- String optimization for display text
- Minimal memory footprint during operation

## Troubleshooting

### Button Not Responding
- Check GPIO 21 connection to button
- Verify button is connected to GND (active LOW)
- Check `DIAGNOSTIC_BUTTON_PIN` setting in config.h

### No Diagnostic Data
- Ensure OBD2 connection is established first
- Some vehicles support limited PIDs
- Check vehicle compatibility with OBD2 standard

### Display Issues
- Verify TFT display is working in normal mode
- Check text size settings if display appears corrupted
- Ensure adequate power supply for ESP32 and display

## Compatibility

### Vehicle Compatibility
- Works with any OBD2-compliant vehicle (1996+)
- Automatically adapts to vehicle's supported PID set
- Gracefully handles vehicles with limited diagnostic support

### OBD2 Interface Compatibility
- ELM327-based Bluetooth adapters
- Standard OBD2 protocols (ISO 9141, KWP2000, CAN)
- Automatic protocol detection

## Future Enhancements

### Planned Features
- DTC (Diagnostic Trouble Code) reading and clearing
- Freeze frame data display
- Data logging to SD card
- Custom PID definitions
- Real-time graphing of selected parameters

### Performance Optimizations
- Faster PID scanning algorithms
- Cached PID support detection
- Optimized display refresh rates
- Background data collection
