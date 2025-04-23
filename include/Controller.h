#ifndef CONTROL_H
#define CONTROL_H

#include "DataCollection.h"
#include "QuaternionUtils.h"




static const Quaternion q_offset = {
   0.9844f, 0.0f, 0.0f, -0.1760f  // Hardcoded for -20.27° yaw
   //1.0000f, 0.0000f, 0.0000f, 0.0000f 
  };
  

void updateController(const SensorData& data);
void setDesiredQuaternion(float w, float x, float y, float z);

#endif