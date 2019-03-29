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
#define TXTFILEPATH "./correctResult.txt"
#define OUTPUTFILE  "./"
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
	vector<CBMPManager::img> bitmap;
public:
	DataBase(WordSpec*word, vector<CBMPManager::img> bitmap) {
		this->word = word;
		this->bitmap = bitmap;
	}

	WordSpec getWord(unsigned int id) const {

		return word[id];
	}
	CBMPManager::img getBitmap(int i) {
		return bitmap[i];
	}
};

class DataBaseManager {
private:
	ifstream peek;
	DataBase*database;
public:
	DataBaseManager() {
		database = nullptr;
	}
	virtual ~DataBaseManager() {
		deleteDatabase();
	}
	void deleteDatabase() {
		if (database) {
			delete database;
		}
	}

	static WordData worddata;
	static vector<int> ipag_pattan;
	ifstream ipag[3];
	WordSpec *word;
	CBMPManager bmpManager;
	vector<CBMPManager::img> bitmap;
	
	void makeDatabase(){
		peek.open(CSVFILEPATH);
		if (peek.fail()) {
			cout << "Error:cannot open file(" << CSVFILEPATH << ")";
			exit(1);
		}
		static string str;
		static vector<string> strs;
		static int max_line=0;
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
				if (strs[i][j] == ','||j==strs[i].size()-1) {
					temp<<strs[i].substr(pivot,j);
					cout << "***" << strs[i].substr(pivot, j)<<endl;
					if (strs[i][1] == 'T') {
						switch (cnt) {
						case 1:
							temp >> temp_int[0];
							break;
						case 2:
							ipag[temp_int[0]].open(strs[i].substr(pivot,j), ios::binary);
							CBMPManager::img temp_img;
							char temp_char[20];
							temp >> temp_char;
							cout << temp_char;
							bmpManager.ReadBmp(temp_char, &temp_img);
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
					pivot = j + 1;
					cnt++;
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

	//Å´Ç±ÇÃíÜÇ…ÉfÅ[É^Ç™ì¸Ç¡ÇƒÇ‹Ç∑
	vector<WordData> worddata;
	/***************************/
	vector<pair<unsigned int, unsigned char>> words;
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
		static int max_line = 0;
		while (getline(file, str)) {
			strs.push_back(str);
			max_line++;
		}
		
		for (int i = 1; i < max_line; i++) {
			stringstream temp_str;
			pair<unsigned int,unsigned char> c;
				temp_str << strs[i].substr(2,7);
				temp_str >> c.first;
				temp_str << strs[i].substr(9, strs[i].size()-1);
				temp_str >> c.second;
				words.emplace_back(c);
		}
	}
	void makeMozikodo() {
		static int pivot = 0;
		static int cnt = 0;
		static unsigned int firstbite = 0;
		for (unsigned int j = 0; j < words.size(); j++) {
			if (words[j].second == 0x0D) continue;
			if (words[j].second == 0x0A) continue;
			if (words[j].second >= 0x80 && words[j].second <= 0x9F) {
				firstbite = (words[j].second << 8) - 0x8040;
				continue;
			}
			else if (words[j].second >= 0xE0 && words[j].second <= 0xEf) {
				firstbite = (words[j].second << 8) - 0x8040 - 0x4041;
			}
			else {
				sentence.push_back(words[j].second + firstbite);
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
					temp_w.emplace_back(database->getBitmap(word.page).data[word.y + y][word.x + x].g);
					temp_c.emplace_back(words[i].second);
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
		makeMozikodo();
		makeDottoData();

	}
};

int main(){
	DataBaseManager* databasemanager = new DataBaseManager();
	databasemanager->makeDatabase();
	Writing *writing = new Writing(databasemanager->showDatabase());
	writing->Execute();
	delete writing;
	delete databasemanager;
	return 0;
}