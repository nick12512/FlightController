//DataCollection.cpp

#include "DataCollection.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_BNO08x.h>
#include <Adafruit_GPS.h>
#include "QuaternionUtils.h"
#include "controller.h"

#define GPSSerial Serial1
#define DEBUG_SERIAL Serial

// === Global Sensor Setup ===
// GPS
Adafruit_GPS GPS(&GPSSerial);
#define GPSECHO false

// BMP390
Adafruit_BMP3XX bmp;
#define SEALEVELPRESSURE_HPA (1013.25)

// BNO085
#define BNO08X_RESET -1
Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

// === Initialization Functions ===
void InitIMU() {
  if (!bno08x.begin_I2C()) {
    DEBUG_SERIAL.println("BNO085 not found!");
    while (1);
  }

  // Enable quaternion + accelerometer
  bno08x.enableReport(SH2_ARVR_STABILIZED_RV, 2000);   // quaternion @ 200 Hz
  bno08x.enableReport(SH2_ACCELEROMETER, 2000);        // accel @ 1000 Hz
}

void InitGPS() {
  GPSSerial.begin(9600);
  GPSSerial.println("$PMTK251,38400*27"); // Change to 38400 baud
  delay(100);
  GPSSerial.begin(38400);
  delay(100);

  DEBUG_SERIAL.println("Fast Loop: GPS + BMP390 + BNO085");

  // GPS commands
  GPSSerial.println("$PMTK313,1*2E");     // Enable SBAS (WAAS/EGNOS)
  delay(100);
  GPSSerial.println("$PMTK301,2*2E");     // Set DGPS mode to WAAS
  delay(100);
  GPSSerial.println("$PMTK220,200*2C");   // Set update rate to 5Hz
  delay(100);
  GPSSerial.println("$PMTK501,2*28");     // Set DGPS Mode to WAAS
  delay(100);
  GPSSerial.println("$CDCMD,33,1*7C");    // Antenna command
  GPSSerial.println("$PGCMD,33,1*6C");    // Request antenna status
  delay(100);

  GPS.begin(38400);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
}

void InitAlt() {
  if (!bmp.begin_I2C()) {
    DEBUG_SERIAL.println("BMP390 not found!");
    while (1);
  }
  bmp.setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
  bmp.setPressureOversampling(BMP3_NO_OVERSAMPLING);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);
  bmp.setOutputDataRate(BMP3_ODR_200_HZ);
}

// === Data Retrieval Functions ===
void getIMU(SensorData& data) {
  if (bno08x.getSensorEvent(&sensorValue) && sensorValue.sensorId == SH2_ARVR_STABILIZED_RV) {
    data.qw = sensorValue.un.arvrStabilizedRV.real;
    data.qx = sensorValue.un.arvrStabilizedRV.i;
    data.qy = sensorValue.un.arvrStabilizedRV.j;
    data.qz = sensorValue.un.arvrStabilizedRV.k;

    Quaternion q_imu = {
      sensorValue.un.arvrStabilizedRV.real,
      sensorValue.un.arvrStabilizedRV.i,
      sensorValue.un.arvrStabilizedRV.j,
      sensorValue.un.arvrStabilizedRV.k
    };
    
    Quaternion q_body = quatMultiply(q_offset, q_imu);
    data.qw = q_body.w;
    data.qx = q_body.x;
    data.qy = q_body.y;
    data.qz = q_body.z;
    
  }
  if (bno08x.getSensorEvent(&sensorValue) && sensorValue.sensorId == SH2_ACCELEROMETER) {
    data.ax = sensorValue.un.accelerometer.x;
    data.ay = sensorValue.un.accelerometer.y;
    data.az = sensorValue.un.accelerometer.z;
    data.totalAccel = sqrt(data.ax * data.ax + data.ay * data.ay + data.az * data.az);

    float a_imu[3] = {
      sensorValue.un.accelerometer.x,
      sensorValue.un.accelerometer.y,
      sensorValue.un.accelerometer.z
    };
    
    float a_body[3];
    rotateVector(a_imu, a_body, q_offset);
    
    data.ax = a_body[0];
    data.ay = a_body[1];
    data.az = a_body[2];
    data.totalAccel = sqrt(a_body[0]*a_body[0] + a_body[1]*a_body[1] + a_body[2]*a_body[2]);
    
    
  }


}


void getGPS(SensorData &data) {
  while (GPSSerial.available()) {
    GPS.read();
  }

  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA())) return;

    data.gpsFix = GPS.fix;
    if (GPS.fix) {
      data.gpsLat = GPS.latitude;
      data.gpsLon = GPS.longitude;
      data.gpsSpeed = GPS.speed;
      data.gpsAlt = GPS.altitude;
    }
  }
}


float getAlt() {
  return bmp.performReading() ? bmp.readAltitude(SEALEVELPRESSURE_HPA) : -999.0f;
}


void getRadio() {
  // Stub: Add radio logic here
}


