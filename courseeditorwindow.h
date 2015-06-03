#ifndef COURSEEDITORWINDOW_H
#define COURSEEDITORWINDOW_H

#include <QMainWindow>

class CourseModel;
class QDataWidgetMapper;
class Course;

namespace Ui {
class CourseEditorWindow;
}

class CourseEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    CourseEditorWindow(Course * course, CourseModel * model, QWidget *parent = 0);
    ~CourseEditorWindow();

private slots:
    void itemClicked(const QModelIndex & index);

private:
    Ui::CourseEditorWindow *ui;
    CourseModel * mModel;
    Course * mCourse;

    QDataWidgetMapper *mPromptMapper;
    QDataWidgetMapper *mDescriptionMapper;
};

#endif // COURSEEDITORWINDOW_H
