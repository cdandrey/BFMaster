#include <QApplication>
#include <QFile>
#include <QBitArray>
#include "cwindow_main.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file("://qss/stylesheet.qss");

    if (file.open(QIODevice::ReadOnly))
        a.setStyleSheet(file.readAll());

    CWindowMain w;
    w.showMaximized();

//    qDebug() << "size unsigned short" << sizeof(unsigned short);
//    qDebug() << "size int" << sizeof(int);
//    qDebug() << "size unsigned long" << sizeof(unsigned long);
//    qDebug() << "size unsigned long long" << sizeof(unsigned long long);
//    qDebug() << "size unsigned double" << sizeof(double);

//    short arr[] = {1,2,3,4,5};
//    char ch[] = "123456789";
//    QChar qch = '1';
//    QString str("12");
//    QByteArray by("12");
//    QBitArray bi(16,false);

//    qDebug() << "size int[]" << sizeof(arr);
//    qDebug() << "size char" << sizeof(ch);
//    qDebug() << "size QChar" << sizeof(qch);
//    qDebug() << "size QString" << sizeof(str.at(0));
//    qDebug() << "size QByteArray" << sizeof(by.at(0));
//    qDebug() << "size QBitArray" << sizeof(bi);

    return a.exec();
}
