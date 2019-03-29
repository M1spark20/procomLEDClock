#pragma once
#include <vector>

class CControl64x32Matrix{
//[Pin assign]: (GPIO pin No.)
//	[R0,G0,B0]		: [10, 9,11]
//	[R1,G1,B1]		: [25, 8, 7]
//	[A,B,C,D]		: [23,24,15,18]
//	[CLK,LAT,OE]	: [17,27,22]

public:
	typedef std::vector<unsigned long long> TExpressData;

private:
	static const short Pin_R0  = 10;
	static const short Pin_G0  =  9;
	static const short Pin_B0  = 11;
	static const short Pin_R1  = 25;
	static const short Pin_G1  =  8;
	static const short Pin_B1  =  7;
	static const short Pin_A   = 23;
	static const short Pin_B   = 24;
	static const short Pin_C   = 15;
	static const short Pin_D   = 18;
	static const short Pin_CLK = 17;
	static const short Pin_LAT = 27;
	static const short Pin_OE  = 22;
	static const short Pin_CFG = 21;
	
	bool				 m_nightModeFlag;
	
	// outputData
	TExpressData m_redData, m_greenData, m_blueData;
	
	int Decode(unsigned long long pData, unsigned int pPos, bool pInvert);
	
public:
	bool Init();
	void DataSet(TExpressData pRedData, TExpressData pGreenData, TExpressData pBlueData);
	bool TransferToMatrix();
	unsigned int getTime();
	void setNightModeFlag(bool value);
};
