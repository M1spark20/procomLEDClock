#pragma once
#include "SStringDots.hpp"
#include <thread>

class CControl32x16Matrix;

// [act]�h�b�g�f�[�^���}�g���N�X�ɓ]������
class CSentenceLayouter{
	SStringDots				m_dotData[2];		// ��������h�b�g�ɂ����f�[�^[��i(y=0�`), ���i(y=16�`)]
	
	SStringDots				m_showData[2];		// �}�g���N�X�ɕ\������f�[�^�F�_�u���o�b�t�@�ɂ�荂���� �Е��͕\���p�E�Е��͍X�V�p
	SStringDots*			m_pUsingData;		// ���ݕ\���p�̃}�g���N�X�f�[�^�i�[���ۑ�
	SStringDots*			m_pBufData;			// ���ݍX�V�p�̃}�g���N�X�f�[�^�i�[���ۑ�
	
	std::thread				m_flushThread;		// �}�g���N�X�̍X�V����񏈗��ōs�����߂̕ϐ��B�������邱�ƂŃX���[�Y�ɉ�ʂ̐؂�ւ����s��(std::thread�Ō���)
	bool					m_isSwapped;		// �f�[�^�̍X�V���s���Ă���A�_�u���o�b�t�@�̐؂�ւ����s��ꂽ��
	bool					m_terminateFlag;	// �v���O�������I���������ǂ���(�}�g���N�X�ւ̕\������񏈗��ōs�����߂ɑ���)
	
	unsigned int			m_scrollSpeed[2];	// �X�N���[�����x: 1�h�b�g�ړ�����̂ɉ�ms�����邩���i�[[��i(y=0�`), ���i(y=16�`)]
	unsigned int			m_nextScrollTime[2];// �X�N���[�����x�ɉ����Ď��ɃX�N���[�����鎞�Ԃ��i�[[��i(y=0�`), ���i(y=16�`)]
	int						m_scrollPos[2];		// ���݂̃X�N���[���ʒu���i�[[��i(y=0�`), ���i(y=16�`)] �l���[ -127, m_dotData[*][0].size() )
	CControl32x16Matrix&	m_matrixManager;	// �}�g���N�X�N���X�̎Q��(�C���X�^���X��main�ɂĐ�������)
public:
	// [act]�e�ϐ��̏������E���񏈗��̏��������s��
	// [prm]matrix: �}�g���N�X�Ǘ��N���X�̃C���X�^���X
	CSentenceLayouter(CControl32x16Matrix& matrix);
	
	// [act]�X�N���[�����x�̍X�V���s��
	// [prm]timeMS	: �X�V�Ԋu���w�� [ms]�P��
	//		isUpper	: �X�V����f�[�^�͏�i�����w��(true:��i, false:���i)
	void setScrollSpeed(unsigned int timeMS, bool isUpper);
	
	// [act]�}�g���N�X�ɕ\������h�b�g�f�[�^�̍X�V���s��
	// [prm]pData	: �V�����h�b�g�f�[�^
	//		isUpper	: �X�V����f�[�^�͏�i�����w��(true:��i, false:���i)
	//		isReset	: �X�N���[���ʒu�����Z�b�g���邩(true:����, false:���Ȃ�)
	void setData(const SStringDots& pData, bool isUpper, bool isReset);
	
	// [act]�}�g���N�X�ɕ\������f�[�^�̍X�V���s��
	//		�ǂ��炩�Ƃ����ƁA���setData�֐��Ŏw�肵���f�[�^�𔽉f����E�X�N���[�����s�����o
	// [ret]�X�V�ɐ���������
	bool refresh();
	
	// [act]�}�g���N�X�Ƀf�[�^�𗬂�����ŕ\������B���񏈗��ŉ�]����֐��B
	void flushProcess();
	
	// [act]���񏈗��̌�Еt��������
	~CSentenceLayouter();
};
