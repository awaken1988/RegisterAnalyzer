//TODO: http://stackoverflow.com/questions/2750005/testing-with-qts-qtestlib-module

#include "mainwindow.h"
#include "bitconverter.h"
#include <QApplication>
#include <QLineEdit>
#include <QBitArray>
#include "analyzerwidget.h"
#include  "mainwindow.h"

int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);

    MainWindow* mainWindow = new MainWindow;

    mainWindow->show();

    //analyzerWidget->show();



    return a.exec();
}
