#include "commonmessagehandler.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
#include <QTime>

#ifndef QT_MESSAGELOGCONTEXT
    #warning "QT_MESSAGELOGCONTEXT undefined. Message handler will not work"
#endif

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
    this->setFilePath();
}

QString CommonMessageHandler::message(const char* type,
                                         const QMessageLogContext &context,
                                         const QString &msg)
{
    QFileInfo fileInfo(context.file ? context.file : "");
    const char *function = context.function ? context.function : "";

    auto message = QObject::tr("[%1] %2: %3 <%4> %5: %6\n").
                                    arg(QTime::currentTime().toString()).
                                    arg(fileInfo.absoluteFilePath()).
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
    auto& messageHandler = CommonMessageHandler::instance();

    if(messageHandler.mIsFileLoggingActive) {
        messageHandler.logToFile(type, context, msg);
    }

    if(messageHandler.mIsStreamLoggingActive){
        messageHandler.logToStream(type, context, msg);
    }

    if(messageHandler.mIsUILoggingActive) {
        #ifdef QT_DEBUG
            messageHandler.showDebugMessageBox(type, context, msg);
        #else
            messageHandler.showReleaseMessageBox(type, msg);
        #endif
    }
}

CommonMessageHandler& CommonMessageHandler::instance()
{
    return *messageHandler;
}

void CommonMessageHandler::logToFile(QtMsgType type,
                                     const QMessageLogContext &context,
                                     const QString &msg)
{
    if(mFile.open(QFile::WriteOnly | QFile::Append)) {
        QTextStream out(&mFile);
        auto msgType = types.value(type, "unknown").toStdString().c_str();
        out << this->message(msgType, context, msg);
        mFile.close();
    }
}
void CommonMessageHandler::logToStream(QtMsgType type,
                                       const QMessageLogContext &context,
                                       const QString &msg)
{
    QTextStream stream(type==QtInfoMsg ? stdout : stderr);
    auto msgType = types.value(type, "unknown").toStdString().c_str();
    stream << this->message(msgType, context, msg);
}

void CommonMessageHandler::setIsFileLoggingActive(bool isActive)
{
    mIsFileLoggingActive = isActive;
}

void CommonMessageHandler::setIsStreamLoggingActive(bool isActive)
{
    mIsStreamLoggingActive = isActive;
}

void CommonMessageHandler::setIsUILoggingActive(bool isActive)
{
    mIsUILoggingActive = isActive;
}

QString CommonMessageHandler::setFilePath(QString path)
{
    QFileInfo fileInfo(path);

    if(!fileInfo.absoluteDir().exists() || path.isEmpty()) {
        fileInfo.setFile(QDir(QApplication::applicationDirPath()), QString("logs.txt"));
    }

    mFile.setFileName(fileInfo.absoluteFilePath());

    return mFile.fileName();
}

void CommonMessageHandler::showDebugMessageBox(QtMsgType type,
                                               const QMessageLogContext &context,
                                               const QString &msg)
{
    auto messageType = types.value(type, "unknown").toStdString().c_str();
    auto message = msg.toStdString().c_str();

    QFileInfo fileInfo(context.file ? context.file : "");
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
                                         .arg(fileInfo.absoluteFilePath())
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
                                          .arg(fileInfo.absoluteFilePath())
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
                                          .arg(fileInfo.absoluteFilePath())
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
