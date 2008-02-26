#include "MainWindow.h"
#include "OrientationAxes.h"
#include "RenderPoints.h"

#include "vtkRenderer.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

// ---------------------------------------------------------------------------


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    connect(actionExit, SIGNAL(triggered()), this, SLOT(fileExit()));

    // renderer
    ren = vtkRenderer::New();
    ren->SetBackground(25./256, 51./256, 102./256);
    qvtkWidget->GetRenderWindow()->AddRenderer(ren);
    AddOrientationAxes(qvtkWidget);
    RenderPoints(ren);
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
