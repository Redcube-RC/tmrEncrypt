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
#include <QMEssageBox>

#include "aes.h"

using namespace std;

void MainWindow::setEnabled(bool key3){
    ui->labeltmr2->setVisible(!key3);
    ui->labeltmr3->setVisible(key3);
    ui->e1->setEnabled(define);
    ui->e2->setEnabled(define);
    ui->e3->setEnabled(define);
    ui->e4->setEnabled(define);
    ui->e5->setEnabled(define);
    ui->e6->setEnabled(define);
    ui->e7->setEnabled(define);
    ui->e8->setEnabled(define);
    ui->labelgggg->setEnabled(define);
    ui->labellgg->setEnabled(define);
    ui->labeldefine->setEnabled(define);
}

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
    connect(ui->algo, SIGNAL(currentIndexChanged()), this, SLOT(on_algo_currentIndexChanged(int index)));

    ui->progressBar->hide();
    ui->labeltmr2->hide();
}


void MainWindow::on_algo_currentIndexChanged(int index)
{
    index = ui->algo->currentIndex();
    if (index == 0 || index == 1){
        define = true;
        setEnabled(true);
    }else if (index == 2){
        define = false;
        setEnabled(true);
    }else if (index == 3){
        define = false;
        setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

static const long long pow62[10] = {1, 62, 3844, 238328, 14776336, 916132832, 56800235584, 3521614606208, 218340105584896, 13537086546263552};
static const long pow16[8] = {1, 16, 256, 4096, 65536, 1048576, 16777216, 268435456};

int keyToNumber(char a){
    int num = static_cast<int>(a);
    if (num >= 48 && num <= 57) return num - 48;
    else if (num >= 65 && num <= 90) return num - 55;
    else if (num >= 97 && num <= 122) return num - 61;
    else return 0;
}

void MainWindow::enList() {
    if ((ui->e1->text()).toStdString() != "" && define) en[0] = (ui->e1->text()).toStdString(); else en[0] = "灵"; // 灵
    if ((ui->e2->text()).toStdString() != "" && define) en[1] = (ui->e2->text()).toStdString(); else en[1] = "感"; // 感
    if ((ui->e3->text()).toStdString() != "" && define) en[2] = (ui->e3->text()).toStdString(); else en[2] = "菇"; // 菇
    if ((ui->e4->text()).toStdString() != "" && define) en[3] = (ui->e4->text()).toStdString(); else en[3] = "哩"; // 哩
    if ((ui->e5->text()).toStdString() != "" && define) en[4] = (ui->e5->text()).toStdString(); else en[4] = "刮"; // 刮
    if ((ui->e6->text()).toStdString() != "" && define) en[5] = (ui->e6->text()).toStdString(); else en[5] = "擦"; // 擦
    if ((ui->e7->text()).toStdString() != "" && define) en[6] = (ui->e7->text()).toStdString(); else en[6] = "咕"; // 咕
    if ((ui->e8->text()).toStdString() != "" && define) en[7] = (ui->e8->text()).toStdString(); else en[7] = "嘎"; // 嘎

    bool legal = true;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

    for (int i = 0; i < 8 && legal; i++) { 
        if (converter.from_bytes(en[i]).size() != 1) {
            legal = false;
            QMessageBox::critical(nullptr, "错误", "自定义密文组成字符不得大于一个字符");
            break;
        }
        for (int j = 0; j < 8; j++) {
            if (i != j && en[i] == en[j]) {
                legal = false;
                QMessageBox::critical(nullptr, "错误", "自定义密文组成字符有重复");
                break;
            }
        }
    }

    if (!legal || !define) {
        en[0] = "灵", en[1] = "感", en[2] = "菇", en[3] = "哩", en[4] = "刮", en[5] = "擦", en[6] = "咕", en[7] = "嘎";
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            lggen[4 * i + j] = en[i] + en[j];
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            lggen[16 + 2 * i + j] = en[i] + en[j + 4];
            lggen[24 + 4 * j + i] = en[j + 4] + en[i];
        }
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            lggen[32 + 2 * i + j] = en[i + 4] + en[j + 4];
        }
    }

    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < 2; b++) {
            for (int c = 0; c < 2; c++) {
                for (int d = 0; d < 2; d++) {
                    ggggen[8 * a + 4 * b + 2 * c + d] = en[a + 6] + en[b + 6] + en[c + 6] + en[d + 6];
                }
            }
        }
    }
}

int MainWindow::lgg(string a) {
    for (int i = 0; i <= 35; i++) {
        if (lggen[i] == a) {
            if (i >= 16 && i <= 31) return i - 16;
            else return i;
        }
    }
    return -1;
}

int MainWindow::gggg(string a) {
    for (int i = 0; i <= 15; i++) {
        if (ggggen[i] == a) {
            return i;
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
    if (ui->algo->currentIndex() == 0) de_5();
    else if (ui->algo->currentIndex() == 1) de_4();
    else if (ui->algo->currentIndex() == 2) de_3();
    else de_2();
    ui->progressBar->hide();
}

void MainWindow::lggen_clicked(){
    ui->progressBar->show();
    if (ui->algo->currentIndex() == 0) lggen_5();
    else if (ui->algo->currentIndex() == 1) lggen_4();
    else if (ui->algo->currentIndex() == 2) lggen_3();
    else lggen_2();
    ui->progressBar->hide();
}

void MainWindow::ggggen_clicked(){
    ui->progressBar->show();
    if (ui->algo->currentIndex() == 0) ggggen_5();
    else if (ui->algo->currentIndex() == 1) ggggen_4();
    else if (ui->algo->currentIndex() == 2) ggggen_3();
    else ggggen_2();
    ui->progressBar->hide();
}

string MainWindow::key_translater_3(){
    long long keyNum = 0;
    string keyinput = (ui->keyEdit->text()).toStdString() + "                       ";
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

int rd(int a, int b) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}

void MainWindow::de_2() {
    enList();
    int keyinput = 0;
    int keyEditinput = (ui->keyEdit->text()).toInt();
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
    enList();
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
        exchangeList[i] = (str_long(aes(plain_text, key))%(inputCache.length() / 3)+(inputCache.length() / 3))%(inputCache.length() / 3);
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
                    uint32_t result = ((lgg(de[i]) <= 33) ? 0 : 0x100000), index = 4;
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

void MainWindow::de_4() {
    string key = MainWindow::key_translater_3();
    enList();
    string done;
    long numCount;
    string input = ui->deEdit->toPlainText().toUtf8().constData();
    u32string u32input = ui->deEdit->toPlainText().toStdU32String();
    if (input.find(en[0]) != string::npos || input.find(en[1]) != string::npos || input.find(en[2]) != string::npos ||
        input.find(en[3]) != string::npos || input.find(en[4]) != string::npos || input.find(en[5]) != string::npos) {
        numCount = u32input.size() / 2;
        vector<int> num(numCount);
        vector<long> exchangeList(numCount);
        vector<int> shiftList(numCount);
        ui->progressBar->setRange(0, numCount * 2);

        bool InputCorrect = true;
        for (long i = 0; i < numCount; i++) {
            num[i] = lgg(QString::fromStdU32String(u32input.substr(i * 2, 2)).toStdString());
            if (num[i] == -1) InputCorrect = false;
        }

        // 生成交换表
        long exchangeCount = 0;
        for (int i = 0; exchangeCount < numCount; i++) {
            string plain_text = int_to_chs(i);
            plain_text = string(32 - plain_text.length(), '0') + plain_text;
            string aesResult = aes(plain_text, key);
            int exchangeLength = ceil(log2(static_cast<double>(numCount)) / 4); // 交换表中每个元素的十六进制位数
            for (int j = 0; j <= 32 / exchangeLength && exchangeCount < numCount; j++, exchangeCount++) {
                exchangeList[exchangeCount] = str_long(aesResult.substr(j * exchangeLength, exchangeLength));
				ui->progressBar->setValue(exchangeCount);
            }
        }

        // 进行交换
        for (long i = numCount - 1; i >= 0; i--) {
            long exchangeIndex = exchangeList[i] % numCount;
            swap(num[i], num[exchangeIndex]);
        }

        // 生成偏移表
        long shiftCount = 0;
        for (long i = 0; shiftCount < numCount; i++) {
            string plain_text = int_to_chs(i);
            plain_text = string(32 - plain_text.length(), '0') + plain_text;
            string aesResult = aes(plain_text, key);
            for (int j = 0; j < 32 && shiftCount < numCount; j++, shiftCount++) {
                shiftList[shiftCount] = ch_to_int(aesResult[j]);
                ui->progressBar->setValue(numCount + shiftCount);
            }
        }

        // 进行偏移
        long count = 0; // 索引
        for (long i = 0; i < numCount; i++) {
            if (num[i] < 32) {
                num[i] = awa(num[i] - shiftList[count]);
                count++;
            }
        }

        if (InputCorrect) {
            for (int i = 0; i < numCount; ) {
                if (num[i] >= 32) {
                    i++;
                    uint32_t result = ((num[i] <= 32) ? 0 : 0x100000), index = 4;
                    for (int j = i; j <= i + 4; j++) {
                        result += num[j] * pow16[index];
                        index--;
                    }
                    i += 5;
                    char32_t ch = static_cast<char32_t>(result);
                    QString qstr = QString::fromUcs4(&ch, 1);
                    done += qstr.toStdString();

                }
                else {
                    uint32_t result = 0, index = 3;
                    for (int j = i; j <= i + 3; j++) {
                        result += num[j] * pow16[index];
                        index--;
                    }
                    i += 4;
                    char32_t ch = static_cast<char32_t>(result);
                    QString qstr = QString::fromUcs4(&ch, 1);
                    done += qstr.toStdString();
                }
            }
        }
    }
    else if (input.find(en[6]) != string::npos || input.find(ggggen[6]) != string::npos) {
        numCount = u32input.size() / 4;
        vector<int> num(numCount);
        vector<long> exchangeList(numCount);
        vector<int> shiftList(numCount);
        ui->progressBar->setRange(0, numCount * 2);

        bool InputCorrect = true;
        for (long i = 0; i < numCount; i++) {
            num[i] = gggg(QString::fromStdU32String(u32input.substr(i * 4, 4)).toStdString());
            if (num[i] == -1) InputCorrect = false;
        }

        // 生成交换表
        long exchangeCount = 0;
        for (int i = 0; exchangeCount < numCount; i++) {
            string plain_text = int_to_chs(i);
            plain_text = string(32 - plain_text.length(), '0') + plain_text;
            string aesResult = aes(plain_text, key);
            int exchangeLength = ceil(log2(static_cast<double>(numCount)) / 4); // 交换表中每个元素的十六进制位数
            for (int j = 0; j <= 32 / exchangeLength && exchangeCount < numCount; j++, exchangeCount++) {
                exchangeList[exchangeCount] = str_long(aesResult.substr(j * exchangeLength, exchangeLength));
                ui->progressBar->setValue(exchangeCount);
            }
        }

        // 进行交换
        for (long i = numCount - 1; i >= 0; i--) {
            long exchangeIndex = exchangeList[i] % numCount;
            swap(num[i], num[exchangeIndex]);
        }

        // 生成偏移表
        long shiftCount = 0;
        for (long i = 0; shiftCount < numCount; i++) {
            string plain_text = int_to_chs(i);
            plain_text = string(32 - plain_text.length(), '0') + plain_text;
            string aesResult = aes(plain_text, key);
            for (int j = 0; j < 32 && shiftCount < numCount; j++, shiftCount++) {
                shiftList[shiftCount] = ch_to_int(aesResult[j]);
                ui->progressBar->setValue(numCount + shiftCount);
            }
        }

        // 进行偏移
        for (long i = 0; i < numCount; i++) { num[i] = awa(num[i] - shiftList[i]); }

        if (InputCorrect) {
            for (int i = 0; i < numCount; ) {
                uint32_t result = 0, index = 3;
                for (int j = i; j <= i + 3; j++) {
                    result += num[j] * pow16[index];
                    index--;
                }
                if (result >= 0x2FE0 && result <= 0x2FEF) {
                    result = ((result <= 0x2FE7) ? 0 : 0x100000), index = 4;
                    i += 4;
                    for (int j = i; j <= i + 4; j++) {
                        result += num[j] * pow16[index];
                        index--;
                    }
                    i += 5;
                }
                else {
                    i += 4;
                }
                char32_t ch = static_cast<char32_t>(result);
                QString qstr = QString::fromUcs4(&ch, 1);
                done += qstr.toStdString();
            }
        }
    }
    ui->enEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::de_5() {
    string key = MainWindow::key_translater_3();
    enList();
    string done;
    long numCount;
    vector<int> a;
    string input = ui->deEdit->toPlainText().toUtf8().constData();
    u32string u32input = ui->deEdit->toPlainText().toStdU32String();
    if (input.find(en[0]) != string::npos || input.find(en[1]) != string::npos || input.find(en[2]) != string::npos ||
        input.find(en[3]) != string::npos || input.find(en[4]) != string::npos || input.find(en[5]) != string::npos) {
        ui->progressBar->setRange(0, u32input.size() / 64);
        for (int i = 0; i < u32input.size() / 64; i++){
            string aesResult;
            for(int j = 0; j < 32; j++){
                int num = lgg(QString::fromStdU32String(u32input.substr(i*64+j*2, 2)).toStdString());
                aesResult += (num == 0) ? "0" : int_to_chs(num);
            }
            string plain_text = in_aes(aesResult, key);
            cout<<plain_text<<endl<<aesResult<<endl;
            for(int j = 0; j < 8; j++){
                a.push_back(str_long(plain_text.substr(j*4, 4)));
            }
            ui->progressBar->setValue(i);
        }

        for (int i = 0; i < u32input.size() / 8; ){
            uint32_t result;
            if (a[i] <= 0x2FEF && a[i] >= 0x2FE1){
                result = (a[i] % 16) * 0x10000 + a[i+1];
                i += 2;
            }else if(a[i] == 0x2FE0){
                result = 0x100000 + a[i+1];
                i += 2;
            }else{
                result = a[i];
                i++;
            }
            char32_t ch = static_cast<char32_t>(result);
            QString qstr = QString::fromUcs4(&ch, 1);
            if (result != 0) done += qstr.toStdString();
        }
    }
    else if (input.find(en[6]) != string::npos || input.find(ggggen[6]) != string::npos) {
        ui->progressBar->setRange(0, u32input.size() / 128);
        for (int i = 0; i < u32input.size() / 128; i++){
            string aesResult;
            for(int j = 0; j < 32; j++){
                int num = gggg(QString::fromStdU32String(u32input.substr(i*128+j*4, 4)).toStdString());
                aesResult += (num == 0) ? "0" : int_to_chs(num);
            }
            string plain_text = in_aes(aesResult, key);
            for(int j = 0; j < 8; j++){
                a.push_back(str_long(plain_text.substr(j*4, 4)));
            }
            ui->progressBar->setValue(i);
        }

        for (int i = 0; i < u32input.size() / 16; ){
            uint32_t result;
            if (a[i] <= 0x2FEF && a[i] >= 0x2FE1){
                result = (a[i] % 16) * 0x10000 + a[i+1];
                i += 2;
            }else if(a[i] == 0x2FE0){
                result = 0x100000 + a[i+1];
                i += 2;
            }else{
                result = a[i];
                i++;
            }
            char32_t ch = static_cast<char32_t>(result);
            QString qstr = QString::fromUcs4(&ch, 1);
            if (result != 0) done += qstr.toStdString();
        }
    }
    ui->enEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::lggen_2() {
    enList();
    int keyinput = 0;
    int keyEditinput = (ui->keyEdit->text()).toInt();
    if (keyEditinput >= 0 && keyEditinput <= 65535) keyinput = keyEditinput;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16[i]);
        keyinput -= key[i] * pow16[i];
    }

    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }


    for (int j = 0; j < u32input.length(); j++) {
        if (a[j] > 65536) {
            done += lggen[rd(32, 35)];
            for (int i = 7; i >= 0; i--) {
                int result = a[j] / pow16[i];
                if (i >= 4) result += key[i - 4];
                else result += key[i];
                if (result >= 16) result -= 16;
                done += lggen[result + rd(0, 1) * 16];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }
        else {
            for (int i = 3; i >= 0; i--) {
                int result = a[j] / pow16[i] + key[i];
                if (result >= 16) result -= 16;
                done += lggen[result + rd(0, 1) * 16];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }
    }
    ui->deEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::lggen_3(){
    string key = MainWindow::key_translater_3();
    enList();
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
            done += lggen[(a[j] <= 1048575) ? rd(32, 33) : rd(34, 35)];
            for (int i = 4; i >= 0; i--){
                int result = a[j] / pow16[i];
                done += lggen[result+rd(0, 1)*16];
                a[j] -= pow16[i] * result;
            }
        }else{
            for (int i = 3; i >= 0; i--){
                int result = a[j] / pow16[i];
                done += lggen[result+rd(0, 1)*16];
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
        exchangeList[i] = (str_long(aes(plain_text, key)) %(done.length() / 3)+(done.length() / 3))%(done.length() / 3);
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

void MainWindow::lggen_4() {
    string key = MainWindow::key_translater_3();
    enList();
    // 分割明文
    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<int> a(u32input.length());
    long numCount = 0; // 计算总的数字个数

    for (long i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
		numCount += (a[i] >= 65536) ? 6 : 4;
    }

    ui->progressBar->setRange(0, numCount * 2);
    vector<int> num(numCount);
	long count = 0; // 索引
    for (long j = 0; j < u32input.length(); j++) {
        if (a[j] >= 65536) {
            num[count] = (a[j] <= 1048575) ? rd(32, 33) : rd(34, 35);
            count++;
            for (int i = 4; i >= 0; i--, count++) {
                int result = a[j] / pow16[i];
                num[count] = result;
                a[j] -= pow16[i] * result;
            }
        }
        else {
            for (int i = 3; i >= 0; i--, count++) {
                int result = a[j] / pow16[i];
                num[count] = result;
                a[j] -= pow16[i] * result;
            }
        }
    }
    
    // 生成偏移表
    vector<int> shiftList(numCount);
    long shiftCount = 0;
    for (long i = 0; shiftCount < numCount; i++) {
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
		string aesResult = aes(plain_text, key);
        for (int j = 0; j < 32 && shiftCount < numCount; j++, shiftCount++) {
            shiftList[shiftCount] = ch_to_int(aesResult[j]);
            ui->progressBar->setValue(shiftCount);
        }
    }
	
    // 进行偏移
    count = 0; // 索引
    for (long i = 0; i < numCount; i++) {
        if (num[i] < 32) {
            num[i] = (num[i] + shiftList[count]) % 16;
            count++;
        }
    }

	// 生成交换表
    vector<int> exchangeList(numCount);
    long exchangeCount = 0;
    for (int i = 0; exchangeCount < numCount; i++) {
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        string aesResult = aes(plain_text, key);
        int exchangeLength = ceil(log2(static_cast<double>(numCount)) / 4); // 交换表中每个元素的十六进制位数
        for (int j = 0; j <= 32 / exchangeLength && exchangeCount < numCount; j++, exchangeCount++) {
            exchangeList[exchangeCount] = str_long(aesResult.substr(j * exchangeLength, exchangeLength));
            ui->progressBar->setValue(numCount + exchangeCount);
        }
    }

	// 进行交换
	for (long i = 0; i < numCount; i++) {
		long exchangeIndex = exchangeList[i] % numCount;
		swap(num[i], num[exchangeIndex]);
	}

    // 合并
    string result;
    for (long i = 0; i < numCount; i++) {
        if (num[i] < 32) {
            result += lggen[num[i] + rd(0, 1) * 16];
        }
        else {
            result += lggen[num[i]];
        }
    }

    ui->deEdit->setPlainText(QString::fromUtf8(result));
}

void MainWindow::lggen_5(){
    string key = key_translater_3();
    enList();
    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<int> a;
    string done;
    long numCount = 0; // 计算总的数字个数

    for (long i = 0; i < u32input.length(); i++) {
        int ch = static_cast<uint32_t>(u32input[i]);
        if (ch >= 65536) {
            numCount += 2;
            a.push_back(0x2FE0 + (ch/0x10000)%10);
        }else{
            numCount++;
        }
        a.push_back(ch%0x10000);
    }

    for (int i = 0; i < (8-numCount%8)%8; i++){
        a.push_back(0);
    }
    ui->progressBar->setRange(0, static_cast<int>(a.size())/8);

    for (int i = 0; i < static_cast<int>(a.size())/8; i++){
        string plain_text = "";
        for(int j = 0; j < 8; j++){
            string chs = int_to_chs(a[8*i+j]);
            chs = string(4 - chs.length(), '0') + chs;
            plain_text += chs;
        }
        string aesResult = aes(plain_text, key);
        for(int j = 0; j < 32; j++){
            done += lggen[ch_to_int(aesResult[j]) + rd(0, 1)*16];
        }
        ui->progressBar->setValue(i);
    }

    ui->deEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::ggggen_2(){
    enList();
    int keyinput = 0;
    int keyEditinput = (ui->keyEdit->text()).toInt();
    if (keyEditinput >= 0 && keyEditinput <= 65535) keyinput = keyEditinput;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16[i]);
        keyinput -= key[i] * pow16[i];
    }


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
            done += ggggen[awa(2+key[3])]+ggggen[awa(15+key[2])]+ggggen[awa(14+key[1])]+ggggen[rd(0, 15)];
            for (int i = 7; i >= 0; i--){
                int result = a[j] / pow16[i];
                if (i >= 4) result += key[i - 4];
                else result += key[i];
                if (result >= 16) result -= 16;
                done += ggggen[result];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }else{
            for (int i = 3; i >= 0; i--){
                int result = a[j] / pow16[i] + key[i];
                if (result >= 16) result -= 16;
                done += ggggen[result];
                a[j] -= pow16[i] * ((int)(a[j] / pow16[i]));
            }
        }
    }
    ui->deEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::ggggen_3() {
    string key = MainWindow::key_translater_3();
    enList();
    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<long> a(u32input.length());
    string done;

    for (int i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }


    for (int j = 0; j < u32input.length(); j++) {
        if (a[j] >= 65536) {
            done += ggggen[2] + ggggen[15] + ggggen[14] + ggggen[(a[j] <= 1048575) ? rd(0, 7) : rd(8, 15)];
            for (int i = 4; i >= 0; i--) {
                int result = a[j] / pow16[i];
                done += ggggen[result];
                a[j] -= pow16[i] * result;
            }
        }
        else {
            for (int i = 3; i >= 0; i--) {
                int result = a[j] / pow16[i];
                done += ggggen[result];
                a[j] -= pow16[i] * result;
            }
        }
    }

    ui->progressBar->setRange(0, done.length() / 3);
    vector<string> donechar(done.length() / 3);
    vector<int> exchangeList(done.length() / 3);
    for (int i = 0; i < done.length() / 3; i++) {
        // 分割密文
        donechar[i] = done.substr(i * 3, 3);
        // 生成交换表
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        exchangeList[i] = (str_long(aes(plain_text, key)) % (done.length() / 3) + (done.length() / 3)) % (done.length() / 3);
        ui->progressBar->setValue(i);
    }
    // 交换
    for (int i = 0; i < done.length() / 3; i++) {
        string exchangeCache1 = donechar[i];
        string exchangeCache2 = donechar[exchangeList[i]];
        donechar[i] = exchangeCache2;
        donechar[exchangeList[i]] = exchangeCache1;
    }
    // 合并
    string result = "";
    for (int i = 0; i < done.length() / 3; i++) {
        result += donechar[i];
    }
    ui->deEdit->setPlainText(QString::fromUtf8(result));
}

void MainWindow::ggggen_4() {
    string key = MainWindow::key_translater_3();
    MainWindow::enList();
    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<int> a(u32input.length());
    string done;
    long numCount = 0; // 计算总的数字个数

    for (long i = 0; i < u32input.length(); i++) {
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
        numCount += (a[i] >= 65536) ? 9 : 4;
    }

    ui->progressBar->setRange(0, numCount * 2);
    vector<int> num(numCount);
    long count = 0; // 索引
    for (int j = 0; j < u32input.length(); j++) {
        if (a[j] >= 65536) {
            num[count] = 2, num[count + 1] = 15, num[count + 2] = 14, num[count + 3] = (a[j] <= 1048575) ? rd(0, 7) : rd(8, 15);
            count += 4;
            for (int i = 4; i >= 0; i--, count++) {
                int result = a[j] / pow16[i];
                num[count] = result;
                a[j] -= pow16[i] * result;
            }
        }
        else {
            for (int i = 3; i >= 0; i--, count++) {
                int result = a[j] / pow16[i];
                num[count] = result;
                a[j] -= pow16[i] * result;
            }
        }
    }

    // 生成偏移表
    vector<int> shiftList(numCount);
    long shiftCount = 0;
    for (long i = 0; shiftCount < numCount; i++) {
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        string aesResult = aes(plain_text, key);
        for (int j = 0; j < 32 && shiftCount < numCount; j++, shiftCount++) {
            shiftList[shiftCount] = ch_to_int(aesResult[j]);
            ui->progressBar->setValue(shiftCount);
        }
    }

    // 进行偏移
    for (long i = 0; i < numCount; i++) { num[i] = (num[i] + shiftList[i]) % 16; }

    // 生成交换表
    vector<int> exchangeList(numCount);
    long exchangeCount = 0;
    for (int i = 0; exchangeCount < numCount; i++) {
        string plain_text = int_to_chs(i);
        plain_text = string(32 - plain_text.length(), '0') + plain_text;
        string aesResult = aes(plain_text, key);
        int exchangeLength = ceil(log2(static_cast<double>(numCount)) / 4); // 交换表中每个元素的十六进制位数
        for (int j = 0; j <= 32 / exchangeLength && exchangeCount < numCount; j++, exchangeCount++) {
            exchangeList[exchangeCount] = str_long(aesResult.substr(j * exchangeLength, exchangeLength));
            ui->progressBar->setValue(numCount + exchangeCount);
        }
    }

    // 进行交换
    for (long i = 0; i < numCount; i++) {
        long exchangeIndex = exchangeList[i] % numCount;
        swap(num[i], num[exchangeIndex]);
    }

    // 合并
    string result;
    for (long i = 0; i < numCount; i++) { result += ggggen[num[i]]; }

    ui->deEdit->setPlainText(QString::fromUtf8(result));
}

void MainWindow::ggggen_5(){
    string key = key_translater_3();
    enList();
    QString qinput = ui->enEdit->toPlainText();
    u32string u32input = qinput.toStdU32String();
    vector<int> a;
    string done;
    long numCount = 0; // 计算总的数字个数

    for (long i = 0; i < u32input.length(); i++) {
        int ch = static_cast<uint32_t>(u32input[i]);
        if (ch >= 65536) {
            numCount += 2;
            a.push_back(0x2FE0 + (ch/0x10000)%10);
        }else{
            numCount++;
        }
        a.push_back(ch%0x10000);
    }

    for (int i = 0; i < (8-numCount%8)%8; i++){
        a.push_back(0);
    }
    ui->progressBar->setRange(0, static_cast<int>(a.size())/8);

    for (int i = 0; i < static_cast<int>(a.size())/8; i++){
        string plain_text = "";
        for(int j = 0; j < 8; j++){
            string chs = int_to_chs(a[8*i+j]);
            chs = string(4 - chs.length(), '0') + chs;
            plain_text += chs;
        }
        string aesResult = aes(plain_text, key);
        for(int j = 0; j < 32; j++){
            done += ggggen[ch_to_int(aesResult[j])];
        }
        ui->progressBar->setValue(i);
    }

    ui->deEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::enDelete_clicked() { ui->enEdit->clear(); }
void MainWindow::deDelete_clicked() { ui->deEdit->clear(); }

void MainWindow::exchange_clicked(){
    QString enEdit_cache = ui->enEdit->toPlainText();
    QString deEdit_cache = ui->deEdit->toPlainText();
    ui->enEdit->setPlainText(deEdit_cache);
    ui->deEdit->setPlainText(enEdit_cache);
}

void MainWindow::enCopy_clicked() { QApplication::clipboard()->setText(ui->enEdit->toPlainText()); }
void MainWindow::deCopy_clicked() { QApplication::clipboard()->setText(ui->deEdit->toPlainText()); }
