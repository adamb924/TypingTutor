#ifndef TEXTINPUTFORM_H
#define TEXTINPUTFORM_H

#include <QWidget>

#include "prompt.h"

namespace Ui {
class PromptForm;
}

class TargetHighlighter;
class Keyboard;
class QAbstractItemModel;
class QDataWidgetMapper;

class PromptForm : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString targetText MEMBER mTargetText NOTIFY targetTextChanged)

public:
    PromptForm(QWidget *parent = 0);
    ~PromptForm();

    void setKeyboard(const Keyboard * kbd);
    void setDescriptionStyle(const QString & style);
    void setTextEditStyle(const QString & style);
    void clearInput();
    void setModel(QAbstractItemModel * model);
    void setIndex(const QModelIndex & index, const QModelIndex & root);

signals:
    void textEnteredCorrectly();
    void inputPrompt(const QString & newPrompt);
    void showHint(const QString & hint);
    void targetTextChanged();

private slots:
    void updateTargetText();
    void updateTargetTextDisplay();

public:
    static int getFirstNonmatching(const QString & one, const QString & two);

private:
    Ui::PromptForm *ui;
    QString mTargetText;
    QString mEnteredText;
    TargetHighlighter * mTargetHighlighter;
    const Keyboard * mKeyboard;
    QDataWidgetMapper * mMapper;
};

#endif // TEXTINPUTFORM_H
