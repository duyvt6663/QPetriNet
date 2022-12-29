#ifndef QPLACE_H
#define QPLACE_H
#include <QGraphicsPathItem>

class QToken;
class QGate;

class QPlace : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    enum { Type = QGraphicsItem::UserType + 3 };
    QPlace(QGraphicsItem *parent = nullptr, int num_ofTok = 0);
    ~QPlace();
    void setGate();
    void setTok(int num_ofTok = 0);
    int type() const { return Type; }
    int token() const;
    void shoot() { setTok(tokCount - 1); }
    void setName(const QString &name);
    QVector<QGate *>& getGate() { return this->gate; }
    QString getName() const { return this->name; }
public slots:
    void receive() { setTok(tokCount + 1); }
signals:
    void nameChanged(QPlace* self, const QString &newName);
    void invalidName(QPlace* self);
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    int tokCount;
    int radius;
    QVector<QGate *> gate;
    QVector<QToken *> tokList;
    QGraphicsTextItem *tokLabel, *label;
    QString name;
};
#endif // QPLACE_H
