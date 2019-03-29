#include "CSentenceLayouter.hpp"
#include "CControl32x16Matrix.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

CSentenceLayouter::CSentenceLayouter(CControl32x16Matrix& matrix) : m_matrixManager(matrix){
	m_showData[0].brightness.resize(32);
	m_showData[0].colorData.resize(32);
	m_terminateFlag = false;
	for(auto i=0; i<m_showData[0].brightness.size(); ++i){
		m_showData[0].brightness[i].resize(32*4);
		m_showData[0].colorData[i].resize(32*4);
	}
	m_showData[1] = SStringDots(m_showData[0]);
	m_flushThread = std::thread(&CSentenceLayouter::flushProcess, this);
	m_scrollSpeed = 20;
	m_nextScrollTime = m_matrixManager.getTime() + m_scrollSpeed;
	m_pUsingData = &m_showData[0]; m_pBufData = &m_showData[1];
	
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(3, &cpuset);
	int rc = pthread_setaffinity_np(m_flushThread.native_handle(), sizeof(cpu_set_t), &cpuset);
	//std::cout << rc << std::endl;
}

void CSentenceLayouter::setData(const SStringDots& pData){
	m_dotData = SStringDots(pData);
	m_scrollPos = -127;	// -127
	m_nextScrollTime = m_matrixManager.getTime();
	//std::cout << pData.brightness.size() << "," << pData.brightness[0].size() << std::endl;
}

bool CSentenceLayouter::refresh(){
	if(m_dotData.brightness.size() == 0) return false;
	if(m_matrixManager.getTime() < m_nextScrollTime) return true;
	if(++m_scrollPos >= (signed int)m_dotData.brightness[0].size()) m_scrollPos = -127;
	const int left  = m_scrollPos < 0 ? -m_scrollPos : 0;
	const int right = m_scrollPos + 128 >= m_dotData.brightness[0].size() ?
		m_scrollPos + 128 - m_dotData.brightness[0].size() : 0;
	const int strBeg = m_scrollPos < 0 ? 0 : m_scrollPos;
	//std::cout << m_nextScrollTime << " " << m_scrollPos << " " << left << " " << right << " " << strBeg << std::endl;
	for(auto y=0; y<m_dotData.brightness.size(); ++y){
		for(auto x=m_pBufData->brightness[y].begin(); x<m_pBufData->brightness[y].end(); ++x) *x = 0;
		for(auto x=m_pBufData->colorData[y].begin(); x<m_pBufData->colorData[y].end(); ++x) *x = 0;
		for(auto x=m_pBufData->brightness[y+16].begin(); x<m_pBufData->brightness[y+16].end(); ++x) *x = 0;
		for(auto x=m_pBufData->colorData[y+16].begin(); x<m_pBufData->colorData[y+16].end(); ++x) *x = 0;
		std::copy(m_dotData.brightness[y].begin()+strBeg,m_dotData.brightness[y].begin()+strBeg+128-left-right, m_pBufData->brightness[y].begin()+left);
		std::copy(m_dotData.colorData[y].begin()+strBeg,m_dotData.colorData[y].begin()+strBeg+128-left-right, m_pBufData->colorData[y].begin()+left);
		std::copy(m_dotData.brightness[y].begin()+strBeg,m_dotData.brightness[y].begin()+strBeg+128-left-right, m_pBufData->brightness[y+16].begin()+left);
		std::copy(m_dotData.colorData[y].begin()+strBeg,m_dotData.colorData[y].begin()+strBeg+128-left-right, m_pBufData->colorData[y+16].begin()+left);
	}
	for(auto y=0; y<m_dotData.colorData.size(); ++y){
	}
	m_isSwapped = false;
	m_nextScrollTime = m_matrixManager.getTime() + m_scrollSpeed;
	return true;
}

void CSentenceLayouter::flushProcess(){
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
