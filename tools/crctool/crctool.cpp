#include "crctool.h"

CrcTool::CrcTool(RegisterFieldModel& aRegModel, QWidget *parent ) :
    QWidget(parent)
{
   QVBoxLayout* mainLayout = new QVBoxLayout(parent);
   QFormLayout* formLayout = new QFormLayout(parent);

   QWidget* formWidget = new QWidget(parent);
   QLabel* warningText = new QLabel("warning: there is no warranty about the corectness of the crc sum", parent );

   this->setLayout(mainLayout);        //is this a memory leak or handle this Qt?
   formWidget->setLayout(formLayout);

   mainLayout->addWidget(warningText);
   mainLayout->addWidget(formWidget);

    //polynoms from:
    //  http://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks

   bool cutZero = false;

    m_calc = {
        { "CRC-4-ITU",      new QLabel("...", this), NsCRC::toQBitarray<4>(0x3) },
        { "CRC-5-EPC",      new QLabel("...", this), NsCRC::toQBitarray<5>(0x09) },
        { "CRC-5-ITU",      new QLabel("...", this), NsCRC::toQBitarray<5>(0x15) },
        { "CRC-5-USB",      new QLabel("...", this), NsCRC::toQBitarray<5>(0x05) },
        { "CRC-8",          new QLabel("...", this), NsCRC::toQBitarray<8>(0xD5) },
        { "CRC-8-CCITT",    new QLabel("...", this), NsCRC::toQBitarray<8>(0x07) },
        { "CRC-16-IBM",     new QLabel("...", this), NsCRC::toQBitarray<16>(0x8005) },
        { "CRC-16-CCITT",   new QLabel("...", this), NsCRC::toQBitarray<16>(0x1021) },
        { "CRC-32",         new QLabel("...", this), NsCRC::toQBitarray<32>(0x04C11DB7) },
        { "CRC-32C ",       new QLabel("...", this), NsCRC::toQBitarray<32>(0x1EDC6F41) },
        { "CRC-32K ",       new QLabel("...", this), NsCRC::toQBitarray<32>(0x741B8CD7) },
        { "CRC-32Q",        new QLabel("...", this), NsCRC::toQBitarray<32>(0x814141AB) },
    };


    formLayout->setMargin(48);
    for(auto& iCalc: m_calc)
    {
        formLayout->addRow(iCalc.crcName, iCalc.crcVal);
    }

    connect(&aRegModel, SIGNAL(fieldChanged(const QBitArray&)), this, SLOT(updateCrc(const QBitArray&)));
}

void CrcTool::updateCrc(const QBitArray& aContent)
{
    qDebug()<<__FUNCTION__;

    for(auto& iCalc: m_calc)
    {
        qDebug()<<"poly: "<<iCalc.poly;
        QString   crcResultHex = "...";


        if( aContent.size() > 0 )
        {
            QBitArray crcPoly       = iCalc.poly;
            QBitArray crcResult     = NsCRC::generate(aContent, crcPoly);
            crcResultHex  = NsConverter::toHexStr(crcResult);

        }

        iCalc.crcVal->setText(crcResultHex);
    }
}
