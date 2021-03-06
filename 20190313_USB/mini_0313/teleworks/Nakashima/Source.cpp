#include<iostream>
#include<vector>
#include<fstream>
#include<strstream>
#include<sstream>
#include<string>
#include <cstdlib>
#include <utility>
#include "bmp.h"

#define CSVFILEPATH "./ipag18-2.csv"
#define TXTFILEPATH "./load.csv"
#define MAXID 15306;

using namespace std;
struct WordData
{
	vector<vector<unsigned char>> brightness;//[x][y]
	vector<vector<int>>			  colorData;
};

struct  WordSpec{
	int ID;
	int page;
	int x;
	int y;
	int w;
	int h;
};

class DataBase {
private:
	WordSpec *word;
	vector<CBMPManager::img*> bitmap;
public:
	DataBase(WordSpec*word, vector<CBMPManager::img*> bitmap) {
		this->word = word;
		this->bitmap = bitmap;
	}

	WordSpec getWord(unsigned int id) const {
		return word[id];
	}
	CBMPManager::img* getBitmap(int i) {
		return bitmap[i];
	}
};

class DataBaseManager {
private:
	ifstream peek;
	DataBase*database;
public:
	WordData worddata;
	vector<int> ipag_pattan;
	ifstream ipag[3];
	WordSpec *word;
	CBMPManager bmpManager;
	vector<CBMPManager::img*> bitmap;
	
	void makeDatabase(){
		peek.open(CSVFILEPATH);
		if (peek.fail()) {
			cout << "Error:cannot open file(" << CSVFILEPATH << ")";
			exit(1);
		}
		string str;
		vector<string> strs;
		int max_line=0;
		while (getline(peek, str)) {
			strs.push_back(str);
			max_line++;
		}
		int line = 0;
		for (int i = 0; i < max_line;i++) {
			int pivot = 0;
			int cnt = 0;
			int temp_int[2];
			WordSpec temp_word;
			for (int j = 0; j < (int)strs[i].size(); j++) {
				stringstream temp;
				if (strs[i][j] == ',') {
					temp<<strs[i].substr(pivot,pivot+j);
					if (strs[i][1] == 'T') {
						switch (cnt) {
						case 1:
							temp >> temp_int[0];
							break;
						case 2:
							ipag[temp_int[0]].open(strs[i].substr(pivot, pivot + j), ios::binary);
							CBMPManager::img *temp_img = {};
							char temp_char[30];
							temp >> temp_char;
							bmpManager.ReadBmp(temp_char, temp_img);
							bitmap.emplace_back(temp_img);
							break;
						}
					}
					if (strs[i][1] == 'R') {
						switch (cnt) {
						case 1:
							temp >> temp_word.ID;
							break;
						case 2:
							temp >> temp_word.page;
							break;
						case 3:
							temp >> temp_word.x;
							break;
						case 4:
							temp >> temp_word.y;
							break;
						case 5:
							temp >> temp_word.w;
							break;
						case 6:
							temp >> temp_word.h;
							word[temp_word.ID] = temp_word;
							break;
						}
					}
				}
			}
		}
		database = new DataBase(word,bitmap);
		peek.close();
	}
	DataBase* showDatabase()const { return database; }
};


class Writing {
private:

	//↓この中にデータが入ってます
	vector<WordData> worddata;
	/***************************/

	ifstream file;
	vector<unsigned int> sentence;
	DataBase* database;
	void loadTxt() {
		file.open(TXTFILEPATH);
		if (file.fail()) {
			cout << "Error:cannot open file(" << TXTFILEPATH << ")";
			exit(1);
		}
		string str;
		vector<string> strs;
		int max_line = 0;
		while (getline(file, str)) {
			strs.push_back(str);
			max_line++;
		}
		
		vector<unsigned int> words;
		for (int i = 0; i < max_line; i++) {
			stringstream temp_str;
			unsigned int c;
				temp_str << strs[i];
				temp_str >> c;
				words.emplace_back(c);
		}
		makeMozikodo(words);
	}
	void makeMozikodo(vector<unsigned int> strs) {
		int pivot = 0;
		int cnt = 0;
		unsigned int firstbite = 0;
		for (unsigned int j = 0; j < strs.size(); j++) {
			if (strs[j] == 0x0D) continue;
			if (strs[j] == 0x0A) continue;
			if (strs[j] >= 0x80 && strs[j] <= 0x9F) {
				firstbite = (strs[j] << 8) - 0x8040;
				continue;
			}
			else if (strs[j] >= 0xE0 && strs[j] <= 0xEf) {
				firstbite = (strs[j] << 8) - 0x8040 - 0x4041;
			}
			else {
				sentence.push_back(strs[j] + firstbite);
				firstbite = 0;
			}
		}
	}

	void makeDottoData() {
		for (unsigned int i = 0; i < sentence.size(); i++) {
			WordSpec word = database->getWord(sentence[i]);
			WordData temp;	
			for (int y = 0; y < word.h; y++) {
				vector<unsigned char > temp_w;
				vector<int> temp_c;
				for (int x = 0; x < word.w; x++) {
					temp_w.emplace_back((database->getBitmap(database->getWord(sentence[i]).page)->data[y + word.y][word.x + x].r << 4)
						+ (database->getBitmap(database->getWord(sentence[i]).page)->data[y + word.y][word.x + x].g << 2)
						+ (database->getBitmap(database->getWord(sentence[i]).page)->data[y + word.y][word.x + x].b));
					temp_c.emplace_back((database->getBitmap(database->getWord(sentence[i]).page)->data[y + word.y][word.x + x].r << 4)
						+ (database->getBitmap(database->getWord(sentence[i]).page)->data[y + word.y][word.x + x].g << 2)
						+ (database->getBitmap(database->getWord(sentence[i]).page)->data[y + word.y][word.x + x].b));
				}
				temp.brightness.emplace_back(temp_w);
				temp.colorData.emplace_back(temp_c);
			}
			worddata.emplace_back(temp);
		}
		
	}
public:

	Writing(DataBase*database) {
		this->database = database;
	}
	void Execute() {
		loadTxt();
		makeDottoData();
	}
};

int main(){
	DataBaseManager* databasemanager = new DataBaseManager();

	databasemanager->makeDatabase();
	Writing *writing = new Writing(databasemanager->showDatabase());
	return 0;
}