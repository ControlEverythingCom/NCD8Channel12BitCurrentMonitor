/* Includes ------------------------------------------------------------------*/
#include "application.h"

SYSTEM_MODE(MANUAL);
NCD8Channel12BitCurrentMonitor controller;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	Serial.begin(115200);
	controller.setAddress(0,0);
	controller.adcVRef = 4.958;
//	controller.adcVRef = 5.022;
	controller.calibrateInputs();
}

/* This function loops forever --------------------------------------------*/
void loop()
{
//	//Test read 1 input
//	float currentReading = controller.readInputCurrent(1);
//	Serial.print("Input ");
//	Serial.print(1);
//	Serial.print(" current reading: ");
//	Serial.println(currentReading, 4);
//	Serial.println();
//
//	delay(1000);

	//Test read all inputs:
	float inputReadings[8];
	float* currentReadings = controller.readAllInputsCurrent(inputReadings);
	for(int i = 0; i < 8; i++){
		Serial.print("Input ");
		Serial.print(i+1);
		Serial.print(" reading: ");
		Serial.println(currentReadings[i], 4);
	}
	delay(1000);

}
