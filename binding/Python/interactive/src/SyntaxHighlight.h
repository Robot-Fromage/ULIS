// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SyntaxHighlight.h
* @author       Clement Berthaud
* @brief        Info application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
    const QTextCharFormat getTextCharFormatForColor( const QColor& color, const QString& style = QString() );

    QList<FHighlightingRule> rules;
    QRegExp triSingleQuote;
    QRegExp triDoubleQuote;
};

