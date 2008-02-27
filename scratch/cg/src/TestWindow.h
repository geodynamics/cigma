#ifndef __TEST_WINDOW_H__
#define __TEST_WINDOW_H__

#include <QtGui>
#include "ui_TestWindow.h"

class vtkRenderer;

class TestWindow : public QMainWindow, Ui::TestWindow
{
    Q_OBJECT

//----------------------------------------------------------------------------
public:
    TestWindow(QWidget *parent = 0);
    ~TestWindow();

//----------------------------------------------------------------------------
public slots:
    void fileExit();

//----------------------------------------------------------------------------
public:
    vtkRenderer *ren;
};

#endif
