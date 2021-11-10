#ifndef ASNTOOL_H
#define ASNTOOL_H

#include <QDialog>

namespace Ui {
class ASNTool;
}

class ASNTool : public QDialog{
        Q_OBJECT

    public:
        explicit ASNTool(QWidget *parent = nullptr);
        ~ASNTool();

private slots:
    void on_buttonAnalyze_clicked();

private:
        Ui::ASNTool *ui;
};

#endif // ASNTOOL_H
