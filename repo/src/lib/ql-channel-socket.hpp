#ifndef QL_CHANNEL_SOCKET_H
    #define QL_CHANNEL__SOCKET_H

#include <QIODevice>
#include <QString>
#include <QDebug>
#include <QTcpSocket>
#include <QUdpSocket>
#include "ql-channel.hpp"

class QlChannelTcpSocket : public QlChannel {
    Q_OBJECT

    public:
        QlChannelTcpSocket(QObject* parent=0);

        Q_INVOKABLE virtual bool open(const QString &name);
        Q_INVOKABLE virtual void close();
        Q_INVOKABLE virtual bool isOpen();
        Q_INVOKABLE virtual QString name();

        Q_INVOKABLE virtual QList<int> readBytes();
        Q_INVOKABLE virtual bool writeBytes(const QList<int> &b);

        Q_INVOKABLE virtual bool writeString(const QString &s);

	protected:
		QTcpSocket *socket_;
		bool open_;
};

class QlChannelUdpSocket : public QlChannel {
    Q_OBJECT

    public:
        QlChannelUdpSocket(QObject* parent=0);

        Q_INVOKABLE virtual bool open(const QString &name);
        Q_INVOKABLE virtual void close();
        Q_INVOKABLE virtual bool isOpen();
        Q_INVOKABLE virtual QString name();

        Q_INVOKABLE virtual QList<int> readBytes();
        Q_INVOKABLE virtual bool writeBytes(const QList<int> &b);

        Q_INVOKABLE virtual bool writeString(const QString &s);

	protected:
		QUdpSocket *socket_;
		bool open_;
};

#endif

