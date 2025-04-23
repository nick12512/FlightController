#include "Controller.h"
#include "ServoOutput.h"  // for writeFlightOutput()
#include <Arduino.h>
#include "QuaternionUtils.h"


static float qd_w = 1.0, qd_x = 0.0, qd_y = 0.0, qd_z = 0.0;

void setDesiredQuaternion(float w, float x, float y, float z) {
  qd_w = w; qd_x = x; qd_y = y; qd_z = z;
}

void updateController(const SensorData& data) {
  Quaternion q = { data.qw, data.qx, data.qy, data.qz };
  Quaternion qd = { qd_w, qd_x, qd_y, qd_z };

  // Quaternion error: q_err = qd ⊗ q⁻¹
  Quaternion q_inv = quatInverse(q);
  Quaternion q_err = quatMultiply(qd, q_inv);

  // Use imaginary part as rotation error (torque vector in IMU frame)
  float tau_imu[3] = {
    -q_err.x,
    -q_err.y,
    -q_err.z
  };

  // Rotate torque into body frame
  float tau_body[3];
  rotateVector(tau_imu, tau_body, q_offset);

  // Send to servos
  writeFlightOutput(tau_body[0], tau_body[1], tau_body[2]);
}