#include "CControl32x16Matrix.hpp"
#include <wiringPi.h>
#include <unistd.h>
// compile Check
//#define OUTPUT 0
//int wiringPiSetupGpio(){ return 0; }
//void pinMode(int pos, int it){};
//void digitalWrite(int a, int b){};

unsigned int CControl32x16Matrix::getTime(){
	return millis();
}

void CControl32x16Matrix::setNightModeFlag(bool value){
	m_nightModeFlag = value;
}

int CControl32x16Matrix::Decode(unsigned int pData, unsigned int pPos, bool pInvert){
	const unsigned int mask = 1 << pPos;
	const bool ans = pInvert ? !(pData & mask) : (pData & mask);
	return ans ? 1 : 0;
}

bool CControl32x16Matrix::Init(){
	// Initialize WiringPi
	if(wiringPiSetupGpio() == -1) return false;

	// Set pin mode
	pinMode(Pin_R0 , OUTPUT);
	pinMode(Pin_G0 , OUTPUT);
	pinMode(Pin_B0 , OUTPUT);
	pinMode(Pin_R1 , OUTPUT);
	pinMode(Pin_G1 , OUTPUT);
	pinMode(Pin_B1 , OUTPUT);
	pinMode(Pin_R2 , OUTPUT);
	pinMode(Pin_G2 , OUTPUT);
	pinMode(Pin_B2 , OUTPUT);
	pinMode(Pin_R3 , OUTPUT);
	pinMode(Pin_G3 , OUTPUT);
	pinMode(Pin_B3 , OUTPUT);
	pinMode(Pin_A  , OUTPUT);
	pinMode(Pin_B  , OUTPUT);
	pinMode(Pin_CLK, OUTPUT);
	pinMode(Pin_LAT, OUTPUT);
	pinMode(Pin_OE , OUTPUT);
	
	digitalWrite(Pin_LAT, 1);
	digitalWrite(Pin_LAT, 0);
	return true;
}

void CControl32x16Matrix::DataSet(
	CControl32x16Matrix::TExpressData pRedData  , 
	CControl32x16Matrix::TExpressData pGreenData, 
	CControl32x16Matrix::TExpressData pBlueData )
{
	m_redData = pRedData;
	m_greenData = pGreenData;
	m_blueData = pBlueData;
}

bool CControl32x16Matrix::TransferToMatrix(){
	const TExpressData *const data[] = { &m_redData, &m_greenData, &m_blueData };
	unsigned int size[3], maxSize=0;
	
	for(int i=0; i<3; ++i){
		size[i] = data[i]->size();
		if(size[i] > maxSize) maxSize = size[i];
	}
	
	for(unsigned int port=0; port<4; ++port){
		for(unsigned int panel=0; panel*32<maxSize; ++panel){
			digitalWrite(Pin_OE, 0);
			for(unsigned int section=0; section<8; ++section){
				// ã‰º”½“]‚³‚¹‚é
				const unsigned int sectionBegin[][2] = {
					{ (section/2)*8, 32-((section%2)*4+port) + panel*32},
					{ (section/2)*8, 24-((section%2)*4+port) + panel*32},
					{ (section/2)*8, 16-((section%2)*4+port) + panel*32},
					{ (section/2)*8,  8-((section%2)*4+port) + panel*32},
				};
				unsigned int transferData[][4] = {
					{ 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }
				};// [0,1,2,3][R0,G0,B0]
						
				for(unsigned int address=0; address<4; ++address)
				for(unsigned int color=0; color<3; ++color){
					if(size[color] <= sectionBegin[address][1]) continue;						// out of range
					transferData[color][address] = (*data[color])[sectionBegin[address][1]];	// 1 line 32 dots
				}
					
				for(unsigned int dots=0; dots<8; ++dots){
					unsigned int showPosX[2];
					for(int i=0; i<2; ++i){
						showPosX[i] = sectionBegin[i][0];
						showPosX[i] += (section%2) ? dots : 7-dots;
					}
					digitalWrite(Pin_R0, Decode(transferData[0][0], showPosX[0], false));
					digitalWrite(Pin_G0, Decode(transferData[1][0], showPosX[0], false));
					digitalWrite(Pin_B0, Decode(transferData[2][0], showPosX[0], false));
					digitalWrite(Pin_R1, Decode(transferData[0][1], showPosX[1], false));
					digitalWrite(Pin_G1, Decode(transferData[1][1], showPosX[1], false));
					digitalWrite(Pin_B1, Decode(transferData[2][1], showPosX[1], false));
					digitalWrite(Pin_R2, Decode(transferData[0][2], showPosX[0], false));
					digitalWrite(Pin_G2, Decode(transferData[1][2], showPosX[0], false));
					digitalWrite(Pin_B2, Decode(transferData[2][2], showPosX[0], false));
					digitalWrite(Pin_R3, Decode(transferData[0][3], showPosX[1], false));
					digitalWrite(Pin_G3, Decode(transferData[1][3], showPosX[1], false));
					digitalWrite(Pin_B3, Decode(transferData[2][3], showPosX[1], false));
					digitalWrite(Pin_CLK, 1);
					delayMicroseconds(1);		// from wiringPi
					digitalWrite(Pin_CLK, 0);
				}
			}
		}
		digitalWrite(Pin_OE , 1);
		digitalWrite(Pin_A , Decode(port, 0, false));
		digitalWrite(Pin_B , Decode(port, 1, false));
		digitalWrite(Pin_LAT, 1);
		//delayMicroseconds(2);		// from wiringPi(‚¿‚ç‚Â‚­‚æ‚¤‚È‚çƒRƒƒ“ƒg‚ð‰ðœ‚µ‚Ä‚­‚¾‚³‚¢)
		digitalWrite(Pin_LAT, 0);
	}
	
	return true;
}
