//Main.CPP
#include "Debug.h"
#include <Arduino.h>
#include "DataCollection.h"
#include "ServoOutput.h"
#include "Controller.h"



#define DEBUG_SERIAL Serial

const int refresh_rate = 50; // Refresh Rate in Hz
const float update_time = 1000/refresh_rate; //refresh rate in millis
const unsigned long deployDelay = 3500;  


// Create Sensor Data Struct in order to store all data
SensorData rocketData;



unsigned long launchTime = 0;



void updateData(unsigned long now, unsigned long dt);
void updateFlightPhase();
void FlightLogic();

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
  updateData(now,dt);
  updateFlightPhase();
  FlightLogic();
  updateController(rocketData);
  }
  
  
}


void updateData(unsigned long now,unsigned long dt){
  
 

  
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


// === Flight Phase Logic ===
void updateFlightPhase() {
  switch (rocketData.phase) {
    case SensorData::IDLE:
      if (rocketData.totalAccel < 6.00 ) {
        launchTime = rocketData.timestamp;
        rocketData.lastPhase = rocketData.phase;
        rocketData.phase = SensorData::POWERED;
      }
      break;

    case SensorData::POWERED:
      if (rocketData.timestamp - launchTime > deployDelay) {
        rocketData.lastPhase = rocketData.phase;
        rocketData.phase = SensorData::PARACHUTE_DEPLOYED;
      }
      break;

    case SensorData::PARACHUTE_DEPLOYED:
      // No transitions from here
      break;

    default:
      DEBUG_SERIAL.println("⚠ Unknown flight phase!");
      break;
  }
}



void FlightLogic() {
  switch (rocketData.phase) {
    case SensorData::IDLE:
      ZeroServos();
      break;

    case SensorData::POWERED:
      // Insert controller logic here
      break;

    case SensorData::PARACHUTE_DEPLOYED:
      // Cut servo power or stop actuation
      break;
  }
}


