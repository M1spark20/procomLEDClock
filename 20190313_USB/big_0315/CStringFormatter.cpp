#include "CStringFormatter.hpp"
#include <fstream>
#include <iostream>

// please do not include \n in pData.
std::vector<std::string> CStringFormatter::SplitStr(const std::string pData, char delim){
	std::stringstream sstr(pData);
	std::vector<std::string> ans;
	// 分割はstd::getlineを使う。検索すれば機能がでてくるよ。
	for (std::string section; std::getline(sstr, section, delim); ) {
		ans.push_back(section);
	}
	return ans;
}

bool CStringFormatter::Init(std::string pFilePath){
	std::ifstream peek(pFilePath);	// ファイル読み込みストリーム
	CBMPManager bmpManager;			// bmp.hのインスタンス
	if (peek.fail()) {
		cout << "Error - cannot open charactor definition data: " << pFilePath << std::endl;
		return false;
	}
	// 1行ずつ読み込んで操作する。
	for (std::string lineStr; std::getline(peek, lineStr); ) {
		// データをカンマで区切る
		std::vector<std::String> stringArr = SplitStr(lineStr, ',');
		if(stringArr.empty()) continue;
		if(stringArr[0] == "#T"){
			// imageFile path
			// 画像を読み込む
			int ID; ConvStr(stringArr[1], ID);
			CBMPManager::img* imgData = new CBMPManager::img;
			bmpManager.ReadBmp(stringArr[2].c_str(), imgData);
			// std::mapの生成方法にのっとってデータを追加
			m_imgData.insert(std::make_pair(ID, imgData));
		}
		if(stringArr[0] == "#R"){
			// charactor positions
			// 文字情報を読み込む
			SCharData charData;
			int ID; ConvStr(stringArr[1], ID);
			ConvStr(stringArr[2], charData.page);
			ConvStr(stringArr[3], charData.x);
			ConvStr(stringArr[4], charData.y);
			ConvStr(stringArr[5], charData.w);
			ConvStr(stringArr[6], charData.h);
			// std::mapの生成方法にのっとってデータを追加
			m_wordData.insert(std::make_pair(ID, charData));
		}
	}
	return true;
}

SStringDots CStringFormatter::MakeData(std::string pString){
	// 生成するドットデータの最大サイズ[x,y]
	std::pair<int,int> maxSize(0,0);
	
	// 使用するデータのポインタを格納する変数(m_wordDataの1要素のアドレスを格納)
	//   1. 使用する文字を1文字ずつ確認し、戻り値変数の幅と高さを決定する
	//		その際にここにデータのポインタを格納することで次に再検索しないようにする
	//   2. ポインタに格納された文字を1つずつ読み込んでいく
	std::vector<SWordData*> strs;
	
	/* measure Size */{
		std::stringstream sstr(pString);
		// process each lines
		for (std::string lineStr; std::getline(sstr, lineStr); ) {
			// その行が条件を満たさない場合処理しない
			if(lineStr.size()<9) continue;
			if(lineStr[0] != '[') continue;
			if(lineStr[8] != ']') continue;
			
			// 1byteずつ操作
			const unsigned char* data = lineStr.substr(9).c_str();
			unsigned char lastStr = 0;
			for(int i=0; data[i]; ++i){
				unsigned int segment;
				if(lastStr != 0){
					// 2nd charactor
					// 2バイト文字の下のバイトを読み込む
					segment = lastStr << 8 | data[i];
					if(lastStr&0xE0) segment -= 0x4041;
					lastStr = 0;
				} else {
					// 1st charactor
					if(!(data[i] & 0x80) || (data[i]>160 && data[i]<224)){
						segment = data[i];
					} else {
						// 2バイト文字の場合ここに流れる
						lastStr = data[i];
						continue;
					}
				}
				// 文字列を取り出して保存・長さを特定
				const SWordData* ptr = &m_wordData[segment];
				maxSize.first += ptr->w;
				maxSize.second = (maxSize.second < ptr->h) ? maxSize.second : ptr->h;
				strs.push_back(ptr);
			}
		}
	}
	
	// データ生成(改善可能)
	SStringDots ans;
	ans.brightness.resize(maxSize.second);
	ans.colorData .resize(maxSize.second);
	
	for(int y=0; y<maxSize.second; ++y){
		ans.brightness[y].resize(maxSize.first);
		ans.colorData [y].resize(maxSize.first);
		int x=0;
		for(auto ptr : strs)
		for(int px=0; px<ptr->w; ++px){
			// 画像の画素から色情報を指定
			ans.brightness[y][x  ] = m_imgData[ptr->page]->data[y+ptr->y][px+ptr->x].g;	// 画像から色を取得
			ans.colorData [y][x++] = 0xFFFFFF;	// とりあえず白(#以降の6文字から取得する)
		}
	}
	return ans;
}

CStringFormatter::~CStringFormatter(){
	for(_ptr : m_imgData){
		delete _ptr;
	}
}
