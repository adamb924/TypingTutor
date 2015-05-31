#ifndef SECTION_H
#define SECTION_H

#include <QString>
#include <QList>

#include "prompt.h"

class Section
{
public:
    Section(const QString & name, const QString & description, const QString & conclusionHeader, const QString & conclusionMessage);

    QString name() const;
    QString description() const;

    void addPrompt(const Prompt & p);

    QList<Prompt>* prompts();

    QString conclusionHeader() const;
    QString conclusionMessage() const;

private:
    QString mName;
    QString mDescription;

    QString mConclusionHeader;
    QString mConclusionMessage;

    QList<Prompt> mPrompts;
};

#endif // SECTION_H
