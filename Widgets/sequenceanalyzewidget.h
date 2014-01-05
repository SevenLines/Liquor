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
    void addKeyPoints(KeyPoints* keyPoints)
    {
        multiKeyPoints.addSet(keyPoints);
    }
    
    /// очистка списка наборов
    /// force - удаление из памяти прямо сейчас
    void clearKeyPoints(bool force = false);
    void removeSet(KeyPoints *keyPoints, bool force = false);
    bool isContains(KeyPoints* keyPoints);
    
    
private:
    Ui::SequenceAnalyzeWidget *ui;

    MultiKeyPoints multiKeyPoints;
    
    QCPGraph *graph;
    QCPPlotTitle *title;
    QCPItemText *label;
    
signals:
    void graphUpdated();
    
public slots:
    void updateGraph();
    void changeScale(int value);
    void plottableClick(QCPAbstractPlottable*p ,QMouseEvent*e);
    void plottableMouseMove(QMouseEvent*e);
};

#endif // SEQUENCEANALYZEWIDGET_H
