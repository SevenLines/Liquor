#ifndef APPLICATIONINFO_H
#define APPLICATIONINFO_H

#include <QString>

class MainWindow;

class ApplicationInfo {

public:
    /// имя приложения
    QString name;
    
    /// путь к файлу журналу
    QString logFilePath;
    
    /// путь к файлу с переводом
    QString translationPath;
    QString qtTranslationPath;
    
    /// ссылка на главное окно, используется логером, 
    /// обнуляется при закрытии главного окна
    MainWindow *mainWindow;

    ApplicationInfo() {
        name = "liqour"; 
        logFilePath = QString("%1.log").arg(name);
        translationPath = QString("%1_%2").arg(name).arg(QLocale::system().name());
        qtTranslationPath = QString("qt_%1").arg(QLocale::system().name());
        mainWindow = 0;
    }
};

#endif // APPLICATIONINFO_H
