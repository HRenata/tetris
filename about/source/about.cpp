#include "about.h"
#include <QMessageBox>

void about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About programm");
    msgBox.setText("The programm was developed by Hauryliuk Renata\ncourse : 2\ngroup: PO-3\n2020");
    msgBox.exec();
}
