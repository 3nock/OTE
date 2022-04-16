/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef JSONSYNTAXHIGHLIGHTER_H
#define JSONSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>


class JsonSyntaxHighlighter: public QSyntaxHighlighter{
public:
    JsonSyntaxHighlighter(QTextDocument *document = nullptr):
        QSyntaxHighlighter(document) {}

    void forDarkTheme(){
        HighlightingRule rule;

        /* keys */
        rule.pattern = QRegExp ("(\"[^\"]*\")");
        rule.format.setForeground(Qt::cyan);
        rules.append(rule);

        /* numbers */
        rule.pattern = QRegExp("([-0-9.]+)(?!([^\"]*\"[\\s]*\\:))");
        rule.format.setForeground(Qt::green);
        rules.append(rule);

        /* strings */
        rule.pattern = QRegExp(":\\s*([\"](?:[^\"])*[\"])");
        rule.format.setForeground(Qt::white);
        rules.append(rule);

        /* special strings */
        rule.pattern = QRegExp("(true|false|null)(?!\"[^\"]*\")");
        rule.format.setForeground(Qt::magenta);
        rules.append(rule);
    }

    void forLightTheme(){
        HighlightingRule rule;

        /* keys */
        rule.pattern = QRegExp ("(\"[^\"]*\")");
        rule.format.setForeground(Qt::blue);
        rules.append(rule);

        /* numbers */
        rule.pattern = QRegExp("([-0-9.]+)(?!([^\"]*\"[\\s]*\\:))");
        rule.format.setForeground(Qt::darkRed);
        rules.append(rule);

        /* strings */
        rule.pattern = QRegExp(":\\s*([\"](?:[^\"])*[\"])");
        rule.format.setForeground(Qt::black);
        rules.append(rule);

        /* special strings */
        rule.pattern = QRegExp("(true|false|null)(?!\"[^\"]*\")");
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

#endif // JSONSYNTAXHIGHLIGHTER_H
