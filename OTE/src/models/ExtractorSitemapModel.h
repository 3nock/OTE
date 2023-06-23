#ifndef MODELS_EXTRACTORSITEMAPMODEL_H
#define MODELS_EXTRACTORSITEMAPMODEL_H

#include <QString>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

namespace OTE {

namespace SitemapModel {

class Extractor
{
public:
    Extractor();
    ~Extractor();

public:
    QSortFilterProxyModel *proxy;
    QList<QPair<QString,QStandardItemModel* > > models;

    QString currentModelName;
    QStandardItemModel *currentModel = nullptr;

    void clear();
    void addModel(const QString &modelName);
    void setCurrentModel(const QString &modelName);
    QStandardItemModel *getModel(const QString &modelName);
};

} // namespace SitemapModel

} // namespace OTE

#endif // MODELS_EXTRACTORSITEMAPMODEL_H
