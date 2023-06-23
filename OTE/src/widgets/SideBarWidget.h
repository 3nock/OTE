//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef WIDGETS_SIDEBARWIDGET_H
#define WIDGETS_SIDEBARWIDGET_H

#include <QAction>
#include <QWidget>

class SideBarWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SideBarWidget(QWidget *parent = nullptr);

  void addAction(QAction *action);
  QAction *addAction(const QString &text, const QIcon &icon = QIcon());
  QSize minimumSizeHint() const;
  void click(int pos);

protected:
  void paintEvent(QPaintEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void leaveEvent(QEvent *event);

  QAction *actionAt(const QPoint &at);

private:
  QList<QAction *> mActions;

  QAction *mCheckedAction;
  QAction *mOverAction;
};

#endif // WIDGETS_SIDEBARWIDGET_H
