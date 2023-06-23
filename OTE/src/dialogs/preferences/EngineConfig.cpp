#include "EngineConfig.h"
#include "ui_EngineConfig.h"

#include "src/core/Config.h"

EngineConfig::EngineConfig(QWidget *parent) :
    AbstractConfig(parent),
    ui(new Ui::EngineConfig)
{
    ui->setupUi(this);
}

EngineConfig::~EngineConfig()
{
    delete ui;
}

void EngineConfig::openConfig()
{
}

void EngineConfig::saveConfig()
{
}

void EngineConfig::resetConfig()
{

}
