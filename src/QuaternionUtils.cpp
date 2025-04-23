#include "QuaternionUtils.h"
#include <math.h>

Quaternion quatFromAxisAngle(float angle_rad, float ax, float ay, float az) {
  float half = angle_rad / 2.0;
  float s = sin(half);
  return { cos(half), ax * s, ay * s, az * s };
}

Quaternion quatInverse(const Quaternion& q) {
  return { q.w, -q.x, -q.y, -q.z };
}

Quaternion quatMultiply(const Quaternion& a, const Quaternion& b) {
  return {
    a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
    a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
    a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
    a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w
  };
}

void rotateVector(const float v_in[3], float v_out[3], const Quaternion& q) {
  Quaternion p = { 0, v_in[0], v_in[1], v_in[2] };
  Quaternion q_inv = quatInverse(q);
  Quaternion temp = quatMultiply(q, p);
  Quaternion rotated = quatMultiply(temp, q_inv);
  v_out[0] = rotated.x;
  v_out[1] = rotated.y;
  v_out[2] = rotated.z;
}
