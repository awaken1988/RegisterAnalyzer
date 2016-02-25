#ifndef ANALYZERWIDGET_H
#define ANALYZERWIDGET_H

#include <functional>
#include <memory>
#include <QWidget>
#include <QBitArray>
#include <QDebug>
#include <QStyle>
#include <QMap>
#include <QLineEdit>
#include <QTextLayout>
#include <functional>
#include "registerfieldmodel.h"
#include "registerfieldmodel_proxy.h"
#include "ui_analyzer.h"
#include "bitconverter.h"
#include "bitdef.h"


class AnalyzerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnalyzerWidget(std::shared_ptr<RegisterFieldModel> aModel, QWidget *parent = 0);
    void initInputConverters();
    void initTools();
    void initShiftInvert();
    void initTreeView();
    void bitColoring();

    std::shared_ptr<RegisterFieldModel> getModel();

    ~AnalyzerWidget();

    enum class edit_cause_t : int
    {
        HEX,
        BIN,
    };



public:
    struct updaterFuns_t
    {
        std::function<bool(QBitArray&,QString,bool)>    strToContent;
        std::function<bool(const QBitArray&)>           contentToWidget;

    };

private:
    Ui::AnalyzerForm*                       ui;
    std::shared_ptr<RegisterFieldModel>     m_model;
    updaterFuns_t                           m_updater;

signals:
    void registerEdited(edit_cause_t aCause);
public slots:

    //------------------
    // For external use
    //------------------
    void shiftLeft();
    void shiftRight();
    void mirror();
    void invert();
};

#endif // ANALYZERWIDGET_H
