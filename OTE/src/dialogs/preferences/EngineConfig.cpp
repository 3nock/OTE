#include "EngineConfig.h"
#include "ui_EngineConfig.h"

#include "src/core/Config.h"

EngineConfig::EngineConfig(QWidget *parent) :
    AbstractConfig(parent),
    ui(new Ui::EngineConfig)
{
    ui->setupUi(this);

    ui->lineEditTimeout->setPlaceholderText(tr("timeout in milliseconds e.g. 3000"));
}

EngineConfig::~EngineConfig()
{
    delete ui;
}

void EngineConfig::openConfig()
{
    ui->groupBoxTimeout->setChecked(OTE::gConfig.engine.timeout.use);
    ui->lineEditTimeout->setText(QString::number(OTE::gConfig.engine.timeout.msec));

    ui->groupBoxAccept->setChecked(OTE::gConfig.engine.accept.use);
    ui->radioButtonAcceptAll->setChecked(OTE::gConfig.engine.accept.all);
    ui->radioButtonAcceptJsonXml->setChecked(OTE::gConfig.engine.accept.jsonXml);

    ui->groupBoxUA->setChecked(OTE::gConfig.engine.user_agent.use);
    ui->radioButtonUA_OTE->setChecked(OTE::gConfig.engine.user_agent.oteUA);
    ui->radioButtonUA_Random->setChecked(OTE::gConfig.engine.user_agent.randomUA);
}

void EngineConfig::saveConfig()
{
    OTE::gConfig.engine.timeout.use = ui->groupBoxTimeout->isChecked();
    OTE::gConfig.engine.timeout.msec = ui->lineEditTimeout->text().toInt();

    OTE::gConfig.engine.accept.use = ui->groupBoxAccept->isChecked();
    OTE::gConfig.engine.accept.all = ui->radioButtonAcceptAll->isChecked();
    OTE::gConfig.engine.accept.jsonXml = ui->radioButtonAcceptJsonXml->isChecked();

    OTE::gConfig.engine.user_agent.use = ui->groupBoxUA->isChecked();
    OTE::gConfig.engine.user_agent.oteUA = ui->radioButtonUA_OTE->isChecked();
    OTE::gConfig.engine.user_agent.randomUA = ui->radioButtonUA_Random->isChecked();
}

void EngineConfig::resetConfig()
{

}
