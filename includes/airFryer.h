#ifndef AIRFRYER_H
#define AIRFRYER_H

#include <iostream>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdlib.h> 
#include <signal.h>
#include "UART.h"
#include "pid.h"
#include "menu.h"
#include "bme280Communicate.h"

#define GPIO23 4
#define GPIO24 5

static bool allOn = true;

class airFryer : public uart, public pid, public menu, public bme280Communicate{

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> timeStart;
	std::chrono::time_point<std::chrono::high_resolution_clock> timeEnd;
	std::chrono::duration<float> Timer;
	
	void activateResistence(int percentage);
	void activateFan(int percentage);
	void coolDown();
	void PreHeating();
	bool Baking();

public:
	airFryer();
	~airFryer();

	void bake();
};

#endif