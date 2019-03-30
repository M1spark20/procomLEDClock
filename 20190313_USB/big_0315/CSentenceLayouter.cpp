#include "CSentenceLayouter.hpp"
#include "CControl32x16Matrix.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

CSentenceLayouter::CSentenceLayouter(CControl32x16Matrix& matrix) : m_matrixManager(matrix){
	// 変数の初期化
	m_showData[0].brightness.resize(32);
	m_showData[0].colorData.resize(32);
	m_terminateFlag = false;
	for(auto i=0; i<m_showData[0].brightness.size(); ++i){
		m_showData[0].brightness[i].resize(32*4);
		m_showData[0].colorData[i].resize(32*4);
	}
	m_showData[1] = SStringDots(m_showData[0]);
	m_flushThread = std::thread(&CSentenceLayouter::flushProcess, this);
	m_scrollSpeed[0] = -1;	// 自動スクロールしない
	m_scrollSpeed[1] = 50;
	m_nextScrollTime = m_matrixManager.getTime() + m_scrollSpeed;
	m_pUsingData = &m_showData[0]; m_pBufData = &m_showData[1];
	
	// 並列処理のための初期化
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(3, &cpuset);
	int rc = pthread_setaffinity_np(m_flushThread.native_handle(), sizeof(cpu_set_t), &cpuset);
	//std::cout << rc << std::endl;
}

void CSentenceLayouter::setData(const SStringDots& pData, bool isUpper, bool isReset){
	m_dotData[isUpper ? 0:1] = SStringDots(pData);
	if(!isReset) return;
	m_scrollPos[isUpper ? 0:1] = m_scrollSpeed[isUpper ? 0:1] < 0 ? 0 : -127;	// スクロールしない場合は左端に文字を描く
	m_nextScrollTime[isUpper ? 0:1] = m_matrixManager.getTime();
	//std::cout << pData.brightness.size() << "," << pData.brightness[0].size() << std::endl;
}

bool CSentenceLayouter::refresh(){
	//std::cout << m_nextScrollTime << " " << m_scrollPos << " " << left << " " << right << " " << strBeg << std::endl;
	for(auto side=0; side<2; ++side){
		if(m_dotData[side].brightness.size() == 0) continue;
		if(m_matrixManager.getTime() < m_nextScrollTime[side]) continue;
		if(++m_scrollPos[side] >= (signed int)m_dotData[side].brightness[0].size()) m_scrollPos[side] = -127;
		// はみだし防止
		const int left  = m_scrollPos[side] < 0 ? -m_scrollPos[side] : 0;
		const int right = m_scrollPos[side] + 128 >= m_dotData[side].brightness[0].size() ?
			m_scrollPos[side] + 128 - m_dotData[side].brightness[0].size() : 0;
		
		// データを1行ずつコピーする
		const int strBeg = m_scrollPos[side] < 0 ? 0 : m_scrollPos[side];
		for(auto _y=0; _y<m_dotData[side].brightness.size(); ++_y){
			const auto y = _y + side*16;
			for(auto x=m_pBufData->brightness[y].begin(); x<m_pBufData->brightness[y].end(); ++x) *x = 0;
			for(auto x=m_pBufData->colorData[y].begin(); x<m_pBufData->colorData[y].end(); ++x) *x = 0;
			std::copy(m_dotData[side].brightness[y].begin()+strBeg,m_dotData[side].brightness[y].begin()+strBeg+128-left-right, m_pBufData->brightness[y].begin()+left);
			std::copy(m_dotData[side].colorData[y].begin()+strBeg,m_dotData[side].colorData[y].begin()+strBeg+128-left-right, m_pBufData->colorData[y].begin()+left);
			m_nextScrollTime[side] = m_matrixManager.getTime() + m_scrollSpeed[side];
		}
	}
	m_isSwapped = false;
	return true;
}

void CSentenceLayouter::setScrollSpeed(unsigned int timeMS, bool isUpper){
	m_scrollSpeed[isUpper ? 0:1] = timeMS;
	m_nextScrollTime[isUpper ? 0:1] = m_matrixManager.getTime() + m_scrollSpeed;
}

void CSentenceLayouter::flushProcess(){
	// PWMによって点灯。正直ここはいじらなくても何も支障はないはず
	const int pwmCycle=7;
	int cycleCount=0;
	
	while(!m_terminateFlag){
		if(!m_isSwapped){
			std::swap(m_pUsingData, m_pBufData);
			m_isSwapped = true;
		}
		
		CControl32x16Matrix::TExpressData dd[3];
		for(int i=0; i<3; ++i)	dd[i].resize(128, 0);
		
		for(unsigned int y=0; y<m_pUsingData->brightness.size(); ++y)
		for(unsigned int x=0; x<m_pUsingData->brightness[y].size(); ++x)
		for(int i=0; i<3; ++i){
			const unsigned int pos[] = { x%32u, y+(x/32u)*32u };
			const unsigned int color = m_pUsingData->colorData[y][x];
			unsigned int brightness = (unsigned int)m_pUsingData->brightness[y][x] * ((color >> 8*(2-i)) & 0xFF);
			brightness >>= 8;
			const unsigned int comp = (brightness*(pwmCycle+1)) >> 8;
			//std::cout << "x:" << x << " y:" << y << " val: "  << std::hex << color << std::endl;
			if(cycleCount < comp){
				dd[i][pos[1]] |= (1ull << pos[0]);
				//std::cout << pos[0] << " " << pos[1] << std::endl;
			}
		}
		if(++cycleCount >= pwmCycle) cycleCount = 0;
		m_matrixManager.DataSet(dd[0], dd[1], dd[2]);
		m_matrixManager.TransferToMatrix();
		//break;
	}
}

CSentenceLayouter::~CSentenceLayouter(){
	m_terminateFlag = true;
	m_flushThread.join();
}
