


#include "Debug.h"
#include <Arduino.h>

void printSensorData(const SensorData &data) {
    static unsigned long lastPrintTime = 0;
    static float printHz = 0.0;

    unsigned long now = millis();
    unsigned long delta = now - lastPrintTime;
    if (delta > 0) printHz = 1000.0 / delta;
    lastPrintTime = now;

    // --- Normal Output ---
    Serial.print("Q: ");
    Serial.print(data.qw, 3); Serial.print(",");
    Serial.print(data.qx, 3); Serial.print(",");
    Serial.print(data.qy, 3); Serial.print(",");
    Serial.print(data.qz, 3);

    Serial.print(" | Alt: ");
    Serial.print(data.altitude, 2); Serial.print(" m");

    if (data.gpsFix) {
        Serial.print(" | GPS: ");
        Serial.print(data.gpsLat, 4); Serial.print(",");
        Serial.print(data.gpsLon, 4);
        Serial.print(" | Speed: ");
        Serial.print(data.gpsSpeed, 2); Serial.print(" kn");
        Serial.print(" | GPS Alt: ");
        Serial.print(data.gpsAlt, 2); Serial.print(" m");
    } else {
        Serial.print(" | GPS: No Fix");
    }

    Serial.print(" | Hz: ");
    Serial.print(data.updateHz, 1);
    

    Serial.println();
}

  

