#ifndef LCD_H
#define LCD_H

#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>


#define I2C_ADDR   0x27 

#define LCD_CHR  1
#define LCD_CMD  0 

#define LINE1  0x80 
#define LINE2  0xC0 

#define LCD_BACKLIGHT   0x08 
#define ENABLE  0b00000100 

class lcd {
private:
	int fd;  

	void lcd_byte(int bits, int mode);
	void cursorAtStart(int line);
	void moveCursor();
	void typeln(const char *str);
	void lcd_toggle_enable(int bits);

public:
	lcd();
	void WriteOnLcd(bool line2,const char *str);
	void WriteInt(bool begin,bool line2,const char *str,int myInt);
	void WriteFloat(bool begin,bool line2,const char *str,float myFloat);

};

#endif 