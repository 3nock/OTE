#ifndef WORDLIST_H
#define WORDLIST_H

#include <QWidget>
#include <QFile>
#include <QStringListModel>
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

    private:
        Ui::InputWidget *ui;
        //...
        QStringList m_list;
        QSet<const QString> m_wordlist;

    public:
        explicit InputWidget(QWidget *parent = nullptr);
        ~InputWidget();
        //...
        void init(const QString& name);
        void add(const QString&);
        void add(QFile&);
        //...
        QStringListModel *listModel;

    private slots:
        void on_buttonClear_clicked();
        void on_buttonRemove_clicked();
        void on_buttonLoad_clicked();
        void on_buttonAdd_clicked();
        void on_lineEdit_returnPressed();
};

#endif // WORDLIST_H
