# Wiring Guide

## 📋 Complete Connection Diagram

### ESP32 to ILI9341 TFT Display

```
ESP32 DevKit v1          ILI9341 TFT Display
┌─────────────────┐      ┌─────────────────┐
│                 │      │                 │
│ 3V3        ●────┼──────┼────●  VCC       │
│ GND        ●────┼──────┼────●  GND       │
│ GPIO 23    ●────┼──────┼────●  MOSI/SDA  │
│ GPIO 19    ●────┼──────┼────●  MISO      │
│ GPIO 18    ●────┼──────┼────●  SCK/CLK   │
│ GPIO 15    ●────┼──────┼────●  CS        │
│ GPIO 2     ●────┼──────┼────●  DC        │
│ GPIO 4     ●────┼──────┼────●  RST       │
│                 │      │                 │
│ GPIO 21    ●────┼──┐   │                 │
│ GND        ●────┼──┼─┐ │                 │
└─────────────────┘  │ │ └─────────────────┘
                     │ │
              ┌──────┘ │
              │  ┌─────┘
              │  │
         ┌────▼──▼────┐
         │ Push Button │
         │   (N.O.)    │
         └─────────────┘
```

### Detailed Pin Connections

| ESP32 Pin | Function | ILI9341 Pin | Wire Color (Suggested) |
|-----------|----------|-------------|------------------------|
| 3V3       | Power    | VCC         | Red                    |
| GND       | Ground   | GND         | Black                  |
| GPIO 23   | SPI MOSI | MOSI/SDA    | Blue                   |
| GPIO 19   | SPI MISO | MISO        | Green                  |
| GPIO 18   | SPI CLK  | SCK/CLK     | Yellow                 |
| GPIO 15   | Chip Select | CS       | Orange                 |
| GPIO 2    | Data/Command | DC      | Purple                 |
| GPIO 4    | Reset    | RST         | White                  |
| GPIO 21   | Button Input | -       | Brown                  |
| GND       | Button Ground | -      | Black                  |

## 🔌 Power Supply Options

### Option 1: USB Development Power
```
Computer USB ──→ ESP32 USB Port ──→ 3V3 Regulator ──→ TFT Display
```
- **Pros**: Simple, safe for development
- **Cons**: Not suitable for vehicle installation

### Option 2: Vehicle 12V Power
```
Vehicle 12V ──→ DC-DC Converter ──→ 5V ──→ ESP32 VIN ──→ 3V3 ──→ TFT
                     (Buck)
```
- **Pros**: Permanent installation
- **Cons**: Requires additional components

### Option 3: USB Power Bank
```
Power Bank ──→ USB Cable ──→ ESP32 ──→ TFT Display
```
- **Pros**: Portable, rechargeable
- **Cons**: Requires periodic charging

## 🛠️ Assembly Instructions

### Step 1: Prepare Components
1. **ESP32 DevKit v1**: Verify all pins are accessible
2. **ILI9341 Display**: Check for bent pins
3. **Push Button**: Test mechanical operation
4. **Jumper Wires**: Use high-quality wires (20-24 AWG)

### Step 2: Display Connections
1. **Power First**: Connect 3V3 and GND
2. **SPI Data**: Connect MOSI, MISO, SCK
3. **Control Signals**: Connect CS, DC, RST
4. **Double-check**: Verify all connections with multimeter

### Step 3: Button Connection
1. **GPIO 21**: Connect to one side of button
2. **GND**: Connect to other side of button
3. **Test**: Button should read LOW when pressed

### Step 4: Power Supply
1. **Development**: Use USB cable
2. **Vehicle**: Install DC-DC converter
3. **Portable**: Use power bank

## ⚡ Power Requirements

### Current Consumption
| Component | Typical Current | Peak Current |
|-----------|----------------|--------------|
| ESP32 (WiFi/BT active) | 160-260mA | 400mA |
| ILI9341 Display | 100-150mA | 200mA |
| Total System | 260-410mA | 600mA |

### Power Supply Specifications
- **Voltage**: 3.3V for display, 5V for ESP32 VIN
- **Current**: Minimum 1A recommended
- **Regulation**: ±5% voltage tolerance
- **Ripple**: <50mV peak-to-peak

## 🔧 Vehicle Installation

### OBD2 Port Location
```
Standard OBD2 Port (16-pin)
┌─────────────────┐
│ 1  2  3  4  5  6│
│                 │
│ 9 10 11 12 13 14│
└─────────────────┘
  16 15 14 13 12 11

Pin 16: +12V Battery Power
Pin 4:  Chassis Ground
Pin 5:  Signal Ground
```

### Power Connection Options

#### Option A: OBD2 Port Power
```
OBD2 Pin 16 (+12V) ──→ Fuse ──→ DC-DC Converter ──→ ESP32
OBD2 Pin 5 (GND)   ──→ Common Ground
```

#### Option B: Cigarette Lighter
```
12V Socket ──→ USB Adapter ──→ ESP32
```

#### Option C: Direct Battery
```
Battery +12V ──→ Fuse ──→ Switch ──→ DC-DC ──→ ESP32
Battery GND  ──→ Common Ground
```

### Mounting Considerations
- **Location**: Dashboard, A-pillar, or center console
- **Visibility**: Driver should see display without distraction
- **Access**: Button should be easily reachable
- **Heat**: Avoid direct sunlight and heat sources
- **Vibration**: Secure mounting to prevent damage

## 🛡️ Protection Circuits

### Fuse Protection
```
+12V ──→ [5A Fuse] ──→ DC-DC Converter ──→ ESP32
```

### Reverse Polarity Protection
```
+12V ──→ [Diode] ──→ DC-DC Converter
```

### Voltage Spike Protection
```
+12V ──→ [TVS Diode] ──→ DC-DC Converter
              │
             GND
```

## 🔍 Testing Procedures

### Continuity Testing
1. **Power Off**: Ensure no power applied
2. **Multimeter**: Set to continuity mode
3. **Check Each Wire**: Verify end-to-end connection
4. **No Shorts**: Verify no unintended connections

### Voltage Testing
1. **Power On**: Apply power to system
2. **Measure 3V3**: Should be 3.2-3.4V
3. **Measure 5V**: Should be 4.8-5.2V
4. **Check Ripple**: Use oscilloscope if available

### Signal Testing
1. **SPI Signals**: Check with logic analyzer
2. **Button Signal**: Verify LOW when pressed
3. **Display Response**: Confirm display initialization

## ⚠️ Safety Warnings

### Electrical Safety
- **Never work on live circuits**
- **Use proper fuses for vehicle installation**
- **Ensure proper grounding**
- **Check polarity before connecting power**

### Vehicle Safety
- **Do not obstruct driver vision**
- **Secure all wiring to prevent interference**
- **Use automotive-grade components for permanent installation**
- **Test thoroughly before final installation**

### Component Safety
- **ESD Protection**: Use anti-static precautions
- **Voltage Limits**: Never exceed component ratings
- **Current Limits**: Ensure adequate power supply capacity
- **Temperature**: Consider operating temperature ranges

## 🔧 Tools Required

### Basic Tools
- Soldering iron (if permanent connections)
- Wire strippers
- Multimeter
- Screwdrivers
- Heat shrink tubing

### Advanced Tools
- Logic analyzer (for debugging)
- Oscilloscope (for signal analysis)
- Crimping tool (for automotive connectors)
- 3D printer (for custom enclosures)

## 📐 Mechanical Considerations

### Enclosure Design
- **Material**: ABS plastic for automotive use
- **Size**: Accommodate ESP32 + display + ventilation
- **Mounting**: Standard automotive mounting points
- **Access**: Openings for USB, button, display

### Cable Management
- **Length**: Minimize wire length to reduce noise
- **Routing**: Avoid high-current or high-frequency circuits
- **Strain Relief**: Prevent wire fatigue at connections
- **Labeling**: Mark wires for future maintenance
