#include "course.h"

#include <QtDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

Course::Course() :
    mRemainderFg(0,0,155),
    mRemainderBg(255,255,255),
    mErrorFg(255,255,255),
    mErrorBg(192,192,192)
{
    mValid = true;
}

Course::Course(const QString &filename) :
    mRemainderFg(0,0,155),
    mRemainderBg(255,255,255),
    mErrorFg(255,255,255),
    mErrorBg(192,192,192)
{
    mValid = readXmlFile(filename);
}

Course::~Course()
{
    qDeleteAll(mSections);
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

void Course::setTextEntryStyle(const QString &str)
{
    mTextEntryStyle = str;
}

void Course::setPromptStyle(const QString &str)
{
    mPromptStyle = str;
}

void Course::setHeaderStyle(const QString &str)
{
    mHeaderStyle = str;
}

void Course::setDescriptionStyle(const QString &str)
{
    mDescriptionStyle = str;
}

const Keyboard *Course::keyboard() const
{
    return &mKeyboard;
}

Keyboard *Course::keyboard()
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

void Course::insertSectionAt(Section *section, int i)
{
    Q_ASSERT( i < mSections.count() );
    mSections.insert( i, section );
}

void Course::removeSectionAt(int i)
{
    Q_ASSERT( i < mSections.count() );
    delete mSections.takeAt( i );
}

int Course::getSectionIndex(const Prompt *prompt) const
{
    for(int i=0; i<mSections.count(); i++)
    {
        for(int j=0; j<mSections.at(i)->prompts()->count(); j++)
        {
            if( mSections.at(i)->prompts()->at(j) == prompt )
            {
                return i;
            }
        }
    }
    return -1;
}

Section *Course::getSection(const Prompt *prompt)
{
    int index = getSectionIndex(prompt);
    if( index != -1 )
    {
        return mSections.at(index);
    }
    return 0;
}

void Course::writeXmlFile(const QString &filename)
{
    QFile file(filename);
    file.open(QFile::WriteOnly);
    QXmlStreamWriter stream(&file);

    stream.setCodec("UTF-8");
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("course");
    stream.writeAttribute("name", mName );
    stream.writeAttribute("text-entry-style", mTextEntryStyle );
    stream.writeAttribute("prompt-style", mPromptStyle );
    stream.writeAttribute("header-style", mHeaderStyle );
    stream.writeAttribute("description-style", mDescriptionStyle );
    stream.writeAttribute("layout-direction", QString("%1").arg(mLayoutDirection) );

    stream.writeAttribute("error-foreground", mErrorFg.name() );
    stream.writeAttribute("error-background", mErrorBg.name() );
    stream.writeAttribute("remainder-foreground", mRemainderFg.name() );
    stream.writeAttribute("remainder-background", mRemainderBg.name() );

    stream.writeStartElement("course-description");
    stream.writeAttribute("xmlns","http://www.w3.org/1999/xhtml");
    stream.writeCharacters("");
    stream.device()->write( mDescription.toUtf8() );
    stream.writeEndElement();

    stream.writeStartElement("course-conclusion");
    stream.writeAttribute("header", mConclusionHeader);
    stream.writeCharacters("");
    stream.device()->write( mConclusionMessage.toUtf8() );
    stream.writeEndElement();

    for(int i=0; i<mSections.count(); i++)
    {
        stream.writeStartElement("section");
        stream.writeAttribute("name", mSections.at(i)->name() );

        stream.writeStartElement("section-description");
        stream.writeAttribute("xmlns","http://www.w3.org/1999/xhtml");
        stream.writeCharacters("");
        stream.device()->write( mSections.at(i)->description().toUtf8() );
        stream.writeEndElement();

        for(int j=0; j<mSections.at(i)->prompts()->count(); j++)
        {
            stream.writeStartElement("prompt");

            stream.writeStartElement("prompt-description");
            stream.writeAttribute("xmlns","http://www.w3.org/1999/xhtml");
            stream.writeCharacters("");
            stream.device()->write( mSections.at(i)->prompts()->at(j)->description().toUtf8() );
            stream.writeEndElement();

            stream.writeTextElement("target-text", mSections.at(i)->prompts()->at(j)->targetText() );

            stream.writeEndElement();
        }
        stream.writeEndElement();
    }

    stream.writeStartElement("keyboard");
    stream.writeAttribute("name", mKeyboard.name() );
    for(int i=0; i<mKeyboard.letters()->count(); i++)
    {
        stream.writeEmptyElement("letter");
        stream.writeAttribute("unicode", mKeyboard.letters()->at(i)->unicode() );
        stream.writeAttribute("hint", mKeyboard.letters()->at(i)->hint() );
        stream.writeAttribute("prompt", mKeyboard.letters()->at(i)->prompt() );
    }

    stream.writeEndElement(); // keyboard

    stream.writeEndElement(); // course

    stream.writeEndDocument();
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
                && attr.hasAttribute("text-entry-style")
                && attr.hasAttribute("prompt-style")
                && attr.hasAttribute("header-style")
                && attr.hasAttribute("description-style")
                && attr.hasAttribute("layout-direction")
                && attr.hasAttribute("error-foreground")
                && attr.hasAttribute("error-background")
                && attr.hasAttribute("remainder-foreground")
                && attr.hasAttribute("remainder-background")
                )
        {
            mName = attr.value("name").toString();
            mTextEntryStyle = attr.value("text-entry-style").toString();
            mPromptStyle = attr.value("prompt-style").toString();
            mHeaderStyle = attr.value("header-style").toString();
            mDescriptionStyle = attr.value("description-style").toString();
            mLayoutDirection = (Qt::LayoutDirection)attr.value("layout-direction").toInt();
            mErrorFg = QColor( attr.value("error-foreground").toString() );
            mErrorBg = QColor( attr.value("error-background").toString() );
            mRemainderFg = QColor( attr.value("remainder-foreground").toString() );
            mRemainderBg = QColor( attr.value("remainder-background").toString() );
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
                QString name, description;
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

                mSections.append( new Section(name, description) );
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
            /// for reading the keyboard
            else if( xml.name() == "letter" )
            {
                QXmlStreamAttributes  attr = xml.attributes();
                if( attr.hasAttribute("unicode")
                        && attr.hasAttribute("hint")
                        && attr.hasAttribute("prompt") )
                {
                    mKeyboard.appendLetter( new Letter( attr.value("unicode").toString() , attr.value("hint").toString() , attr.value("prompt").toString() ) );
                }
            }
            else if ( xml.name() == "keyboard" )
            {
                QXmlStreamAttributes  attr = xml.attributes();
                if( attr.hasAttribute("name") )
                {
                    mKeyboard.setName( attr.value("name").toString() );
                }
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
QColor Course::errorBg() const
{
    return mErrorBg;
}

void Course::setErrorBg(const QColor &value)
{
    mErrorBg = value;
}

QColor Course::errorFg() const
{
    return mErrorFg;
}

void Course::setErrorFg(const QColor &value)
{
    mErrorFg = value;
}

QColor Course::remainderBg() const
{
    return mRemainderBg;
}

void Course::setRemainderBg(const QColor &value)
{
    mRemainderBg = value;
}

QColor Course::remainderFg() const
{
    return mRemainderFg;
}

void Course::setRemainderFg(const QColor &value)
{
    mRemainderFg = value;
}


Qt::LayoutDirection Course::textDirection() const
{
    return mLayoutDirection;
}

void Course::setTextDirection(Qt::LayoutDirection value)
{
    mLayoutDirection = value;
}

