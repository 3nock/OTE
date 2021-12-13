#ifndef ASNTOOL_H
#define ASNTOOL_H

#include <QDialog>
#include "src/models/AsnModel.h"
#include "src/modules/osint/OsintModulesHeaders.h"


namespace Ui {
class ASNTool;
}

class ASNTool : public QDialog{
        Q_OBJECT

    public:
        explicit ASNTool(QWidget *parent = nullptr);
        ~ASNTool();

    signals:
        void stopScanThread();
        void pauseScanThread();

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
        void on_checkBoxExpand_clicked(bool checked);
        void on_buttonStart_clicked();
        void on_buttonConfig_clicked();

    private:
        Ui::ASNTool *ui;
        AsModel *m_model;
};

#endif // ASNTOOL_H
