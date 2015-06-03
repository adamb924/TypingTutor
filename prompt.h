#ifndef PROMPT_H
#define PROMPT_H

#include <QObject>
#include <QString>

class Prompt : public QObject
{
    Q_OBJECT
public:
    Prompt(const QString & targetText, const QString & description);

    QString targetText() const;
    QString description() const;

    void setTargetText(const QString & str);
    void setDescription(const QString & str);

private:
    QString mTargetText;
    QString mDescription;
};

#endif // PROMPT_H
