#ifndef COURSE_H
#define COURSE_H

#include <QString>
#include <QList>

#include "section.h"
#include "keyboard.h"

class QXmlStreamReader;

class Course
{
public:
    Course(const QString & filename);

    QString name() const;
    QString description() const;
    bool isValid() const;
    QList<Section>* sections();

    QString conclusionHeader() const;
    QString conclusionMessage() const;
    QString keyboardFilename() const;

    QString textEntryStyle() const;
    QString promptStyle() const;

    QString headerStyle() const;
    QString descriptionStyle() const;

    const Keyboard *keyboard() const;

private:
    bool readXmlFile(const QString & filename);
    QString readHtml( QXmlStreamReader &xml );

    QString mName;
    QString mDescription;
    QString mConclusionHeader;
    QString mConclusionMessage;
    QString mKeyboardFilename;

    QString mTextEntryStyle;
    QString mPromptStyle;

    QString mHeaderStyle;
    QString mDescriptionStyle;

    bool mValid;

    Keyboard mKeyboard;

    QList<Section> mSections;
};

#endif // COURSE_H
