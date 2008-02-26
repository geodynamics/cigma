#ifndef __ORIENTATION_AXES_H__
#define __ORIENTATION_AXES_H__

#include <cassert>
#include "QVTKWidget.h"

#include "vtkAxesActor.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkRenderWindowInteractor.h"

void AddOrientationAxes(QVTKWidget *qvtkWidget)
{
    assert(qvtkWidget != 0);

    vtkRenderWindowInteractor *interactor = qvtkWidget->GetInteractor();

    vtkAxesActor *axesActor = vtkAxesActor::New();
    axesActor->SetNormalizedTipLength(0.4, 0.4, 0.4);
    axesActor->SetNormalizedShaftLength(0.6, 0.6, 0.6);
    axesActor->SetShaftTypeToCylinder();

    vtkOrientationMarkerWidget *marker = vtkOrientationMarkerWidget::New();
    marker->SetInteractor(interactor);
    marker->SetOrientationMarker(axesActor);
    marker->SetEnabled(true);
    //marker->SetViewPort(0.0, 0.8, 0.2, 0.10);
    marker->InteractiveOn();
}

#endif
