QT += \
    core \
    widgets \

#DEFINES += QT_MESSAGELOGCONTEXT

TARGET = commonMessageHandler
TEMPLATE = lib

DEFINES += COMMONMESSAGEHANDLER_LIBRARY

HEADERS += \
    commonmessagehandler.h \
    commonmessagehandler_global.h

SOURCES += \
    commonmessagehandler.cpp \

install_headers.path = $$(HEADER_PATH)
install_headers.files = $$HEADERS

INSTALLS += install_headers

DESTDIR = $$(LIB_PATH)
