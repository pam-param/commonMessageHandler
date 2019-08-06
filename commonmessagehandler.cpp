#include "commonmessagehandler.h"

#include <QApplication>
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

CommonMessageHandler::CommonMessageHandler(QObject *parent): QObject(parent)
{
    mFile = new QFile(QApplication::applicationDirPath() + "/logs.txt", this);
}

QString CommonMessageHandler::getMessage(const char* type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";

    QString message = QObject::tr("[%1] %2: %3 <%4> %5: %6\n").
                                    arg(QTime::currentTime().toString()).
                                    arg(file).
                                    arg(context.line).
                                    arg(function).
                                    arg(type).
                                    arg(localMsg.constData());
    return message;
}

/*void CommonMessageHandler::install(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";

    const char *messageType;

    switch(type) {
    case QtDebugMsg: {
        messageType = "Debug";
        break;

    }

    case QtInfoMsg: {
        messageType = "Information";
        QMessageBox::information(nullptr, QObject::tr(messageType), QObject::tr(localMsg.constData()));

        break;
    }

    case QtWarningMsg: {
        messageType = "Warning";

        #ifdef QT_DEBUG
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Warning in %1: %2.\r\n"
                                                                          "Call: %3.\r\n"
                                                                          "Details: %4").arg(file).arg(context.line).arg(function).arg(localMsg.constData()));
        #endif

        break;
    }

    case QtCriticalMsg: {
        messageType = "Critical";

        #ifdef QT_DEBUG
        QMessageBox::critical(nullptr, QObject::tr("Critical"), QObject::tr("Critical error in %1: %2.\r\n"
                                                                            "Call: %3.\r\n"
                                                                            "Details: %4").arg(file).arg(context.line).arg(function).arg(localMsg.constData()));
        #endif

        break;
    }
    case QtFatalMsg: {
    messageType = "Fatal";
        QMessageBox::critical(nullptr, QObject::tr("Fatal"), QObject::tr("Fatal error in %1: %2.\r\n"
                                                                            "Call: %3.\r\n"
                                                                            "Details: %4").arg(file).arg(context.line).arg(function).arg(localMsg.constData()));
        break;
    }

    }

    QString message = QObject::tr("[%1] %2: %3 <%4> %5: %6\n").
                                    arg(QTime::currentTime().toString()).
                                    arg(file).
                                    arg(context.line).
                                    arg(function).
                                    arg(messageType).
                                    arg(localMsg.constData());

    QTextStream stream(type==QtInfoMsg ? stdout : stderr);
    stream << message;

    QFile *logFile = CommonMessageHandler::instance().mFile;

    if(logFile->open(QFile::WriteOnly | QFile::Append)) {
        QTextStream out(logFile);
        out << message;
        logFile->close();
    }

}*/

void CommonMessageHandler::customMessageHandlerFunction(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    auto msgType = types.value(type, "unknown").toStdString().c_str();
    QString message = CommonMessageHandler::getMessage(msgType, context, msg);
    CommonMessageHandler::log(type, message);

    #ifdef QT_DEBUG
        CommonMessageHandler::showDebugMessageBox(type, context, msg);
    #else
        CommonMessageHandler::showReleaseMessageBox(type, msg);
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

    QFile *logFile = CommonMessageHandler::instance().mFile;

    if(logFile->open(QFile::WriteOnly | QFile::Append)) {
        QTextStream out(logFile);
        out << message;
        logFile->close();
    }
}

void CommonMessageHandler::showDebugMessageBox(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    auto messageType = types.value(type, "unknown").toStdString().c_str();

    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";

    switch(type) {

    case QtInfoMsg: {
        QMessageBox::information(nullptr, QObject::tr(messageType), QObject::tr(localMsg.constData()));

        break;
    }

    case QtWarningMsg: {
        QMessageBox::warning(nullptr, QObject::tr(messageType), QObject::tr("Warning in %1: %2.\r\n"
                                                                          "Call: %3.\r\n"
                                                                          "Details: %4").arg(file).arg(context.line).arg(function).arg(localMsg.constData()));

        break;
    }

    case QtCriticalMsg: {

        QMessageBox::critical(nullptr, QObject::tr(messageType), QObject::tr("Critical error in %1: %2.\r\n"
                                                                            "Call: %3.\r\n"
                                                                            "Details: %4").arg(file).arg(context.line).arg(function).arg(localMsg.constData()));

        break;
    }
    case QtFatalMsg: {
        QMessageBox::critical(nullptr, QObject::tr(messageType), QObject::tr("Fatal error in %1: %2.\r\n"
                                                                            "Call: %3.\r\n"
                                                                            "Details: %4").arg(file).arg(context.line).arg(function).arg(localMsg.constData()));
        break;
    }
    default: break;

    }
}

void CommonMessageHandler::showReleaseMessageBox(QtMsgType type, const QString &msg)
{
    auto messageType = types.value(type, "unknown").toStdString().c_str();
    QByteArray localMsg = msg.toLocal8Bit();

    switch(type) {
    case QtInfoMsg: {
        QMessageBox::information(nullptr, QObject::tr(messageType), QObject::tr(localMsg.constData()));

        break;
    }

    case QtFatalMsg: {
        QMessageBox::critical(nullptr, QObject::tr(messageType), QObject::tr(localMsg.constData()));
        break;
    }

    default: break;

    }

}

CommonMessageHandler::~CommonMessageHandler()
{

}
