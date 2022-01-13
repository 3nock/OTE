#ifndef TOOLSCONFIGDIALOG_H
#define TOOLSCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ToolsConfigDialog;
}

class ToolsConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ToolsConfigDialog(QWidget *parent = nullptr);
    ~ToolsConfigDialog();

private:
    Ui::ToolsConfigDialog *ui;
};

#endif // TOOLSCONFIGDIALOG_H
