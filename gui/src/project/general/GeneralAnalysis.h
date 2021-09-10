#ifndef GENERALANALYSIS_H
#define GENERALANALYSIS_H

#include <QWidget>

namespace Ui {
    class GeneralAnalysis;
}

class GeneralAnalysis : public QWidget{
        Q_OBJECT

    public:
        explicit GeneralAnalysis(QWidget *parent = nullptr);
        ~GeneralAnalysis();

    private:
        Ui::GeneralAnalysis *ui;
};

#endif // GENERALANALYSIS_H
