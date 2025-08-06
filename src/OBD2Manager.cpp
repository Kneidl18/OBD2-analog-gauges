/**
 * @file OBD2Manager.cpp
 * @brief Implementation of OBD2 communication manager
 * @author Andreas Auer
 */

#include "OBD2Manager.h"

#include <utility>
#include "config.h"

OBD2Manager::OBD2Manager(String  deviceName)
    : deviceName(std::move(deviceName)), currentState(OBD2State::DISCONNECTED),
      lastInitCommand(0), initStep(0), lastBatteryUpdate(0) {
}

bool OBD2Manager::begin() {
    Serial.println("Initializing OBD2Manager...");

    if (!serialBT.begin(deviceName)) {
        Serial.println("Failed to initialize Bluetooth");
        currentState = OBD2State::ERROR;
        return false;
    }

    Serial.println("Bluetooth initialized. Device name: " + deviceName);
    currentState = OBD2State::DISCONNECTED;
    return true;
}

void OBD2Manager::update() {
    switch (currentState) {
        case OBD2State::DISCONNECTED:
            if (isConnected()) {
                Serial.println("Bluetooth client connected");
                currentState = OBD2State::CONNECTED;
                initStep = 0;
                lastInitCommand = 0;
            }
            break;

        case OBD2State::CONNECTED:
            if (!isConnected()) {
                Serial.println("Bluetooth client disconnected");
                currentState = OBD2State::DISCONNECTED;
            } else {
                currentState = OBD2State::INITIALIZING;
                initializeOBD2();
            }
            break;

        case OBD2State::INITIALIZING:
            if (!isConnected()) {
                currentState = OBD2State::DISCONNECTED;
            }
            // Initialization is handled in initializeOBD2()
            break;

        case OBD2State::READY:
            if (!isConnected()) {
                Serial.println("Connection lost during operation");
                currentState = OBD2State::DISCONNECTED;
            }
            break;

        case OBD2State::ERROR:
            // Try to recover after delay
            if (millis() - lastInitCommand > 5000) {
                Serial.println("Attempting to recover from error state");
                currentState = OBD2State::DISCONNECTED;
            }
            break;
    }
}

SensorData OBD2Manager::requestSensorData() {
    if (currentState != OBD2State::READY) {
        return lastSensorData; // Return last valid data
    }

    SensorData newData;
    bool dataValid = false;

    // Request coolant temperature (PID 0105)
    sendCommand("0105");
    delay(100);
    const String tempResponse = readResponse(2000);
    if (isValidResponse(tempResponse)) {
        const float temp = parseCoolantTemp(tempResponse);
        if (temp != -999) {
            newData.coolantTemperature = temp;
            dataValid = true;
        }
    }

    delay(200);

    // Request intake manifold pressure (PID 010B)
    sendCommand("010B");
    delay(100);
    const String pressureResponse = readResponse(2000);
    if (isValidResponse(pressureResponse)) {
        const float pressure = parseBoostPressure(pressureResponse);
        if (pressure != -999) {
            newData.boostPressure = pressure;
            dataValid = true;
        }
    }

    // Request battery voltage if enabled and enough time has passed
    #if BATTERY_VOLTAGE_ENABLED
    float batteryVoltage = requestBatteryVoltage();
    if (batteryVoltage > 0.0f) {
        newData.updateBatteryVoltage(batteryVoltage);
    } else {
        // Copy previous battery voltage data if current request failed
        newData.batteryVoltage = lastSensorData.batteryVoltage;
        newData.batteryVoltageValid = lastSensorData.batteryVoltageValid;
        newData.batteryTimestamp = lastSensorData.batteryTimestamp;
    }
    #endif

    if (dataValid) {
        newData.isValid = true;
        newData.timestamp = millis();
        lastSensorData = newData;

        #if DEBUG_ENABLED && DEBUG_SENSOR_DATA
        String logMessage = "Sensor data updated - Temp: " + String(newData.coolantTemperature) +
                           "°C, Pressure: " + String(newData.boostPressure) + " kPa";
        #if BATTERY_VOLTAGE_ENABLED
        if (newData.batteryVoltageValid) {
            logMessage += ", Battery: " + String(newData.batteryVoltage, 1) + "V";
        }
        #endif
        Serial.println(logMessage);
        #endif
    }

    return newData;
}

bool OBD2Manager::isConnected() {
    return serialBT.hasClient();
}

String OBD2Manager::getStatusString() const {
    switch (currentState) {
        case OBD2State::DISCONNECTED: return "Disconnected";
        case OBD2State::CONNECTED: return "Connected";
        case OBD2State::INITIALIZING: return "Initializing";
        case OBD2State::READY: return "Ready";
        case OBD2State::ERROR: return "Error";
        default: return "Unknown";
    }
}

void OBD2Manager::initializeOBD2() {
    if (millis() - lastInitCommand < INIT_COMMAND_DELAY) {
        return; // Wait for delay between commands
    }

    if (initStep < 6) {
        const String descriptions[] = {"Reset", "Echo off", "Linefeeds off", "Spaces off", "Headers off", "Auto protocol"};
        const String commands[] = {"ATZ", "ATE0", "ATL0", "ATS0", "ATH0", "ATSP0"};
        Serial.println("Init step " + String(initStep) + ": " + descriptions[initStep]);
        sendCommand(commands[initStep]);
        lastInitCommand = millis();
        initStep++;
    } else {
        Serial.println("OBD2 initialization complete");
        currentState = OBD2State::READY;
    }
}

float OBD2Manager::parseCoolantTemp(const String& response) {
    // Expected response format: "4105XX" where XX is the temperature value
    if (response.length() >= 6 && response.substring(0, 4) == "4105") {
        const String hexValue = response.substring(4, 6);
        const int tempValue = strtol(hexValue.c_str(), NULL, 16);
        const float temperature = tempValue - 40; // Convert to Celsius

        // Validate reasonable temperature range
        if (temperature >= -40 && temperature <= 150) {
            return temperature;
        }
    }
    return -999; // Invalid data
}

float OBD2Manager::parseBoostPressure(const String& response) {
    // Expected response format: "410BXX" where XX is the pressure value
    if (response.length() >= 6 && response.substring(0, 4) == "410B") {
        const String hexValue = response.substring(4, 6);
        const int pressureValue = strtol(hexValue.c_str(), nullptr, 16);
        const float pressureKpa = pressureValue; // Value is in kPa from OBD2
        const float pressureBar = KPA_TO_BAR(pressureKpa); // Convert to bar

        // Validate reasonable pressure range (in bar)
        if (pressureBar >= 0 && pressureBar <= 3.0) {
            return pressureBar;
        }
    }
    return -999; // Invalid data
}

bool OBD2Manager::isValidResponse(const String& response) {
    // Check for common error responses
    if (response.length() < 4) return false;
    if (response.indexOf("NODATA") >= 0) return false;
    if (response.indexOf("ERROR") >= 0) return false;
    if (response.indexOf("?") >= 0) return false;

    return true;
}

void OBD2Manager::sendCommand(const String& command) {
    if (serialBT.hasClient()) {
        serialBT.println(command);
        Serial.println("Sent: " + command);
    }
}

String OBD2Manager::readResponse(const unsigned long timeout) {
    String response = "";
    const unsigned long startTime = millis();

    while (millis() - startTime < timeout) {
        if (serialBT.available()) {
            const char c = serialBT.read();
            if (c == '\r' || c == '\n') {
                if (response.length() > 0) {
                    break;
                }
            } else if (c != ' ') { // Remove spaces
                response += c;
            }
        }
        delay(10);
    }

    Serial.println("Received: " + response);
    return response;
}

float OBD2Manager::requestBatteryVoltage() {
    if (currentState != OBD2State::READY) {
        #if DEBUG_ENABLED
        Serial.println("OBD2 not ready for battery voltage request");
        #endif
        return -1.0f;
    }

    // Check if enough time has passed since last battery update
    const unsigned long currentTime = millis();
    if (currentTime - lastBatteryUpdate < BATTERY_UPDATE_INTERVAL) {
        return -1.0f; // Too soon for another update
    }

    #if DEBUG_ENABLED && DEBUG_OBD2_COMMANDS
    Serial.println("Requesting battery voltage with AT RV command");
    #endif

    // Send ELM327 AT RV command to read voltage
    sendCommand("AT RV");
    String response = readResponse(RESPONSE_TIMEOUT);

    if (response.length() == 0) {
        #if DEBUG_ENABLED
        Serial.println("No response to AT RV command");
        #endif
        return -1.0f;
    }

    // Parse the voltage from response
    float voltage = parseBatteryVoltage(response);

    if (voltage > 0.0f) {
        lastBatteryUpdate = currentTime;
        #if DEBUG_ENABLED && DEBUG_SENSOR_DATA
        Serial.printf("Battery voltage: %.1fV\n", voltage);
        #endif
    } else {
        #if DEBUG_ENABLED
        Serial.println("Failed to parse battery voltage from response: " + response);
        #endif
    }

    return voltage;
}

float OBD2Manager::parseBatteryVoltage(const String& response) {
    // Expected response format: "13.2V" or "13.2V\r>" or similar
    // Remove any trailing characters and extract the voltage value

    String cleanResponse = response;
    cleanResponse.trim();

    // Find the 'V' character which indicates voltage
    int vIndex = cleanResponse.indexOf('V');
    if (vIndex == -1) {
        // Try alternative format without 'V'
        vIndex = cleanResponse.length();
    }

    // Extract the numeric part before 'V'
    String voltageStr = cleanResponse.substring(0, vIndex);
    voltageStr.trim();

    // Remove any non-numeric characters except decimal point
    String cleanVoltageStr = "";
    bool hasDecimal = false;
    for (int i = 0; i < voltageStr.length(); i++) {
        const char c = voltageStr.charAt(i);
        if (isDigit(c)) {
            cleanVoltageStr += c;
        } else if (c == '.' && !hasDecimal) {
            cleanVoltageStr += c;
            hasDecimal = true;
        }
    }

    if (cleanVoltageStr.length() == 0) {
        return -1.0f;
    }

    float voltage = cleanVoltageStr.toFloat();

    // Validate voltage range (reasonable automotive battery voltage)
    if (voltage < 8.0f || voltage > 18.0f) {
        #if DEBUG_ENABLED
        Serial.printf("Invalid voltage reading: %.1fV (out of range 8.0-18.0V)\n", voltage);
        #endif
        return -1.0f;
    }

    return voltage;
}

std::vector<String> OBD2Manager::discoverSupportedPIDs() {
    std::vector<String> supportedPIDs;

    if (currentState != OBD2State::READY) {
        #if DEBUG_ENABLED
        Serial.println("OBD2 not ready for PID discovery");
        #endif
        return supportedPIDs;
    }

    #if DEBUG_ENABLED
    Serial.println("Starting PID discovery...");
    #endif

    // Check PIDs 01-20
    sendCommand(PID_SUPPORTED_01_20);
    String response = readResponse(RESPONSE_TIMEOUT);
    if (isValidResponse(response)) {
        parseSupportedPIDs(response, 0x01, supportedPIDs);
    }

    delay(200);

    // Check PIDs 21-40
    sendCommand(PID_SUPPORTED_21_40);
    response = readResponse(RESPONSE_TIMEOUT);
    if (isValidResponse(response)) {
        parseSupportedPIDs(response, 0x21, supportedPIDs);
    }

    delay(200);

    // Check PIDs 41-60
    sendCommand(PID_SUPPORTED_41_60);
    response = readResponse(RESPONSE_TIMEOUT);
    if (isValidResponse(response)) {
        parseSupportedPIDs(response, 0x41, supportedPIDs);
    }

    #if DEBUG_ENABLED
    Serial.printf("PID discovery complete. Found %d supported PIDs\n", supportedPIDs.size());
    #endif

    return supportedPIDs;
}

String OBD2Manager::getDiagnosticData(const String& pid) {
    if (currentState != OBD2State::READY) {
        return "OBD2 Not Ready";
    }

    sendCommand(pid);
    delay(100);
    String response = readResponse(2000);

    if (!isValidResponse(response)) {
        return getPIDName(pid) + ": No Response";
    }

    // Parse the response based on PID type
    String value = parsePIDValue(pid, response);
    return getPIDName(pid) + ": " + value;
}

std::vector<String> OBD2Manager::getAllDiagnosticData() {
    std::vector<String> diagnosticData;

    // Common PIDs to check
    String commonPIDs[] = {
        PID_ENGINE_RPM, PID_VEHICLE_SPEED, PID_THROTTLE_POSITION,
        PID_ENGINE_LOAD, PID_FUEL_LEVEL, PID_INTAKE_TEMP,
        PID_MAF_FLOW_RATE, PID_TIMING_ADVANCE, PID_FUEL_PRESSURE,
        TEMP_PID, BOOST_PID, PID_SHORT_FUEL_TRIM_1, PID_LONG_FUEL_TRIM_1,
        PID_O2_SENSOR_1, PID_RUNTIME_SINCE_START
    };

    int numPIDs = sizeof(commonPIDs) / sizeof(commonPIDs[0]);

    for (int i = 0; i < numPIDs && i < DIAGNOSTIC_MAX_PIDS; i++) {
        String data = getDiagnosticData(commonPIDs[i]);
        diagnosticData.push_back(data);
        delay(150); // Small delay between requests
    }

    // Add battery voltage
    float voltage = requestBatteryVoltage();
    if (voltage > 0) {
        diagnosticData.push_back("Battery Voltage: " + String(voltage, 2) + "V");
    }

    return diagnosticData;
}

void OBD2Manager::parseSupportedPIDs(const String& response, int startPID, std::vector<String>& supportedPIDs) {
    // Parse the 4-byte response to determine which PIDs are supported
    // Each bit represents a PID (1 = supported, 0 = not supported)

    if (response.length() < 8) return; // Need at least 4 bytes (8 hex chars)

    // Extract the hex data (skip the first 4 characters which are the PID echo)
    String hexData = response.substring(4);
    hexData.replace(" ", ""); // Remove spaces

    if (hexData.length() < 8) return;

    // Convert hex string to 4 bytes
    for (int byteIndex = 0; byteIndex < 4; byteIndex++) {
        String byteStr = hexData.substring(byteIndex * 2, byteIndex * 2 + 2);
        int byteValue = strtol(byteStr.c_str(), NULL, 16);

        // Check each bit in the byte
        for (int bit = 7; bit >= 0; bit--) {
            if (byteValue & (1 << bit)) {
                int pidNumber = startPID + (byteIndex * 8) + (7 - bit);
                String pidStr = String(pidNumber, HEX);
                if (pidStr.length() == 1) pidStr = "0" + pidStr;
                pidStr.toUpperCase();
                pidStr = "01" + pidStr;
                supportedPIDs.push_back(pidStr);
            }
        }
    }
}

String OBD2Manager::getPIDName(const String& pid) {
    // Return human-readable names for common PIDs
    if (pid == PID_ENGINE_RPM) return "Engine RPM";
    if (pid == PID_VEHICLE_SPEED) return "Vehicle Speed";
    if (pid == PID_THROTTLE_POSITION) return "Throttle Position";
    if (pid == PID_ENGINE_LOAD) return "Engine Load";
    if (pid == PID_FUEL_LEVEL) return "Fuel Level";
    if (pid == PID_INTAKE_TEMP) return "Intake Air Temp";
    if (pid == PID_MAF_FLOW_RATE) return "MAF Flow Rate";
    if (pid == PID_TIMING_ADVANCE) return "Timing Advance";
    if (pid == PID_FUEL_PRESSURE) return "Fuel Pressure";
    if (pid == TEMP_PID) return "Coolant Temp";
    if (pid == BOOST_PID) return "Intake Pressure";
    if (pid == PID_SHORT_FUEL_TRIM_1) return "Short Fuel Trim 1";
    if (pid == PID_LONG_FUEL_TRIM_1) return "Long Fuel Trim 1";
    if (pid == PID_O2_SENSOR_1) return "O2 Sensor 1";
    if (pid == PID_RUNTIME_SINCE_START) return "Runtime";
    if (pid == PID_BAROMETRIC_PRESSURE) return "Barometric Press";

    return "PID " + pid; // Default name
}

String OBD2Manager::parsePIDValue(const String& pid, const String& response) {
    // Parse response based on PID type
    if (response.length() < 6) return "Invalid";

    // Extract data bytes (skip PID echo)
    String dataStr = response.substring(4);
    dataStr.replace(" ", "");

    if (pid == PID_ENGINE_RPM) {
        if (dataStr.length() >= 4) {
            int a = strtol(dataStr.substring(0, 2).c_str(), NULL, 16);
            int b = strtol(dataStr.substring(2, 4).c_str(), NULL, 16);
            int rpm = ((a * 256) + b) / 4;
            return String(rpm) + " RPM";
        }
    }
    else if (pid == PID_VEHICLE_SPEED) {
        if (dataStr.length() >= 2) {
            int speed = strtol(dataStr.substring(0, 2).c_str(), NULL, 16);
            return String(speed) + " km/h";
        }
    }
    else if (pid == PID_THROTTLE_POSITION) {
        if (dataStr.length() >= 2) {
            int throttle = strtol(dataStr.substring(0, 2).c_str(), NULL, 16);
            float percent = (throttle * 100.0) / 255.0;
            return String(percent, 1) + "%";
        }
    }
    else if (pid == PID_ENGINE_LOAD) {
        if (dataStr.length() >= 2) {
            int load = strtol(dataStr.substring(0, 2).c_str(), NULL, 16);
            float percent = (load * 100.0) / 255.0;
            return String(percent, 1) + "%";
        }
    }
    else if (pid == PID_FUEL_LEVEL) {
        if (dataStr.length() >= 2) {
            int fuel = strtol(dataStr.substring(0, 2).c_str(), NULL, 16);
            float percent = (fuel * 100.0) / 255.0;
            return String(percent, 1) + "%";
        }
    }
    else if (pid == TEMP_PID) {
        if (dataStr.length() >= 2) {
            int temp = strtol(dataStr.substring(0, 2).c_str(), NULL, 16);
            return String(temp - 40) + "°C";
        }
    }
    else if (pid == BOOST_PID) {
        if (dataStr.length() >= 2) {
            int pressure = strtol(dataStr.substring(0, 2).c_str(), NULL, 16);
            return String(pressure) + " kPa";
        }
    }

    // Default: show raw hex data
    return dataStr;
}
