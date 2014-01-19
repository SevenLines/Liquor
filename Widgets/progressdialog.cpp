#include "progressdialog.h"
#include "ui_progressdialog.h"

#include <QMovie>
#include <QDebug>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QMessageBox>

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    //ui->wdgAnimate->init(, QSize());
    
    movieIcon = new QMovie(":images/loading5.gif");
    movieIcon->setParent(this);

    
    movieIconSmall= new QMovie(":images/loading2.gif");
    movieIconSmall->setParent(this);

    // необходимо чтобы правильно персчиталась геометрия объекта
    movieIcon->start();
    movieIcon->stop();
    movieIconSmall->start();
    movieIconSmall->stop();
    
    connect(ui->btnCancel, SIGNAL(clicked()),
            SIGNAL(rejected()));
    
    ui->lblIcon2->setMovie(movieIconSmall);
    ui->lblIcon->setMovie(movieIcon);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::setLabel(QString label)
{
    ui->lblLabel->setText(label);
}

QString ProgressDialog::label()
{
    return ui->lblLabel->text();
}

void ProgressDialog::showCancelButton(bool fShow)
{
    ui->btnCancel->setVisible(fShow);
}

void ProgressDialog::showProgressBar(bool fShow)
{
    ui->progressBar->setVisible(fShow);
}

void ProgressDialog::setProgress(int value)
{
    ui->progressBar->setValue(value);
}

void ProgressDialog::setProgress(int value, int max, QString label)
{
    setMax(max);
    setProgress(value);
    if (!label.isNull()) setLabel(label);
}

void ProgressDialog::setMax(int value)
{
    ui->progressBar->setMaximum(value);
}

void ProgressDialog::setMin(int value)
{
    ui->progressBar->setMinimum(value);
}

int ProgressDialog::min()
{
    return ui->progressBar->minimum();
}

int ProgressDialog::max()
{
    return ui->progressBar->maximum();
}

int ProgressDialog::progress()
{
    return ui->progressBar->value();
}


void ProgressDialog::showEvent(QShowEvent *)
{
    // двигаю в центр
    QRect g = geometry();
    QRect desktopG = QApplication::desktop()->geometry();
    g.moveCenter(desktopG.center());
    setGeometry(g);
    
    // запускаю анимацию
    movieIconSmall->start();
    movieIcon->start();
}

void ProgressDialog::hideEvent(QHideEvent *)
{
    // останавливаю анимацию
    movieIconSmall->stop();
    movieIcon->stop();
}


void ProgressDialog::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()) {
    case Qt::Key_Return:
        break;
    default:
        QDialog::keyPressEvent(e);
    }
}


void ProgressDialog::reject()
{
    if ( QMessageBox::question(this,
           "Confirmation",
           tr("Are u sure u want to stop current process?"),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QDialog::reject();
    }
}


void ProgressDialog::accept()
{
    qDebug() << "accept";
}
