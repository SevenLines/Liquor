#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    /*QTranslator translator;
    bool r = translator.load("main.qm");
    a.installTranslator(&translator);*/
    
    MainWindow w(argv[1]);
    
    w.show();

    return a.exec();
}
