#include "windows/CG_CubicGameEngineWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CubicGameEngineWindow cubicGameEngineWindow;
    cubicGameEngineWindow.show();

    return a.exec();
}
