#include "mainwindow.h"
#include "ApplicationInfo.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>
#include <QFile>

bool dontLog = false;
void logMsgHandler(QtMsgType, const char *);

ApplicationInfo applicationInfo;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::addLibraryPath("qt");
    
    // установка логера
    qInstallMsgHandler(logMsgHandler);
    qDebug() << app.tr("begin session");
    
    // установка языка приложения
    QTranslator myappTranslator;
    myappTranslator.load(applicationInfo.translationPath);
    app.installTranslator(&myappTranslator); 
    
    QTranslator qtTranslator;
    qtTranslator.load(applicationInfo.qtTranslationPath);
    app.installTranslator(&qtTranslator);
    
    // открытие главного окна приложения
    MainWindow w(argv[1]);
    w.show();
 
    
    // запуск приложения
    int result = app.exec();
    
    // конец сессии
    qDebug() << app.tr("end session");
    
    return result;
}

// main logger for application
void logMsgHandler(QtMsgType type, const char *message)
{
    if (!dontLog) {
        QString str = QString::fromLocal8Bit(message);
        QString messageType;
        
        // флаг чтобы избежать возможного зацикливания
        dontLog = true;
        
        // передаем сообщение главному окну
        if (applicationInfo.mainWindow) {
            applicationInfo.mainWindow->log(str, type);
        }
        
        // префикс типа сообщения
        switch(type) {
        case QtDebugMsg:
            messageType = QCoreApplication::tr("DEBUG");
            break;
        case QtWarningMsg:
            messageType = QCoreApplication::tr("WARNING");
            break;
        case QtCriticalMsg:
            messageType = QCoreApplication::tr("CRITICAL");
            break;
        case QtFatalMsg:
            messageType = QCoreApplication::tr("FATAL");
            break;
        }
        
       std::cout << str.toLocal8Bit().constData() << std::endl;
        
        // запись в лог
        QFile file(applicationInfo.logFilePath);
        if (file.open( QIODevice::Append | QIODevice::Text)) {
            QTextStream sw(&file);
            sw << QString(QCoreApplication::tr("[%1][%2] %3"))
                  .arg(QDateTime::currentDateTime()
                       .toString(QCoreApplication::tr("hh:mm dd.MM.yyyy"))) // время события
                  .arg(messageType) // тип
                  .arg(str) // сообщение
               << endl;
            file.close();
        }
        
        // сброс антизацикливающего флага
        dontLog = false;
    }
}
