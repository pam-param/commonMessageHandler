#ifndef COMMONMESSAGEHANDLER_H
#define COMMONMESSAGEHANDLER_H

#include "commonmessagehandler_global.h"

#include <QFile>
#include <QGlobalStatic>
#include <QObject>
#include <QHash>

#ifndef QT_MESSAGELOGCONTEXT
    #warning "QT_MESSAGELOGCONTEXT undefined. Message handler will not work"
#endif

#define CUSTOM_LOG CommonMessageHandler::customMessageHandlerFunction

class COMMONMESSAGEHANDLER_EXPORT CommonMessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit CommonMessageHandler(QObject *parent = nullptr);
    virtual ~CommonMessageHandler() override;
    static CommonMessageHandler& instance();

    static void customMessageHandlerFunction(QtMsgType type,
                                             const QMessageLogContext &context,
                                             const QString &msg);

    void setIsFileLoggingActive(bool isActive);
    void setIsStreamLoggingActive(bool isActive);
    void setIsUILoggingActive(bool isActive);

    QString setFilePath(QString path = QString());

private:
    void logToFile(QtMsgType type,
                   const QMessageLogContext &context,
                   const QString &msg);
    void logToStream(QtMsgType type,
                     const QMessageLogContext &context,
                     const QString &msg);
    QString message(const char* type,
                    const QMessageLogContext &context,
                    const QString &msg);

    void showDebugMessageBox(QtMsgType type,
                             const QMessageLogContext &context,
                             const QString &msg);

    void showReleaseMessageBox(QtMsgType type,
                               const QString &msg);

    QFile mFile;
    bool mIsFileLoggingActive = false;
    bool mIsStreamLoggingActive = false;
    bool mIsUILoggingActive = false;
    static const QHash <QtMsgType, QString> types;

};

#endif // COMMONMESSAGEHANDLER_H
