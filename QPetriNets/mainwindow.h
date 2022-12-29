#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QtWidgets>

class QEventHandler;
class QDocument;
class QPlace;
class QTransition;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setMainDock();
    void setMenu();
    void setSideDock();
    void setBottomDock();
public slots:
    void add_place();
    void add_trans();
    void list_mark();
    static void setPlaceDialog(QPlace *);
    void setTransDialog(QTransition *);
private:
    QGraphicsScene *scene, *sideScene;
    QMenu *Add, *Show;
    QGraphicsView *view, *sideView;
    QEventHandler *handle;
    QDocument *track;
    QListWidget *markList;
    QGraphicsPixmapItem *item;
    qreal factor;
};
#endif // MAINWINDOW_H
