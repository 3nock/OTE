#ifndef IPCHECKER_H
#define IPCHECKER_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>

class IpModel{
    public:
        IpModel();

    public:
        ///
        /// the needed Info...
        ///
        QStandardItemModel mainModel;
        //...
        QStandardItem *info;
        QStandardItem *asnInfo;
        QStandardItem *companyInfo;
        QStandardItem *neighbours;
};

namespace Ui {
    class IpChecker;
}

class IpChecker : public QDialog{
    Q_OBJECT

    public:
        explicit IpChecker(QWidget *parent = nullptr);
        ~IpChecker();

    private:
        Ui::IpChecker *ui;
};

#endif // IPCHECKER_H
