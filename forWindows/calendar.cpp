#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#define SWITCH_TODAY true //「今日」の反映on/off
#define SIZEX 1920.0
#define SIZEY 1080.0
// #define PI 3.14159265358979323846
using namespace std;
typedef unsigned int uint;
struct Pos{
    double x;
    double y;
};
struct RGB{
    uint r;
    uint g;
    uint b;
};
int pow(int a, int b){
    int ans = 1;
    for (int i = 0; i < b; i++) {
        ans *= a;
    }
    return ans;
}
void start(Pos, Pos, string, ofstream&); //SVGのヘッダを出力 指定座標でキャンバスサイズとビューポイント
void finish(ofstream&); //SVGタグを閉じる
void line(Pos, Pos, RGB, ofstream&); //直線を出力
void rect(Pos, Pos, RGB, ofstream&); //矩形(長方形)を出力
void circle(Pos, Pos, RGB, ofstream&); //円を出力
void text(Pos, string, RGB, uint, ofstream&); //文字を出力
double split(double, double, int, int); //7分割した時のi番目の座標を返す (iは0から)
double splitC(double, double, int, int); //n分割したi番目の中心の座標を返す (iは0から)
bool uruu(int); //その年がうるう年かどうか
int func_maxDay(int, int); //その年/月の日数を返す
struct tm *func_now(void); //現在日時を取得して構造体のポインタを返す
void printMonthYear(const tm*, RGB, ofstream&); // 月と年を出力
void printDate(Pos, Pos, const tm*, RGB, ofstream&); //日付を出力
void printWday(Pos, Pos, RGB, uint, ofstream&); //曜日を出力
void matrix(Pos, Pos, uint, uint, RGB, ofstream&); //格子を出力
void fillToday(Pos, ofstream&); //今日に色を付ける (printDateから呼び出し)
void fillSunSat(Pos, Pos, ofstream&); //日曜日と土曜日の背景に色を付ける
void generateFileName(string&, const tm*, bool); //SVGのファイル名を生成

int main(){
    time_t timer = time(nullptr);
    const tm now = *localtime(&timer);
    string fName;
    generateFileName(fName, &now, SWITCH_TODAY);
    // cout << fName << endl;

    ofstream fout(fName);
    if (!fout) {
        cout << "ファイル(" << fName << ")が開けません" << endl;
        return 1;
    }
    fout.fill('0');
    Pos pos1{0.0, 0.0};
    Pos pos2{SIZEX, SIZEY};
    // RGB rgb{0, 0, 0};
    RGB rgb{0, 0, 0};
    string unit = "px";
    start(pos1, pos2, unit, fout);
    rect(pos1, pos2, rgb, fout);
    pos1 = Pos{50.0, 150.0};
    pos2 = Pos{SIZEX - pos1.x, SIZEY - 20};
    rgb = RGB{128, 255, 128};
    fillSunSat(pos1, pos2, fout);
    matrix(pos1, pos2, 7, 5, rgb, fout);
    printMonthYear(&now, rgb, fout);
    printDate(pos1, pos2, &now, rgb, fout);
    printWday(Pos{pos1.x, pos1.y - 10}, pos2, rgb, 32, fout);
    finish(fout);
    fout.close();
    return 0;
}

void generateFileName(string& fName, const tm *now, bool flag){
    ostringstream sout;
    char fillSaved = sout.fill('0');
    sout << setw(2) << now->tm_year - 100;
    sout << setw(2) << now->tm_mon + 1;
    if (flag) {
        sout << setw(2) << now->tm_mday;
    }
    sout.fill(fillSaved);
    fName = sout.str() + ".svg";
    /* cout 書式フラグテスト */
    // ios::fmtflags flagsSaved = cout.flags();
    // cout << dec << 10 << endl;
    // cout << oct << 10 << endl;
    // cout << 12 << endl;
    // cout << hex << 10 << endl;
    // cout << scientific << 3.141592 << endl;
    // cout << PI * 10 << endl;
    // cout.flags(flagsSaved);
    return;
}
void fillSunSat(Pos pos1, Pos pos2, ofstream& fout){
    const RGB rgbSun{32, 4, 4}, rgbSat{4, 4, 32};
    Pos tmp1 = {split(pos1.x, pos2.x, 7, 0), pos1.y};
    Pos tmp2 = {split(pos1.x, pos2.x, 7, 1), pos2.y}; //何度もsplit呼び出すのはご愛嬌
    rect(tmp1, tmp2, rgbSun, fout);
    tmp1 = Pos{split(pos1.x, pos2.x, 7, 6), pos1.y};
    tmp2 = Pos{split(pos1.x, pos2.x, 7, 7), pos2.y};
    rect(tmp1, tmp2, rgbSat, fout);
    return;
}
void matrix(Pos pos1, Pos pos2, uint H, uint V, RGB rgb, ofstream& fout){
    for (int i = 0; i <= H; i++) {
        double tmp1x = split(pos1.x, pos2.x, H, i);
        line(Pos{tmp1x, pos1.y}, Pos{tmp1x, pos2.y}, rgb, fout);
    }
    for (int i = 0; i <= V; i++) {
        double tmp1y = split(pos1.y, pos2.y, V, i);
        line(Pos{pos1.x, tmp1y}, Pos{pos2.x, tmp1y}, rgb, fout);
    }
    return;
}
void printWday(Pos pos1, Pos pos2, RGB rgb, uint size, ofstream& fout){
    string wday[7] = {"日", "月", "火", "水", "木", "金", "土"};
    for (int i = 0; i < 7; i++) {
        double tmp1x = splitC(pos1.x, pos2.x, 7, i) - size / 2;
        text(Pos{tmp1x, pos1.y}, wday[i], rgb, size, fout);
    }
    return;
}
void printMonthYear(const tm* now, RGB rgb, ofstream& fout){ //位置の変数化は？
    //printMonth
    const uint size = 60; //font-size
    string str = "<tspan text-anchor=\"end\">" + to_string(now->tm_mon + 1) + "月</tspan>";
    text(Pos{100 + 60 * 2, 80}, str, rgb, size, fout); //注意:右寄せ
    //printYear
    str = to_string(now->tm_year + 1900) + "年";
    text(Pos{100 + 60 * 3, 80}, str, rgb, size / 2, fout);
}
void printDate(Pos pos1, Pos pos2, const tm* now, RGB rgb, ofstream& fout){
    int wdayOf1 = (now->tm_wday + 7 - (now->tm_mday - 1) % 7) % 7; //1日の曜日
    int maxDay = func_maxDay(now->tm_year + 1900, now->tm_mon + 1);
    uint cnt = 1;
    for (int i = wdayOf1, j = 0; cnt <= maxDay; i++, cnt++){
        if (6 < i) {
            i = 0;
            j++;
        }
        string str = to_string(cnt);
        const int margin = 5; //padding?
        const int size = 32; //font-size
        double x = split(pos1.x, pos2.x, 7, i);
        double y;
        if (j < 5) {
            y = split(pos1.y, pos2.y, 5, j);
        } else {
            y = splitC(pos1.y, pos2.y, 5, j - 1);
        }
        text(Pos{x + margin, y + size + margin}, str, rgb, size, fout);
        if (SWITCH_TODAY && cnt == now->tm_mday) {
            fillToday(Pos{x + margin + size / 2, y + margin + size / 2}, fout);
        }
    }
    return;
}
void fillToday(Pos pos, ofstream& fout){
    const double r = 32; //半径
    const RGB rgbToday{255, 255 ,0};
    circle(Pos{pos.x, pos.y}, Pos{pos.x + r, pos.y + r}, rgbToday, fout);
    return;
}

//お便利func
int func_maxDay(int year, int mon){
    int arr[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int ans = arr[mon - 1];
    if (mon == 2 && uruu(year)) { //うるう年
        ans++;
    }
    return ans;
}
bool uruu(int year){
    if (year % 4) {
        return 0;
    } else if (year % 100) {
        return 1;
    } else if (year & 400) {
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
void start(Pos pos1, Pos pos2, string unit, ofstream& fout){
    fout << "<?xml version=\"1.0\" encoding=\"Shift-JIS\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n  \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg version=\"1.1\" width=\"" << pos2.x << unit << "\" height=\"" << pos2.y << unit << "\"\n  viewBox=\"" << pos1.x << ' ' << pos1.y << ' ' << pos2.x << ' ' << pos2.y << "\"\n  preserveAspectRatio=\"xMidYMid\"\n  fill-rule=\"evenodd\"\n  xmlns=\"http://www.w3.org/2000/svg\"\n  xmlns:xlink=\"http://www.w3.org/1999/xlink\">" << endl;
    cout << pos1.x << ' ' << pos2.x << endl;
    return;
}
void finish(ofstream& fout){
    fout << "</svg>" << endl;
    return;
}
void text(Pos pos, string str, RGB rgb, uint size, ofstream& fout){
    fout << "<text x=\"" << pos.x << "\" y=\"" << pos.y << "\" font-family=\"Shift-JIS\" font-size=\"" << size << "\" stroke=\"none\" fill=\"#" << hex << setw(2) << rgb.r << setw(2) << rgb.g << setw(2) << rgb.b << dec << "\">\n  " << str << "\n</text>" << endl;
    return;
}
void circle(Pos pos1, Pos pos2, RGB rgb, ofstream& fout){
    const double strokeWidth = 8.0;
    fout << "<circle cx=\"" << pos1.x << "\" cy=\"" << pos1.y << "\" r=\"" << pos2.x - pos1.x << "\" fill=\"none\" stroke=\"#" << hex << setw(2) << rgb.r << setw(2) << rgb.g << setw(2) << rgb.b << dec << "\" stroke-width=\"" << strokeWidth << "\" />" << endl;
    return;
}
void rect(Pos pos1, Pos pos2, RGB rgb, ofstream& fout){
    fout << "<rect x=\"" << pos1.x << "\" y=\"" << pos1.y << "\" width=\"" << pos2.x - pos1.x << "\" height=\"" << pos2.y - pos1.y << "\" style=\" fill:#" << setw(2) << hex << rgb.r << setw(2) << rgb.g << setw(2) << rgb.b << dec << "\" />" << endl;
    return;
}
void line(Pos pos1, Pos pos2, RGB rgb, ofstream& fout){
    fout << "<line x1=\"" << fixed << pos1.x << "\" y1=\"" << pos1.y << "\" x2=\"" << pos2.x << "\" y2=\"" << pos2.y << "\" stroke=\"#" << hex << setw(2) << rgb.r << setw(2) << rgb.g << setw(2) << rgb.b << dec << "\" stroke-width=\"" << 1 << "\" stroke-opacity=\"" << 1 << "\" stroke-linecap=\"round\" />" << endl;
    return;
}
    // cout << resetiosflags(ios_base::floatfield);