#ifndef QTRANSITION_H
#define QTRANSITION_H
#include <QGraphicsPathItem>

class QGate;
class QToken;

class QTransition : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    enum { Type = QGraphicsItem::UserType + 4 };
    QTransition(QGraphicsItem *parent = nullptr, int num_of_token = 0);
    ~QTransition();
    void set_gate();
    int type() const { return Type; }
    void setName(const QString &n);
    bool isEnabled() const;
    void fire();
    void shoot(QGate *g);
    void receive(QGate *g);
    QString getName() const { return this->name; }
signals:
    void invalidName(QTransition *self);
    void nameChanged(const QString &oldName, const QString &newName);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
    QVector<QGate *> gate;
    QGraphicsTextItem *label;
    int token_count;
    int sideLen;
    QString name;
    bool busy;
};

#endif // QTRANSITION_H
