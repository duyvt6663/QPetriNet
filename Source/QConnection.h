#ifndef QCONNECTION_H
#define QCONNECTION_H
#include <QGraphicsPathItem>
#include <QPropertyAnimation>

class QGate;
class QToken;

class QConnection : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    enum { Type = QGraphicsItem::UserType + 2 };
    QConnection(QGraphicsItem *parent = nullptr);
    ~QConnection();
    void set_pos_src(const QPointF& psrc);
    void set_pos_des(const QPointF& pdes);
    void set_src(QGate *src);
    void set_des(QGate *des);
    void scene_update_pos();
    void update_path();
    QGate *src_gate() const;
    QGate *des_gate() const;
    int type() const { return Type; }
    void fire(bool isFiringIn);
public slots:
    void reset();
signals:
    void isFiredIn();
    void isFiredOut();
private:
    QGate *src, *des;
    QPointF pos_src, pos_des;
    QToken *tok;
    QPropertyAnimation *ani;
    bool isFiringIn;
private:
    void emitter();
};
#endif // QCONNECTION_H
