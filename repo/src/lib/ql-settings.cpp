#include "ql-settings.hpp"
#include <QtDebug>

QlSettings::QlSettings(QObject* parent) : QObject(parent) { settings_ = nullptr; m_cache = new QHash<QString,QVariant>; }

QlSettings::~QlSettings(){ sync(); }

void QlSettings::open(const QString &fn){ settings_ = new QSettings(fn, QSettings::IniFormat); }

void QlSettings::setValue(const QString &key, const QVariant &value){ put(key, value); }
QVariant QlSettings::value(const QString &key, const QVariant &defaultValue) const { return get(key, defaultValue); }

void QlSettings::put(const QString &key, const QVariant &value){ m_cache->insert(key,value); settings_->setValue(key, value); }
QVariant QlSettings::get(const QString &key, const QVariant &defaultValue) const {
	if (m_cache->contains(key)) return m_cache->value(key);
	return settings_->value(key, defaultValue);
}

void QlSettings::sync(){
	settings_->sync();
	qDebug() << "Saving config" << m_name;
};

void QlSettings::setName(const QString &name){
	m_name = name;
	open(m_name);
	qDebug() << "Loading config" << m_name;
}
QString QlSettings::name(){ return m_name; }

