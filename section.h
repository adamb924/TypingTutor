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
    Section(const QString & name = QString(), const QString & description = QString());
    ~Section();

    QString name() const;
    QString description() const;

    void setName(const QString & str);
    void setDescription(const QString & str);

    void removePromptAt(int i);

    void addPrompt(Prompt * p);
    void addPromptAtBeginning(Prompt * p);
    void addPromptAt(Prompt * p, Prompt *before);

    QList<Prompt*>* prompts();

private:
    QString mName;
    QString mDescription;

    QList<Prompt*> mPrompts;
};

#endif // SECTION_H
