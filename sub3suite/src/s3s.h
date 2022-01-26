#ifndef S3S_H
#define S3S_H

#include <QLabel>

class s3s_ClickableLabel : public QLabel{
       Q_OBJECT
    public:
       s3s_ClickableLabel(QString text, QWidget *parent = nullptr) : QLabel(text, parent) {}
       ~s3s_ClickableLabel(){}

    signals:
       void clicked();

    protected:
       void mousePressEvent(QMouseEvent *event) { Q_UNUSED(event); emit clicked(); }
};

#endif // S3S_H
