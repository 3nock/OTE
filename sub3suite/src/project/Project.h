#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QMenuBar>

#include "model/ProjectModel.h"
#include "src/utils/JsonSyntaxHighlighter.h"

#define SITEMAP_TYPE "smt"


namespace Ui {
class Project;
}

class Project : public QWidget{
        Q_OBJECT

    public:
        Project(QWidget *parent = nullptr, ProjectModel *projectModel = nullptr);
        ~Project();

        void initProject();

    signals:
        void changeTabToOsint();
        void changeTabToRaw();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToDns();
        void changeTabToSSL();
        void changeTabToURL();
        void changeTabToIpEnum();
        void changeTabToAsnEnum();
        void changeTabToCidrEnum();
        void changeTabToNSEnum();
        void changeTabToMXEnum();
        void changeTabToSSLEnum();
        void changeTabToEmailEnum();

        /* sending to engine */
        void sendToOsint(QSet<QString>, RESULT_TYPE);
        void sendToRaw(QSet<QString>, RESULT_TYPE);
        void sendToBrute(QSet<QString>, RESULT_TYPE);
        void sendToActive(QSet<QString>, RESULT_TYPE);
        void sendToDns(QSet<QString>, RESULT_TYPE);
        void sendToSsl(QSet<QString>, RESULT_TYPE);
        void sendToUrl(QSet<QString>, RESULT_TYPE);
        /* sending to an Enumerator */
        void sendToIpEnum(QSet<QString>, RESULT_TYPE);
        void sendToAsnEnum(QSet<QString>, RESULT_TYPE);
        void sendToCidrEnum(QSet<QString>, RESULT_TYPE);
        void sendToNSEnum(QSet<QString>, RESULT_TYPE);
        void sendToMXEnum(QSet<QString>, RESULT_TYPE);
        void sendToSSLEnum(QSet<QString>, RESULT_TYPE);
        void sendToEmailEnum(QSet<QString>, RESULT_TYPE);

    private slots:
        void on_treeViewExplorer_clicked(const QModelIndex &index);
        void on_lineEditFilter_textChanged(const QString &arg1);
        void on_treeViewTree_clicked(const QModelIndex &index);
        void on_treeViewTree_doubleClicked(const QModelIndex &index);
        void on_treeViewTree_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::Project *ui;

        QItemSelectionModel *m_selectionModel;
        JsonSyntaxHighlighter *m_jsonHighlighter;

        ProjectModel *model;
        QSortFilterProxyModel *proxyModel;

        QString m_projectFile;
        QString *item_comment = nullptr;

        QMenuBar *menubar_tree;
        QMenuBar *menubar_project;

        QMenu *menu_send;
        QMenu *menu_copy;
        QMenu *menu_save;

        /* main actions */
        QAction a_copy;
        QAction a_save;
        QAction a_send;
        QAction a_clear;
        QAction a_expand;
        QAction a_collapse;

        void initUI();
        void init_menubar_tree();
        void init_menubar_project();

        void init_action_save();
        void init_action_copy();
        void init_action_send();

        /* menu bar's actions */
        void action_clear();

        void action_copy(const RESULT_TYPE&);
        void action_save(const RESULT_TYPE&);

        void action_send_host(const ENGINE&);
        void action_send_ip(const ENGINE&);
        void action_send_url(const ENGINE&);
        void action_send_email(const ENGINE&);
        void action_send_asn(const ENGINE&);
        void action_send_cidr(const ENGINE&);
        void action_send_ssl(const ENGINE&);
        void action_send_ns(const ENGINE&);
        void action_send_mx(const ENGINE&);

        void action_send_ip();
        void action_send_email();
        void action_send_asn();
        void action_send_cidr();
        void action_send_ssl();
        void action_send_ns();
        void action_send_mx();

        void action_openInBrowser();
        void action_save_selected();
        void action_copy_selected();
        void action_remove_selected();
        void action_send_selected_toEnum(const TOOL&);
        void action_send_selected_toEngine(const ENGINE&, const RESULT_TYPE&);

        void action_extract(bool subdomain, bool tld);
        void action_extract_selected(bool subdomain, bool tld);
};

#endif // S3SPROJECT_H
