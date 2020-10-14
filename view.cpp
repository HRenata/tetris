#include "view.h"
#include "ui_view.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

#include <iostream>
#include <windows.h>
View::View(ICallbackListener *listener, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
    , mFigureMovementListener(nullptr)
{
    ui->setupUi(this);

    this->setFigureMovementListener(listener);
    this->initializeFigure();

    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(1500);
}

void View::setFigureMovementListener(ICallbackListener *listener)
{
    this->mFigureMovementListener = listener;
}

void View::initializeFigure()
{
    srand(time(0));
    int  i = rand() % 3 + 1;
    switch (i)
    {
    case 1:
        this->mFigure = new FigureO();
        break;
    case 2:
        this->mFigure = new FigureZ();
        break;
    case 3:
        this->mFigure = new FigureL();
        break;
    }

    if(this->mFigureMovementListener->hasCollisions(this->mFigure))
    {
        std::cout << "END" << std::endl;
    }
}

void View::animate() {
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
        break;
    case Qt::Key_Down:
        if(!this->mFigureMovementListener->movementDown(this->mFigure))
        {
            this->mFigureMovementListener->lockFigure(this->mFigure);
            this->initializeFigure();
        }
        break;
    case Qt::Key_Left:
        if(!this->mFigureMovementListener->movementLeft(this->mFigure))
        {
            this->mFigureMovementListener->lockFigure(this->mFigure);
            this->initializeFigure();
        }
        break;
    case Qt::Key_Right:
        if(!this->mFigureMovementListener->movementRight(this->mFigure))
        {
            this->mFigureMovementListener->lockFigure(this->mFigure);
            this->initializeFigure();
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

   QPainter Painter(this);

   //drawing map
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

   //drawing figure
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
    delete this->mFigure;
    delete ui;
}

