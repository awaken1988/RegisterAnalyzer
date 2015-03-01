#include "analyzerwidget.h"

//tools
#include "tools/crctool/crctool.h"

/*
 * for aModel there may be better solutions so that the Model is also destroyed with that widget
 */
AnalyzerWidget::AnalyzerWidget(std::shared_ptr<RegisterFieldModel> aModel, QWidget *parent) :
    QWidget(parent), ui(new Ui::AnalyzerForm)
{
    ui->setupUi(this);

    m_model = aModel;

    //----------------------
    // setup TreeView
    //----------------------
    {
        Registerfieldmodel_proxy* modelProxy = new Registerfieldmodel_proxy(this);
        modelProxy->setSourceModel(m_model.get());
        modelProxy->setDynamicSortFilter(true);

        ui->registerFieldView->setModel(modelProxy);
        ui->registerFieldView->setSortingEnabled(true);
    }

    //add updateRegister
//    connect(this, SIGNAL(registerEdited(edit_cause_t)), this, SLOT(updateRegister(edit_cause_t)));

    //converters for inputs
    initInputConverters();

    //RegisterView Buttons
    ui->btnAddRow->setIcon( style()->standardIcon(QStyle::SP_MediaVolume) );
    connect(ui->btnAddRow, &QPushButton::clicked, this, [=](bool){
        m_model->addRow(ui->ledit_addFieldName->text());
        ui->ledit_addFieldName->clear(); });

    //change register name
    connect(ui->ledit_registerName, &QLineEdit::textEdited, this, [=](QString aName){m_model->setRegisterName(aName);});

    //tools
    initTools();
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

        qDebug()<<binStrStart<<" blaaa";

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

              qDebug()<<"start="<<virtStart<<"; end="<<virtEnd<<endl;

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






