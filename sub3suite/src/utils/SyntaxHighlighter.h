#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class SyntaxHighlighter: public QSyntaxHighlighter{
public:
    SyntaxHighlighter(QTextDocument *document = nullptr);
    ~SyntaxHighlighter();

    void json_light();
    void json_dark();

    void Logs_light();
    void logs_dark();

private:
     struct HighlightingRule{
         QRegExp pattern;
         QTextCharFormat format;
     };

     QVector<HighlightingRule> rules;

protected:
     void highlightBlock(const QString &text);
};

#endif // SYNTAXHIGHLIGHTER_H
