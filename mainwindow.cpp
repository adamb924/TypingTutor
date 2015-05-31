#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "course.h"
#include "descriptionform.h"
#include "promptform.h"

#include <QStackedWidget>
#include <QLabel>
#include <QFileDialog>
#include <QtDebug>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mStackedWidget(0),
    mCourseModel(0),
    mCourse(0)
{
    ui->setupUi(this);

    connect( ui->actionOpen_course, SIGNAL(triggered()), this, SLOT(openCourse()) );

    connect( ui->promptDock, SIGNAL(visibilityChanged(bool)), ui->actionShow_hint, SLOT(setChecked(bool)) );
    connect( ui->actionShow_hint, SIGNAL(toggled(bool)), ui->promptDock, SLOT(setVisible(bool)) );

    connect( ui->progressDock, SIGNAL(visibilityChanged(bool)), ui->actionShow_progress, SLOT(setChecked(bool)) );
    connect( ui->actionShow_progress, SIGNAL(toggled(bool)), ui->progressDock, SLOT(setVisible(bool)) );

    showOrHideDocks();
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
    if( mStackedWidget != 0 )
    {
        delete mStackedWidget;
    }

    /// Create the tree model
    if( mCourseModel != 0 )
    {
        delete mCourseModel;
    }
    mCourseModel = new QStandardItemModel;

    /// Create the actual interface
    mStackedWidget = new QStackedWidget;

    DescriptionForm * courseDesc = new DescriptionForm( mCourse->name(), mCourse->description() );
    courseDesc->setHasPreviousNext(false,true);
    courseDesc->setHeaderStyle( mCourse->headerStyle() );
    courseDesc->setDescriptionStyle( mCourse->descriptionStyle() );
    connect( courseDesc, SIGNAL(next()), this, SLOT(nextSlide()) );
    connect( courseDesc, SIGNAL(previous()), this, SLOT(previousSlide()) );
    mStackedWidget->addWidget( courseDesc );

    QStandardItem *titleItem = new QStandardItem( mCourse->name() );
    titleItem->setEditable(false);
    mCourseModel->appendRow( titleItem );
    mIndexWidgetHash.insert(titleItem->index(), courseDesc);

    QList<Section>* sections = mCourse->sections();
    for(int s=0; s<sections->count(); s++)
    {
        Section section = sections->at(s);
        DescriptionForm * desc = new DescriptionForm( section.name(), section.description() );
        desc->setHeaderStyle( mCourse->headerStyle() );
        desc->setDescriptionStyle( mCourse->descriptionStyle() );
        connect( desc, SIGNAL(next()), this, SLOT(nextSlide()) );
        connect( desc, SIGNAL(previous()), this, SLOT(previousSlide()) );
        mStackedWidget->addWidget( desc );

        QStandardItem *sectionItem = new QStandardItem( mCourse->sections()->at(s).name() );
        sectionItem->setEditable(false);
        mCourseModel->appendRow( sectionItem );

        mIndexWidgetHash.insert(sectionItem->index(), desc);

        QList<Prompt>* prompts = section.prompts();
        for(int p=0; p<prompts->count(); p++)
        {
            PromptForm *promptForm = new PromptForm( prompts->at(p), mCourse->keyboard(), mCourse->name(), section.name() );
            promptForm->setHeaderStyle( mCourse->headerStyle() );
            promptForm->setDescriptionStyle( mCourse->descriptionStyle() );
            promptForm->setTextEditStyle(mCourse->textEntryStyle());
            connect( promptForm, SIGNAL(next()), this, SLOT(nextSlide()) );
            connect( promptForm, SIGNAL(previous()), this, SLOT(previousSlide()) );
            connect( promptForm, SIGNAL(changePrompt(QString)), this, SLOT(setPrompt(QString)) );
            connect( promptForm, SIGNAL(showHint(QString)), statusBar(), SLOT(showMessage(QString)) );
            mStackedWidget->addWidget( promptForm );

            QStandardItem *promptItem = new QStandardItem( prompts->at(p).targetText() );
            promptItem->setEditable(false);
            sectionItem->appendRow( promptItem );

            mIndexWidgetHash.insert(promptItem->index(), promptForm);
        }
/*
        DescriptionForm * concPage = new DescriptionForm( section.conclusionHeader(), section.conclusionMessage() );
        connect( concPage, SIGNAL(next()), this, SLOT(nextSlide()) );
        connect( concPage, SIGNAL(previous()), this, SLOT(previousSlide()) );
        mStackedWidget->addWidget( concPage );

        QStandardItem *concItem = new QStandardItem( mCourse->sections()->at(s).conclusionHeader() );
        concItem->setEditable(false);
        sectionItem->appendRow( concItem );
        mIndexWidgetHash.insert(concItem->index(), concPage);
*/
    }

    DescriptionForm * concPage = new DescriptionForm( mCourse->conclusionHeader(), mCourse->conclusionMessage() );
    concPage->setHeaderStyle( mCourse->headerStyle() );
    concPage->setDescriptionStyle( mCourse->descriptionStyle() );
    connect( concPage, SIGNAL(next()), this, SLOT(nextSlide()) );
    connect( concPage, SIGNAL(previous()), this, SLOT(previousSlide()) );
    concPage->setHasPreviousNext(true,false);
    mStackedWidget->addWidget( concPage );

    QStandardItem *concItem = new QStandardItem( mCourse->conclusionHeader() );
    concItem->setEditable(false);
    mCourseModel->appendRow( concItem );
    mIndexWidgetHash.insert(concItem->index(), concPage);

    ui->prompt->setText("");


    ui->treeView->setModel(mCourseModel);
    ui->treeView->setHeaderHidden(true);
    connect( ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(focusWidget(QModelIndex)) );

    // user-defined styles
    ui->prompt->setStyleSheet(mCourse->promptStyle());

    ui->verticalLayout->addWidget( mStackedWidget );

    showSlide(0);
}

void MainWindow::nextSlide()
{
    int current = mStackedWidget->currentIndex();
    if( current < mStackedWidget->count() - 1 )
    {
        showSlide(current+1);
    }
}

void MainWindow::previousSlide()
{
    int current = mStackedWidget->currentIndex();
    if( current > 0 )
    {
        showSlide(current-1);
    }
}

void MainWindow::showSlide(int index)
{
    mStackedWidget->setCurrentIndex( index );
    PromptForm* p = qobject_cast<PromptForm*>(mStackedWidget->currentWidget());
    if( p != 0 )
    {
        p->clearInput();
    }
    QWidget * w = mStackedWidget->currentWidget();
    QModelIndex i = mIndexWidgetHash.key( w, QModelIndex() );
    if( i.isValid() )
    {
        QItemSelectionModel *selectionModel = ui->treeView->selectionModel();
        selectionModel->select( i , QItemSelectionModel::ClearAndSelect);
        ui->treeView->collapseAll();
        QModelIndex parent = i.parent();
        if( parent != QModelIndex() )
        {
            ui->treeView->setExpanded(parent, true);
        }
    }
    showOrHideDocks();
}

void MainWindow::setPrompt(const QString &newPrompt)
{
    ui->prompt->setText(newPrompt);
}

void MainWindow::focusWidget(const QModelIndex &index)
{
    QWidget *w = mIndexWidgetHash.value(index, 0);
    if( w != 0 )
    {
        int currentSlide = mStackedWidget->indexOf( w );
        if( currentSlide != -1)
        {
            showSlide(currentSlide);
        }
    }
}

void MainWindow::showOrHideDocks()
{
    if( mStackedWidget == 0 )
    {
        ui->progressDock->hide();
    }
    else
    {
        ui->progressDock->setVisible( ui->actionShow_progress->isChecked() );
    }
    if( mStackedWidget == 0 || qobject_cast<PromptForm*>(mStackedWidget->currentWidget()) == 0 )
    {
        ui->promptDock->hide();
    }
    else
    {
        ui->promptDock->setVisible( ui->actionShow_hint->isChecked() );
    }
}

