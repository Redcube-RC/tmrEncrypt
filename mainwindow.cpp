// -*- coding: utf-8 -*-

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>
#include <locale>
#include <codecvt>
#include <cmath>
#include <random>
#include <vector>

#include <QString>
#include <QApplication>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QClipboard>
#include <QLineEdit>
#include <QRadioButton>

using namespace std;

// 用于只选取低8bit
#define MOD 256

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->de, SIGNAL(clicked()), this, SLOT(de_clicked()));
    connect(ui->lggen, SIGNAL(clicked()), this, SLOT(lggen_clicked()));
    connect(ui->ggggen, SIGNAL(clicked()), this, SLOT(ggggen_clicked()));
    connect(ui->deDelete, SIGNAL(clicked()), this, SLOT(deDelete_clicked()));
    connect(ui->deCopy, SIGNAL(clicked()), this, SLOT(deCopy_clicked()));
    connect(ui->enDelete, SIGNAL(clicked()), this, SLOT(enDelete_clicked()));
    connect(ui->enCopy, SIGNAL(clicked()), this, SLOT(enCopy_clicked()));
    connect(ui->exchange, SIGNAL(clicked()), this, SLOT(exchange_clicked()));

    ui->radioButtontmr3->setChecked(true);
    ui->keyEdittmr2->setEnabled(false);
    ui->labeltmr2->setEnabled(false);

    connect(ui->radioButtontmr3, &QRadioButton::toggled, [=](bool checked){
        if (checked) {
            ui->keyEdittmr2->setEnabled(false);
            ui->keyEdittmr3->setEnabled(true);
            ui->labeltmr2->setEnabled(false);
            ui->labeltmr3->setEnabled(true);
        }
    });
    connect(ui->radioButtontmr2, &QRadioButton::toggled, [=](bool checked){
        if (checked){
            ui->keyEdittmr3->setEnabled(false);
            ui->keyEdittmr2->setEnabled(true);
            ui->labeltmr3->setEnabled(false);
            ui->labeltmr2->setEnabled(true);
        }
    });

    ui->progressBar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

static const long long pow62[10] = {1, 62, 3844, 238328, 14776336, 916132832, 56800235584, 3521614606208, 218340105584896, 13537086546263552};
static const long pow16[8] = {1, 16, 256, 4096, 65536, 1048576, 16777216, 268435456};

// 以下直至Line 439为csdn大佬 物联黄同学 的AES-128加密，非原创
// s 盒， 用于密钥生成和加密时的字节代换
static const int S[16][16] = { 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
                              0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                              0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
                              0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                              0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
                              0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                              0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
                              0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                              0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
                              0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                              0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
                              0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                              0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
                              0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                              0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
                              0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

// 常量轮值表
// 这里将原来的复制粘贴过来的轮常量类别改成了 long long
// 原因在于第8位超出int限，数据溢出
static const long long Rcon[10] = {
    0x01000000, 0x02000000,
    0x04000000, 0x08000000,
    0x10000000, 0x20000000,
    0x40000000, 0x80000000,
    0x1b000000, 0x36000000
};

// 将字符转整数
int ch_to_int(char& ch);

// 将16进制字符串转为数字
long long str_long(string str)
{
    long long ans = 0;
    // 遍历字符串，将字符串的内容变为16进制数字
    for (char ch : str)
    {
        ans = ans * 16 + ch_to_int(ch);
    }
    return ans;
}

// 出于方便考虑，还是写一个字符转成数字的函数
int ch_to_int(char& ch)
{
    int ans = 0;
    // 数字的时候
    if (ch >= 48 && ch <= 57)
    {
        ans = ch - '0';
    }
    // 16进制中a 到 f
    else if (ch >= 'a' && ch <= 'f')
    {
        ans = ch - 'a' + 10;
    }
    // 这个在本实验的样例中，其实不会运行到的
    else if (ch >= 'A' && ch <= 'F')
    {
        ans = ch - 'A' + 10;
    }
    return ans;
}

// 然后再写一个数字转16进制字符串的吧，只考虑小写吧
// 针对的其实是单个16进制
string int_to_chs(long long num)
{
    string ans = "";
    while (num)
    {
        // 通过位运算得到低四位
        int x = num & 0xf;
        // 根据数值进行区分
        if (x <= 9)
        {
            char ch = x + '0';
            ans += ch;
        }
        else
        {
            char ch = x - 10 + 'a';
            ans += ch;
        }
        // 移位，其实相当于 / 16
        num >>= 4;
    }
    // 然后反转字符
    int left = 0, right = ans.length() - 1;
    // 双指针实现字符串反转
    while (left < right)
    {
        char  ch = ans[left];
        ans[left] = ans[right];
        ans[right] = ch;
        left++;
        right--;
    }
    return ans;
}

// 得先分组
vector<string> group_key(string& key)
{
    // 四组
    vector<string> groups(4);
    // 初始下标
    int index = 0;
    // 分组
    for (string& g : groups)
    {
        g = key.substr(index, 8);
        index += 8;
    }
    return groups;
}

// 字节循环
string loop_wordbyte(string& wi_1);

// 字节代换
string wordbyte_sub(string& wi_1);

// 轮常量异或
string xor_with_const(string& wi_1, int rounds);


// 拓展的时候，下标为4 的倍数时，需要麻烦一些，需要使用一个变换的T函数
string T(string& wi_1, int round)
{
    // T 变换由3部分构成， 所以可以再写三个函数
    // 先进行字循环
    string ans = loop_wordbyte(wi_1);
    // 然后字节代换
    ans = wordbyte_sub(ans);
    // 最后是轮异或
    ans = xor_with_const(ans, round);

    return ans;
}

// 字节循环实现
string loop_wordbyte(string& wi_1)
{
    string ans = wi_1.substr(2) + wi_1.substr(0, 2);
    return ans;
}

// 字节代换 的实现
string wordbyte_sub(string& wi_1)
{
    int len = wi_1.length();
    string ans = "";
    for (int i = 0; i < len; i += 2)
    {
        // 先获取当前的下标
        int x = ch_to_int(wi_1[i]), y = ch_to_int(wi_1[i + 1]);
        // 然后获取当前的数字
        int num = S[x][y];
        // 先将数值转化为字符串
        string s = int_to_chs(num);
        // 然后不足的话补0
        while (s.length() < 2)
        {
            s = "0" + s;
        }

        // 加起来
        ans += s;
    }
    return ans;
}

// 轮常量异或
string xor_with_const(string& wi_1, int rounds)
{
    // 先将字符串变为数字
    long long num = 0;
    for (int i = 0; i < 8; ++i)
    {
        char ch = wi_1[i];
        num = num * 16 + ch_to_int(ch);
    }
    // 计算异或结果
    num ^= Rcon[rounds];

    // 将num转化为字符串
    string res = int_to_chs(num);
    while (res.length() < 8)
    {
        res = "0" + res;
    }
    return res;
}

// 写一个字符串对应的16进制数异或的函数
string string_xor(string s1, string s2)
{
    long long num1 = str_long(s1), num2 = str_long(s2);
    long long num = num1 ^ num2;
    // 再把数字转为字符串
    string ans = int_to_chs(num);
    // 不足8位的时候补位
    while (ans.length() < 8)
    {
        ans = "0" + ans;
    }
    return ans;
}


// 先要进行密钥的拓展
vector<string> extend_key(string& key)
{
    // 先分组
    vector<string> w_key = group_key(key);
    for (int i = 0; i < 40; ++i)
    {
        string w = "";
        int index = 4 + i;
        string temp = w_key[index - 1];
        // 4 的倍数的时候，需要调用T函数
        if (index % 4 == 0)
        {
            temp = T(temp, index / 4 - 1);
        }
        w = string_xor(temp, w_key[index - 4]);

        // 压入数组中
        w_key.push_back(w);
    }

    return w_key;
}

// 行移位函数
vector<string> move_row(vector<string>& s)
{
    vector<string> ans = s;
    // 几个比较麻烦的地方
    // 我的字符串数组其实每个是对应一列, 所以其实是对应到列进行移位
    // 一行对应有两个16进制数，所以需要两个一起移动，其实就是对应两列一起动
    for (int i = 0; i < 4; ++i)
    {
        int k = i * 2;
        // 就原本矩阵对应的行移位，对于字符串数组就是列移位
        for (int j = 0; j < 4; ++j)
        {
            ans[j][k] = s[(j + i) % 4][k];
            ans[j][k + 1] = s[(j + i) % 4][k + 1];
        }
    }
    return ans;
}

// 写个函数分割一下字符串，长度为8变4组
vector<string> split_s(string& s)
{
    vector<string> ans;
    for (int i = 0; i < s.length(); i += 2)
    {
        ans.emplace_back(s.substr(i, 2));
    }
    return ans;
}

string int_ch2(int num)
{
    string ans = int_to_chs(num);
    // 这里是为了确保只有两位字符串
    while (ans.length() < 2)
    {
        ans = "0" + ans;
    }
    return ans;
}

// 移位函数，其实就是在GF（2^8）的范围进行幂次操作
int power(int num)
{
    int ans = (num << 1) % MOD;
    // 如果第七位是1
    if (num & 0x80)
    {
        ans ^= 0x1b;
    }
    return ans;
}

// 接下来就是列混淆
vector<string> col_confuse(vector<string>& s)
{
    vector<string> ans = s;
    // 算法中对应的是列，这边就直接变成了行，即字符
    for (int i = 0; i < 4; ++i)
    {
        // 需要先将字符串拆分成两两一组，共4组
        auto temp = split_s(s[i]);
        // 先转成数字
        int s0 = str_long(temp[0]), s1 = str_long(temp[1]), s2 = str_long(temp[2]),
            s3 = str_long(temp[3]);
        // 计算混淆后的值
        int t0 = power(s0) ^ power(s1) ^ s1 ^ s2 ^ s3;
        int t1 = s0 ^ power(s1) ^ power(s2) ^ s2 ^ s3;
        int t2 = s0 ^ s1 ^ power(s2) ^ s3 ^ power(s3);
        int t3 = s0 ^ power(s0) ^ s1 ^ s2 ^ power(s3);
        // 转换成字符串再相加
        ans[i] = int_ch2(t0) + int_ch2(t1) + int_ch2(t2) + int_ch2(t3);
    }
    return ans;
}

// 先定义一个 aes 加密函数
long long aes(string& plain_text, string& key, int textLength)
{
    // 先拓展密钥
    vector<string> keys = extend_key(key);

    int index = 0;
    // 然后就是10轮迭代
    // 需要知道明文其实是32位，所以需要搞4下
    // 可以先把明文也分组

    // 一开始的先进行一次轮密钥加
    vector<string> texts = group_key(plain_text);
    for (int i = 0; i < 4; ++i)
    {
        texts[i] = string_xor(texts[i], keys[i]);
    }
    index += 4;
    // 然后十次迭代
    for (int k = 0; k < 10; ++k)
    {
        for (int j = 0; j < 4; ++j)
        {
            // 先是字节代换
            texts[j] = wordbyte_sub(texts[j]);
        }
        // 然后是行移位
        texts = move_row(texts);

        if (k < 9)
        {
            // 再来列混淆
            texts = col_confuse(texts);
        }

        // 轮密钥加
        for (int i = 0; i < 4; ++i)
        {
            texts[i] = string_xor(texts[i], keys[i + index]);
        }

        index += 4;
    }
    string result = "";
    for (int i = 0; i < 4; ++i)
    {
        result += texts[i];
    }
    return (str_long(result)%textLength+textLength)%textLength;
}

int keyToNumber(char a){
    int num = static_cast<int>(a);
    if (num >= 48 && num <= 57) return num - 48;
    else if (num >= 65 && num <= 90) return num - 55;
    else if (num >= 97 && num <= 122) return num - 61;
    else return 0;
}

int lgg(string a) {
    string en[36] = {
        "灵灵", "灵感", "灵菇", "灵哩",
        "感灵", "感感", "感菇", "感哩",
        "菇灵", "菇感", "菇菇", "菇哩",
        "哩灵", "哩感", "哩菇", "哩哩",
        "灵刮", "灵擦", "感刮", "感擦",
        "菇刮", "菇擦", "哩刮", "哩擦",
        "刮灵", "刮感", "刮菇", "刮哩",
        "擦灵", "擦感", "擦菇", "擦哩",
        "刮刮", "刮擦", "擦刮", "擦擦"
    };
    for (int i = 0; i <= 35; i++) {
        if (en[i] == a) {
            if (i >= 16 && i <= 31) return i - 16;
            else return i;
        }
    }
    return -1;
}

int awa(int a){
    if (a >= 16) return a-16;
    else if (a < 0) return a+16;
    else return a;
}

void MainWindow::de_clicked(){
    ui->progressBar->show();
    if (ui->radioButtontmr3->isChecked()) MainWindow::de_3();
    else MainWindow::de_2();
    ui->progressBar->hide();
}

void MainWindow::lggen_clicked(){
    ui->progressBar->show();
    if (ui->radioButtontmr3->isChecked()) MainWindow::lggen_3();
    else MainWindow::lggen_2();
    ui->progressBar->hide();
}

void MainWindow::ggggen_clicked(){
    ui->progressBar->show();
    if (ui->radioButtontmr3->isChecked()) MainWindow::ggggen_3();
    else MainWindow::ggggen_2();
    ui->progressBar->hide();
}

string MainWindow::key_translater_3(){
    long long keyNum = 0;
    string keyinput = (ui->keyEdittmr3->text()).toStdString() + "                       ";
    // 转换密钥的前段
    for(int i = 0; i < 10; i++){
        keyNum += keyToNumber(keyinput[i]) * pow62[i];
    }
    string key = int_to_chs(keyNum)+"";
    // 进位1
    key = string(15-key.length(), '0') + key;
    char keyCarrying = key[0];
    key.erase(0, 1);
    keyNum = 0;
    // 转换密钥的中段
    for(int i = 10; i < 20; i++){
        keyNum += keyToNumber(keyinput[i]) * pow62[i-10];
    }
    key = int_to_chs(keyNum + ch_to_int(keyCarrying)) + key;
    // 进位2
    key = string(29-key.length(), '0') + key;
    keyCarrying = key[0];
    key.erase(0, 1);
    keyNum = 0;
    // 转换密钥的后段
    for(int i = 20; i < 24; i++){
        keyNum += keyToNumber(keyinput[i]) * pow62[i-20];
    }
    key = int_to_chs(keyNum + ch_to_int(keyCarrying)) + key;
    // 补位/删位
    if (key.length() < 32) key = string(32-key.length(), '0') + key;
    else if (key.length() > 32) key.erase(0,key.length()-32);
    return key;
}

int gggg(string a) {
    string en[16] = {
        "咕咕咕咕", "咕咕咕嘎", "咕咕嘎咕", "咕咕嘎嘎",
        "咕嘎咕咕", "咕嘎咕嘎", "咕嘎嘎咕", "咕嘎嘎嘎",
        "嘎咕咕咕", "嘎咕咕嘎", "嘎咕嘎咕", "嘎咕嘎嘎",
        "嘎嘎咕咕", "嘎嘎咕嘎", "嘎嘎嘎咕", "嘎嘎嘎嘎"
    };
    for (int i = 0; i <= 15; i++) {
        if (en[i] == a) {
            return i;
        }
    }
    return -1;
}

int rd(int a, int b) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}

void MainWindow::de_2() {
    int keyinput = 0;
    int keyEditinput = (ui->keyEdittmr2->text()).toInt();
    if (keyEditinput >= 0 && keyEditinput <= 65535) keyinput = keyEditinput;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16[i]);
        keyinput -= key[i] * pow16[i];
    }

    string done;
    QString qinput = ui->deEdit->toPlainText();
    string input = qinput.toUtf8().constData();
    bool InputCorrect = true;
    if (input.find("灵") != string::npos || input.find("感") != string::npos || input.find("菇") != string::npos ||
        input.find("擦") != string::npos || input.find("刮") != string::npos || input.find("哩") != string::npos) {
        if (input.length() % 6 != 0) InputCorrect = false;
        else {
            vector<string> de(input.length() / 6);
            for (int i = 0; i < input.length() / 6; i++) {
                de[i] = input.substr(6 * i, 6);
                if (lgg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 6; ) {
                if (lgg(de[i]) >= 32) {
                    i++;
                    uint32_t result = 0, index = 7;
                    for (int j = i; j <= i + 7; j++) {
                        if (index >= 4) result += awa(lgg(de[j])-key[index-4]) * pow16[index];
                        else result += awa(lgg(de[j])-key[index]) * pow16[index];
                        index--;
                    }
                    i += 8;
                    if (InputCorrect) {
                        char32_t ch = static_cast<char32_t>(result);
                        QString qstr = QString::fromUcs4(&ch, 1);
                        done += qstr.toStdString();
                    }

                }
                else {
                    uint32_t result = 0, index = 3;
                    for (int j = i; j <= i + 3; j++) {
                        result += awa(lgg(de[j])-key[index]) * pow16[index];
                        index--;
                    }
                    i += 4;
                    if (InputCorrect) {
                        char32_t ch = static_cast<char32_t>(result);
                        QString qstr = QString::fromUcs4(&ch, 1);
                        done += qstr.toStdString();
                    }
                }
            }
        }
    }
    else if (input.find("咕") != string::npos || input.find("嘎") != string::npos) {
        if (input.length() % 12 != 0) InputCorrect = false;
        else {
            vector<string> de(input.length() / 12);
            for (int i = 0; i < input.length() / 12; i++) {
                de[i] = input.substr(12 * i, 12);
                if (gggg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 12; ) {
                uint32_t result = 0, index = 3;
                for (int j = i; j <= i + 3; j++) {
                    result += awa(gggg(de[j])-key[index]) * pow16[index];
                    index--;
                }
                if (result >= 12256 && result <= 12271) {
                    result = 0, index = 7;
                    i += 4;
                    for (int j = i; j <= i + 7; j++) {
                        if (index >= 4) result += awa(gggg(de[j])-key[index-4]) * pow16[index];
                        else result += awa(gggg(de[j])-key[index]) * pow16[index];
                        index--;
                    }
                    i += 8;
                }
                else {
                    i += 4;
                }
                if (InputCorrect) {
                    char32_t ch = static_cast<char32_t>(result);
                    QString qstr = QString::fromUcs4(&ch, 1);
                    done += qstr.toStdString();
                }
            }
        }
    }
    ui->enEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::de_3() {
    string key = MainWindow::key_translater_3();

    string done;

    string inputCache = (ui->deEdit->toPlainText()).toUtf8().constData();
    ui->progressBar->setRange(0, inputCache.length() / 3);
    vector<string> inputchar(inputCache.length() / 3);
    vector<int> exchangeList(inputCache.length() / 3);
    for (int i = 0; i < inputCache.length() / 3; i++) {
        // 分割密文
        inputchar[i] = inputCache.substr(i * 3, 3);
        // 生成交换表
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        exchangeList[i] = aes(plain_text, key, inputCache.length() / 3);
        //cout<<"exchangeList"<<i<<":"<<exchangeList[i]<<endl;
        ui->progressBar->setValue(i);
    }
    // 交换
    for (int i = inputCache.length() / 3 - 1; i >= 0; i--) {
        string exchangeCache1 = inputchar[i];
        string exchangeCache2 = inputchar[exchangeList[i]];
        inputchar[i] = exchangeCache2;
        inputchar[exchangeList[i]] = exchangeCache1;
    }
    // 合并
    string input = "";
    for (int i = 0; i < inputCache.length() / 3; i++) {
        input += inputchar[i];
    }

    bool InputCorrect = true;
    if (input.find("灵") != string::npos || input.find("感") != string::npos || input.find("菇") != string::npos ||
        input.find("擦") != string::npos || input.find("刮") != string::npos || input.find("哩") != string::npos) {
        if (input.length() % 6 != 0) InputCorrect = false;
        else {
            vector<string> de(input.length() / 6);
            for (int i = 0; i < input.length() / 6; i++) {
                de[i] = input.substr(6 * i, 6);
                if (lgg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 6; ) {
                if (lgg(de[i]) >= 32) {
                    i++;
                    uint32_t result = ((lgg(de[i]) <= 32) ? 0 : 0x100000), index = 4;
                    for (int j = i; j <= i + 4; j++) {
                        result += lgg(de[j]) * pow16[index];
                        index--;
                    }
                    i += 5;
                    if (InputCorrect) {
                        char32_t ch = static_cast<char32_t>(result);
                        QString qstr = QString::fromUcs4(&ch, 1);
                        done += qstr.toStdString();
                    }

                }
                else {
                    uint32_t result = 0, index = 3;
                    for (int j = i; j <= i + 3; j++) {
                        result += lgg(de[j]) * pow16[index];
                        index--;
                    }
                    i += 4;
                    if (InputCorrect) {
                        char32_t ch = static_cast<char32_t>(result);
                        QString qstr = QString::fromUcs4(&ch, 1);
                        done += qstr.toStdString();
                    }
                }
            }
        }
    }
    else if (input.find("咕") != string::npos || input.find("嘎") != string::npos) {
        if (input.length() % 12 != 0) InputCorrect = false;
        else {
            vector<string> de(input.length() / 12);
            for (int i = 0; i < input.length() / 12; i++) {
                de[i] = input.substr(12 * i, 12);
                if (gggg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 12; ) {
                uint32_t result = 0, index = 3;
                for (int j = i; j <= i + 3; j++) {
                    result += gggg(de[j]) * pow16[index];
                    index--;
                }
                if (result >= 0x2FE0 && result <= 0x2FEF) {
                    result = ((result <= 0x2FE7) ? 0 : 0x100000), index = 4;
                    i += 4;
                    for (int j = i; j <= i + 4; j++) {
                        result += gggg(de[j]) * pow16[index];
                        index--;
                    }
                    i += 5;
                }
                else {
                    i += 4;
                }
                if (InputCorrect) {
                    char32_t ch = static_cast<char32_t>(result);
                    QString qstr = QString::fromUcs4(&ch, 1);
                    done += qstr.toStdString();
                }
            }
        }
    }
    ui->enEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::lggen_3(){
    string key = MainWindow::key_translater_3();
    string en[36] = {
        "灵灵", "灵感", "灵菇", "灵哩",
        "感灵", "感感", "感菇", "感哩",
        "菇灵", "菇感", "菇菇", "菇哩",
        "哩灵", "哩感", "哩菇", "哩哩",
        "灵刮", "灵擦", "感刮", "感擦",
        "菇刮", "菇擦", "哩刮", "哩擦",
        "刮灵", "刮感", "刮菇", "刮哩",
        "擦灵", "擦感", "擦菇", "擦哩",
        "刮刮", "刮擦", "擦刮", "擦擦"
    };
    // 分割明文
    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++){
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }

    for (int j = 0; j < u32input.length(); j++){
        if (a[j] >= 65536) {
            done += en[(a[j] <= 1048575) ? rd(32, 33) : rd(34, 35)];
            for (int i = 4; i >= 0; i--){
                int result = a[j] / pow16[i];
                done += en[result+rd(0, 1)*16];
                a[j] -= pow16[i] * result;
            }
        }else{
            for (int i = 3; i >= 0; i--){
                int result = a[j] / pow16[i];
                done += en[result+rd(0, 1)*16];
                a[j] -= pow16[i] * result;
            }
        }
    }
    ui->progressBar->setRange(0, done.length() / 3);
    vector<string> donechar(done.length() / 3);
    vector<int> exchangeList(done.length() / 3);
    for (int i = 0; i < done.length() / 3; i++){
        // 分割密文
        donechar[i] = done.substr(i * 3, 3);
        // 生成交换表
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        exchangeList[i] = aes(plain_text, key, done.length() / 3);
        ui->progressBar->setValue(i);
    }
    // 交换
    for (int i = 0; i < done.length() / 3; i++){
        string exchangeCache1 = donechar[i];
        string exchangeCache2 = donechar[exchangeList[i]];
        donechar[i] = exchangeCache2;
        donechar[exchangeList[i]] = exchangeCache1;
    }
    // 合并
    string result = "";
    for (int i = 0; i < done.length() / 3; i++){
        result += donechar[i];
    }
    ui->deEdit->setPlainText(QString::fromUtf8(result));
}

void MainWindow::lggen_2(){
    int keyinput = 0;
    int keyEditinput = (ui->keyEdittmr2->text()).toInt();
    if (keyEditinput >= 0 && keyEditinput <= 65535) keyinput = keyEditinput;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16[i]);
        keyinput -= key[i] * pow16[i];
    }

    string en[36] = {
        "灵灵", "灵感", "灵菇", "灵哩",
        "感灵", "感感", "感菇", "感哩",
        "菇灵", "菇感", "菇菇", "菇哩",
        "哩灵", "哩感", "哩菇", "哩哩",
        "灵刮", "灵擦", "感刮", "感擦",
        "菇刮", "菇擦", "哩刮", "哩擦",
        "刮灵", "刮感", "刮菇", "刮哩",
        "擦灵", "擦感", "擦菇", "擦哩",
        "刮刮", "刮擦", "擦刮", "擦擦"
    };
    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++){
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }


    for (int j = 0; j < u32input.length(); j++){
        if (a[j] > 65536) {
            done += en[rd(32, 35)];
            for (int i = 7; i >= 0; i--){
                int result = a[j] / pow16[i];
                if (i >= 4) result += key[i - 4];
                else result += key[i];
                if (result >= 16) result -= 16;
                done += en[result+rd(0, 1)*16];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }else{
            for (int i = 3; i >= 0; i--){
                int result = a[j] / pow16[i] + key[i];
                if (result >= 16) result -= 16;
                done += en[result+rd(0, 1)*16];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }
    }
    ui->deEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::ggggen_2(){
    int keyinput = 0;
    int keyEditinput = (ui->keyEdittmr2->text()).toInt();
    if (keyEditinput >= 0 && keyEditinput <= 65535) keyinput = keyEditinput;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16[i]);
        keyinput -= key[i] * pow16[i];
    }

    string en[16] = {
        "咕咕咕咕", "咕咕咕嘎", "咕咕嘎咕", "咕咕嘎嘎",
        "咕嘎咕咕", "咕嘎咕嘎", "咕嘎嘎咕", "咕嘎嘎嘎",
        "嘎咕咕咕", "嘎咕咕嘎", "嘎咕嘎咕", "嘎咕嘎嘎",
        "嘎嘎咕咕", "嘎嘎咕嘎", "嘎嘎嘎咕", "嘎嘎嘎嘎"
    };

    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++){
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }


    for (int j = 0; j < u32input.length(); j++){
        if (a[j] > 65536){
            done += en[awa(2+key[3])]+en[awa(15+key[2])]+en[awa(14+key[1])]+en[rd(0, 15)];
            for (int i = 7; i >= 0; i--){
                int result = a[j] / pow16[i];
                if (i >= 4) result += key[i - 4];
                else result += key[i];
                if (result >= 16) result -= 16;
                done += en[result];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }else{
            for (int i = 3; i >= 0; i--){
                int result = a[j] / pow16[i] + key[i];
                if (result >= 16) result -= 16;
                done += en[result];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }
    }
    ui->deEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::ggggen_3(){
    string key = MainWindow::key_translater_3();

    string en[16] = {
        "咕咕咕咕", "咕咕咕嘎", "咕咕嘎咕", "咕咕嘎嘎",
        "咕嘎咕咕", "咕嘎咕嘎", "咕嘎嘎咕", "咕嘎嘎嘎",
        "嘎咕咕咕", "嘎咕咕嘎", "嘎咕嘎咕", "嘎咕嘎嘎",
        "嘎嘎咕咕", "嘎嘎咕嘎", "嘎嘎嘎咕", "嘎嘎嘎嘎"
    };

    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++){
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }


    for (int j = 0; j < u32input.length(); j++){
        if (a[j] >= 65536){
            done += en[2]+en[15]+en[14]+en[(a[j] <= 1048575) ? rd(0, 7) : rd(8, 15)];
            for (int i = 4; i >= 0; i--){
                int result = a[j] / pow16[i];
                done += en[result];
                a[j] -= pow16[i] * result;
            }
        }else{
            for (int i = 3; i >= 0; i--){
                int result = a[j] / pow16[i];
                done += en[result];
                a[j] -= pow16[i] * result;
            }
        }
    }

    ui->progressBar->setRange(0, done.length() / 3);
    vector<string> donechar(done.length() / 3);
    vector<int> exchangeList(done.length() / 3);
    for (int i = 0; i < done.length() / 3; i++){
        // 分割密文
        donechar[i] = done.substr(i * 3, 3);
        // 生成交换表
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        exchangeList[i] = aes(plain_text, key, done.length() / 3);
        ui->progressBar->setValue(i);
    }
    // 交换
    for (int i = 0; i < done.length() / 3; i++){
        string exchangeCache1 = donechar[i];
        string exchangeCache2 = donechar[exchangeList[i]];
        donechar[i] = exchangeCache2;
        donechar[exchangeList[i]] = exchangeCache1;
    }
    // 合并
    string result = "";
    for (int i = 0; i < done.length() / 3; i++){
        result += donechar[i];
    }
    ui->deEdit->setPlainText(QString::fromUtf8(result));
}

void MainWindow::enDelete_clicked(){ui->enEdit->clear();}
void MainWindow::deDelete_clicked(){ui->deEdit->clear();}

void MainWindow::exchange_clicked(){
    QString enEdit_cache = ui->enEdit->toPlainText();
    QString deEdit_cache = ui->deEdit->toPlainText();
    ui->enEdit->setPlainText(deEdit_cache);
    ui->deEdit->setPlainText(enEdit_cache);
}

void MainWindow::enCopy_clicked(){QApplication::clipboard()->setText(ui->enEdit->toPlainText());}
void MainWindow::deCopy_clicked(){QApplication::clipboard()->setText(ui->deEdit->toPlainText());}
