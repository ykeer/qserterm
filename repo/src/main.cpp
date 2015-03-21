#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "main.hpp"
#include "lib/ql-settings.hpp"
#include "lib/ql-files.hpp"
#include "lib/ql-channel-serial.hpp"

int main(int argc, char **argv){
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

	qmlRegisterType<QlSettings>("com.ql.settings", 1,0, "QlSettings");
	qmlRegisterType<QlFiles>("com.ql.files", 1,0, "QlFiles");
	qmlRegisterType<QlChannelSerial>("com.ql.channels", 1,0, "QlChannelSerial");

    engine.load(QUrl::fromLocalFile("main.qml"));

    return app.exec();
}

