#include "../includes/lcd.h"

lcd::lcd(){

	this->fd = wiringPiI2CSetup(I2C_ADDR);
	lcd_byte(0x33, LCD_CMD);
	lcd_byte(0x32, LCD_CMD); 
	lcd_byte(0x06, LCD_CMD); 
	lcd_byte(0x0C, LCD_CMD); 
	lcd_byte(0x28, LCD_CMD); 
	lcd_byte(0x01, LCD_CMD); 
	delayMicroseconds(500);
}


void lcd::cursorAtStart(int line){
	
	lcd_byte(line, LCD_CMD);
}

void lcd::typeln(const char *str){

	while (*str) lcd_byte(*(str++), LCD_CHR);
}

void lcd::lcd_toggle_enable(int bits){
	delayMicroseconds(500);
	wiringPiI2CReadReg8(fd, (bits | ENABLE));
	delayMicroseconds(500);
	wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
	delayMicroseconds(500);
}

void lcd::WriteOnLcd(bool line2,const char *str){
	if (line2){
		this->cursorAtStart(LINE2);
		this->typeln(str);
		return;
	}
	this->cursorAtStart(LINE1);
	this->typeln(str);
}

void lcd::WriteInt(bool begin,bool line2,const char *str,int myInt){
	if (begin){
		if (line2) this->cursorAtStart(LINE2);
		if (!line2)this->cursorAtStart(LINE1);
	}
	this->typeln(str);

	char buffer[20];
	sprintf(buffer, "%d",  myInt);
	this->typeln(buffer);
}

void lcd::WriteFloat(bool begin,bool line2,const char *str,float myFloat){
	
	if (begin){
		if (line2) this->cursorAtStart(LINE2);
		if (!line2)this->cursorAtStart(LINE1);
	}
	this->typeln(str);


	char buffer[20];
	sprintf(buffer, "%4.2f",  myFloat);
	this->typeln(buffer);
}

void lcd::moveCursor(){
	this->lcd_byte(0x01, LCD_CMD);
	this->lcd_byte(0x02, LCD_CMD);
}

void lcd::lcd_byte(int bits, int mode){

	int bits_high;
	int bits_low;
	bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
	bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

	wiringPiI2CReadReg8(fd, bits_high);
	this->lcd_toggle_enable(bits_high);

	wiringPiI2CReadReg8(fd, bits_low);
	this->lcd_toggle_enable(bits_low);
}