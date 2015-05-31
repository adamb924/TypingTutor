#ifndef PROMPT_H
#define PROMPT_H

#include <QString>

class Prompt
{
public:
    Prompt(const QString & targetText, const QString & description);

    QString targetText() const;
    QString description() const;

private:
    QString mTargetText;
    QString mDescription;
};

#endif // PROMPT_H
