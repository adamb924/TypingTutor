#include "section.h"

Section::Section(const QString &name, const QString &description, const QString &conclusionHeader, const QString &conclusionMessage) :
    mName(name), mDescription(description), mConclusionHeader(conclusionHeader), mConclusionMessage(conclusionMessage)
{
}

QString Section::name() const
{
    return mName;
}

QString Section::description() const
{
    return mDescription;
}

void Section::setName(const QString &str)
{
    mName = str;
}

void Section::setDescription(const QString &str)
{
    mDescription = str;
}

void Section::addPrompt(Prompt * p)
{
    mPrompts.append(p);
}

QList<Prompt *> *Section::prompts()
{
    return &mPrompts;
}

QString Section::conclusionHeader() const
{
    return mConclusionHeader;
}

QString Section::conclusionMessage() const
{
    return mConclusionMessage;
}
