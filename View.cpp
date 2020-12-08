#include "View.h"
#include "ui_view.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>
#include <QLibrary>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <iostream>

typedef QString (*getWindowTitle)();
typedef void (*about)();

View::View(ICallbackFigureWatcher *figureListener,
           ICallbackGameStateWatcher *gameListener,
           QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
    , mFigureMovementListener(nullptr)
    , mGameStateListener(nullptr)
{
    ui->setupUi(this);    
    this->setFixedSize(540, 635);

    //uploading helper library at run-time
    QLibrary *titleLib = new QLibrary("helper");
    getWindowTitle getWindowTitle_ = (getWindowTitle)titleLib->resolve("getWindowTitle");
    QString windowTitle = getWindowTitle_();
    delete titleLib;

    this->setWindowTitle(windowTitle);

    QAction *newGameAction = new QAction(tr("&New Game"), this);
    connect(newGameAction, SIGNAL(triggered()), this, SLOT(newGame()));
    QAction *exitAction = new QAction(tr("&Exit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(endGame()));
    QAction *aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutGame()));

    QMenu *menu = menuBar()->addMenu("Menu");
    menu->addAction(newGameAction);
    menu->addAction(exitAction);
    menu->addSeparator();
    menu->addAction(aboutAction);

    //plugins
    QMenu *modeMenu = menuBar()->addMenu("Mode");
    QDir *dir;
    #ifdef QT_DEBUG
        dir = new QDir("..\\plugins\\debug");
    #else
        #ifdef QT_RELEASE
           dir = new QDir("..\\plugins\\release");
        #endif
    #endif


    foreach(QString str, dir->entryList(QDir::Files))
    {
        QPluginLoader loader(dir->absoluteFilePath(str));
        QObject *object = qobject_cast<QObject*>(loader.instance());//извлекаем плагин
        Interface *plugin = qobject_cast<Interface*>(object);//приводим к интерфейсу игры
        if(plugin)
        {
            this->mPlugins.push_back(plugin);
            const char * namePlugin = plugin->pluginName().toLocal8Bit().data();
            QAction *applyPlugin = new QAction(tr(namePlugin), this);
            connect(applyPlugin, SIGNAL(triggered()), this, SLOT(applyPlugin()));
            modeMenu->addAction(applyPlugin);
        }
    }

    #ifdef QT_DEBUG
        dir = new QDir("..\\figures\\debug");
    #else
        #ifdef QT_RELEASE
           dir = new QDir("..\\figures\\release");
        #endif
    #endif


    foreach(QString str, dir->entryList(QDir::Files))
    {
        QPluginLoader loader(dir->absoluteFilePath(str));
        QObject *object = qobject_cast<QObject*>(loader.instance()); //извлекаем фигуру
        Figure *figure = qobject_cast<Figure*>(object); //приводим к интерфейсу игры

        if(figure)
        {
            this->mFigures.push_back(figure);
        }
    }
    delete dir;

    this->setFigureMovementListener(figureListener);
    this->setGameStateListener(gameListener);
    this->initializeFigure();

    this->mTimer = new QTimer(this);
    connect(this->mTimer, SIGNAL(timeout()), this, SLOT(animate()));
    this->mTimer->start(1000);

    this->mStartButton = new QPushButton("NEW GAME", this);
    this->mStartButton->setFocusPolicy(Qt::NoFocus);
    this->mStartButton->setGeometry(QRect(QPoint(78, 563), QSize(180, 30)));
    connect(this->mStartButton, SIGNAL (clicked()), this, SLOT (handlePushStartButton()));

    this->mPauseButton = new QPushButton("PAUSE", this);
    this->mPauseButton->setGeometry(QRect(QPoint(278, 563), QSize(180, 30)));
    this->mPauseButton->setFocusPolicy(Qt::NoFocus);
    connect(this->mPauseButton, SIGNAL (clicked()), this, SLOT (handlePushPauseButton()));

    this->mConnectButton = new QPushButton("CONNECT", this);
    this->mConnectButton->setGeometry(QRect(QPoint(78, 600), QSize(381, 30)));
    this->mConnectButton->setFocusPolicy(Qt::NoFocus);
    connect(this->mConnectButton, SIGNAL (clicked()), this, SLOT (handlePushConnectButton()));
    this->mConnectButton->setVisible(true);

    this->mDisconnectButton = new QPushButton("DISCONNECT", this);
    this->mDisconnectButton->setGeometry(QRect(QPoint(78, 600), QSize(180, 30)));
    this->mDisconnectButton->setFocusPolicy(Qt::NoFocus);
    connect(this->mDisconnectButton, SIGNAL (clicked()), this, SLOT (handlePushDisconnectButton()));
    this->mDisconnectButton->setVisible(false);

    this->mCheckUpdateButton = new QPushButton("CHECK UPDATE", this);
    this->mCheckUpdateButton->setGeometry(QRect(QPoint(278, 600), QSize(180, 30)));
    this->mCheckUpdateButton->setFocusPolicy(Qt::NoFocus);
    connect(this->mCheckUpdateButton, SIGNAL (clicked()), this, SLOT (handlePushCheckUpdateButton()));
    this->mCheckUpdateButton->setVisible(false);

    this->mClient = new ClientStuff("localhost", 6547);

    setStatus(this->mClient->getStatus());

    connect(this->mClient, &ClientStuff::hasReadSome, this, &View::receivedSomething);
    connect(this->mClient, &ClientStuff::statusChanged, this, &View::setStatus);
    connect(this->mClient->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(gotError(QAbstractSocket::SocketError)));

    this->setVersion(this->getDefaultVersion());
}

void View::setStatus(bool newStatus)
{
    if(newStatus)
    {
        this->mConnectButton->setVisible(false);
        this->mDisconnectButton->setVisible(true);
        this->mCheckUpdateButton->setVisible(true);
    }
    else
    {
        this->mConnectButton->setVisible(true);
        this->mDisconnectButton->setVisible(false);
        this->mCheckUpdateButton->setVisible(false);
    }
}

void View::receivedSomething(QString msg)
{
    if(msg.contains("version:"))
    {
        QString version = msg.remove(0, 8);

        QMessageBox msgBox;
        if(version == this->mVersion)
        {
            msgBox.setText("No available updates");
            msgBox.exec();
        }
        else
        {
            msgBox.setText("New version " + version + " is available");
            msgBox.setInformativeText("Do you want to update app?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int res = msgBox.exec();

            if (res == QMessageBox::Yes)
            {
                this->setVersion(version);
                QByteArray arrBlock;
                QDataStream out(&arrBlock, QIODevice::WriteOnly);
                QString answer = "OK ON UPDATE";
                out << quint16(0) << answer;

                out.device()->seek(0);
                out << quint16(arrBlock.size() - sizeof(quint16));

                this->mClient->tcpSocket->write(arrBlock);
            }
        }
    }
    //не нужно
    else if(msg.contains("update:"))
    {
        QString version = msg.remove(0, 7);
        this->setVersion(version);
    }
}

void View::gotError(QAbstractSocket::SocketError err)
{
    QMessageBox msgBox;
    QString strError = "unknown";
    switch (err)
    {
        case 0:
            strError = "Connection was refused";
            break;
        case 1:
            strError = "Remote host closed the connection";
            break;
        case 2:
            strError = "Host address was not found";
            break;
        case 5:
            strError = "Connection timed out";
            break;
        default:
            strError = "Unknown error";
    }
    msgBox.setText(strError);
    msgBox.exec();
}

void View::handlePushConnectButton()
{
    this->mClient->connect2host();
}

void View::handlePushCheckUpdateButton()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0) << "version:" + this->mVersion;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    this->mClient->tcpSocket->write(arrBlock);
}

void View::handlePushDisconnectButton()
{
    this->mClient->closeConnection();
}


void View::newGame()
{
    this->mGameStateListener->startGame();
    this->initializeFigure();
    Game::mGameIsPaused = false;
    this->animate();
}

void View::endGame()
{
    this->close();
}

void View::aboutGame()
{
    //uploading about library at run-time
    QLibrary *aboutLib = new QLibrary("about");
    about showWindowAboutProgramm = (about)aboutLib->resolve("about");
    showWindowAboutProgramm();
    delete aboutLib;
}

void View::applyPlugin()
{
    int subMode = 0;
    QAction *action = qobject_cast<QAction *>(sender());
    for(int i = 0; i < this->mPlugins.size(); ++i)
    {
        if(action->text() == this->mPlugins[i]->pluginName())
        {
            subMode = i;
        }
    }

    QPalette palette = this->mPlugins[subMode]->changeView();
    this->setPalette(palette);
    this->repaint();
}

void View::setFigureMovementListener(ICallbackFigureWatcher *listener)
{
    this->mFigureMovementListener = listener;
}

void View::setGameStateListener(ICallbackGameStateWatcher *listener)
{
    this->mGameStateListener = listener;
}

void View::initializeFigure()
{
    srand(time(0));
    int rand_ = rand() % this->mFigures.size();
    this->mFigure = this->mFigures[rand_];
    this->mFigure->reInitialize();

    rand_ = 1 + rand() % 4;
    for(int i = 0; i < rand_; ++i)
    {
        this->mFigureMovementListener->rotation(this->mFigure);
    }

    if(this->mFigureMovementListener->hasCollisions(this->mFigure))
    {
        this->mGameStateListener->endGame();
    }
}

QString View::getDefaultVersion()
{
    QString val;
    QFile file;
    file.setFileName("client.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QString version = json["version"].toString();

    return version;
}

void View::setVersion(QString version)
{
    QJsonObject recordObject;
    recordObject.insert("version", QJsonValue::fromVariant(version));
    QJsonDocument doc(recordObject);
    QString jsonString = doc.toJson(QJsonDocument::Indented);

    QFile file;
    file.setFileName("client.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream( &file );
    stream << jsonString;
    file.close();

    this->mVersion = version;
}

QString View::getVersion()
{
    return this->mVersion;
}

void View::animate()
{
    if(!Game::mGameIsPaused)
    {
        repaint();

        if(Game::mGameIsActive)
        {
            if(!this->mFigureMovementListener->movementDown(this->mFigure))
            {
                this->mFigureMovementListener->lockFigure(this->mFigure);
                this->initializeFigure();
            }

            this->mFigureMovementListener->deleteFilledRows();
        }
    }
}

void View::handlePushStartButton()
{
    this->newGame();
}

void View::handlePushPauseButton()
{
    if(Game::mGameIsPaused)
    {
        this->mGameStateListener->continueGame();
    }
    else
    {
        this->mGameStateListener->pauseGame();
    }
}

void View::keyPressEvent(QKeyEvent *e)
{
    if(Game::mGameIsActive)
    {
        switch (e->key())
        {
        case Qt::Key_Up:
            if(!this->mFigureMovementListener->rotation(this->mFigure))
            {
                this->mFigureMovementListener->lockFigure(this->mFigure);
                this->initializeFigure();
            }
            else
            {
                repaint();
            }
            break;
        case Qt::Key_Down:
            this->animate();
            break;
        case Qt::Key_Left:
            if(!this->mFigureMovementListener->movementLeft(this->mFigure))
            {
                this->mFigureMovementListener->lockFigure(this->mFigure);
                this->initializeFigure();
            }
            else
            {
                repaint();
            }
            break;
        case Qt::Key_Right:
            if(!this->mFigureMovementListener->movementRight(this->mFigure))
            {
                this->mFigureMovementListener->lockFigure(this->mFigure);
                this->initializeFigure();
            }
            else
            {
                repaint();
            }
            break;
        case Qt::Key_Space:
            this->handlePushPauseButton();
            break;
        }
    }
}

void View::paintEvent(QPaintEvent *event)
{
    QPainter Painter(this);
    Painter.setFont(QFont("Arial", 18));
    QString result = QString::asprintf("Score : %d", Game::mScore);
    Painter.drawText(QRect(320, 20, 140, 30), Qt::AlignRight, result);

    this->paintMap(Painter);

    if(Game::mGameIsActive)
    {
        this->paintFigure(Painter);
    }
    else if(!Game::mGameIsActive
            && this->mFigureMovementListener->hasCollisions(this->mFigure))
    {
        Painter.fillRect(QRect(80, 245, 15 * 25, 90), QBrush(Qt::magenta));
        result = QString::asprintf("GAME OVER\nScore : %d", Game::mScore);
        Painter.drawText(QRect(80, 245, 15 * 25, 90), Qt::AlignCenter, result);
    }
}

void View::paintMap(QPainter &Painter)
{
    for(int i = 0; i < Map::mN; ++i)
    {
        for(int j = 0; j < Map::mM; ++j)
        {
            QRect el(Map::mOffsetX + i*Map::mCellWidth,
                     Map::mOffsetY + j*Map::mCellWidth,
                     Map::mCellWidth, Map::mCellHeight);
            switch(Map::mMap[i][j])
            {
            case 1:
                Painter.fillRect(el, QBrush(Qt::red));
                break;
            case 2:
                Painter.fillRect(el, QBrush(Qt::green));
                break;
            case 3:
                Painter.fillRect(el, QBrush(Qt::blue));
                break;
            }
            Painter.drawRect(el);
        }
    }
}

void View::paintFigure(QPainter &Painter)
{
    int **figureArr = this->mFigure->getFigure();

    for(int i = 0; i < this->mFigure->getN(); ++i)
    {
        for(int j = 0; j < this->mFigure->getM(); ++j)
        {
            QRect el(this->mFigure->getOffsetX() + j*Map::mCellHeight,
                     this->mFigure->getOffsetY() + i*Map::mCellWidth,
                     Map::mCellWidth, Map::mCellHeight);

            if(figureArr[i][j] != 0)
            {
                Painter.fillRect(el, QBrush(this->mFigure->getColor()));
                Painter.drawRect(el);
            }
        }
    }
}

View::~View()
{
    delete this->mFigureMovementListener;
    delete this->mGameStateListener;
    delete this->mFigure;
    delete this->mStartButton;
    delete this->mPauseButton;
    delete this->mConnectButton;
    delete this->mDisconnectButton;
    delete this->mCheckUpdateButton;
    delete this->mTimer;
    delete this->mClient;
    delete this->ui;
}

