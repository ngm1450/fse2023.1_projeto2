#ifndef BM280COMMUNICATE
#define BM280COMMUNICATE

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include "bme280.h"

struct identifier {	
	uint8_t dev_addr;
	int8_t fd;
};

static struct bme280_dev dev;
static struct identifier id;

static int8_t rslt;

void user_delay_us(uint32_t period, void *intf_ptr);

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

class bme280Communicate {
private:
	
	float temperature;


public: 
	bme280Communicate();
	~bme280Communicate();
	double getExternalTemperature();
};


#endif