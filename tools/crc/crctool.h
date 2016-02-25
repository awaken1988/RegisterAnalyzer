#ifndef CRCTOOL_H
#define CRCTOOL_H

#include <QWidget>
#include "crcgenerator.h"
#include "../../registerfieldmodel.h"
#include <QVBoxLayout>
#include <QAbstractAnimation>
#include <QFormLayout>
#include <QDebug>
#include <QVector>
#include <QLabel>
#include <QBitArray>
#include <QFormLayout>
#include "../../bitmanipulation/bitconverter.h"

class CrcTool : public QWidget
{
    Q_OBJECT
public:
    explicit CrcTool(RegisterFieldModel& aRegModel, QWidget *parent = 0);

protected:
    QFormLayout* m_form;

    struct crc_calc_t {
        QString     crcName;
        QLabel*     crcVal;
        QBitArray   poly;
    };

    QVector<crc_calc_t> m_calc;


signals:

public slots:
    void updateCrc(const QBitArray&);

};

#endif // CRCTOOL_H
