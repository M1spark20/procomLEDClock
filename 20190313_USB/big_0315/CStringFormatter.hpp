#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "bmp.h"
#include "SStringDots.hpp"

// [act]文字列からドットデータを生成する。
//		文字の格納された画像を読み込み、ドットの輝度値と色情報を決定する。
//		表示内容とマトリクス表示との橋渡しになる部分。
class CStringFormatter{
	// internal stracture
	// [act]各文字に関するcsvからの読み取り情報を格納する。vector配列として文字データが格納されたcsvの読み取り結果となる。
	struct SCharData {
		// int ID;	// 文字列のID(ex: 半角空白=0x20) -> std::mapのKeyとして保存する(std::mapについては調べてみてほしい)
		int page;	// 文字が格納された画像の番号
		int x;
		int y;
		int w;
		int h;
	}
	
	std::map<unsigned int, SCharData>			m_wordData;		// Key: 文字ID, value: その文字の読み取り方法
	std::map<unsigned int, CBMPManager::img*>	m_imgData;		// Key: 画像ID, value: その画像のimg構造体(ポインタ)->bmp.hで処理する
	
	// [act]delimで指定した文字によりpDataを分割し、分割結果を返す。csvで、カンマで区切って文字列を管理するとかがこの関数でできる。
	// [prm]pData	: 分割を行う文字列(※改行を含まない文字列を入力すること! 一行ずつデータを読み込んで入れるとか対策がとれる。)
	//		delim	: 分割区切り文字(ex: '\t', ',' etc...)
	// [ret]分割した結果がstd::stringのvector配列で出力される(区切り文字を含まない)
	std::vector<std::string>	SplitStr(const std::string pData, char delim);
	
	// [act]文字列を任意の型に変換する。変換する型は2番目の引数に指定した変数の型から自動的に決定される(cf:テンプレート関数)
	// [prm]pSrc	: 変換を行いたい文字列
	//		pDst	: 変換したデータの格納先: 指定した変数の型に変換される 参照渡しなので結果もここに指定した変数に格納される
	template<class T> void		ConvStr(const std::string pSrc, T& pDst){
		std::istringstream ist(pSrc);
		ist >> T;
	}

public:
	// [act]文字に関するcsvを読み込んで生成の準備をする。この関数はかなり処理が重いので注意。
	// [prm]pFilePath	: 文字に関するcsv保存先
	// [ret]生成の準備が行えたかどうか
	bool		Init(std::string pFilePath);
	
	// [act]指定した文字列からドットデータを生成する。
	//		現在この関数は処理概要を書いてみたもののノーテスト。コンパイルしながらバグつぶしを頂けると嬉しい。
	// [prm]pString	: 変換する文字列(format.tsv記載のもの, あるいは色と表示内容を格納したもの。ex:[#FFFFFF]ここが表示内容)
	// [ret]文字列から生成したドットデータ
	SStringDots	MakeData(std::string pString);
	
	// [act]m_imgDataが持つすべての画像データをメモリから削除(delete)する。
	~CStringFormatter();
};
