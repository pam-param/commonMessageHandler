#include <QApplication>

#include "commonmessagehandler.h"
#include "mainwidget.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    qInstallMessageHandler(CUSTOM_LOG);

    MainWidget wgt;
    wgt.show();

    return app.exec();
}
