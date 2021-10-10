#ifndef INTERNETCHECK_H
#define INTERNETCHECK_H

#include <QObject>

class InternetCheck : public QObject
{
    Q_OBJECT
public:
    explicit InternetCheck(QObject *parent = nullptr);

signals:

public slots:
};

#endif // INTERNETCHECK_H
