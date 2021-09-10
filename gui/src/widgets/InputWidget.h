#ifndef WORDLIST_H
#define WORDLIST_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QListWidget>
#include <QSet>

namespace Ui {
    class InputWidget;
}

class InputWidget : public QWidget{
        Q_OBJECT

    public:
        explicit InputWidget(QWidget *parent = nullptr);
        ~InputWidget();
        //...
        void init(const QString& name);
        void add(const QString&);
        void add(const QStringList&);
        void add(QFile&);
        //...
        QListWidget *listWidget = nullptr;

    private slots:
        void on_buttonClear_clicked();
        void on_buttonRemove_clicked();
        void on_buttonLoad_clicked();
        void on_buttonAdd_clicked();
        void on_lineEdit_returnPressed();

    private:
        Ui::InputWidget *ui;
        //...
        QSet<const QString> m_wordlist;
};

#endif // WORDLIST_H
