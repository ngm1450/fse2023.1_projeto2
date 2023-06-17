#ifndef MENU_H
#define MENU_H

#include "lcd.h"
#include <iostream> 

class menu : public lcd{
private:

public:
	menu();
	void presentPreHeating(float intTemp, float refTemp);
	void presentBaking(int currentTime, int userTime, float refTemp);
	void clearMenu();

};

#endif //MENU_H