#include "QDocument.h"
#include "QTransition.h"
#include "QConnection.h"
#include "QGate.h"
#include "QPlace.h"
#include "QMarking.h"
#include <QMessageBox>

QMap<QString, QDocument::Trans*> QDocument::TMap;
QMap<QString, QPlace*> QDocument::PMap;
QDocument::Trans::Trans(const QString &name)
{
    this->name = name;
}

QDocument::Trans::~Trans()
{
    while(!inList.empty())
        inList.pop_back();
    while(!outList.empty())
        outList.pop_back();
}

void QDocument::Trans::addIn(QPlace *p)
{
    inList.append(p);
}

void QDocument::Trans::addOut(QPlace *p)
{
    outList.append(p);
}

void QDocument::Trans::delIn(QPlace *p)
{
    inList.remove(inList.indexOf(p));
}

void QDocument::Trans::delOut(QPlace *p)
{
    outList.remove(outList.indexOf(p));
}

bool QDocument::Trans::isEnabled() const
{
    foreach(QPlace *p, inList)
        if(p->token() <= 0)
            return false;
    return true;
}

void QDocument::addConn(QConnection *p)
{
    QGraphicsItem *des = p->des_gate()->get_parent(),
            *src = p->src_gate()->get_parent();
    if(src->type() == QPlace::Type)
        TMap[((QTransition*) des)->getName()]->addIn((QPlace*) src);
    else
        TMap[((QTransition*) src)->getName()]->addOut((QPlace*) des);
}

void QDocument::delPlace(QPlace *p)
{
    foreach(QGate *g, p->getGate())
        foreach(QConnection *p, g->connections())
            delConn(p);
    PMap.remove(p->getName());
}

void QDocument::delConn(QConnection *p)
{
    QGraphicsItem *des = p->des_gate()->get_parent(),
            *src = p->src_gate()->get_parent();
    if(src->type() == QPlace::Type)
        TMap[((QTransition*) des)->getName()]->delIn((QPlace*) src);
    else
        TMap[((QTransition*) src)->getName()]->delOut((QPlace*) des);
}

void QDocument::delTrans(QTransition *p)
{
    TMap[p->getName()]->~Trans();
    TMap.remove(p->getName());
}

void QDocument::eliminate(QGraphicsItem *item)
{
    switch((int) item->type())
    {
    case QPlace::Type:
        delPlace((QPlace*) item);
        break;
    case QTransition::Type:
        delTrans((QTransition*) item);
        break;
    case QConnection::Type:
        delConn((QConnection*) item);
        break;
    default:
        break;
    }
}

bool QDocument::isValid(const QString &name)
{
    if(TMap.contains(name) || PMap.contains(name))
    {
        QMessageBox error;
        error.critical(nullptr, "Error", "Name already set!");
        error.setFixedSize(500, 200);
        return false;
    }
    return true;
}

void QDocument::storeMark(QMarking *v)
{
    MarkList << ("[" + v->getFireSeq().join(", ") + "]");
    QString str = "[";
    if(!v->getMarking().empty())
    {
        foreach(int k, v->getMarking().values())
        {
            if(k <= 5000000)
                str += QString::number(k) + ", ";
            else str += "w, ";
        }
        str.chop(2);
    }
    str += "]";
    MarkList << str;
}

bool QDocument::containMark(const QString &str)
{
    return MarkList.contains(str);
}

QStringList QDocument::displayMark()
{
    QStringList ret, placeList;
    foreach(QPlace* p, PMap.values())
        placeList << p->getName();
    ret << "Order of places: [" + placeList.join(", ") + "]";
    ret << "Count of reachable markings: " + count;
    for(int i = 0; i < MarkList.size(); ++i)
    {
        ret << QString("Firing sequence: ") + MarkList.at(i++).toLocal8Bit().constData()
            << QString("Reached marking: ") + MarkList.at(i).toLocal8Bit().constData() + "\n";
    }
    return ret;
}

void QDocument::update()
{
    MarkList.clear();
    count.clear();
}

void QDocument::storeTrans(const QString &oldName, const QString &newName)
{
    if(TMap.contains(oldName))
    {
        Trans* temp = TMap[oldName];
        temp->name = newName;
        TMap.remove(oldName);
        TMap[newName] = temp;
    }
    else TMap[newName] = new Trans(newName);
}

void QDocument::storePlace(QPlace *self, const QString &newName)
{ 
    if(PMap.contains(self->getName()))
    {
        PMap.remove(self->getName());
    }
    PMap[newName] = self; // this order is important
}

QDocument::QDocument(QObject *parent)
    : QObject(parent)
{

}

QDocument::~QDocument()
{
    foreach(Trans* k, TMap.values())
        delete k;
    TMap.~QMap();
    PMap.~QMap();
}
