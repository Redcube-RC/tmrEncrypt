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

#include <QString>
#include <QApplication>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QClipboard>
#include <QLineEdit>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::u32string tou32(const QString& qstr) {
    return qstr.toStdU32String();
}

long pow16(int i) {
    long result = 1;
    for (int j = 1; j <= i; j++) {
        result *= 16;
    }
    return result;
}

int lgg(std::string a) {
    std::string en[36] = {
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

int gggg(std::string a) {
    std::string en[16] = {
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
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}

void MainWindow::de_clicked() {
    int keyinput = 0;
    int keyEditinput = (ui->keyEdit->text()).toInt();
    if (keyEditinput >= 0 && keyEditinput <= 65535) keyinput = keyEditinput;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16(i));
        keyinput -= key[i] * pow16(i);
        std::cout<<key[i]<<std::endl;
    }

    std::string done;
    QString qinput = ui->deEdit->toPlainText();
    std::string input = qinput.toUtf8().constData();
    bool InputCorrect = true;
    if (input.find("灵") != std::string::npos || input.find("感") != std::string::npos || input.find("菇") != std::string::npos ||
        input.find("擦") != std::string::npos || input.find("刮") != std::string::npos || input.find("哩") != std::string::npos) {
        std::cout << "lgg" << input.length() << std::endl;
        if (input.length() % 6 != 0) InputCorrect = false;
        else {
            std::string de[input.length() / 6];
            for (int i = 0; i < input.length() / 6; i++) {
                de[i] = input.substr(6 * i, 6);
                std::cout<<de[i]<<std::endl;
                if (lgg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 6; ) {
                if (lgg(de[i]) >= 32) {
                    i++;
                    uint32_t result = 0, index = 7;
                    for (int j = i; j <= i + 7; j++) {
                        if (index >= 4) result += awa(lgg(de[j])-key[index-4]) * pow16(index);
                        else result += awa(lgg(de[j])-key[index]) * pow16(index);
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
                        result += awa(lgg(de[j])-key[index]) * pow16(index);
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
    else if (input.find("咕") != std::string::npos || input.find("嘎") != std::string::npos) {
        std::cout << "gggg" << input.length() << std::endl;
        if (input.length() % 12 != 0) InputCorrect = false;
        else {
            std::string de[input.length() / 12];
            for (int i = 0; i < input.length() / 12; i++) {
                de[i] = input.substr(12 * i, 12);
                std::cout<<de[i]<<gggg(de[i])<<std::endl;
                if (gggg(de[i]) == -1) InputCorrect = false;
            }
            for (int i = 0; i < input.length() / 12; ) {
                uint32_t result = 0, index = 3;
                for (int j = i; j <= i + 3; j++) {
                    result += awa(gggg(de[j])-key[index]) * pow16(index);
                    index--;
                }
                if (result >= 12256 && result <= 12271) {
                    result = 0, index = 7;
                    i += 4;
                    for (int j = i; j <= i + 7; j++) {
                        if (index >= 4) result += awa(gggg(de[j])-key[index-4]) * pow16(index);
                        else result += awa(gggg(de[j])-key[index]) * pow16(index);
                        index--;
                    }
                    i += 8;
                }
                else {
                    i += 4;
                }std::cout<<result<<std::endl;
                if (InputCorrect) {
                    char32_t ch = static_cast<char32_t>(result);
                    QString qstr = QString::fromUcs4(&ch, 1);
                    done += qstr.toStdString();
                }
            }
        }
    }
    std::cout << done << std::endl;
    ui->enEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::lggen_clicked(){
    int keyinput = 0;
    int keyEditinput = (ui->keyEdit->text()).toInt();
    if (keyEditinput >= 0 && keyEditinput <= 65535) keyinput = keyEditinput;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16(i));
        keyinput -= key[i] * pow16(i);
        std::cout<<key[i]<<std::endl;
    }

    std::string en[36] = {
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
    std::u32string u32input = qinput.toStdU32String();
    long a[u32input.length()];
    std::string done;

    for (int i = 0; i < u32input.length(); i++){
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }


    for (int j = 0; j < u32input.length(); j++){
        if (a[j] > 65536) {
            done += en[rd(32, 35)];
            for (int i = 7; i >= 0; i--){
                int result = a[j] / pow16(i);
                if (i >= 4) result += key[i - 4];
                else result += key[i];
                if (result >= 16) result -= 16;
                done += en[result+rd(0, 1)*16];
                a[j] -= pow16(i) * ((int)(a[j] / pow16(i)));
            }
        }else{
            for (int i = 3; i >= 0; i--){
                int result = a[j] / pow16(i) + key[i];
                if (result >= 16) result -= 16;
                done += en[result+rd(0, 1)*16];
                a[j] -= pow16(i) * ((int)(a[j] / pow16(i)));
                std::cout<<result<<std::endl;
            }
        }
    }
    ui->deEdit->setPlainText(QString::fromUtf8(done));
}

void MainWindow::ggggen_clicked(){
    int keyinput = 0;
    int keyEditinput = (ui->keyEdit->text()).toInt();
    if (keyEditinput >= 0 && keyEditinput <= 65535) keyinput = keyEditinput;
    int key[4];
    for (int i = 3; i >= 0; i--) {
        key[i] = (int)(keyinput / pow16(i));
        keyinput -= key[i] * pow16(i);
        std::cout<<key[i]<<std::endl;
    }

    std::string en[16] = {
        "咕咕咕咕", "咕咕咕嘎", "咕咕嘎咕", "咕咕嘎嘎",
        "咕嘎咕咕", "咕嘎咕嘎", "咕嘎嘎咕", "咕嘎嘎嘎",
        "嘎咕咕咕", "嘎咕咕嘎", "嘎咕嘎咕", "嘎咕嘎嘎",
        "嘎嘎咕咕", "嘎嘎咕嘎", "嘎嘎嘎咕", "嘎嘎嘎嘎"
    };

    QString qinput = ui->enEdit->toPlainText();
    std::u32string u32input = qinput.toStdU32String();
    long a[u32input.length()];
    std::string done;

    for (int i = 0; i < u32input.length(); i++){
        char32_t ch = u32input[i];
        a[i] = static_cast<uint32_t>(ch);
    }


    for (int j = 0; j < u32input.length(); j++){
        std::cout<<a[j]<<std::endl;
        if (a[j] > 65536){
            done += en[awa(2+key[3])]+en[awa(15+key[2])]+en[awa(14+key[1])]+en[rd(0, 15)];
            for (int i = 7; i >= 0; i--){
                int result = a[j] / pow16(i);
                if (i >= 4) result += key[i - 4];
                else result += key[i];
                if (result >= 16) result -= 16;
                done += en[result];
                a[j] -= pow16(i) * ((int)(a[j] / pow16(i)));
            }
        }else{
            for (int i = 3; i >= 0; i--){
                int result = a[j] / pow16(i) + key[i];
                if (result >= 16) result -= 16;
                done += en[result];
                a[j] -= pow16(i) * ((int)(a[j] / pow16(i)));
            }
        }
    }
    ui->deEdit->setPlainText(QString::fromUtf8(done));
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
