#ifndef NOTESSYNTAXHIGHLIGHTER_H
#define NOTESSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class NotesSyntaxHighlighter: public QSyntaxHighlighter{
    public:
         NotesSyntaxHighlighter(QTextDocument *document = nullptr);

    private:
         struct HighlightingRule{
             QRegExp pattern;
             QTextCharFormat format;
         };

         QVector<HighlightingRule> rules;

    protected:
        void highlightBlock(const QString &text);
};

#endif // NOTESSYNTAXHIGHLIGHTER_H
