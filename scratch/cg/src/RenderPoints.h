#ifndef __RENDER_POINTS_H__
#define __RENDER_POINTS_H__

#include <ctime>
#include <cmath>
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkFloatArray.h"
#include "vtkPoints.h"
#include "vtkPolyVertex.h"
#include "vtkUnstructuredGrid.h"
#include "vtkOutlineFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"

using namespace std;

void RenderPoints(vtkRenderer *ren)
{
    assert(ren != 0);

    int i;
    int N = 10 * 1000;
    double pts[3];

    vtkFloatArray *pcoords = vtkFloatArray::New();
    pcoords->SetNumberOfComponents(3);
    pcoords->SetNumberOfTuples(N);

    srand(time(0));
    for (i = 0; i < N; i++)
    {
        pts[0] = rand()/(1.0 + RAND_MAX);
        pts[1] = rand()/(1.0 + RAND_MAX);
        pts[2] = rand()/(1.0 + RAND_MAX);
        pcoords->SetTuple(i, pts);
    }

    vtkPoints *points = vtkPoints::New();
    points->SetData(pcoords);

    vtkPolyVertex *polyvertex = vtkPolyVertex::New();
    polyvertex->GetPointIds()->SetNumberOfIds(N);
    for (i = 0; i < N; i++)
    {
        polyvertex->GetPointIds()->SetId(i,i);
    }

    vtkUnstructuredGrid *ugrid = vtkUnstructuredGrid::New();
    ugrid->Allocate(1,1); // proper values for Allocate() method?
    ugrid->InsertNextCell(polyvertex->GetCellType(), polyvertex->GetPointIds());
    ugrid->SetPoints(points);

    vtkDataSetMapper *mapper = vtkDataSetMapper::New();
    mapper->ImmediateModeRenderingOn();
    mapper->SetInput(ugrid);

    vtkActor *actor = vtkActor::New();
    actor->SetMapper(mapper);

    vtkOutlineFilter *outlineData = vtkOutlineFilter::New();
    outlineData->SetInput(ugrid);

    vtkPolyDataMapper *outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(outlineData->GetOutputPort());

    vtkActor *outlineActor = vtkActor::New();
    outlineActor->SetMapper(outlineMapper);
    outlineActor->GetProperty()->SetColor(1,1,1);

    ren->AddActor(actor);
    ren->AddActor(outlineActor);
    ren->ResetCamera();
    ren->GetRenderWindow()->Render();
}

#endif
