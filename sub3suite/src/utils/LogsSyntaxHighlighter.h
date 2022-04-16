/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef LOGSSYNTAXHIGHLIGHTER_H
#define LOGSSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>


class LogsSyntaxHighlighter: public QSyntaxHighlighter{
public:
     LogsSyntaxHighlighter(QTextDocument *document = nullptr):
         QSyntaxHighlighter(document) {}

     void forDarkTheme(){
         HighlightingRule rule;

         rule.pattern = QRegExp ("(INFO:[^\"]*)");
         rule.format.setForeground(Qt::blue);
         rules.append(rule);

         rule.pattern = QRegExp ("(DEBUG:[^\"]*)");
         rule.format.setForeground(Qt::green);
         rules.append(rule);

         rule.pattern = QRegExp ("(WARNING:[^\"]*)");
         rule.format.setForeground(Qt::yellow);
         rules.append(rule);

         rule.pattern = QRegExp ("(CRITICAL:[^\"]*)");
         rule.format.setForeground(Qt::red);
         rules.append(rule);

         rule.pattern = QRegExp ("(FATAL:[^\"]*)");
         rule.format.setForeground(Qt::red);
         rules.append(rule);
     }

     void forLightTheme(){
         HighlightingRule rule;

         rule.pattern = QRegExp ("(INFO:[^\"]*)");
         rule.format.setForeground(Qt::blue);
         rules.append(rule);

         rule.pattern = QRegExp ("(DEBUG:[^\"]*)");
         rule.format.setForeground(Qt::darkGreen);
         rules.append(rule);

         rule.pattern = QRegExp ("(WARNING:[^\"]*)");
         rule.format.setForeground(Qt::darkYellow);
         rules.append(rule);

         rule.pattern = QRegExp ("(CRITICAL:[^\"]*)");
         rule.format.setForeground(Qt::red);
         rules.append(rule);

         rule.pattern = QRegExp ("(FATAL:[^\"]*)");
         rule.format.setForeground(Qt::red);
         rules.append(rule);
     }

private:
     struct HighlightingRule{
         QRegExp pattern;
         QTextCharFormat format;
     };

     QVector<HighlightingRule> rules;

protected:
     void highlightBlock(const QString &text){
         foreach (const HighlightingRule &rule, rules) {
             QRegExp expression(rule.pattern);
             int index = expression.indexIn(text);

             while (index >= 0) {
                 index = expression.pos(1);
                 int length = expression.cap(1).length();
                 setFormat(index, length, rule.format);
                 index = expression.indexIn(text, index + length);
             }
         }
     }
};

#endif // LOGSSYNTAXHIGHLIGHTER_H
