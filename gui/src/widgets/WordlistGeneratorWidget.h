#ifndef WORDLISTGENERATORWIDGET_H
#define WORDLISTGENERATORWIDGET_H

#include <QWidget>

namespace Ui {
    class WordlistGeneratorWidget;
}

class WordlistGeneratorWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit WordlistGeneratorWidget(QWidget *parent = nullptr);
        ~WordlistGeneratorWidget();

    private:
        Ui::WordlistGeneratorWidget *ui;
};

#endif // WORDLISTGENERATORWIDGET_H
