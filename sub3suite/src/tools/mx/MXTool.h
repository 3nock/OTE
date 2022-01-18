/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef MXTOOL_H
#define MXTOOL_H

#include <QWidget>
#include "src/models/MXModel.h"
#include "src/modules/passive/OsintModulesHeaders.h"


namespace Ui {
class MXTool;
}

class MXTool : public QWidget{
        Q_OBJECT

    public:
        explicit MXTool(QWidget *parent = nullptr);
        ~MXTool();

    public slots:
        void onResultsMX(MXModelStruct results);
        //...
        void onEnumerationComplete();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    signals:
        void stopScanThread();
        void pauseScanThread();

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();

    private:
        Ui::MXTool *ui;
        MXModel *m_model;
};

#endif // MXTOOL_H