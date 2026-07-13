#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
