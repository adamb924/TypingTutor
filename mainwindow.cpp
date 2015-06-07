#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "course.h"
#include "promptform.h"
#include "courseeditorwindow.h"
#include "coursemodel.h"
#include "keyboardeditorwindow.h"
#include "edittextstylesdialog.h"

#include <QStackedWidget>
#include <QLabel>
#include <QFileDialog>
#include <QtDebug>
#include <QStandardItemModel>
#include <QDataWidgetMapper>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mModel(0),
    mCourse(0)
{
    ui->setupUi(this);

    connect( ui->actionOpen_course, SIGNAL(triggered()), this, SLOT(openCourse()) );
    connect( ui->actionShow_hint, SIGNAL(toggled(bool)), ui->hintDock, SLOT(setVisible(bool)) );
    connect( ui->progressDock, SIGNAL(visibilityChanged(bool)), ui->actionShow_progress, SLOT(setChecked(bool)) );
    connect( ui->actionShow_progress, SIGNAL(toggled(bool)), ui->progressDock, SLOT(setVisible(bool)) );
    connect( ui->actionEdit_course, SIGNAL(triggered()), this, SLOT(editCourse()) );
    connect( ui->actionEdit_keyboard, SIGNAL(triggered()), this, SLOT(editKeyboard()) );
    connect( ui->nextButton, SIGNAL(clicked()), this, SLOT(nextSlide()) );
    connect( ui->previousButton, SIGNAL(clicked()), this, SLOT(previousSlide()) );
    connect( ui->skipButton, SIGNAL(clicked()), this, SLOT(nextSlide()) );
    connect( ui->promptPage, SIGNAL(showHint(QString)), this, SLOT(setHint(QString)));
    connect( ui->promptPage, SIGNAL(inputPrompt(QString)), statusBar(), SLOT(showMessage(QString)) );
    connect( ui->promptPage, SIGNAL(textEnteredCorrectly()), this, SLOT(promptToMoveForward()) );

    connect( ui->actionSave_course, SIGNAL(triggered()), this, SLOT(saveCourse()) );
    connect( ui->actionSave_course_as, SIGNAL(triggered()), this, SLOT(saveCourseAs()) );
    connect( ui->actionNew_course, SIGNAL(triggered()), this, SLOT(newCourse()) );

    connect( ui->actionEdit_text_styles, SIGNAL(triggered()), this, SLOT(editTextStyles()) );

    connect( ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    ui->progressDock->hide();
    ui->hintDock->hide();
    ui->previousButton->hide();
    ui->nextButton->hide();
    ui->skipButton->hide();
}

MainWindow::~MainWindow()
{
    if( mCourse != 0 )
    {
        delete mCourse;
    }
    delete ui;
}

void MainWindow::openCourse()
{
    mFilename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Typing Courses (*.xml)"));
    if( mFilename.isEmpty() )
    {
        return;
    }
    if( mCourse != 0 )
    {
        delete mCourse;
    }
    mCourse = new Course(mFilename);
    if( mCourse->isValid() )
    {
        setupCourseLayout();
    }
}

void MainWindow::saveCourse()
{
    if( mCourse != 0 )
    {
        if( mFilename.isEmpty() )
            saveCourseAs();
        else
            mCourse->writeXmlFile(mFilename);
    }
}

void MainWindow::saveCourseAs()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Course"),
                               "",
                               tr("XML Files (*.xml)"));
    if( !filename.isEmpty() )
    {
        mFilename = filename;
        saveCourse();
    }
}

void MainWindow::newCourse()
{
    if( mCourse != 0 )
        delete mCourse;
    mCourse = new Course;
    mCourse->setName(tr("New Course"));
    mCourse->setConclusionHeader(tr("Conclusion"));
    setupCourseLayout();
}

void MainWindow::setupCourseLayout()
{
    /// Create the tree model
    if( mModel != 0 )
    {
        delete mModel;
    }

    ui->promptPage->setKeyboard( mCourse->keyboard() );

    mModel = new CourseModel(mCourse);

    ui->actionEdit_course->setEnabled(true);
    ui->actionEdit_keyboard->setEnabled(true);
    ui->actionEdit_text_styles->setEnabled(true);

    ui->hintLabel->setText("");

    ui->treeView->setModel(mModel);
    ui->treeView->hideColumn(1);
    ui->treeView->setHeaderHidden(true);

    // description
    mDescriptionMapper = new QDataWidgetMapper;
    mDescriptionMapper->setModel(mModel);
    mDescriptionMapper->addMapping( ui->headerLabel, 0, "text");
    mDescriptionMapper->addMapping( ui->descriptionLabel , 1, "text");
    mDescriptionMapper->toFirst();

    ui->promptPage->setModel(mModel);

    connect( ui->treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(itemClicked(QModelIndex,QModelIndex)));

    ui->treeView->setModel( mModel );

    setStyles();

    QModelIndex index = mModel->index(0,0);
    selectIndex( index );
}

void MainWindow::setStyles()
{
    ui->promptPage->setDescriptionStyle( mCourse->descriptionStyle() );
    ui->promptPage->setTextEditStyle( mCourse->textEntryStyle() );

    // user-defined styles
    ui->headerLabel->setStyleSheet( mCourse->headerStyle() );
    ui->descriptionLabel->setStyleSheet( mCourse->descriptionStyle() );
    ui->hintLabel->setStyleSheet(mCourse->promptStyle());
}

QModelIndex MainWindow::selectedOrFirst() const
{
    if( ui->treeView->selectionModel()->selectedIndexes().count() > 0 )
    {
        return ui->treeView->selectionModel()->selectedIndexes().first();
    }
    else
    {
        return mModel->index(0,0);
    }
}

void MainWindow::selectIndex(const QModelIndex &index)
{
    ui->treeView->clearSelection();
    ui->treeView->selectionModel()->select( index, QItemSelectionModel::Select );
    ui->treeView->scrollTo(index);
    itemClicked( index );
}

void MainWindow::nextSlide()
{
    QModelIndex index = selectedOrFirst();
    ui->treeView->expand(index);
    selectIndex( ui->treeView->indexBelow(index) );
}

void MainWindow::previousSlide()
{
    QModelIndex index = selectedOrFirst();
    QModelIndex above = ui->treeView->indexAbove(index);
    if( mModel->hasChildren(above) && mModel->hasChildren(index) )
    {
        ui->treeView->expand(above);
        selectIndex( mModel->index( mModel->rowCount(above)-1 , 0, above ) );
    }
    else
    {
        selectIndex( above );
    }
}

void MainWindow::setHint(const QString &newHint)
{
    ui->hintLabel->setText(newHint);
}

void MainWindow::editCourse()
{
    CourseEditorWindow * editor = new CourseEditorWindow(mCourse, mModel);
    editor->show();
}

void MainWindow::editKeyboard()
{
    KeyboardEditorWindow * editor = new KeyboardEditorWindow(mCourse->keyboard());
    editor->setStyles( mCourse->textEntryStyle(),  mCourse->promptStyle() );
    editor->show();
}

void MainWindow::editTextStyles()
{
    EditTextStylesDialog dialog(mCourse);
    if( dialog.exec() == QDialog::Accepted )
    {
        setStyles();
    }
}

void MainWindow::promptToMoveForward()
{
    ui->nextButton->setEnabled(true);
    ui->nextButton->setFocus();
}

void MainWindow::itemClicked(const QModelIndex &index, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    QModelIndex parent = index.parent();
    if( parent.isValid() )
    {
        ui->stackedWidget->setCurrentIndex( 1 );
        ui->promptPage->setIndex( index, parent );
        ui->promptPage->clearInput();
        ui->nextButton->setEnabled(false);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex( 0 );
        mDescriptionMapper->setRootIndex(parent);
        mDescriptionMapper->setCurrentModelIndex(index);
        ui->nextButton->setEnabled(true);
    }
    showOrHideDocksAndButtons(index);
}

void MainWindow::showOrHideDocksAndButtons(QModelIndex index)
{
    if( mCourse == 0 || mModel == 0 )
    {
        ui->progressDock->hide();
//        ui->hintDock->hide();
        ui->previousButton->hide();
        ui->nextButton->hide();
        ui->skipButton->hide();
        return;
    }

    ui->progressDock->setVisible( ui->actionShow_progress->isChecked() );

    if( !index.isValid() )
    {
        index = selectedOrFirst();
    }

    if( !index.parent().isValid() && index.row() == 0 )
    {
        ui->previousButton->hide();
    }
    else
    {
        ui->previousButton->show();
    }

    if( !index.parent().isValid() && index.row() == mModel->rowCount() - 1 )
    {
        ui->nextButton->hide();
    }
    else
    {
        ui->nextButton->show();
    }

    if( isPrompt(index) )
    {
        ui->skipButton->show();
        ui->hintDock->setVisible( ui->actionShow_hint->isChecked() );
    }
    else
    {
        ui->skipButton->hide();
        ui->hintDock->hide();
    }
}

bool MainWindow::isPrompt(const QModelIndex & index) const
{
    if( mCourse == 0 || mModel == 0 )
    {
        return false;
    }
    return index.parent().isValid();
}
