#include "../includes/pid.h"

pid::pid(){
	
	this->referenceTemperature = 80.0;
	this->proportionalController = 30.0;
	this->integralController = 0.2;
	this->derivativeController = 400.0;

	this->period = 1.0;
	
	std::cout << "pid at default values of 30, 0.2 and 400.0 respectvely" << std::endl;
	std::cout << "Reference Temperature at place holder valeu of 80.0" << std::endl; 
}

pid::pid(float refTemp){
	
	this->referenceTemperature = refTemp;

	this->proportionalController = 30.0;
	this->integralController = 0.2;
	this->derivativeController = 400.0;

	this->period = 1.0;
}

pid::pid(float refTemp, float kp, float ki, float kd, float T){
	
	this->referenceTemperature = refTemp;

	this->proportionalController = kp;
	this->integralController = ki;
	this->derivativeController = kd;

	this->period = T;
}

void pid::setReferenceTemperature(float refTemp){
	this->referenceTemperature = refTemp;
}

float pid::boundedValue(float candidate){
	if(candidate >= MAXCONTROL){
		return MAXCONTROL;
	}

	if (candidate <= MINCONTROL){
		return MINCONTROL;
	}

	return candidate;
}

float pid::computePID(float inputTemperature){
	
	float errorCandidate;
	float deltaError;
	float newTemperature;

	this->deltaTemperature = this->referenceTemperature - inputTemperature;
	errorCandidate = this->totalError + deltaTemperature;
	
	this->totalError = this->boundedValue(errorCandidate);
	deltaError = this->deltaTemperature - this->previusError;

	newTemperature = this->proportionalController*this->deltaTemperature    + 
					 this->integralController*this->totalError*this->period + 
					 (this->derivativeController*deltaError)/this->period;

	newTemperature = this->boundedValue(newTemperature);

	this->previusError = this->deltaTemperature;

	return newTemperature;
}