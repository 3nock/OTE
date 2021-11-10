#ifndef CERTTOOL_H
#define CERTTOOL_H

#include <QDialog>

namespace Ui {
class CertTool;
}

class CertTool : public QDialog
{
    Q_OBJECT

public:
    explicit CertTool(QWidget *parent = nullptr);
    ~CertTool();

private:
    Ui::CertTool *ui;
};

#endif // CERTTOOL_H
