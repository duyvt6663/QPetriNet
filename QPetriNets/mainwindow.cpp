#include "mainwindow.h"
//#include "mainwindow_ui.h"
#include "QPlace.h"
#include "QEventHandler.h"
#include "QGate.h"
#include "QTransition.h"
#include "QToken.h"
#include "QDocument.h"
#include "QGraph.h"
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    scene = new QGraphicsScene();

    this->setWindowState(Qt::WindowMaximized);
    this->setMenu();
    this->setMainDock();
    this->setSideDock();
    this->setBottomDock();

    handle = new QEventHandler(this);
    track = new QDocument(this);
    handle->install(scene);
    handle->trackScene(track);

    // never recommend adding item directly like this, prone to error!
    QPlace *p = new QPlace(nullptr, 3);
    connect(p, &QPlace::invalidName,
            &MainWindow::setPlaceDialog);
    connect(p, &QPlace::nameChanged,
            track, &QDocument::storePlace);
    scene->addItem(p);
    p->setPos(-70, 0);
    p->setName("p1"); // setName is important


    p = new QPlace(nullptr, 1);
    connect(p, &QPlace::invalidName,
            this, &MainWindow::setPlaceDialog);
    connect(p, &QPlace::nameChanged,
            track, &QDocument::storePlace);
    scene->addItem(p);
    p->setPos(70, 0);
    p->setName("p2"); // setName is important


    QTransition *t = new QTransition();
    connect(t, &QTransition::invalidName,
            this, &MainWindow::setTransDialog);
    connect(t, &QTransition::nameChanged,
            track, &QDocument::storeTrans);
    scene->addItem(t);
    t->setName("t1");
}

MainWindow::~MainWindow()
{

}

void MainWindow::setMainDock()
{

    view = new QGraphicsView(this);
    view->setScene(scene);
    view->setRenderHint(QPainter::Antialiasing, true);
    view->show();
    setCentralWidget(view);

}

void MainWindow::setMenu()
{
    QAction *addPlace = new QAction(tr("&Place"), this);
    addPlace->setStatusTip(tr("Add a place"));
    connect(addPlace, SIGNAL(triggered()), this, SLOT(add_place()));

    QAction *addTrans = new QAction(tr("&Transition"), this);
    addTrans->setStatusTip(tr("Add a transition"));
    connect(addTrans, SIGNAL(triggered()), this, SLOT(add_trans()));

    QAction *listMarking = new QAction(tr("&Marking"), this);
    listMarking->setStatusTip(tr("List reachable markings"));
    connect(listMarking, SIGNAL(triggered()), this, SLOT(list_mark()));

    Add = menuBar()->addMenu(tr("&Add"));
    Add->addAction(addPlace);
    Add->addAction(addTrans);
    menuBar()->addAction(listMarking);

    Show = menuBar()->addMenu(tr("&Show"));
    setWindowTitle(tr("QPetriNets Demo (c) 2021 TxtbookDestroyer"));
}

void MainWindow::setSideDock()
{
    QDockWidget *right = new QDockWidget(tr("Coverability graph"), this);

    sideScene = new QGraphicsScene();
    sideView = new QGraphicsView(right);
    sideView->setScene(sideScene);
    sideView->setRenderHint(QPainter::Antialiasing, true);

    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    right->setAllowedAreas(Qt::RightDockWidgetArea);
    right->setWidget(sideView);
    right->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, right);

    QAction *wig = right->toggleViewAction();
    wig->setText("Coverability graph");
    Show->addAction(wig);
    resizeDocks({right}, {500}, Qt::Horizontal);

    QRectF r = sideView->rect();
    item = new QGraphicsPixmapItem(QPixmap());

    QPushButton *PlusButton = new QPushButton(QIcon("icons/plus.png"), "", sideView);
    PlusButton->setObjectName("plus");
    QString plusStyle = "QPushButton#plus {"
                        "border:none;"
                        "background-color:rgba(255, 255, 255, 100);"
                        "}"
                        "QPushButton#plus:hover {"
                        "background-color:rgba(107, 108, 110, 14);"
                        "border:none;"
                        "}";
    PlusButton->setStyleSheet(plusStyle); // Style sheet
    PlusButton->setIconSize(QSize(35,35));
    PlusButton->setMinimumSize(35,35);
    PlusButton->setMaximumSize(35,35);
    PlusButton->setGeometry(r.x() + 445, r.y(),
                             r.width(), r.height());
    connect(PlusButton, &QPushButton::pressed, [&]()
    {
        this->factor += 0.05;
        item->setScale(this->factor);
    });

    QPushButton *MinusButton = new QPushButton(QIcon("icons/minus.png"), "", sideView);
    MinusButton->setObjectName("minus");
    QString minusStyle = "QPushButton#minus {"
                        "border:none;"
                        "background-color:rgba(255, 255, 255, 100);"
                        "}"
                        "QPushButton#minus:hover {"
                        "background-color:rgba(107, 108, 110, 14);"
                        "border:none;"
                        "}";
    MinusButton->setStyleSheet(minusStyle); // Style sheet
    MinusButton->setIconSize(QSize(35,35));
    MinusButton->setMinimumSize(35,35);
    MinusButton->setMaximumSize(35,35);
    MinusButton->setGeometry(r.x() + 445, r.y() + PlusButton->size().height(),
                             r.width(), r.height());
    connect(MinusButton, &QPushButton::pressed, [&]()
    {
        this->factor -= 0.05;
        item->setScale(this->factor);
    });
}

void MainWindow::setBottomDock()
{
    QDockWidget *bottom = new QDockWidget(tr("Copyright"), this);
    markList = new QListWidget(bottom);

    bottom->setAllowedAreas(Qt::BottomDockWidgetArea);
    bottom->setWidget(markList);
    bottom->setFont(QFont("Helvetica [Cronyx]", 12));
    //bottom->setVisible(false);
    addDockWidget(Qt::BottomDockWidgetArea, bottom);
    bottom->setMinimumHeight(410);

    markList->addItems(QStringList()
                       << "Copyright (c) 2021 TxtbookDestroyer\n"

                       << "Permission is hereby granted, free of charge, to any person obtaining a copy"
                       << "of this software and associated documentation files (the \"Software\"), to deal"
                       << "in the Software without restriction, including without limitation the rights"
                       << "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell"
                       << "copies of the Software, and to permit persons to whom the Software is"
                       << "furnished to do so, subject to the following conditions:\n"

                       << "The above copyright notice and this permission notice shall be included in all"
                       << "copies or substantial portions of the Software.\n"

                       << "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
                       << "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
                       << "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE"
                       << "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
                       << "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
                       << "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
                       << "SOFTWARE.");
    QAction *wig = bottom->toggleViewAction();
    wig->setText("Markings");
    Show->addAction(wig);

}

void MainWindow::add_place()
{
    QPlace *b = new QPlace();
    connect(b, &QPlace::invalidName,
            &MainWindow::setPlaceDialog);
    connect(b, &QPlace::nameChanged,
            track, &QDocument::storePlace);
    scene->addItem(b);
    b->setPos(view->sceneRect().center().toPoint());
    setPlaceDialog(b);
}

void MainWindow::add_trans()
{
    QTransition *b = new QTransition();
    connect(b, &QTransition::invalidName,
            this, &MainWindow::setTransDialog);
    connect(b, &QTransition::nameChanged,
            track, &QDocument::storeTrans);
    scene->addItem(b);
    b->setPos(view->sceneRect().center().toPoint());
    setTransDialog(b);
}

void MainWindow::list_mark()
{
    track->update();
    QGraph transGraph(track, nullptr);
    transGraph.setCovGraph();
    transGraph.createDotFile();
    transGraph.saveImageGV();

    markList->clear();
    markList->addItems(track->displayMark());
    //((QDockWidget*) markList->parent())->setVisible(true);

    //QPixmap p("covGraph.png");
    //p = p.scaled(200, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImageReader im("image/covGraph.png");

    QImage img("image/covGraph.png");
    item->setPixmap(QPixmap::fromImage(img));
    sideScene->addItem(item);
    this->factor = 0.3;
    item->setScale(this->factor);

    ((QDockWidget*) sideView->parent())->setVisible(true);
    ((QDockWidget*) markList->parent())->setMinimumHeight(200);
    ((QDockWidget*) markList->parent())->setMaximumHeight(200);
    ((QDockWidget*) markList->parent())->setWindowTitle(tr("Markings data and firing sequences"));
}

void MainWindow::setPlaceDialog(QPlace *b)
{
    QDialog dialog(nullptr);
    // Use a layout allowing to have a label next to each field
    QFormLayout form(&dialog);

    // Add some text above the fields
    dialog.setWindowTitle(tr("Set place"));

    // Add the lineEdits with their respective labels
    QLineEdit *lineEdit2 = new QLineEdit(&dialog);
    lineEdit2->setText(b->getName());
    QString label2 = QString("Name (must input)");
    form.addRow(label2, lineEdit2);

    QSpinBox *lineEdit1 = new QSpinBox(&dialog);
    lineEdit1->setRange(0, 10);
    QString label1 = QString("Number of tokens (10 max)");
    form.addRow(label1, lineEdit1);


    // Add some standard buttons (Cancel/Ok) at the bottom of the dialog
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Show the dialog as modal
    if (dialog.exec() == QDialog::Accepted)
    {
        b->setTok(lineEdit1->value());
        b->setName(lineEdit2->text());
    }
    else if(b->getName() == "" && b->token() == 0)
    {
        delete b;
    }
}

void MainWindow::setTransDialog(QTransition *b)
{
    QString text = QInputDialog::getText(view, "Set Transition", "Name");
    b->setName(text);
}
