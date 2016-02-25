#-------------------------------------------------
#
# Project created by QtCreator 2014-08-19T22:32:59
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets testlib
QMAKE_CXXFLAGS += -std=c++11

TARGET = RegisterAnalyzer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bitmanipulation/registerfield.cpp \
    bitmanipulation/bitconverter.cpp \
    analyzerwidget.cpp \
    registerfieldmodel.cpp \
    coloring.cpp \
    registerfieldmodel_proxy.cpp \
    tools/crc/crcgenerator.cpp \
    tools/crc/crctool.cpp

HEADERS  += mainwindow.h \
    bitmanipulation/registerfield.h \
    bitmanipulation/bitconverter.h \
    bitmanipulation/bitdef.h \
    analyzerwidget.h \
    registerfieldmodel.h \
    coloring.h \
    registerfieldmodel_proxy.h \
    tools/crc/crcgenerator.h \
    tools/crc/crctool.h

FORMS    += mainwindow.ui \
    analyzer.ui

OTHER_FILES += \
    references.txt

INCLUDEPATH += bitmanipulation/
