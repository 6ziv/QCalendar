#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.connect(&a,SIGNAL(lastWindowClosed()),&a,SLOT(quit()));
    MainWindow w;
    w.setAttribute(Qt::WA_QuitOnClose, true);
    w.show();

    return a.exec();
}
