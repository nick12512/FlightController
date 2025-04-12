//Main.CPP

#include <Arduino.h>
#include "DataCollection.h"

#define DEBUG_SERIAL Serial

SensorData rocketData;

void setup() {
  DEBUG_SERIAL.begin(115200);
  InitIMU();
  InitAlt();
  InitGPS();
}

void loop() {
  getIMU(rocketData);
  getGPS(rocketData);

  DEBUG_SERIAL.print("Q: ");
  DEBUG_SERIAL.print(rocketData.qw, 3); DEBUG_SERIAL.print(",");
  DEBUG_SERIAL.print(rocketData.qx, 3); DEBUG_SERIAL.print(",");
  DEBUG_SERIAL.print(rocketData.qy, 3); DEBUG_SERIAL.print(",");
  DEBUG_SERIAL.print(rocketData.qz, 3); DEBUG_SERIAL.print(" | ");

  DEBUG_SERIAL.print("Alt: ");
  DEBUG_SERIAL.print(rocketData.altitude, 2); DEBUG_SERIAL.print(" m | ");

  if (rocketData.gpsFix) {
    DEBUG_SERIAL.print("GPS: ");
    DEBUG_SERIAL.print(rocketData.gpsLat, 4); DEBUG_SERIAL.print(",");
    DEBUG_SERIAL.print(rocketData.gpsLon, 4); DEBUG_SERIAL.print(" | ");
    DEBUG_SERIAL.print("Speed: ");
    DEBUG_SERIAL.print(rocketData.gpsSpeed, 2); DEBUG_SERIAL.print(" kn | ");
    DEBUG_SERIAL.print("GPS Alt: ");
    DEBUG_SERIAL.print(rocketData.gpsAlt, 2);
  }

  DEBUG_SERIAL.println();
  delay(100);  // Adjust for desired update rate
}


