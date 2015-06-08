#include "targethighlighter.h"

#include <QtDebug>

#include "promptform.h"
#include "course.h"

TargetHighlighter::TargetHighlighter(const Course *course, QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    badFormat.setForeground( course->errorFg() );
    badFormat.setBackground( course->errorBg() );
}

void TargetHighlighter::setTargetText(const QString &targetText)
{
    mTargetText = targetText;
}

void TargetHighlighter::highlightBlock(const QString &text)
{
    int matchLength = PromptForm::getFirstNonmatching(text, mTargetText);
    if( matchLength == -1 )
    {
        setFormat(0, text.length(), goodFormat );
    }
    else
    {
        setFormat(0, matchLength, goodFormat );
        setFormat(matchLength, text.length() - matchLength, badFormat );
    }
}
