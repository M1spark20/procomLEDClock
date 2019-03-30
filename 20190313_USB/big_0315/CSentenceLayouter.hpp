#pragma once
#include "SStringDots.hpp"
#include <thread>

class CControl32x16Matrix;

// [act]ドットデータをマトリクスに転送する
class CSentenceLayouter{
	SStringDots				m_dotData[2];		// 文字列をドットにしたデータ[上段(y=0～), 下段(y=16～)]
	
	SStringDots				m_showData[2];		// マトリクスに表示するデータ：ダブルバッファにより高速化 片方は表示用・片方は更新用
	SStringDots*			m_pUsingData;		// 現在表示用のマトリクスデータ格納先を保存
	SStringDots*			m_pBufData;			// 現在更新用のマトリクスデータ格納先を保存
	
	std::thread				m_flushThread;		// マトリクスの更新を並列処理で行うための変数。こうすることでスムーズに画面の切り替えを行う(std::threadで検索)
	bool					m_isSwapped;		// データの更新を行ってから、ダブルバッファの切り替えが行われたか
	bool					m_terminateFlag;	// プログラムが終了したかどうか(マトリクスへの表示を並列処理で行うために存在)
	
	unsigned int			m_scrollSpeed[2];	// スクロール速度: 1ドット移動するのに何msかけるかを格納[上段(y=0～), 下段(y=16～)]
	unsigned int			m_nextScrollTime[2];// スクロール速度に応じて次にスクロールする時間を格納[上段(y=0～), 下段(y=16～)]
	int						m_scrollPos[2];		// 現在のスクロール位置を格納[上段(y=0～), 下段(y=16～)] 値域は[ -127, m_dotData[*][0].size() )
	CControl32x16Matrix&	m_matrixManager;	// マトリクスクラスの参照(インスタンスはmainにて生成する)
public:
	// [act]各変数の初期化・並列処理の初期化を行う
	// [prm]matrix: マトリクス管理クラスのインスタンス
	CSentenceLayouter(CControl32x16Matrix& matrix);
	
	// [act]スクロール速度の更新を行う
	// [prm]timeMS	: 更新間隔を指定 [ms]単位
	//		isUpper	: 更新するデータは上段かを指定(true:上段, false:下段)
	void setScrollSpeed(unsigned int timeMS, bool isUpper);
	
	// [act]マトリクスに表示するドットデータの更新を行う
	// [prm]pData	: 新しいドットデータ
	//		isUpper	: 更新するデータは上段かを指定(true:上段, false:下段)
	//		isReset	: スクロール位置をリセットするか(true:する, false:しない)
	void setData(const SStringDots& pData, bool isUpper, bool isReset);
	
	// [act]マトリクスに表示するデータの更新を行う
	//		どちらかというと、上のsetData関数で指定したデータを反映する・スクロールを行う感覚
	// [ret]更新に成功したか
	bool refresh();
	
	// [act]マトリクスにデータを流し込んで表示する。並列処理で回転する関数。
	void flushProcess();
	
	// [act]並列処理の後片付けをする
	~CSentenceLayouter();
};
