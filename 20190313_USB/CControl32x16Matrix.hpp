#pragma once
#include <vector>

typedef std::vector<std::vector<char>> tMatrixData;

class CControl32x16Matrix{
//[Pin assign]: (GPIO pin No.)
//	[R0,G0,B0]	: [10, 9,11]
//	[R1,G1,B1]	: [25, 8, 7]
//	[R2,G2,B2]	: [21,20,26]
//	[R3,G3,B3]	: [16,19,13]
//	[A,B]		: [23,24]
//	[CLK,LAT,OE]: [17,27,22]

public:
	typedef std::vector<unsigned int> TExpressData;

private:
	static const short Pin_R0  = 10;
	static const short Pin_G0  =  9;
	static const short Pin_B0  = 11;
	static const short Pin_R1  = 25;
	static const short Pin_G1  =  8;
	static const short Pin_B1  =  7;
	static const short Pin_R2  = 21;
	static const short Pin_G2  = 20;
	static const short Pin_B2  = 26;
	static const short Pin_R3  = 16;
	static const short Pin_G3  = 19;
	static const short Pin_B3  = 13;
	static const short Pin_A     = 23;
	static const short Pin_B     = 24;
	static const short Pin_CLK   = 17;
	static const short Pin_LAT   = 27;
	static const short Pin_OE    = 22;
	
	// wait処理用
	void* g_timer;
	
	// outputData
	TExpressData m_redData, m_greenData, m_blueData;
	
	int Decode(unsigned int pData, unsigned int pPos, bool pInvert);
	
public:
	bool Init();
	void DataSet(TExpressData pRedData, TExpressData pGreenData, TExpressData pBlueData);
	bool TransferToMatrix();
};
