#include "QEventHandler.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QThread>
#include <QCursor>

#include "QGate.h"
#include "QConnection.h"
#include "QPlace.h"
#include "QTransition.h"
#include "mainwindow.h"
#include "QDocument.h"

QEventHandler::QEventHandler(QObject *parent) :
    QObject(parent)
{
    conn = nullptr;
    mouse = nullptr;
}

void QEventHandler::install(QGraphicsScene *scene)
{
    scene->installEventFilter(this);
    this->scene = scene;
}

void QEventHandler::trackScene(QDocument *newDoc)
{
    this->track = newDoc;
}

QGraphicsItem* QEventHandler::itemAt(const QPointF &pos)
{
    QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

    foreach(QGraphicsItem *item, items)
        if (item->type() > QGraphicsItem::UserType)
            return item;

    return nullptr;
}

bool QEventHandler::eventFilter(QObject *o, QEvent *e)
{
    QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;
    switch ((int) e->type())
    {
    case QEvent::GraphicsSceneMouseDoubleClick:
    {
        if((int) me->button() == Qt::LeftButton)
        {
            QGraphicsItem *item = itemAt(me->scenePos());
            if (item && item->type() == QPlace::Type)
            {
                QPlace *p = (QPlace*) item;
                MainWindow::setPlaceDialog(p);
                return true;
            }
        }
        break;
    }
    case QEvent::GraphicsSceneMousePress:
    {

        switch ((int) me->button())
        {
        case Qt::LeftButton:
        {
            QGraphicsItem *item = itemAt(me->scenePos());
            if (item && item->type() == QGate::Type)
            {
                conn = new QConnection(nullptr);
                scene->addItem(conn);
                conn->set_src((QGate*) item);
                conn->set_pos_src(item->scenePos());
                conn->set_pos_des(me->scenePos());
                conn->update_path();

                return true;
            } else if (item && item->type() == QTransition::Type &&
                       ((QTransition*)item)->isEnabled())
            {
                ((QTransition*)item)->fire();
                scene->update();
                return true;
            }
            break;
        }
        case Qt::RightButton:
        {
            QGraphicsItem *item = itemAt(me->scenePos());
            if (item && (item->type() == QConnection::Type ||
                         item->type() == QPlace::Type ||
                         item->type() == QTransition::Type))
            {
                track->eliminate(item);
                delete item;
                mouse = nullptr;
            }
            // if (selBlock == (QNEBlock*) item)
            // selBlock = 0;
            break;
        }
        }
        break;
    }
    case QEvent::GraphicsSceneMouseMove:
    {
        if (conn != nullptr)
        {
            QGraphicsItem *newItem = itemAt(me->scenePos());
            conn->set_pos_des(me->scenePos());
            conn->update_path();
            if(mouse != nullptr)
            {
                if(mouse->type() != QGate::Type ||
                        (QGate*)mouse == conn->src_gate())
                {
                    mouse = nullptr;
                    break;
                }
                QGate *des = (QGate*) mouse;
                if(newItem == nullptr ||
                        newItem->type() != QGate::Type ||
                        newItem != mouse)
                {
                    des->hoverLeaveEvent((QGraphicsSceneHoverEvent*) e);
                    mouse = (newItem)? newItem : nullptr;
                }
                else des->hoverEnterEvent((QGraphicsSceneHoverEvent*) e);
            }
            else mouse = itemAt(me->scenePos());
            return true;
        }
        break;
    }
    case QEvent::GraphicsSceneMouseRelease:
    {
        if (conn && me->button() == Qt::LeftButton)
        {
            QGraphicsItem *item = itemAt(me->scenePos());
            if (item && item->type() == QGate::Type)
            {
                QGate *src = conn->src_gate();
                QGate *des = (QGate*) item;
                des->hoverLeaveEvent((QGraphicsSceneHoverEvent*) e);
                src->hoverLeaveEvent((QGraphicsSceneHoverEvent*) e);
                if (src->parent->type() != des->parent->type() &&
                        !src->isConnected(des))
                {
                    if((src->parent->type() == QTransition::Type               &&
                        !track->getTmap()[((QTransition*) src->parent)->getName()]
                        ->getOutList().contains((QPlace*) des->parent))    ||
                            (des->parent->type() == QTransition::Type               &&
                             !track->getTmap()[((QTransition*) des->parent)->getName()]
                             ->getInList().contains((QPlace*) src->parent)      ))
                    {
                        conn->set_pos_des(des->scenePos());
                        conn->set_des(des);
                        conn->update_path();
                        track->addConn(conn);
                        conn = nullptr;
                        return true;
                    }
                }
            }

            delete conn;
            mouse = nullptr;
            conn = nullptr;
            return true;
        }
        break;
    }
    }
    return QObject::eventFilter(o, e);
}
