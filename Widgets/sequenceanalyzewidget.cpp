#include "sequenceanalyzewidget.h"
#include "ui_sequenceanalyzewidget.h"

SequenceAnalyzeWidget::SequenceAnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SequenceAnalyzeWidget)
{
    ui->setupUi(this);
}

SequenceAnalyzeWidget::~SequenceAnalyzeWidget()
{
    delete ui;
}
