#include "course.h"

#include <QtDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

Course::Course(const QString &filename)
{
    mValid = readXmlFile(filename);
}

QString Course::name() const
{
    return mName;
}

QString Course::description() const
{
    return mDescription;
}

bool Course::isValid() const
{
    return mValid;
}

QList<Section *> *Course::sections()
{
    return &mSections;
}

QString Course::conclusionHeader() const
{
    return mConclusionHeader;
}

QString Course::conclusionMessage() const
{
    return mConclusionMessage;
}

QString Course::keyboardFilename() const
{
    return mKeyboardFilename;
}

QString Course::textEntryStyle() const
{
    return mTextEntryStyle;
}

QString Course::promptStyle() const
{
    return mPromptStyle;
}

QString Course::headerStyle() const
{
    return mHeaderStyle;
}

QString Course::descriptionStyle() const
{
    return mDescriptionStyle;
}

const Keyboard *Course::keyboard() const
{
    return &mKeyboard;
}

void Course::setName(const QString &str)
{
    mName = str;
}

void Course::setDescription(const QString &str)
{
    mDescription = str;
}

void Course::setConclusionHeader(const QString &str)
{
    mConclusionHeader = str;
}

void Course::setConclusionMessage(const QString &str)
{
    mConclusionMessage = str;
}

bool Course::readXmlFile(const QString &filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);
    QXmlStreamReader xml(&file);

    xml.readNextStartElement();
    if( xml.name() == "course" )
    {
        QXmlStreamAttributes  attr = xml.attributes();
        if( attr.hasAttribute("name")
                && attr.hasAttribute("keyboard-filename")
                && attr.hasAttribute("text-entry-style")
                && attr.hasAttribute("prompt-style")
                && attr.hasAttribute("header-style")
                && attr.hasAttribute("description-style")
                )
        {
            mName = attr.value("name").toString();
            mKeyboardFilename = attr.value("keyboard-filename").toString();
            mTextEntryStyle = attr.value("text-entry-style").toString();
            mPromptStyle = attr.value("prompt-style").toString();
            mHeaderStyle = attr.value("header-style").toString();
            mDescriptionStyle = attr.value("description-style").toString();
        }
    }

    xml.readNextStartElement();
    if ( xml.name() == "course-description" )
    {
        mDescription = readHtml(xml);
    }

    xml.readNextStartElement();
    if ( xml.name() == "course-conclusion" )
    {
        QXmlStreamAttributes  attr = xml.attributes();
        if( attr.hasAttribute("header") )
        {
            mConclusionHeader = attr.value("header").toString();
        }
        mConclusionMessage = readHtml(xml);
    }

    while (!xml.atEnd())
    {
        xml.readNext();
        if( xml.isStartElement() )
        {
            if ( xml.name() == "section" )
            {
                QString name, description, conclusionHeader, conclusionMessage;
                QXmlStreamAttributes  attr = xml.attributes();
                if( attr.hasAttribute("name") )
                {
                    name = attr.value("name").toString();
                }

                xml.readNextStartElement();
                if ( xml.name() == "section-description" )
                {
                    description = readHtml(xml);
                }

                xml.readNextStartElement();
                if ( xml.name() == "section-conclusion" )
                {
                    QXmlStreamAttributes  attr = xml.attributes();
                    if( attr.hasAttribute("header") )
                    {
                        conclusionHeader = attr.value("header").toString();
                    }
                    conclusionMessage = readHtml(xml);
                }

                mSections.append( new Section(name, description, conclusionHeader, conclusionMessage) );
            }
            else if ( xml.name() == "prompt" )
            {
                QString targetText, description;

                xml.readNextStartElement();
                if ( xml.name() == "prompt-description" )
                {
                    description = readHtml(xml);
                }

                xml.readNextStartElement();
                if ( xml.name() == "target-text" )
                {
                    targetText = readHtml(xml);
                }

                mSections.last()->addPrompt( new Prompt( targetText, description ) );
            }
        }
    }
    if (xml.hasError())
    {
        qWarning() << "Error (Line " << xml.lineNumber() << ", Column " << xml.columnNumber() << "): " << xml.errorString();
        return false;
    }

    QFileInfo inSameDirectory(file);
    if ( inSameDirectory.dir().exists( mKeyboardFilename ) )
    {
        mKeyboard = Keyboard( inSameDirectory.dir().absoluteFilePath(mKeyboardFilename) );
    }
    else if ( QDir::current().exists( mKeyboardFilename ) )
    {
        mKeyboard = Keyboard( QDir::current().absoluteFilePath(mKeyboardFilename) );
    }
    else
    {
        qWarning() << "Could not find the keyboard file: " << mKeyboardFilename;
        return false;
    }

    mKeyboard = Keyboard(mKeyboardFilename);

    return true;
}

QString Course::readHtml(QXmlStreamReader &xml)
{
    if( xml.namespaceUri().toString() != "http://www.w3.org/1999/xhtml" )
    {
        return xml.readElementText(QXmlStreamReader::IncludeChildElements);
    }
    QString terminatingElement = xml.name().toString();
    QString html;
    QXmlStreamWriter writer(&html);
    do
    {
          xml.readNext();
          switch( xml.tokenType() )
          {
          case QXmlStreamReader::StartElement:
              writer.writeStartElement(xml.name().toString());
              writer.writeAttributes(xml.attributes());
              break;
          case QXmlStreamReader::EndElement:
              writer.writeEndElement();
              break;
          case QXmlStreamReader::Characters:
              writer.writeCharacters(xml.text().toString());
              break;
          // a more thorough approach would handle these; enumerating them removes a compiler warning
          case QXmlStreamReader::NoToken:
          case QXmlStreamReader::Invalid:
          case QXmlStreamReader::StartDocument:
          case QXmlStreamReader::EndDocument:
          case QXmlStreamReader::Comment:
          case QXmlStreamReader::DTD:
          case QXmlStreamReader::EntityReference:
          case QXmlStreamReader::ProcessingInstruction:
              break;
          }
    }
    while (!xml.atEnd() && xml.name() != terminatingElement );
    return html;
}
