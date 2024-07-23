#include "mainwin.h"

#include <QApplication>
#include <QtHelp/QtHelp>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //加载样式表
//    QFile file(":/res/qss/ManjaroMix.qss");
//    if(!file.open(QIODevice::ReadOnly)){
//        qDebug() << ("QSS加载失败");
//    }

//    auto qss = file.readAll();
//    file.close();
//    qApp->setStyleSheet(qss);
//    qApp->processEvents();
    MainWin w;
    w.show();
    return a.exec();
}
