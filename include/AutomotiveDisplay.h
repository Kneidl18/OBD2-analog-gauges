/**
 * @file AutomotiveDisplay.h
 * @brief Main application class for ESP32 automotive display
 * @author Andreas Auer
 */

#ifndef AUTOMOTIVE_DISPLAY_H
#define AUTOMOTIVE_DISPLAY_H

#include <Arduino.h>
#include "OBD2Manager.h"
#include "GaugeDisplay.h"
#include "SensorData.h"

/**
 * @brief Application states
 */
enum class AppState {
    INITIALIZING,     ///< System initialization
    WAITING_BT,       ///< Waiting for Bluetooth connection
    CONNECTING_OBD2,  ///< Connecting to OBD2 interface
    RUNNING,          ///< Normal operation
    DIAGNOSTIC_MODE,  ///< Diagnostic scanner mode
    ERROR             ///< Error state
};

/**
 * @brief Main application class coordinating all components
 */
class AutomotiveDisplay {
public:
    /**
     * @brief Constructor
     */
    AutomotiveDisplay();

    /**
     * @brief Destructor
     */
    ~AutomotiveDisplay() = default;

    /**
     * @brief Initialize all components
     * @return True if initialization successful
     */
    bool begin();

    /**
     * @brief Main update loop - call this in Arduino loop()
     */
    void update();

    /**
     * @brief Get current application state
     * @return Current AppState
     */
    AppState getState() const { return currentState; }

    /**
     * @brief Get status string for debugging
     * @return Human-readable status string
     */
    String getStatusString() const;

private:
    OBD2Manager obd2Manager;         ///< OBD2 communication manager
    GaugeDisplay gaugeDisplay;       ///< Display manager
    AppState currentState;           ///< Current application state
    SensorData currentSensorData;    ///< Current sensor readings
    
    // Timing variables
    unsigned long lastUpdate;        ///< Last sensor update timestamp
    unsigned long lastStateCheck;    ///< Last state check timestamp
    
    /**
     * @brief Handle state transitions
     */
    void handleStateTransitions();

    /**
     * @brief Update sensor data from OBD2
     */
    void updateSensorData();

    /**
     * @brief Update display with current data
     */
    void updateDisplay();

    /**
     * @brief Handle error conditions
     */
    void handleErrors();

    /**
     * @brief Log debug information to Serial
     */
    void logDebugInfo();

    /**
     * @brief Update display test simulation (only compiled when DISPLAY_TEST is defined)
     */
    void updateDisplayTest();

    /**
     * @brief Generate simulated sensor data for testing (only compiled when DISPLAY_TEST is defined)
     */
    void generateTestData();

    /**
     * @brief Handle diagnostic button press
     */
    void handleDiagnosticButton();

    /**
     * @brief Enter diagnostic scanner mode
     */
    void enterDiagnosticMode();

    /**
     * @brief Exit diagnostic scanner mode
     */
    void exitDiagnosticMode();

    /**
     * @brief Update diagnostic display
     */
    void updateDiagnosticMode();

    /**
     * @brief Check button state with debouncing
     * @return True if button is pressed (debounced)
     */
    bool isButtonPressed();

    // Display test state variables (only used when DISPLAY_TEST is defined)
    float testTemperature;           ///< Current test temperature
    float testPressure;              ///< Current test pressure
    float testVoltage;               ///< Current test voltage
    bool testTempIncreasing;         ///< Temperature direction flag
    bool testPressureIncreasing;     ///< Pressure direction flag
    bool testVoltageIncreasing;      ///< Voltage direction flag
    unsigned long lastTempUpdate;    ///< Last temperature update time
    unsigned long lastPressureUpdate; ///< Last pressure update time
    unsigned long lastVoltageUpdate; ///< Last voltage update time
    unsigned long lastStatusUpdate;  ///< Last status message update time

    // Diagnostic mode state variables
    bool diagnosticModeActive;       ///< Whether diagnostic mode is active
    unsigned long buttonPressTime;   ///< Time when button was first pressed
    unsigned long lastButtonCheck;   ///< Last time button was checked
    bool lastButtonState;            ///< Previous button state for debouncing
    unsigned long diagnosticStartTime; ///< When diagnostic mode started
    int currentDiagnosticIndex;      ///< Current PID index being displayed

    // Configuration constants
    static constexpr unsigned long UPDATE_INTERVAL = 2000;      ///< Sensor update interval (ms)
    static constexpr unsigned long STATE_CHECK_INTERVAL = 1000; ///< State check interval (ms)
    static constexpr unsigned long ERROR_RETRY_INTERVAL = 5000; ///< Error retry interval (ms)
};

#endif // AUTOMOTIVE_DISPLAY_H
