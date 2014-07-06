#include <QApplication>
#include "cwindow_main.h"

int main(int argc, char *argv[])
{
    //char *argv2[] = {"QtFunctionBool","-platformpluginpath", "c:/Qt/Qt5.1.1/5.1.1/mingw48_32/plugins/platforms",NULL};
    //int argc2 = sizeof(argv2) / sizeof(char*) - 1;
    QApplication a(argc, argv);
    CWindowMain w;
    w.showMaximized();

    return a.exec();
}
