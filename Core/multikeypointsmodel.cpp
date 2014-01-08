#include "multikeypointsmodel.h"
#include <QDebug>

MultiKeyPointsModel::MultiKeyPointsModel(MultiKeyPoints *data, QObject *parent) :
    QAbstractListModel(parent)
{
    // init values
    multiKeyPoints = 0;
    
    setData(data);
}

void MultiKeyPointsModel::setData(MultiKeyPoints *data)
{
    if (data != multiKeyPoints) {
        beginResetModel();
        if (multiKeyPoints) {
            multiKeyPoints->disconnect(this);
            multiKeyPoints = 0;
        }
        if (data) {
            multiKeyPoints = data;
            connect(multiKeyPoints, SIGNAL(graphChanged()),
                    SLOT(resetModel()));
        }
        endResetModel();
    }
}

void MultiKeyPointsModel::resetModel()
{
    beginResetModel();
    endResetModel();
}

void MultiKeyPointsModel::setActive(KeyPoints *keyPoints)
{
    beginResetModel();
    keyPointsActive = keyPoints;
    endResetModel();
}


int MultiKeyPointsModel::rowCount(const QModelIndex &parent) const
{
    return multiKeyPoints?multiKeyPoints->count():0;
}

QVariant MultiKeyPointsModel::data(const QModelIndex &index, int role) const
{
    if(!multiKeyPoints) 
        return QVariant();
    if(!index.isValid()) 
        return QVariant();
    if(index.row() >= multiKeyPoints->count()) 
        return QVariant();
    
    KeyPoints *kp = multiKeyPoints->at(index.row());
    
    switch(role) {
    case Qt::DisplayRole:
        if (keyPointsActive == kp) {
            return QString(tr(">>> %1").arg(kp->title()));
        } else {
            return kp->title();
        }
        break;
    case Qt::CheckStateRole:
        return kp->isEnabled();
    }
    return QVariant();
}

KeyPoints *MultiKeyPointsModel::data(const QModelIndex &index)
{
    if(!multiKeyPoints) 
        return 0;
    if(!index.isValid()) 
        return 0;
    if(index.row() >= multiKeyPoints->count()) 
        return 0;
    return multiKeyPoints->at(index.row());
}

bool MultiKeyPointsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!multiKeyPoints) 
        return 0;
    if(!index.isValid()) 
        return 0;
    if(index.row() >= multiKeyPoints->count()) 
        return 0;
    switch(role) {
    case Qt::CheckStateRole:
        bool state = multiKeyPoints->at(index.row())->isEnabled();
        multiKeyPoints->at(index.row())->setEnabled(!state);
        break;
    }

    return false;
}


Qt::ItemFlags MultiKeyPointsModel::flags(const QModelIndex &index) const
{   
    return Qt::ItemIsUserCheckable |  Qt::ItemIsEnabled;
}
