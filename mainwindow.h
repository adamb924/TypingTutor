#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHash>
#include <QModelIndex>

class Course;
class QStackedWidget;
class QStandardItem;
class Prompt;
class DescriptionForm;
class QDataWidgetMapper;
class CourseModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ModelDataType { Description = Qt::UserRole + 1 };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openCourse();
    void editCourse();
    void editKeyboard();
    void promptToMoveForward();

    void setupCourseLayout();
    void nextSlide();
    void previousSlide();

    void setHint(const QString & newHint);

    void itemClicked(const QModelIndex & index, const QModelIndex &previous = QModelIndex() );

private:
    void showOrHideDocksAndButtons(QModelIndex index = QModelIndex());
    bool isPrompt(const QModelIndex &index) const;
    QModelIndex selectedOrFirst() const;
    void selectIndex(const QModelIndex & index);

private:
    Ui::MainWindow *ui;
    CourseModel *mModel;
    Course * mCourse;
    QDataWidgetMapper *mDescriptionMapper;
};

#endif // MAINWINDOW_H
