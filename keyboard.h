#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QList>
#include "letter.h"

class Keyboard
{
public:
    Keyboard();

    QString name() const;

    void setName(const QString & name);
    void appendLetter( const Letter & letter );

    bool isValid() const;

    QString nextThingToType(const QString & remainder) const;
    QString nextHint(const QString & remainder) const;

private:
    QString mName;
    QList<Letter> mLetters;
};

#endif // KEYBOARD_H
