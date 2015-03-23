#include "ql-channel-serial.hpp"

QlChannelSerial::QlChannelSerial() : QlChannel() {
	port_ = new QSerialPort();
	open_ = false;
	params_ << "baud" << "bauds" << "bits" << "parity" << "stops" << "rts" << "cts" << "dtr" << "dsr" << "dcd" << "ri";
}

QlChannelSerial::QlChannelSerial(QObject* parent) : QlChannel(parent) {
	port_ = new QSerialPort();
	open_ = false;
	params_ << "baud" << "bauds" << "bits" << "parity" << "stops" << "rts" << "cts" << "dtr" << "dsr" << "dcd" << "ri";
}

QStringList QlChannelSerial::channels() {
	QList<QSerialPortInfo> pil = QSerialPortInfo::availablePorts();
	QStringList psl = QStringList();
	for (int i=0; i<pil.size(); i++){ psl.append( pil[i].portName() ); }
	return psl;
}

bool QlChannelSerial::open(const QString &name) {
	close();
	port_->setPortName(name);
	open_ = port_->open(QIODevice::ReadWrite);
	if (open_) name_ = name; else name_ = QString::fromLatin1("");
	return open_;
}

void QlChannelSerial::close() {
	if (isOpen()){
		port_->close();
		open_ = false;
	}
}

bool QlChannelSerial::isOpen() { return open_; }

QString QlChannelSerial::name() { return name_; }

QString QlChannelSerial::param(const QString &name) {
	if (!isOpen()) return QString::fromLatin1("");
	
	if (name == "baud") return QString::number(port_->baudRate());

	else if (name == "bits") return QString::number(port_->dataBits());

	else if (name == "parity") return QString::fromLatin1(
		(port_->parity() == QSerialPort::NoParity)    ? "n" :
		(port_->parity() == QSerialPort::EvenParity)  ? "e" :
		(port_->parity() == QSerialPort::OddParity)   ? "o" :
		(port_->parity() == QSerialPort::SpaceParity) ? "s" :
		(port_->parity() == QSerialPort::MarkParity)  ? "m" : "u"
		);

	else if (name == "stops") return QString::number(port_->stopBits());

	else if (name == "rts") return QString::number(port_->isRequestToSend());
	else if (name == "cts") return QString::number(!!(port_->pinoutSignals() & QSerialPort::ClearToSendSignal));
	else if (name == "dtr") return QString::number(port_->isDataTerminalReady());
	else if (name == "dsr") return QString::number(!!(port_->pinoutSignals() & QSerialPort::DataSetReadySignal));
	else if (name == "dcd") return QString::number(!!(port_->pinoutSignals() & QSerialPort::DataCarrierDetectSignal));
	else if (name == "ri")  return QString::number(!!(port_->pinoutSignals() & QSerialPort::RingIndicatorSignal));

	else return QString::fromLatin1("");
}

bool QlChannelSerial::paramSet(const QString &name, const QString &value) {
	if (!isOpen()) return false;
	
	if (name == "baud")        return port_->setBaudRate(value.toInt());
	else if (name == "bits")   return port_->setDataBits((QSerialPort::DataBits)value.toInt());
	else if (name == "parity") return port_->setParity(
		value.startsWith("n") ? QSerialPort::NoParity :
		value.startsWith("e") ? QSerialPort::EvenParity :
		value.startsWith("o") ? QSerialPort::OddParity :
		value.startsWith("s") ? QSerialPort::SpaceParity :
		value.startsWith("m") ? QSerialPort::MarkParity :
		QSerialPort::UnknownParity
	);
	else if (name == "stops") return port_->setStopBits((QSerialPort::StopBits)value.toInt());
	else if (name == "rts")   return port_->setRequestToSend((bool)value.toInt());
	else if (name == "dtr")   return port_->setDataTerminalReady((bool)value.toInt());
	else return false;
}

Q_INVOKABLE QList<int> QlChannelSerial::readBytes() {
	QList<int> *l = new QList<int>();
	if (isOpen() && port_->bytesAvailable()){
		QByteArray buf = port_->readAll();
		for (int i=0; i<buf.size(); i++) l->append(buf.at(i));
	}
	return *l;
}

Q_INVOKABLE bool QlChannelSerial::writeBytes(const QList<int> &l) {
	if (isOpen()){
		if (!l.size()) return true;
		out_->resize(l.size());
		for (int i=0; i<l.size(); i++) out_->data()[i] = l[i];
		return port_->write(*out_) > 0;
	}
	return false;
}

Q_INVOKABLE bool QlChannelSerial::writeString(const QString &s) {
	if (isOpen()){
		return port_->write(s.toLatin1()) > 0;
	}
	return false;
}

