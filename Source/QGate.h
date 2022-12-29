#ifndef QGATE_H
#define QGATE_H

#include <QGraphicsPathItem>

class QPlace;
class QConnection;
class QEvenHandler;

class QGate : public QGraphicsPathItem
{
public:
    enum { Type = QGraphicsItem::UserType + 1 };
    QGate(QGraphicsItem *parent = nullptr);
    QVector<QConnection *>& connections();
    int type() const { return Type; }
    void set_parent(QGraphicsItem *parent);
    QGraphicsItem* get_parent() const;
    bool isConnected(QGate *g);
    static qreal get_radius();
    QPainterPath shape() const;
protected:
    friend class QEventHandler;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
private:
    constexpr static qreal radius = 0;
    QVector<QConnection *> conn_list;
    QGraphicsItem *parent;
};
#endif // QGATE_H
