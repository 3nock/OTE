/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "NotesSyntaxHighlighter.h"


NotesSyntaxHighlighter::NotesSyntaxHighlighter(QTextDocument *document) : QSyntaxHighlighter(document)
{
   HighlightingRule rule;

   /* heading ends with : */
   rule.pattern = QRegExp("([^\"]*:)");
   rule.format.setForeground(Qt::cyan);
   rules.append(rule);

   /* numbers */
   rule.pattern = QRegExp("([-0-9.]+)(?!([^\"]*\"[\\s]*\\:))");
   rule.format.setForeground(Qt::green);
   rules.append(rule);

   /* special strings */
   rule.pattern = QRegExp("(true|false|null)(?!\"[^\"]*\")");
   rule.format.setForeground(Qt::magenta);
   rules.append(rule);

   /* color codes*/

   // red
   rule.pattern = QRegExp ("(r;[^\"]*)");
   rule.format.setForeground(Qt::red);
   rules.append(rule);

   // blue
   rule.pattern = QRegExp ("(b;[^\"]*)");
   rule.format.setForeground(Qt::blue);
   rules.append(rule);

   // green
   rule.pattern = QRegExp ("(g;[^\"]*)");
   rule.format.setForeground(Qt::green);
   rules.append(rule);

   // white
   rule.pattern = QRegExp ("(w;[^\"]*)");
   rule.format.setForeground(Qt::white);
   rules.append(rule);

   // yellow
   rule.pattern = QRegExp ("(y;[^\"]*)");
   rule.format.setForeground(Qt::yellow);
   rules.append(rule);

   /* comments starts with # */
   rule.pattern = QRegExp ("(#[^\"]*)");
   rule.format.setForeground(Qt::gray);
   rules.append(rule);
}

void NotesSyntaxHighlighter::highlightBlock(const QString &text){
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
