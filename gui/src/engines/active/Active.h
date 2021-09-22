#ifndef ACTIVE_H
#define ACTIVE_H

#include "src/engines/Base.h"
#include "src/utils/utils.h"
#include "ActiveScanner.h"

namespace Ui {
    class Active;
}

class Active : public BaseClass{
        Q_OBJECT

    public:
        Active(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr, Status *status = nullptr);
        ~Active();

    public slots:
        void scanResult(QString subdomain, QString ipAddress);
        void scanThreadEnded();

    private slots:
        void on_buttonStart_clicked();
        void on_buttonPause_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_buttonConfig_clicked();
        void on_buttonClearResults_clicked();
        //...
        void on_comboBoxOption_currentIndexChanged(int index);
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);

        void on_checkBoxCustomActive_clicked(bool checked);

private:
        Ui::Active *ui;
        //...
        void startScan();
        active::ScanArguments *m_scanArguments;
};

#endif // ACTIVE_H
