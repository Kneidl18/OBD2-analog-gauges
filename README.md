# ESP32 OBD2 Analog Gauges with Diagnostic Scanner

[![PlatformIO CI](https://github.com/Kneidl18/OBD2-analog-gauges/workflows/PlatformIO%20CI/badge.svg)](https://github.com/Kneidl18/OBD2-analog-gauges/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-green.svg)](https://www.espressif.com/en/products/socs/esp32)

A comprehensive ESP32-based automotive display system that provides real-time analog gauges and advanced OBD2 diagnostic capabilities. Perfect for car enthusiasts, mechanics, and makers who want professional-grade vehicle monitoring.

![Project Demo](docs/images/demo.jpg)

## 🚗 Features

### Analog Gauge Display
- **Real-time Analog Gauges**: Beautiful analog-style gauges for temperature and boost pressure
- **Battery Voltage Monitoring**: Digital display with color-coded status indicators
- **Customizable Thresholds**: Configurable warning zones (green/yellow/red)
- **Professional Interface**: Clean, automotive-grade display design

### Advanced Diagnostic Scanner
- **Comprehensive PID Support**: Access to 15+ vehicle parameters
- **Automatic PID Discovery**: Detects vehicle-supported diagnostic parameters
- **Real-time Data**: Live updates of engine RPM, speed, fuel level, emissions data
- **Scrollable Interface**: Easy navigation through all available diagnostics

### Smart Connectivity
- **Bluetooth OBD2**: Wireless connection to ELM327-compatible adapters
- **Auto-reconnection**: Robust connection handling with automatic recovery
- **Universal Compatibility**: Works with any OBD2-compliant vehicle (1996+)

### Hardware Features
- **Physical Button Control**: Dedicated button for diagnostic mode switching
- **TFT Color Display**: High-resolution 240x320 pixel display
- **Compact Design**: Fits standard automotive mounting solutions

## 📋 Hardware Requirements

### Core Components
| Component | Specification | Quantity | Estimated Cost |
|-----------|---------------|----------|----------------|
| ESP32 Development Board | ESP32 DevKit v1 or similar | 1 | $8-15 |
| TFT Display | ILI9341 240x320 SPI Display | 1 | $10-20 |
| Push Button | Momentary tactile switch | 1 | $1-3 |
| OBD2 Bluetooth Adapter | ELM327 compatible | 1 | $15-30 |
| Jumper Wires | Male-to-male/female | 10+ | $5 |
| Breadboard/PCB | For prototyping | 1 | $5-10 |

**Total Estimated Cost: $44-83**

### Optional Components
- 3D printed enclosure
- Automotive-grade connectors
- Power regulation circuit
- SD card module for data logging

## 🔌 Wiring Diagram

### TFT Display Connection (ILI9341)
```
ESP32 Pin    →    TFT Pin
GPIO 23      →    MOSI (SDA)
GPIO 19      →    MISO
GPIO 18      →    SCK (CLK)
GPIO 15      →    CS
GPIO 2       →    DC
GPIO 4       →    RST
3.3V         →    VCC
GND          →    GND
```

### Diagnostic Button
```
ESP32 Pin    →    Component
GPIO 21      →    Button (one side)
GND          →    Button (other side)
```

### Power Supply
```
12V Vehicle Power → Voltage Regulator → 5V → ESP32 VIN
                                     → 3.3V → TFT Display
```

## 🚀 Quick Start

### 1. Hardware Setup
1. Connect the TFT display to ESP32 according to the wiring diagram
2. Connect the diagnostic button between GPIO 21 and GND
3. Power the ESP32 via USB (development) or 5V supply (deployment)

### 2. Software Installation

#### Prerequisites
- [PlatformIO](https://platformio.org/) installed
- Git for version control
- USB cable for ESP32 programming

#### Clone and Build
```bash
git clone https://github.com/Kneidl18/OBD2-analog-gauges.git
cd OBD2-analog-gauges
pio run --target upload
```

#### Alternative: Arduino IDE
1. Install ESP32 board support
2. Install required libraries (see [dependencies](#dependencies))
3. Open `src/main.cpp` in Arduino IDE
4. Select ESP32 board and upload

### 3. OBD2 Setup
1. Plug ELM327 adapter into vehicle's OBD2 port
2. Pair ESP32 with ELM327 via Bluetooth
3. Start engine and verify connection

## 📱 Usage Guide

### Normal Gauge Mode
- **Temperature Gauge**: Shows engine coolant temperature with color zones
- **Boost Pressure**: Displays turbo/intake manifold pressure
- **Battery Voltage**: Digital readout with status indicators
- **Status Bar**: Connection status and system messages

### Diagnostic Scanner Mode
1. **Enter Mode**: Short press of diagnostic button
2. **Navigate**: Short press to advance through parameters
3. **Auto-scroll**: Automatic advancement every 3 seconds
4. **Exit Mode**: Long press (1 second) to return to gauges

### Available Diagnostic Parameters
- Engine RPM and vehicle speed
- Throttle position and engine load
- Fuel level and fuel system trims
- Intake air and coolant temperatures
- O2 sensor readings and emissions data
- System runtime and diagnostic codes

## ⚙️ Configuration

### Display Test Mode
Enable hardware testing without OBD2 connection:
```cpp
// In include/config.h
#define DISPLAY_TEST  // Uncomment to enable test mode
```

### Gauge Customization
```cpp
// Temperature gauge settings
#define TEMP_BLUE_THRESHOLD     80.0f   // Cold zone (°C)
#define TEMP_GREEN_THRESHOLD    100.0f  // Normal zone (°C)
#define TEMP_YELLOW_THRESHOLD   115.0f  // Warning zone (°C)

// Boost pressure settings
#define BOOST_GREEN_THRESHOLD   1.8f    // Normal zone (bar)
#define BOOST_YELLOW_THRESHOLD  2.2f    // Warning zone (bar)
```

### Button Configuration
```cpp
#define DIAGNOSTIC_BUTTON_PIN   21      // GPIO pin
#define BUTTON_DEBOUNCE_MS      50      // Debounce time
#define BUTTON_LONG_PRESS_MS    1000    // Long press duration
```

## 📚 Dependencies

### PlatformIO Libraries (Automatic)
- `TFT_eSPI` - TFT display driver
- `BluetoothSerial` - ESP32 Bluetooth communication

### Manual Installation (Arduino IDE)
```
TFT_eSPI by Bodmer
ESP32 BluetoothSerial (built-in)
```

## 🔧 Troubleshooting

### Common Issues

#### Display Not Working
- ✅ Check TFT wiring connections
- ✅ Verify 3.3V power supply
- ✅ Ensure correct TFT_eSPI configuration

#### OBD2 Connection Failed
- ✅ Verify ELM327 adapter compatibility
- ✅ Check Bluetooth pairing
- ✅ Ensure vehicle is running
- ✅ Try different OBD2 adapter

#### Button Not Responding
- ✅ Check GPIO 21 connection
- ✅ Verify button wiring (active LOW)
- ✅ Test with multimeter

#### Compilation Errors
- ✅ Update PlatformIO/Arduino IDE
- ✅ Check library versions
- ✅ Verify ESP32 board selection

### Debug Mode
Enable detailed logging:
```cpp
#define DEBUG_ENABLED true
#define DEBUG_OBD2_COMMANDS true
```

## 📖 Documentation

- [Diagnostic Scanner Guide](README_DIAGNOSTIC_SCANNER.md) - Detailed diagnostic features
- [Hardware Deployment](README_HARDWARE_DEPLOYMENT.md) - Installation guide
- [API Documentation](docs/API.md) - Code reference
- [Troubleshooting](docs/TROUBLESHOOTING.md) - Extended troubleshooting

## 🤝 Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Development Setup
1. Fork the repository
2. Create feature branch: `git checkout -b feature/amazing-feature`
3. Commit changes: `git commit -m 'Add amazing feature'`
4. Push to branch: `git push origin feature/amazing-feature`
5. Open Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library by Bodmer
- ESP32 community for excellent documentation
- OBD2 specification contributors
- Automotive enthusiast community

## 📞 Support

- 🐛 **Bug Reports**: [GitHub Issues](https://github.com/Kneidl18/OBD2-analog-gauges/issues)
- 💡 **Feature Requests**: [GitHub Discussions](https://github.com/Kneidl18/OBD2-analog-gauges/discussions)
- 📧 **Contact**: andreas.auer03@gmail.com

---

**⭐ If this project helped you, please give it a star! ⭐**
