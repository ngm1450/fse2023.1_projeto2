#include "../includes/menu.h"

menu::menu(){
	this->WriteOnLcd(false,"AirFryer pronta");
}

void menu::presentPreHeating(float intTemp, float refTemp){
	
	this->clearMenu();
	
	if(intTemp < refTemp) this->WriteOnLcd(false,"Aquecendo:      ");
	if(intTemp > refTemp) this->WriteOnLcd(false,"Resfriando:     ");
	
	this->WriteFloat(true,true,"",intTemp);
	this->WriteFloat(false,true,"//",refTemp);
}

void menu::presentBaking(int currentTime, int userTime, float refTemp){
	
	int curentMin = currentTime/60; 
	int curentSec = currentTime%60;
	
	int userMin = userTime/60;
	int userSec = userTime%60;

	this->clearMenu();

	this->WriteInt(true,false,"Assando:        ",refTemp);
	
	this->WriteInt(true,true,"Tempo: ",curentMin);
	this->WriteInt(false,true,":",curentSec);
	
	this->WriteInt(false,true,"//",userMin);
	this->WriteInt(false,true,":",userSec);
	
	std::cout << "currentTime: " << currentTime << std::endl;
}

void menu::clearMenu(){
	this->WriteOnLcd(true,"                ");
	this->WriteOnLcd(false,"                ");
}