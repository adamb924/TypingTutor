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
        if( row == 0 || row == mCourse->sections()->count() +1 ) // intro/conclusion
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
        int section = mCourse->getSectionIndex(p);
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
        else if ( index.row() == mCourse->sections()->count() + 1 ) // conclusion
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
        Prompt *p = qobject_cast<Prompt*>(o);
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

    return Qt::ItemIsSelectable	| Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
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

bool CourseModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    if( !index.isValid() )
        return false;

    QVector<int> rolesVector;
    rolesVector.append( Qt::DisplayRole );
    rolesVector.append( Qt::EditRole );

    if( !index.parent().isValid() )
    {
        if( index.row() == 0 ) // intro
        {
            if( index.column() == 0 )
                mCourse->setName( value.toString() );
            else
                mCourse->setDescription( value.toString() );
            emit dataChanged(index, index, rolesVector);
            return true;
        }
        else if ( index.row() == mCourse->sections()->count() - 1 ) // conclusion
        {
            if( index.column() == 0 )
                mCourse->setConclusionHeader( value.toString() );
            else
                mCourse->setConclusionMessage( value.toString() );
            emit dataChanged(index, index, rolesVector);
            return true;
        }
    }

    QObject * o = static_cast<QObject*>(index.internalPointer());
    Section *s = qobject_cast<Section*>(o);
    if( s != 0 )
    {
        if( index.column() == 0 )
            s->setName(value.toString());
        else
            s->setDescription(value.toString());
        emit dataChanged(index, index, rolesVector);
        return true;
    }
    else
    {
        Prompt *p = qobject_cast<Prompt*>(o);
        if( p != 0 )
        {
            if( index.column() == 0 )
                p->setTargetText(value.toString());
            else
                p->setDescription(value.toString());
        }
        emit dataChanged(index, index, rolesVector);
        return true;
    }

    return false;
}

Qt::DropActions CourseModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions CourseModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

bool CourseModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if( parent.isValid() )
    {
        beginRemoveRows(parent, row, row+count-1);
        QObject * o = static_cast<QObject*>(parent.internalPointer());
        Section *s = qobject_cast<Section*>(o);
        if( s != 0 )
        {
            for(int i=0; i<count; i++)
            {
                s->removePromptAt(row);
            }
        }
        endRemoveRows();
        return true;
    }
    else
    {
        beginRemoveRows(parent, row, row+count-1);
        for(int i=0; i<count; i++)
        {
            mCourse->removeSectionAt( row - 1 );
        }
        endRemoveRows();
        return false;
    }
}

bool CourseModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(count);
    if( parent.isValid() )
    {
        QObject * o = static_cast<QObject*>(parent.internalPointer());
        Section *s = qobject_cast<Section*>(o);
        if( s != 0 )
        {
            beginInsertRows(parent, row, row);
            s->addPromptAtBeginning( new Prompt(tr("New Prompt"), tr("")) );
            endInsertRows();
        }
        else
        {
            Prompt *p = qobject_cast<Prompt*>(o);
            if( p != 0 )
            {
                int sectionIndex = mCourse->getSectionIndex( p );
                if( sectionIndex != -1 )
                {
                    Section * section = mCourse->sections()->at(sectionIndex);
                    beginInsertRows(parent, row, row);
                    section->addPromptAt( new Prompt(tr("New Prompt"), tr("")) , p );
                    endInsertRows();
                }
            }
        }
    }
    else
    {
        beginInsertRows(parent, row, row);
        /// minus one to account for the header
        mCourse->insertSectionAt( new Section(tr("New section"),tr(""),tr(""),tr("")) , row - 1 );
        endInsertRows();
    }

    return true;
}

bool CourseModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    Q_UNUSED(count);

    QModelIndex source = sourceParent.isValid() ? sourceParent.child(sourceRow, 0) : index(sourceRow, 0);
    QModelIndex destination = destinationParent.isValid() ? destinationParent.child(destinationChild, 0) : index(destinationChild, 0);
    if( source.isValid() && destination.isValid() )
    {
        QObject * oSource = static_cast<QObject*>(source.internalPointer());
        Section *sSource = qobject_cast<Section*>(oSource);
        Prompt *pSource = qobject_cast<Prompt*>(oSource);

        QObject * oDestination = static_cast<QObject*>(destination.internalPointer());
        Section *sDestination = qobject_cast<Section*>(oDestination);
        Prompt *pDestination = qobject_cast<Prompt*>(oDestination);

        if( pSource && pDestination )
        {
            Section * sourceSection = mCourse->getSection(pSource);
            Section * destinationSection = mCourse->getSection(pDestination);
            if( sourceSection && destinationSection )
            {
                beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild);
                sourceSection->prompts()->removeAt( sourceSection->prompts()->indexOf(pSource) );
                destinationSection->addPromptAt( pSource, pDestination );
                endMoveRows();
            }
        }
        else if ( pSource && sDestination )
        {
            Section * sourceSection = mCourse->getSection(pSource);
            if( sourceSection )
            {
                beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild);
                sourceSection->prompts()->removeAt( sourceSection->prompts()->indexOf(pSource) );
                sDestination->addPrompt( pSource );
                endMoveRows();
            }
        }
        else if ( sSource && sDestination )
        {
            int source = mCourse->sections()->indexOf( sSource );
            beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild);
            mCourse->sections()->removeAt( source );
            int destination = mCourse->sections()->indexOf( sDestination );
            mCourse->insertSectionAt( sSource , destination );
            endMoveRows();
        }
    }

    return true;
}
