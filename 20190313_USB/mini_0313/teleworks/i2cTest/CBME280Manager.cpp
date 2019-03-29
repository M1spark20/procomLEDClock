#include "CBME280Manager.hpp"
#include <iostream>
// for I2C with virtual files
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>

// user_... functions are from linux_userspace.c in the API project.
int fd;

int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len){
	write(fd, &reg_addr,1);
	read(fd, data, len);
	return 0;
}
void user_delay_ms(uint32_t period){
	usleep(period*1000);
}
int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len){
	int8_t *buf;
	buf = (int8_t*)malloc(len +1);
	buf[0] = reg_addr;
	memcpy(buf +1, data, len);
	write(fd, buf, len +1);
	free(buf);
	return 0;
}

bool CBME280Manager::Init(){
	if ((m_fileHandleI2C = open(VIRTUALI2CFILE, O_RDWR)) < 0) {
		std::cout << "Failed to open the i2c bus: " << VIRTUALI2CFILE << std::endl;
		return false;
	}
	fd = m_fileHandleI2C;
	if (ioctl(m_fileHandleI2C, I2C_SLAVE, 0x76) < 0) {
		std::cout << "Failed to acquire bus access and/or talk to slave." << std::endl;
		return false;
	}
	m_systemData.dev_id = BME280_I2C_ADDR_PRIM;
	m_systemData.intf = BME280_I2C_INTF;
	m_systemData.read = user_i2c_read;
	m_systemData.write = user_i2c_write;
	m_systemData.delay_ms = user_delay_ms;
	return bme280_init(&m_systemData) == BME280_OK;
}

bool CBME280Manager::Measure(){
	uint8_t settings_sel;

	/* Recommended mode of operation: Weather monitoring */
	m_systemData.settings.osr_h = BME280_OVERSAMPLING_1X;
	m_systemData.settings.osr_p = BME280_OVERSAMPLING_1X;
	m_systemData.settings.osr_t = BME280_OVERSAMPLING_1X;
	m_systemData.settings.filter = BME280_FILTER_COEFF_OFF;

	settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

	if(bme280_set_sensor_settings(settings_sel, &m_systemData) != BME280_OK){
		std::cout << "Couldn't change sensor settings." << std::endl;
		return false;
	}

	if(bme280_set_sensor_mode(BME280_FORCED_MODE, &m_systemData) != BME280_OK){
		std::cout << "Coundn't set sensor mode to forced mode." << std::endl;
		return false;
	}
	/* Wait for the measurement */
	m_systemData.delay_ms(10);
	if(bme280_get_sensor_data(BME280_ALL, &m_measureData, &m_systemData) != BME280_OK){
		std::cout << "Coundn't get sensor values." << std::endl;
		return false;
	}
	return true;
}

double CBME280Manager::getValue(EGetSensorDataFlag dataID){
	switch(dataID){
		case eTemp:
			return m_measureData.temperature;
		case ePressure:
			return m_measureData.pressure;
		case eHumid:
			return m_measureData.humidity;
	}
	return 0;
}
