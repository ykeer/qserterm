#ifndef QL_CHANNEL_SERIAL_H
    #define QL_CHANNEL__SERIAL_H

#include <QtSerialPort/QSerialPort>
#include "ql-channel.hpp"

class QlChannelSerial : public QlChannel {
    Q_OBJECT

    public:
        QlChannelSerial();
        QlChannelSerial(QObject* parent);

        Q_INVOKABLE virtual QStringList channels();

        Q_INVOKABLE virtual bool open(const QString &name);
        Q_INVOKABLE virtual void close();
        Q_INVOKABLE virtual bool isOpen();
        Q_INVOKABLE virtual QString name();

        Q_INVOKABLE virtual QString param(const QString &name);
        Q_INVOKABLE virtual bool paramSet(const QString &name, const QString &value);

        Q_INVOKABLE virtual QList<int> readBytes();
        Q_INVOKABLE virtual bool writeBytes(const QList<int> &b);

        Q_INVOKABLE virtual bool writeString(const QString &s);

	protected:
		QSerialPort *port_;
		bool open_;
};

#endif

