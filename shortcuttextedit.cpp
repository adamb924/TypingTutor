#include "shortcuttextedit.h"

#include <QKeyEvent>
#include <QtDebug>
#include <QGuiApplication>

ShortcutTextEdit::ShortcutTextEdit(QWidget *parent) :
    QTextEdit(parent)
{

}

void ShortcutTextEdit::keyPressEvent(QKeyEvent *e)
{
    QTextEdit::keyPressEvent(e);
    if( e->key() == 0x200c )
    {
        insertPlainText( QChar(0x200C) );
    }
}

void ShortcutTextEdit::focusInEvent(QFocusEvent *e)
{
    QTextEdit::focusInEvent(e);
    emit focusIn();
}
