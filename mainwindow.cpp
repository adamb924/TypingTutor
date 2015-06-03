#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "course.h"
#include "promptform.h"
#include "courseeditorwindow.h"

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
    connect( ui->nextButton, SIGNAL(clicked()), this, SLOT(nextSlide()) );
    connect( ui->previousButton, SIGNAL(clicked()), this, SLOT(previousSlide()) );
    connect( ui->skipButton, SIGNAL(clicked()), this, SLOT(nextSlide()) );
    connect( ui->promptPage, SIGNAL(showHint(QString)), this, SLOT(setHint(QString)));
    connect( ui->promptPage, SIGNAL(inputPrompt(QString)), statusBar(), SLOT(showMessage(QString)) );
    connect( ui->promptPage, SIGNAL(textEnteredCorrectly()), this, SLOT(promptToMoveForward()) );

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
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Typing Courses (*.xml)"));
    if( filename.isEmpty() )
    {
        return;
    }
    if( mCourse != 0 )
    {
        delete mCourse;
    }
    mCourse = new Course(filename);
    if( mCourse->isValid() )
    {
        setupCourseLayout();
    }
}

void MainWindow::setupCourseLayout()
{
    /// Create the tree model
    if( mModel != 0 )
    {
        delete mModel;
    }

    ui->promptPage->setKeyboard( mCourse->keyboard() );

    mModel = new QStandardItemModel;

    ui->actionEdit_course->setEnabled(true);

    newDescriptionItem( mCourse->name(), mCourse->description() );

    QList<Section*>* sections = mCourse->sections();
    for(int s=0; s<sections->count(); s++)
    {
        Section * section = sections->at(s);
        newDescriptionItem( section->name(), section->description() );

        QList<Prompt*>* prompts = section->prompts();
        for(int p=0; p<prompts->count(); p++)
        {
            newPromptItem(prompts->at(p));
      }
    }

    newDescriptionItem( mCourse->conclusionHeader(), mCourse->conclusionMessage() );

    ui->hintLabel->setText("");

    ui->treeView->setModel(mModel);
    ui->treeView->hideColumn(1);
    ui->treeView->setHeaderHidden(true);

    // user-defined styles
    ui->headerLabel->setStyleSheet( mCourse->headerStyle() );
    ui->descriptionLabel->setStyleSheet( mCourse->descriptionStyle() );
    ui->hintLabel->setStyleSheet(mCourse->promptStyle());

    connect( mModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)) );

    // description
    mDescriptionMapper = new QDataWidgetMapper;
    mDescriptionMapper->setModel(mModel);
    mDescriptionMapper->addMapping( ui->headerLabel, 0, "text");
    mDescriptionMapper->addMapping( ui->descriptionLabel , 1, "text");
    mDescriptionMapper->toFirst();

    ui->promptPage->setModel(mModel);
    ui->promptPage->setDescriptionStyle( mCourse->descriptionStyle() );
    ui->promptPage->setTextEditStyle( mCourse->textEntryStyle() );

    connect( ui->treeView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(itemClicked(QModelIndex,QModelIndex)));

    QModelIndex index = mModel->index(0,0);
    selectIndex( index );
}

void MainWindow::newDescriptionItem(const QString &header, const QString &description )
{
    QStandardItem *item = new QStandardItem( header );
    QStandardItem *descriptionColumn = new QStandardItem( description );

    QList<QStandardItem*> columns;
    columns << item << descriptionColumn;

    mModel->appendRow( columns );
}

void MainWindow::newPromptItem(const Prompt * prompt)
{
    QStandardItem *first = new QStandardItem( prompt->targetText() );
    QStandardItem *second = new QStandardItem( prompt->description() );
    first->setEditable(false);
    second->setEditable(false);

    QList<QStandardItem*> columns;
    columns << first << second;

    mModel->item( mModel->rowCount() -1 , 0 )->appendRow( columns );
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

void MainWindow::promptToMoveForward()
{
    ui->nextButton->setEnabled(true);
    ui->nextButton->setFocus();
}

void MainWindow::itemChanged(QStandardItem *item)
{
    QModelIndex index = item->index();
    QModelIndex parent = index.parent();

    int row = index.row();
    int col = index.column();

    if( parent.isValid() ) // prompt
    {
        int section = parent.row() - 1;
        if( col == 0 )
        {
            mCourse->sections()->at(section)->prompts()->at( row )->setTargetText( item->text() );
        }
        else
        {
            mCourse->sections()->at(section)->prompts()->at( row )->setDescription( item->text() );
        }
    }
    else // header
    {
        if( row == 0 ) // course header
        {
            if( col == 0 )
            {
                mCourse->setName( item->text() );
            }
            else
            {
                mCourse->setDescription( item->text() );
            }
        }
        else if( row == mCourse->sections()->count() + 1 ) // course conclusion
        {
            if( col == 0 )
            {
                mCourse->setConclusionHeader( item->text() );
            }
            else
            {
                mCourse->setConclusionMessage( item->text() );
            }
        }
        else // section header
        {
            row--;
            if( col == 0 )
            {
                mCourse->sections()->at(row)->setName( item->text() );
            }
            else
            {
                mCourse->sections()->at(row)->setDescription( item->text() );
            }
        }
    }
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
