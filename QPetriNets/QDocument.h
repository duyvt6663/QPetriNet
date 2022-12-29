#ifndef QDOCUMENT_H
#define QDOCUMENT_H

#include <QGraphicsItem>
class QPlace;
class QTransition;
class QConnection;
class QMarking;

class QDocument : public QObject
{
    Q_OBJECT
public:
    class Trans
    {
    public:
        Trans(const QString &name);
        ~Trans();
        void addIn(QPlace*);
        void addOut(QPlace*);
        void delIn(QPlace*);
        void delOut(QPlace*);
        bool isEnabled() const;
        QString getName() const { return this->name; }
        QVector<QPlace*>& getInList() { return this->inList; }
        QVector<QPlace*>& getOutList() { return this->outList; }
    private:
        friend class QDocument;
        QVector<QPlace*> inList, outList;
        QString name;
    };
public:
    QDocument(QObject* parent);
    ~QDocument();
    void addConn(QConnection *);
    void delPlace(QPlace *);
    void delConn(QConnection *);
    void delTrans(QTransition *);
    void eliminate(QGraphicsItem *item);
    void storeMark(QMarking *v);
    bool containMark(const QString &str);
    void update();

    QMap<QString, Trans*>& getTmap() { return this->TMap; }
    QMap<QString, QPlace*>& getPmap() { return this->PMap; }
    QStringList displayMark();
    QStringList& getMarkList() { return this->MarkList; }
    QString& getCount() { return this->count; }

    static bool isValid(const QString &name);
public slots:
    void storeTrans(const QString &oldName, const QString &newName);
    void storePlace(QPlace* self, const QString &newName);
private:
    QStringList MarkList;
    static QMap<QString, Trans*> TMap;
    static QMap<QString, QPlace*> PMap;
    QString count;
};


#endif // QDOCUMENT_H
