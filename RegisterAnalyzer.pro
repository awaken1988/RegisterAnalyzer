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
    registerfield.cpp \
    bitconverter.cpp \
    testbitconverter.cpp \
    analyzerwidget.cpp \
    registerfieldmodel.cpp \
    coloring.cpp \
    registerfieldmodel_proxy.cpp

HEADERS  += mainwindow.h \
    registerfield.h \
    bitconverter.h \
    testbitconverter.h \
    bitdef.h \
    analyzerwidget.h \
    registerfieldmodel.h \
    coloring.h \
    registerfieldmodel_proxy.h

FORMS    += mainwindow.ui \
    analyzer.ui

OTHER_FILES += \
    references.txt

