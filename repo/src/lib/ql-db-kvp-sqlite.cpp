#include "ql-db-kvp-sqlite.hpp"
#include <QDebug>

QlDbKvpSqlite::QlDbKvpSqlite( QObject* parent ) : QObject( parent ) {
	db_ = QSqlDatabase::addDatabase("QSQLITE");
	m_name = QString("none");
	m_db = QString("kvp");
	m_cache = new QHash<QString,QString>;
	m_cached = false;
}

QlDbKvpSqlite::~QlDbKvpSqlite(){
	qDebug() << "Key-Value closing" << m_db << "at" << m_name;
	db_.close();
}

bool QlDbKvpSqlite::openDb(const QString &name){
	db_.setDatabaseName(name);
	db_.open();

	db_.exec("CREATE TABLE IF NOT EXISTS " + m_db + "(k TEXT PRIMARY KEY, v TEXT);");

	return db_.isOpen() && db_.isValid();
}

QString QlDbKvpSqlite::name(){ return m_name; }
void QlDbKvpSqlite::setName(const QString &name){
	int i = name.lastIndexOf("::");
	if (i > 0){
		m_name = name.left(i);
		m_db = name.mid(i+2,-1);
	} else m_name = name;
	qDebug() << "Key-Value opening" << m_db << "at" << m_name;
	openDb(m_name);
}

bool QlDbKvpSqlite::cached(){ return m_cached; }
void QlDbKvpSqlite::setCached(const bool cached){ m_cached = cached; }

bool QlDbKvpSqlite::put(const QString &k, const QString &v){
	if (m_cached) m_cache->insert(k,v); // update cache
	QSqlQuery query(("INSERT OR REPLACE INTO " + m_db + "(k, v) VALUES (?, ?);"), db_);
	query.bindValue(0, k); query.bindValue(1, v);
	return query.exec();
}

QString QlDbKvpSqlite::get(const QString &k, const QString &def){
	if (m_cached && m_cache->contains(k)){ /*qDebug() << "Cache hit at" << k;*/ return m_cache->value(k); } // cache hit
	QSqlQuery query(("SELECT v FROM " + m_db + " WHERE k = ? ;"), db_);
	query.bindValue(0, k);
	QString result = def;
	if (query.exec() && query.first()){
		result = query.value(0).toString();
		if (m_cached){ /*qDebug() << "Cache miss at" << k;*/ m_cache->insert(k,result); } // cache miss, update
	}
	query.finish();
	return result;
}

bool QlDbKvpSqlite::clear(){ db_.exec("DELETE FROM " + m_db + ";").finish(); return true; }

QString QlDbKvpSqlite::lastError(){ return db_.lastError().databaseText() + " / " + db_.lastError().driverText(); }

