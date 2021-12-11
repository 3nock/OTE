#ifndef SPECIFICANALYSIS_H
#define SPECIFICANALYSIS_H

#include <QWidget>

namespace Ui {
    class SpecificAnalysis;
}

class SpecificAnalysis : public QWidget{
        Q_OBJECT

    public:
        explicit SpecificAnalysis(QWidget *parent = nullptr);
        ~SpecificAnalysis();

    private:
        Ui::SpecificAnalysis *ui;
};

#endif // SPECIFICANALYSIS_H
