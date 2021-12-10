#ifndef MXTOOL_H
#define MXTOOL_H

#include <QDialog>
#include "src/models/MXModel.h"
#include "src/modules/osint/OsintModulesHeaders.h"


namespace Ui {
class MXTool;
}

class MXTool : public QDialog{
        Q_OBJECT

    public:
        explicit MXTool(QWidget *parent = nullptr);
        ~MXTool();

    public slots:
        void onResultsMX(MXModelStruct results);
        //...
        void onEnumerationComplete();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_checkBoxExpand_clicked(bool checked);
        void on_buttonStart_clicked();

    private:
        Ui::MXTool *ui;
        MXModel *m_model;
        ScanArgs *m_args = nullptr;
};

#endif // MXTOOL_H
