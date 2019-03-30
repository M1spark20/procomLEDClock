#pragma once
#include <vector>

// [act]表示文字の輝度値(brightness)・色情報(colorData)を格納する。
//		実際の表示色は各数値の乗算によって決まる。(参考：乗算ブレンド)
struct SStringDots{
	std::vector<std::vector<unsigned char>> brightness;
	std::vector<std::vector<unsigned int >> colorData;
};
