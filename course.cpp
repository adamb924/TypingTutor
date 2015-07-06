#include "course.h"

#include <QtDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QRegularExpression>

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

QString Course::navigationPaneStyle() const
{
    return mNavigationPaneStyle;
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

void Course::setNavigationPaneStyle(const QString &str)
{
    mNavigationPaneStyle = str;
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
    stream.writeAttribute("navigation-pane-style", mNavigationPaneStyle );
    stream.writeAttribute("layout-direction", QString("%1").arg(mLayoutDirection) );

    stream.writeAttribute("error-foreground", mErrorFg.name() );
    stream.writeAttribute("error-background", mErrorBg.name() );
    stream.writeAttribute("remainder-foreground", mRemainderFg.name() );
    stream.writeAttribute("remainder-background", mRemainderBg.name() );

    writeHtmlElement( stream, "course-description", mDescription );

    writeHtmlElement( stream, "course-conclusion", mConclusionMessage, QXmlStreamAttribute("header", mConclusionHeader) );

    for(int i=0; i<mSections.count(); i++)
    {
        stream.writeStartElement("section");
        stream.writeAttribute("name", mSections.at(i)->name() );

        writeHtmlElement( stream, "section-description", mSections.at(i)->description() );

        for(int j=0; j<mSections.at(i)->prompts()->count(); j++)
        {
            stream.writeStartElement("prompt");
            writeHtmlElement( stream, "prompt-description", mSections.at(i)->prompts()->at(j)->description());
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

    setChanged(false);
}

void Course::writeHtmlElement(QXmlStreamWriter & stream, const QString &elementName, QString html, const QXmlStreamAttributes & attributes)
{
    stream.writeStartElement(elementName);
    stream.writeAttribute("xmlns","http://www.w3.org/1999/xhtml");
    stream.writeAttributes(attributes);
    stream.writeCharacters("");
    QRegularExpression filter("[ ]?style=\"[^\"]*\"");
    html.replace(filter, "");
    stream.device()->write( html.toUtf8() );
    stream.writeEndElement();
}

void Course::writeHtmlElement(QXmlStreamWriter &stream, const QString &elementName, QString html, const QXmlStreamAttribute &attribute)
{
    QXmlStreamAttributes attr;
    attr.insert(0, attribute);
    writeHtmlElement(stream, elementName, html, attr);
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
        if( attr.hasAttribute("name") )
            mName = attr.value("name").toString();
        if( attr.hasAttribute("text-entry-style") )
            mTextEntryStyle = attr.value("text-entry-style").toString();
        if( attr.hasAttribute("prompt-style") )
            mPromptStyle = attr.value("prompt-style").toString();
        if( attr.hasAttribute("header-style") )
            mHeaderStyle = attr.value("header-style").toString();
        if( attr.hasAttribute("description-style") )
            mDescriptionStyle = attr.value("description-style").toString();
        if( attr.hasAttribute("navigation-pane-style") )
            mNavigationPaneStyle = attr.value("navigation-pane-style").toString();
        if( attr.hasAttribute("layout-direction") )
            mLayoutDirection = (Qt::LayoutDirection)attr.value("layout-direction").toInt();
        if( attr.hasAttribute("error-foreground") )
            mErrorFg = QColor( attr.value("error-foreground").toString() );
        if( attr.hasAttribute("error-background") )
            mErrorBg = QColor( attr.value("error-background").toString() );
        if( attr.hasAttribute("remainder-foreground") )
            mRemainderFg = QColor( attr.value("remainder-foreground").toString() );
        if( attr.hasAttribute("remainder-background") )
            mRemainderBg = QColor( attr.value("remainder-background").toString() );
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
          QXmlStreamAttributes attr = xml.attributes();
          QXmlStreamAttributes filtered;
          switch( xml.tokenType() )
          {
          case QXmlStreamReader::StartElement:
              writer.writeStartElement(xml.name().toString());
              for(int i=0; i<attr.count(); i++)
              {
                  if( attr.at(i).name() != "style" )
                  {
                      filtered.insert(0, attr.at(i) );
                  }
              }
              writer.writeAttributes(filtered);
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

bool Course::changed() const
{
    return mChanged;
}

void Course::setChanged(bool changed)
{
    mChanged = changed;
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

