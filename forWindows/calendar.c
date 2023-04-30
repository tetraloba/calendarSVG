//2022/08/05 21:30 // Vivaldi履歴からの推定時刻
//2022/08/06 03:24:03 一旦完成？
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define SWITCH_TODAY 1 //「今日」の反映On/Off
#define SIZEX 1920.0
#define SIZEY 1080.0
typedef unsigned int uint;
typedef struct{
    double x;
    double y;
} Pos;
typedef struct{
    uint r;
    uint g;
    uint b;
} RGB;
int pow(int a, int b){
    int ans = 1;
    for (int i = 0; i < b; i++) {
        ans *= a;
    }
    return ans;
}
void start(Pos, Pos, char*, FILE*); //SVGのヘッダを出力 指定座標でキャンバスサイズとビューポイント
void finish(FILE*); //SVGタグを閉じる
void circle(Pos, Pos, RGB, FILE*); //円を出力
void rect(Pos, Pos, RGB, FILE*); //矩形(長方形)を出力
void line(Pos, Pos, RGB, FILE*); //直線を出力
void text(Pos, char*, RGB, uint, FILE*); //文字を出力
double split(double, double, int, int); //7分割した時のi番目の座標を返す (iは0から)
double splitC(double, double, int, int); //n分割したi番目の中心の座標を返す (iは0から)
int uruu(int); //その年がうるう年かどうか
int func_maxDay(int, int); //その年/月の日数を返す
struct tm *func_now(void); //現在日時を取得して構造体のポインタを返す
void printDate(Pos, Pos, struct tm*, RGB, FILE*); //月、日付を出力
void printWday(Pos, Pos, RGB, uint, FILE*); //曜日を出力
void matrix(Pos, Pos, uint, uint, RGB, FILE*); //格子を出力
void fillToday(Pos, FILE*); //今日に色を付ける (printDateから呼び出し)
void fillSunSat(Pos, Pos, FILE*); //日曜日と土曜日の背景に色を付ける
void generateFileName(char*, struct tm*, int); //SVGのファイル名を生成

int main(void){
    struct tm now = *func_now(); //アドレスを返すと寿命で†死†
    int s_y, s_m, s_d, s_w;
    /*scanf("%d %d %d %d", &s_y, &s_m, &s_d, &s_w); //debug (任意の日付のカレンダーを出す用)
    now.tm_year = s_y + 100;
    now.tm_mon = s_m - 1;
    now.tm_mday = s_d;
    now.tm_wday = s_w; */
    char fName[16];
    generateFileName(fName, &now, SWITCH_TODAY);

    FILE *fp = fopen(fName, "w");
    if (!fp) {
        printf("ファイル(%s)が開けません\n", fName);
        return 1;
    }
    Pos pos1 = {0.0, 0.0};
    Pos pos2 = {SIZEX, SIZEY};
    RGB rgb = {0, 0, 0};
    char unit[3] = {"px"};
    start(pos1, pos2, unit, fp);
    rect(pos1, pos2, rgb, fp);
    pos1 = (Pos){50.0, 150.0};
    pos2 = (Pos){SIZEX - pos1.x, SIZEY - 20};
    rgb = (RGB){128, 255, 128};
    fillSunSat(pos1, pos2, fp);
    printDate(pos1, pos2, &now, rgb, fp);
    matrix(pos1, pos2, 7, 5, rgb, fp);
    printWday((Pos){pos1.x, pos1.y - 10}, pos2, rgb, 32, fp);
    finish(fp);
    fclose(fp);
    return 0;
}

struct tm *func_now(void){
    time_t timer;
    time(&timer);
    return localtime(&timer);
}
void generateFileName(char fName[], struct tm *now, int flag){ //お下品なのでいつか直す
    *fName++ = now->tm_year / 10 % 10 + '0';
    *fName++ = now->tm_year % 10 + '0';
    *fName++ = (now->tm_mon + 1) / 10 + '0';
    *fName++ = (now->tm_mon + 1) % 10 + '0';
    if (flag) {
        *fName++ = now->tm_mday / 10 % 10 + '0';
        *fName++ = now->tm_mday % 10 + '0';
    }
    strcpy(fName, ".svg");
    return;
}
void printDate(Pos pos1, Pos pos2, struct tm* now, RGB rgb, FILE* fp){
    int wdayOf1 = (now->tm_wday + 7 - (now->tm_mday - 1) % 7) % 7; //1日の曜日
    char str[64] = {"<tspan text-anchor=\"end\"> 0月</tspan>"}; //月と年の出力。別関数に分けたほうが良いかも。
    //空白を出力するには"<tspan xml:space=\"preserve\"> 0月</tspan>"という文字列を出力する必要がある。 お下品。
    //xml:space使わないなら要素数8で足りる。
    //右寄せtext-snchor="end"の方が無難か
    if ((now->tm_mon + 1) / 10) {
        str[25] = (now->tm_mon + 1) / 10 + '0';
    }
    str[26] = (now->tm_mon + 1) % 10 + '0';
    text((Pos){100 + 60 * 2, 80}, str, rgb, 60, fp); //注意:右寄せ
    strcpy(str, "0000年");
    for (int i = 0; i < 4; i++) {
        str[i] = (now->tm_year + 1900) / pow(10, 3 - i) % 10 + '0';
    }
    text((Pos){100 + 60 * 3, 80}, str, rgb, 30, fp);
    uint cnt = 1;
    int maxDay = func_maxDay(now->tm_year + 1900, now->tm_mon);
    for (int i = wdayOf1, j = 0; cnt <= maxDay; i++, cnt++){
        if (6 < i) { // i = 0, 1, 2, 3, 4, 5, 6
            i = 0;
            j++;
        }
        sprintf(str, "%d", cnt);
        int margin = 5; //padding?
        int size = 32; //font-size
        double x = split(pos1.x, pos2.x, 7, i);
        double y;
        if (j < 5) { //6段目(30 or 31日のはみ出し)に対応
            y = split(pos1.y, pos2.y, 5, j);
        } else {
            y = splitC(pos1.y, pos2.y, 5, j - 1);
        }
        text((Pos){x + margin, y + size + margin}, str, rgb, size, fp); //jは使わずに日付と曜日から計算するのもアリ
        if (SWITCH_TODAY && cnt == now->tm_mday) {
            fillToday((Pos){x + margin + size / 2, y + margin + size / 2}, fp);
        }
    }
    return;
}
void fillSunSat(Pos pos1, Pos pos2, FILE* fp){
    Pos tmp1 = {split(pos1.x, pos2.x, 7, 0), pos1.y};
    Pos tmp2 = {split(pos1.x, pos2.x, 7, 1), pos2.y}; //何度もsplit呼び出すのはご愛嬌
    rect(tmp1, tmp2, (RGB){32, 4, 4}, fp);
    tmp1 = (Pos){split(pos1.x, pos2.x, 7, 6), pos1.y};
    tmp2 = (Pos){split(pos1.x, pos2.x, 7, 7), pos2.y};
    rect(tmp1, tmp2, (RGB){4, 4, 32}, fp);
    return;
}
void fillToday(Pos pos, FILE *fp){
    double r = 32;
    circle((Pos){pos.x, pos.y}, (Pos){pos.x + r, pos.y + r}, (RGB){255, 255, 0}, fp);
    return;
}
void matrix(Pos pos1, Pos pos2, uint H, uint V, RGB rgb, FILE* fp){
    for (int i = 0; i <= H; i++) {
        double tmp1x = split(pos1.x, pos2.x, H, i);
        line((Pos){tmp1x, pos1.y}, (Pos){tmp1x, pos2.y}, rgb, fp);
    }
    for (int i = 0; i <= V; i++) {
        double tmp1y = split(pos1.y, pos2.y, V, i);
        line((Pos){pos1.x, tmp1y}, (Pos){pos2.x, tmp1y}, rgb, fp);
    }
    return;
}
void printWday(Pos pos1, Pos pos2, RGB rgb, uint size, FILE* fp){
    char wday[7][5] = {"日", "月", "火", "水", "木", "金", "土"};
    for (int i = 0; i < 7; i++) {
        double tmp1x = splitC(pos1.x, pos2.x, 7, i) - size / 2;
        text((Pos){tmp1x, pos1.y}, wday[i], rgb, size, fp);
    }
    return;
}

int func_maxDay(int year, int mon){
    int arr[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int ans = arr[mon];
    if (mon == 1 && uruu(year)) { //うるう年
        ans++;
    }
    return ans;
}
int uruu(int year){
    if (year % 4) {
        return 0;
    } else if (year % 100) {
        return 1;
    } else if (year % 400) {
        return 0;
    } else {
        return 1;
    }
}
double splitC(double d1, double d2, int n, int i){
    return split(d1, d2, n, i) + (d2 - d1) / n / 2;
}
double split(double d1, double d2, int n, int i){
    return d1 + i * ((d2 - d1) / n);
}

//SVG操作用
void start(Pos pos1, Pos pos2, char *unit, FILE *fp){
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"Shift-JIS\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n  \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg version=\"1.1\" width=\"%f%s\" height=\"%f%s\"\n  viewBox=\"%f %f %f %f\"\n  preserveAspectRatio=\"xMidYMid\"\n  fill-rule=\"evenodd\"\n  xmlns=\"http://www.w3.org/2000/svg\"\n  xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", pos2.x, unit, pos2.y, unit, pos1.x, pos1.y, pos2.x, pos2.y);
    return;
}
void finish(FILE *fp){
    fputs("</svg>\n", fp);
    return;
}
void text(Pos pos, char* str, RGB rgb, uint size, FILE *fp){
    fprintf(fp, "<text x=\"%.6f\" y=\"%.6f\" font-family=\"Verdana\" font-size=\"%u\" stroke=\"none\" fill=\"#%02x%02x%02x\">\n  %s\n</text>\n", pos.x, pos.y, size, rgb.r, rgb.g, rgb.b, str);
    return;
}
void circle(Pos pos1, Pos pos2, RGB rgb, FILE *fp){
    fprintf(fp, "<circle cx=\"%.6f\" cy=\"%.6f\" r=\"%.6f\" fill=\"none\" stroke=\"#%02x%02x%02x\" stroke-width=\"%0.6f\" />\n", pos1.x, pos1.y, pos2.x - pos1.x, rgb.r, rgb.g, rgb.b, 8.0);
    return;
}
void rect(Pos pos1, Pos pos2, RGB rgb, FILE *fp){
    fprintf(fp, "<rect x=\"%.6f\" y=\"%.6f\" width=\"%.6f\" height=\"%.6f\" style=\"fill:#%02x%02x%02x\" />\n", pos1.x, pos1.y, pos2.x - pos1.x, pos2.y - pos1.y, rgb.r, rgb.g, rgb.b);
    return;
}
void line(Pos pos1, Pos pos2, RGB rgb, FILE *fp){
    fprintf(fp, "<line x1=\"%.6f\" y1=\"%.6f\" x2=\"%.6f\" y2=\"%.6f\"  stroke=\"#%02x%02x%02x\" stroke-width=\"%.6f\" stroke-opacity=\"%.6f\"  stroke-linecap=\"round\" />\n", pos1.x, pos1.y, pos2.x, pos2.y, rgb.r, rgb.g, rgb.b, 1.000000, 1.000000);
    return;
}
//背景の塗りつぶし、pathを使った矩形でも良い。path使いこなせるようになりたいなあ…
//<path d="M0 0 L 640 0 L 640 320 L 0 320" style="fill:#FFFFFF;stroke-width:0" />
//参考: https://www.ipentec.com/document/html-svg-set-canvas-background-color
//textのPosは左下基準？

//2022/08/06 15:53:51
//6段目にはみ出す日付に対応
//「今日の日付」の丸の位置がずれるのを修正、printDate()内で呼び出すように変更
//月の表示が1桁にしか対応していなかったのを修正 位置を揃えるのが大変だった
//年の表示を追加