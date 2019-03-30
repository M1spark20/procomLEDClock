#include "CStringFormatter.hpp"
#include <fstream>
#include <iostream>

// please do not include \n in pData.
std::vector<std::string> CStringFormatter::SplitStr(const std::string pData, char delim){
	std::stringstream sstr(pData);
	std::vector<std::string> ans;
	// ������std::getline���g���B��������΋@�\���łĂ����B
	for (std::string section; std::getline(sstr, section, delim); ) {
		ans.push_back(section);
	}
	return ans;
}

bool CStringFormatter::Init(std::string pFilePath){
	std::ifstream peek(pFilePath);	// �t�@�C���ǂݍ��݃X�g���[��
	CBMPManager bmpManager;			// bmp.h�̃C���X�^���X
	if (peek.fail()) {
		cout << "Error - cannot open charactor definition data: " << pFilePath << std::endl;
		return false;
	}
	// 1�s���ǂݍ���ő��삷��B
	for (std::string lineStr; std::getline(peek, lineStr); ) {
		// �f�[�^���J���}�ŋ�؂�
		std::vector<std::String> stringArr = SplitStr(lineStr, ',');
		if(stringArr.empty()) continue;
		if(stringArr[0] == "#T"){
			// imageFile path
			// �摜��ǂݍ���
			int ID; ConvStr(stringArr[1], ID);
			CBMPManager::img* imgData = new CBMPManager::img;
			bmpManager.ReadBmp(stringArr[2].c_str(), imgData);
			// std::map�̐������@�ɂ̂��Ƃ��ăf�[�^��ǉ�
			m_imgData.insert(std::make_pair(ID, imgData));
		}
		if(stringArr[0] == "#R"){
			// charactor positions
			// ��������ǂݍ���
			SCharData charData;
			int ID; ConvStr(stringArr[1], ID);
			ConvStr(stringArr[2], charData.page);
			ConvStr(stringArr[3], charData.x);
			ConvStr(stringArr[4], charData.y);
			ConvStr(stringArr[5], charData.w);
			ConvStr(stringArr[6], charData.h);
			// std::map�̐������@�ɂ̂��Ƃ��ăf�[�^��ǉ�
			m_wordData.insert(std::make_pair(ID, charData));
		}
	}
	return true;
}

SStringDots CStringFormatter::MakeData(std::string pString){
	// ��������h�b�g�f�[�^�̍ő�T�C�Y[x,y]
	std::pair<int,int> maxSize(0,0);
	
	// �g�p����f�[�^�̃|�C���^���i�[����ϐ�(m_wordData��1�v�f�̃A�h���X���i�[)
	//   1. �g�p���镶����1�������m�F���A�߂�l�ϐ��̕��ƍ��������肷��
	//		���̍ۂɂ����Ƀf�[�^�̃|�C���^���i�[���邱�ƂŎ��ɍČ������Ȃ��悤�ɂ���
	//   2. �|�C���^�Ɋi�[���ꂽ������1���ǂݍ���ł���
	std::vector<SWordData*> strs;
	
	/* measure Size */{
		std::stringstream sstr(pString);
		// process each lines
		for (std::string lineStr; std::getline(sstr, lineStr); ) {
			// ���̍s�������𖞂����Ȃ��ꍇ�������Ȃ�
			if(lineStr.size()<9) continue;
			if(lineStr[0] != '[') continue;
			if(lineStr[8] != ']') continue;
			
			// 1byte������
			const unsigned char* data = lineStr.substr(9).c_str();
			unsigned char lastStr = 0;
			for(int i=0; data[i]; ++i){
				unsigned int segment;
				if(lastStr != 0){
					// 2nd charactor
					// 2�o�C�g�����̉��̃o�C�g��ǂݍ���
					segment = lastStr << 8 | data[i];
					if(lastStr&0xE0) segment -= 0x4041;
					lastStr = 0;
				} else {
					// 1st charactor
					if(!(data[i] & 0x80) || (data[i]>160 && data[i]<224)){
						segment = data[i];
					} else {
						// 2�o�C�g�����̏ꍇ�����ɗ����
						lastStr = data[i];
						continue;
					}
				}
				// ����������o���ĕۑ��E���������
				const SWordData* ptr = &m_wordData[segment];
				maxSize.first += ptr->w;
				maxSize.second = (maxSize.second < ptr->h) ? maxSize.second : ptr->h;
				strs.push_back(ptr);
			}
		}
	}
	
	// �f�[�^����(���P�\)
	SStringDots ans;
	ans.brightness.resize(maxSize.second);
	ans.colorData .resize(maxSize.second);
	
	for(int y=0; y<maxSize.second; ++y){
		ans.brightness[y].resize(maxSize.first);
		ans.colorData [y].resize(maxSize.first);
		int x=0;
		for(auto ptr : strs)
		for(int px=0; px<ptr->w; ++px){
			// �摜�̉�f����F�����w��
			ans.brightness[y][x  ] = m_imgData[ptr->page]->data[y+ptr->y][px+ptr->x].g;	// �摜����F���擾
			ans.colorData [y][x++] = 0xFFFFFF;	// �Ƃ肠������(#�ȍ~��6��������擾����)
		}
	}
	return ans;
}

CStringFormatter::~CStringFormatter(){
	for(_ptr : m_imgData){
		delete _ptr;
	}
}
