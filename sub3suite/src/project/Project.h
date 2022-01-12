/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef PROJECT_H
#define PROJECT_H

#include <QSortFilterProxyModel>
#include "ProjectDataModel.h"
#include "src/utils/utils.h"
#include "general/GeneralAnalysis.h"
#include "specific/SpecificAnalysis.h"

namespace Ui {
    class Project;
}

class Project : public QWidget{
        Q_OBJECT

    public:
        Project(QWidget *parent = nullptr, ProjectDataModel *projectDataModel = nullptr);
        ~Project();
        ///
        /// ...
        ///
        void updateFilter();
        GeneralAnalysis *general = nullptr;
        SpecificAnalysis *specific = nullptr;

    private slots:
        void on_pushButton_clearInScope_clicked();
        void on_pushButton_removeInScope_clicked();
        void on_pushButton_loadInScope_clicked();
        void on_pushButton_addInScope_clicked();
        void on_lineEdit_inScope_returnPressed();
        void on_checkBox_enableFilter_clicked(bool checked);
        void on_checkBox_columnIpAddress_clicked(bool checked);
        void on_checkBox_columnScopeTarget_clicked(bool checked);
        void on_pushButton_filter_clicked();

    private:
        Ui::Project *ui;
        ProjectDataModel *m_projectDataModel;
        QSortFilterProxyModel *m_proxyModel;
};

#endif // PROJECT_H
