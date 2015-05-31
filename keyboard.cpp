#include "keyboard.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QtDebug>

Keyboard::Keyboard()
{
}

Keyboard::Keyboard(const QString &filename)
{
    loadKeyboardFile(filename);
}

bool Keyboard::loadKeyboardFile(const QString &filename)
{
    mLetters.clear();

    QFile file(filename);
    file.open(QFile::ReadOnly);
    QXmlStreamReader xml(&file);

    while (!xml.atEnd())
    {
        xml.readNext();
        QXmlStreamAttributes  attr = xml.attributes();
        if( xml.name() == "letter" )
        {
            if( attr.hasAttribute("unicode")
                    && attr.hasAttribute("hint")
                    && attr.hasAttribute("prompt") )
            {
                mLetters.append( Letter( attr.value("unicode").toString() , attr.value("hint").toString() , attr.value("prompt").toString() ) );
            }
        }
        else if ( xml.name() == "keyboard" )
        {
            if( attr.hasAttribute("name") )
            {
                mName = attr.value("name").toString();
            }
        }
    }
    if (xml.hasError())
    {
        qWarning() << "Error (Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << "): " << xml.errorString();
        return false;
    }    
    return true;
}

QString Keyboard::name() const
{
    return mName;
}

bool Keyboard::isValid() const
{
    return mLetters.count();
}

QString Keyboard::nextThingToType(const QString &remainder) const
{
    for(int i=0; i<mLetters.count(); i++)
    {
        if( remainder.startsWith( mLetters.at(i).unicode() ) )
        {
            return mLetters.at(i).prompt();
        }
    }
    qWarning() << QString("%1").arg( remainder.at(0).unicode() , 0, 16);
    return "Err";
}

QString Keyboard::nextHint(const QString &remainder) const
{
    for(int i=0; i<mLetters.count(); i++)
    {
        if( remainder.startsWith( mLetters.at(i).unicode() ) )
        {
            return mLetters.at(i).hint();
        }
    }
    qWarning() << QString("%1").arg( remainder.at(0).unicode() , 0, 16);
    return "Err";
}
