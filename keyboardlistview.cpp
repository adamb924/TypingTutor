#include "keyboardlistview.h"

#include <QMenu>
#include <QContextMenuEvent>

KeyboardListView::KeyboardListView(QWidget * parent) : QListView(parent)
{
}

void KeyboardListView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu * menu = new QMenu(this);
    menu->addAction(tr("Insert letter"), this, SLOT(insert()));
    QModelIndex index = indexAt(event->pos());
    if( index.isValid() )
    {
        menu->addAction(tr("Remove letter"), this, SLOT(remove()));
    }
    menu->exec(event->globalPos());
}

void KeyboardListView::insert()
{
    QModelIndex index = currentIndex();
    if( index.isValid() )
    {
        model()->insertRow( index.row() );
    }
    else
    {
        model()->insertRow( 0 );
    }
}

void KeyboardListView::remove()
{
    QModelIndex index = currentIndex();
    if( index.isValid() )
    {
        model()->removeRow( index.row() );
    }
}
