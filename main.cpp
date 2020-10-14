#include "view.h"

#include <QApplication>
#include <controller.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;

    Controller *controller = new Controller();
    w.setFigureMovementListener(controller);

    w.show();
    return a.exec();
}
