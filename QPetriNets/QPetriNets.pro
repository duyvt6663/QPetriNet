QT       += core gui \
            widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QConnection.cpp \
    QDocument.cpp \
    QEventHandler.cpp \
    QGate.cpp \
    QGraph.cpp \
    QMarking.cpp \
    QPlace.cpp \
    QToken.cpp \
    QTransition.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    QConnection.h \
    QDocument.h \
    QEventHandler.h \
    QGate.h \
    QGraph.h \
    QMarking.h \
    QPlace.h \
    QToken.h \
    QTransition.h \
    mainwindow.h \
    mainwindow_ui.h

FORMS += \
    mainwindow.ui

DEFINES += WIN32_DLL
DEFINES += GVDLL
GRAPHVIZ_PATH = "C:\Program Files\Graphviz"
INCLUDEPATH += $$GRAPHVIZ_PATH/include/graphviz
LIBS += -L$$GRAPHVIZ_PATH/lib -lgvc -lcgraph -lcdt -lgvplugin_dot_layout
QMAKE_POST_LINK = "C:\Users\WIN\Documents\Qt programs\QPetriNets\bat file\move_libs.bat"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
