//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "SideBarWidget.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>

#define action_height 70

SideBarWidget::SideBarWidget(QWidget *parent):
    QWidget(parent),
    mCheckedAction(nullptr),
    mOverAction(nullptr)
{
    setMouseTracking(true);
}

void SideBarWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    auto font = p.font();
    font.setBold(true);
    p.setFont(font);

    int action_y = 0;
    p.fillRect(rect(), QColor("#434343"));

    for (auto action : mActions)
    {
        QRect actionRect(0, action_y, event->rect().width(), action_height);

        if (action->isChecked())
            p.fillRect(actionRect, QColor(53,53,53));

        if (action == mOverAction)
            p.fillRect(actionRect, QColor("#505050"));

        p.setPen(QColor(225, 225, 225));
        QSize size = p.fontMetrics().size(Qt::TextSingleLine, action->text());
        QRect actionTextRect(QPoint(actionRect.width() / 2 - size.width() / 2,
                                    actionRect.bottom() - size.height() - 5), size);

        p.drawText(actionTextRect, Qt::AlignCenter, action->text());

        QRect actionIconRect(0, action_y + 10, actionRect.width(),
                             actionRect.height() - 2 * actionTextRect.height() - 10);

        QIcon actionIcon(action->icon());
        actionIcon.paint(&p, actionIconRect);

        action_y += actionRect.height();
    }
}

QSize SideBarWidget::minimumSizeHint() const
{
    return action_height * QSize(1, mActions.size());
}

void SideBarWidget::click(int pos)
{
    if(pos >= mActions.count())
        return;

    QAction *tempAction = mActions.at(pos);
    if (tempAction == nullptr || tempAction->isChecked())
        return;
    tempAction->trigger();
    if (mCheckedAction)
        mCheckedAction->setChecked(false);
    if (mOverAction == tempAction)
        mOverAction = nullptr;
    mCheckedAction = tempAction;
    tempAction->setChecked(true);
    update();
}

void SideBarWidget::addAction(QAction *action)
{
    mActions.push_back(action);
    action->setCheckable(true);
    update();
}

QAction *SideBarWidget::addAction(const QString &text, const QIcon &icon)
{
    QAction *action = new QAction(icon, text, this);
    action->setCheckable(true);
    mActions.push_back(action);
    update();
    return action;
}

void SideBarWidget::mousePressEvent(QMouseEvent *event)
{
    QAction *tempAction = actionAt(event->pos());
    if (tempAction == nullptr || tempAction->isChecked())
        return;
    tempAction->trigger();
    if (mCheckedAction)
        mCheckedAction->setChecked(false);
    if (mOverAction == tempAction)
        mOverAction = nullptr;
    mCheckedAction = tempAction;
    tempAction->setChecked(true);
    update();
    QWidget::mousePressEvent(event);
}

void SideBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    QAction *tempAction = actionAt(event->pos());
    if (tempAction == nullptr)
    {
        mOverAction = nullptr;
        update();
        return;
    }
    if (tempAction->isChecked() || mOverAction == tempAction)
        return;
    mOverAction = tempAction;
    update();
    QWidget::mouseMoveEvent(event);
}

void SideBarWidget::leaveEvent(QEvent *event)
{
    mOverAction = nullptr;
    update();
    QWidget::leaveEvent(event);
}

QAction *SideBarWidget::actionAt(const QPoint &at)
{
    int action_y = 0;
    for (auto action : mActions)
    {
        QRect actionRect(0, action_y, rect().width(), action_height);
        if (actionRect.contains(at))
          return action;

        action_y += actionRect.height();
    }
    return nullptr;
}

#undef action_height
