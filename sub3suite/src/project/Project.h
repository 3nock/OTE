#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QMenuBar>

#include "src/models/ProjectModel.h"

#define PROJECT_MODEL_TYPE "pmt"


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
        void on_lineEditFilter_textChanged(const QString &arg1);

    private:
        void m_initUI();
        void init_sitemapMenuBar();
        void init_notesMenuBar();
        Ui::Project *ui;
        QString m_projectFile;
        ProjectModel *m_projectModel;
        QSortFilterProxyModel *m_proxyModel;

        /* menu bars */
        void action_save();
        void action_clear();
        void action_copy();
        void action_send();
        QMenuBar *m_sitemapMenuBar;
        QMenuBar *m_notesMenuBar;
};

#endif // S3SPROJECT_H
