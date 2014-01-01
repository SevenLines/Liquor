#ifndef SEQUENCEANALYZEWIDGET_H
#define SEQUENCEANALYZEWIDGET_H

#include <QWidget>

namespace Ui {
class SequenceAnalyzeWidget;
}

class SequenceAnalyzeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SequenceAnalyzeWidget(QWidget *parent = 0);
    ~SequenceAnalyzeWidget();
    
private:
    Ui::SequenceAnalyzeWidget *ui;
};

#endif // SEQUENCEANALYZEWIDGET_H
