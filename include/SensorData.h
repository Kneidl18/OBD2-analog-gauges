/**
 * @file SensorData.h
 * @brief Data structure for automotive sensor readings
 * @author Andreas Auer
 */

#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <Arduino.h>

/**
 * @brief Structure to hold automotive sensor data
 */
struct SensorData {
    // Existing sensor fields (preserve exactly)
    float coolantTemperature;       ///< Engine coolant temperature in Celsius
    float boostPressure;            ///< Intake manifold pressure in kPa
    bool isValid;                   ///< Flag indicating if data is valid
    unsigned long timestamp;        ///< Timestamp of last update

    // New battery voltage fields
    float batteryVoltage;           ///< Battery voltage in volts
    bool batteryVoltageValid;       ///< True if voltage reading is valid
    unsigned long batteryTimestamp; ///< Timestamp of last voltage reading

    /**
     * @brief Default constructor
     */
    SensorData() : coolantTemperature(0.0f), boostPressure(0.0f), isValid(false), timestamp(0),
                   batteryVoltage(0.0f), batteryVoltageValid(false), batteryTimestamp(0) {}

    /**
     * @brief Constructor with values
     * @param temp Coolant temperature in Celsius
     * @param pressure Boost pressure in kPa
     */
    SensorData(float temp, float pressure)
        : coolantTemperature(temp), boostPressure(pressure), isValid(true), timestamp(millis()),
          batteryVoltage(0.0f), batteryVoltageValid(false), batteryTimestamp(0) {}

    /**
     * @brief Update sensor data
     * @param temp New coolant temperature
     * @param pressure New boost pressure
     */
    void update(float temp, float pressure) {
        coolantTemperature = temp;
        boostPressure = pressure;
        isValid = true;
        timestamp = millis();
    }

    /**
     * @brief Update battery voltage
     * @param voltage New battery voltage in volts
     */
    void updateBatteryVoltage(float voltage) {
        batteryVoltage = voltage;
        batteryVoltageValid = true;
        batteryTimestamp = millis();
    }

    /**
     * @brief Mark battery voltage as invalid
     */
    void invalidateBatteryVoltage() {
        batteryVoltageValid = false;
    }

    /**
     * @brief Mark data as invalid
     */
    void invalidate() {
        isValid = false;
    }
};

#endif // SENSOR_DATA_H
