# ESP32 OBD2 Automotive Display - Hardware Deployment Guide

## Overview

This is the production-ready firmware for the ESP32 OBD2 Automotive Display system. It provides real-time automotive sensor monitoring with analog gauge displays, specifically designed for coolant temperature and turbo boost pressure monitoring.

## Hardware Requirements

### Required Components

1. **ESP32 DevKit v1 Board**
   - 240MHz dual-core processor
   - 4MB Flash memory
   - Built-in WiFi and Bluetooth

2. **ILI9341 TFT Display (240x320)**
   - 2.8" or 3.2" TFT LCD
   - SPI interface
   - 16-bit color (RGB565)

3. **OBD2 Bluetooth Dongle**
   - ELM327 compatible (vLinker BM+ recommended)
   - Bluetooth Classic (not BLE)
   - Supports PIDs 0105 (coolant temp) and 010B (manifold pressure)
   - Must support AT RV command for battery voltage monitoring

4. **Connecting Wires**
   - Dupont jumper wires
   - Breadboard or PCB for connections

### Wiring Diagram

```
ESP32 DevKit v1    →    ILI9341 TFT Display
================        ===================
3.3V               →    VCC
GND                →    GND
GPIO 23 (MOSI)     →    SDI/MOSI
GPIO 19 (MISO)     →    SDO/MISO
GPIO 18 (SCK)      →    SCK
GPIO 15 (CS)       →    CS
GPIO 2  (DC)       →    DC/RS
GPIO 4  (RST)      →    RESET
```

### Power Requirements

- **ESP32**: 3.3V, ~240mA (typical)
- **TFT Display**: 3.3V, ~100mA (typical)
- **Total**: ~340mA @ 3.3V
- **Recommended**: 5V 1A power supply with onboard regulator

## Software Features

### Automotive Sensor Monitoring

1. **Coolant Temperature Gauge**
   - Range: 40°C to 130°C
   - Special behavior: Needle stays at 0% below 40°C
   - Always displays actual temperature value
   - Color zones: Blue (below 80°C - cold engine), Green (80-100°C - normal operating), Yellow (100-115°C - warning hot), Red (115°C+ - overheating)

2. **Turbo Boost Pressure Gauge**
   - Range: 0.9 to 2.5 bar
   - Automatic kPa to bar conversion
   - Color zones: Green (0.9-1.8bar), Yellow (1.8-2.2bar), Red (2.2bar+)

3. **Battery Voltage Monitoring**
   - Range: 8.0V to 18.0V (automotive battery range)
   - Real-time voltage display in top-right corner
   - Color zones: Red (<12.5V or >14.5V), Yellow (12.5-13.0V), Green (13.0-14.5V)
   - Uses ELM327 AT RV command for direct voltage reading

4. **OBD2 Communication**
   - Bluetooth connection to ELM327 dongle
   - PID 0105: Engine Coolant Temperature
   - PID 010B: Intake Manifold Absolute Pressure
   - AT RV: Battery Voltage (ELM327 built-in command)
   - 2-second update interval for sensors, 3-second for battery voltage

### Display Features

- **Analog Gauges**: Realistic needle movement with smooth animation
- **Color-coded Zones**: Visual indication of safe/warning/critical ranges
- **Real-time Values**: Numeric display of current sensor readings
- **Status Messages**: Connection status and error notifications
- **Auto-reconnect**: Automatic Bluetooth reconnection on disconnect

## Installation Instructions

### Step 1: Hardware Assembly

1. **Connect the TFT Display** to ESP32 according to wiring diagram
2. **Double-check connections** - incorrect wiring can damage components
3. **Power the ESP32** via USB or external 5V supply

### Step 2: Software Installation

#### Prerequisites

- **PlatformIO IDE** or **PlatformIO Core**
- **USB cable** for ESP32 programming
- **Git** for version control

#### Build and Upload

```bash
# Clone the repository (if not already done)
git clone <repository-url>
cd c++/obd2_turbo_watertemperature_display

# Switch to production branch
git checkout production-hardware

# Build the firmware
platformio run

# Upload to ESP32 (connect via USB)
platformio run --target upload

# Monitor serial output (optional)
platformio device monitor
```

### Step 3: OBD2 Setup

1. **Connect OBD2 dongle** to vehicle's OBD2 port
2. **Power on** the ESP32 automotive display
3. **Pair Bluetooth**: Look for "ESP32_OBD2_Display" in Bluetooth settings
4. **Wait for connection**: Display will show "OBD2 Connected" when ready

## Configuration

### Hardware Configuration

Edit `include/config.h` to modify:

```cpp
// Pin assignments (if different wiring used)
#define TFT_CS_PIN              15
#define TFT_DC_PIN              2
#define TFT_RST_PIN             4
#define TFT_MOSI_PIN            23
#define TFT_SCLK_PIN            18
#define TFT_MISO_PIN            19

// Sensor ranges (if needed)
#define TEMP_MAX_VALUE          130.0f      // Maximum temperature
#define BOOST_MAX_VALUE         2.5f        // Maximum boost pressure

// Update intervals
#define SENSOR_UPDATE_INTERVAL  2000        // 2 seconds
#define DISPLAY_UPDATE_INTERVAL 100         // 100ms (10 FPS)

// Battery voltage monitoring (disable if dongle doesn't support AT RV)
#define BATTERY_VOLTAGE_ENABLED true        // Set to false for incompatible dongles
```

### Bluetooth Configuration

```cpp
// Bluetooth device name (visible to OBD2 dongle)
#define BT_DEVICE_NAME          "ESP32_OBD2_Display"

// OBD2 communication settings
#define OBD2_BAUD_RATE          38400       // Standard ELM327 baud rate
#define OBD2_TIMEOUT_MS         3000        // Response timeout
```

## Troubleshooting

### Common Issues

1. **Display not working**
   - Check wiring connections
   - Verify 3.3V power supply
   - Ensure correct pin assignments in config.h

2. **Bluetooth not connecting**
   - Verify OBD2 dongle compatibility (ELM327)
   - Check vehicle OBD2 port functionality
   - Ensure dongle is in pairing mode

3. **No sensor data**
   - Verify vehicle supports PIDs 0105 and 010B
   - Check OBD2 dongle LED indicators
   - Try different OBD2 dongle

4. **Battery voltage not displaying**
   - Check if OBD2 dongle supports AT RV command
   - Verify BATTERY_VOLTAGE_ENABLED is set to true in config.h
   - Some older ELM327 dongles may not support voltage reading
   - Try different OBD2 dongle if "BATT ERR" persists

5. **Compilation errors**
   - Update PlatformIO platform: `pio platform update espressif32`
   - Clean build: `pio run --target clean`
   - Check library dependencies

### Debug Mode

To enable debug output for troubleshooting:

```cpp
// In include/config.h
#define DEBUG_ENABLED           true
#define DEBUG_OBD2_COMMANDS     true
#define DEBUG_SENSOR_DATA       true
```

Then monitor serial output:
```bash
platformio device monitor --baud 115200
```

## Performance Specifications

### Memory Usage

- **Flash**: ~1.1MB (86% of 1.3MB available)
- **RAM**: ~39KB (12% of 320KB available)
- **Stack**: ~8KB per task

### Timing Performance

- **Sensor Update**: 2 seconds (configurable)
- **Display Refresh**: 10 FPS (100ms interval)
- **Bluetooth Latency**: <500ms typical
- **Boot Time**: ~3 seconds to full operation

### Power Consumption

- **Active Operation**: ~340mA @ 3.3V
- **Bluetooth Scanning**: +20mA
- **Display Backlight**: ~80mA (if used)
- **Total System**: ~440mA @ 3.3V

## Automotive Integration

### Vehicle Compatibility

**Supported Vehicles**: Any vehicle with OBD2 port (1996+ in US, 2001+ in EU)

**Required OBD2 PIDs**:
- **PID 0105**: Engine Coolant Temperature (mandatory on most vehicles)
- **PID 010B**: Intake Manifold Absolute Pressure (turbocharged vehicles)

**Tested Vehicles**:
- Turbocharged gasoline engines
- Diesel engines with turbocharger
- Hybrid vehicles (OBD2 portion)

### Installation Locations

**Dashboard Mount**: Use suction cup or adhesive mount
**A-Pillar Mount**: Custom bracket for permanent installation
**Center Console**: Integration with existing gauge cluster

### Safety Considerations

- **Do not obstruct driver vision**
- **Secure mounting** to prevent movement during driving
- **Emergency access** to OBD2 port for diagnostics
- **Fuse protection** if hardwired to vehicle power

## Maintenance

### Regular Maintenance

- **Clean display** with soft cloth
- **Check connections** periodically
- **Update firmware** when new versions available
- **Backup configuration** before modifications

### Firmware Updates

```bash
# Pull latest changes
git pull origin production-hardware

# Build and upload
platformio run --target upload
```

## Support and Documentation

### Additional Resources

- **PlatformIO Documentation**: https://docs.platformio.org/
- **ESP32 Documentation**: https://docs.espressif.com/projects/esp-idf/
- **TFT_eSPI Library**: https://github.com/Bodmer/TFT_eSPI
- **OBD2 PID Reference**: https://en.wikipedia.org/wiki/OBD-II_PIDs

### Version Information

- **Firmware Version**: v1.0.0-hardware-ready
- **Target Platform**: ESP32 DevKit v1
- **Build Environment**: PlatformIO + Arduino Framework
- **Last Updated**: [Current Date]

---

**⚠️ Important**: This device is for informational purposes only. Do not rely solely on this display for critical engine monitoring. Always refer to vehicle's original gauges for safety-critical information.
