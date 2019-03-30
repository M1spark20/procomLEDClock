#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "bmp.h"
#include "SStringDots.hpp"

// [act]�����񂩂�h�b�g�f�[�^�𐶐�����B
//		�����̊i�[���ꂽ�摜��ǂݍ��݁A�h�b�g�̋P�x�l�ƐF�������肷��B
//		�\�����e�ƃ}�g���N�X�\���Ƃ̋��n���ɂȂ镔���B
class CStringFormatter{
	// internal stracture
	// [act]�e�����Ɋւ���csv����̓ǂݎ������i�[����Bvector�z��Ƃ��ĕ����f�[�^���i�[���ꂽcsv�̓ǂݎ�茋�ʂƂȂ�B
	struct SCharData {
		// int ID;	// �������ID(ex: ���p��=0x20) -> std::map��Key�Ƃ��ĕۑ�����(std::map�ɂ��Ă͒��ׂĂ݂Ăق���)
		int page;	// �������i�[���ꂽ�摜�̔ԍ�
		int x;
		int y;
		int w;
		int h;
	}
	
	std::map<unsigned int, SCharData>			m_wordData;		// Key: ����ID, value: ���̕����̓ǂݎ����@
	std::map<unsigned int, CBMPManager::img*>	m_imgData;		// Key: �摜ID, value: ���̉摜��img�\����(�|�C���^)->bmp.h�ŏ�������
	
	// [act]delim�Ŏw�肵�������ɂ��pData�𕪊����A�������ʂ�Ԃ��Bcsv�ŁA�J���}�ŋ�؂��ĕ�������Ǘ�����Ƃ������̊֐��łł���B
	// [prm]pData	: �������s��������(�����s���܂܂Ȃ����������͂��邱��! ��s���f�[�^��ǂݍ���œ����Ƃ��΍􂪂Ƃ��B)
	//		delim	: ������؂蕶��(ex: '\t', ',' etc...)
	// [ret]�����������ʂ�std::string��vector�z��ŏo�͂����(��؂蕶�����܂܂Ȃ�)
	std::vector<std::string>	SplitStr(const std::string pData, char delim);
	
	// [act]�������C�ӂ̌^�ɕϊ�����B�ϊ�����^��2�Ԗڂ̈����Ɏw�肵���ϐ��̌^���玩���I�Ɍ��肳���(cf:�e���v���[�g�֐�)
	// [prm]pSrc	: �ϊ����s������������
	//		pDst	: �ϊ������f�[�^�̊i�[��: �w�肵���ϐ��̌^�ɕϊ������ �Q�Ɠn���Ȃ̂Ō��ʂ������Ɏw�肵���ϐ��Ɋi�[�����
	template<class T> void		ConvStr(const std::string pSrc, T& pDst){
		std::istringstream ist(pSrc);
		ist >> T;
	}

public:
	// [act]�����Ɋւ���csv��ǂݍ���Ő����̏���������B���̊֐��͂��Ȃ菈�����d���̂Œ��ӁB
	// [prm]pFilePath	: �����Ɋւ���csv�ۑ���
	// [ret]�����̏������s�������ǂ���
	bool		Init(std::string pFilePath);
	
	// [act]�w�肵�������񂩂�h�b�g�f�[�^�𐶐�����B
	//		���݂��̊֐��͏����T�v�������Ă݂����̂̃m�[�e�X�g�B�R���p�C�����Ȃ���o�O�Ԃ��𒸂���Ɗ������B
	// [prm]pString	: �ϊ����镶����(format.tsv�L�ڂ̂���, ���邢�͐F�ƕ\�����e���i�[�������́Bex:[#FFFFFF]�������\�����e)
	// [ret]�����񂩂琶�������h�b�g�f�[�^
	SStringDots	MakeData(std::string pString);
	
	// [act]m_imgData�������ׂẲ摜�f�[�^������������폜(delete)����B
	~CStringFormatter();
};
