#ifndef QTOKEN_H
#define QTOKEN_H
#include <QGraphicsPathItem>

class QPlace;

class QToken : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
    Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry)
public:
    QToken(QGraphicsItem *parent = nullptr);
    int getRadius() { return radius; }
    void scale(qreal factor);
    void setGeometry(const QRectF rect);
    QRectF geometry() const { return geo; }
protected:
    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
private:
    qreal radius;
    QRectF geo;
};

#endif // QTOKEN_H
