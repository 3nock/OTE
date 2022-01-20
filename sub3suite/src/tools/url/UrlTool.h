#ifndef URLTOOL_H
#define URLTOOL_H

#include <QWidget>

namespace Ui {
class UrlTool;
}

class UrlTool : public QWidget
{
    Q_OBJECT

public:
    explicit UrlTool(QWidget *parent = nullptr);
    ~UrlTool();

private:
    Ui::UrlTool *ui;
};

#endif // URLTOOL_H
