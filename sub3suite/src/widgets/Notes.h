#ifndef NOTES_H
#define NOTES_H

#include <QWidget>

namespace Ui {
class Notes;
}

class Notes : public QWidget
{
    Q_OBJECT

public:
    explicit Notes(QWidget *parent = nullptr);
    ~Notes();

private:
    Ui::Notes *ui;
};

#endif // NOTES_H
