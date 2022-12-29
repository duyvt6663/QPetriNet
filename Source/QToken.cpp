#include "QToken.h"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>

QToken::QToken(QGraphicsItem *parent):
    QGraphicsPathItem(parent)
{
    this->radius = 2.0;
    QPainterPath p;
    setBrush(Qt::black);
    p.addEllipse(0, 0, 2*radius, 2*radius);
    setPath(p);
}

void QToken::scale(qreal factor)
{
    this->radius *= factor;
    setScale(factor);
    prepareGeometryChange();
    update(boundingRect());
}

void QToken::setGeometry(const QRectF rect)
{
    geo = rect;
    setPos(rect.topLeft());
}

/*
#include <QStyleOptionGraphicsItem>
void QToken::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing, true);
    QBrush brush(Qt::black);
    brush.setStyle(Qt::SolidPattern);
    painter->fillPath(path(), brush);
}
*/
