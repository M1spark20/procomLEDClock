#pragma once
#include <vector>

// [act]�\�������̋P�x�l(brightness)�E�F���(colorData)���i�[����B
//		���ۂ̕\���F�͊e���l�̏�Z�ɂ���Č��܂�B(�Q�l�F��Z�u�����h)
struct SStringDots{
	std::vector<std::vector<unsigned char>> brightness;
	std::vector<std::vector<unsigned int >> colorData;
};
