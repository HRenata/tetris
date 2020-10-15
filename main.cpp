#include "View.h"

#include <QApplication>
#include <Controller.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller *controller = new Controller();

    View w(controller);

    w.show();
    return a.exec();
}
