/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef SSLENUM_H
#define SSLENUM_H

#include <QSortFilterProxyModel>

#include "../AbstractEnum.h"
#include "src/models/SSLModel.h"
#include "src/modules/passive/OsintModulesHeaders.h"


namespace Ui {
class SSLEnum;
}

class SSLEnum : public AbstractEnum {
        Q_OBJECT

    public:
        SSLEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~SSLEnum();
        //...
        void analyzeDomain(QString domain);
        void analyzeCert(QString cert);

    private slots:
        void on_buttonStart_clicked();
        void on_comboBoxTargetType_currentIndexChanged(int index);
        void on_buttonConfig_clicked();

    public slots:
        void onResult(QByteArray results); // raw certificate in pem format...
        void onEnumerationComplete();
        //...
        void onInfoLogTxt(QString log);
        void onErrorLogTxt(QString log);
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private:
        Ui::SSLEnum *ui;
        CertModel *m_model;
        QSortFilterProxyModel *m_proxyModel;

        /* for option combobox */
        QStringList m_protocals{"HTTPS", "SSH", "FTP"};
        QStringList m_osintModules{"Crtsh", "Certspotter"};
};

#endif // SSLENUM_H
