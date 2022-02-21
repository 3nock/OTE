#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QNetworkAccessManager>


class UpdateChecker : public QNetworkAccessManager {
        Q_OBJECT
    public:
        UpdateChecker(QWidget* parent = nullptr);
        void checkForUpdates();
        void checkForUpdates_onStart();

    private slots:
        void onFinished(QNetworkReply* reply);

    private:
        QWidget* m_parent;
        bool onStart = false;

};

#endif // UPDATECHECKER_H
