//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef CORE_THEMES_H
#define CORE_THEMES_H

#include <QApplication>
#include <QFile>
#include <QProxyStyle>
#include <QStyleFactory>

namespace OTE {

class DarkStyle : public QProxyStyle {
    Q_OBJECT

public:
    DarkStyle();
    explicit DarkStyle(QStyle *style);

    QStyle *baseStyle() const;

    void polish(QPalette &palette) override;
    void polish(QApplication *app) override;

private:
    QStyle *styleBase(QStyle *style = Q_NULLPTR) const;
};

class LightStyle : public QProxyStyle {
    Q_OBJECT

public:
    LightStyle();
    explicit LightStyle(QStyle *style);

    QStyle *baseStyle() const;

    void polish(QPalette &palette) override;
    void polish(QApplication *app) override;

private:
    QStyle *styleBase(QStyle *style = Q_NULLPTR) const;
};

} // namespace OTE

#endif // CORE_THEMES_H
