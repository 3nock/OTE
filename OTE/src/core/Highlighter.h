//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef CORE_HIGHLIGHTER_H
#define CORE_HIGHLIGHTER_H

#include <QRegExp>
#include <QSyntaxHighlighter>

namespace OTE {

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    enum Language {
        CodeTXT,
        CodeJSON,
        CodeXML,
        CodeXMLComment
    };

    enum Token {
        CodeKeyWord,
        CodeString,
        CodeComment,
        CodeLiteral,
        CodeNumeral,
        CodeXmlAttr
    };

    explicit SyntaxHighlighter(QTextDocument *doc, Language language = Language::CodeTXT);

    void setCurrentLanguage(Language language);
    Language currentLanguage();

protected:
    void highlightBlock(const QString &text) override;

private:
    void highlightSyntax(const QString &text);
    int toEndOfString(const QString &text, int i);
    int highlightNumericLiterals(const QString &text, int i);
    int highlightStringLiterals(const QString &text, int i);
    int highlightLiterals(int i, const QString &text);

    static constexpr inline bool isOctal(const char c) {
        return (c >= '0' && c <= '7');
    }

    static constexpr inline bool isHex(const char c) {
        return (
            (c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F')
        );
    }

    void jsonHighlighter(const QString &text, int start_at = 0);
    void xmlHighlighter(const QString &text);

    static inline QStringRef strMidRef(const QString& str, int position, int n = -1){
        return str.midRef(position, n);
    }

    QHash<Token, QTextCharFormat> _formats;
    Language _language;
};

class SyntaxHighlighterTheme
{
public:
    static QHash<SyntaxHighlighter::Token, QTextCharFormat> formats()
    {
        QHash<SyntaxHighlighter::Token, QTextCharFormat> _formats;

        QTextCharFormat defaultFormat = QTextCharFormat();
        _formats[SyntaxHighlighter::Token::CodeKeyWord] = defaultFormat;
        _formats[SyntaxHighlighter::Token::CodeString] = defaultFormat;
        _formats[SyntaxHighlighter::Token::CodeComment] = defaultFormat;
        _formats[SyntaxHighlighter::Token::CodeLiteral] = defaultFormat;
        _formats[SyntaxHighlighter::Token::CodeNumeral] = defaultFormat;
        _formats[SyntaxHighlighter::Token::CodeXmlAttr] = defaultFormat;

        return _formats;
    }

    static QHash<SyntaxHighlighter::Token, QTextCharFormat> customTheme()
    {
        QHash<SyntaxHighlighter::Token, QTextCharFormat> _formats = formats();

        _formats[SyntaxHighlighter::Token::CodeKeyWord].setForeground(QColor(10,160,170));
        _formats[SyntaxHighlighter::Token::CodeString].setForeground(QColor(10,160,170));
        _formats[SyntaxHighlighter::Token::CodeComment].setForeground(QColor(95, 180, 180));
        _formats[SyntaxHighlighter::Token::CodeComment].setFontItalic(true);
        _formats[SyntaxHighlighter::Token::CodeLiteral].setForeground(Qt::magenta);
        _formats[SyntaxHighlighter::Token::CodeNumeral].setForeground(Qt::green);
        _formats[SyntaxHighlighter::Token::CodeXmlAttr].setForeground(QColor(95, 180, 180));
        return _formats;
    }
};

} // namespace OTE

#endif // CORE_HIGHLIGHTER_H
