#include "MainWindow.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ---------------------------------------------------------------------------


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    connect(actionExit, SIGNAL(triggered()), this, SLOT(fileExit()));

}


MainWindow::~MainWindow()
{

}


// ---------------------------------------------------------------------------


void MainWindow::fileExit()
{
    qApp->exit();
}


// ---------------------------------------------------------------------------
