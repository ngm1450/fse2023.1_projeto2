#include "../includes/bme280Communicate.h"

bme280Communicate::bme280Communicate() {
	rslt = BME280_OK;

	if ((id.fd = open("/dev/i2c-1", O_RDWR)) < 0){
		fprintf(stderr, "Failed to open the i2c bus %s\n", "/dev/i2c-1");
		exit(1);
	}

	id.dev_addr = BME280_I2C_ADDR_PRIM;

	if (ioctl(id.fd, I2C_SLAVE, id.dev_addr) < 0){
		fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
		exit(1);
	}

	dev.intf = BME280_I2C_INTF;
	dev.read = user_i2c_read;
	dev.write = user_i2c_write;
	dev.delay_us = user_delay_us;

	dev.intf_ptr = &id;

	rslt = bme280_init(&dev);
	if (rslt != BME280_OK){
		fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
		exit(1);
	}
}

bme280Communicate::~bme280Communicate() = default;

void user_delay_us(uint32_t period, void *intf_ptr) {
	id = *((struct identifier *)intf_ptr);
	usleep(period);
}

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr) {

	id = *((struct identifier *)intf_ptr);

	write(id.fd, &reg_addr, 1);
	read(id.fd, data, len);

	return 0;
}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr) {
	uint8_t *buf;

	id = *((struct identifier *)intf_ptr);

	buf = (uint8_t *)malloc(len + 1);
	buf[0] = reg_addr;
	memcpy(buf + 1, data, len);
	if (write(id.fd, buf, len + 1) < (uint16_t)len)
	{
		return BME280_E_COMM_FAIL;
	}

	free(buf);

	return BME280_OK;
}

double bme280Communicate::getExternalTemperature() {

	rslt = BME280_OK; 
	uint32_t req_delay;

	uint8_t settings_sel;
	struct bme280_data comp_data;

	dev.settings.osr_h = BME280_OVERSAMPLING_1X;
	dev.settings.osr_p = BME280_OVERSAMPLING_16X;
	dev.settings.osr_t = BME280_OVERSAMPLING_2X;
	dev.settings.filter = BME280_FILTER_COEFF_16;

	req_delay = bme280_cal_meas_delay(&dev.settings);

	settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

	rslt = bme280_set_sensor_settings(settings_sel, &dev);
	rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &dev);
	
	if (rslt != BME280_OK) {
		fprintf(stderr, "Failed to set sensor mode (code %+d).", rslt);
		return -1;
	}
	dev.delay_us(req_delay, dev.intf_ptr);

	rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);

	if (rslt != BME280_OK) {
		fprintf(stderr, "Failed to get sensor data (code %+d).", rslt);
		return -1;
	}

	return comp_data.temperature;

}