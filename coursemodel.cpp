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
        ptr = mCourse->sections()->at( parent.row() )->prompts()->at( row );
    else // section
        ptr = mCourse->sections()->at( row );
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
            return createIndex( section, 0, mCourse->sections()->at(section) );
    }

    return QModelIndex();
}

QVariant CourseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole )
        return QVariant();

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
        if( s != 0 )
        {
            return s->prompts()->count();
        }
        else // must be a prompt
        {
            return 0;
        }
    }
    else
    {
        return mCourse->sections()->count();
    }
}

int CourseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent )
    /// this always happens to be 2
    return 2;
}
