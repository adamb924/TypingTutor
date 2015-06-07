#ifndef KEYBOARDEDITORWINDOW_H
#define KEYBOARDEDITORWINDOW_H

#include <QMainWindow>

class Keyboard;

namespace Ui {
class KeyboardEditorWindow;
}

class KeyboardEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    KeyboardEditorWindow(Keyboard * keyboard, QWidget *parent = 0);
    ~KeyboardEditorWindow();

    void setStyles(const QString & unicode, const QString & prompt);

private slots:
    void copyUnicode();

private:
    Ui::KeyboardEditorWindow *ui;
};

#endif // KEYBOARDEDITORWINDOW_H
