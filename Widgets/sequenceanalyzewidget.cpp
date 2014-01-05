#include "sequenceanalyzewidget.h"
#include "ui_sequenceanalyzewidget.h"

SequenceAnalyzeWidget::SequenceAnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequenceAnalyzeWidget)
{
    ui->setupUi(this);
    
    multiKeyPoints.setScale(25);
    connect(&multiKeyPoints, SIGNAL(graphChanged()),
            SLOT(updateGraph()));
    
    graph = ui->plotGraph->addGraph();
    graph->setPen(QPen( QColor(30, 40, 255)));
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 
                                          QColor(30, 40, 255) , Qt::white, 4));
    
    
    ui->plotGraph->yAxis->setLabel(tr("count"));
    ui->plotGraph->xAxis->setLabel(tr("size (px.)"));
    ui->plotGraph->yAxis->setRange(0,1);
    
    title = new QCPPlotTitle(ui->plotGraph, "");
    
    ui->plotGraph->plotLayout()->insertRow(0);
    ui->plotGraph->plotLayout()->addElement( 0,0, title );
    
    ui->plotGraph->setInteractions(
                QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

SequenceAnalyzeWidget::~SequenceAnalyzeWidget()
{
    delete ui;
}

void SequenceAnalyzeWidget::addKeyPoints(KeyPoints *keyPoints)
{
    multiKeyPoints.addSet(keyPoints);
}

void SequenceAnalyzeWidget::clearKeyPoints(bool force)
{
    multiKeyPoints.clearSets(force);
}

void SequenceAnalyzeWidget::updateGraph()
{
    graph->setData(multiKeyPoints.keys(), multiKeyPoints.values());
    title->setText(QString(tr("count: %1")).arg(multiKeyPoints.count()));
    ui->plotGraph->rescaleAxes();
    ui->plotGraph->replot();
}

void SequenceAnalyzeWidget::changeScale(int value)
{
    multiKeyPoints.setScale(value);
}
