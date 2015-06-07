#ifndef EDITTEXTSTYLESDIALOG_H
#define EDITTEXTSTYLESDIALOG_H

#include <QDialog>

class Course;

namespace Ui {
class EditTextStylesDialog;
}

class EditTextStylesDialog : public QDialog
{
    Q_OBJECT

public:
    EditTextStylesDialog(Course * course, QWidget *parent = 0);
    ~EditTextStylesDialog();

private slots:
    void choosePromptStyle();
    void chooseTextEntryStyle();
    void chooseDescriptionStyle();
    void chooseHeaderStyle();
    void saveValues();

private:
    QString styleFromFontDialog(const QString &initial) const;

private:
    Ui::EditTextStylesDialog *ui;
    Course * mCourse;
};

#endif // EDITTEXTSTYLESDIALOG_H
