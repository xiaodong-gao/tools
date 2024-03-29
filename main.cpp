#include "Dialog/mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 获取屏幕的几何信息
    QDesktopWidget desktopWidget;
    QRect screenGeometry = desktopWidget.availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    // 计算窗口的大小和位置
    int windowWidth = screenWidth / 2;
    int windowHeight = screenHeight / 2;
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;
    MainWindow w;
    w.setWindowTitle("tools");
    w.setGeometry(x, y, windowWidth, windowHeight);
    w.show();
    return a.exec();
}
