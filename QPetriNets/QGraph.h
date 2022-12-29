#ifndef QGRAPH_H
#define QGRAPH_H
#include <QGraphicsPathItem>

class QMarking;
class QDocument;
class QConnection;

class QGraph : public QGraphicsPathItem
{
public:
    QGraph(QDocument* doc, QGraphicsItem *parent);
    ~QGraph();
    void setCovGraph();
    QMarking* Monotonic(QMarking *v, QMarking *w);

    bool isLessThan(QMarking *v, QMarking *w);
    void storeMark(QMarking *v);
    void createDotFile();
    void saveImageGV();
private:
    QMarking *v0;
    QDocument *track;
    QMap<QString, QMarking*> V;
    //QVector<QConnection*> A;
private:
    QMarking* Monotonic(QMarking *v, QMarking *w, QHash<QMarking*, bool>& visited);
};

#endif // QGRAPH_H
