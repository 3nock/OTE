#ifndef PROJECTCONFIGDIALOG_H
#define PROJECTCONFIGDIALOG_H

#include <QMap>
#include <QDialog>

#include "src/project/ProjectModel.h"

namespace Ui {
class ProjectConfigDialog;
}

class ProjectConfigDialog : public QDialog{
        Q_OBJECT

    public:
        ProjectConfigDialog(ProjectModel *projectModel = nullptr, QWidget *parent = nullptr);
        ~ProjectConfigDialog();

    private slots:
        void on_buttonCancel_clicked();
        void on_buttonOk_clicked();
        void on_buttonChoosePath_clicked();

    private:
        Ui::ProjectConfigDialog *ui;
        ProjectModel *m_projectModel;
};

#endif // PROJECTCONFIGDIALOG_H
