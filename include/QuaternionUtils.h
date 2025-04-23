#ifndef QUATERNION_UTILS_H
#define QUATERNION_UTILS_H

struct Quaternion {
  float w, x, y, z;
};

Quaternion quatFromAxisAngle(float angle_rad, float ax, float ay, float az);
Quaternion quatInverse(const Quaternion& q);
Quaternion quatMultiply(const Quaternion& a, const Quaternion& b);
void rotateVector(const float v_in[3], float v_out[3], const Quaternion& q);

#endif
