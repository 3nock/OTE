#ifndef PROJECT_H
#define PROJECT_H

#include "src/core.h"

namespace Ui {
    class Project;
}

class Project : public QWidget{
        Q_OBJECT

    private:
        Ui::Project *ui;

    public:
        ResultsModel *m_resultsModel;

    public:
        explicit Project(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Project();
};

#endif // PROJECT_H
