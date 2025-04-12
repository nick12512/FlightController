#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

struct SensorData {
  float qw=0, qx=0, qy=0, qz=0;     // Quaternion from IMU
  float altitude=0;           // Barometric altitude
  float gpsLat=0, gpsLon=0;     // GPS location
  float gpsSpeed=0;           // GPS speed in knots
  float gpsAlt =0;             // GPS-reported altitude
  bool gpsFix =0;              // GPS fix status
  unsigned long timestamp = 0; // Total Time (millis)
  unsigned long last_update = 0; // Time since last update (millis)
  float updateHz = 0.0;

};

void InitIMU();
void InitGPS();
void InitAlt();
void getIMU(SensorData &data);
void getGPS(SensorData &data);
float getAlt();
void getRadio();

#endif
