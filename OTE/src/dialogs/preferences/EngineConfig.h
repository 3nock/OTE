#ifndef DIALOGS_PREFERENCES_ENGINECONFIG_H
#define DIALOGS_PREFERENCES_ENGINECONFIG_H

#include "AbstractConfig.h"

namespace Ui {
class EngineConfig;
}

class EngineConfig : public AbstractConfig
{
    Q_OBJECT

public:
    explicit EngineConfig(QWidget *parent = nullptr);
    ~EngineConfig() override;

    void openConfig() override;
    void saveConfig() override;

private:
    Ui::EngineConfig *ui;
};

#endif // DIALOGS_PREFERENCES_ENGINECONFIG_H
