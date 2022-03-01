#include "Project.h"
#include "ui_Project.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


Project::Project(QWidget *parent, ProjectModel *projectModel) :QWidget(parent),
    ui(new Ui::Project),
    model(projectModel),
    proxyModel(new QSortFilterProxyModel)
{
    this->initUI();

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
    ui->treeViewProjectExplorer->setModel(model->explorer);
    ui->treeViewProjectExplorer->expand(model->explorer->project->index());

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setRecursiveFilteringEnabled(true);
    proxyModel->setFilterKeyColumn(0);
    ui->treeViewSiteMap->setModel(proxyModel);

    this->init_notesMenuBar();
    this->init_sitemapMenuBar();
}
Project::~Project(){
    delete m_sitemapMenuBar;
    delete m_notesMenuBar;
    delete proxyModel;
    delete ui;
}


void Project::initUI(){
    /* setup ui */
    ui->setupUi(this);

    /* setting widgets special properties for diff stylesheets */
    ui->labelCount->setProperty("s3s_color", true);
    ui->treeViewProjectExplorer->setProperty("no_item_border", true);
    ui->tabWidget->setProperty("upside", true);

    /* hiding un-used widgets */
    ui->comboBoxFilter->hide();

    /* placeholder texts */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->plainTextEdit_item_comment->setPlaceholderText("comment on item...");

    /* resizing */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.22))
                           << static_cast<int>((this->width() * 0.78)));

    ui->splitter_2->setSizes(QList<int>() << static_cast<int>((this->width() * 0.80))
                             << static_cast<int>((this->width() * 0.20)));
}

void Project::initActions(){
    a_copy.setIcon(QIcon(":/img/res/icons/copy.png"));
    a_save.setIcon(QIcon(":/img/res/icons/save.png"));
    a_send.setIcon(QIcon(":/img/res/icons/send.png"));
    a_clear.setIcon(QIcon(":/img/res/icons/delete.png"));
    a_expand.setIcon(QIcon(":/img/res/icons/expand.png"));
    a_collapse.setIcon(QIcon(":/img/res/icons/collapse.png"));

    a_copy.setToolTip("Copy");
    a_save.setToolTip("Save");
    a_send.setToolTip("Send");
    a_clear.setToolTip("Clear");
    a_expand.setToolTip("Expand");
    a_collapse.setToolTip("Collapse");

    connect(&a_copy, &QAction::triggered, this, [=](){this->action_copy();});
    connect(&a_save, &QAction::triggered, this, [=](){this->action_save();});
    connect(&a_send, &QAction::triggered, this, [=](){this->action_send();});
    connect(&a_clear, &QAction::triggered, this, [=](){this->action_clear();});
    connect(&a_expand, &QAction::triggered, this, [=](){ui->treeViewSiteMap->expandAll();});
    connect(&a_collapse, &QAction::triggered, this, [=](){ui->treeViewSiteMap->collapseAll();});
}

void Project::init_sitemapMenuBar(){
    this->initActions();
    m_sitemapMenuBar = new QMenuBar(this);
    m_sitemapMenuBar->addAction(&a_save);
    m_sitemapMenuBar->addAction(&a_copy);
    m_sitemapMenuBar->addAction(&a_send);
    m_sitemapMenuBar->addAction(&a_clear);
    m_sitemapMenuBar->addAction(&a_expand);
    m_sitemapMenuBar->addAction(&a_collapse);
    m_sitemapMenuBar->setToolTipDuration(2);
    ui->horizontalLayoutMap->insertWidget(0, m_sitemapMenuBar);
}

void Project::init_notesMenuBar(){
    m_notesMenuBar = new QMenuBar(this);
    m_notesMenuBar->addAction("Clear", this, [=](){ui->plainTextEdit->clear();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    ui->horizontalLayoutNotes->insertWidget(0, m_notesMenuBar);
}
