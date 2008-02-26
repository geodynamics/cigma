#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "MainWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "OrientationAxes.h"
#include "RenderPoints.h"
#include "RenderTetrahedron.h"
#include "RenderHexahedron.h"


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

    //RenderPoints(ren);
    //RenderTetrahedron(ren);
    RenderHexahedron(ren);
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
