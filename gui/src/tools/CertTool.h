#ifndef CERTTOOL_H
#define CERTTOOL_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "src/models/certmodel.h"
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

    private slots:
        void on_buttonAnalyze_clicked();
        void on_checkBoxExpand_clicked(bool checked);
        void on_comboBoxTargetType_currentIndexChanged(int index);

    public slots:
        void onRawCert(QByteArray);
        void onEnumerationComplete();
        void onErrorLog(QString log);
        void onInfoLog(QString log);


    private:
        Ui::CertTool *ui;
        CertModel *m_certModel;
        QStandardItemModel *m_model;
        QSortFilterProxyModel *m_proxyModel;
        ScanArgs *m_scanArgs;

        /* for option combobox */
        QStringList m_protocals{"HTTPS", "SSH", "FTP"};
        QStringList m_osintModules{"Crtsh", "Certspotter"};
};

#endif // CERTTOOL_H
