#include "sequenceanalyzewidget.h"
#include "ui_sequenceanalyzewidget.h"
#include <QDebug>

SequenceAnalyzeWidget::SequenceAnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequenceAnalyzeWidget)
{
    ui->setupUi(this);
    
    // setup multiKeyPoints
    multiKeyPoints.setScale(25);
    connect(&multiKeyPoints, SIGNAL(graphChanged()),
            SLOT(updateGraph()));
    connect(&multiKeyPoints, SIGNAL(graphChanged()),
            SIGNAL(graphUpdated()));
    
    // setup graph style
    graph = ui->plotGraph->addGraph();
    graph->setPen(QPen( QColor(30, 40, 255)));
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 
                                          QColor(30, 40, 255) , Qt::white, 4));
    
    /*label = new QCPItemText(ui->plotGraph);
    label->setText("info");
    label->setPositionAlignment( Qt::AlignLeft | Qt::AlignBottom);
    label->setPen(QPen());*/
    
    //ui->plotGraph->addItem(label);
    
    // add header title
    title = new QCPPlotTitle(ui->plotGraph, "");
    
    // setup plot
    ui->plotGraph->plotLayout()->insertRow(0);
    ui->plotGraph->plotLayout()->addElement( 0,0, title );
    ui->plotGraph->setInteractions(
                QCP::iRangeDrag | QCP::iRangeZoom );
    
    ui->plotGraph->yAxis->setLabel(tr("count"));
    ui->plotGraph->xAxis->setLabel(tr("size (px.)"));
    ui->plotGraph->yAxis->setRange(0,1);
    
    connect(ui->plotGraph, SIGNAL(mouseMove(QMouseEvent*)),
            SLOT(plottableMouseMove(QMouseEvent*)));
    
    connect(ui->plotGraph, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)),
            SLOT(plottableClick(QCPAbstractPlottable*,QMouseEvent*)));
}

SequenceAnalyzeWidget::~SequenceAnalyzeWidget()
{
    delete ui;
}

void SequenceAnalyzeWidget::clearKeyPoints(bool force)
{
    multiKeyPoints.clearSets(force);
}

void SequenceAnalyzeWidget::removeSet(KeyPoints *keyPoints, bool force)
{
    multiKeyPoints.removeSet(keyPoints, force);
}

bool SequenceAnalyzeWidget::isContains(KeyPoints *keyPoints)
{
    return multiKeyPoints.isContains(keyPoints);
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

void SequenceAnalyzeWidget::plottableClick(QCPAbstractPlottable *p, QMouseEvent *e)
{
    
}

void SequenceAnalyzeWidget::plottableMouseMove(QMouseEvent *e)
{ 
}
