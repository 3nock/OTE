#ifndef RAWOSINT_H
#define RAWOSINT_H

#include <QDialog>
#include <QTextDocument>
#include <QStandardItemModel>
//...
#include "src/modules/osint/api/ViewDns.h"
#include "src/modules/osint/api/IpApi.h"
#include "src/modules/osint/api/ZoomEye.h"
#include "src/modules/osint/api/ZETAlytics.h"
#include "src/modules/osint/api/WhoisXmlApi.h"
#include "src/modules/osint/api/ThreatBook.h"
#include "src/modules/osint/api/Shodan.h"
#include "src/modules/osint/api/SecurityTrails.h"
#include "src/modules/osint/api/Robtex.h"
#include "src/modules/osint/api/RiskIq.h"
#include "src/modules/osint/api/MnemonicFree.h"
#include "src/modules/osint/api/MnemonicPaid.h"
#include "src/modules/osint/api/IpInfo.h"
#include "src/modules/osint/api/HunterSearch.h"
#include "src/modules/osint/api/Github.h"
#include "src/modules/osint/api/C99.h"
#include "src/modules/osint/api/BinaryEdge.h"
#include "src/modules/osint/api/Bgpview.h"
#include "src/modules/osint/api/Anubis.h"
#include "src/modules/osint/api/Otx.h"
#include "src/modules/osint/api/Sublist3r.h"
#include "src/modules/osint/api/Threatminer.h"
#include "src/modules/osint/api/Threatcrowd.h"
#include "src/modules/osint/api/HackerTargetFree.h"
#include "src/modules/osint/api/HackerTargetPaid.h"
#include "src/modules/osint/api/Dnsbufferoverun.h"
#include "src/modules/osint/api/Projectdiscovery.h"
#include "src/modules/osint/api/Spyse.h"
#include "src/modules/osint/api/Omnisint.h"
#include "src/modules/osint/api/Qwant.h"
#include "src/modules/osint/api/VirusTotal.h"
#include "src/modules/osint/api/Urlscan.h"
#include "src/modules/osint/archive/Waybackmachine.h"
#include "src/modules/osint/archive/ArchiveToday.h"
#include "src/modules/osint/archive/CommonCrawl.h"
#include "src/modules/osint/archive/ArchiveIt.h"
#include "src/modules/osint/cert/Crtsh.h"
#include "src/modules/osint/cert/GoogleCert.h"
#include "src/modules/osint/cert/Certspotter.h"
#include "src/modules/osint/api/WebResolver.h"
#include "src/modules/osint/api/Circl.h"

namespace Ui {
    class RawOsint;
}

class RawOsint : public QDialog{
        Q_OBJECT

    public:
        explicit RawOsint(QWidget *parent = nullptr);
        ~RawOsint();

    public slots:
        void onEnumerationComplete();
        void onErrorLog(QString log);
        void onInfoLog(QString log);
        void onResults(QByteArray); // for Json results
        void onResultsTxt(QByteArray); // for normal txt results

    signals:
        void stopScanThread();

    private slots:
        void on_buttonStart_clicked();
        void on_buttonClear_clicked();
        void on_buttonCopy_clicked();
        void on_buttonSave_clicked();
        void on_comboBoxModule_currentIndexChanged(int index);
        void on_comboBoxOptions_currentIndexChanged(const QString &arg1);
        ///
        /// for Find...
        ///
        void on_buttonNext_clicked();
        void on_buttonPrev_clicked();
        void on_lineEditFind_textEdited(const QString &arg1);

    private:
        Ui::RawOsint *ui;
        QStandardItemModel *m_model;
        ScanArgs *m_scanArgs;
        //...
        QMap<QString, QString> m_optionSet;
        //...
        void loadEngines();
        void startScan();
        //...
        void setJsonText(QJsonDocument &document);
        void setJsonTree(QJsonDocument &document);
        void treeObject(QJsonObject object, QStandardItem *item);
        void treeArray(QJsonArray array, QStandardItem *item);
        //...
        void find(QString, QTextDocument::FindFlags);
};

#endif // RAWOSINT_H
