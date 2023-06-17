#ifndef PID_H
#define PID_H

#include <iostream>

static float MAXCONTROL = 100.0;
static float MINCONTROL = -100.0;

class pid {

private:
	float referenceTemperature = 0.0;

	float totalError = 0.0;
	float previusError = 0.0;
	float deltaTemperature = 0.0;

	float proportionalController = 0.0; 
	float integralController = 0.0; 	
	float derivativeController = 0.0;

	float period = 1.0;
	float boundedValue(float candidate);

public:
	pid();
	pid(float refTemp);
	pid(float refTemp, float kp, float ki, float kd, float T);

	float computePID(float inputTemperature);
	void setReferenceTemperature(float refTemp);
};

#endif 