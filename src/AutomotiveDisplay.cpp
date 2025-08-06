/**
 * @file AutomotiveDisplay.cpp
 * @brief Implementation of main application class for ESP32 automotive display
 * @author Andreas Auer
 */

#include "AutomotiveDisplay.h"
#include "config.h"

AutomotiveDisplay::AutomotiveDisplay()
    : currentState(AppState::INITIALIZING), lastUpdate(0), lastStateCheck(0),
      testTemperature(0), testPressure(0), testVoltage(0),
      testTempIncreasing(true), testPressureIncreasing(true), testVoltageIncreasing(true),
      lastTempUpdate(0), lastPressureUpdate(0), lastVoltageUpdate(0), lastStatusUpdate(0),
      diagnosticModeActive(false), buttonPressTime(0), lastButtonCheck(0),
      lastButtonState(false), diagnosticStartTime(0), currentDiagnosticIndex(0)
{
#ifdef DISPLAY_TEST
    testTemperature = TEST_TEMP_MIN;
    testPressure = TEST_PRESSURE_MIN;
    testVoltage = TEST_VOLTAGE_MIN;
#endif
}

bool AutomotiveDisplay::begin() {
    DEBUG_PRINTLN("Initializing AutomotiveDisplay...");
    
    // Initialize Serial for debugging
    Serial.begin(DEBUG_SERIAL_BAUD);
    delay(1000);
    
    DEBUG_PRINTLN(PROJECT_NAME " " PROJECT_VERSION);
    DEBUG_PRINTLN("Build: " BUILD_DATE " " BUILD_TIME);
    
    // Initialize display first
    if (!gaugeDisplay.begin()) {
        DEBUG_PRINTLN("Failed to initialize display");
        currentState = AppState::ERROR;
        return false;
    }

    // Initialize diagnostic button
    pinMode(DIAGNOSTIC_BUTTON_PIN, INPUT_PULLUP);
    DEBUG_PRINTLN("Diagnostic button initialized on pin " + String(DIAGNOSTIC_BUTTON_PIN));

#ifdef DISPLAY_TEST
    // In display test mode, skip OBD2 initialization
    DEBUG_PRINTLN("Display test mode - skipping OBD2 initialization");
    gaugeDisplay.showStatus(TEST_CONNECTION_MSG);
    currentState = AppState::RUNNING;

    // Initialize test timing variables
    unsigned long currentTime = millis();
    lastTempUpdate = currentTime;
    lastPressureUpdate = currentTime;
    lastVoltageUpdate = currentTime;
    lastStatusUpdate = currentTime;
#else
    // Initialize OBD2 manager
    if (!obd2Manager.begin()) {
        DEBUG_PRINTLN("Failed to initialize OBD2 manager");
        currentState = AppState::ERROR;
        return false;
    }

    // Show initial status
    gaugeDisplay.showStatus("System Ready");
    currentState = AppState::WAITING_BT;
#endif

    lastUpdate = millis();
    lastStateCheck = millis();
    
    DEBUG_PRINTLN("AutomotiveDisplay initialization complete");
    return true;
}

void AutomotiveDisplay::update() {
    unsigned long currentTime = millis();

    // Handle diagnostic button press (always check, regardless of mode)
    handleDiagnosticButton();

#ifdef DISPLAY_TEST
    // In display test mode, run test simulation
    if (!diagnosticModeActive) {
        updateDisplayTest();
    }
#else
    // Update OBD2 manager state
    obd2Manager.update();

    // Handle state transitions
    if (currentTime - lastStateCheck >= STATE_CHECK_INTERVAL) {
        handleStateTransitions();
        lastStateCheck = currentTime;
    }

    // Update sensor data and display
    if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
        switch (currentState) {
            case AppState::RUNNING:
                if (!diagnosticModeActive) {
                    updateSensorData();
                    updateDisplay();
                }
                break;

            case AppState::DIAGNOSTIC_MODE:
                updateDiagnosticMode();
                break;

            case AppState::WAITING_BT:
                gaugeDisplay.showStatus("Waiting for Bluetooth...");
                break;

            case AppState::CONNECTING_OBD2:
                gaugeDisplay.showStatus("Connecting to OBD2...");
                break;

            case AppState::ERROR:
                handleErrors();
                break;
                
            default:
                break;
        }

        lastUpdate = currentTime;
    }

    // Log debug info periodically
    if (DEBUG_ENABLED && (currentTime % HEARTBEAT_INTERVAL == 0)) {
        logDebugInfo();
    }
#endif
}

String AutomotiveDisplay::getStatusString() const {
    switch (currentState) {
        case AppState::INITIALIZING: return "Initializing";
        case AppState::WAITING_BT: return "Waiting for Bluetooth";
        case AppState::CONNECTING_OBD2: return "Connecting to OBD2";
        case AppState::RUNNING: return "Running";
        case AppState::ERROR: return "Error";
        default: return "Unknown";
    }
}

void AutomotiveDisplay::handleStateTransitions() {
    switch (currentState) {
        case AppState::WAITING_BT:
            if (obd2Manager.isConnected()) {
                currentState = AppState::CONNECTING_OBD2;
                DEBUG_PRINTLN("State: WAITING_BT -> CONNECTING_OBD2");
            }
            break;
            
        case AppState::CONNECTING_OBD2:
            if (!obd2Manager.isConnected()) {
                currentState = AppState::WAITING_BT;
                DEBUG_PRINTLN("State: CONNECTING_OBD2 -> WAITING_BT");
            } else if (obd2Manager.isReady()) {
                currentState = AppState::RUNNING;
                DEBUG_PRINTLN("State: CONNECTING_OBD2 -> RUNNING");
                gaugeDisplay.showStatus("OBD2 Connected");
            }
            break;
            
        case AppState::RUNNING:
            if (!obd2Manager.isConnected()) {
                currentState = AppState::WAITING_BT;
                DEBUG_PRINTLN("State: RUNNING -> WAITING_BT");
                currentSensorData.invalidate();
            } else if (!obd2Manager.isReady()) {
                currentState = AppState::CONNECTING_OBD2;
                DEBUG_PRINTLN("State: RUNNING -> CONNECTING_OBD2");
            }
            break;
            
        case AppState::ERROR:
            // Try to recover after delay
            if (millis() - lastStateCheck > ERROR_RETRY_INTERVAL) {
                currentState = AppState::INITIALIZING;
                DEBUG_PRINTLN("State: ERROR -> INITIALIZING (retry)");
                begin(); // Reinitialize
            }
            break;
            
        default:
            break;
    }
}

void AutomotiveDisplay::updateSensorData() {
    if (obd2Manager.isReady()) {
        const SensorData newData = obd2Manager.requestSensorData();
        if (newData.isValid) {
            currentSensorData = newData;
            DEBUG_PRINTF("Sensor data updated: Temp=%.1f°C, Boost=%.2fbar\n", 
                        newData.coolantTemperature, newData.boostPressure);
        }
    }
}

void AutomotiveDisplay::updateDisplay() {
    if (currentSensorData.isValid) {
        gaugeDisplay.updateGauges(currentSensorData);
        
        // Check for critical values and update status
        String status = "Normal";
        if (currentSensorData.coolantTemperature > TEMP_CRITICAL_VALUE) {
            status = "HIGH TEMP WARNING!";
        } else if (currentSensorData.boostPressure > BOOST_CRITICAL_VALUE) {
            status = "HIGH BOOST WARNING!";
        }
        #if BATTERY_VOLTAGE_ENABLED
        else if (currentSensorData.batteryVoltageValid) {
            if (currentSensorData.batteryVoltage < VOLTAGE_LOW_THRESHOLD) {
                status = "LOW BATTERY WARNING!";
            } else if (currentSensorData.batteryVoltage > VOLTAGE_HIGH_THRESHOLD) {
                status = "HIGH VOLTAGE WARNING!";
            }
        }
        #endif

        if (status != "Normal") {
            gaugeDisplay.showStatus(status);
        }
    } else {
        gaugeDisplay.showStatus("No sensor data");
    }
}

void AutomotiveDisplay::handleErrors() {
    gaugeDisplay.showStatus("System Error - Retrying...");
    DEBUG_PRINTLN("In error state, attempting recovery");
}

void AutomotiveDisplay::logDebugInfo() {
    #if BATTERY_VOLTAGE_ENABLED
    if (currentSensorData.batteryVoltageValid) {
        DEBUG_PRINTF("State: %s, OBD2: %s, Temp: %.1f°C, Boost: %.2fbar, Battery: %.1fV\n",
                    getStatusString().c_str(),
                    obd2Manager.getStatusString().c_str(),
                    currentSensorData.coolantTemperature,
                    currentSensorData.boostPressure,
                    currentSensorData.batteryVoltage);
    } else {
        DEBUG_PRINTF("State: %s, OBD2: %s, Temp: %.1f°C, Boost: %.2fbar, Battery: N/A\n",
                    getStatusString().c_str(),
                    obd2Manager.getStatusString().c_str(),
                    currentSensorData.coolantTemperature,
                    currentSensorData.boostPressure);
    }
    #else
    DEBUG_PRINTF("State: %s, OBD2: %s, Temp: %.1f°C, Boost: %.2fbar\n",
                getStatusString().c_str(),
                obd2Manager.getStatusString().c_str(),
                currentSensorData.coolantTemperature,
                currentSensorData.boostPressure);
    #endif
}

#ifdef DISPLAY_TEST
void AutomotiveDisplay::updateDisplayTest() {
    unsigned long currentTime = millis();

    // Update test data generation
    generateTestData();

    // Update display with test data
    updateDisplay();

    // Update status message periodically
    if (currentTime - lastStatusUpdate >= TEST_STATUS_INTERVAL) {
        String statusMsg = TEST_CONNECTION_MSG;
        statusMsg += " - T:" + String(testTemperature, 1) + "°C";
        statusMsg += " P:" + String(testPressure, 1) + "bar";
        statusMsg += " V:" + String(testVoltage, 1) + "V";
        gaugeDisplay.showStatus(statusMsg);
        lastStatusUpdate = currentTime;

        #if TEST_ENABLE_DEBUG
        DEBUG_PRINTF("Display Test - Temp: %.1f°C, Pressure: %.1fbar, Voltage: %.1fV\n",
                    testTemperature, testPressure, testVoltage);
        #endif
    }
}

void AutomotiveDisplay::generateTestData() {
    unsigned long currentTime = millis();

    // Update temperature
    if (currentTime - lastTempUpdate >= TEST_TEMP_INTERVAL) {
        if (testTempIncreasing) {
            testTemperature += TEST_TEMP_STEP;
            if (testTemperature >= TEST_TEMP_MAX) {
                testTemperature = TEST_TEMP_MIN;  // Reset to minimum
            }
        }

        // Update sensor data with test temperature
        currentSensorData.coolantTemperature = testTemperature;
        currentSensorData.isValid = true;
        currentSensorData.timestamp = currentTime;

        lastTempUpdate = currentTime;
    }

    // Update pressure
    if (currentTime - lastPressureUpdate >= TEST_PRESSURE_INTERVAL) {
        if (testPressureIncreasing) {
            testPressure += TEST_PRESSURE_STEP;
            if (testPressure >= TEST_PRESSURE_MAX) {
                testPressure = TEST_PRESSURE_MIN;  // Reset to minimum
                testPressureIncreasing = true;
            }
        }

        // Update sensor data with test pressure
        currentSensorData.boostPressure = testPressure;

        lastPressureUpdate = currentTime;
    }

    // Update voltage
    if (currentTime - lastVoltageUpdate >= TEST_VOLTAGE_INTERVAL) {
        if (testVoltageIncreasing) {
            testVoltage += TEST_VOLTAGE_STEP;
            if (testVoltage >= TEST_VOLTAGE_MAX) {
                testVoltage = TEST_VOLTAGE_MIN;  // Reset to minimum
            }
        }

        // Update sensor data with test voltage
        #if BATTERY_VOLTAGE_ENABLED
        currentSensorData.updateBatteryVoltage(testVoltage);
        #endif

        lastVoltageUpdate = currentTime;
    }
}
#endif

void AutomotiveDisplay::handleDiagnosticButton() {
    unsigned long currentTime = millis();

    // Check button state with debouncing
    if (currentTime - lastButtonCheck >= BUTTON_DEBOUNCE_MS) {
        bool currentButtonState = isButtonPressed();

        // Button press detected (falling edge)
        if (currentButtonState && !lastButtonState) {
            buttonPressTime = currentTime;
            DEBUG_PRINTLN("Diagnostic button pressed");
        }

        // Button release detected (rising edge)
        if (!currentButtonState && lastButtonState) {
            unsigned long pressDuration = currentTime - buttonPressTime;

            if (diagnosticModeActive) {
                // In diagnostic mode: short press = next PID, long press = exit
                if (pressDuration >= BUTTON_LONG_PRESS_MS) {
                    DEBUG_PRINTLN("Long press detected - exiting diagnostic mode");
                    exitDiagnosticMode();
                } else {
                    DEBUG_PRINTLN("Short press detected - next diagnostic item");
                    currentDiagnosticIndex++;
                }
            } else {
                // Not in diagnostic mode: any press enters diagnostic mode
                if (pressDuration >= BUTTON_DEBOUNCE_MS) {
                    DEBUG_PRINTLN("Entering diagnostic mode");
                    enterDiagnosticMode();
                }
            }
        }

        lastButtonState = currentButtonState;
        lastButtonCheck = currentTime;
    }
}

bool AutomotiveDisplay::isButtonPressed() {
    return digitalRead(DIAGNOSTIC_BUTTON_PIN) == LOW; // Assuming active low with pullup
}

void AutomotiveDisplay::enterDiagnosticMode() {
    if (currentState == AppState::RUNNING || currentState == AppState::WAITING_BT) {
        diagnosticModeActive = true;
        currentState = AppState::DIAGNOSTIC_MODE;
        diagnosticStartTime = millis();
        currentDiagnosticIndex = 0;

        gaugeDisplay.clear();
        gaugeDisplay.showDiagnosticHeader();

        DEBUG_PRINTLN("Diagnostic mode activated");
    }
}

void AutomotiveDisplay::exitDiagnosticMode() {
    diagnosticModeActive = false;
    currentDiagnosticIndex = 0;

    // Return to previous state
    if (obd2Manager.isReady()) {
        currentState = AppState::RUNNING;
    } else if (obd2Manager.isConnected()) {
        currentState = AppState::CONNECTING_OBD2;
    } else {
        currentState = AppState::WAITING_BT;
    }

    gaugeDisplay.clear();
    DEBUG_PRINTLN("Diagnostic mode deactivated");
}

void AutomotiveDisplay::updateDiagnosticMode() {
    if (!diagnosticModeActive || !obd2Manager.isReady()) {
        gaugeDisplay.showStatus("OBD2 not ready for diagnostics");
        return;
    }

    // Get all diagnostic data
    std::vector<String> diagnosticData = obd2Manager.getAllDiagnosticData();

    if (diagnosticData.empty()) {
        gaugeDisplay.showStatus("No diagnostic data available");
        return;
    }

    // Wrap around if index exceeds available data
    if (currentDiagnosticIndex >= diagnosticData.size()) {
        currentDiagnosticIndex = 0;
    }

    // Display current diagnostic data
    gaugeDisplay.showDiagnosticData(diagnosticData, currentDiagnosticIndex);

    // Auto-advance every few seconds if no button press
    unsigned long currentTime = millis();
    if (currentTime - diagnosticStartTime >= DIAGNOSTIC_SCROLL_INTERVAL) {
        currentDiagnosticIndex++;
        diagnosticStartTime = currentTime;
    }
}
