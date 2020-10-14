#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>

#include <map.h>
#include <figureo.h>
#include <figurez.h>
#include <figurel.h>
#include <icallbacklistener.h>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    ~View();
    void setFigureMovementListener(ICallbackListener *listener);

public slots:
    void animate();

protected:
    void keyPressEvent (QKeyEvent *e) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::View *ui;
    Figure *mFigure;
    ICallbackListener *mFigureMovementListener;
};
#endif // VIEW_H
