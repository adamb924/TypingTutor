#include "descriptionform.h"
#include "ui_descriptionform.h"

#include <QtDebug>

DescriptionForm::DescriptionForm(const QString &name, const QString &description, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DescriptionForm)
{
    ui->setupUi(this);
    setNameDescription(name, description);

    ui->nextButton->setFocus();

    connect( ui->nextButton, SIGNAL(clicked()), this, SIGNAL(next()) );
    connect( ui->previousButton, SIGNAL(clicked()), this, SIGNAL(previous()) );
}

DescriptionForm::~DescriptionForm()
{
    delete ui;
}

void DescriptionForm::setNameDescription(const QString &name, const QString &description)
{
    ui->name->setText(name);
    ui->description->setText(description);
}

void DescriptionForm::setHasPreviousNext(bool hasPrevious, bool hasNext)
{
    ui->previousButton->setVisible(hasPrevious);
    ui->nextButton->setVisible(hasNext);
}

void DescriptionForm::setHeaderStyle(const QString &style)
{
    ui->name->setStyleSheet(style);
}

void DescriptionForm::setDescriptionStyle(const QString &style)
{
    ui->description->setStyleSheet(style);
}
