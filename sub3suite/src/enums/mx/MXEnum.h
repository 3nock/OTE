/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef MXENUM_H
#define MXENUM_H

#include "../AbstractEnum.h"
#include "src/models/MXModel.h"
#include "src/modules/passive/OsintModulesHeaders.h"


namespace Ui {
class MXEnum;
}

class MXEnum : public AbstractEnum {
        Q_OBJECT

    public:
        MXEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~MXEnum();

    public slots:
        void onResultsMX(MXModelStruct results);
        //...
        void onEnumerationComplete();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();

    private:
        Ui::MXEnum *ui;
        MXModel *m_model;
};

#endif // MXENUM_H
