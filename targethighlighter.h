#ifndef TARGETHIGHLIGHTER_H
#define TARGETHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class TargetHighlighter : public QSyntaxHighlighter
{
public:
    TargetHighlighter(const QString & targetText, QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    QString mTargetText;

    QTextCharFormat goodFormat;
    QTextCharFormat badFormat;
};

#endif // TARGETHIGHLIGHTER_H
