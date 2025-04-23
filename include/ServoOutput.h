#ifndef FLIGHT_OUTPUT_H
#define FLIGHT_OUTPUT_H

#include <Arduino.h>
#include "DataCollection.h"

void InitFlightOutput();
void writeFlightOutput(float tx, float ty, float tz);
void cutFlightOutput();
void ZeroServos();


#endif