/**
 * @file GaugeDisplay.cpp
 * @brief Implementation of TFT display manager for automotive gauges
 * @author Andreas Auer
 */

#include "GaugeDisplay.h"
#include "config.h"

GaugeDisplay::GaugeDisplay() : initialized(false) {
    initializeGaugeConfigs();
}

bool GaugeDisplay::begin() {
    Serial.println("Initializing TFT display...");

    tft.init();
    tft.setRotation(0); // Portrait mode
    tft.fillScreen(COLOR_BACKGROUND);
    tft.setTextColor(COLOR_TEXT);

    initialized = true;
    showStatus("Display Ready");

    Serial.println("TFT display initialized successfully");
    return true;
}

void GaugeDisplay::updateGauges(const SensorData& data) {
    if (!initialized) return;

    // Clear gauge areas (preserve status area)
    tft.fillRect(0, STATUS_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - STATUS_HEIGHT, COLOR_BACKGROUND);

    // Draw temperature gauge
    drawAnalogGauge(tempGaugeConfig, data.coolantTemperature);

    // Draw boost pressure gauge
    drawAnalogGauge(boostGaugeConfig, data.boostPressure);

    // Draw battery voltage display
    #if BATTERY_VOLTAGE_ENABLED
    drawBatteryVoltage(data);
    #endif
}

void GaugeDisplay::showStatus(const String& message) {
    if (!initialized) return;

    // Clear status area
    tft.fillRect(0, 0, SCREEN_WIDTH, STATUS_HEIGHT, COLOR_BACKGROUND);

    // Draw status message
    tft.setTextSize(1);
    tft.setTextColor(COLOR_TEXT);
    tft.drawString(message, 5, 10);
}

void GaugeDisplay::clear() {
    if (!initialized) return;
    tft.fillScreen(COLOR_BACKGROUND);
}

void GaugeDisplay::setBrightness(uint8_t brightness) {
    // Note: TFT_eSPI doesn't have built-in brightness control
    // This would need to be implemented with hardware PWM on backlight pin
    // For now, this is a placeholder
}

void GaugeDisplay::drawAnalogGauge(const GaugeConfig& config, float value) {
    // Draw color zones first (background)
    drawColorZones(config);

    // Draw gauge outline
    tft.drawCircle(config.centerX, config.centerY, config.radius, COLOR_GAUGE_OUTLINE);
    tft.drawCircle(config.centerX, config.centerY, config.radius - 1, COLOR_GAUGE_OUTLINE);

    // Draw tick marks and labels
    drawGaugeTicks(config, 8);

    // Draw title
    tft.setTextSize(1);
    tft.setTextColor(COLOR_TEXT);
    const int titleWidth = config.title.length() * 6;
    tft.drawString(config.title, config.centerX - titleWidth/2, config.centerY - config.radius - 20);

    // Draw current value (always show actual value)
    const String valueStr = String(value, 1) + config.unit;
    tft.setTextSize(2);
    const int valueWidth = valueStr.length() * 12;
    tft.drawString(valueStr, config.centerX - valueWidth/2, config.centerY + config.radius + 10);

    // Calculate needle position value
    float needleValue = value;

    // Special handling for temperature gauge: needle stays at minimum if temp < 40°C
    if (config.title == "Coolant" && value < TEMP_NEEDLE_MIN_TEMP) {
        needleValue = config.minValue; // Use gauge minimum for needle position
    }

    // Draw needle
    drawNeedle(config, needleValue);

    // Draw center dot
    tft.fillCircle(config.centerX, config.centerY, 3, config.needleColor);
}

void GaugeDisplay::drawGaugeTicks(const GaugeConfig& config, const int numTicks) {
    for (int i = 0; i <= numTicks; i++) {
        const float angle = mapFloat(i, 0, numTicks, -90.0, 90.0);
        const float angleRad = (angle + 90) * PI / 180.0;

        const int outerX = config.centerX + (config.radius - 5) * cos(angleRad);
        const int outerY = config.centerY + (config.radius - 5) * sin(angleRad);
        const int innerX = config.centerX + (config.radius - 15) * cos(angleRad);
        const int innerY = config.centerY + (config.radius - 15) * sin(angleRad);

        tft.drawLine(outerX, outerY, innerX, innerY, COLOR_TICK);

        // Draw tick labels
        if (i % 2 == 0) { // Only label every other tick
            const float tickValue = mapFloat(i, 0, numTicks, config.minValue, config.maxValue);
            auto tickLabel = String(static_cast<int>(tickValue));

            const int labelX = config.centerX + (config.radius - 25) * cos(angleRad);
            const int labelY = config.centerY + (config.radius - 25) * sin(angleRad);

            tft.setTextSize(1);
            tft.setTextColor(COLOR_TEXT);
            const int labelWidth = tickLabel.length() * 6;
            tft.drawString(tickLabel, labelX - labelWidth/2, labelY - 4);
        }
    }
}

void GaugeDisplay::initializeGaugeConfigs() {
    // Temperature gauge configuration
    tempGaugeConfig.centerX = TEMP_GAUGE_X;
    tempGaugeConfig.centerY = TEMP_GAUGE_Y;
    tempGaugeConfig.radius = GAUGE_RADIUS;
    tempGaugeConfig.minValue = TEMP_GAUGE_MIN_VALUE;
    tempGaugeConfig.maxValue = TEMP_MAX_VALUE;
    tempGaugeConfig.blueThreshold = TEMP_BLUE_THRESHOLD;
    tempGaugeConfig.greenThreshold = TEMP_GREEN_THRESHOLD;
    tempGaugeConfig.yellowThreshold = TEMP_YELLOW_THRESHOLD;
    tempGaugeConfig.title = TEMP_TITLE;
    tempGaugeConfig.unit = TEMP_UNIT;
    tempGaugeConfig.needleColor = COLOR_NEEDLE;

    // Boost pressure gauge configuration
    boostGaugeConfig.centerX = BOOST_GAUGE_X;
    boostGaugeConfig.centerY = BOOST_GAUGE_Y;
    boostGaugeConfig.radius = GAUGE_RADIUS;
    boostGaugeConfig.minValue = BOOST_MIN_VALUE;
    boostGaugeConfig.maxValue = BOOST_MAX_VALUE;
    boostGaugeConfig.blueThreshold = BOOST_MIN_VALUE;  // No blue zone for pressure
    boostGaugeConfig.greenThreshold = BOOST_GREEN_THRESHOLD;
    boostGaugeConfig.yellowThreshold = BOOST_YELLOW_THRESHOLD;
    boostGaugeConfig.title = BOOST_TITLE;
    boostGaugeConfig.unit = BOOST_UNIT;
    boostGaugeConfig.needleColor = COLOR_NEEDLE;
}
void GaugeDisplay::drawColorZones(const GaugeConfig& config) {
    // Draw colored arcs for different zones
    const int numSegments = 60;

    for (int i = 0; i < numSegments; i++) {
        const float angle1 = mapFloat(i, 0, numSegments, -90.0, 90.0);
        const float angle2 = mapFloat(i + 1, 0, numSegments, -90.0, 90.0);

        const float value = mapFloat(i, 0, numSegments, config.minValue, config.maxValue);

        uint16_t color;
        if (value <= config.blueThreshold) {
            color = COLOR_BLUE_ZONE;        // Cold engine (below 80°C)
        } else if (value <= config.greenThreshold) {
            color = COLOR_GREEN_ZONE;       // Normal operating (80-100°C)
        } else if (value <= config.yellowThreshold) {
            color = COLOR_YELLOW_ZONE;      // Warning hot (100-115°C)
        } else {
            color = COLOR_RED_ZONE;         // Overheating (115°C+)
        }

        // Draw arc segment
        float angleRad1 = (angle1 + 90) * PI / 180.0;
        float angleRad2 = (angle2 + 90) * PI / 180.0;

        int x1 = config.centerX + (config.radius - 10) * cos(angleRad1);
        int y1 = config.centerY + (config.radius - 10) * sin(angleRad1);
        int x2 = config.centerX + (config.radius - 10) * cos(angleRad2);
        int y2 = config.centerY + (config.radius - 10) * sin(angleRad2);

        tft.drawLine(x1, y1, x2, y2, color);
    }
}

void GaugeDisplay::drawNeedle(const GaugeConfig& config, const float value) {
    // Calculate needle angle (180 degrees sweep, from -90 to +90 degrees)
    float angle = mapFloat(value, config.minValue, config.maxValue, -90.0, 90.0);
    angle = constrain(angle, -90.0, 90.0);

    // Convert to radians
    float angleRad = (angle + 90) * PI / 180.0; // Offset by 90 degrees and convert to radians

    // Calculate needle end point
    int needleLength = config.radius - 15;
    int needleX = config.centerX + needleLength * cos(angleRad);
    int needleY = config.centerY + needleLength * sin(angleRad);

    // Draw needle (multiple lines for thickness)
    tft.drawLine(config.centerX, config.centerY, needleX, needleY, config.needleColor);
    tft.drawLine(config.centerX + 1, config.centerY, needleX + 1, needleY, config.needleColor);
    tft.drawLine(config.centerX, config.centerY + 1, needleX, needleY + 1, config.needleColor);
}

float GaugeDisplay::mapFloat(const float x, const float in_min, const float in_max, const float out_min, const float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void GaugeDisplay::drawBatteryVoltage(const SensorData& data) {
    if (!initialized) return;

    // Position for battery voltage display (top-right corner)
    int x = VOLTAGE_DISPLAY_X;
    int y = VOLTAGE_DISPLAY_Y;

    // Clear the battery voltage area
    tft.fillRect(x - 5, y - 2, 40, 20, COLOR_BACKGROUND);

    // Set font size
    tft.setTextSize(VOLTAGE_FONT_SIZE);

    if (data.batteryVoltageValid) {
        // Determine color based on voltage thresholds
        uint16_t voltageColor;
        if (data.batteryVoltage < VOLTAGE_LOW_THRESHOLD) {
            voltageColor = COLOR_STATUS_ERROR;      // Red - critical low voltage
        } else if (data.batteryVoltage < VOLTAGE_MID_THRESHOLD) {
            voltageColor = COLOR_STATUS_WARNING;    // Yellow - low voltage warning
        } else if (data.batteryVoltage <= VOLTAGE_HIGH_THRESHOLD) {
            voltageColor = COLOR_STATUS_OK;         // Green - normal voltage
        } else {
            voltageColor = COLOR_STATUS_ERROR;      // Red - overcharge warning
        }

        // Format voltage string with specified decimal places
        String voltageStr = String(data.batteryVoltage, VOLTAGE_DISPLAY_DECIMALS) + "V";

        // Draw voltage text
        tft.setTextColor(voltageColor);
        tft.drawString(voltageStr, x, y);

        #if DEBUG_ENABLED && DEBUG_DISPLAY_UPDATES
        Serial.printf("Battery voltage displayed: %s (color: 0x%04X)\n",
                     voltageStr.c_str(), voltageColor);
        #endif

    } else {
        // Display error message if voltage reading is invalid
        tft.setTextColor(COLOR_STATUS_ERROR);
        tft.drawString("BATT ERR", x, y);

        #if DEBUG_ENABLED && DEBUG_DISPLAY_UPDATES
        Serial.println("Battery voltage error displayed");
        #endif
    }

    // Reset text color to default
    tft.setTextColor(COLOR_TEXT);
}