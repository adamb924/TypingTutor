#ifndef TEXTINPUTFORM_H
#define TEXTINPUTFORM_H

#include <QWidget>

#include "prompt.h"

namespace Ui {
class PromptForm;
}

class TargetHighlighter;
class Keyboard;

class PromptForm : public QWidget
{
    Q_OBJECT

public:
    PromptForm(const Prompt & p, const Keyboard * kbd, const QString & courseName, const QString & sectionName, QWidget *parent = 0);
    ~PromptForm();

    void setHeaderStyle(const QString & style);
    void setDescriptionStyle(const QString & style);
    void setTextEditStyle(const QString & style);
    void clearInput();

signals:
    void next();
    void previous();
    void textEnteredCorrectly();
    void changePrompt(const QString & newPrompt);
    void showHint(const QString & hint);

private slots:
    void updateTargetTextDisplay();
    void allowUserToAdvance();

public:
    static int getFirstNonmatching(const QString & one, const QString & two);

private:
    Ui::PromptForm *ui;
    Prompt mPrompt;
    QString mEnteredText;
    TargetHighlighter * mTargetHighlighter;
    const Keyboard * mKeyboard;
};

#endif // TEXTINPUTFORM_H
