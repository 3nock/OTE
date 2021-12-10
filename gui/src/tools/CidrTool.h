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

    private:
        Ui::CidrTool *ui;
        CidrModel *m_model;
        ScanArgs *m_args = nullptr;
};

#endif // CIDRTOOL_H
