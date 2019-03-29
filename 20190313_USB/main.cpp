#include "CControl32x16Matrix.hpp"

int main(){
	CControl32x16Matrix matrix;
	if(!matrix.Init()) return -1;
	
	unsigned int dot = 1;
	int height = 0;
	CControl32x16Matrix::TExpressData data[3];
	const unsigned int testData[] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000 };
	for(int i=0; i<3; ++i){
		data[i].resize(64);
		//for(int j=0; j<64; ++j) data[i][j]=testData[i];
		//matrix.DataSet(data[0],data[1],data[2]);
	}
	
	bool eraseFlag = false;
	unsigned int pwmTest = 0;
	for(int count=0; ; ++count){
		// update
		/*if(count >= 7){
			count=0; pwmTest = 0;
		}
		pwmTest = pwmTest << 4 | 0xF;
		unsigned int buf = pwmTest;
		//for(int i=0; i<4; ++i) buf |= (~pwmTest << 8);
		for(int line=0; line<64; ++line){
			unsigned int cl = (line / 8) % 8;
			unsigned int mask = 0x1;
			for(int i=0; i<3; ++i){
				const unsigned int gradFlag = 1<<i, role=(line/2)%4;
				if(cl & gradFlag) data[i][line] = buf;
				else {
					data[i][line] = (role&mask) ? 0xFFFFFFFF : 0x00000000;
					mask<<=1;
				}
			}
			
		}
		matrix.DataSet(data[0],data[1],data[2]);*/
		
		if(count >= 20){
			data[2].swap(data[1]);
			data[1] = CControl32x16Matrix::TExpressData(data[0]);
			dot = dot << 1;
			if(dot == 0){
				data[0][height] = 0;
				if(height==63) height=0;
				else if(height>=48) height-=47;
				else height += 16;
				
				dot = 1;
				eraseFlag = true;
			}
			data[0][height] = dot;
			matrix.DataSet(data[0],data[1],data[2]);
			count = 0;
		}
		
		// show
		matrix.TransferToMatrix();
	}
	return 0;
}
