#include "coloring.h"

namespace NsColoring
{
    void setLineEditTextFormat(QLineEdit* lineEdit, const QList<QTextLayout::FormatRange>& formats)
    {
        if(!lineEdit)
            return;

        QList<QInputMethodEvent::Attribute> attributes;
        foreach(const QTextLayout::FormatRange& fr, formats)
        {
            QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
            int start = fr.start - lineEdit->cursorPosition();
            int length = fr.length;
            QVariant value = fr.format;
            attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
        }
        QInputMethodEvent event(QString(), attributes);
        QCoreApplication::sendEvent(lineEdit, &event);
    }

    void clearLineEditTextFormat(QLineEdit* lineEdit)
    {
        setLineEditTextFormat(lineEdit, QList<QTextLayout::FormatRange>());
    }


    QTextLayout::FormatRange createFormat(QColor aBgColor, int aStartPos, int aEndPos)
    {
        QTextCharFormat format;
        format.setBackground(aBgColor);

        QTextLayout::FormatRange ret;
        ret.start   = aStartPos;
        ret.length  = aEndPos - aStartPos;
        ret.format  = format;

        return ret;
    }


    //-----------------------------------------------------
    //ColorGenerator
    //-----------------------------------------------------
    ColorGenerator::ColorGenerator()
    {
        m_colors =  {
            QColor(0xFF9933),
            QColor(0x5C85D6),
            QColor(0x669999),
            QColor(0x00FFCC),
            QColor(0x666699),
            QColor(0x00CC66),
            QColor(0x66FF66),
            QColor(0xFF66CC),
            QColor(0xFF9999),
        };
    }

    QColor ColorGenerator::getNextColor()
    {
        m_index++;

        if( m_index >= m_colors.size() )
            m_index = 0;

        return m_colors[m_index];
    }


}
