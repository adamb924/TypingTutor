#include "courseeditorwindow.h"
#include "ui_courseeditorwindow.h"

#include "course.h"
#include "coursemodel.h"

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QtDebug>

CourseEditorWindow::CourseEditorWindow(Course * course, CourseModel * model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CourseEditorWindow),
    mModel(model),
    mCourse(course)
{
    ui->setupUi(this);
    ui->treeView->setModel(mModel);

    ui->treeView->hideColumn(1);

    connect( ui->treeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(itemClicked(QModelIndex)) );

    ui->descriptionHeadingEdit->setStyleSheet( course->headerStyle() );
    ui->descriptionIntroductionEdit->setStyleSheet( course->descriptionStyle() );
    ui->promptEdit->setStyleSheet( course->promptStyle() );
    ui->introductionEdit->setStyleSheet( course->descriptionStyle() );

    // description
    mDescriptionMapper = new QDataWidgetMapper;
    mDescriptionMapper->setModel(mModel);
    mDescriptionMapper->addMapping( ui->descriptionHeadingEdit, 0);
    mDescriptionMapper->addMapping( ui->descriptionIntroductionEdit , 1);

    // description
    mPromptMapper = new QDataWidgetMapper;
    mPromptMapper->setModel(mModel);
    mPromptMapper->addMapping( ui->promptEdit, 0);
    mPromptMapper->addMapping( ui->introductionEdit , 1);

    ui->stackedWidget->setCurrentIndex(1);
    mDescriptionMapper->toFirst();
}

CourseEditorWindow::~CourseEditorWindow()
{
    delete ui;
}

void CourseEditorWindow::itemClicked(const QModelIndex &index)
{
    QModelIndex parent = index.parent();
    if( parent.isValid() )
    {
        ui->stackedWidget->setCurrentIndex( 0 );
        mPromptMapper->setRootIndex(parent);
        mPromptMapper->setCurrentModelIndex(index);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex( 1 );
        mDescriptionMapper->setRootIndex(parent);
        mDescriptionMapper->setCurrentModelIndex(index);
    }
}
