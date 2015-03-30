#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QString>
#include <QStringList>

#include "lib/ql-settings.hpp"
#include "lib/ql-files.hpp"
#include "lib/ql-channel-serial.hpp"
#include "lib/ql-server.hpp"

int main(int argc, char **argv){
	QApplication app(argc, argv);
	QQmlApplicationEngine engine;

	for (int i=0; i<argc; i++) QlFiles::argv_.append(argv[i]);

	qmlRegisterType<QlSettings>("com.ql.settings", 1,0, "QlSettings");
	qmlRegisterType<QlFiles>("com.ql.files", 1,0, "QlFiles");
	qmlRegisterType<QlChannelSerial>("com.ql.channels", 1,0, "QlChannelSerial");
	qmlRegisterType<QlServer>("com.ql.server", 1,0, "QlServer");

	engine.load(QUrl::fromLocalFile("main.qml"));

//QlServer server;
//server.listenPort(8080);

	int c = app.exec();

//server.close();

	return c;
}

