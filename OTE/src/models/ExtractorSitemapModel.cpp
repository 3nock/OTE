#include "ExtractorSitemapModel.h"

#include "src/template/Template.h"

OTE::SitemapModel::Extractor::Extractor():
    proxy(new QSortFilterProxyModel)
{
    foreach(const QString &modelName, OTE::gInputOutput.outputTypes)
    {
        QPair<QString,QStandardItemModel*> modelPair;

        QStandardItemModel *model = new QStandardItemModel;
        model->setHorizontalHeaderLabels({modelName});

        modelPair.first = modelName;
        modelPair.second = model;

        models << modelPair;
    }

    proxy->setFilterKeyColumn(0);
}

OTE::SitemapModel::Extractor::~Extractor()
{
    for(const QPair<QString,QStandardItemModel*> &model: models)
        delete model.second;

    delete proxy;
}

void OTE::SitemapModel::Extractor::clear()
{
    for(const QPair<QString,QStandardItemModel*> &model: models)
    {
        model.second->clear();
        model.second->setHorizontalHeaderLabels({model.first});
    }
}

void OTE::SitemapModel::Extractor::addModel(const QString &modelName)
{
    for(const QPair<QString,QStandardItemModel*> &model: models)
    {
        if(model.first.compare(modelName, Qt::CaseInsensitive) == 0)
            return;
    }

    QStandardItemModel *newModel = new QStandardItemModel;
    models.append(QPair<QString,QStandardItemModel*>{modelName, newModel});
}

void OTE::SitemapModel::Extractor::setCurrentModel(const QString &modelName)
{
    if(currentModelName.compare(modelName, Qt::CaseInsensitive) == 0)
        return;

    for(const QPair<QString,QStandardItemModel*> &model: models)
    {
        if(model.first.compare(modelName, Qt::CaseInsensitive) == 0)
        {
            currentModelName = model.first;
            currentModel = model.second;
            proxy->setSourceModel(currentModel);
        }
    }
}

QStandardItemModel *OTE::SitemapModel::Extractor::getModel(const QString &modelName)
{
    for(const QPair<QString,QStandardItemModel*> &model: models)
    {
        if(model.first.compare(modelName, Qt::CaseInsensitive) == 0)
            return model.second;
    }

    // if model isn't available, create, add and return a new model

    QStandardItemModel *newModel = new QStandardItemModel;
    models.append(QPair<QString,QStandardItemModel*>{modelName, newModel});
    return newModel;
}
