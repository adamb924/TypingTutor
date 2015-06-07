#include "keyboardeditorwindow.h"
#include "ui_keyboardeditorwindow.h"

#include "course.h"
#include "keyboard.h"
#include "keyboardmodel.h"

#include <QDataWidgetMapper>

KeyboardEditorWindow::KeyboardEditorWindow(Course *course, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KeyboardEditorWindow),
    mKeyboard(course->keyboard())
{
    ui->setupUi(this);

    KeyboardModel * model = new KeyboardModel(mKeyboard);

    ui->keyboardNameEdit->setText(mKeyboard->name());

    ui->listView->setModel( model );
    ui->listView->setModelColumn(0);

    ui->unicodeEdit->setLayoutDirection( course->textDirection() );
    ui->promptEdit->setLayoutDirection( course->textDirection() );

    QDataWidgetMapper * mapper = new QDataWidgetMapper;
    mapper->setModel(model);
    mapper->addMapping( ui->unicodeEdit, 0);
    mapper->addMapping( ui->hintEdit , 1);
    mapper->addMapping( ui->promptEdit , 2);

    connect( ui->listView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)) );
    connect( ui->keyboardNameEdit, SIGNAL(textChanged(QString)), this, SLOT(setKeyboardName(QString)) );
    connect( ui->copyUnicodePrompt, SIGNAL(clicked()), this, SLOT(copyUnicode()) );
}

KeyboardEditorWindow::~KeyboardEditorWindow()
{
    delete ui;
}

void KeyboardEditorWindow::setStyles(const QString &unicode, const QString &prompt)
{
    ui->unicodeEdit->setStyleSheet(unicode);
    ui->promptEdit->setStyleSheet(prompt);
}

void KeyboardEditorWindow::copyUnicode()
{
    ui->promptEdit->setText( ui->unicodeEdit->text() );
}

void KeyboardEditorWindow::setKeyboardName(const QString &str)
{
    mKeyboard->setName(str);
}
