#include "QGraph.h"
#include "QDocument.h"
#include "QMarking.h"
#include "QQueue"
#include "QPlace.h"
#include "QGate.h"
#include "QConnection.h"
#include "QFile"
#include "QPair"
#include <gvc.h>
#include <gvplugin.h>
#include <cgraph.h>
#include <cdt.h>
#include <iostream>
using namespace std;

QGraph::QGraph(QDocument *doc, QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    this->track = doc;
    v0 = new QMarking(doc);
    V[v0->getName()] = v0;
}

QGraph::~QGraph()
{
    delete v0;
    V.clear();
}

void QGraph::setCovGraph()
{
    QQueue<QMarking*> U;
    U.enqueue(v0);

    while(!U.empty())
    {
        QMarking *v = U.dequeue();
        storeMark(v);
        foreach(QDocument::Trans *t, track->getTmap().values())
            if(v->isEnabled(t))
            {
                QMarking *w = new QMarking(v);
                w->shoot(t);
                QMarking *u = Monotonic(v, w);
                if(u != nullptr)
                {
                    foreach(QPlace *p, track->getPmap().values())
                    {
                        int &temp = w->getMarking()[p->getName()];
                        if(u->getMarking()[p->getName()] < temp )
                        {
                            temp = 10000000;
                            track->getCount() = "infinite";
                        }
                    }
                }
                w->setName();
                if(V.count(w->getName()) == 0)
                {
                    V[w->getName()] = w;
                    U.enqueue(w);
                }
                else
                {
                    storeMark(w);
                    QMarking* oldNode = w;
                    w = V[w->getName()];
                    delete oldNode;
                }
                v->getTransSet().append({w, t->getName()});
                w->getParSet().append(v);
            }
    }
    if(track->getCount() != "infinite")
        track->getCount() = QString::number(V.count());
}

QMarking* QGraph::Monotonic(QMarking *v, QMarking *w)
{
    // find u such that M(u) < M'
    QHash<QMarking*, bool> visited;
    QMarking *temp = Monotonic(v, w, visited);
    visited.clear();
    return temp;
}

bool QGraph::isLessThan(QMarking *v, QMarking *w)
{
    // function to compare two marking multiset
    bool flag = false;
    foreach(QPlace* p, track->getPmap().values())
    {
        int& lhs = v->getMarking()[p->getName()],
                & rhs = w->getMarking()[p->getName()];
        if(lhs > rhs && rhs <= 5000000)
            return false;
        else if(lhs < rhs)
            flag = true;
    }
    return flag;
}

void QGraph::storeMark(QMarking *v)
{
    track->storeMark(v);
}

void QGraph::createDotFile()
{
    if(V.count() > 30)
        return;
    QString pos[] = {"", "x"};
    QFile file("image/covGraph.gv");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return;
    QPair<QMarking*, QString> pair;
    QTextStream out(&file);
    out << "digraph {\n" << "graph [ dpi = 500 ];";
    foreach(QMarking* v, V.values())
    {
        if(!v->getTransSet().empty())
        {
            foreach(pair, v->getTransSet())
            {
                out << "\"" << v->getName() << "\"" << " -> "
                    << "\"" << pair.first->getName() << "\""
                    << " [" << pos[rand()%2] << "label=\""  << pair.second << "\",arrowsize=.5];\n";
            }
        }
        else
        {
            out << "\"" << v->getName() << "\"" << ";\n";
        }
    }
    QStringList pList;
    foreach(QPlace *p, track->getPmap().values())
        pList << p->getName();
    out << "labelloc = \"t\";\n"
        << "label=\""
        << "[" << pList.join(", ") << "]\";\n}";
}

void QGraph::saveImageGV()
{
    GVC_t *gvc = gvContext();
    Agiodisc_t my_iodisc = {
        .afread = [](void* chan, char* buf, int bufsize)->int
        {
            return fread(buf, 1, bufsize, (FILE*)chan);
        },
        .putstr = [](void* chan, const char* buf)->int
        {
            return fwrite(buf, 1, strlen(buf), (FILE*)chan);
        },
        .flush = [](void* chan)->int
        {
            return fflush((FILE*)chan);
        }
    };
    Agdisc_t my_disc = {
        .mem = NULL, // use system default
        .id = NULL, // use system default
        .io = &my_iodisc
    };
    FILE *fi = fopen("image/covGraph.gv", "r");
    Agraph_t *G = agread(fi, &my_disc);
    gvLayout(gvc, G, "dot");
    gvRenderFilename(gvc, G, "png", "image/covGraph.png");
    agclose(G);
    fclose(fi);
}

QMarking *QGraph::Monotonic(QMarking *v, QMarking *w, QHash<QMarking*, bool> &visited)
{
    if(isLessThan(v, w))
        return v;
    visited[v] = true;
    foreach(QMarking* m, v->getParSet())
    {
        if(visited[m] == false)
        {
            QMarking *temp = Monotonic(m, w, visited);
            if(temp != nullptr)
                return temp;
        }
    }
    return nullptr;
}
