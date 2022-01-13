/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef IPTOOL_H
#define IPTOOL_H

#include <QWidget>
#include "src/modules/passive/OsintModulesHeaders.h"

namespace Ui {
    class IpTool;
}

class IpTool : public QWidget{
    Q_OBJECT

    public:
        explicit IpTool(QWidget *parent = nullptr);
        ~IpTool();

    signals:
        void stopScanThread();
        void pauseScanThread();

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
        Ui::IpTool *ui;
        IpModel *m_model;
};

#endif // IPTOOL_H
