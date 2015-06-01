#include "promptform.h"
#include "ui_promptform.h"

#include "targethighlighter.h"
#include "keyboard.h"

#include <QKeyEvent>
#include <QDataWidgetMapper>
#include <QtDebug>

PromptForm::PromptForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PromptForm),
    mKeyboard(0),
    mMapper(0)
{
    ui->setupUi(this);

    ui->inputEdit->setFocusPolicy(Qt::StrongFocus);
    ui->inputEdit->setFocus(Qt::OtherFocusReason);
    mTargetHighlighter = new TargetHighlighter(ui->inputEdit->document() );

    updateTargetTextDisplay();
    connect( ui->inputEdit, SIGNAL(textChanged()), this, SLOT(updateTargetTextDisplay()) );
    connect( ui->inputEdit, SIGNAL(focusIn()), this, SLOT(updateTargetTextDisplay()));
    connect( this, SIGNAL(targetTextChanged()), this, SLOT(updateTargetText()));
}

PromptForm::~PromptForm()
{
    delete ui;
}

void PromptForm::setKeyboard(const Keyboard *kbd)
{
    mKeyboard = kbd;
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

void PromptForm::setModel(QAbstractItemModel *model)
{
    mMapper = new QDataWidgetMapper;
    mMapper->setModel(model);
    mMapper->addMapping( this, 0, "targetText" );
    mMapper->addMapping( ui->targetEdit, 0, "plainText" );
    mMapper->addMapping( ui->description, 1, "text" );
}

void PromptForm::setIndex(const QModelIndex &index, const QModelIndex &root)
{
    if( mMapper == 0 )
        return;
    mMapper->setRootIndex(root);
    mMapper->setCurrentModelIndex(index);
}

void PromptForm::updateTargetText()
{
    mTargetHighlighter->setTargetText(mTargetText);
    updateTargetTextDisplay();
}

void PromptForm::updateTargetTextDisplay()
{
    if( mKeyboard == 0 )
    {
        return;
    }

    ui->targetEdit->setText( mTargetText );
    QString enteredText = ui->inputEdit->toPlainText();
    int matchLength = PromptForm::getFirstNonmatching(enteredText, mTargetText );

    if( matchLength == -1 )
    {
        emit textEnteredCorrectly();
        emit showHint("");
        return;
    }

    if( matchLength < mTargetText.length() )
    {
        QString remainder = mTargetText.mid(matchLength);
        emit showHint( mKeyboard->nextThingToType( remainder ) );
        if( enteredText.length() > matchLength )
        {
            emit inputPrompt( mKeyboard->nextHint(remainder) );
        }
        else
        {
            emit inputPrompt("");
        }
    }

    if( enteredText.length() < mTargetText.length() )
    {
        QString targetHtml = mTargetText.left( matchLength );
        if( matchLength > 0 )
        {
            QString remainderString = mTargetText.mid(matchLength);
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
            targetHtml = mTargetText;
        }
        ui->targetEdit->setText(targetHtml);
    }
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
