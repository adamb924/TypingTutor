#ifndef COURSEEDITORWINDOW_H
#define COURSEEDITORWINDOW_H

#include <QMainWindow>

class QStandardItemModel;
class QDataWidgetMapper;

namespace Ui {
class CourseEditorWindow;
}

class CourseEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    CourseEditorWindow(QStandardItemModel * model, QWidget *parent = 0);
    ~CourseEditorWindow();

private slots:
    void itemClicked(const QModelIndex & index);

private:
    Ui::CourseEditorWindow *ui;
    QStandardItemModel * mModel;

    QDataWidgetMapper *mPromptMapper;
    QDataWidgetMapper *mDescriptionMapper;
};

#endif // COURSEEDITORWINDOW_H
