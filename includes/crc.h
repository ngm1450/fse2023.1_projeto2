#ifndef CRC16_H_
#define CRC16_H_

#include <stdint.h>

class crc16
{
private:
	uint16_t* table;
uint16_t crcFromTable(uint16_t crc, char data);

public:
	crc16();
	uint16_t computeCrc(unsigned char *commands, int size);

};


#endif 
