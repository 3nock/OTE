#ifndef CERTTOOL_H
#define CERTTOOL_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "src/models/CertModel.h"
#include "src/modules/osint/OsintModulesHeaders.h"


namespace Ui {
class CertTool;
}

class CertTool : public QDialog{
        Q_OBJECT

    public:
        explicit CertTool(QWidget *parent = nullptr);
        ~CertTool();
        //...
        void analyzeDomain(QString domain);
        void analyzeCert(QString cert);

    signals:
        void stopScanThread();
        void pauseScanThread();

    private slots:
        void on_buttonStart_clicked();
        void on_checkBoxExpand_clicked(bool checked);
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
        Ui::CertTool *ui;
        CertModel *m_model;
        QSortFilterProxyModel *m_proxyModel;

        /* for option combobox */
        QStringList m_protocals{"HTTPS", "SSH", "FTP"};
        QStringList m_osintModules{"Crtsh", "Certspotter"};
};

#endif // CERTTOOL_H
