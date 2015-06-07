#include "letter.h"

Letter::Letter(const QString & unicode, const QString & hint, const QString & prompt) :
    mUnicode(unicode),
    mHint(hint),
    mPrompt(prompt)
{
}

QString Letter::unicode() const
{
    return mUnicode;
}

QString Letter::hint() const
{
    return mHint;
}

QString Letter::prompt() const
{
    return mPrompt;
}

void Letter::setUnicode(const QString &str)
{
    mUnicode = str;
}

void Letter::setHint(const QString &str)
{
    mHint = str;
}

void Letter::setPrompt(const QString &str)
{
    mPrompt = str;
}
