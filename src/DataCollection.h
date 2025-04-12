#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

struct SensorData {
  float qw, qx, qy, qz;     // Quaternion from IMU
  float altitude;           // Barometric altitude
  float gpsLat, gpsLon;     // GPS location
  float gpsSpeed;           // GPS speed in knots
  float gpsAlt;             // GPS-reported altitude
  bool gpsFix;              // GPS fix status
};

void InitIMU();
void InitGPS();
void InitAlt();
void getIMU(SensorData &data);
void getGPS(SensorData &data);
float getAlt();
void getRadio();

#endif
