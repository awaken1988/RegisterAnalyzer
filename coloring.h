#ifndef COLORING_H
#define COLORING_H

#include <QLineEdit>
#include <QList>
#include <QTextLayout>
#include <QInputMethodEvent>
#include <QTextLayout>
#include <QColor>
#include <QCoreApplication>


namespace NsColoring
{

    //Format particular part of the text:
    //  found at: http://stackoverflow.com/questions/14417333/how-can-i-change-color-of-part-of-the-text-in-qlineedit
    void setLineEditTextFormat(QLineEdit* lineEdit, const QList<QTextLayout::FormatRange>& formats);
    void clearLineEditTextFormat(QLineEdit* lineEdit);

    QTextLayout::FormatRange    createFormat(QColor aBgColor, int aStartPos, int aEndPos);

    QColor createNextColor();
}

#endif //COLORING_H
