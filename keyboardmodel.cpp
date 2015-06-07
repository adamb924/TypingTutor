#include "keyboardmodel.h"

#include <QtDebug>

#include "keyboard.h"
#include "letter.h"

KeyboardModel::KeyboardModel(Keyboard *keyboard, QObject *parent) :
    QAbstractTableModel(parent),
    mKeyboard(keyboard)
{
}

int KeyboardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mKeyboard->letters()->count();
}

int KeyboardModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant KeyboardModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole )
        return QVariant();

    Q_ASSERT(index.row() < mKeyboard->letters()->count() );
    switch( index.column() )
    {
    case 0:
        return mKeyboard->letters()->at(index.row())->unicode();
        break;
    case 1:
        return mKeyboard->letters()->at(index.row())->hint();
        break;
    case 2:
        return mKeyboard->letters()->at(index.row())->prompt();
        break;
    default:
        break;
    }
    return QVariant();
}

bool KeyboardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() )
        return false;

    if( role == Qt::EditRole )
    {
        Q_ASSERT(index.row() < mKeyboard->letters()->count() );
        switch( index.column() )
        {
        case 0:
            mKeyboard->letters()->at(index.row())->setUnicode( value.toString() );
            break;
        case 1:
            mKeyboard->letters()->at(index.row())->setHint( value.toString() );
            break;
        case 2:
            mKeyboard->letters()->at(index.row())->setPrompt( value.toString() );
            break;
        default:
            break;
        }
        return true;
    }
    else
    {
        return false;
    }
}

Qt::ItemFlags KeyboardModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

bool KeyboardModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(count);
    beginInsertRows( parent, row, row );
    mKeyboard->letters()->insert(row, new Letter());
    endInsertRows();
    return true;
}

bool KeyboardModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(count);
    beginRemoveRows( parent, row, row );
    mKeyboard->letters()->removeAt(row);
    endRemoveRows();
    return true;
}
