#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>

class Course;
class QStackedWidget;
class QStandardItemModel;
class QModelIndex;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openCourse();
    void setupCourseLayout();
    void nextSlide();
    void previousSlide();
    void showSlide(int index);
    void setPrompt(const QString & newPrompt);
    void focusWidget(const QModelIndex & index);

private:
    void showOrHideDocks();

private:
    Ui::MainWindow *ui;
    QStackedWidget *mStackedWidget;
    QStandardItemModel *mCourseModel;
    Course * mCourse;
    QHash<QModelIndex,QWidget*> mIndexWidgetHash;
};

#endif // MAINWINDOW_H
