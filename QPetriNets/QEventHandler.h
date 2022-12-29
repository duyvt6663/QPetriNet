#ifndef QEVENTHANDLER_H
#define QEVENTHANDLER_H

#include <QObject>

class QGraphicsScene;
class QConnection;
class QGraphicsItem;
class QPointF;
class QPlace;
class QDocument;

class QEventHandler : public QObject
{
    Q_OBJECT
public:
    explicit QEventHandler(QObject *parent = 0);

    void install(QGraphicsScene *scene);
    void trackScene(QDocument *newDoc);

    bool eventFilter(QObject *o, QEvent *e);
private:
    QGraphicsItem *itemAt(const QPointF & pos);

private:
    QGraphicsScene *scene;
    QConnection *conn;
    QGraphicsItem *mouse;
    QDocument *track;
    // QPlace *selBlock;
};
#endif // QEVENT_HANDLER
