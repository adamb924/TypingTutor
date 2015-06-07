#include "section.h"

Section::Section(const QString &name, const QString &description) :
    mName(name), mDescription(description)
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

void Section::removePromptAt(int i)
{
    Q_ASSERT( i < mPrompts.count() );
    mPrompts.removeAt(i);
}

void Section::addPrompt(Prompt * p)
{
    mPrompts.append(p);
}

void Section::addPromptAtBeginning(Prompt *p)
{
    mPrompts.prepend(p);
}

void Section::addPromptAt(Prompt *p, Prompt *before)
{
    int index = mPrompts.indexOf(before);
    if( index != -1 )
    {
        mPrompts.insert(index, p);
    }
}

QList<Prompt *> *Section::prompts()
{
    return &mPrompts;
}
