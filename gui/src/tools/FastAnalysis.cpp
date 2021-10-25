#include "FastAnalysis.h"
#include "ui_FastAnalysis.h"

/*
 * a minor analysis tool
 * analyze hostnames/ip/email/url/asn/sha1 SSL Cert
 * accessed via context menu of results models only
 * for a fast non-project related analysis
 * one data at a time
 */
FastAnalysis::FastAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FastAnalysis)
{
    ui->setupUi(this);
}

FastAnalysis::~FastAnalysis()
{
    delete ui;
}
