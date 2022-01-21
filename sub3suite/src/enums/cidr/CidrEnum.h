/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef CIDRENUM_H
#define CIDRENUM_H

#include "../AbstractEnum.h"
#include "src/models/CIDRModel.h"
#include "src/modules/passive/OsintModulesHeaders.h"


namespace Ui {
class CidrEnum;
}

class CidrEnum : public AbstractEnum{
        Q_OBJECT

    public:
        CidrEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~CidrEnum();

    public slots:
        void onResult(CidrModelStruct results);
        /* ... */
        void onEnumerationComplete();
        void onErrorLog(ScanLog log);
        void onInfoLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();

    private:
        Ui::CidrEnum *ui;
        CidrModel *m_model;
};

#endif // CIDRENUM_H
