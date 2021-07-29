#ifndef PROJECT_H
#define PROJECT_H

#include "src/core.h"

namespace Ui {
    class Project;
}

class Project : public QWidget{
        Q_OBJECT

    private:
        Ui::Project *ui;

    public:
        void updateFilter();
        //...
        ResultsModel *m_resultsModel;
        QSortFilterProxyModel *m_proxyModel;

    public:
        explicit Project(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Project();

    private slots:
        void on_pushButton_clearInScope_clicked();
        void on_pushButton_removeInScope_clicked();
        void on_pushButton_loadInScope_clicked();
        void on_pushButton_addInScope_clicked();
        void on_lineEdit_inScope_returnPressed();
        void on_checkBox_enableFilter_clicked(bool checked);
        void on_checkBox_columnIpAddress_clicked(bool checked);
        void on_checkBox_columnScopeTarget_clicked(bool checked);
};

#endif // PROJECT_H
