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
         JsonSyntaxHighlighter(QTextDocument *document = nullptr);

    private:
         struct HighlightingRule{
             QRegExp pattern;
             QTextCharFormat format;
         };

         QVector<HighlightingRule> rules;

    protected:
        void highlightBlock(const QString &text);
};

#endif // JSONSYNTAXHIGHLIGHTER_H
