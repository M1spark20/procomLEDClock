#include "CControl64x32Matrix.hpp"
#include <wiringPi.h>
#include <iostream>
#include <iomanip>

// compile Check
//#define OUTPUT 0
//int wiringPiSetupGpio(){ return 0; }
//void pinMode(int pos, int it){};
//void digitalWrite(int a, int b){};

unsigned int CControl64x32Matrix::getTime(){
	return millis();
}

int CControl64x32Matrix::Decode(unsigned long long pData, unsigned int pPos, bool pInvert){
	const unsigned int match = pData >> pPos & 0x1;
	return match ? 1 : 0;
}

void CControl64x32Matrix::setNightModeFlag(bool value){
	m_nightModeFlag = value;
}

bool CControl64x32Matrix::Init(){
	// Initialize WiringPi
	if(wiringPiSetupGpio() == -1) return false;
	m_nightModeFlag = false;

	// Set pin mode
	pinMode(Pin_R0 , OUTPUT);
	pinMode(Pin_G0 , OUTPUT);
	pinMode(Pin_B0 , OUTPUT);
	pinMode(Pin_R1 , OUTPUT);
	pinMode(Pin_G1 , OUTPUT);
	pinMode(Pin_B1 , OUTPUT);
	pinMode(Pin_A  , OUTPUT);
	pinMode(Pin_B  , OUTPUT);
	pinMode(Pin_C  , OUTPUT);
	pinMode(Pin_D  , OUTPUT);
	pinMode(Pin_CLK, OUTPUT);
	pinMode(Pin_LAT, OUTPUT);
	pinMode(Pin_OE , OUTPUT);
	pinMode(Pin_CFG,  INPUT);
	
	digitalWrite(Pin_LAT, 0);
	return true;
}

void CControl64x32Matrix::DataSet(
	CControl64x32Matrix::TExpressData pRedData  , 
	CControl64x32Matrix::TExpressData pGreenData, 
	CControl64x32Matrix::TExpressData pBlueData )
{
	m_redData = pRedData;
	m_greenData = pGreenData;
	m_blueData = pBlueData;
}

bool CControl64x32Matrix::TransferToMatrix(){
	const TExpressData *const data[] = { &m_redData, &m_greenData, &m_blueData };
	unsigned int size[3], maxSize=0;
	
	for(int i=0; i<3; ++i){
		size[i] = data[i]->size();
		if(size[i] > maxSize) maxSize = size[i];
	}
	
	
	for(unsigned int port=0; port<16; ++port){
		digitalWrite(Pin_OE , 0);		// active LOW
		for(unsigned int panel=0; panel*32<maxSize; ++panel){
			const unsigned int sectionBegin[][2] = {
				{ 0, panel*32+port    },
				{ 0, panel*32+port+16 },
			}; // [0, 1][x, y]
			unsigned long long transferData[][2] = {
				{ 0, 0 }, { 0, 0 }, { 0, 0 }
			};
			
			for(unsigned int address=0; address<2; ++address)
			for(unsigned int color=0; color<3; ++color){
				if(size[color] <= sectionBegin[address][1]) continue;
				//std::cout << sectionBegin[address][1] << std::endl;
				transferData[color][address] = (*data[color])[sectionBegin[address][1]];
			}
			if(m_nightModeFlag) digitalWrite(Pin_OE, 1);
				
			for(unsigned int dots=0; dots<64; ++dots){
				digitalWrite(Pin_R0 ,Decode(transferData[0][0], sectionBegin[0][0]+dots, false));
				digitalWrite(Pin_G0 ,Decode(transferData[1][0], sectionBegin[0][0]+dots, false));
				digitalWrite(Pin_B0 ,Decode(transferData[2][0], sectionBegin[0][0]+dots, false));
				digitalWrite(Pin_R1 ,Decode(transferData[0][1], sectionBegin[1][0]+dots, false));
				digitalWrite(Pin_G1 ,Decode(transferData[1][1], sectionBegin[1][0]+dots, false));
				digitalWrite(Pin_B1 ,Decode(transferData[2][1], sectionBegin[1][0]+dots, false));
				digitalWrite(Pin_CLK, 1);
				while(digitalRead(Pin_CFG)==0);
				digitalWrite(Pin_CLK, 0);
			}
		}
		if(!m_nightModeFlag) digitalWrite(Pin_OE , 1);
		delayMicroseconds(1);
		digitalWrite(Pin_A , Decode(port, 0, false));
		digitalWrite(Pin_B , Decode(port, 1, false));
		digitalWrite(Pin_C , Decode(port, 2, false));
		digitalWrite(Pin_D , Decode(port, 3, false));
		digitalWrite(Pin_LAT, 1);
		delayMicroseconds(1);
		digitalWrite(Pin_LAT, 0);
	}
	return true;
}
