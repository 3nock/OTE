/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef CIDRTOOL_H
#define CIDRTOOL_H

#include <QDialog>
#include "src/models/CidrModel.h"
#include "src/modules/osint/OsintModulesHeaders.h"


namespace Ui {
class CidrTool;
}

class CidrTool : public QDialog{
        Q_OBJECT

    public:
        explicit CidrTool(QWidget *parent = nullptr);
        ~CidrTool();

    signals:
        void stopScanThread();
        void pauseScanThread();

    public slots:
        void onResult(CidrModelStruct results);
        /* ... */
        void onEnumerationComplete();
        void onErrorLog(ScanLog log);
        void onInfoLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_checkBoxExpand_clicked(bool checked);
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();

    private:
        Ui::CidrTool *ui;
        CidrModel *m_model;
};

#endif // CIDRTOOL_H
