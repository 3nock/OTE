#include "Project.h"
#include "ui_Project.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


Project::Project(QWidget *parent, ProjectModel *projectModel) :QWidget(parent),
    ui(new Ui::Project),
    m_projectModel(projectModel),
    m_proxyModel(new QSortFilterProxyModel)
{
    this->m_initUI();

    /* underdevelopment txt */
    QFile file(":/files/res/files/under_development.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QByteArray data = file.readAll();
        ui->textBrowserGraph->append(data);
        ui->textBrowserAnalysis->append(data);
        file.close();
    }

    /* set cursor at begining of document */
    QTextCursor textCursor_graph = ui->textBrowserGraph->textCursor();
    QTextCursor textCursor_analysis = ui->textBrowserAnalysis->textCursor();
    textCursor_graph.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    textCursor_analysis.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    ui->textBrowserGraph->setTextCursor(textCursor_graph);
    ui->textBrowserAnalysis->setTextCursor(textCursor_analysis);

    /* resolve external links in default browser */
    ui->textBrowserGraph->setOpenExternalLinks(true);
    ui->textBrowserAnalysis->setOpenExternalLinks(true);

    /* data models */
    ui->treeViewProjectExplorer->setModel(m_projectModel->model_explorer);
    ui->treeViewProjectExplorer->expand(m_projectModel->project_explorer->index());

    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setRecursiveFilteringEnabled(true);
    m_proxyModel->setFilterKeyColumn(0);
    ui->treeViewSiteMap->setModel(m_proxyModel);

    this->init_notesMenuBar();
    this->init_sitemapMenuBar();
}
Project::~Project(){
    delete m_sitemapMenuBar;
    delete m_notesMenuBar;
    delete m_proxyModel;
    delete ui;
}


void Project::m_initUI(){
    /* setup ui */
    ui->setupUi(this);

    /* setting widgets special properties for diff stylesheets */
    ui->labelCount->setProperty("s3s_color", true);
    ui->treeViewProjectExplorer->setProperty("no_item_border", true);
    ui->treeViewSummary->setProperty("no_item_border", true);
    ui->tabWidget->setProperty("upside", true);

    /* hiding un-used widgets */
    ui->comboBoxFilter->hide();

    /* placeholder texts */
    ui->lineEditFilter->setPlaceholderText("Filter...");

    /* resizing */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.22))
                           << static_cast<int>((this->width() * 0.78)));

    ui->splitter_2->setSizes(QList<int>() << static_cast<int>((this->width() * 0.80))
                             << static_cast<int>((this->width() * 0.20)));
}

void Project::init_sitemapMenuBar(){
    m_sitemapMenuBar = new QMenuBar(this);
    m_sitemapMenuBar->addAction("Save", this, [=](){this->action_save();})->setIcon(QIcon(":/img/res/icons/save.png"));
    m_sitemapMenuBar->addAction("Copy", this, [=](){this->action_copy();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    m_sitemapMenuBar->addAction("Send", this, [=](){this->action_send();})->setIcon(QIcon(":/img/res/icons/send.png"));
    m_sitemapMenuBar->addAction("Clear", this, [=](){this->action_clear();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    m_sitemapMenuBar->addAction("Expand", this, [=](){ui->treeViewSiteMap->expandAll();})->setIcon(QIcon(":/img/res/icons/expand.png"));
    m_sitemapMenuBar->addAction("Collapse", this, [=](){ui->treeViewSiteMap->collapseAll();})->setIcon(QIcon(":/img/res/icons/collapse.png"));
    ui->horizontalLayoutMap->insertWidget(0, m_sitemapMenuBar);
}

void Project::init_notesMenuBar(){
    m_notesMenuBar = new QMenuBar(this);
    m_notesMenuBar->addAction("Clear", this, [=](){ui->plainTextEdit->clear();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    ui->horizontalLayoutNotes->insertWidget(0, m_notesMenuBar);
}
