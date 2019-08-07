#ifndef COMMONMESSAGEHANDLER_H
#define COMMONMESSAGEHANDLER_H

#include <QFile>
#include <QGlobalStatic>
#include <QObject>
#include <QHash>

class CommonMessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit CommonMessageHandler(QObject *parent = nullptr);
    virtual ~CommonMessageHandler() override;
    static CommonMessageHandler& instance();

    static void customMessageHandlerFunction(QtMsgType type,
                                             const QMessageLogContext &context,
                                             const QString &msg);

private:
    QString getMessage(const char* type,
                              const QMessageLogContext &context,
                              const QString &msg);

    void log(QtMsgType type, QString msg);

    void showDebugMessageBox(QtMsgType type,
                                    const QMessageLogContext &context,
                                    const QString &msg);

    void showReleaseMessageBox(QtMsgType type,
                                      const QString &msg);

    QFile mFile;
    static const QHash <QtMsgType, QString> types;

};

#endif // COMMONMESSAGEHANDLER_H
