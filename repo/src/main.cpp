#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include <QStringList>
#include <QTranslator>

#include "lib/ql-settings.hpp"
#include "lib/ql-files.hpp"
#include "lib/ql-channel-serial.hpp"
#include "lib/ql-channel-socket.hpp"
#include "lib/ql-server.hpp"
#include "lib/ql-lineplot.hpp"
#include "lib/ql-locale.hpp"
#include "lib/ql-db-kvp-sqlite.hpp"

int main(int argc, char **argv){
	QApplication app(argc, argv);
	QQmlApplicationEngine engine;

	// install translator, load available/current translations
	QlLocale::l_init(&app, "./languages", "en_EN");
	qmlRegisterType<QlLocale>("QlLocale", 1,0, "QlLocale");

	// register types
	qmlRegisterType<QlSettings>("QlSettings", 1,0, "QlSettings");

	// initialize ARGV
	QlFiles::argvSet(&app);
	qmlRegisterType<QlFiles>("QlFiles", 1,0, "QlFiles");

	qmlRegisterType<QlServer>("QlServer", 1,0, "QlServer");

	qmlRegisterType<QlChannelSerial>("QlChannelSerial", 1,0, "QlChannelSerial");
	qmlRegisterType<QlChannelTcpSocket>("QlChannelTcpSocket", 1,0, "QlChannelTcpSocket");
	qmlRegisterType<QlChannelUdpSocket>("QlChannelUdpSocket", 1,0, "QlChannelUdpSocket");

	qmlRegisterType<QlLinePlotItem>("QlLinePlotItem", 1,0, "QlLinePlotItem");

	qmlRegisterType<QlDbKvpSqlite>("QlDbKvpSqlite", 1,0, "QlDbKvpSqlite");

	// load main QML file
	engine.load(QUrl::fromLocalFile("main.qml"));

	// execute
	int c = app.exec();

	return c;
}

