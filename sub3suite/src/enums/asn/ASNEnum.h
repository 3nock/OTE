/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef ASNENUM_H
#define ASNENUM_H

#include "../AbstractEnum.h"
#include "src/models/ASNModel.h"
#include "src/modules/passive/OsintModulesHeaders.h"


namespace Ui {
class ASNEnum;
}

class ASNEnum : public AbstractEnum{
        Q_OBJECT

    public:
        ASNEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~ASNEnum();

    public slots:
        void onResultsAsn(AsModelStruct results);
        void onResultsAsnPeers(AsModelStruct results);
        void onResultsAsnPrefixes(AsModelStruct results);
        //...
        void onEnumerationComplete();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();

    private:
        Ui::ASNEnum *ui;
        AsModel *m_model;
};

#endif // ASNENUM_H
