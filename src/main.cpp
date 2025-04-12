//Main.CPP
#include "Debug.h"
#include <Arduino.h>
#include "DataCollection.h"

#define DEBUG_SERIAL Serial

int refresh_rate = 10; // Refresh Rate in Hz
float update_time = 1000/refresh_rate; //refresh rate in millis
// Create Sensor Data Struct in order to store all data
SensorData rocketData;

void setup() {
  DEBUG_SERIAL.begin(115200);
  //Initialize IMU, Altimeter, and GPS
  InitIMU();
  InitAlt();
  InitGPS();
  
}




void loop() {
  //update loop time
  unsigned long now = millis();
  unsigned long dt = now - rocketData.last_update;
 


  // check if at correct update time (hz)
  if (dt >= update_time) {
    rocketData.timestamp = now;

    // Calculate Hz based on time since last update
    if (dt > 0) {
      rocketData.updateHz = 1000.0f / dt;
    }
    // Get Data
    getIMU(rocketData);
    getGPS(rocketData);

    rocketData.last_update = now;

    // Debug Print
    printSensorData(rocketData);


    // clear serial buffer
    while (Serial.available()) {
      Serial.read(); // clears unread incoming bytes
    }
    
    
  }

  delay(1);

  
  
}






