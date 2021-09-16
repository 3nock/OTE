#include "SpecificAnalysis.h"
#include "ui_SpecificAnalysis.h"

 /*
  * use the api:
  *         https://jsonwhois.com/
  * and     https://whoisxmlapi.github.io/bulk-whois
  * and     https://www.domaintools.com/resources/api-documentation/authentication/
  * and     https://team-cymru.com/community-services/ip-asn-mapping/
  *
  * and     https://ipapi.com/documentation
  * and     https://ip-api.com/docs
  * and     https://toolbox.googleapps.com/apps/dig/
  */
SpecificAnalysis::SpecificAnalysis(QWidget *parent) :QWidget(parent),ui(new Ui::SpecificAnalysis)
{
    ui->setupUi(this);
}
SpecificAnalysis::~SpecificAnalysis(){
    delete ui;
}
