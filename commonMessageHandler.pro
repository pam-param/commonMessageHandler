QT += \
    core \
    widgets \

#DEFINES += QT_MESSAGELOGCONTEXT

TARGET = myProgram
TEMPLATE = app

HEADERS += \
    commonmessagehandler.h \
    mainwidget.h

SOURCES += \
    commonmessagehandler.cpp \
    main.cpp \
    mainwidget.cpp
