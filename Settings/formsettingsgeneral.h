#ifndef FORMSETTINGSGENERAL_H
#define FORMSETTINGSGENERAL_H

#include <QWidget>

namespace Ui {
class FormSettingsGeneral;
}

class FormSettingsGeneral : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormSettingsGeneral(QWidget *parent = 0);
    ~FormSettingsGeneral();
    
private:
    Ui::FormSettingsGeneral *ui;
};

#endif // FORMSETTINGSGENERAL_H
