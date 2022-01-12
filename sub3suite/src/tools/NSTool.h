/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef NSTOOL_H
#define NSTOOL_H

#include <QDialog>
#include "src/models/NSModel.h"
#include "src/modules/osint/OsintModulesHeaders.h"


namespace Ui {
class NSTool;
}

class NSTool : public QDialog{
        Q_OBJECT

    public:
        explicit NSTool(QWidget *parent = nullptr);
        ~NSTool();

    public slots:
        void onResultsNS(NSModelStruct results);
        //...
        void onEnumerationComplete();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    signals:
        void stopScanThread();
        void pauseScanThread();

    private slots:
        void on_checkBoxExpand_clicked(bool checked);
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();

    private:
        Ui::NSTool *ui;
        NSModel *m_model;
};

#endif // NSTOOL_H
