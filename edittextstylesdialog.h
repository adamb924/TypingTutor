#ifndef EDITTEXTSTYLESDIALOG_H
#define EDITTEXTSTYLESDIALOG_H

#include <QDialog>
#include <QColor>

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

    void errorFg();
    void errorBg();
    void remainderFg();
    void remainderBg();

private:
    QString styleFromFontDialog(const QString &initial) const;
    void setLabelStyles();

private:
    Ui::EditTextStylesDialog *ui;
    Course * mCourse;
    QColor mRemainderFg, mRemainderBg;
    QColor mErrorFg, mErrorBg;
};

#endif // EDITTEXTSTYLESDIALOG_H
