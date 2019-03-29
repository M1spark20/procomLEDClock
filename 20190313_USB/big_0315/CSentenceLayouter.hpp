#pragma once
#include "SStringDots.hpp"
#include <thread>

class CControl32x16Matrix;

class CSentenceLayouter{
	SStringDots			m_dotData;
	SStringDots			m_showData[2];
	SStringDots*			m_pUsingData;
	SStringDots*			m_pBufData;
	bool					m_isSwapped;
	bool					m_terminateFlag;
	unsigned int			m_scrollSpeed;
	unsigned int			m_nextScrollTime;
	int						m_scrollPos;	// base: left edge of matrix
	CControl32x16Matrix&	m_matrixManager;
	std::thread			m_flushThread;
public:
	CSentenceLayouter(CControl32x16Matrix& matrix);
	void setScrollSpeed(unsigned int timeMS);
	void setData(const SStringDots& pData);
	bool refresh();
	// unsyncronized
	void flushProcess();
	~CSentenceLayouter();
};
