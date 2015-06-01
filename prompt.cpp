#include "prompt.h"

Prompt::Prompt(const QString &targetText, const QString &description) :
    mTargetText(targetText), mDescription(description)
{
}

QString Prompt::targetText() const
{
    return mTargetText;
}

QString Prompt::description() const
{
    return mDescription;
}

void Prompt::setTargetText(const QString &str)
{
    mTargetText = str;
}

void Prompt::setDescription(const QString &str)
{
    mDescription = str;
}
