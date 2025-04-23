#include "ServoOutput.h"
#include <Servo.h>

Servo servo1, servo2, servo3, servo4;

const int servoPin1 = 3;
const int servoPin2 = 4;
const int servoPin3 = 5;
const int servoPin4 = 6;

void InitFlightOutput() {
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
}

void writeFlightOutput(float tx, float ty, float tz) {
  // For now: map [-1, 1] to [0, 180]
  int servo1_output = 90;
  int servo2_output = 90;
  int servo3_output = 90;
  int servo4_output= 90; // placeholder for thrust or yaw symmetry

  // Send to hardware
  servo1.write(servo1_output);
  servo2.write(servo2_output);
  servo3.write(servo3_output);
  servo4.write(servo4_output);
}

void ZeroServos(){
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    servo4.write(90);
}
void cutFlightOutput() {
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
}