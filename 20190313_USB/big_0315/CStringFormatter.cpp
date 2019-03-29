#include "CStringFormatter.cpp"
#include <fstream>
#include <iostream>

// please do not include \n in pData.
std::vector<std::string> CStringFormatter::SplitStr(const std::string pData, char delim){
	std::stringstream sstr(pData);
	std::vector<std::string> ans;
	for (std::string section; std::getline(sstr, section, delim); ) {
		ans.push_back(section);
	}
	return ans;
}

bool CStringFormatter::Init(std::string pFilePath){
	std::ifstream peek(pFilePath);
	CBMPManager bmpManager;
	if (peek.fail()) {
		cout << "Error - cannot open charactor definition data: " << pFilePath << std::endl;
		return false;
	}
	// process each lines
	for (std::string lineStr; std::getline(peek, lineStr); ) {
		std::vector<std::String> stringArr = SplitStr(lineStr, ',');
		if(stringArr.empty()) continue;
		if(stringArr[0] == "#T"){
			// imageFile path
			int ID; ConvStr(stringArr[1], ID);
			CBMPManager::img* imgData = new CBMPManager::img;
			bmpManager.ReadBmp(stringArr[2].c_str(), imgData);
			m_imgData.insert(std::make_pair(ID, imgData));
		}
		if(stringArr[0] == "#R"){
			// charactor positions
			SCharData charData;
			int ID; ConvStr(stringArr[1], ID);
			ConvStr(stringArr[2], charData.page);
			ConvStr(stringArr[3], charData.x);
			ConvStr(stringArr[4], charData.y);
			ConvStr(stringArr[5], charData.w);
			ConvStr(stringArr[6], charData.h);
			m_wordData.insert(std::make_pair(ID, charData));
		}
	}
	return true;
}

SStringDots CStringFormatter::MakeData(std::string pString){
	std::pair<int,int> maxSize(0,0);
	std::vector<SWordData*> strs;
	/* measure Size */{
	std::stringstream sstr(pString);
		// process each lines
		for (std::string lineStr; std::getline(sstr, lineStr); ) {
			if(lineStr.empty()) continue;
			if(lineStr[0] != '[') continue;
			const char* proc = lineStr.substr(9).c_str();
			
			std::vector<std::String> stringArr = SplitStr(lineStr, '\t');
			if(stringArr.empty()) continue;
		}
	}
}

CStringFormatter::~CStringFormatter(){
	for(_ptr : m_imgData){
		delete _ptr;
	}
}
