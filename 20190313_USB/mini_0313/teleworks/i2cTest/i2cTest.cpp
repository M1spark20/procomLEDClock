#include <iostream>
#include <iomanip>
#include "CBME280Manager.hpp"
#include <unistd.h>

int main(){
	CBME280Manager sensorManager;
	sensorManager.Init();
	std::cout << std::fixed;
	while(1){
		sensorManager.Measure();
		std::cout << "Temparature: "	<< std::setprecision(2) << sensorManager.getValue(eTemp)				<< "[deg] ";
		std::cout << "Pressure: "	<< std::setprecision(2) << sensorManager.getValue(ePressure)/100.0	<< "[hPa] ";
		std::cout << "Humidity: "	<< std::setprecision(2) << sensorManager.getValue(eHumid)				<< "[%]";
		std::cout << std::endl;
		sleep(1);
	}
	return 0;
}
