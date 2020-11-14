#ifndef INTERFACE_H
#define INTERFACE_H

#include <QString>
#include <QPalette>

class Interface
{
public:
    virtual ~Interface(){}
    virtual QString pluginName() = 0;//плагин должен возвращать свое имя
    virtual QPalette changeView() = 0;//плагин выполняет действия над объектами в структуре Styles
};

Q_DECLARE_INTERFACE(Interface, "Tetris.Interface");

#endif // INTERFACE_H
