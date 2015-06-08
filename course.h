#ifndef COURSE_H
#define COURSE_H

#include <QString>
#include <QList>
#include <QColor>

#include "section.h"
#include "keyboard.h"

class QXmlStreamReader;

class Course
{
public:
    Course();
    Course(const QString & filename);
    ~Course();

    QString name() const;
    QString description() const;
    bool isValid() const;
    QList<Section*>* sections();

    QString conclusionHeader() const;
    QString conclusionMessage() const;

    QString textEntryStyle() const;
    QString promptStyle() const;
    QString headerStyle() const;
    QString descriptionStyle() const;

    void setTextEntryStyle(const QString & str);
    void setPromptStyle(const QString & str);
    void setHeaderStyle(const QString & str);
    void setDescriptionStyle(const QString & str);

    const Keyboard *keyboard() const;
    Keyboard *keyboard();

    void setName(const QString & str);
    void setDescription(const QString & str);
    void setConclusionHeader(const QString & str);
    void setConclusionMessage(const QString & str);

    void insertSectionAt(Section * section, int i);
    void removeSectionAt(int i);
    int getSectionIndex(const Prompt * prompt) const;
    Section * getSection(const Prompt * prompt);

    void writeXmlFile(const QString & filename);

    Qt::LayoutDirection textDirection() const;
    void setTextDirection(Qt::LayoutDirection value);

    QColor remainderFg() const;
    void setRemainderFg(const QColor &value);

    QColor remainderBg() const;
    void setRemainderBg(const QColor &value);

    QColor errorFg() const;
    void setErrorFg(const QColor &value);

    QColor errorBg() const;
    void setErrorBg(const QColor &value);

    bool changed() const;
    void setChanged(bool changed);

private:
    bool readXmlFile(const QString & filename);
    QString readHtml( QXmlStreamReader &xml );

    QString mName;
    QString mDescription;
    QString mConclusionHeader;
    QString mConclusionMessage;

    QString mTextEntryStyle;
    QString mPromptStyle;

    QString mHeaderStyle;
    QString mDescriptionStyle;

    bool mValid;

    Qt::LayoutDirection mLayoutDirection;

    Keyboard mKeyboard;

    QList<Section*> mSections;

    QColor mRemainderFg, mRemainderBg;
    QColor mErrorFg, mErrorBg;

    bool mChanged;
};

#endif // COURSE_H
