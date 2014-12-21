#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_registers->setTabsClosable(true);

    connect(ui->actionNewTab, SIGNAL(triggered()), this, SLOT(addNewTab()));
    connect(ui->tab_registers, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    addNewTab();


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
