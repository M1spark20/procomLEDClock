#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "bmp.h"
#include "SStringDots.hpp"

class CStringFormatter{
	// internal stracture
	struct SCharData {
		// int ID; // Map's key
		int page;
		int x;
		int y;
		int w;
		int h;
	}
	
	std::map<unsigned int, SCharData>			m_wordData;
	std::map<unsigned int, CBMPManager::img*>	m_imgData;
	
	std::vector<std::string>	SplitStr(const std::string pData, char delim);
	template<class T> void		ConvStr(const std::string pSrc, T& pDst){
		std::istringstream ist(pSrc);
		ist >> T;
	}

public:
	bool		Init(std::string pFilePath);
	SStringDots	MakeData(std::string pString);
	~CStringFormatter();
};
