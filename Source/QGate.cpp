#include "QGate.h"

#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QFontMetrics>
#include <QPainter>

#include "QConnection.h"

QGate::QGate(QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    this->setAcceptHoverEvents(true);
    set_parent(parent);
    QPainterPath p;
    p.addEllipse(0, 0, radius, radius);
    setPath(p);
    setPen(QPen(Qt::black));
    setBrush(Qt::black);
    //setFlag(QGraphicsItem::ItemIsSelectable);

}
QVector<QConnection *>& QGate::connections()
{
    return this->conn_list;
}

#include <QStyleOptionGraphicsItem>

void QGate::set_parent(QGraphicsItem *new_parent)
{
    this->parent = new_parent;
}
QGraphicsItem *QGate::get_parent() const
{
    return this->parent;
}
bool QGate::isConnected(QGate *g)
{
    foreach(QConnection *conn, conn_list)
        if (conn->src_gate() == g || conn->des_gate() == g)
            return true;

    return false;
}
void QGate::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    QColor green70 = Qt::green;
    green70.setAlphaF( 0.7 );
    setPen(QPen(green70, 2));
    setBrush(green70);
    QPainterPath p;
    p.addEllipse(-3 ,-3 ,6 , 6);
    setPath(p);
    update();
}
void QGate::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    setPen(QPen(Qt::black, 2));
    setBrush(Qt::black);
    QPainterPath p;
    p.addEllipse(0 ,0 ,radius ,radius);
    setPath(p);
    update();
}

qreal QGate::get_radius()
{
    return radius;
}

QPainterPath QGate::shape() const
{
    QPainterPath path;
    path.addEllipse(-5, -5, 10, 10);
    return path;
}
