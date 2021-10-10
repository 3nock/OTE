#ifndef NAMESERVERCHECK_H
#define NAMESERVERCHECK_H

#include <QObject>

class NameserverCheck : public QObject
{
    Q_OBJECT
public:
    explicit NameserverCheck(QObject *parent = nullptr);

signals:

public slots:
};

#endif // NAMESERVERCHECK_H
