//DataCollection.h
#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

struct SensorData {
 //=============IMU Data===========
  float qw=0, qx=0, qy=0, qz=0;     // Quaternion from IMU
 float ax = 0, ay = 0, az = 0;  // Raw linear acceleration
 float totalAccel = 0;          // Magnitude of acceleration vecto

 
 //=============Barometer===========
  float altitude=0;           // Barometric altitude


  //=============GPS===========
  float gpsLat=0, gpsLon=0;     // GPS location
  float gpsSpeed=0;           // GPS speed in knots
  float gpsAlt =0;             // GPS-reported altitude
  bool gpsFix =0;              // GPS fix status

  ////=============Timer===========
  unsigned long timestamp = 0; // Total Time (millis)
  unsigned long last_update = 0; // Time since last update (millis)
  float updateHz = 0.0;


  // === Flight Phase Tracking ===
  enum FlightPhase {
  IDLE,
  POWERED,
  PARACHUTE_DEPLOYED
  };

  FlightPhase phase = IDLE;
  FlightPhase lastPhase = IDLE;

};






void InitIMU();
void InitGPS();
void InitAlt();
void getIMU(SensorData &data);
void getGPS(SensorData &data);
float getAlt();
void getRadio();

#endif
