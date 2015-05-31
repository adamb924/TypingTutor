#ifndef DESCRIPTIONFORM_H
#define DESCRIPTIONFORM_H

#include <QWidget>

namespace Ui {
class DescriptionForm;
}

class DescriptionForm : public QWidget
{
    Q_OBJECT

public:
    explicit DescriptionForm(const QString & name, const QString & description, QWidget *parent = 0);
    ~DescriptionForm();

    void setNameDescription(const QString & name, const QString & description);
    void setHasPreviousNext(bool hasPrevious, bool hasNext);
    void setHeaderStyle(const QString & style);
    void setDescriptionStyle(const QString & style);

signals:
    void next();
    void previous();

private:
    Ui::DescriptionForm *ui;
};

#endif // DESCRIPTIONFORM_H
