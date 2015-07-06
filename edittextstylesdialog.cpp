#include "edittextstylesdialog.h"
#include "ui_edittextstylesdialog.h"

#include <QFontDialog>
#include <QtDebug>
#include <QColorDialog>

#include "course.h"

EditTextStylesDialog::EditTextStylesDialog(Course *course, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTextStylesDialog),
    mCourse(course)
{
    ui->setupUi(this);

    ui->promptStyleEdit->setText( mCourse->promptStyle() );
    ui->headerStyleEdit->setText( mCourse->headerStyle() );
    ui->descriptionStyleEdit->setText( mCourse->descriptionStyle() );
    ui->paneStyleEdit->setText( mCourse->navigationPaneStyle() );
    ui->textEntryStyleEdit->setText( mCourse->textEntryStyle() );
    ui->textDirectionCombo->setCurrentIndex( (int)mCourse->textDirection() );

    connect(ui->promptStyleButton, SIGNAL(clicked()), this, SLOT(choosePromptStyle()) );
    connect(ui->headerStyleButton, SIGNAL(clicked()), this, SLOT(chooseHeaderStyle()) );
    connect(ui->descriptionStyleButton, SIGNAL(clicked()), this, SLOT(chooseDescriptionStyle()) );
    connect(ui->textEntryStyleButton, SIGNAL(clicked()), this, SLOT(chooseTextEntryStyle()) );
    connect(ui->paneStyleButton, SIGNAL(clicked(bool)), this, SLOT(choosePaneStyle()) );

    mRemainderFg = mCourse->remainderFg();
    mRemainderBg = mCourse->remainderBg();
    mErrorFg = mCourse->errorFg();
    mErrorBg = mCourse->errorBg();

    connect(ui->errorBGButton, SIGNAL(clicked()), this, SLOT(errorBg()));
    connect(ui->errorFGButton, SIGNAL(clicked()), this, SLOT(errorFg()));
    connect(ui->remainderBGButton, SIGNAL(clicked()), this, SLOT(remainderBg()));
    connect(ui->remainderFGButton, SIGNAL(clicked()), this, SLOT(remainderFg()));

    setLabelStyles();

    connect(this, SIGNAL(accepted()), this, SLOT(saveValues()) );
}

EditTextStylesDialog::~EditTextStylesDialog()
{
    delete ui;
}

void EditTextStylesDialog::choosePromptStyle()
{
    QString style = styleFromFontDialog( ui->promptStyleEdit->text() );
    if( !style.isEmpty() )
    {
        ui->promptStyleEdit->setText( style );
    }
}

void EditTextStylesDialog::chooseTextEntryStyle()
{
    QString style = styleFromFontDialog( ui->textEntryStyleEdit->text() );
    if( !style.isEmpty() )
    {
        ui->textEntryStyleEdit->setText( style );
    }
}

void EditTextStylesDialog::chooseDescriptionStyle()
{
    QString style = styleFromFontDialog( ui->descriptionStyleEdit->text() );
    if( !style.isEmpty() )
    {
        ui->descriptionStyleEdit->setText( style );
    }
}

void EditTextStylesDialog::chooseHeaderStyle()
{
    QString style = styleFromFontDialog( ui->headerStyleEdit->text() );
    if( !style.isEmpty() )
    {
        ui->headerStyleEdit->setText( style );
    }
}

void EditTextStylesDialog::choosePaneStyle()
{
    QString style = styleFromFontDialog( ui->paneStyleEdit->text() );
    if( !style.isEmpty() )
    {
        ui->paneStyleEdit->setText( style );
    }
}

void EditTextStylesDialog::saveValues()
{
    mCourse->setPromptStyle( ui->promptStyleEdit->text() );
    mCourse->setTextDirection( (Qt::LayoutDirection)ui->textDirectionCombo->currentIndex() );
    mCourse->setHeaderStyle( ui->headerStyleEdit->text() );
    mCourse->setDescriptionStyle( ui->descriptionStyleEdit->text() );
    mCourse->setNavigationPaneStyle( ui->paneStyleEdit->text() );
    mCourse->setRemainderFg(mRemainderFg);
    mCourse->setRemainderBg(mRemainderBg);
    mCourse->setErrorFg(mErrorFg);
    mCourse->setErrorBg(mErrorBg);
}

void EditTextStylesDialog::errorFg()
{
    QColorDialog dlg( mErrorFg, this);
    if( dlg.exec() )
    {
        mErrorFg = dlg.currentColor();
        setLabelStyles();
    }
}

void EditTextStylesDialog::errorBg()
{
    QColorDialog dlg( mErrorBg, this);
    if( dlg.exec() )
    {
        mErrorBg = dlg.currentColor();
        setLabelStyles();
    }
}

void EditTextStylesDialog::remainderFg()
{
    QColorDialog dlg( mRemainderFg, this);
    if( dlg.exec() )
    {
        mRemainderFg = dlg.currentColor();
        setLabelStyles();
    }
}

void EditTextStylesDialog::remainderBg()
{
    QColorDialog dlg( mRemainderBg, this);
    if( dlg.exec() )
    {
        mRemainderBg = dlg.currentColor();
        setLabelStyles();
    }
}

QString EditTextStylesDialog::styleFromFontDialog(const QString & initial) const
{
    bool ok;
    QString defaultFamily = "Times New Roman";
    int defaultPointSize = 12;

    QRegExp rx("font-family: ([^;,]*).*;");
    rx.indexIn(initial);
    QStringList list = rx.capturedTexts();
    if( list.count() > 1 )
        defaultFamily = list.at(1);

    QRegExp rx2("font-size: ([1234567890]+)pt;");
    rx2.indexIn(initial);
    list = rx2.capturedTexts();
    if( list.count() > 1 )
        defaultPointSize = list.at(1).toInt();

    QFont font = QFontDialog::getFont(
                    &ok, QFont(defaultFamily, defaultPointSize) );
    if (ok)
    {
        return tr("font-family: %1; font-size: %2pt;").arg(font.family()).arg(font.pointSize());
    }
    else {
        return "";
    }
}

void EditTextStylesDialog::setLabelStyles()
{
    ui->remainderLabel->setStyleSheet("color: " + mRemainderFg.name() + "; background-color: "+ mRemainderBg.name() +";");
    ui->errorLabel->setStyleSheet("color: " + mErrorFg.name() + "; background-color: "+ mErrorBg.name() +";");
}
