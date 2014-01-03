#include "imagestack.h"


ImageStack::ImageStack(QObject *parent) :
    QAbstractListModel(parent)
{
}


int ImageStack::rowCount(const QModelIndex &) const
{
    return stack.count();
}

QVariant ImageStack::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();
    if(index.row() >= stack.count()) return QVariant();
    
    switch(role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return stack.value(index.row()).Title;
        break;
    }
    return QVariant();
}

PixmapInfo ImageStack::data(const QModelIndex &index)
{
    return stack.value(index.row());
}

QVariant ImageStack::headerData(int section, Qt::Orientation, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        switch(section) {
        case 0:
            return tr("image title");
        }
        break;
    }
    return QVariant();
}

QPixmap ImageStack::pop()
{
    beginResetModel();
    
    PixmapInfo out = stack.pop(); 
    
    endResetModel();
    
    return out.Pixmap;
}

void ImageStack::push(QPixmap pixmap, QString title, int pos)
{
    beginResetModel();
    
    switch (pos) {
    case -1:
        stack.push(PixmapInfo(title, pixmap));
        break;
    default:
        while(stack.count() > pos + 1) {
            stack.pop();
        }
        stack.push(PixmapInfo(title, pixmap));
        break;
    }

    endResetModel();
}


bool ImageStack::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole) {
        stack[index.row()].Title = value.toString();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags ImageStack::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;
    
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}


PixmapInfo::PixmapInfo()
{  
    PixmapInfo("", QPixmap());
}

PixmapInfo::PixmapInfo(QString title, QPixmap pixmap)
{
    this->Title = title;
    this->Pixmap = pixmap;
}
