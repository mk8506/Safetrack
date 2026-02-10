#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

class Battery {
private:
    uint8_t analogPin;
    float voltage;
    float percentage;
    float lastPercentage;
    unsigned long lastReadTime;
    unsigned long readInterval;
    
    // Battery voltage range
    const float MIN_VOLTAGE = 2.8;
    const float MAX_VOLTAGE = 4.2;
    
    // ADC configuration
    const float ADC_MAX = 1024.0; // ADC of nRF52840 is 10-bit => 2^10 = 1024 (0~1023)
    const float ADC_VREF = 3.3; // Reference Voltage of nRF52840
    const float VOLTAGE_DIVIDER = 2.0;  // Adjust based on the resistor divider. it could be 1 if not used
    
    // Helper function to map float values
    float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
    
public:
    // Constructor
    Battery(uint8_t pin, unsigned long updateInterval = 1000);
    
    // Initialize the battery monitor
    void begin();
    
    // Update battery reading (call this in loop)
    void update();
    
    // Check if battery value has changed significantly
    bool hasChanged(float threshold = 1.0);
    
    // Get current battery percentage
    float getPercentage();
    
    // Get current voltage
    float getVoltage();
    
    // Get battery percentage as uint8_t (0-100)
    uint8_t getPercentageInt();
    
    // Check if it's time to read battery
    bool isTimeToRead();
    
    // Force a battery read
    void forceRead();
};

#endif