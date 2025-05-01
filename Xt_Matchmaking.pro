QT       += core gui \
    quick
QMAKE_PROJECT_DEPTH=0
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += multimedia  # 添加这一行，用于引入multimedia模块

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GamePage.cpp \
    Hoverbutton.cpp \
    Rect.cpp \
    RulePage.cpp \
    Settlement.cpp \
    Show_Button.cpp \
    StartPage.cpp \
    dialogbox.cpp \
    main.cpp

HEADERS += \
    GamePage.h \
    Hoverbutton.h \
    Rect.h \
    RulePage.h \
    Settlement.h \
    Show_Button.h \
    StartPage.h \
    dialogbox.h

FORMS += \
    GamePage.ui \
    RulePage.ui \
    Settlement.ui \
    StartPage.ui \
    dialogbox.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    music.qrc
