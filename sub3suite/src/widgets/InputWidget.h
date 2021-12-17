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
        /* ... */
        QStringListModel *m_listModel;

    public:
        explicit InputWidget(QWidget *parent = nullptr);
        ~InputWidget();
        /* ... */
        void setListModel(QStringListModel *listModel);
        void setListName(const QString& listName);
        void add(const QString& item);
        void add(QFile& file);

    private slots:
        void on_buttonClear_clicked();
        void on_buttonRemove_clicked();
        void on_buttonLoad_clicked();
        void on_buttonAdd_clicked();
        void on_lineEdit_returnPressed();
};

#endif // WORDLIST_H
