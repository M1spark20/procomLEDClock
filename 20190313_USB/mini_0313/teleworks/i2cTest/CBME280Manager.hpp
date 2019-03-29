#pragma once
#include "bme280.h"

enum EGetSensorDataFlag {
	eTemp, ePressure, eHumid, eEoM
};

class CBME280Manager{
	// data
	bme280_data	m_measureData;
	bme280_dev		m_systemData;
	int				m_fileHandleI2C;
	const char*	VIRTUALI2CFILE;
public:
	CBME280Manager() : VIRTUALI2CFILE("/dev/i2c-1"){}	// available after rasPi2
	bool Init();
	bool Measure();
	double getValue(EGetSensorDataFlag dataID);
};
