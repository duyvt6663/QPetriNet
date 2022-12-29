#include "QConnection.h"
#include "QGate.h"
#include "QToken.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include <QPainter>


QConnection::QConnection(QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
    src = des = nullptr;
    setPen(QPen(Qt::black));
    setBrush(Qt::black);
    setZValue(-1);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QConnection::~QConnection()
{
    if (src != nullptr)
        src->connections().remove(src->connections().indexOf(this));
    if (des != nullptr)
        des->connections().remove(des->connections().indexOf(this));
}
void QConnection::set_pos_src(const QPointF &psrc)
{
    this->pos_src = psrc;
}
void QConnection::set_pos_des(const QPointF &pdes)
{
    this->pos_des = pdes;
}
void QConnection::set_src(QGate *src)
{
    this->src = src;
    this->src->connections().append(this);
}
void QConnection::set_des(QGate *des)
{
    this->des = des;
    this->des->connections().append(this);
}
void QConnection::scene_update_pos()
{
    if(this->src != nullptr)
        this->pos_src = this->src->scenePos();
    if(this->des != nullptr)
        this->pos_des = this->des->scenePos();
}
void QConnection::update_path()
{
    scene_update_pos();
    QPainterPath p;
    p.moveTo(pos_src);
    p.lineTo(pos_des);
    // draw an arrow
    QLineF line(pos_src, pos_des);
    QPointF v1, v2;
    double angle = std::atan2(-line.dy(), line.dx());
    v1 = pos_des + QPointF(sin(angle - M_PI / 1.8) * qMin(10.0, line.length()),
                           cos(angle - M_PI / 1.8) * qMin(10.0, line.length()));
    v2 = pos_des + QPointF(sin(angle - M_PI + M_PI / 1.8) * qMin(10.0, line.length()),
                           cos(angle - M_PI + M_PI / 1.8) * qMin(10.0, line.length()));
    p.addPolygon(QPolygonF() << pos_des << v1 << v2 << pos_des);

    setPath(p);
}
QGate * QConnection::src_gate() const
{
    return this->src;
}

QGate* QConnection::des_gate() const
{
    return this->des;
}

void QConnection::fire(bool isFiringIn)
{
    this->isFiringIn = isFiringIn;

    // set animation path
    QPainterPath p;
    QColor cyan70 = Qt::cyan;
    cyan70.setAlphaF(0.7);
    setPen(QPen(cyan70));
    p.moveTo(pos_src);
    p.lineTo(pos_des);
    setPath(p);

    tok = new QToken(this);
    tok->scale(1.5); // size of shot token: 3
    ani = new QPropertyAnimation(tok, "geometry", nullptr);
    connect(ani, &QPropertyAnimation::finished, this, &QConnection::reset);
    connect(ani, &QPropertyAnimation::finished, this, &QConnection::emitter);
    ani->setStartValue(QRectF(pos_src - QPointF(tok->getRadius() + this->pen().width()/2,
                                                tok->getRadius() + this->pen().width()/2),
                       QSize(2*tok->getRadius(), 2*tok->getRadius())));
    ani->setDuration(1000);
    ani->setEndValue(QRectF(pos_des - QPointF(tok->getRadius() + this->pen().width()/2,
                                              tok->getRadius() + this->pen().width()/2),
                     QSize(2*tok->getRadius(), 2*tok->getRadius())));
    ani->start(QAbstractAnimation::DeleteWhenStopped);

}

void QConnection::emitter()
{
    if(isFiringIn == true)
        emit isFiredIn();
    else
        emit isFiredOut();
}

void QConnection::reset()
{
    // reset the path
    setPen(QPen(Qt::black));
    update_path();
    tok->scale(0);
    delete tok;
}

