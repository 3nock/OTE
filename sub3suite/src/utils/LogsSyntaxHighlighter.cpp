#include "LogsSyntaxHighlighter.h"


LogsSyntaxHighlighter::LogsSyntaxHighlighter(QTextDocument *document) : QSyntaxHighlighter(document)
{
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

void LogsSyntaxHighlighter::highlightBlock(const QString &text){
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
