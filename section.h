#ifndef SECTION_H
#define SECTION_H

#include <QObject>
#include <QString>
#include <QList>

#include "prompt.h"

class Section : public QObject
{
    Q_OBJECT
public:
    Section(const QString & name, const QString & description, const QString & conclusionHeader, const QString & conclusionMessage);

    QString name() const;
    QString description() const;

    void setName(const QString & str);
    void setDescription(const QString & str);

    void addPrompt(Prompt * p);

    QList<Prompt*>* prompts();

    QString conclusionHeader() const;
    QString conclusionMessage() const;

private:
    QString mName;
    QString mDescription;

    QString mConclusionHeader;
    QString mConclusionMessage;

    QList<Prompt*> mPrompts;
};

#endif // SECTION_H
