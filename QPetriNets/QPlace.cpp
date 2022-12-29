#include "QPlace.h"
#include "QToken.h"
#include "QGate.h"
#include "QConnection.h"
#include "QDocument.h"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <QMutex>

QPlace::QPlace(QGraphicsItem *parent, int numTok)
    : QGraphicsPathItem(parent), tokCount(numTok)
{
    label = new QGraphicsTextItem(this);
    tokLabel = new QGraphicsTextItem(this);
    radius = 20;
    QPainterPath p;
    p.addEllipse(-radius, -radius, 2*radius, 2*radius);
    setPath(p);
    setPen(QPen(Qt::black, 1.2));
    setBrush(Qt::white);
    setFlag(QGraphicsItem::ItemIsMovable);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    setGate();
    setTok(tokCount);
}

void QPlace::setGate()
{
    for(int i = 0; i < 8; i++)
    {
        QGate *b = new QGate(this);
        b->setPos(radius * qCos(i * M_PI_4) - b->get_radius(),
                  radius * qSin(i * M_PI_4) - b->get_radius());
        gate.append(b);
    }
}
QVariant QPlace::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange && scene())
    {
        foreach(QGate *g, gate)
            foreach(QConnection *conn, g->connections())
                conn->update_path();
    }
    return value;
}
QPlace::~QPlace()
{
    foreach(QGate *g, gate)
    {
        foreach(QConnection *conn, g->connections())
            delete conn;
        delete g;
    }
}
void QPlace::setTok(int num_ofTok)
{
    tokLabel->setPlainText("");
    while(!tokList.empty())
    {
        tokList[0]->scale(0);
        tokList.pop_front();
    }
    this->tokCount = num_ofTok;
    switch(tokCount)
    {
    case 0:
        break;
    case 1:
    {
        tokList.append(new QToken(this));
        tokList[0]->scale(3);
        tokList[0]->setPos(-tokList[0]->getRadius(), -tokList[0]->getRadius());
        break;
    }
    case 2:
    {
        for(int i = 0; i < tokCount; i++)
        {
            tokList.append(new QToken(this));
            tokList[i]->scale(2);
            tokList[i]->setPos(-tokList[i]->getRadius() + 6 * qCos(i * M_PI),
                               -tokList[i]->getRadius());
        }
        break;
    }
    case 3:
    {
        for(int i = 0; i < tokCount; i++)
        {
            tokList.append(new QToken(this));
            tokList[i]->scale(1.8);
            tokList[i]->setPos(-tokList[i]->getRadius() + 6 * qCos(-M_PI_2 + i * 4.0/3.0 * M_PI_2),
                               -tokList[i]->getRadius() + 6 * qSin(-M_PI_2 + i * 4.0/3.0 * M_PI_2));
        }
        break;
    }
    default:
    {
        QString str = QString::number(tokCount);
        tokLabel->setPlainText(str);
        QFontMetrics fm(scene()->font());
        QRect r = fm.boundingRect(str);
        tokLabel->setPos(-r.width()/2 - 4, -r.height()+4);
        break;
    }
    }
}

int QPlace::token() const
{
    return this->tokCount;
}

void QPlace::setName(const QString &name)
{
    if(name != this->name && !QDocument::isValid(name))
    {
        emit invalidName(this);
        return;
    }
    emit nameChanged(this, name);
    this->name = name;
    label->setPlainText("");
    label->setFlag(QGraphicsItem::ItemIsSelectable);
    label->setFlag(QGraphicsItem::ItemIsMovable);
    label->setPlainText(name);
    QFontMetrics fm(scene()->font());
    QRect r = fm.boundingRect(name);
    label->setPos(-r.width()/2 - 4, radius - 3);
}
