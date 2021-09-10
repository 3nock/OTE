#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QWidget>
#include <QSet>
#include <QStandardItemModel>

namespace Ui {
    class ResultsWidget;
}

class ResultsWidget : public QWidget{
        Q_OBJECT

    public:
        explicit ResultsWidget(QWidget *parent = nullptr);
        ~ResultsWidget();
        //...
        void init(const QString& name, QStandardItemModel* resultsModel);
        void addLog(const QString &log);

    signals:
        void clearProgressBar();

    private slots:
        void on_buttonClear_clicked();
        void on_buttonAction_clicked();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::ResultsWidget *ui;
        //...
        QStandardItemModel *m_resultsModel = nullptr;
        QSet<QString> m_subdomains;
};

#endif // RESULTSWIDGET_H
