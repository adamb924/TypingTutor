#include "promptform.h"
#include "ui_promptform.h"

#include "targethighlighter.h"
#include "keyboard.h"

#include <QKeyEvent>
#include <QtDebug>

PromptForm::PromptForm(const Prompt &p, const Keyboard *kbd, const QString &courseName, const QString &sectionName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PromptForm),
    mPrompt(p),
    mKeyboard(kbd)
{
    ui->setupUi(this);

    ui->inputEdit->setFocusPolicy(Qt::StrongFocus);
    ui->inputEdit->setFocus(Qt::OtherFocusReason);

    ui->description->setText(mPrompt.description());

    mTargetHighlighter = new TargetHighlighter( mPrompt.targetText(), ui->inputEdit->document() );

    updateTargetTextDisplay();

    ui->sectionName->setText(sectionName);
    ui->courseName->setText(courseName);

    connect( ui->inputEdit, SIGNAL(textChanged()), this, SLOT(updateTargetTextDisplay()) );
    connect( ui->nextButton, SIGNAL(clicked()), this, SIGNAL(next()) );
    connect( ui->skipButton, SIGNAL(clicked()), this, SIGNAL(next()) );
    connect( ui->previousButton, SIGNAL(clicked()), this, SIGNAL(previous()) );
    connect( this, SIGNAL(textEnteredCorrectly()), this, SLOT(allowUserToAdvance()) );
    connect( ui->inputEdit, SIGNAL(focusIn()), this, SLOT(updateTargetTextDisplay()));
}

PromptForm::~PromptForm()
{
    delete ui;
}

void PromptForm::setHeaderStyle(const QString &style)
{
    ui->courseName->setStyleSheet(style);
    ui->sectionName->setStyleSheet(style);
}

void PromptForm::setDescriptionStyle(const QString &style)
{
    ui->description->setStyleSheet(style);
}

void PromptForm::setTextEditStyle(const QString &style)
{
    ui->inputEdit->setStyleSheet(style);
    ui->targetEdit->setStyleSheet(style);
}

void PromptForm::clearInput()
{
    ui->inputEdit->clear();
    ui->inputEdit->setFocus(Qt::OtherFocusReason);
}

void PromptForm::updateTargetTextDisplay()
{
    ui->targetEdit->setText( mPrompt.targetText() );
    QString enteredText = ui->inputEdit->toPlainText();
    int matchLength = PromptForm::getFirstNonmatching(enteredText, mPrompt.targetText());

    if( matchLength == -1 )
    {
        emit textEnteredCorrectly();
        emit showHint("");
        return;
    }

    QString remainder = mPrompt.targetText().mid(matchLength);
    emit changePrompt( mKeyboard->nextThingToType( remainder ) );
    emit showHint( enteredText.length() > matchLength ? mKeyboard->nextHint(remainder) : "" );

    if( enteredText.length() < mPrompt.targetText().length() )
    {
        QString targetHtml = mPrompt.targetText().left( matchLength );
        if( matchLength > 0 )
        {
            QString remainderString = mPrompt.targetText().mid(matchLength);
            if( targetHtml.at(targetHtml.length()-1).isLetterOrNumber() && remainderString.at(0).isLetterOrNumber() )
            {
                targetHtml += QChar(0x200D);
            }
            targetHtml += "<font style='color: blue'>‍";
            if( remainderString.at(0).isLetterOrNumber() )
            {
                targetHtml += QChar(0x200D);
            }
            targetHtml += remainderString;
            targetHtml += "</font>";

        }
        else
        {
            targetHtml = mPrompt.targetText();
        }
        ui->targetEdit->setText(targetHtml);
    }
}

void PromptForm::allowUserToAdvance()
{
    ui->nextButton->setEnabled(true);
    ui->nextButton->setFocus(Qt::OtherFocusReason);
}

int PromptForm::getFirstNonmatching(const QString &one, const QString &two)
{
    if( one.length() == 0 || two.length() == 0 )
    {
        return 0;
    }
    if( one == two )
    {
        return -1;
    }
    QVector<uint> first = one.toUcs4();
    QVector<uint> second = two.toUcs4();
    for(int i=0; i < qMin(first.size(), second.size()); i++ )
    {
        if( first.at(i) != second.at(i) )
        {
            return i;
        }
    }
    if( first.size() != second.size() )
    {
        return qMin(first.size(), second.size());
    }
    return -1;
}
