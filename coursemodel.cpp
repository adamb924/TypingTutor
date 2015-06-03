#include "coursemodel.h"

#include "course.h"
#include "section.h"
#include "prompt.h"

#include <QtDebug>

CourseModel::CourseModel(Course *course, QObject *parent) :
    QAbstractItemModel(parent),
    mCourse(course)
{
}

QModelIndex CourseModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    void * ptr;
    if( parent.isValid() ) // prompt
    {
        Q_ASSERT( (parent.row() - 1) < mCourse->sections()->count() );
        Q_ASSERT( row < mCourse->sections()->at( parent.row() - 1 )->prompts()->count() );
        ptr = mCourse->sections()->at( parent.row() -1 )->prompts()->at( row );
    }
    else // first-level element
    {
        if( row == 0 || row == rowCount() - 1 ) // intro/conclusion
        {
            ptr = 0;
        }
        else  // section
        {
            Q_ASSERT( (row-1) < mCourse->sections()->count() );
            ptr = mCourse->sections()->at( row - 1 );
        }
    }
    return createIndex(row, column, ptr );
}

QModelIndex CourseModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QObject * o = static_cast<QObject*>(index.internalPointer());
    Prompt *p = qobject_cast<Prompt*>(o);
    if( p != 0 )
    {
        int section = mCourse->whichSection(p);
        if( section != -1 )
        {
            Q_ASSERT( section < mCourse->sections()->count() );
            return createIndex( section + 1, 0, mCourse->sections()->at(section) );
        }
    }

    return QModelIndex();
}

QVariant CourseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole )
        return QVariant();

    if( !index.parent().isValid() )
    {
        if( index.row() == 0 ) // intro
        {
            if( index.column() == 0 )
                return mCourse->name();
            else
                return mCourse->description();
        }
        else if ( index.row() == rowCount() - 1 ) // conclusion
        {
            if( index.column() == 0 )
                return mCourse->conclusionHeader();
            else
                return mCourse->conclusionMessage();
        }
    }

    QObject * o = static_cast<QObject*>(index.internalPointer());
    Section *s = qobject_cast<Section*>(o);
    if( s != 0 )
    {
        if( index.column() == 0 )
            return s->name();
        else
            return s->description();
    }
    else
    {
        Prompt *p = static_cast<Prompt*>(o);
        if( p != 0 )
        {
            if( index.column() == 0 )
                return p->targetText();
            else
                return p->description();
        }
    }
    return QVariant();
}

Qt::ItemFlags CourseModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    // TODO consider this
    return QAbstractItemModel::flags(index);
}

QVariant CourseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED( section )
    Q_UNUSED( orientation )
    Q_UNUSED( role )
    return QVariant();
}

int CourseModel::rowCount(const QModelIndex &parent) const
{
    if( parent.isValid() ) // either section or prompt
    {
        QObject * o = static_cast<QObject*>(parent.internalPointer());
        Section *s = qobject_cast<Section*>(o);
        if( s != 0 ) // section
        {
            return s->prompts()->count();
        }
        else // prompt
        {
            return 0;
        }
    }
    else
    {
        return mCourse->sections()->count() + 2;
    }
}

int CourseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent )
    /// this always happens to be 2
    return 2;
}
