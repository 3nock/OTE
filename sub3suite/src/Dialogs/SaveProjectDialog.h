#ifndef SAVEPROJECTDIALOG_H
#define SAVEPROJECTDIALOG_H

#include <QDialog>

#include "src/models/ProjectModel.h"


namespace Ui {
class SaveProjectDialog;
}

class SaveProjectDialog : public QDialog{
        Q_OBJECT

    public:
        SaveProjectDialog(ProjectModel *projectModel = nullptr, QWidget *parent = nullptr);
        ~SaveProjectDialog();

    private slots:
        void on_buttonSave_clicked();
        void on_buttonCancel_clicked();
        void on_buttonChoosePath_clicked();

    private:
        Ui::SaveProjectDialog *ui;
        ProjectModel *m_projectModel;
};

#endif // SAVEPROJECTDIALOG_H
