#ifndef LETTER_H
#define LETTER_H

#include <QString>

class Letter
{
public:
    Letter(const QString & unicode = QString(), const QString & hint = QString(), const QString & prompt = QString());

    QString unicode() const;
    QString hint() const;
    QString prompt() const;

    void setUnicode(const QString & str);
    void setHint(const QString & str);
    void setPrompt(const QString & str);

private:
    QString mUnicode;
    QString mHint;
    QString mPrompt;
};

#endif // LETTER_H
