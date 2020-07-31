#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<TextEditor>();
    qRegisterMetaType<QList<TextEditor>>();
    MainWindow w;
    w.show();

    return a.exec();
}
