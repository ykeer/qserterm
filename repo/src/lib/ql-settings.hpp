#ifndef QL_SETTINGS_H
	#define QL_SETTINGS_H

#include <QSettings>
#include <QString>
#include <QVariant>

class QlSettings : public QObject {
    Q_OBJECT

    public:
		QlSettings();
		QlSettings(QObject* parent);
		Q_INVOKABLE void open(const char *fn);
		Q_INVOKABLE void open(const QString &fn);
		Q_INVOKABLE void setValue(const QString &key, const QVariant &value);
		Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
		Q_INVOKABLE void sync();

	private:
		QSettings *settings_;
};

#endif

