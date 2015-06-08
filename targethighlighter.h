#ifndef TARGETHIGHLIGHTER_H
#define TARGETHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class Course;

class TargetHighlighter : public QSyntaxHighlighter
{
public:
    TargetHighlighter(const Course * course, QTextDocument *parent = 0);

    void setTargetText(const QString & targetText);

protected:
    void highlightBlock(const QString &text);

private:
    QString mTargetText;

    QTextCharFormat goodFormat;
    QTextCharFormat badFormat;
};

#endif // TARGETHIGHLIGHTER_H
