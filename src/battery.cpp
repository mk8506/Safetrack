#include "battery.h"

Battery::Battery(uint8_t pin, unsigned long updateInterval) {
    analogPin = pin;
    readInterval = updateInterval;
    voltage = 0.0;
    percentage = 0.0;
    lastPercentage = 0.0;
    lastReadTime = 0;
}

void Battery::begin() {
    pinMode(analogPin, INPUT);
    forceRead();
    lastPercentage = percentage;
}

float Battery::mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool Battery::isTimeToRead() {
    return (millis() - lastReadTime) >= readInterval;
}

void Battery::forceRead() {
    int sensorValue = analogRead(analogPin); // 0 ~ 1023 (10-bit ADC) 
    // Say the battery is 4.2V full. Divide the voltage in half => 2.1 
    // (2.1 / 3.3) * 1024 = 652 when full, (1.85 / 3.3) * 1024 = 574 when about half, (1.4 / 3.3) * 1024 = 434 when low
    voltage = ((sensorValue * ADC_VREF) / ADC_MAX) * VOLTAGE_DIVIDER; //voltage = (sensorValue / 1024.0) * 3.3 (V)
        // | 4.2V (100%) | 2.1V | 652 | (652/1024)×3.3×2 = 4.2V |  
        // | 3.7V (64%)  | 1.85V | 574 | (574/1024)×3.3×2 = 3.7V |
        // | 3.3V (36%)  | 1.65V | 512 | (512/1024)×3.3×2 = 3.3V |
        // | 2.8V (0%)   | 1.4V | 434 | (434/1024)×3.3×2 = 2.8V |
    percentage = mapFloat(voltage, MIN_VOLTAGE, MAX_VOLTAGE, 0.0, 100.0); //convert voltage to percentage
    percentage = constrain(percentage, 1.0, 100.0); //0-100 valid range
    
    lastReadTime = millis();
    
    Serial.print("Battery - Raw: ");
    Serial.print(sensorValue);
    Serial.print(" | Voltage: ");
    Serial.print(voltage, 2);
    Serial.print("V | Percentage: ");
    Serial.print(percentage, 1);
    Serial.println("%");
}

void Battery::update() {
    if (isTimeToRead()) {
        forceRead();
    }
}

bool Battery::hasChanged(float threshold) {
    if (abs(percentage - lastPercentage) >= threshold) {
        lastPercentage = percentage;
        return true;
    }
    return false;
}

float Battery::getPercentage() {
    return percentage;
}

float Battery::getVoltage() {
    return voltage;
}

uint8_t Battery::getPercentageInt() {
    return (uint8_t)percentage;
}