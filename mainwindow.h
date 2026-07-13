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
