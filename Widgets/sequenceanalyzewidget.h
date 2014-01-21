#ifndef SEQUENCEANALYZEWIDGET_H
#define SEQUENCEANALYZEWIDGET_H

#include <QSettings>
#include <QWidget>
#include <QMenu>
#include "qcustomplot.h"
#include "multikeypoints.h"
#include "multikeypointsmodel.h"

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

    /// удалить набор
    void removeSet(KeyPoints *keyPoints, bool force = false);
    /// проверят содержиться ли набор 
    bool isContains(KeyPoints* keyPoints);
    
    
private:
    Ui::SequenceAnalyzeWidget *ui;

    MultiKeyPoints multiKeyPoints;
    MultiKeyPointsModel multiKeyPointsModel;
    
    QMenu lstSetsContextMenu;
    
    QCPGraph *graph;
    QCPPlotTitle *title;
    QCPItemText *label;
    
    QString lastPath;
    
signals:
    
    /// график обновился
    void graphUpdated();
    /// был актирован набор точек
    void keyPointsSetActivated(KeyPoints *keyPoints);
    /// посылает сигнал о том что виджет хочет чтобы к нему добавили точки
    void addKeyPointsToGraph();
    
public slots:
    /// очистка списка наборов
    /// force - удаление из памяти прямо сейчас
    void clearKeyPoints(bool force = false);
    /// обновить график
    void updateGraph();
    /// установить кол-во точек на графике (шкалу)
    void changeScale(int value);
    void rescaleGraphAxis(bool fFit);
    
    void setActive(KeyPoints *keyPoints);
    void savePoints();
    void loadPoints();
    
    void plottableClick(QCPAbstractPlottable*p ,QMouseEvent*e);
    void plottableMouseMove(QMouseEvent*e);
    
    void saveIni(QSettings *ini);
    void loadIni(QSettings *ini);
    
    void setAddParticlesButtonText(QString text);
    void toggleAddParticleButtonEnbaled(bool fEnabled);
    
    // удаляет набор под номером index
    void remove(int index);
    // удаляет текущий набор
    void removeCurrentSet();
    
private slots:
    void on_lstSets_activated(const QModelIndex &index);
};

#endif // SEQUENCEANALYZEWIDGET_H
