/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef IPENUM_H
#define IPENUM_H

#include "../AbstractEnum.h"
#include "src/modules/passive/OsintModulesHeaders.h"


namespace Ui {
    class IpEnum;
}

class IpEnum : public AbstractEnum{
    Q_OBJECT

    public:
        IpEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~IpEnum();

    public slots:
        void onResult();
        //...
        void onEnumerationComplete();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();

    private:
        Ui::IpEnum *ui;
};

#endif // IPENUM_H
