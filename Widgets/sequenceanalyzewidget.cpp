#include "sequenceanalyzewidget.h"
#include "ui_sequenceanalyzewidget.h"
#include <QDebug>
#include <QFileDialog>

SequenceAnalyzeWidget::SequenceAnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequenceAnalyzeWidget)
{
    ui->setupUi(this);
    
    // setup multiKeyPoints
    connect(ui->sldScale, SIGNAL(valueChanged(int)),
            &multiKeyPoints, SLOT(setScale(int)));
    connect(&multiKeyPoints, SIGNAL(graphChanged()),
            SLOT(updateGraph()));
    connect(&multiKeyPoints, SIGNAL(graphChanged()),
            SIGNAL(graphUpdated()));

    
    // setup multiKeyPoints model
    multiKeyPointsModel.setData(&multiKeyPoints);
    ui->lstSets->setModel(&multiKeyPointsModel);
    
    // отмечаем актывный набор в списке
    connect(this, SIGNAL(keyPointsSetActivated(KeyPoints*)),
            &multiKeyPointsModel, SLOT(setActive(KeyPoints*)));
    
    // setup graph style
    graph = ui->plotGraph->addGraph();
    graph->setPen(QPen( QColor(0, 0, 0)));
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 
                                          QColor(30, 40, 255) , Qt::white, 4));
    graph->setBrush(QBrush(QColor(30, 40, 255, 128)));
    
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
    
    ui->plotGraph->xAxis->setPadding(5);
    ui->plotGraph->yAxis->setPadding(5);
    
    // rescale event
    connect(ui->btnFitToScreen, SIGNAL(clicked(bool)),
            SLOT(rescaleGraphAxis(bool)));
    
    // connect graphic events
    connect(ui->plotGraph, SIGNAL(mouseMove(QMouseEvent*)),
            SLOT(plottableMouseMove(QMouseEvent*)));
    connect(ui->plotGraph, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)),
            SLOT(plottableClick(QCPAbstractPlottable*,QMouseEvent*)));
    
    // connect load / save buttons
    connect(ui->btnSave, SIGNAL(clicked()), SLOT(savePoints()));
    connect(ui->btnLoad, SIGNAL(clicked()), SLOT(loadPoints()));
            
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
    title->setText(QString(tr("count: %1")).arg(multiKeyPoints.countOfParticles()));
    ui->plotGraph->replot();
    if (ui->btnFitToScreen->isChecked()) {
        rescaleGraphAxis(true);
    }
}

void SequenceAnalyzeWidget::changeScale(int value)
{
    multiKeyPoints.setScale(value);
}

void SequenceAnalyzeWidget::rescaleGraphAxis(bool fFit)
{
    if (fFit) {
        ui->plotGraph->rescaleAxes();
        ui->plotGraph->replot();
    }
}

void SequenceAnalyzeWidget::setActive(KeyPoints *keyPoints)
{
    multiKeyPointsModel.setActive(keyPoints);
}

void SequenceAnalyzeWidget::savePoints()
{
    QString savePath = QFileDialog::getSaveFileName(this, 
                                tr("select dump file"), 
                                lastPath,
                                tr("Text files (*.txt)"));
    if (!savePath.isNull()) {
        multiKeyPoints.saveDumpToFile(savePath);
        lastPath = savePath;
    }
}

void SequenceAnalyzeWidget::loadPoints()
{
    QString loadPath = QFileDialog::getOpenFileName(this, 
                                tr("select dump file"), 
                                lastPath,
                                tr("Text files (*.txt)"));
    if (!loadPath.isNull()) {
        multiKeyPoints.loadDumpFromFile(loadPath);
        lastPath = loadPath;
    }
}

void SequenceAnalyzeWidget::plottableClick(QCPAbstractPlottable *p, QMouseEvent *e)
{
    
}

void SequenceAnalyzeWidget::plottableMouseMove(QMouseEvent *e)
{ 
}

void SequenceAnalyzeWidget::saveIni(QSettings *ini)
{
    ini->beginGroup("SequenceAnalyzeWidget");
    ini->setValue("lastPath", lastPath);
    ini->setValue("sldScale/Value", ui->sldScale->value());
    ini->setValue("fitGraphic", ui->btnFitToScreen->isChecked());
    ini->endGroup();
}

void SequenceAnalyzeWidget::loadIni(QSettings *ini)
{
    ini->beginGroup("SequenceAnalyzeWidget");
    lastPath = ini->value("lastPath", QString()).toString();
    ui->sldScale->setValue(
                ini->value("sldScale/Value", 25).toInt());
    ui->btnFitToScreen->setChecked(
                ini->value("fitGraphic", true).toBool());
    ini->endGroup();
}

void SequenceAnalyzeWidget::on_lstSets_activated(const QModelIndex &index)
{
    emit keyPointsSetActivated(multiKeyPointsModel.data(index));
}
