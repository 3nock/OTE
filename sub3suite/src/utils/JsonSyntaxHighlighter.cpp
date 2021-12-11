#include "JsonSyntaxHighlighter.h"


JsonSyntaxHighlighter::JsonSyntaxHighlighter(QTextDocument *document) : QSyntaxHighlighter(document)
{
   HighlightingRule rule;

   /* keys */
   rule.pattern = QRegExp ("(\"[^\"]*\")");
   rule.format.setForeground(Qt::cyan);
   rules.append(rule);

   /* strings */
   rule.pattern = QRegExp(":\\s*([\"](?:[^\"])*[\"])");
   rule.format.setForeground(QColor(220,220,220));
   rules.append(rule);

   /* numbers */
   rule.pattern = QRegExp("([-0-9.]+)(?!([^\"]*\"[\\s]*\\:))");
   rule.format.setForeground(QColor(220,220,220));
   rules.append(rule);

   /* special strings */
   rule.pattern = QRegExp("(true|false|null)(?!\"[^\"]*\")");
   rule.format.setForeground(Qt::magenta);
   rules.append(rule);
}

void JsonSyntaxHighlighter::highlightBlock(const QString &text){
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
