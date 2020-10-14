#include "view.h"
#include "ui_view.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

#include <iostream>
View::View(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
    , mFigureMovementListener(nullptr)
{
    ui->setupUi(this);


    this->mFigure = new FigureL();
    Map::mMap[0][0] = 1;
    Map::mMap[4][9] = 1;
    Map::mMap[9][2] = 1;
    Map::mMap[7][5] = 1;

    auto timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(1000);
}

void View::setFigureMovementListener(ICallbackListener *listener)
{
    this->mFigureMovementListener = listener;
}

void View::animate() {
    repaint();
}

void View::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Up:
        this->mFigureMovementListener->rotation(this->mFigure);
        break;
    case Qt::Key_Down:
        this->mFigureMovementListener->movementDown(this->mFigure);
        break;
    case Qt::Key_Left:
        this->mFigureMovementListener->movementLeft(this->mFigure);
        break;
    case Qt::Key_Right:
        this->mFigureMovementListener->movementRight(this->mFigure);
        break;
    }
}

void View::paintEvent(QPaintEvent *event)
{
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
   this->mFigureMovementListener->movementDown(this->mFigure);
}

View::~View()
{
    delete this->mFigureMovementListener;
    delete this->mFigure;
    delete ui;
}

