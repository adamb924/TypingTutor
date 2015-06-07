#include "keyboardeditorwindow.h"
#include "ui_keyboardeditorwindow.h"

#include "keyboard.h"
#include "keyboardmodel.h"

#include <QDataWidgetMapper>

KeyboardEditorWindow::KeyboardEditorWindow(Keyboard *keyboard, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KeyboardEditorWindow)
{
    ui->setupUi(this);

    KeyboardModel * model = new KeyboardModel(keyboard);

    ui->listView->setModel( model );
    ui->listView->setModelColumn(0);

    QDataWidgetMapper * mapper = new QDataWidgetMapper;
    mapper->setModel(model);
    mapper->addMapping( ui->unicodeEdit, 0);
    mapper->addMapping( ui->hintEdit , 1);
    mapper->addMapping( ui->promptEdit , 2);

    connect( ui->listView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)) );
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
