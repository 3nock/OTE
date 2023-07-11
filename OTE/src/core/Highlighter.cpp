//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "Highlighter.h"

#include <algorithm>

#include <QDebug>
#include <QRegularExpression>
#include <QTextDocument>

namespace OTE {

const QMultiHash<char, QLatin1String> cJSON_LITERALS {
    {('f'), QLatin1String("false")},
    {('n'), QLatin1String("null")},
    {('t'), QLatin1String("true")}
};

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *doc, Language language):
    QSyntaxHighlighter(doc),
    _formats(SyntaxHighlighterTheme::customTheme()),
    _language(language)
{
}

void SyntaxHighlighter::setCurrentLanguage(Language language)
{
    if (language != _language)
        _language = language;
}

SyntaxHighlighter::Language SyntaxHighlighter::currentLanguage()
{
    return _language;
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    if(_language == CodeTXT)
        return;
    else if(currentBlock() == document()->firstBlock())
        setCurrentBlockState(_language);
    else
        setCurrentBlockState(previousBlockState());

    highlightSyntax(text);
}

void SyntaxHighlighter::highlightSyntax(const QString &text)
{
    if (text.isEmpty())
        return;

    switch (_language) {
        case CodeTXT:
            break;
        case CodeJSON:
            jsonHighlighter(text);
            break;
        case CodeXML:
        case CodeXMLComment:
            xmlHighlighter(text);
            break;
    }
}

int SyntaxHighlighter::toEndOfString(const QString &text, int i)
{
    while (i < text.length())
    {
        ++i;
        if (text.at(i) == '"')
        {
            if(text.at(i-1) != QLatin1Char('\\'))
            {
                ++i;
                break;
            }
        }
    }
    return i;
}

int SyntaxHighlighter::highlightStringLiterals(const QString &text, int i)
{
    int pos = i;
    while (i < text.length())
    {
        ++i;
        if (text.at(i) == '"')
        {
            if(text.at(i-1) != QLatin1Char('\\'))
            {
                ++i;
                break;
            }
        }
    }
    setFormat(pos, i-pos,  _formats[CodeString]);
    return i;
}

int SyntaxHighlighter::highlightLiterals(int i, const QString &text)
{
    // check if we are at the beginning OR if this is the start of a word
    if (i == 0 || (!text.at(i - 1).isLetterOrNumber() &&
                   text.at(i-1) != QLatin1Char('_')))
    {
        const auto wordList = cJSON_LITERALS.values(text.at(i).toLatin1());
        for (const QLatin1String &word : wordList)
        {
            // we have a word match check
            // 1. if we are at the end
            // 2. if we have a complete word
            if (word == strMidRef(text, i, word.size()) &&
                    (i + word.size() == text.length() ||
                     (!text.at(i + word.size()).isLetterOrNumber() &&
                      text.at(i + word.size()) != QLatin1Char('_'))))
            {
                setFormat(i, word.size(), _formats[CodeLiteral]);
                i += word.size();
            }
        }
    }
    return i;
}

int SyntaxHighlighter::highlightNumericLiterals(const QString &text, int i)
{
    int count = 0;
    for(int j = i; j < text.length(); j++)
    {
        if(text[j].isNumber())
            count++;
        else
            break;
    }

    setFormat(i, count, _formats[CodeNumeral]);

    return i+count;
}

void SyntaxHighlighter::jsonHighlighter(const QString &text, int start_at)
{
    bool isName = true;
    const auto textLen = text.length();

    for (int i = start_at; i < textLen; ++i)
    {
        while (i < textLen && !text[i].isLetter())
        {
            if (text[i].isSpace())
            {
                ++i;
                //make sure we don't cross the bound
                if (i == textLen)
                    return;

                if (text[i].isLetter())
                    break;
                else
                    continue;
            }
            else if (text[i].isNumber())
                i = highlightNumericLiterals(text, i);
            else if (text[i] == QLatin1Char('\"')){
                if(isName)
                {
                    i = highlightStringLiterals(text, i);
                    isName = false;
                }
                else
                    i = toEndOfString(text, i);
            }

            if (i >= textLen)
                return;
            ++i;
        }

        if (i >= textLen || !text[i].isLetter())
            continue;

        i = highlightLiterals(i, text);
    }
}

void SyntaxHighlighter::xmlHighlighter(const QString &text)
{
    if (text.isEmpty())
        return;
    const auto textLen = text.length();

    int insideTag = 0;
    for (int i = 0; i < textLen; ++i)
    {
        if(currentBlockState() == CodeXMLComment)
            goto XMLComment;

        if (((i+3) < textLen) &&
            text[i] == QLatin1Char('<') &&
            text[i+1] == QLatin1Char('!') &&
            text[i+2] == QLatin1Char('-') &&
            text[i+3] == QLatin1Char('-'))
        {
XMLComment:
            int next = text.indexOf("-->", i);
            if (next == -1)
            {
                //we didn't find a comment end. Check if we are already in a comment block
                if (currentBlockState() == CodeXML)
                    setCurrentBlockState(CodeXMLComment);
                setFormat(i, textLen,  _formats[CodeComment]);
                return;
            }
            else
            {
                //we found a comment end. Mark this block as code if it was previously comment
                if (currentBlockState() == CodeXMLComment)
                    setCurrentBlockState(CodeXML);
                next += 3;
                setFormat(i, next - i,  _formats[CodeComment]);
                i = next;
                if (i >= textLen)
                    return;
            }
        }

        if (text[i] == QLatin1Char('<'))
        {
            const int found = text.indexOf(QLatin1Char('>'), i);
            if (found)
            {
                insideTag = found;
                ++i;
                if (text[i] == QLatin1Char('/'))
                    ++i;
                setFormat(i, found - i, _formats[CodeKeyWord]);
            }
        }

        if ((i < insideTag) && text[i] == QLatin1Char('='))
        {
            int lastSpace = text.lastIndexOf(QLatin1Char(' '), i);
            if (lastSpace == i-1)
                lastSpace = text.lastIndexOf(QLatin1Char(' '), i-2);
            if (lastSpace > 0)
                setFormat(lastSpace, i - lastSpace, _formats[CodeXmlAttr]);
        }

        if ((i < insideTag) &&
            (text[i] == QLatin1Char('"') || text[i] == QLatin1Char('\'')))
        {
            const int pos = i;
            int cnt = 1;
            ++i;

            if ( (i+1) >= textLen)
                return;
            while (i < textLen)
            {
                if (text[i] == QLatin1Char('"') || text[i] == QLatin1Char('\''))
                {
                    ++cnt;
                    ++i;
                    break;
                }
                ++i;
                ++cnt;

                if ( (i+1) >= textLen)
                {
                    ++cnt;
                    break;
                }
            }
            setFormat(pos, cnt, _formats[CodeXmlAttr]);
        }
    }
}

} // namespace OTE
