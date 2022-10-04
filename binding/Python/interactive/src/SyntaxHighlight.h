// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SyntaxHighlight.h
* @author       Clement Berthaud
* @brief        Info application for ULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once

#include <QSyntaxHighlighter>

class FHighlightingRule
{
public:
    FHighlightingRule(const QString& patternStr, int n, const QTextCharFormat& matchingFormat )
    {
        originalRuleStr = patternStr;
        pattern = QRegExp( patternStr );
        nth = n;
        format = matchingFormat;
    }

    QString originalRuleStr;
    QRegExp pattern;
    int nth;
    QTextCharFormat format;
};

class FPythonSyntaxHighlighter
    : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    FPythonSyntaxHighlighter( QTextDocument* iParent = nullptr );

protected:
     void highlightBlock( const QString& iText );

private:
    QStringList keywords;
    QStringList operators;
    QStringList braces;

    QHash< QString, QTextCharFormat > basicStyles;

    void initializeRules();
    bool matchMultiline( const QString& text, const QRegExp& delimiter, const int inState, const QTextCharFormat& style );
    const QTextCharFormat getTextCharFormat( const QString& colorName, const QString& style = QString() );

    QList<FHighlightingRule> rules;
    QRegExp triSingleQuote;
    QRegExp triDoubleQuote;
};

