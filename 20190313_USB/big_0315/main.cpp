#include "CControl32x16Matrix.hpp"
#include "CSentenceLayouter.hpp"
#include "CStringFormatter.hpp"
#include <fstream>
#include <iostream>
#include <thread>
#include <unistd.h>

int main(int argc, char* argv[]){
	// マトリクス初期化
	CControl32x16Matrix matrix;
	if(!matrix.Init()) return -1;
	//matrix.setNightModeFlag(true);
	
	// 仮ファイル読み込み
	/*SStringDots dotData;
	if(argc<2){
		std::cerr << "please set a text file as a cmd parameter." << std::endl;
	}
	
	std::ifstream ifs(argv[1]);
	if(!ifs) return -1;
	int w,h;
	ifs >> w >> h;
	dotData.brightness.resize(h);
	dotData.colorData.resize(h);
	unsigned int temp;
	for(auto y=0; y<h; ++y){
		dotData.brightness[y].resize(w);
		for(auto x=0; x<w; ++x){
			ifs >> temp;
			dotData.brightness[y][x] = temp & 0xFF;
		}
	}
	for(auto y=0; y<h; ++y){
		dotData.colorData[y].resize(w);
		for(auto x=0; x<w; ++x){
			ifs >> temp;
			dotData.colorData[y][x] = temp & 0xFFFFFF;
		}
	}
	CSentenceLayouter layouter(matrix);
	layouter.setData(dotData);*/
	
	
	// 文字列から表示内容指定(ここの文字を時間によって変えることで情報を生成していく)
	CSentenceLayouter	layouter(matrix);	// レイアウトクラス起動
	CStringFormatter	formatter;			// 文字データ生成クラス起動
	formatter.Init("charactor/m+_1p_th_22w_ff.csv");	// 文字データ読み込み
	layouter.setData(formatter.MakeData("[#FFFFFF]2019/04/01 12:34:56"), false, true);	// 文字読み込み: 例のように時間を指定することで時計になる
	layouter.setData(formatter.MakeData("[#FFFFFF]プロコン部LED時計テスト中！"), true, true);	// 文字読み込み: 情報部例
	
	int changeMode = matrix.getTime() + 60000;
	bool flag=false;
	// 表示内容の更新を行いながら、定期操作が行える
	// ただし、refresh()は現状のように可能な限り絶え間なく呼ばれるようにしてほしい。
	while(layouter.refresh()){
		if(matrix.getTime() >= changeMode){
			flag = !flag;
			matrix.setNightModeFlag(flag);
			changeMode = matrix.getTime() + 120000;
		}
	}
	
	return 0;
}

		
	
/*	unsigned int dot = 1;
	int height = 0;
	CControl64x32Matrix::TExpressData data[3];
	for(int i=0; i<3; ++i){
		data[i].resize(64, 0);
	}
	matrix.DataSet(data[0], data[1], data[2]);
	
	for(int count=0; ; ++count){
		// update
		if(count >= 1){
			for(int i=0; i<3; ++i)
				data[i][height%64] = 0;
			height+=1;
			for(int i=0; i<3; ++i)
				data[i][height%64] = 0xFFFFFFFFFFFFFFFF;
			matrix.DataSet(data[0], data[1], data[2]);
			count = 0;
			
			/*dot = dot << 1;
			if(data == 0){
				data[0][height] = 0;
				if(++height>=16) height=0;
				dot = 1;
			}
			data[0][height] = dot;
			matrix.DataSet(data[0],data[1],data[2]);
			count = 0;
		}
		
		// show
		matrix.DataSet(data[0], data[1], data[2]);
		matrix.TransferToMatrix();
	}*/
