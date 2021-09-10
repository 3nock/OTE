#include "ResultsWidget.h"
#include "ui_ResultsWidget.h"

ResultsWidget::ResultsWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ResultsWidget)
{
    ui->setupUi(this);
    ///
    /// initializations...
    ///
}
ResultsWidget::~ResultsWidget(){
    delete ui;
}

void ResultsWidget::init(const QString &name, QStandardItemModel *resultsModel){
    ///
    /// initializations...
    ///
    m_resultsModel = resultsModel;
    ui->labelName->setText(name+" Count: ");
    ui->tableViewResults->setModel(m_resultsModel);
}

void ResultsWidget::addLog(const QString &log){
    ///
    /// adding the log to local log widget and setting color depending on error...
    ///
    ui->listWidgetLogs->addItem(log);
    if(log.startsWith("[ERROR]") || log.startsWith("[Error]")){
        ui->listWidgetLogs->item(ui->listWidgetLogs->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]")){
        ui->listWidgetLogs->item(ui->listWidgetLogs->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}

void ResultsWidget::on_buttonClear_clicked(){
    ///
    /// if the current tab is subdomains clear subdomains...
    ///
    if(ui->tabWidget->currentIndex() == 0)
    {
        m_subdomains.clear();
        m_resultsModel->clear();
        ui->labelResultsCount->clear();
        //...
        m_resultsModel->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
        //...
        emit clearProgressBar();
    }
    ///
    /// if the current tab is logs clear logs...
    ///
    else
        ui->listWidgetLogs->clear();
}

void ResultsWidget::on_buttonAction_clicked(){

}

void ResultsWidget::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
}
