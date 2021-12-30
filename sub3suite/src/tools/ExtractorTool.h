#ifndef EXTRACTORTOOL_H
#define EXTRACTORTOOL_H

#include <QDialog>

namespace Ui {
class ExtractorTool;
}

class ExtractorTool : public QDialog
{
    Q_OBJECT

public:
    explicit ExtractorTool(QWidget *parent = nullptr);
    ~ExtractorTool();

private:
    Ui::ExtractorTool *ui;
};

#endif // EXTRACTORTOOL_H
