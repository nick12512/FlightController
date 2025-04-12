//DataCollection.cpp

#include "DataCollection.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_BNO08x.h>
#include <Adafruit_GPS.h>

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
  if (!bno08x.enableReport(SH2_ARVR_STABILIZED_RV, 2000)) {
    DEBUG_SERIAL.println("Failed to enable quaternion report!");
    while (1);
  }
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
void getIMU(SensorData &data) {
  if (bno08x.getSensorEvent(&sensorValue)) {
    data.qw = sensorValue.un.arvrStabilizedRV.real;
    data.qx = sensorValue.un.arvrStabilizedRV.i;
    data.qy = sensorValue.un.arvrStabilizedRV.j;
    data.qz = sensorValue.un.arvrStabilizedRV.k;
  }

  data.altitude = bmp.performReading() ? bmp.readAltitude(SEALEVELPRESSURE_HPA) : -999.0;
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
    if (sensorValue.sensorId == SH2_ARVR_STABILIZED_RV)
  return bmp.performReading() ? bmp.readAltitude(SEALEVELPRESSURE_HPA) : -999.0;
}

void getRadio() {
  // Stub: Add radio logic here
}
