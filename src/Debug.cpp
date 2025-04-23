#include "Debug.h"
#include <Arduino.h>
#include "DataCollection.h"


const char* phaseToString(const SensorData& data) {
  switch (data.phase) {
    case SensorData::IDLE: return "IDLE";
    case SensorData::POWERED: return "POWERED";
    case SensorData::PARACHUTE_DEPLOYED: return "CHUTE";
    default: return "UNKNOWN";
  }
}


void printSensorData(const SensorData& data) {
  static unsigned long lastPrint = 0;
  static float loopHz = 0;

  unsigned long delta = data.timestamp - lastPrint;
  if (delta > 0) loopHz = 1000.0 / delta;
  lastPrint = data.timestamp;

  
  Serial.print(" | Accel: ");
  Serial.print(data.ax, 2); Serial.print(", ");
  Serial.print(data.ay, 2); Serial.print(", ");
  Serial.print(data.az, 2); Serial.print(" m/s²");

  Serial.print(" | Total: ");
  Serial.print(data.totalAccel, 2);

  Serial.print(" | Q: ");
  Serial.print(data.qw, 2); Serial.print(", ");
  Serial.print(data.qx, 2); Serial.print(", ");
  Serial.print(data.qy, 2); Serial.print(", ");
  Serial.print(data.qz, 2);

  Serial.print(" | Alt: ");
  Serial.print(data.altitude, 2); Serial.print(" m");

  if (data.gpsFix) {
    Serial.print(" | GPS: ");
    Serial.print(data.gpsLat, 4); Serial.print(", ");
    Serial.print(data.gpsLon, 4);

    Serial.print(" | Speed: ");
    Serial.print(data.gpsSpeed, 2); Serial.print(" kn");

    Serial.print(" | GPS Alt: ");
    Serial.print(data.gpsAlt, 2); Serial.print(" m");
  } else {
    Serial.print(" | GPS: No Fix");
  }

  Serial.print(" | Hz: ");
  Serial.print(loopHz, 1);
  Serial.println();
  Serial.print("Phase: ");
  Serial.print(phaseToString(data));
  
}



  

