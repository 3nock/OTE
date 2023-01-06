#ifndef PROJECT_H
#define PROJECT_H

#include <QWidget>
#include <QMenuBar>

#include "src/models/ProjectModel.h"
#include "src/utils/SyntaxHighlighter.h"

#define SITEMAP_TYPE "smt"


namespace Ui {
class Project;
}

class Project : public QWidget{
        Q_OBJECT

    public:
        Project(QWidget *parent = nullptr, ProjectModel *projectModel = nullptr);
        ~Project();

    signals:
        void changeTabToOsint();
        void changeTabToRaw();
        void changeTabToBrute();
        void changeTabToHost();
        void changeTabToDns();
        void changeTabToSSL();
        void changeTabToURL();
        void changeTabToIP();
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
        void sendToHost(QSet<QString>, RESULT_TYPE);
        void sendToDns(QSet<QString>, RESULT_TYPE);
        void sendToSsl(QSet<QString>, RESULT_TYPE);
        void sendToUrl(QSet<QString>, RESULT_TYPE);
        void sendToIP(QSet<QString>, RESULT_TYPE);
        /* sending to an Enumerator */
        void sendToIpEnum(QSet<QString>, RESULT_TYPE);
        void sendToAsnEnum(QSet<QString>, RESULT_TYPE);
        void sendToCidrEnum(QSet<QString>, RESULT_TYPE);
        void sendToNSEnum(QSet<QString>, RESULT_TYPE);
        void sendToMXEnum(QSet<QString>, RESULT_TYPE);
        void sendToSSLEnum(QSet<QString>, RESULT_TYPE);
        void sendToEmailEnum(QSet<QString>, RESULT_TYPE);

    public slots:
        void onProjectLoaded();

    private slots:
        void on_treeViewExplorer_clicked(const QModelIndex &index);
        void on_lineEditFilter_textChanged(const QString &arg1);
        void on_treeViewTree_clicked(const QModelIndex &index);
        void on_treeViewTree_doubleClicked(const QModelIndex &index);
        void on_treeViewTree_customContextMenuRequested(const QPoint &pos);

        /* find */
        void on_buttonNext_clicked();
        void on_buttonPrev_clicked();
        void on_lineEditFind_textChanged(const QString &arg1);

    private:
        Ui::Project *ui;

        ProjectModel *model;
        QSortFilterProxyModel *proxyModel;

        QItemSelectionModel *m_selectionModel = nullptr;
        SyntaxHighlighter *mJsonHighlighter = nullptr;

        QString m_projectFile;
        QString *item_comment = nullptr;

        QMenuBar *menubar_tree = nullptr;
        QMenuBar *menubar_project = nullptr;

        QMenu *menu_send = nullptr;
        QMenu *menu_copy = nullptr;
        QMenu *menu_save = nullptr;
        QMenu *menu_extract = nullptr;

        /* main actions */
        QAction a_copy;
        QAction a_save;
        QAction a_send;
        QAction a_clear;
        QAction a_expand;
        QAction a_collapse;
        QAction a_cancel;
        QAction a_extract;
        QAction a_remove_duplicates;

        void find(const QString &, QTextDocument::FindFlags);

        void initUI();
        void init_menubar_tree();
        void init_menubar_project();

        void init_action_save();
        void init_action_copy();
        void init_action_send();
        void init_action_extract();

        /* menu bar's actions */
        void action_clear();

        void action_copy(const RESULT_TYPE&);
        void action_save(const RESULT_TYPE&);

        void action_sendToEngine(const TOOL&, const RESULT_TYPE&);
        void action_sendToEnum(const ENUMERATOR&, const RESULT_TYPE&);

        void action_send_host(const TOOL&);
        void action_send_ip(const TOOL&);
        void action_send_url(const TOOL&);
        void action_send_email(const TOOL&);
        void action_send_asn(const TOOL&);
        void action_send_cidr(const TOOL&);
        void action_send_ssl(const TOOL&);
        void action_send_ns(const TOOL&);
        void action_send_mx(const TOOL&);

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
        void action_send_selected_toEnum(const ENUMERATOR&);
        void action_send_selected_toEngine(const TOOL&, const RESULT_TYPE&);

        void action_extract(bool subdomain, bool tld, bool hosts);
        void action_extract_selected(bool subdomain, bool tld, bool host);

        void action_remove_duplicates();
};

#endif // S3SPROJECT_H
