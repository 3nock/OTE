#ifndef LOGSSYNTAXHIGHLIGHTER_H
#define LOGSSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>


class LogsSyntaxHighlighter: public QSyntaxHighlighter{
    public:
         LogsSyntaxHighlighter(QTextDocument *document = nullptr);

    private:
         struct HighlightingRule{
             QRegExp pattern;
             QTextCharFormat format;
         };

         QVector<HighlightingRule> rules;

    protected:
        void highlightBlock(const QString &text);
};

#endif // LOGSSYNTAXHIGHLIGHTER_H
