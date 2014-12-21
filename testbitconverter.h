#ifndef TESTBITCONVERTER_H
#define TESTBITCONVERTER_H

#include <QtTest/QTest>
#include <QObject>
#include <QBitArray>
#include "bitconverter.h"

class TestBitConverter : public QObject
{
    Q_OBJECT
private slots:
    void convertFromHex();
};

#endif // TESTBITCONVERTER_H
