#include "QTransition.h"
#include "QToken.h"
#include "QGate.h"
#include "QConnection.h"
#include "QPlace.h"
#include "QDocument.h"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <QCursor>
#include <QEventLoop>
#include <QSignalMapper>

QTransition::QTransition(QGraphicsItem *parent, int num_of_token):
    QGraphicsPathItem(parent), token_count(num_of_token)
{
    label = new QGraphicsTextItem(this);
    sideLen = 40;

    QPainterPath p;
    setBrush(Qt::white);
    p.addRect(-sideLen/2, -sideLen/2, sideLen, sideLen);
    setPath(p);
    setPen(QPen(Qt::black, 1.2));
    setFlag(QGraphicsItem::ItemIsMovable);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);

    set_gate();
    busy = false;
}

void QTransition::set_gate()
{
    for(int i = 0; i < 8; i++)
    {
        QGate *b = new QGate(this);
        b->setPos(sideLen/2 * qCos(i * M_PI_2) - b->get_radius(),
                  sideLen/2 * qSin(i * M_PI_2) - b->get_radius());
        gate.append(b);
    }
}
QVariant QTransition::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange && scene())
    {
        foreach(QGate *g, gate)
            foreach(QConnection *conn, g->connections())
                conn->update_path();
    }
    return value;
}
void QTransition::setName(const QString &name)
{
    if(!QDocument::isValid(name))
    {
        emit invalidName(this);
        return;
    }
    emit nameChanged(this->name, name);
    this->name = name;
    label->setPlainText(name);
    label->setFlag(QGraphicsItem::ItemIsMovable);
    label->setFlag(QGraphicsItem::ItemIsSelectable);
    QFontMetrics fm(scene()->font());
    QRect r = fm.boundingRect(name);
    label->setPos(-r.width()/2 - 5, -r.height() - 5);
}
QTransition::~QTransition()
{
    foreach(QGate *g, gate)
    {
        foreach(QConnection *conn, g->connections())
            delete conn;
        delete g;
    }
}
bool QTransition::isEnabled() const
{
    bool flag1 = false, flag2 = false;
    foreach(QGate *g, gate)
        foreach(QConnection *conn, g->connections())
        {
            QGraphicsItem *p = conn->src_gate()->get_parent();
            if(p->type() == QPlace::Type)
            {
                flag1 = true;
                if(((QPlace*)p)->token() <= 0)
                    return false;
            }
            else flag2 = true;
        }
    return flag1 and flag2 and !busy;
}
void QTransition::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(isEnabled())
        setCursor(Qt::PointingHandCursor);
    QGraphicsPathItem::hoverEnterEvent(event);
}

void QTransition::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    unsetCursor();
    QGraphicsPathItem::hoverLeaveEvent(event);
}

void QTransition::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(!isEnabled())
        unsetCursor();
    QGraphicsPathItem::hoverMoveEvent(event);
}
void QTransition::fire()
{
    QSignalMapper *map = new QSignalMapper(this);
    foreach(QGate *g, gate)
    {
        foreach(QConnection *conn, g->connections())
        {
            QGate *p = conn->src_gate();
            if(p != g)
            {
                connect(conn, &QConnection::isFiredIn,
                        map,  static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map),
                        Qt::QueuedConnection);
                map->setMapping(conn, rand()%100);
                ((QPlace*)p->get_parent())->shoot();
                conn->fire(true);
                busy = true;
            }
        }
    }
    QEventLoop loop;
    connect(map,   &QSignalMapper::mappedInt,
            &loop, &QEventLoop::quit);
    loop.exec();
    busy = false;
    foreach(QGate *g, gate)
    {
        foreach(QConnection *conn, g->connections())
        {
            QGate *p = conn->src_gate();
            if(p == g)
            {
                QPlace *des = (QPlace*)conn->des_gate()->get_parent();
                connect(conn, &QConnection::isFiredOut,
                        des,  &QPlace::receive        ,
                        Qt::SingleShotConnection      );
                conn->fire(false);
            }
        }
    }
}
