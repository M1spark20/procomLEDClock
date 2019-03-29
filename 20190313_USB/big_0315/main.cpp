#include "CControl32x16Matrix.hpp"
#include "CSentenceLayouter.hpp"
#include <fstream>
#include <iostream>
#include <thread>
#include <unistd.h>

int main(int argc, char* argv[]){
	CControl32x16Matrix matrix;
	if(!matrix.Init()) return -1;
	//matrix.setNightModeFlag(true);
	
	SStringDots dotData;
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
	layouter.setData(dotData);
	
	int changeMode = matrix.getTime() + 60000;
	bool flag=false;
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
