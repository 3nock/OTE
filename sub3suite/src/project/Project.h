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
        void initUI();
        void initActions();
        void init_sitemapMenuBar();
        void init_notesMenuBar();
        void initMenuBar_project();
        Ui::Project *ui;

        ProjectModel *model;
        QSortFilterProxyModel *proxyModel;

        QString m_projectFile;

        QString *item_comment = nullptr;

        /* menu bars */
        void action_save();
        void action_clear();
        void action_copy();
        void action_send();
        QMenuBar *m_sitemapMenuBar;
        QMenuBar *m_notesMenuBar;
        QMenuBar *m_menuBar_project;
        QAction a_copy;
        QAction a_save;
        QAction a_send;
        QAction a_clear;
        QAction a_expand;
        QAction a_collapse;
};

#endif // S3SPROJECT_H
