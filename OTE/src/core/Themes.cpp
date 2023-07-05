//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "Themes.h"

namespace OTE {

DarkStyle::DarkStyle() :
    DarkStyle(styleBase())
{}

DarkStyle::DarkStyle(QStyle *style) :
    QProxyStyle(style)
{}

QStyle *DarkStyle::styleBase(QStyle *style) const
{
    static QStyle *base = !style ? QStyleFactory::create(QStringLiteral("Fusion")) : style;
    return base;
}

QStyle *DarkStyle::baseStyle() const
{
    return styleBase();
}

void DarkStyle::polish(QPalette &palette)
{
    // modify palette to dark
    palette.setColor(QPalette::Link, QColor(0,130,140,255));
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::Disabled, QPalette::Window, QColor(80, 80, 80));
    palette.setColor(QPalette::WindowText, QColor(238, 238, 238));
    palette.setColor(QPalette::Disabled, QPalette::WindowText,
                        QColor(127, 127, 127));
    palette.setColor(QPalette::Base, QColor(40, 40, 40));
    palette.setColor(QPalette::Disabled, QPalette::Base, QColor(80, 80, 80));
    palette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    palette.setColor(QPalette::ToolTipBase, QColor(0, 0, 0));
    palette.setColor(QPalette::ToolTipText, QColor(250, 250, 250));
    palette.setColor(QPalette::Text, QColor(238, 238, 238));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    palette.setColor(QPalette::Dark, QColor(35, 35, 35));
    palette.setColor(QPalette::Shadow, QColor(20, 20, 20));
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, QColor(238, 238, 238));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,
                        QColor(127, 127, 127));
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor(0,130,140,30));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
    palette.setColor(QPalette::HighlightedText, QColor(238, 238, 238));
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText,
                        QColor(127, 127, 127));
}

void DarkStyle::polish(QApplication *app)
{
    if (!app)
        return;

    // set stylesheet
    QFile darkTheme(QStringLiteral(":/style/res/style/dark.qss"));
    if (darkTheme.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString qsStylesheet = QString::fromLatin1(darkTheme.readAll());
        app->setStyleSheet(qsStylesheet);
        darkTheme.close();
    }
}

/*************************************************************************************************
                                        Light Theme
**************************************************************************************************/

LightStyle::LightStyle():
    LightStyle(styleBase())
{}

LightStyle::LightStyle(QStyle *style):
    QProxyStyle(style)
{}

QStyle *LightStyle::baseStyle() const
{
    return styleBase();
}

void LightStyle::polish(QPalette &palette)
{
    palette.setColor(QPalette::Link, QColor(0,130,140,255));
    palette.setColor(QPalette::Highlight, QColor(0,130,140,100));
}

void LightStyle::polish(QApplication *app)
{
    if (!app)
        return;

    // set stylesheet
    QFile darkTheme(QStringLiteral(":/style/res/style/light.qss"));
    if (darkTheme.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString qsStylesheet = QString::fromLatin1(darkTheme.readAll());
        app->setStyleSheet(qsStylesheet);
        darkTheme.close();
    }
}

QStyle *LightStyle::styleBase(QStyle *style) const
{
    static QStyle *base = !style ? QStyleFactory::create(QStringLiteral("Fusion")) : style;
    return base;
}

} // namespace OTE
