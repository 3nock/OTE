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
