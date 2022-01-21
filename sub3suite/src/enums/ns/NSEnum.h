/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef NSENUM_H
#define NSENUM_H

#include "../AbstractEnum.h"
#include "src/models/NSModel.h"
#include "src/modules/passive/OsintModulesHeaders.h"


namespace Ui {
class NSEnum;
}

class NSEnum : public AbstractEnum {
        Q_OBJECT

    public:
        NSEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~NSEnum();

    public slots:
        void onResultsNS(NSModelStruct results);
        //...
        void onEnumerationComplete();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();

    private:
        Ui::NSEnum *ui;
        NSModel *m_model;
};

#endif // NSENUM_H
