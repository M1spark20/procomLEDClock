#pragma once
#include <vector>

typedef std::vector<std::vector<char>> tMatrixData;

// [act]32x16�̃}�g���N�X�Ǘ��N���X
class CControl32x16Matrix{
//[Pin assign]: (GPIO pin No.)
//	[R0,G0,B0]	: [10, 9,11]
//	[R1,G1,B1]	: [25, 8, 7]
//	[R2,G2,B2]	: [21,20,26]
//	[R3,G3,B3]	: [16,19,13]
//	[A,B]		: [23,24]
//	[CLK,LAT,OE]: [17,27,22]
//	�ڑ����̃q���g: A,B,CLK,LAT,OE,GND�͏�i�E���i�ŋ��ʁB���̕���̂��߂Ƀu���b�h�{�[�h��z�u���Ă���B
//	�s���z��͌�����n�����܂��c�B

public:
	// 32bit�ϐ��B���ォ�牡1���32�h�b�g��3�F�̐F�̓_���L��������B
	// �z�񂪐i�ނɂ�ĉ��ɁA��ԉ��ɂ����炻�̉E��ɁA�Ƃ����`�őJ�ڂ��Ă���
	typedef std::vector<unsigned int> TExpressData;

private:
	static const short Pin_R0  = 10;
	static const short Pin_G0  =  9;
	static const short Pin_B0  = 11;
	static const short Pin_R1  = 25;
	static const short Pin_G1  =  8;
	static const short Pin_B1  =  7;
	static const short Pin_R2  = 21;
	static const short Pin_G2  = 20;
	static const short Pin_B2  = 26;
	static const short Pin_R3  = 16;
	static const short Pin_G3  = 19;
	static const short Pin_B3  = 13;
	static const short Pin_A     = 23;
	static const short Pin_B     = 24;
	static const short Pin_CLK   = 17;
	static const short Pin_LAT   = 27;
	static const short Pin_OE    = 22;
	
	// ������
	bool				 m_nightModeFlag;
	
	// outputData(Red, Green, Blue)
	TExpressData m_redData, m_greenData, m_blueData;
	
	// [act]�����f�[�^����C�ӂ̈ʒu�̓_���̗L��������o��
	// [prm]pData	: ���؃f�[�^ 32bit�ϐ�
	//		pPos	: ���؂��s���ʒu bit�ʒu�ł悢
	//		pInvert	: ���̂Ƃ���g�p�Ȃ�
	int Decode(unsigned int pData, unsigned int pPos, bool pInvert);
	
public:
	// [act]�}�g���N�X�̏��������s��
	// [ret]�������ɐ���������
	bool Init();
	
	// [act]�}�g���N�X�ɕ\������f�[�^���O������ݒ肷��
	// [prm]pRedData	: �ԂɊւ���f�[�^
	//		pGreenData	: �΂Ɋւ���f�[�^
	//		pBlueData	: �Ɋւ���f�[�^
	void DataSet(TExpressData pRedData, TExpressData pGreenData, TExpressData pBlueData);
	
	// [act]�f�[�^���}�g���N�X��1��]������
	// [ret]�]���ɐ���������
	bool TransferToMatrix();
	
	// [act]�v���O�����N������̎�����ms�P�ʂŎ擾����
	// [ret]�N������̎���
	unsigned int getTime();
	
	// [act]���̂Ƃ�����ʂȂ�
	void setNightModeFlag(bool value);
};
