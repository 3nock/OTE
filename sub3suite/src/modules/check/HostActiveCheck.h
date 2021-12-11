#ifndef HOSTACTIVECHECK_H
#define HOSTACTIVECHECK_H

#include <QObject>
#include <QDnsLookup>


class HostActiveCheck: public QObject {
    Q_OBJECT

    public:
        HostActiveCheck(QObject *parent = nullptr);

    signals:
        void anotherLookup();
        //...
        void isActive();
        void notActive();

    private slots:
        void lookupFinished();
        void lookup();
        void isTargetActive(QString target);

    private:
        int m_check = 0;
        QDnsLookup *m_dns;
};

#endif // HOSTACTIVECHECK_H
