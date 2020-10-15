#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>

#include <Map.h>
#include <FigureO.h>
#include <FigureZ.h>
#include <FigureL.h>
#include <ICallbackFigureWatcher.h>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(ICallbackFigureWatcher *listener, QWidget *parent = nullptr);
    ~View();
    void setFigureMovementListener(ICallbackFigureWatcher *listener);
    void initializeFigure();

public slots:
    void animate();

protected:
    void keyPressEvent (QKeyEvent *e) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::View *ui;
    Figure *mFigure;
    ICallbackFigureWatcher *mFigureMovementListener;
};
#endif // VIEW_H
