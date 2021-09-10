#ifndef IP_H
#define IP_H

#include "src/utils.h"
#include "src/engines/Base.h"
#include "IpScanner.h"
#include "src/dialogs/ConfigDialog.h"

namespace Ui {
    class Ip;
}

class Ip : public BaseClass{
        Q_OBJECT

    public:
        Ip(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Ip();

    public slots:
        void scanResult(QString subdomain, QString ipAddress);
        void scanThreadEnded();

    private slots:
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonPause_clicked();
        void on_buttonAction_clicked();
        void on_buttonConfig_clicked();
        void on_buttonClear_clicked();
        //...
        void on_comboBoxOption_currentIndexChanged(int index);
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::Ip *ui;
        //...
        void startScan();
        ip::ScanArguments *m_scanArguments;
};

#endif // IP_H
