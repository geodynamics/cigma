#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QtGui>
#include "ui_MainWindow.h"

class vtkRenderer;


class MainWindow : public QMainWindow, Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void fileExit();

public:
    vtkRenderer *ren;
};

#endif
