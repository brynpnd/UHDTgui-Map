#include "mainwindow.h"
#include "terminal.h"
#include "settingsdialog.h"
#include "console.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();

    return a.exec();
}
