#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include "src/models/ProjectModel.h"


namespace Ui {
class Project;
}

class Project : public QWidget{
        Q_OBJECT

    public:
        Project(QWidget *parent = nullptr, ProjectModel *projectModel = nullptr);
        ~Project();

private slots:
        void on_treeViewProjectExplorer_clicked(const QModelIndex &index);

private:
        Ui::Project *ui;
        ProjectModel *m_projectModel;
        QSortFilterProxyModel *m_siteMapProxyModel;
};

#endif // S3SPROJECT_H
