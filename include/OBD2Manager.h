/**
 * @file OBD2Manager.h
 * @brief OBD2 communication manager for ESP32 Bluetooth connectivity
 * @author Andreas Auer
 */

#ifndef OBD2_MANAGER_H
#define OBD2_MANAGER_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <vector>
#include "SensorData.h"

/**
 * @brief OBD2 connection states
 */
enum class OBD2State {
    DISCONNECTED,    ///< No Bluetooth connection
    CONNECTED,       ///< Bluetooth connected but OBD2 not initialized
    INITIALIZING,    ///< Sending initialization commands
    READY,           ///< Ready to receive sensor data
    ERROR            ///< Error state
};

/**
 * @brief Manages OBD2 communication via Bluetooth
 */
class OBD2Manager {
public:
    /**
     * @brief Constructor
     * @param deviceName Bluetooth device name for pairing
     */
    explicit OBD2Manager(String  deviceName = "ESP32_OBD2_Display");

    /**
     * @brief Destructor
     */
    ~OBD2Manager() = default;

    /**
     * @brief Initialize Bluetooth and start advertising
     * @return True if initialization successful
     */
    bool begin();

    /**
     * @brief Update connection status and handle initialization
     * Should be called regularly in main loop
     */
    void update();

    /**
     * @brief Request sensor data from OBD2 interface
     * @return SensorData structure with current readings
     */
    SensorData requestSensorData();

    /**
     * @brief Request battery voltage using ELM327 AT RV command
     * @return Battery voltage in volts, or -1.0f on failure
     */
    float requestBatteryVoltage();

    /**
     * @brief Get current connection state
     * @return Current OBD2State
     */
    OBD2State getState() const { return currentState; }

    /**
     * @brief Check if OBD2 is ready for data requests
     * @return True if ready
     */
    bool isReady() const { return currentState == OBD2State::READY; }

    /**
     * @brief Check if Bluetooth client is connected
     * @return True if connected
     */
    bool isConnected();

    /**
     * @brief Get status string for display
     * @return Human-readable status string
     */
    String getStatusString() const;

    /**
     * @brief Discover supported PIDs from the vehicle
     * @return Vector of supported PID strings
     */
    std::vector<String> discoverSupportedPIDs();

    /**
     * @brief Get diagnostic data for a specific PID
     * @param pid PID string (e.g., "010C")
     * @return Formatted string with PID name and value
     */
    String getDiagnosticData(const String& pid);

    /**
     * @brief Get all available diagnostic data
     * @return Vector of formatted diagnostic strings
     */
    std::vector<String> getAllDiagnosticData();

private:
    BluetoothSerial serialBT;           ///< Bluetooth Serial interface
    String deviceName;                  ///< Bluetooth device name
    OBD2State currentState;             ///< Current connection state
    unsigned long lastInitCommand;      ///< Timestamp of last init command
    int initStep;                       ///< Current initialization step
    SensorData lastSensorData;          ///< Last valid sensor reading
    unsigned long lastBatteryUpdate;    ///< Timestamp of last battery voltage update

    // OBD2 Communication
    /**
     * @brief Send OBD2 command
     * @param command Command string to send
     */
    void sendCommand(const String& command);

    /**
     * @brief Read response from OBD2 interface
     * @param timeout Timeout in milliseconds
     * @return Response string
     */
    String readResponse(unsigned long timeout = 3000);

    /**
     * @brief Initialize OBD2 interface with AT commands
     */
    void initializeOBD2();

    /**
     * @brief Parse coolant temperature from OBD2 response
     * @param response Raw OBD2 response string
     * @return Temperature in Celsius, or -999 if parsing failed
     */
    static float parseCoolantTemp(const String& response);

    /**
     * @brief Parse boost pressure from OBD2 response
     * @param response Raw OBD2 response string
     * @return Pressure in kPa, or -999 if parsing failed
     */
    static float parseBoostPressure(const String& response);

    /**
     * @brief Parse battery voltage from ELM327 AT RV response
     * @param response Raw AT RV response string (e.g., "13.2 V\r")
     * @return Voltage in volts, or -1.0f if parsing failed
     */
    static float parseBatteryVoltage(const String& response);

    /**
     * @brief Check if response is valid OBD2 data
     * @param response Response string to validate
     * @return True if valid
     */
    static bool isValidResponse(const String& response);

    /**
     * @brief Parse supported PIDs from response
     * @param response Raw OBD2 response
     * @param startPID Starting PID number for this range
     * @param supportedPIDs Vector to store supported PID strings
     */
    void parseSupportedPIDs(const String& response, int startPID, std::vector<String>& supportedPIDs);

    /**
     * @brief Get human-readable name for a PID
     * @param pid PID string
     * @return Human-readable name
     */
    String getPIDName(const String& pid);

    /**
     * @brief Parse PID value from response
     * @param pid PID string
     * @param response Raw OBD2 response
     * @return Formatted value string
     */
    String parsePIDValue(const String& pid, const String& response);

    // Constants
    static constexpr unsigned long INIT_COMMAND_DELAY = 500;  ///< Delay between init commands
    static constexpr unsigned long RESPONSE_TIMEOUT = 3000;   ///< Response timeout in ms
    static constexpr int MAX_INIT_RETRIES = 3;                ///< Maximum initialization retries
};

#endif // OBD2_MANAGER_H
