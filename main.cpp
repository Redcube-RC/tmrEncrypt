#include "mainwindow.h"
#include "cli.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char* argv[])
{
    // 有参数 → CLI 模式：不初始化 GUI，命令行工具不该拖着一整套图形环境
    if (argc > 1) {
        QCoreApplication a(argc, argv);
        return runCli(argc, argv, a);
    }

    // 无参数 → GUI 模式
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
