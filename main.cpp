#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    /*QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
             QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);*/
    
    QTranslator myappTranslator;
    myappTranslator.load("liqour_" + QLocale::system().name());
    app.installTranslator(&myappTranslator); 
    
    MainWindow w(argv[1]);
    
    w.show();

    return app.exec();
}
