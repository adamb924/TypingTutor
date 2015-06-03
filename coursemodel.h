#ifndef COURSEMODEL_H
#define COURSEMODEL_H

#include <QAbstractItemModel>

class Course;

class CourseModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CourseModel(Course * course, QObject *parent = 0);

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
signals:

public slots:

private:
    Course * mCourse;

};

#endif // COURSEMODEL_H
