/**
 * @file GaugeDisplay.h
 * @brief TFT display manager for automotive gauge rendering
 * @author Andreas Auer
 */

#ifndef GAUGE_DISPLAY_H
#define GAUGE_DISPLAY_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <vector>
#include "SensorData.h"

/**
 * @brief Configuration for individual gauge
 */
struct GaugeConfig {
    int centerX;           ///< X coordinate of gauge center
    int centerY;           ///< Y coordinate of gauge center
    int radius;            ///< Gauge radius in pixels
    float minValue;        ///< Minimum scale value
    float maxValue;        ///< Maximum scale value
    float blueThreshold;   ///< Blue zone upper limit (cold)
    float greenThreshold;  ///< Green zone upper limit (normal)
    float yellowThreshold; ///< Yellow zone upper limit (warning)
    String title;          ///< Gauge title
    String unit;           ///< Unit string
    uint16_t needleColor;  ///< Needle color
};

/**
 * @brief Manages TFT display and gauge rendering
 */
class GaugeDisplay {
public:
    /**
     * @brief Constructor
     */
    GaugeDisplay();

    /**
     * @brief Destructor
     */
    ~GaugeDisplay() = default;

    /**
     * @brief Initialize TFT display
     * @return True if initialization successful
     */
    bool begin();

    /**
     * @brief Update display with new sensor data
     * @param data Sensor data to display
     */
    void updateGauges(const SensorData& data);

    /**
     * @brief Display status message
     * @param message Status message to show
     */
    void showStatus(const String& message);

    /**
     * @brief Clear the display
     */
    void clear();

    /**
     * @brief Set display brightness
     * @param brightness Brightness level (0-255)
     */
    static void setBrightness(uint8_t brightness);

    /**
     * @brief Display diagnostic text data
     * @param diagnosticData Vector of diagnostic strings to display
     * @param currentIndex Current index for scrolling
     */
    void showDiagnosticData(const std::vector<String>& diagnosticData, int currentIndex);

    /**
     * @brief Show diagnostic mode header
     */
    void showDiagnosticHeader();

    /**
     * @brief Draw battery voltage display
     * @param data Sensor data containing battery voltage information
     */
    void drawBatteryVoltage(const SensorData& data);

private:
    TFT_eSPI tft;                    ///< TFT display instance
    GaugeConfig tempGaugeConfig;     ///< Temperature gauge configuration
    GaugeConfig boostGaugeConfig;    ///< Boost pressure gauge configuration
    bool initialized;                ///< Initialization status

    /**
     * @brief Draw analog gauge with needle
     * @param config Gauge configuration
     * @param value Current value to display
     */
    void drawAnalogGauge(const GaugeConfig& config, float value);

    /**
     * @brief Draw gauge tick marks and labels
     * @param config Gauge configuration
     * @param numTicks Number of tick marks
     */
    void drawGaugeTicks(const GaugeConfig& config, int numTicks);

    /**
     * @brief Draw colored zones (green/yellow/red)
     * @param config Gauge configuration
     */
    void drawColorZones(const GaugeConfig& config);

    /**
     * @brief Draw gauge needle
     * @param config Gauge configuration
     * @param value Current value for needle position
     */
    void drawNeedle(const GaugeConfig& config, float value);

    /**
     * @brief Map float value from one range to another
     * @param x Input value
     * @param in_min Input range minimum
     * @param in_max Input range maximum
     * @param out_min Output range minimum
     * @param out_max Output range maximum
     * @return Mapped value
     */
    static float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);

    /**
     * @brief Initialize gauge configurations
     */
    void initializeGaugeConfigs();

    // Display constants
    static constexpr int SCREEN_WIDTH = 240;
    static constexpr int SCREEN_HEIGHT = 320;
    static constexpr int GAUGE_RADIUS = 80;
    static constexpr int STATUS_HEIGHT = 30;

    // Color definitions
    static constexpr uint16_t COLOR_BACKGROUND = TFT_BLACK;
    static constexpr uint16_t COLOR_GAUGE_OUTLINE = TFT_WHITE;
    static constexpr uint16_t COLOR_NEEDLE = TFT_RED;
    static constexpr uint16_t COLOR_GREEN_ZONE = TFT_GREEN;
    static constexpr uint16_t COLOR_YELLOW_ZONE = TFT_YELLOW;
    static constexpr uint16_t COLOR_RED_ZONE = TFT_RED;
    static constexpr uint16_t COLOR_TEXT = TFT_WHITE;
    static constexpr uint16_t COLOR_TICK = TFT_LIGHTGREY;
};

#endif // GAUGE_DISPLAY_H
