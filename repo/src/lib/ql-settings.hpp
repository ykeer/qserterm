#ifndef QL_SETTINGS_H
	#define QL_SETTINGS_H

#include <QSettings>
#include <QString>
#include <QVariant>

class QlSettings : public QObject {
    Q_OBJECT
		Q_PROPERTY(QString name READ name WRITE setName)

	public:
		QlSettings(QObject* parent = 0);
		virtual ~QlSettings();

		void setName(const QString &name);
		QString name();

		Q_INVOKABLE void open(const QString &fn);
		Q_INVOKABLE void setValue(const QString &key, const QVariant &value);
		Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
		Q_INVOKABLE void put(const QString &key, const QVariant &value);
		Q_INVOKABLE QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;
		Q_INVOKABLE void sync();

	private:
		QSettings *settings_;
		QString m_name;
		QHash<QString,QVariant> *m_cache;
};

#endif

