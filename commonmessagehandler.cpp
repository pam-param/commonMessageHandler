#include "commonmessagehandler.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QTime>

const QHash <QtMsgType, QString> CommonMessageHandler::types =
QHash <QtMsgType, QString>{
    {QtDebugMsg, "Debug"},
    {QtWarningMsg, "Warning"},
    {QtCriticalMsg, "Critical"},
    {QtFatalMsg, "Fatal"},
    {QtInfoMsg, "Info"}
};

Q_GLOBAL_STATIC(CommonMessageHandler, messageHandler)

CommonMessageHandler::CommonMessageHandler(QObject *parent):
    QObject(parent)
{
    QFileInfo fileInfo;
    fileInfo.setFile(QDir(QApplication::applicationDirPath()), QString("logs.txt"));
    mFile.setFileName(fileInfo.absoluteFilePath());
}

QString CommonMessageHandler::getMessage(const char* type,
                                         const QMessageLogContext &context,
                                         const QString &msg)
{
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";

    QString message = QObject::tr("[%1] %2: %3 <%4> %5: %6\n").
                                    arg(QTime::currentTime().toString()).
                                    arg(file).
                                    arg(context.line).
                                    arg(function).
                                    arg(type).
                                    arg(msg);
    return message;
}

void CommonMessageHandler::customMessageHandlerFunction(QtMsgType type,
                                                        const QMessageLogContext &context,
                                                        const QString &msg)
{
    auto msgType = types.value(type, "unknown").toStdString().c_str();

    CommonMessageHandler& messageHandler = CommonMessageHandler::instance();

    QString message = messageHandler.getMessage(msgType, context, msg);
    messageHandler.log(type, message);

    #ifdef QT_DEBUG
        messageHandler.showDebugMessageBox(type, context, msg);
    #else
        messageHandler.showReleaseMessageBox(type, msg);
    #endif
}

CommonMessageHandler& CommonMessageHandler::instance()
{
    return *messageHandler;
}

void CommonMessageHandler::log(QtMsgType type, QString message)
{

    QTextStream stream(type==QtInfoMsg ? stdout : stderr);
    stream << message;

    if(mFile.open(QFile::WriteOnly | QFile::Append)) {
        QTextStream out(&mFile);
        out << message;
        mFile.close();
    }
}

void CommonMessageHandler::showDebugMessageBox(QtMsgType type,
                                               const QMessageLogContext &context,
                                               const QString &msg)
{
    auto messageType = types.value(type, "unknown").toStdString().c_str();
    auto message = msg.toStdString().c_str();

    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";

    switch(type) {

    case QtInfoMsg: {
        QMessageBox::information(nullptr, QObject::tr(messageType), QObject::tr(message));

        break;
    }

    case QtWarningMsg: {
        QMessageBox::warning(nullptr,
                             QObject::tr(messageType),
                             QObject::tr("Warning in %1: %2.\r\n"
                                         "Call: %3.\r\n"
                                         "Details: %4")
                                         .arg(file)
                                         .arg(context.line)
                                         .arg(function)
                                         .arg(message));

        break;
    }

    case QtCriticalMsg: {

        QMessageBox::critical(nullptr,
                              QObject::tr(messageType),
                              QObject::tr("Critical error in %1: %2.\r\n"
                                          "Call: %3.\r\n"
                                          "Details: %4")
                                          .arg(file)
                                          .arg(context.line)
                                          .arg(function)
                                          .arg(message));

        break;
    }
    case QtFatalMsg: {
        QMessageBox::critical(nullptr,
                              QObject::tr(messageType),
                              QObject::tr("Fatal error in %1: %2.\r\n"
                                          "Call: %3.\r\n"
                                          "Details: %4")
                                          .arg(file)
                                          .arg(context.line)
                                          .arg(function)
                                          .arg(message));
        break;
    }
    default: break;

    }
}

void CommonMessageHandler::showReleaseMessageBox(QtMsgType type,
                                                 const QString &msg)
{
    auto messageType = types.value(type, "unknown").toStdString().c_str();
    auto message = msg.toStdString().c_str();

    switch(type) {
    case QtInfoMsg: {
        QMessageBox::information(nullptr,
                                 QObject::tr(messageType),
                                 QObject::tr(message));

        break;
    }

    case QtFatalMsg: {
        QMessageBox::critical(nullptr,
                              QObject::tr(messageType),
                              QObject::tr(message));
        break;
    }

    default: break;

    }

}

CommonMessageHandler::~CommonMessageHandler()
{

}
