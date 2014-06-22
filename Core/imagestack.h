#ifndef IMAGESTACK_H
#define IMAGESTACK_H

#include <QAbstractListModel>
#include <QPixmap>
#include <QStack>

struct PixmapInfo
{
    QString Title;
    QPixmap Pixmap;
    bool keyImage;
    
    PixmapInfo();
    PixmapInfo(QString title, QPixmap pixmap);
};

class ImageStack : public QAbstractListModel
{
    Q_OBJECT
    
    
private:
    QStack<PixmapInfo> stack;
public:
    explicit ImageStack(QObject *parent = 0);
    
signals:
    
public slots:
    
    
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    PixmapInfo data(const QModelIndex &index);
    QVariant headerData(int section, Qt::Orientation, int role) const;
    
    QPixmap pop();
    void push(QPixmap pixmap, QString title, bool asKeyImage=false, int pos=-1);
    
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // IMAGESTACK_H
