#ifndef COURSETREEVIEW_H
#define COURSETREEVIEW_H

#include <QTreeView>

class CourseTreeView : public QTreeView
{
    Q_OBJECT
public:
    CourseTreeView(QWidget * parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void dropEvent(QDropEvent * event);

public slots:
    void selectRow(const QModelIndex & parent, int start, int end);

private slots:
    void insertSection();
    void insertPrompt();
    void remove();
};

#endif // COURSETREEVIEW_H
