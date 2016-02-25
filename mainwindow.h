#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "analyzerwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    AnalyzerWidget* getCurrent();

private:
    Ui::MainWindow *ui;
public slots:
    void addNewTab();
    void closeTab(int aTabNr);
    void changeRegName(const QString& aText);
    void cloneTab();
};

#endif // MAINWINDOW_H
