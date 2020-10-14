#include "view.h"

#include <QApplication>
#include <controller.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller *controller = new Controller();

    View w(controller);

    w.show();
    return a.exec();
}
