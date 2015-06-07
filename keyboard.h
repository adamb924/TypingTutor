#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QList>
#include "letter.h"

class Keyboard
{
public:
    Keyboard();
    ~Keyboard();

    QString name() const;

    void setName(const QString & name);
    void appendLetter(Letter *letter );

    bool isValid() const;

    QString nextThingToType(const QString & remainder) const;
    QString nextHint(const QString & remainder) const;

    QList<Letter*>* letters();

private:
    QString mName;
    QList<Letter*> mLetters;
};

#endif // KEYBOARD_H
