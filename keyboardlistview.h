#ifndef KEYBOARDLISTVIEW_H
#define KEYBOARDLISTVIEW_H

#include <QListView>

class KeyboardListView : public QListView
{
    Q_OBJECT
public:
    explicit KeyboardListView(QWidget * parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    void insert();
    void remove();
};

#endif // KEYBOARDLISTVIEW_H
