#ifndef BANNERGRABBER_H
#define BANNERGRABBER_H

#include <QDialog>

namespace Ui {
class BannerGrabber;
}

class BannerGrabber : public QDialog
{
    Q_OBJECT

public:
    explicit BannerGrabber(QWidget *parent = nullptr);
    ~BannerGrabber();

private:
    Ui::BannerGrabber *ui;
};

#endif // BANNERGRABBER_H
