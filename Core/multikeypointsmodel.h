#ifndef MULTIKEYPOINTSMODEL_H
#define MULTIKEYPOINTSMODEL_H

#include <QAbstractListModel>
#include "multikeypoints.h"

class MultiKeyPointsModel : public QAbstractListModel
{
    Q_OBJECT
private:
    MultiKeyPoints *multiKeyPoints;
    KeyPoints *keyPointsActive;
    
public:
    explicit MultiKeyPointsModel(MultiKeyPoints *data = 0, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    KeyPoints *data(const QModelIndex &index);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    
signals:
    
public slots:
    void setData(MultiKeyPoints *data);
    void resetModel();
    
    // костыль для того чтобы было видно активный набор
    void setActive(KeyPoints *keyPoints);
    
    void removeSetUnderCursor(QPoint point);
};

#endif // MULTIKEYPOINTSMODEL_H
