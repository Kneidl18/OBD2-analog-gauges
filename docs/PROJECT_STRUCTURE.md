# Project Structure

## 📁 Directory Layout

```
ESP32-OBD2-Analog-Gauges/
├── 📄 README.md                           # Main project documentation
├── 📄 LICENSE                             # MIT License
├── 📄 CONTRIBUTING.md                     # Contribution guidelines
├── 📄 .gitignore                          # Git ignore rules
├── 📄 platformio.ini                      # PlatformIO configuration
│
├── 📁 src/                                # Source code
│   ├── 📄 main.cpp                        # Main application entry point
│   ├── 📄 AutomotiveDisplay.cpp           # Main application controller
│   ├── 📄 OBD2Manager.cpp                 # OBD2 communication handler
│   └── 📄 GaugeDisplay.cpp                # TFT display and rendering
│
├── 📁 include/                            # Header files
│   ├── 📄 config.h                        # System configuration
│   ├── 📄 AutomotiveDisplay.h             # Main controller interface
│   ├── 📄 OBD2Manager.h                   # OBD2 communication interface
│   ├── 📄 GaugeDisplay.h                  # Display interface
│   ├── 📄 SensorData.h                    # Data structures
│   └── 📄 README                          # Include directory info
│
├── 📁 lib/                                # Custom libraries (empty)
│   └── 📄 README                          # Library directory info
│
├── 📁 test/                               # Test files (empty)
│   └── 📄 README                          # Test directory info
│
├── 📁 docs/                               # Documentation
│   ├── 📄 API.md                          # API documentation
│   ├── 📄 BILL_OF_MATERIALS.md            # Component list and costs
│   ├── 📄 PROJECT_STRUCTURE.md            # This file
│   ├── 📄 TROUBLESHOOTING.md              # Common issues and solutions
│   ├── 📄 WIRING_GUIDE.md                 # Hardware connection guide
│   └── 📁 images/                         # Documentation images
│       └── 📄 demo.jpg                    # Project demonstration photo
│
├── 📁 .github/                            # GitHub configuration
│   └── 📁 workflows/                      # CI/CD workflows
│       └── 📄 platformio.yml              # PlatformIO build automation
│
├── 📄 README_DIAGNOSTIC_SCANNER.md        # Detailed diagnostic feature docs
└── 📄 README_HARDWARE_DEPLOYMENT.md       # Hardware installation guide
```

## 📋 File Descriptions

### Core Application Files

#### `src/main.cpp`
- **Purpose**: Application entry point and main loop
- **Key Functions**: `setup()`, `loop()`
- **Dependencies**: AutomotiveDisplay class
- **Size**: ~50 lines

#### `src/AutomotiveDisplay.cpp`
- **Purpose**: Main application controller and state machine
- **Key Functions**: System initialization, state management, button handling
- **Dependencies**: OBD2Manager, GaugeDisplay, SensorData
- **Size**: ~450 lines

#### `src/OBD2Manager.cpp`
- **Purpose**: OBD2 communication and data parsing
- **Key Functions**: Bluetooth connection, PID discovery, data parsing
- **Dependencies**: BluetoothSerial library
- **Size**: ~540 lines

#### `src/GaugeDisplay.cpp`
- **Purpose**: TFT display management and rendering
- **Key Functions**: Gauge drawing, diagnostic display, status messages
- **Dependencies**: TFT_eSPI library
- **Size**: ~345 lines

### Header Files

#### `include/config.h`
- **Purpose**: System-wide configuration constants
- **Contents**: Pin definitions, thresholds, timing parameters
- **Customization**: Main file for user configuration
- **Size**: ~360 lines

#### `include/AutomotiveDisplay.h`
- **Purpose**: Main controller class interface
- **Contents**: Class definition, state enum, method declarations
- **Size**: ~155 lines

#### `include/OBD2Manager.h`
- **Purpose**: OBD2 communication interface
- **Contents**: Class definition, PID constants, method declarations
- **Size**: ~200 lines

#### `include/GaugeDisplay.h`
- **Purpose**: Display management interface
- **Contents**: Class definition, gauge configuration, display methods
- **Size**: ~160 lines

#### `include/SensorData.h`
- **Purpose**: Data structure definitions
- **Contents**: Sensor data structure, validation methods
- **Size**: ~80 lines

### Configuration Files

#### `platformio.ini`
- **Purpose**: PlatformIO project configuration
- **Contents**: Board settings, library dependencies, build flags
- **Key Settings**: ESP32 board, TFT_eSPI configuration

#### `.gitignore`
- **Purpose**: Git version control exclusions
- **Contents**: Build artifacts, IDE files, temporary files

### Documentation Files

#### `README.md`
- **Purpose**: Main project documentation
- **Contents**: Features, installation, usage, troubleshooting
- **Audience**: End users and developers

#### `docs/API.md`
- **Purpose**: Code reference documentation
- **Contents**: Class methods, configuration options, examples
- **Audience**: Developers

#### `docs/BILL_OF_MATERIALS.md`
- **Purpose**: Hardware component specifications
- **Contents**: Component list, costs, supplier information
- **Audience**: Hardware builders

#### `docs/WIRING_GUIDE.md`
- **Purpose**: Hardware connection instructions
- **Contents**: Pin connections, power supply, installation
- **Audience**: Hardware builders

#### `docs/TROUBLESHOOTING.md`
- **Purpose**: Problem resolution guide
- **Contents**: Common issues, solutions, debug procedures
- **Audience**: End users

### Special Documentation

#### `README_DIAGNOSTIC_SCANNER.md`
- **Purpose**: Detailed diagnostic feature documentation
- **Contents**: Button controls, PID support, interface guide
- **Audience**: Advanced users

#### `README_HARDWARE_DEPLOYMENT.md`
- **Purpose**: Vehicle installation guide
- **Contents**: Mounting, power connection, safety considerations
- **Audience**: Installers

## 🔧 Build System

### PlatformIO Configuration
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
    bodmer/TFT_eSPI@^2.5.43
build_flags = 
    -DUSER_SETUP_LOADED=1
    -DILI9341_DRIVER=1
    # ... TFT configuration flags
```

### Library Dependencies
- **TFT_eSPI**: Display driver library
- **BluetoothSerial**: ESP32 Bluetooth communication
- **Arduino Framework**: Core ESP32 functionality

### Build Targets
- **Default**: Standard firmware build
- **Upload**: Build and upload to ESP32
- **Monitor**: Serial monitor for debugging
- **Size**: Memory usage analysis

## 📊 Code Metrics

### Lines of Code
| Component | Lines | Percentage |
|-----------|-------|------------|
| AutomotiveDisplay | 450 | 32% |
| OBD2Manager | 540 | 38% |
| GaugeDisplay | 345 | 24% |
| Main/Config | 85 | 6% |
| **Total** | **1420** | **100%** |

### Memory Usage
| Type | Usage | Available | Percentage |
|------|-------|-----------|------------|
| Flash | 1.1MB | 4MB | 28% |
| RAM | 45KB | 320KB | 14% |

### File Sizes
| File | Size | Purpose |
|------|------|---------|
| config.h | 12KB | Configuration |
| AutomotiveDisplay.cpp | 18KB | Main logic |
| OBD2Manager.cpp | 22KB | Communication |
| GaugeDisplay.cpp | 14KB | Display |

## 🎯 Design Patterns

### Architecture
- **Modular Design**: Separate classes for distinct functionality
- **State Machine**: Clear application state management
- **Configuration-Driven**: Extensive use of compile-time configuration
- **Error Handling**: Robust error detection and recovery

### Code Organization
- **Single Responsibility**: Each class has a clear, focused purpose
- **Loose Coupling**: Minimal dependencies between components
- **High Cohesion**: Related functionality grouped together
- **Consistent Naming**: Clear, descriptive names throughout

### Documentation Standards
- **Header Comments**: File purpose and author information
- **Function Documentation**: Parameter and return value descriptions
- **Inline Comments**: Complex algorithm explanations
- **Configuration Comments**: Parameter usage and valid ranges

## 🚀 Development Workflow

### Adding New Features
1. **Update config.h**: Add configuration parameters
2. **Modify headers**: Update class interfaces
3. **Implement logic**: Add functionality to source files
4. **Test thoroughly**: Verify on hardware
5. **Update documentation**: Reflect changes in docs

### Code Style Guidelines
- **Indentation**: 4 spaces, no tabs
- **Braces**: Opening brace on same line
- **Naming**: camelCase for variables, PascalCase for classes
- **Comments**: Doxygen-style for public interfaces
- **Constants**: ALL_CAPS with underscores

### Version Control
- **Main Branch**: Stable, tested code
- **Feature Branches**: New development
- **Pull Requests**: Code review process
- **Semantic Versioning**: Major.Minor.Patch format
