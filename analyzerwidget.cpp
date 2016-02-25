#include "analyzerwidget.h"

//tools
#include "tools/crc/crctool.h"
#include <algorithm>

/*
 * for aModel there may be better solutions so that the Model is also destroyed with that widget
 */
AnalyzerWidget::AnalyzerWidget(std::shared_ptr<RegisterFieldModel> aModel, QWidget *parent) :
    QWidget(parent), ui(new Ui::AnalyzerForm)
{
    ui->setupUi(this);

    m_model = aModel;





    //call all init... methods
    {
        initTreeView();
        initInputConverters();
        initTools();
        initShiftInvert();
    }

    //init other gui elements
    {
        QHBoxLayout* inputLayout = dynamic_cast<QHBoxLayout*>(ui->hbox_input->layout());
        inputLayout->setStretch(0, 3);
        inputLayout->setStretch(1,1);
    }
}

void AnalyzerWidget::initInputConverters()
{

    struct simple_input_t //export this to the header???
    {
        std::function<bool(QBitArray&, QString, bool)>  convertFrom;
        std::function<void(const QBitArray&)>           convertTo;
        QLineEdit*                                      srcWdiget;
    };

    simple_input_t simpleInput[] = {
        { NsConverter::fromHexStr,
          [=](const QBitArray& aContent) -> void { ui->ledit_registerHex->setText( NsConverter::toHexStr(aContent));},
          ui->ledit_registerHex
        },

        { NsConverter::fromBinStr,
          [=](const QBitArray& aContent) -> void { ui->ledit_registerBin->setText( NsConverter::toBinStr(aContent));},
          ui->ledit_registerBin
        }

    };

    //---------------------------------------------
    // From input (e.g QLineEdit) to m_model
    //---------------------------------------------
    for(auto& inputConverter: simpleInput)
    {
        //value changed from hex input
        connect(inputConverter.srcWdiget,
                &QLineEdit::textEdited, this,
                [=](QString aChanged) -> void {  QBitArray tmpContent;
                                                    if( !(inputConverter.convertFrom(tmpContent, aChanged, true)) )
                                                        return;     //add an error handler
                                                     m_model->updateRegisterValue(tmpContent);       //update register fields
                                                     bitColoring();
                                                 });
    }

    //---------------------------------------------
    // From m_model (QBitArray) to Widget
    //---------------------------------------------
    for(auto& inputConverter: simpleInput)
    {
        //value changed from hex input
        connect(this->m_model.get(), &RegisterFieldModel::fieldChanged, this, inputConverter.convertTo);
    }

    //---------------------------------------------
    // From m_model (QBitArray) to colorin
    //---------------------------------------------
    connect(this->m_model.get(), &RegisterFieldModel::fieldChanged, this, [=](const QBitArray&)->void {this->bitColoring();} );


}

//! Initialize additionals tools
void AnalyzerWidget::initTools()
{
    int lastItem = 0;

    //crc tool
    {
        CrcTool* crcTool = new CrcTool(*this->m_model.get());
        lastItem = ui->registerToolBox->addItem(crcTool, "CRC");
    }

    ui->registerToolBox->setCurrentIndex(lastItem);

    ui->registerToolBox->setItemText(0, "About");
}

//FIXME: delete this function
void AnalyzerWidget::initShiftInvert()
{
    /*
    using namespace NsConverter;


    //shift
    {
        auto fnLeft = [&](bool aChecked) {
            QBitArray tmp = m_model->getRegister();
            tmp = tmp<<shift_t(1, true);
            m_model->updateRegisterValue(tmp);
            qDebug()<<"shiftLeft";
        };
        auto fnRight = [&](bool aChecked) {
            QBitArray tmp = m_model->getRegister();
            tmp = tmp>>shift_t(1, true);
            m_model->updateRegisterValue(tmp);
            qDebug()<<"shiftRight";
        };

        connect(ui->btn_shiftLeft, &QPushButton::clicked, fnLeft);
        connect(ui->btn_shiftRight, &QPushButton::clicked, fnRight);
    }

    //invert
    {
        connect(ui->bn_invert,  &QPushButton::clicked, [&](){
            QBitArray tmp = m_model->getRegister();
            tmp = invert(tmp);
            m_model->updateRegisterValue(tmp);
        });
    }

    */

}

//TODO: make this function more versatile
//  - different coloring modes (by Regster, by Nipple, by Bit)
//  - colorize hex input by Register (difficult because registers are not "nipple" align)
void AnalyzerWidget::bitColoring()
{
    //colorize bin input
    {
        QList<QTextLayout::FormatRange> formats;

        const int binStrStart = ui->ledit_registerBin->text().size();

        NsColoring::clearLineEditTextFormat(ui->ledit_registerBin);

        for(auto iField: m_model->getField())
        {
            const int virtStart = binStrStart - iField.end-1;
            const int virtEnd   = binStrStart - iField.start;



            formats.append(NsColoring::createFormat(iField.color, virtStart, virtEnd));
        }

        NsColoring::setLineEditTextFormat(ui->ledit_registerBin, formats);
    }

    //colorize hex input (by Byte)
    {
        QList<QTextLayout::FormatRange> formats;

        QColor evenColor = QColor((QRgb)0xFFFFFF);
        QColor oddColor = QColor((QRgb) 0xDDDDDD);

        const int textLen = ui->ledit_registerHex->text().size();
        const int markerWidth = 2;

        int currPos = 0;
        for(; currPos <= textLen; currPos+=markerWidth)
        {
            const int remain    = (textLen-currPos);
            const int markerLen = remain >= markerWidth ?
                                                markerWidth : remain;
            const int virtStart = textLen - currPos - markerLen;
            const int virtEnd   = textLen - currPos;

            //qDebug()<<"start="<<virtStart<<"; end="<<virtEnd<<endl;

            if( currPos % 4 )
                formats.append(NsColoring::createFormat(oddColor, virtStart, virtEnd));
            else
                formats.append(NsColoring::createFormat(evenColor, virtStart, virtEnd));
        }

        NsColoring::setLineEditTextFormat(ui->ledit_registerHex, formats);
    }
}



AnalyzerWidget::~AnalyzerWidget()
{
    delete ui;
}

void AnalyzerWidget::shiftLeft()
{
    using namespace NsConverter;

    QBitArray tmp = m_model->getRegister();
    tmp = tmp<<shift_t(1, true);
    m_model->updateRegisterValue(tmp);
    //qDebug()<<"shiftLeft";
}

void AnalyzerWidget::shiftRight()
{
    using namespace NsConverter;

    QBitArray tmp = m_model->getRegister();
    tmp = tmp>>shift_t(1, true);
    m_model->updateRegisterValue(tmp);
    //qDebug()<<"shiftRight";
}

void AnalyzerWidget::mirror()
{
    using namespace NsConverter;

    QBitArray tmp = m_model->getRegister();
    tmp = NsConverter::mirror(tmp);
    m_model->updateRegisterValue(tmp);
}


void AnalyzerWidget::invert()
{
    using namespace NsConverter;

    QBitArray tmp = m_model->getRegister();
    tmp = NsConverter::invert(tmp);
    m_model->updateRegisterValue(tmp);
}

void AnalyzerWidget::initTreeView()
{
    Registerfieldmodel_proxy* modelProxy = new Registerfieldmodel_proxy(this);

    // Attach Model; Sorting proxy...
    {
        modelProxy->setSourceModel(m_model.get());
        modelProxy->setDynamicSortFilter(true);

        ui->registerFieldView->setModel(modelProxy);
        ui->registerFieldView->setSortingEnabled(true);
    }

    // Row Selection; Ctrl/Shift Selection
    {
        QItemSelectionModel* currModel = ui->registerFieldView->selectionModel();

        ui->registerFieldView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->registerFieldView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    }

    //buttos
    {
        //add row
        connect(ui->btnAddRow, &QPushButton::clicked, this, [=](bool){
            m_model->addRow(ui->ledit_addFieldName->text());
            ui->ledit_addFieldName->clear(); });

        //delete selected rows
        connect(ui->btnDelRow, &QPushButton::clicked, [&](bool) {
            QModelIndexList selList = ui->registerFieldView->selectionModel()->selectedRows();

            QSortFilterProxyModel* proxyPtr = dynamic_cast<QSortFilterProxyModel*>(ui->registerFieldView->model());

            if( nullptr == proxyPtr) {
                qDebug()<<"cannot cast QSortFilterProxyModel";
            }


            //translate proxies QModelIndex to RegisterField models
           for(auto& i: selList) {
               qDebug()<<"map "<<i<<" to "<<proxyPtr->mapToSource(i);
               i  = proxyPtr->mapToSource(i);
            };
            std::sort(selList.begin(), selList.end(), [](const QModelIndex& a, const QModelIndex& b){
                if(a.row() < b.row() )
                    return false;
                else
                    return true;
            });


            for(const QModelIndex& i: selList) {
                qDebug()<<"delete row="<<i;

                m_model->removeRows(i.row(), 1);
            }

        });
    }
}

std::shared_ptr<RegisterFieldModel> AnalyzerWidget::getModel()
{
    return m_model;
}





