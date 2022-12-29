#include "QMarking.h"
#include "QDocument.h"
#include "QPlace.h"

QMarking::QMarking(QMarking *parNode, QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    marking = parNode->getMarking();
    fireSeq = parNode->getFireSeq();
}

QMarking::QMarking(QDocument *doc)
{
    foreach(QPlace *p, doc->getPmap().values())
        marking[p->getName()] = p->token();
    setName();
}

QMarking::~QMarking()
{
    marking.clear();
    fireSeq.clear();
    parSet.clear();
    transSet.clear();
}


void QMarking::shoot(QDocument::Trans *p)
{
    foreach(QPlace *k, p->getInList())
        --marking[k->getName()];
    foreach(QPlace *k, p->getOutList())
        ++marking[k->getName()];
    fireSeq << p->getName();
}

bool QMarking::isEnabled(QDocument::Trans *t)
{
    if(t->getInList().empty() || t->getOutList().empty())
        return false;
    foreach(QPlace *p, t->getInList())
        if(marking[p->getName()] <= 0)
            return false;
    return true;;
}

void QMarking::setName()
{
    if(!marking.empty())
    {
        foreach(int k, marking.values())
        {
            if(k <= 5000000)
                name += QString::number(k) + ", ";
            else
                name += "w, ";
        }
        name.chop(2);
    }
}
