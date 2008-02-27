#include <QApplication>
#include "TestWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestWindow *win = new TestWindow;
    win->show();
    return app.exec();
}
