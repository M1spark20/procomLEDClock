/*****************************************************************************/
/*                                                                           */
/*     bmp.h: bmp ファイル処理のライブラリのためのヘッダファイル             */
/*                                                                           */
/*             Kazutoshi Ando (Shizuoka Univ.)                               */
/*                                                                           */
/*                  Ver. 2004.08.18                                          */
/*                  Ver. 2004.08.17                                          */
/*                  Ver. 2003.11.04                                          */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HEADERSIZE   54               /* ヘッダのサイズ 54 = 14 + 40         */
#define PALLETSIZE 1024               /* パレットのサイズ                    */
#define MAXWIDTH   1000               /* 幅(pixel)の上限                     */
#define MAXHEIGHT  1000               /* 高さ(pixel) の上限                  */

/* x と y の交換のための マクロ関数 */
#define SWAP(x,y) {typeof(x) temp; temp=x; x=y; y=temp;}

class CBMPManager {
private:
	unsigned char Bmp_headbuf[HEADERSIZE];/* ヘッダを格納するための変数          */
	unsigned char Bmp_Pallet[PALLETSIZE]; /* カラーパレットを格納                */

	char Bmp_type[2];                     /* ファイルタイプ "BM"                 */
	unsigned long Bmp_size;               /* bmpファイルのサイズ (バイト)        */
	unsigned int Bmp_info_header_size; /* 情報ヘッダのサイズ = 40             */
	unsigned int Bmp_header_size;      /* ヘッダサイズ = 54*/
	long Bmp_height;                      /* 高さ (ピクセル)                     */
	long Bmp_width;                       /* 幅   (ピクセル)                     */
	unsigned short Bmp_planes;          /* プレーン数 常に 1                   */
	unsigned short Bmp_color;          /* 色 (ビット)     24                  */
	long Bmp_comp;                        /* 圧縮方法         0                  */
	long Bmp_image_size;                  /* 画像部分のファイルサイズ (バイト)   */
	long Bmp_xppm;                        /* 水平解像度 (ppm)                    */
	long Bmp_yppm;                        /* 垂直解像度 (ppm)                    */

public:
	typedef struct {                      /* 1ピクセルあたりの赤緑青の各輝度     */
		unsigned char r;
		unsigned char g;
		unsigned char b;
	} color;

	typedef struct {
		long height;
		long width;
		color data[MAXHEIGHT][MAXWIDTH];
	} img;

	void ReadBmp(char *filename, img *imgp);
	void WriteBmp(char *filename, img *tp);
	void PrintBmpInfo(char *filename);
	void HMirror(img *sp, img *tp);
	void VMirror(img *sp, img *tp);
	void Rotate90(int a, img *sp, img *tp);
	void Shrink(int a, img *sp, img *tp);
	void Mosaic(int a, img *sp, img *tp);
	void Gray(img *sp, img *tp);
	void Diminish(img *sp, img *tp, unsigned char x);
};