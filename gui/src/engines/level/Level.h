#ifndef LEVEL_H
#define LEVEL_H

#include "src/utils.h"
#include "src/engines/Base.h"
#include "LevelScanner.h"

namespace Ui {
    class Level;
}

class Level : public BaseClass{
        Q_OBJECT

    public:
        Level(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Level();

    public slots:
        void scanThreadEnd();
        void scanResult(QString subdomain, QString ipAddress);
        //...
        void choosenWordlist(QString wordlistFilename);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonPause_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_buttonClear_clicked();
        void on_buttonAction_clicked();
        void on_buttonWordlist_clicked();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::Level *ui;
        //...
        void startScan();
        void nextLevel();
        level::ScanArguments *m_scanArguments;
        int lastScanResultsCount = 0;
};

#endif // LEVEL_H
