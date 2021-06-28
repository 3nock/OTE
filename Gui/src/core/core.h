#ifndef DNS_MAIN_H
#define DNS_MAIN_H

/*************** HEADERS ******************/
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QMenu>
#include <QFont>
#include <QtGui>
//...
#include <QThread>
#include <QString>
#include <QtCore>
//...
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QTextStream>
//...
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
//...
#include <QHostAddress>
#include <QDnsLookup>
#include <QUrl>
//...
#include <QMainWindow>
#include <QObject>
#include <QWidget>
//...
#undef slots
#include "Python.h"
#define slots

/******************* MACROS *****************/
#define MAX_THREADS 100
#define SINGLE_ITEM 1
#define MULTI_ITEMS 2
#define ALL_ITEMS 3
//...
#define ENUMNAME_OSINT 1
#define ENUMNAME_SUBBRUTE 2
#define ENUMNAME_TLDBRUTE 3
#define ENUMNAME_ACTIVESUBDOMAINS 4
//...
#define NEWLINE "\n"
#define EMPTY ""
#define TITLE_ERROR "Error!"
//...
#define CURRENT_PATH "./"
#define INFO_LOADFILE "Load File..."
#define INFO_SAVETOFILE "Save To File..."

/***************** FUNCTIONS *****************/
QHostAddress RandomNameserver(bool useCustomNameservers);
QString TargetNameFilter(QString domainName, int enumName);
QString EnumName(int enumName);
QString wordlistFilter(QString word);
QString filter(QString word);
// propossed...
void loadFile(QListWidget& wordlist, int& count);

#endif // _DNS_MAIN_H
