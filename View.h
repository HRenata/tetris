#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLibrary>
#include <QString>
#include <QDir>
#include <QPluginLoader>
#include <QPen>

#include <../helper_class/helper_class.h>
#include <Game.h>
#include <Figure.h>
#include <ICallbackFigureWatcher.h>
#include <ICallbackGameStateWatcher.h>
#include <interface.h>
#include <ClientStuff.h>

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

    void setVersion(QString version);
    QString getVersion();
    QString getDefaultVersion();
    void downloadFigures();

public slots:
    void animate();
    void handlePushStartButton();
    void handlePushPauseButton();
    void newGame();
    void endGame();
    void aboutGame();
    void applyPlugin();

    void setStatus(bool newStatus);
    void receivedSomething(QString msg);
    void gotError(QAbstractSocket::SocketError err);

private slots:
    void handlePushCheckUpdateButton();
    void handlePushConnectButton();
    void handlePushDisconnectButton();

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
    QPushButton *mConnectButton;
    QPushButton *mDisconnectButton;
    QPushButton *mCheckUpdateButton;
    Figure *mFigure;
    ICallbackFigureWatcher *mFigureMovementListener;
    ICallbackGameStateWatcher *mGameStateListener;

    QVector< Interface* > mPlugins;
    QVector< Figure* > mFigures;

    ClientStuff *mClient;
    QString mVersion;
};
#endif // VIEW_H
