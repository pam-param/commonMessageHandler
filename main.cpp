#include <QApplication>

#include "commonmessagehandler.h"
#include "mainwidget.h"

int main(int argc, char** argv)
{
    qInstallMessageHandler(CommonMessageHandler::customMessageHandlerFunction);
    QApplication app(argc, argv);

    MainWidget wgt;
    wgt.show();

    return app.exec();
}
