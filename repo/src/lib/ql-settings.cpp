#include "ql-settings.hpp"

QlSettings::QlSettings() : QObject() { settings_ = nullptr; }
QlSettings::QlSettings(QObject* parent) : QObject(parent) { settings_ = nullptr; }

void     QlSettings::open(const char *fn){ settings_ = new QSettings(QString::fromUtf8(fn), QSettings::IniFormat); }
void     QlSettings::open(const QString &fn){ settings_ = new QSettings(fn, QSettings::IniFormat); }

void     QlSettings::setValue(const QString &key, const QVariant &value){ settings_->setValue(key, value); }

QVariant QlSettings::value(const QString &key, const QVariant &defaultValue) const { return settings_->value(key, defaultValue); }

void     QlSettings::sync(){ settings_->sync(); };

Q_DECLARE_METATYPE(QlSettings*)
