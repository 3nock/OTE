#include "SyntaxHighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *document):
    QSyntaxHighlighter(document) {}

SyntaxHighlighter::~SyntaxHighlighter(){}

void SyntaxHighlighter::json_dark(){
    HighlightingRule rule;

    // numbers
    rule.pattern = QRegExp("([-0-9.]+)(?!([^\"]*\"[\\s]*\\:))");
    rule.format.setForeground(Qt::darkGreen);
    rules.append(rule);

    // special strings
    rule.pattern = QRegExp("(true|false|null)(?!\"[^\"]*\")");
    rule.format.setForeground(Qt::red);
    rules.append(rule);

    // keys
    rule.pattern = QRegExp ("(\"[^\"]*\")");
    rule.format.setForeground(Qt::blue);
    rules.append(rule);

    // strings
    rule.pattern = QRegExp(":\\s*([\"](?:[^\"])*[\"])");
    rule.format.setForeground(Qt::black);
    rules.append(rule);
}

void SyntaxHighlighter::logs_dark(){
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

void SyntaxHighlighter::json_light() {
    HighlightingRule rule;

    // numbers
    rule.pattern = QRegExp("([-0-9.]+)(?!([^\"]*\"[\\s]*\\:))");
    rule.format.setForeground(Qt::green);
    rules.append(rule);

    // special strings
    rule.pattern = QRegExp("(true|false|null)(?!\"[^\"]*\")");
    rule.format.setForeground(Qt::magenta);
    rules.append(rule);

    // keys
    rule.pattern = QRegExp ("(\"[^\"]*\")");
    rule.format.setForeground(Qt::cyan);
    rules.append(rule);

    // strings
    rule.pattern = QRegExp(":\\s*([\"](?:[^\"])*[\"])");
    rule.format.setForeground(Qt::white);
    rules.append(rule);
}

void SyntaxHighlighter::Logs_light(){
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

void SyntaxHighlighter::highlightBlock(const QString &text){
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
