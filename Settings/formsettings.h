#ifndef FORMSETTINGS_H
#define FORMSETTINGS_H

#include <QWidget>

namespace Ui {
class FormSettings;
}

class FormSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormSettings(QWidget *parent = 0);
    ~FormSettings();
    
private:
    Ui::FormSettings *ui;
};

#endif // FORMSETTINGS_H
