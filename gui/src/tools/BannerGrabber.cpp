#include "BannerGrabber.h"
#include "ui_BannerGrabber.h"

BannerGrabber::BannerGrabber(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BannerGrabber)
{
    ui->setupUi(this);
}

BannerGrabber::~BannerGrabber()
{
    delete ui;
}
