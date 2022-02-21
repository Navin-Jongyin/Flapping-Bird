/*
ID: 63011210
Name: Navin Jongyin
*/
#include "pillaritem.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QGraphicsScene>
#include "scene.h"
#include "birditem.h"

PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap(":/Images/TopPipe.png"))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap(":/Images/BottomPipe.png"))), pastBird(false)
{
    topPillar->setPos(QPointF(0,0) - QPointF(topPillar->boundingRect().width()/2, topPillar->boundingRect().height() + 60));
    bottomPillar->setPos(QPointF(0,0) + QPointF(-bottomPillar->boundingRect().width()/2, 60));
    addToGroup(topPillar);
    addToGroup(bottomPillar);

    yPos = QRandomGenerator::global()->bounded(150);
    int xRandomizer = QRandomGenerator::global()->bounded(200);

    setPos(QPointF(0, 0) + QPoint(260 + xRandomizer, yPos));

    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(-260);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(1500);

    connect(xAnimation, &QPropertyAnimation::finished,[=]()
    {
        qDebug()<< "Animation Finsish";
        scene()->removeItem(this);
        delete this;
    });

    xAnimation->start();


}

PillarItem::~PillarItem()
{
    qDebug() << "Deleting Pillar";
    delete topPillar;
    delete bottomPillar;
}

qreal PillarItem::x() const
{
    return m_x;
}

void PillarItem::freezeInPlace()
{
    xAnimation->stop();
}

void PillarItem::setX(qreal x)
{
    m_x = x;
    if(x < 0 && !pastBird)
    {
        pastBird = true;
        QGraphicsScene * mScene = scene();
        Scene * myScene = dynamic_cast<Scene*>(mScene);
        if(myScene)
        {
            myScene->incrementScore();
        }
    }



    if(collideWithBird())
    {
        emit collideFail();
    }
    setPos(QPointF(0, 0) + QPointF(x, yPos));
}

bool PillarItem::collideWithBird()
{
    QList<QGraphicsItem*> collidingItems = topPillar->collidingItems();
    collidingItems.append(bottomPillar->collidingItems());

    foreach(QGraphicsItem *item, collidingItems)
    {
        BirdItem * birdItem = dynamic_cast<BirdItem*>(item);
        if(birdItem)
        {
            return true;
        }
    }
    return false;
}
