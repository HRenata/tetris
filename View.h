#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLibrary>
#include <QDir>
#include <QPluginLoader>
#include <QPen>

#include <../helper_class/helper_class.h>
#include <Game.h>
#include <FigureO.h>
#include <FigureZ.h>
#include <FigureL.h>
#include <ICallbackFigureWatcher.h>
#include <ICallbackGameStateWatcher.h>
#include <interface.h>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(ICallbackFigureWatcher *figureListener,
         ICallbackGameStateWatcher *gameListener,
         QWidget *parent = nullptr);
    ~View();
    void setFigureMovementListener(ICallbackFigureWatcher *listener);
    void setGameStateListener(ICallbackGameStateWatcher *listener);
    void initializeFigure();

public slots:
    void animate();
    void handlePushStartButton();
    void handlePushPauseButton();
    void newGame();
    void endGame();
    void aboutGame();
    void applyPlugin();

protected:
    void keyPressEvent (QKeyEvent *e) override;
    void paintEvent(QPaintEvent *event) override;
    void paintMap(QPainter &Painter);
    void paintFigure(QPainter &Painter);

private:
    Ui::View *ui;
    QTimer *mTimer;
    QPushButton *mStartButton;
    QPushButton *mPauseButton;
    Figure *mFigure;
    ICallbackFigureWatcher *mFigureMovementListener;
    ICallbackGameStateWatcher *mGameStateListener;

    QVector< Interface* > plugins;
};
#endif // VIEW_H
