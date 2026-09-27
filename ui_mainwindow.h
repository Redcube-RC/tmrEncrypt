/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *lggen;
    QPushButton *ggggen;
    QTextEdit *enEdit;
    QTextEdit *deEdit;
    QPushButton *de;
    QPushButton *deCopy;
    QPushButton *exchange;
    QPushButton *deDelete;
    QPushButton *enDelete;
    QPushButton *enCopy;
    QLabel *labeltmr3;
    QLineEdit *keyEdittmr2;
    QRadioButton *radioButtontmr2;
    QRadioButton *radioButtontmr3;
    QLabel *label_2;
    QLabel *labeltmr2;
    QLineEdit *keyEdittmr3;
    QProgressBar *progressBar;
    QRadioButton *radioButtontmr4;
    QLineEdit *e1;
    QLineEdit *e2;
    QLineEdit *e3;
    QLineEdit *e4;
    QLineEdit *e5;
    QLineEdit *e6;
    QLineEdit *e7;
    QLineEdit *e8;
    QLabel *labellgg;
    QLabel *labelgggg;
    QLabel *labeldefine;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(657, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lggen = new QPushButton(centralwidget);
        lggen->setObjectName("lggen");
        lggen->setGeometry(QRect(10, 280, 101, 23));
        ggggen = new QPushButton(centralwidget);
        ggggen->setObjectName("ggggen");
        ggggen->setGeometry(QRect(120, 280, 111, 23));
        enEdit = new QTextEdit(centralwidget);
        enEdit->setObjectName("enEdit");
        enEdit->setGeometry(QRect(10, 10, 471, 261));
        deEdit = new QTextEdit(centralwidget);
        deEdit->setObjectName("deEdit");
        deEdit->setGeometry(QRect(10, 310, 471, 261));
        de = new QPushButton(centralwidget);
        de->setObjectName("de");
        de->setGeometry(QRect(410, 280, 75, 23));
        deCopy = new QPushButton(centralwidget);
        deCopy->setObjectName("deCopy");
        deCopy->setGeometry(QRect(490, 500, 75, 23));
        exchange = new QPushButton(centralwidget);
        exchange->setObjectName("exchange");
        exchange->setGeometry(QRect(330, 280, 75, 23));
        deDelete = new QPushButton(centralwidget);
        deDelete->setObjectName("deDelete");
        deDelete->setGeometry(QRect(490, 470, 75, 23));
        enDelete = new QPushButton(centralwidget);
        enDelete->setObjectName("enDelete");
        enDelete->setGeometry(QRect(490, 50, 75, 23));
        enCopy = new QPushButton(centralwidget);
        enCopy->setObjectName("enCopy");
        enCopy->setGeometry(QRect(490, 80, 75, 23));
        labeltmr3 = new QLabel(centralwidget);
        labeltmr3->setObjectName("labeltmr3");
        labeltmr3->setGeometry(QRect(500, 260, 171, 16));
        keyEdittmr2 = new QLineEdit(centralwidget);
        keyEdittmr2->setObjectName("keyEdittmr2");
        keyEdittmr2->setGeometry(QRect(500, 190, 113, 21));
        radioButtontmr2 = new QRadioButton(centralwidget);
        radioButtontmr2->setObjectName("radioButtontmr2");
        radioButtontmr2->setGeometry(QRect(490, 150, 95, 19));
        radioButtontmr3 = new QRadioButton(centralwidget);
        radioButtontmr3->setObjectName("radioButtontmr3");
        radioButtontmr3->setGeometry(QRect(490, 220, 95, 19));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(490, 130, 71, 16));
        labeltmr2 = new QLabel(centralwidget);
        labeltmr2->setObjectName("labeltmr2");
        labeltmr2->setGeometry(QRect(500, 170, 141, 16));
        keyEdittmr3 = new QLineEdit(centralwidget);
        keyEdittmr3->setObjectName("keyEdittmr3");
        keyEdittmr3->setGeometry(QRect(500, 280, 113, 21));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(490, 560, 171, 23));
        progressBar->setValue(24);
        radioButtontmr4 = new QRadioButton(centralwidget);
        radioButtontmr4->setObjectName("radioButtontmr4");
        radioButtontmr4->setGeometry(QRect(490, 240, 95, 19));
        e1 = new QLineEdit(centralwidget);
        e1->setObjectName("e1");
        e1->setGeometry(QRect(500, 360, 21, 23));
        e2 = new QLineEdit(centralwidget);
        e2->setObjectName("e2");
        e2->setGeometry(QRect(520, 360, 21, 23));
        e3 = new QLineEdit(centralwidget);
        e3->setObjectName("e3");
        e3->setGeometry(QRect(540, 360, 21, 23));
        e4 = new QLineEdit(centralwidget);
        e4->setObjectName("e4");
        e4->setGeometry(QRect(560, 360, 21, 23));
        e5 = new QLineEdit(centralwidget);
        e5->setObjectName("e5");
        e5->setGeometry(QRect(580, 360, 21, 23));
        e6 = new QLineEdit(centralwidget);
        e6->setObjectName("e6");
        e6->setGeometry(QRect(600, 360, 21, 23));
        e7 = new QLineEdit(centralwidget);
        e7->setObjectName("e7");
        e7->setGeometry(QRect(500, 410, 21, 23));
        e8 = new QLineEdit(centralwidget);
        e8->setObjectName("e8");
        e8->setGeometry(QRect(520, 410, 21, 23));
        labellgg = new QLabel(centralwidget);
        labellgg->setObjectName("labellgg");
        labellgg->setGeometry(QRect(500, 340, 141, 16));
        labelgggg = new QLabel(centralwidget);
        labelgggg->setObjectName("labelgggg");
        labelgggg->setGeometry(QRect(500, 390, 161, 16));
        labeldefine = new QLabel(centralwidget);
        labeldefine->setObjectName("labeldefine");
        labeldefine->setGeometry(QRect(500, 320, 171, 16));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\351\253\230\346\235\276\347\201\257\346\226\207\346\234\254\345\212\240\345\257\206\345\231\2504.2", nullptr));
        lggen->setText(QCoreApplication::translate("MainWindow", "\347\201\265\346\204\237\350\217\207\345\212\240\345\257\206\346\263\225\342\206\223", nullptr));
        ggggen->setText(QCoreApplication::translate("MainWindow", "\345\222\225\345\222\225\345\230\216\345\230\216\345\212\240\345\257\206\346\263\225\342\206\223", nullptr));
        deEdit->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        de->setText(QCoreApplication::translate("MainWindow", "\350\247\243\345\257\206\342\206\221", nullptr));
        deCopy->setText(QCoreApplication::translate("MainWindow", "\344\270\200\351\224\256\345\244\215\345\210\266", nullptr));
        exchange->setText(QCoreApplication::translate("MainWindow", "\344\272\244\346\215\242\342\206\225", nullptr));
        deDelete->setText(QCoreApplication::translate("MainWindow", "\346\270\205\351\231\244", nullptr));
        enDelete->setText(QCoreApplication::translate("MainWindow", "\346\270\205\351\231\244", nullptr));
        enCopy->setText(QCoreApplication::translate("MainWindow", "\344\270\200\351\224\256\345\244\215\345\210\266", nullptr));
        labeltmr3->setText(QCoreApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\345\257\206\351\222\245\357\274\210\350\213\261\346\226\207\346\225\260\345\255\227\357\274\211\357\274\232", nullptr));
        radioButtontmr2->setText(QCoreApplication::translate("MainWindow", "tmr2", nullptr));
        radioButtontmr3->setText(QCoreApplication::translate("MainWindow", "tmr3", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\345\212\240\345\257\206\346\226\271\345\274\217\357\274\232", nullptr));
        labeltmr2->setText(QCoreApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\345\257\206\351\222\245\357\274\2100~65535\357\274\211\357\274\232", nullptr));
        radioButtontmr4->setText(QCoreApplication::translate("MainWindow", "tmr4", nullptr));
        labellgg->setText(QCoreApplication::translate("MainWindow", "\346\236\204\346\210\220\347\201\265\346\204\237\350\217\207\345\274\217\345\257\206\346\226\207\347\232\204\345\205\255\345\255\227\357\274\232", nullptr));
        labelgggg->setText(QCoreApplication::translate("MainWindow", "\346\236\204\346\210\220\345\222\225\345\222\225\345\230\216\345\230\216\345\274\217\345\257\206\346\226\207\347\232\204\344\270\244\345\255\227\357\274\232", nullptr));
        labeldefine->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
