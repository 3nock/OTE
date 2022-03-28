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
    ui->treeViewExplorer->setModel(model->explorer);
    ui->treeViewExplorer->expand(model->explorer->project->index());

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setRecursiveFilteringEnabled(true);
    proxyModel->setFilterKeyColumn(0);
    ui->treeViewTree->setModel(proxyModel);

    ui->treeViewTree->header()->setSectionResizeMode(QHeaderView::Interactive);

    this->init_menubar_tree();
    this->init_menubar_project();
}
Project::~Project(){
    delete m_jsonHighlighter;
    delete proxyModel;
    delete ui;
}

void Project::initProject(){
    ui->label_project_name->setText(model->info.name);
    ui->label_project_created->setText(model->info.date_created);
    ui->label_project_modified->setText(model->info.last_modified);
    ui->label_project_items->setNum(model->getItemsCount());
    ui->plainTextEditNotes->setDocument(&model->notes);
}

void Project::initUI(){
    /* setup ui */
    ui->setupUi(this);

    /* setting widgets special properties for diff stylesheets */
    ui->labelCount->setProperty("s3s_color", true);
    ui->treeViewExplorer->setProperty("no_item_border", true);
    ui->tabWidget->setProperty("upside", true);
    ui->label_project_name->setProperty("s3s_color", true);
    ui->label_project_created->setProperty("s3s_color", true);
    ui->label_project_modified->setProperty("s3s_color", true);
    ui->label_project_items->setProperty("s3s_color", true);
    ui->label_item_type->setProperty("s3s_color", true);
    ui->label_item_modified->setProperty("s3s_color", true);
    ui->plainTextEdit_item_comment->setProperty("text_edit", true);
    ui->lineEditFind->setPlaceholderText(tr("Find..."));

    /* hiding un-used widgets */
    ui->comboBoxFilter->hide();

    /* placeholder texts */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->plainTextEdit_item_comment->setPlaceholderText("comment...");
    ui->plainTextEditJson->setPlaceholderText("Double-click on an item (first column) on Tree-tab to view it in json...");

    /* resizing */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.22))
                           << static_cast<int>((this->width() * 0.78)));

    ui->splitter_2->setSizes(QList<int>() << static_cast<int>((this->width() * 0.80))
                             << static_cast<int>((this->width() * 0.20)));

    m_jsonHighlighter = new JsonSyntaxHighlighter(ui->plainTextEditJson->document());
}

void Project::init_menubar_tree(){
    menu_send = new QMenu(this);
    menu_copy = new QMenu(this);
    menu_save = new QMenu(this);
    menu_extract = new QMenu(this);

    a_copy.setIcon(QIcon(":/img/res/icons/copy.png"));
    a_save.setIcon(QIcon(":/img/res/icons/save.png"));
    a_send.setIcon(QIcon(":/img/res/icons/send.png"));
    a_clear.setIcon(QIcon(":/img/res/icons/delete.png"));
    a_expand.setIcon(QIcon(":/img/res/icons/expand.png"));
    a_collapse.setIcon(QIcon(":/img/res/icons/collapse.png"));
    a_cancel.setIcon(QIcon(":/img/res/icons/exit.png"));
    a_extract.setIcon(QIcon(":/img/res/icons/extract.png"));
    a_remove_duplicates.setIcon(QIcon(":/img/res/icons/duplicate.png"));

    connect(&a_clear, &QAction::triggered, this, [=](){this->action_clear();});
    connect(&a_expand, &QAction::triggered, this, [=](){ui->treeViewTree->expandAll();});
    connect(&a_collapse, &QAction::triggered, this, [=](){ui->treeViewTree->collapseAll();});
    connect(&a_remove_duplicates, &QAction::triggered, this, [=](){this->action_remove_duplicates();});
    connect(&a_cancel, &QAction::triggered, this, [=](){
        proxyModel->setSourceModel(nullptr);
        a_copy.setDisabled(true);
        a_save.setDisabled(true);
        a_clear.setDisabled(true);
        a_send.setDisabled(true);
        a_expand.setDisabled(true);
        a_collapse.setDisabled(true);
        a_extract.setDisabled(true);
        a_remove_duplicates.setDisabled(true);
        a_cancel.setDisabled(true);

        ui->treeViewExplorer->clearSelection();
        ui->labelCount->clear();
    });

    a_send.setMenu(menu_send);
    a_save.setMenu(menu_save);
    a_copy.setMenu(menu_copy);
    a_extract.setMenu(menu_extract);

    menubar_tree = new QMenuBar(this);
    menubar_tree->addAction(&a_save);
    menubar_tree->addAction(&a_copy);
    menubar_tree->addAction(&a_send);
    menubar_tree->addAction(&a_clear);
    menubar_tree->addAction(&a_expand);
    menubar_tree->addAction(&a_collapse);
    menubar_tree->addAction(&a_extract);
    menubar_tree->addAction(&a_remove_duplicates);
    menubar_tree->addAction(&a_cancel);
    menubar_tree->setToolTipDuration(2);
    ui->horizontalLayoutMap->insertWidget(0, menubar_tree);

    a_save.setDisabled(true);
    a_copy.setDisabled(true);
    a_send.setDisabled(true);
    a_clear.setDisabled(true);
    a_expand.setDisabled(true);
    a_collapse.setDisabled(true);
    a_extract.setDisabled(true);
    a_remove_duplicates.setDisabled(true);
    a_cancel.setDisabled(true);
}

void Project::init_menubar_project(){
    menubar_project = new QMenuBar(this);
    menubar_project->addAction("Save", this, [=](){model->saveProject();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menubar_project->addAction("Clear", this, [=](){model->clearModels();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    ui->horizontalLayout_summary_project->insertWidget(0, menubar_project);
}

///
/// Find...
///

void Project::find(const QString &searchTerm, QTextDocument::FindFlags flags){
    if(ui->plainTextEditJson->find(searchTerm, flags))
        ui->lineEditFind->setStyleSheet("color: white");
    else
        ui->lineEditFind->setStyleSheet("color: rgb(255, 86, 80);");
}

void Project::on_buttonNext_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find(ui->lineEditFind->text(), flags);
}

void Project::on_buttonPrev_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    flags |= QTextDocument::FindBackward;

    this->find(ui->lineEditFind->text(), flags);
}

void Project::on_lineEditFind_textChanged(const QString &searchTerm){
    /* set cursor to the begining of the document... */
    QTextCursor currentCursorPosition;
    currentCursorPosition.setPosition(QTextCursor::Start);
    ui->plainTextEditJson->setTextCursor(currentCursorPosition);

    /* get option flags... */
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find(searchTerm, flags);
}
