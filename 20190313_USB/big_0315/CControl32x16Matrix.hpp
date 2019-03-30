#pragma once
#include <vector>

typedef std::vector<std::vector<char>> tMatrixData;

// [act]32x16のマトリクス管理クラス
class CControl32x16Matrix{
//[Pin assign]: (GPIO pin No.)
//	[R0,G0,B0]	: [10, 9,11]
//	[R1,G1,B1]	: [25, 8, 7]
//	[R2,G2,B2]	: [21,20,26]
//	[R3,G3,B3]	: [16,19,13]
//	[A,B]		: [23,24]
//	[CLK,LAT,OE]: [17,27,22]
//	接続時のヒント: A,B,CLK,LAT,OE,GNDは上段・下段で共通。その分岐のためにブレッドボードを配置してある。
//	ピン配列は後程お渡しします…。

public:
	// 32bit変数。左上から横1列に32ドット分3色の色の点灯有無を決定。
	// 配列が進むにつれて下に、一番下についたらその右上に、という形で遷移していく
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
	
	// 未実装
	bool				 m_nightModeFlag;
	
	// outputData(Red, Green, Blue)
	TExpressData m_redData, m_greenData, m_blueData;
	
	// [act]内部データから任意の位置の点灯の有無を割り出す
	// [prm]pData	: 検証データ 32bit変数
	//		pPos	: 検証を行う位置 bit位置でよい
	//		pInvert	: 今のところ使用なし
	int Decode(unsigned int pData, unsigned int pPos, bool pInvert);
	
public:
	// [act]マトリクスの初期化を行う
	// [ret]初期化に成功したか
	bool Init();
	
	// [act]マトリクスに表示するデータを外部から設定する
	// [prm]pRedData	: 赤に関するデータ
	//		pGreenData	: 緑に関するデータ
	//		pBlueData	: 青に関するデータ
	void DataSet(TExpressData pRedData, TExpressData pGreenData, TExpressData pBlueData);
	
	// [act]データをマトリクスに1回転送する
	// [ret]転送に成功したか
	bool TransferToMatrix();
	
	// [act]プログラム起動からの時刻をms単位で取得する
	// [ret]起動からの時刻
	unsigned int getTime();
	
	// [act]今のところ効果なし
	void setNightModeFlag(bool value);
};
