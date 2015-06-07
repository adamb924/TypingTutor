#include "keyboard.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QtDebug>

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
    qDeleteAll(mLetters);
}

QString Keyboard::name() const
{
    return mName;
}

void Keyboard::setName(const QString &name)
{
    mName = name;
}

void Keyboard::appendLetter(Letter * letter)
{
    mLetters.append(letter);
}

bool Keyboard::isValid() const
{
    return mLetters.count() > 0;
}

QString Keyboard::nextThingToType(const QString &remainder) const
{
    for(int i=0; i<mLetters.count(); i++)
    {
        if( remainder.startsWith( mLetters.at(i)->unicode() ) )
        {
            return mLetters.at(i)->prompt();
        }
    }
    qWarning() << QString("%1").arg( remainder.at(0).unicode() , 0, 16);
    return "Err";
}

QString Keyboard::nextHint(const QString &remainder) const
{
    for(int i=0; i<mLetters.count(); i++)
    {
        if( remainder.startsWith( mLetters.at(i)->unicode() ) )
        {
            return mLetters.at(i)->hint();
        }
    }
    qWarning() << QString("%1").arg( remainder.at(0).unicode() , 0, 16);
    return "Err";
}

QList<Letter *> *Keyboard::letters()
{
    return &mLetters;
}
