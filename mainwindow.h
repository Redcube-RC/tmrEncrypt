#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::string key_translater_3();
    void de_2();
    void de_3();
	void de_4();
    void lggen_2();
    void lggen_3();
    void lggen_4();
    void ggggen_2();
    void ggggen_3();
    void ggggen_4();
    int lgg(std::string a);
	int gggg(std::string a);
    void enList();
	void setEnabled(bool key3);

    std::string lggen[36] = { "灵灵","灵感","灵菇","灵哩",
                              "感灵","感感","感菇","感哩",
                              "菇灵","菇感","菇菇","菇哩",
                              "哩灵","哩感","哩菇","哩哩",
                              "灵刮","灵擦","感刮","感擦",
                              "菇刮","菇擦","哩刮","哩擦",
                              "刮灵","刮感","刮菇","刮哩",
                              "擦灵","擦感","擦菇","擦哩",
                              "刮刮","刮擦","擦刮","擦擦" };
    std::string ggggen[16] = { "咕咕咕咕","咕咕咕嘎","咕咕嘎咕","咕咕嘎嘎",
                               "咕嘎咕咕","咕嘎咕嘎","咕嘎嘎咕","咕嘎嘎嘎",
                               "嘎咕咕咕","嘎咕咕嘎","嘎咕嘎咕","嘎咕嘎嘎",
                               "嘎嘎咕咕","嘎嘎咕嘎","嘎嘎嘎咕","嘎嘎嘎嘎" };
    std::string en[8] = {"灵","感","菇","哩","刮","擦","咕","嘎"};

    bool define = true;

public slots:
    void de_clicked();
    void deCopy_clicked();
    void deDelete_clicked();
    void lggen_clicked();
    void ggggen_clicked();
    void enCopy_clicked();
    void enDelete_clicked();
    void exchange_clicked();

private:
    Ui::MainWindow *ui;  
};
#endif // MAINWINDOW_H
