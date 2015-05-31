#ifndef SHORTCUTTEXTEDIT_H
#define SHORTCUTTEXTEDIT_H

#include <QTextEdit>

class ShortcutTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit ShortcutTextEdit(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent * e);
    void focusInEvent(QFocusEvent * e);

signals:
    void focusIn();

public slots:

};

#endif // SHORTCUTTEXTEDIT_H
