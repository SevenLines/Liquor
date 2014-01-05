#ifndef SEQUENCEANALYZEWIDGET_H
#define SEQUENCEANALYZEWIDGET_H

#include <QWidget>
#include "qcustomplot.h"
#include "multikeypoints.h"

namespace Ui {
class SequenceAnalyzeWidget;
}

class SequenceAnalyzeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SequenceAnalyzeWidget(QWidget *parent = 0);
    ~SequenceAnalyzeWidget();
    
    /// добавляет список точек
    void addKeyPoints(KeyPoints* keyPoints);
    
    /// очистка списка наборов
    /// force - удаление из памяти прямо сейчас
    void clearKeyPoints(bool force = false);
    
private:
    Ui::SequenceAnalyzeWidget *ui;

    MultiKeyPoints multiKeyPoints;
    
    QCPGraph *graph;
    QCPPlotTitle *title;
    
public slots:
    void updateGraph();
    void changeScale(int value);
};

#endif // SEQUENCEANALYZEWIDGET_H
