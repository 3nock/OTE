#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QMenuBar>

#include "model/ProjectModel.h"


#define SITEMAP_TYPE "smt"


namespace Ui {
class Project;
}

class Project : public QWidget{
        Q_OBJECT

    public:
        Project(QWidget *parent = nullptr, ProjectModel *projectModel = nullptr);
        ~Project();

        void initProject();

    private slots:
        void on_treeViewExplorer_clicked(const QModelIndex &index);
        void on_lineEditFilter_textChanged(const QString &arg1);
        void on_treeViewTree_clicked(const QModelIndex &index);

    private:
        Ui::Project *ui;

        ProjectModel *model;
        QSortFilterProxyModel *proxyModel;

        QString m_projectFile;
        QString *item_comment = nullptr;

        QMenuBar *menubar_tree;
        QMenuBar *menubar_notes;
        QMenuBar *menubar_project;

        /* main actions */
        QAction a_copy;
        QAction a_save;
        QAction a_send;
        QAction a_clear;
        QAction a_expand;
        QAction a_collapse;

        void initUI();
        void init_menubar_tree();
        void init_menubar_project();

        /* menu bar's actions */
        void action_save();
        void action_clear();
        void action_copy();
        void action_send();
};

#endif // S3SPROJECT_H
