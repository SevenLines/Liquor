#ifndef IMAGEPROCESSPREVIEWERMODEL_H
#define IMAGEPROCESSPREVIEWERMODEL_H

#include <QAbstractTableModel>
#include "imageprocesspreviewer.h"

class ImageProcessPreviewerModel : public QAbstractListModel
{
    Q_OBJECT
    ImageProcessPreviewer *previewer;
    int itemsPerRow;
public:
    explicit ImageProcessPreviewerModel(ImageProcessPreviewer *preview = 0, QObject *parent = 0);
    
signals:
    
public slots:
    void setPreviewer(ImageProcessPreviewer *preview);

public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
};

#endif // IMAGEPROCESSPREVIEWERMODEL_H
