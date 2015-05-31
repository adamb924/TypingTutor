#ifndef LETTER_H
#define LETTER_H

#include <QString>

class Letter
{
public:
    Letter(const QString & unicode, const QString & hint, const QString & prompt);

    QString unicode() const;
    QString hint() const;
    QString prompt() const;

private:
    QString mUnicode;
    QString mHint;
    QString mPrompt;
};

#endif // LETTER_H
