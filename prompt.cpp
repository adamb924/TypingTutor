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
