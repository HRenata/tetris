#include "View.h"
#include "ui_view.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>

View::View(ICallbackFigureWatcher *figureListener,
           ICallbackGameStateWatcher *gameListener,
           QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
    , mFigureMovementListener(nullptr)
    , mGameStateListener(nullptr)
{
    ui->setupUi(this);    
    this->setFixedSize(540, 605);

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
    QMessageBox::information(0, "Information", "Operation Complete");
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
    int rand_ = 1 + rand() % 3;

    if(rand_ == 3)
    {
        this->mFigure = new FigureO();
    }
    else if(rand_ == 2)
    {
        this->mFigure = new FigureL();
    }
    else
    {
        this->mFigure = new FigureZ();
    }

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

            this->mFigureMovementListener->deleteFilledRows(this->mFigure);
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

            switch(figureArr[i][j])
            {
            case 1:
                Painter.fillRect(el, QBrush(Qt::red));
                Painter.drawRect(el);
                break;
            case 2:
                Painter.fillRect(el, QBrush(Qt::green));
                Painter.drawRect(el);
                break;
            case 3:
                Painter.fillRect(el, QBrush(Qt::blue));
                Painter.drawRect(el);
                break;
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
    delete this->mTimer;
    delete ui;
}

