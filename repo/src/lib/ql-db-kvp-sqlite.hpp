#ifndef QL_DB_KVP_SQLITE
	#define QL_DB_KVP_SQLITE

#include <QtSql/QtSql>

class QlDbKvpSqlite : public QObject {
	Q_OBJECT
		// file name
		Q_PROPERTY(QString name READ name WRITE setName)

		// cached
		Q_PROPERTY(bool cached READ cached WRITE setCached)

	public:
		QlDbKvpSqlite( QObject* parent = 0 );
		virtual ~QlDbKvpSqlite();

		// open database
		Q_INVOKABLE bool openDb(const QString &name);

		QString name();
		void setName(const QString &name);

		bool cached();
		void setCached(const bool cached);
		
		// put
		Q_INVOKABLE bool put(const QString &k, const QString &v);

		// get
		Q_INVOKABLE QString get(const QString &k, const QString &def=QString(""));

		// clear all data
		Q_INVOKABLE bool clear();

		// get last error
		Q_INVOKABLE QString lastError();

	private:
		QSqlDatabase db_;
		QString m_name;
		QString m_db;
		QHash<QString,QString> *m_cache;
		bool m_cached;
};

#endif

