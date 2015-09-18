#include "ql-channel-socket.hpp"

QlChannelTcpSocket::QlChannelTcpSocket(QObject* parent) : QlChannel(parent) {
	socket_ = new QTcpSocket();
	open_ = false;
}

bool QlChannelTcpSocket::open(const QString &name) {
	close();
	QString host = name;
	int port = 9000;
	QStringList ns = name.split(":");
	if (ns.size() == 2){
		bool ok=false;
		port = ns[0].toInt(&ok, 10);
		if (!ok) port = 9000;
		else host = ns[0];
	}
	socket_->connectToHost(host, port);
	if (open_) name_ = name; else name_ = QString("");
	return open_;
}

void QlChannelTcpSocket::close() {
	if (isOpen()){
		socket_->disconnectFromHost();
		socket_->close();
		open_ = false;
	}
}

bool QlChannelTcpSocket::isOpen() { return open_; }

QString QlChannelTcpSocket::name() { return name_; }

QList<int> QlChannelTcpSocket::readBytes() {
	QList<int> *l = new QList<int>();
	if (isOpen() && socket_->bytesAvailable()){
		QByteArray buf = socket_->readAll();
		for (int i=0; i<buf.size(); i++) l->append(buf.at(i));
	}
	return *l;
}

bool QlChannelTcpSocket::writeBytes(const QList<int> &l) {
	if (isOpen()){
		if (!l.size()) return true;
		out_->resize(l.size());
		for (int i=0; i<l.size(); i++) out_->data()[i] = l[i];
		return socket_->write(*out_) > 0;
	}
	return false;
}

bool QlChannelTcpSocket::writeString(const QString &s) {
	if (isOpen()){
		return socket_->write(s.toLatin1()) > 0;
	}
	return false;
}




QlChannelUdpSocket::QlChannelUdpSocket(QObject* parent) : QlChannel(parent) {
	socket_ = new QUdpSocket();
	open_ = false;
}

bool QlChannelUdpSocket::open(const QString &name) {
	close();
	QString host = name;
	int port = 9000;
	QStringList ns = name.split(":");
	if (ns.size() == 2){
		bool ok=false;
		port = ns[0].toInt(&ok, 10);
		if (!ok) port = 9000;
		else host = ns[0];
	}
	socket_->connectToHost(host, port);
	if (open_) name_ = name; else name_ = QString("");
	return open_;
}

void QlChannelUdpSocket::close() {
	if (isOpen()){
		socket_->disconnectFromHost();
		socket_->close();
		open_ = false;
	}
}

bool QlChannelUdpSocket::isOpen() { return open_; }

QString QlChannelUdpSocket::name() { return name_; }

QList<int> QlChannelUdpSocket::readBytes() {
	QList<int> *l = new QList<int>();
	if (isOpen() && socket_->bytesAvailable()){
		QByteArray buf = socket_->readAll();
		for (int i=0; i<buf.size(); i++) l->append(buf.at(i));
	}
	return *l;
}

bool QlChannelUdpSocket::writeBytes(const QList<int> &l) {
	if (isOpen()){
		if (!l.size()) return true;
		out_->resize(l.size());
		for (int i=0; i<l.size(); i++) out_->data()[i] = l[i];
		return socket_->write(*out_) > 0;
	}
	return false;
}

bool QlChannelUdpSocket::writeString(const QString &s) {
	if (isOpen()){
		return socket_->write(s.toLatin1()) > 0;
	}
	return false;
}

