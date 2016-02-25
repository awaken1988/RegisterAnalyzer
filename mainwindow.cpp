#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSpacerItem>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_registers->setTabsClosable(true);

    connect(ui->actionNewTab, SIGNAL(triggered()), this, SLOT(addNewTab()));
    connect(ui->actionClone_Tab, SIGNAL(triggered()), SLOT(cloneTab()));
    connect(ui->tab_registers, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    addNewTab();

    //icons
    {
        ui->actionNewTab->setIcon(
                    style()->standardIcon(QStyle::SP_FileDialogNewFolder));

        ui->actionClone_Tab->setIcon(style()->standardIcon(QStyle::SP_CommandLink));

        ui->actionUndo->setIcon(
                   style()->standardIcon(QStyle::SP_ArrowLeft));

        ui->actionRedo->setIcon(
                   style()->standardIcon(QStyle::SP_ArrowRight));

        // undo-redo function not yet implemented
        ui->toolBar->removeAction(ui->actionUndo);
        ui->toolBar->removeAction(ui->actionRedo);
    }

    //Add spacing to toolbar
    {
        QWidget* spacer = new QWidget;
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        ui->toolBar->addWidget(spacer);
    }
    //gui buttons for register name
    {
        QGroupBox* grpbox_regname = new QGroupBox(ui->toolBar);
        QHBoxLayout* lyt_regname = new QHBoxLayout;
        QLineEdit* ledit_regname = new QLineEdit;

        grpbox_regname->setTitle("RegisterName");
        grpbox_regname->setLayout(lyt_regname);
        lyt_regname->addWidget(ledit_regname);
        ui->toolBar->addWidget(grpbox_regname);
        ui->toolBar->addSeparator();

        connect(ledit_regname, &QLineEdit::textChanged, this, &MainWindow::changeRegName);

        connect(ui->tab_registers, &QTabWidget::currentChanged, [this, ledit_regname ](int index) -> void {
            //TODO: load register name to ledit_regname
            int currIndex = ui->tab_registers->currentIndex();
            if( -1 == currIndex )
                return;

            ledit_regname->setText( ui->tab_registers->tabText(currIndex) );
        });
    }

    //gui buttons for register manipulation
    {
        QGroupBox* grpbox_regManipulation = new QGroupBox(ui->toolBar);
        QHBoxLayout* lyt_reg_manipulation = new QHBoxLayout;

        QPushButton* btn_shiftLeft = new QPushButton("<<", grpbox_regManipulation);
        QPushButton* btn_shiftRight = new QPushButton(">>", grpbox_regManipulation);
        QPushButton* btn_mirror = new QPushButton("Mirror", grpbox_regManipulation);
        QPushButton* btn_invert = new QPushButton("Invert", grpbox_regManipulation);

        lyt_reg_manipulation->addWidget(btn_shiftLeft);
        lyt_reg_manipulation->addWidget(btn_shiftRight);
        lyt_reg_manipulation->addWidget(btn_mirror);
        lyt_reg_manipulation->addWidget(btn_invert);

        grpbox_regManipulation->setTitle("Register Manipulation");
        grpbox_regManipulation->setLayout(lyt_reg_manipulation);
        ui->toolBar->addWidget(grpbox_regManipulation);

        connect(btn_shiftLeft, &QPushButton::clicked, [&]() {
            if( nullptr == getCurrent() ) return;
            getCurrent()->shiftLeft();
        });

        connect(btn_shiftRight, &QPushButton::clicked, [&]() {
            if( nullptr == getCurrent() ) return;
            getCurrent()->shiftRight();
        });

        connect(btn_mirror, &QPushButton::clicked, [&]() {
            if( nullptr == getCurrent() ) return;
            getCurrent()->mirror();
        });

        connect(btn_invert, &QPushButton::clicked, [&]() {
            if( nullptr == getCurrent() ) return;
            getCurrent()->invert();
        });
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addNewTab()
{
    RegisterFieldModel* regModel = new RegisterFieldModel(nullptr);
    AnalyzerWidget* analyzWidget = new AnalyzerWidget(std::shared_ptr<RegisterFieldModel>(regModel), this);

    int tabNum = ui->tab_registers->addTab(analyzWidget, "no-name");

    //catch signal for register rename
    connect(regModel, &RegisterFieldModel::nameChanged,
        [=](QString aName){
            ui->tab_registers->setTabText(tabNum, aName);
         });

    ui->tab_registers->setCurrentIndex(tabNum);
}

void MainWindow::closeTab(int aTabNr)
{
    QWidget* currWidget = ui->tab_registers->widget(aTabNr);
    ui->tab_registers->removeTab(aTabNr);

    delete currWidget;

}

void MainWindow::changeRegName(const QString& aText)
{
    int currIndex = ui->tab_registers->currentIndex();

    ui->tab_registers->setTabText(currIndex, aText);
}

AnalyzerWidget* MainWindow::getCurrent()
{
    return dynamic_cast<AnalyzerWidget*>(
                ui->tab_registers->currentWidget());
}

void MainWindow::cloneTab()
{
    qDebug()<<"MainWindow::cloneTab()";

    if( getCurrent() == nullptr )
        return;

    //get the original vlaues
    std::shared_ptr<RegisterFieldModel> orig = getCurrent()->getModel();


    //add new tab
    addNewTab();

    getCurrent()->getModel()->import(*orig);
}



