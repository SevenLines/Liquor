#ifndef QIMAGEBROWSER_H
#define QIMAGEBROWSER_H

#include <QWidget>

namespace Ui {
class QImageBrowser;
}

class QImageBrowser : public QWidget
{
    Q_OBJECT
    
public:
    explicit QImageBrowser(QWidget *parent = 0);
    ~QImageBrowser();
    
private:
    Ui::QImageBrowser *ui;
};

#endif // QIMAGEBROWSER_H
