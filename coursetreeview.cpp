#include "coursetreeview.h"

#include "section.h"
#include "prompt.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QtDebug>

CourseTreeView::CourseTreeView(QWidget * parent) : QTreeView(parent)
{
}

void CourseTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu * menu = new QMenu(this);

    QModelIndex index = indexAt(event->pos());
    if( index.isValid() )
    {
        QObject * o = static_cast<QObject*>(index.internalPointer());
        Section *s = qobject_cast<Section*>(o);
        Prompt *p = static_cast<Prompt*>(o);
        if( s != 0 )
        {
            menu->addAction(tr("Insert section"), this, SLOT(insertSection()));
            menu->addAction(tr("Remove section"), this, SLOT(remove()));
            menu->addSeparator();
            menu->addAction(tr("Insert prompt"), this, SLOT(insertPrompt()));
        }
        else if ( p != 0 )
        {
            menu->addAction(tr("Insert prompt"), this, SLOT(insertPrompt()));
            menu->addAction(tr("Remove prompt"), this, SLOT(remove()));
        }
    }
    else
    {
        menu->addAction(tr("Insert section"), this, SLOT(insertSection()));
    }


    menu->exec(event->globalPos());
}

void CourseTreeView::dropEvent(QDropEvent *event)
{
    QModelIndex target = indexAt( event->pos() );
    if( target.isValid() && currentIndex().isValid() )
    {
        model()->moveRow( currentIndex().parent() , currentIndex().row(), target.parent(), target.row() );
//        expand( indexAt( event->pos() ) );
    }
}

void CourseTreeView::insertSection()
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

void CourseTreeView::insertPrompt()
{
    QModelIndex index = currentIndex();
    if( index.isValid() )
    {
        if( index.parent().isValid() )
        {
            model()->insertRow( index.row(), index.parent() );
        }
        else
        {
            model()->insertRow( 0 , index );
        }
//        expand(index);
    }
}

void CourseTreeView::remove()
{
    QModelIndex index = currentIndex();
    if( index.isValid() )
    {
        model()->removeRow( index.row(), index.parent() );
    }
}

void CourseTreeView::selectRow(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(end);
    selectionModel()->clear();
    selectionModel()->select( model()->index(start, 0, parent) , QItemSelectionModel::Select );
}
