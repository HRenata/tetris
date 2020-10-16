#include "View.h"
#include "ui_view.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

#include <random>

View::View(ICallbackFigureWatcher *figureListener,
           ICallbackGameStateWatcher *gameListener,
           QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
    , mFigureMovementListener(nullptr)
    , mGameStateListener(nullptr)
{
    ui->setupUi(this);

    this->setFigureMovementListener(figureListener);
    this->setGameStateListener(gameListener);
    this->initializeFigure();

    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));

    timer->start(2000);
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
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> randomFigure(1, Game::mNumberFigures);
    std::uniform_int_distribution<> randomRotation(1, 4);
    int rand = randomFigure(gen);

    if(rand == 3)
    {
        this->mFigure = new FigureO();
    }
    else if(rand == 2)
    {
        this->mFigure = new FigureL();
    }
    else
    {
        this->mFigure = new FigureZ();
    }

    rand = randomRotation(gen);
    for(int i = 0; i < rand; ++i)
    {
        this->mFigureMovementListener->rotation(this->mFigure);
    }

    if(this->mFigureMovementListener->hasCollisions(this->mFigure))
    {
        //game over
        Game::mGameIsActive = false;
    }
}

void View::animate()
{
    repaint();
}

void View::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Up:
        if(!this->mFigureMovementListener->rotation(this->mFigure))
        {
            this->mFigureMovementListener->lockFigure(this->mFigure);
            this->initializeFigure();
        }
        else
        {
            this->animate();
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
            this->animate();
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
            this->animate();
        }
        break;
    }
}

void View::paintEvent(QPaintEvent *event)
{
   if(!this->mFigureMovementListener->movementDown(this->mFigure))
   {
       this->mFigureMovementListener->lockFigure(this->mFigure);
       this->initializeFigure();
   }

   this->mFigureMovementListener->deleteFilledRows(this->mFigure);

   this->paintMap();
   this->paintFigure();
}

void View::paintMap()
{
    QPainter Painter(this);
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

void View::paintFigure()
{
    QPainter Painter(this);
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
    delete ui;
}

