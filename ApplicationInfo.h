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
    
    QString helpFilePath;
    
    /// ссылка на главное окно, используется логером, 
    /// обнуляется при закрытии главного окна
    MainWindow *mainWindow;

    ApplicationInfo() {
        name = "liqour"; 
        logFilePath = QString("%1.log").arg(name);
        translationPath = QString("%1_%2").arg(name).arg(QLocale::system().name());
        qtTranslationPath = QString("qt_%1").arg(QLocale::system().name());
        helpFilePath = "documentation/index.html";
        mainWindow = 0;
    }
    
    bool isMoveCameraButtons(Qt::MouseButtons buttons) {
        return buttons.testFlag(Qt::MidButton) &
                !buttons.testFlag(Qt::LeftButton) &
                !buttons.testFlag(Qt::RightButton);
    }
    
    bool isMoveObjectButtons(Qt::MouseButtons buttons) {
        return !buttons.testFlag(Qt::MidButton) &
                buttons.testFlag(Qt::LeftButton) &
                !buttons.testFlag(Qt::RightButton);
    }
    
    bool isContextButtons(Qt::MouseButtons buttons) {
        return !buttons.testFlag(Qt::MidButton) &
                !buttons.testFlag(Qt::LeftButton) &
                buttons.testFlag(Qt::RightButton);
    }
};

#endif // APPLICATIONINFO_H
