#ifndef MXTOOL_H
#define MXTOOL_H

#include <QDialog>

namespace Ui {
class MXTool;
}

class MXTool : public QDialog
{
    Q_OBJECT

public:
    explicit MXTool(QWidget *parent = nullptr);
    ~MXTool();

private:
    Ui::MXTool *ui;
};

#endif // MXTOOL_H
