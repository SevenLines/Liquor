#include "imageprocesspreviewermodel.h"

ImageProcessPreviewerModel::ImageProcessPreviewerModel(ImageProcessPreviewer *preview, QObject *parent)
    : QAbstractListModel(parent)
{
    previewer = preview;
    itemsPerRow = 4;
}

void ImageProcessPreviewerModel::setPreviewer(ImageProcessPreviewer *preview)
{
    beginResetModel();
    this->previewer = preview;
    endResetModel();
}

int ImageProcessPreviewerModel::rowCount(const QModelIndex &) const
{
    if (!this->previewer) return 0;
    int count = this->previewer->count();
    int value = count % itemsPerRow;
    if (value) value = 1;
    return count / itemsPerRow + 1 * value;
}

int ImageProcessPreviewerModel::columnCount(const QModelIndex &) const
{
    if (!this->previewer) return 0;
    return itemsPerRow;
}

QVariant ImageProcessPreviewerModel::data(const QModelIndex &index, int role) const
{
    int num = index.row() * itemsPerRow + index.column();
    int count = this->previewer->count();
    if (num >= count)
        return QVariant();
    
    if (role == Qt::DisplayRole) {
        return QVariant::fromValue(this->previewer->image(num));
    }
    
    return QVariant();
}
