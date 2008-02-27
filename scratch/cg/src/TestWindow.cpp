#include <iostream>
#include "TestWindow.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "OrientationAxes.h"
#include "RenderPoints.h"


// ---------------------------------------------------------------------------

TestWindow::TestWindow(QWidget *parent)
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


TestWindow::~TestWindow()
{
}


// ---------------------------------------------------------------------------

void TestWindow::fileExit()
{
    qApp->exit();
}

// ---------------------------------------------------------------------------
