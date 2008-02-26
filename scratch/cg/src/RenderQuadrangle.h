#ifndef __RENDER_QUADRANGLE_H__
#define __RENDER_QUADRANGLE_H__

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkUnstructuredGrid.h"

#include "vtkOutlineFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"

#include "vtkQuad.h"


/**
 * Render a quadrangle cell onto a render object.
 * @param ren    parent vtkRenderer object
 * @param coords TBA
 * @param dofs   TBA
 */
void RenderQuadrangle(vtkRenderer *ren)
{
    assert(ren != 0);

    int i;
    const int nno = 4;
    float pts[nno][3] = {
        {-1.0, -1.0, 0.0},
        {+1.0, -1.0, 0.0},
        {+1.0, +1.0, 0.0},
        {-1.0, +1.0, 0.0}
    };

    vtkFloatArray *coords = vtkFloatArray::New();
    coords->SetNumberOfComponents(3);
    coords->SetNumberOfTuples(nno);
    for (i = 0; i < nno; i++)
    {
        coords->SetTuple(i, pts[i]);
    }

    vtkPoints *points = vtkPoints::New();
    points->SetData(coords);

    vtkQuad *quad = vtkQuad::New();
    for (i = 0; i < nno; i++)
    {
        quad->GetPointIds()->SetId(i, i);
    }

    double dofs[nno] = {2.7, 4.1, 5.3, 3.4};
    vtkDoubleArray *phi = vtkDoubleArray::New();
    phi->SetName("phi");
    for (i = 0; i < nno; i++)
    {
        phi->InsertNextValue(dofs[i]);
    }

    vtkUnstructuredGrid *ugrid = vtkUnstructuredGrid::New();
    ugrid->Allocate(1,1);
    ugrid->InsertNextCell(quad->GetCellType(), quad->GetPointIds());
    ugrid->SetPoints(points);
    ugrid->GetPointData()->SetScalars(phi);

    vtkDataSetMapper *mapper = vtkDataSetMapper::New();
    mapper->ImmediateModeRenderingOn();
    mapper->SetInput(ugrid);
    mapper->SetScalarRange(2.5, 5.5);

    vtkActor *actor = vtkActor::New();
    actor->SetMapper(mapper);
    const bool showWireMesh = false;
    if (showWireMesh)
    {
        mapper->ScalarVisibilityOff();
        actor->GetProperty()->SetRepresentationToWireframe();
        actor->GetProperty()->SetLineWidth(2);
    }
    ren->AddActor(actor);


    /* Add an Outline Filter

    vtkOutlineFilter *outlineData = vtkOutlineFilter::New();
    outlineData->SetInput(ugrid);

    vtkPolyDataMapper *outlineMapper = vtkPolyDataMapper::New();
    outlineMapper->SetInputConnection(outlineData->GetOutputPort());

    vtkActor *outlineActor = vtkActor::New();
    outlineActor->SetMapper(outlineMapper);
    outlineActor->GetProperty()->SetColor(1,1,1);
    
    ren->AddActor(outlineActor);

    // */


    ren->ResetCamera();
    ren->GetRenderWindow()->Render();
}

#endif
