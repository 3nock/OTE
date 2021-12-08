#ifndef ASNTOOL_H
#define ASNTOOL_H

#include <QDialog>
#include "src/models/AsnModel.h"
//...
#include "src/modules/osint/OsintModulesHeaders.h"

namespace Ui {
class ASNTool;
}

class ASNTool : public QDialog{
        Q_OBJECT

    public:
        explicit ASNTool(QWidget *parent = nullptr);
        ~ASNTool();

    public slots:
        void onResultsAsn(AsModelStruct);
        void onResultsAsnPeers(AsModelStruct);
        void onResultsAsnPrefixes(AsModelStruct);
        /* ... */
        void onEnumerationComplete();
        void onErrorLog(ScanLog log);
        void onInfoLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_checkBoxExpand_clicked(bool checked);
        void on_buttonAnalyze_clicked();

    private:
        Ui::ASNTool *ui;
        AsModel *asModel;
        ScanArgs *m_scanArgs = nullptr;
};

#endif // ASNTOOL_H
