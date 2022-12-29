#ifndef QMARKING_H
#define QMARKING_H

#include <QObject>
#include <QMap>

#include "QDocument.h"
class QGate;

class QMarking : public QGraphicsPathItem
{
public:
    QMarking(QMarking *parNode, QGraphicsItem *parent = nullptr);
    QMarking(QDocument *doc);
    ~QMarking();

    void shoot(QDocument::Trans *p);
    bool isEnabled(QDocument::Trans *t);
    bool operator<(QMarking *rhs);
    void setName();

    QMap<QString, int>& getMarking() { return this->marking; }
    QStringList& getFireSeq() { return fireSeq; }
    QVector<QMarking*>& getParSet() { return this->parSet; }
    QString getName() { return this->name; }
    QVector<QPair<QMarking*, QString>>& getTransSet() { return this->transSet; }
private:
    QMap<QString, int> marking;
    QStringList fireSeq;
    QVector<QMarking*> parSet;
    QVector<QPair<QMarking*, QString>> transSet;
    QString name;
};
#endif // QMARKING_H
