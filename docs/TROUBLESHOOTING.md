# Troubleshooting Guide

## 🔧 Common Issues and Solutions

### Display Problems

#### Display Shows Nothing (Black Screen)
**Symptoms**: TFT display remains black, no output visible

**Solutions**:
1. **Power Issues**
   - ✅ Check 3.3V supply to display
   - ✅ Verify GND connections
   - ✅ Ensure adequate current supply (>500mA)

2. **Wiring Problems**
   - ✅ Verify all SPI connections (MOSI, MISO, SCK, CS, DC, RST)
   - ✅ Check for loose connections
   - ✅ Confirm pin assignments match config.h

3. **Software Configuration**
   - ✅ Verify TFT_eSPI library configuration
   - ✅ Check display driver selection (ILI9341)

#### Display Shows Corrupted Data
**Symptoms**: Random pixels, wrong colors, distorted images

**Solutions**:
- ✅ Reduce SPI frequency in config.h
- ✅ Use shorter, higher-quality jumper wires
- ✅ Add decoupling capacitors near display
- ✅ Check for electromagnetic interference

### OBD2 Connection Issues

#### Cannot Connect to OBD2 Adapter
**Symptoms**: "Waiting for Bluetooth..." message persists

**Solutions**:
1. **Adapter Verification**
   - ✅ Confirm ELM327 adapter is powered
   - ✅ Check adapter compatibility (v1.5+ recommended)
   - ✅ Test adapter with smartphone OBD2 app

2. **Bluetooth Pairing**
   - ✅ Ensure adapter is in pairing mode
   - ✅ Check device name matches config
   - ✅ Verify PIN code (usually 1234)

3. **Vehicle Compatibility**
   - ✅ Confirm vehicle is OBD2 compliant (1996+)
   - ✅ Engine must be running
   - ✅ Key in ON position

#### No Sensor Data
**Symptoms**: Connected but gauges show no data

**Solutions**:
- ✅ Enable debug mode in config.h
- ✅ Check serial output for errors
- ✅ Verify vehicle supports requested PIDs
- ✅ Test with basic PIDs first (RPM, speed)

### Button Problems

#### Button Not Responding
**Solutions**:
- ✅ Check GPIO 21 connection to button
- ✅ Verify button connects to GND (active LOW)
- ✅ Test button continuity with multimeter
- ✅ Adjust debounce timing if needed

### Compilation Errors

#### Library Not Found
**Error**: `TFT_eSPI.h: No such file or directory`

**Solutions**:
- ✅ Install TFT_eSPI library via PlatformIO
- ✅ Check library dependencies
- ✅ Verify library version compatibility

#### Memory Issues
**Error**: `region 'dram0_0_seg' overflowed`

**Solutions**:
- ✅ Reduce debug output
- ✅ Optimize string usage
- ✅ Disable unused features

## 🔍 Debug Mode

Enable detailed logging:
```cpp
#define DEBUG_ENABLED true
#define DEBUG_OBD2_COMMANDS true
```

## 📞 Getting Help

### Information to Include
- Hardware setup (ESP32 model, display type)
- Software versions (PlatformIO, libraries)
- Serial output with debug enabled
- Photos of wiring if applicable

### Support Channels
- 🐛 GitHub Issues: For bugs
- 💬 GitHub Discussions: For questions
- 📧 Email: andreas.auer03@gmail.com
