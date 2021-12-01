#include "Osint.h"
#include "ui_Osint.h"

#include "src/modules/osint/OsintModulesHeaders.h"


void Osint::initModules(){
    int INPUT_TYPE = ui->comboBoxInput->currentIndex();
    int OUTPUT_TYPE = ui->comboBoxOutput->currentIndex();

    ModuleInfo::Anubis anubis;
    if(anubis.input_output.contains(INPUT_TYPE) && anubis.input_output[INPUT_TYPE].contains(OUTPUT_TYPE))
        ui->moduleAnubis->show();
    else
        ui->moduleAnubis->hide();
}
